/*! @file mem_db.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  mem_db.cpp
文件实现功能 :  内存数据库基类
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   内存数据库基类
*  @author  LiJin
*  @version 1.0
*  @date    2016.11.2
*******************************************************************************/
#include "mem_db.h"

#include "fesapi/fesdatadef.h"
#include "scadaapi.h"

#include "log/log.h"
#include <QObject> 
#include <QString> 
#include <QDebug>


CMemDB::CMemDB() :m_bStopFlag(false),m_pScada(nullptr)
{ 
	 
}

CMemDB::CMemDB(CScadaApi *pScada) : m_bStopFlag(false), m_pScada(pScada)
{
	Q_ASSERT(pScada);
}

CMemDB::~CMemDB(void)
{ 

}
/*! \fn bool CMemDB::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CMemDB::Initialize 
** \details 初始化  加载proj文件，分别接线各个配置文件
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CMemDB::Initialize(const char *pszDataPath, unsigned int nMode)
{ 
	QString szLog;
	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}
	
	LogMsg(szLog.toStdString().c_str(), 0);

	return true;
}

void CMemDB::Run()
{

}

void CMemDB::Shutdown()
{


}

void CMemDB::SetTaskStopFlag()
{
	std::unique_lock<std::mutex> lk(m_mutThread);
	m_bStopFlag = true;
	m_cvThread.notify_one();
}

void CMemDB::StartModule()
{
	if (m_pMainTask != nullptr)
	{
		m_pMainTask.reset(new (std::thread)(&CMemDB::MainTask, this, nullptr));
		//m_pMainTask = std::make_shared<std::thread>(*m_p103Impl);
	}
	else
	{
		m_pMainTask = std::make_shared<std::thread>(&CMemDB::MainTask, this, nullptr);
	}
}

void CMemDB::StopModule()
{
	SetTaskStopFlag();
	// 退出线程
	Q_ASSERT(m_pMainTask != nullptr);
	if (m_pMainTask != nullptr && m_pMainTask->joinable())
	{
		m_pMainTask->join();
	}
}

void CMemDB::MainTask(void *pImpl)
{
//	LogString("start main task....", 2);
	qDebug() << "start main task....";
	while (true)
	{
		std::unique_lock<std::mutex> lk(m_mutThread);

		auto bRet = m_cvThread.wait_for(lk, std::chrono::seconds(1), std::bind(&CMemDB::IsStopped, this));
		if (m_bStopFlag)
		{
			qDebug() << "exit main task....";
			//			LogString("exit main task1....", 2);
			return;
		}
 
	//	MainLoop();
	}
} 
/*! \fn bool CMemDB::GetAinValue(int32u nOccNo, CVariantEx & val, int8u &nQuality)
********************************************************************************************************* 
** \brief CMemDB::GetAinValue 
** \details 应用层获取模拟量测值等信息，供内部模块（网络总线等等）
** \param nOccNo 
** \param val 
** \param nQuality 
** \return bool 
** \author LiJin 
** \date 2016年11月2日 
** \note 
********************************************************************************************************/
bool CMemDB::GetAinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const
{
	Q_ASSERT(false);

	return true;
}

bool CMemDB::GetDinValue(int32u nOccNo, CVariant & val, int8u &nQuality)const
{
	Q_ASSERT(false);

	return true;
}

NODE_STATE CMemDB::GetMyHostState() const
{
	Q_ASSERT(m_pScada);
	if (m_pScada == nullptr)
	{
		return STATE_UNKOWN;
	}

	return m_pScada->GetNodeHostState(m_nMyNodeOccNo);
}
/*! \fn bool  CMemDB::GetOccNoByTagName(const char*pszTagName, int32u &nIddType, int32u &nOccNo, int32u &nFiledID)const
********************************************************************************************************* 
** \brief CMemDB::GetOccNoByTagName 
** \details 根据tagname获取occno
** \param pszTagName 
** \param nIddType 
** \param nOccNo 
** \param nFiledID 
** \return bool 
** \author LiJin 
** \date 2017年9月14日 
** \note 
********************************************************************************************************/
bool  CMemDB::GetOccNoByTagName(const char*pszTagName, int32u &nIddType, int32u &nOccNo, int32u &nFiledID)const
{
	Q_ASSERT(pszTagName && strlen(pszTagName) > 0 && strlen(pszTagName)<= MAX_NAME_LENGTH);
	if (pszTagName == nullptr || strlen(pszTagName) == 0 || strlen(pszTagName) > MAX_NAME_LENGTH)
	{
		return false;
	}
	Q_ASSERT(m_pScada);
	if (m_pScada == nullptr)
	{
		return false;
	}
	nFiledID = 0;
	return m_pScada->GetOccNoByTagName(pszTagName, m_nMyNodeOccNo, nIddType, nOccNo);
}

/** @}*/
