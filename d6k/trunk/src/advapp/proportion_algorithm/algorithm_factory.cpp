
#include "algorithm_factory.h"

CAlgorithmFactory::CAlgorithmFactory()
{
	m_isInit = false;

	m_pTime_4h = nullptr;
	m_pTime_72h = nullptr;
	m_pThread = nullptr;
}

CAlgorithmFactory::~CAlgorithmFactory()
{
	Shutdown();
}

CAlgorithmFactory& CAlgorithmFactory::GetInstance()
{
	static CAlgorithmFactory instance;

	return instance;
}

bool CAlgorithmFactory::Init(const std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo)
{
	if (m_isInit)
	{
		return true;
	}

	m_vecStationInfo = vecStationInfo;

	if (m_vecStationInfo.size() <= 0)
	{
		// log
		return false;
	}

	for (auto var : m_vecStationInfo)
	{
		std::shared_ptr<CProportionAlgorithm> pProPortionAlgorm = std::make_shared<CProportionAlgorithm>();

		pProPortionAlgorm->Init(var);

		m_vecProPortionAlgorm.push_back(pProPortionAlgorm);
	}

	m_pThread = new QThread();

	// 超短期预测定时器初始化，只启动1次，槽函数中再进行start。（为了提高时间精确性）
	m_pTime_4h = new QTimer(this);
	m_pTime_4h->setSingleShot(true);	
	m_pTime_4h->moveToThread(m_pThread);//更改定时器运行的线程 
	connect(m_pTime_4h, SIGNAL(timeout()), this, SLOT(Slot_Timeout_Predict4h()), Qt::DirectConnection);
	// 连接定时器槽,用来停止定时器.(阻塞模式)
	connect(this, SIGNAL(Signal_StopSignal()), m_pTime_4h, SLOT(stop()), Qt::BlockingQueuedConnection);

	m_pTime_72h = new QTimer(this);
	m_pTime_72h->setSingleShot(true);
	m_pTime_72h->moveToThread(m_pThread);//更改定时器运行的线程 
	connect(m_pTime_72h, SIGNAL(timeout()), this, SLOT(Slot_Timeout_Predict72h()), Qt::DirectConnection);
	// 连接定时器槽,用来停止定时器.(阻塞模式)
	connect(this, SIGNAL(Signal_StopSignal()), m_pTime_72h, SLOT(stop()), Qt::BlockingQueuedConnection);
	
	m_isInit = true;

	return true;
}

void CAlgorithmFactory::Run()
{
	if (!m_isInit)
	{
		// log
		return;
	}

	RunTimer4h();

	RunTimer72h();
}

void CAlgorithmFactory::Shutdown()
{
	if (!m_isInit)
	{
		return;
	}

	StopTimer();

	if (m_pThread != nullptr)
	{
		m_pThread->quit();
		m_pThread->wait();

		if (m_pTime_4h != nullptr)
		{
			delete m_pTime_4h;
			m_pTime_4h = nullptr;
		}

		if (m_pTime_72h != nullptr)
		{
			delete m_pTime_72h;
			m_pTime_72h = nullptr;
		}

		delete m_pThread;
		m_pThread = nullptr;
	}

	m_isInit = false;
}

void CAlgorithmFactory::RunTimer4h()
{
	if (m_vecProPortionAlgorm.size() <= 0)
	{
		// log
		return;
	}

	for (auto var : m_vecProPortionAlgorm)
	{
		QDateTime currTime = QDateTime::currentDateTime();
		var->PowerPredict4h(currTime);
	}
	
	QTime now = QTime::currentTime();
	int tick = now.hour() * 3600 + now.minute() * 60 + now.second();

	// 让下次预测基准时间变位1分钟整点预测
	m_pTime_4h->start((60 - tick % 60) * 1000);

	return;
}

void CAlgorithmFactory::RunTimer72h()
{
	if (m_vecProPortionAlgorm.size() <= 0)
	{
		// log
		return;
	}

	for (auto var : m_vecProPortionAlgorm)
	{
		QDateTime currTime = QDateTime::currentDateTime();
		var->PowerPredict72h(currTime);
	}

	QTime now = QTime::currentTime();
	int tick = now.hour() * 3600 + now.minute() * 60 + now.second();

	// 让下次预测基准时间变位1分钟整点预测
	m_pTime_72h->start((60 - tick % 60) * 1000);

	return;
}

void CAlgorithmFactory::StopTimer()
{
	emit Signal_StopSignal();
}

bool CAlgorithmFactory::IsPredictTime4h(const QTime &time)
{
	// 15的倍数过后1分钟开始预测。
	if ((time.minute() % m_usInterval4h) == m_usBaseTime4h)
	{
		return true;
	}

	return false;
}

bool CAlgorithmFactory::IsPredictTime72h(const QTime &time)
{
	// 15的倍数过后1分钟开始预测。
	if ((time.minute() % m_usBaseTime72h) == m_usInterval72h)
	{
		return true;
	}

	return false;
}

void CAlgorithmFactory::Slot_Timeout_Predict4h()
{
	QTime now = QTime::currentTime();

	if (IsPredictTime4h(now))
	{
		for (auto var : m_vecProPortionAlgorm)
		{
			QDateTime currTime = QDateTime::currentDateTime();
			var->PowerPredict72h(currTime);
		}
	}

	int tick = now.hour() * 3600 + now.minute() * 60 + now.second();

	// 让下次预测基准时间变位1分钟整点预测
	m_pTime_4h->start((60 - tick % 60) * 1000);

	return;
}

void CAlgorithmFactory::Slot_Timeout_Predict72h()
{
	QTime now = QTime::currentTime();

	if (IsPredictTime72h(now))
	{
		for (auto var : m_vecProPortionAlgorm)
		{
			QDateTime currTime = QDateTime::currentDateTime();
			var->PowerPredict72h(currTime);
		}
	}

	int tick = now.hour() * 3600 + now.minute() * 60 + now.second();

	// 让下次预测基准时间变位1分钟整点预测
	m_pTime_72h->start((60 - tick % 60) * 1000);

	return;
}

