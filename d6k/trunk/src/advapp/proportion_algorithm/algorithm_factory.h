#pragma once

#ifndef ALGORITHM_FACTORY_H
#define ALGORITHM_FACTORY_H

#include <QObject>
#include <QTimer>
#include <QThread>

#include "proportion_algorithm.h"

class CAlgorithmFactory : public QObject
{
	Q_OBJECT

public:

	static CAlgorithmFactory& GetInstance();

	bool Init(const std::vector<std::shared_ptr<CStationInfo>>& vecStationInfo);

	void Run();

	void Shutdown();

private:
	explicit CAlgorithmFactory();
	~CAlgorithmFactory();

private:
	// ���г�����Ԥ��
	void RunTimer4h();

	// ���ж���Ԥ��
	void RunTimer72h();

private:

	// ֹͣ��ʱ��
	void StopTimer();

	// �Ƿ񵽴ﳬ�����㷨����ʱ��
	bool IsPredictTime4h(const QTime &time);

	// �Ƿ񵽴�����㷨����ʱ��
	bool IsPredictTime72h(const QTime &time);

public slots:
	// ������Ԥ�ⶨʱ����
	void Slot_Timeout_Predict4h();

	// ����Ԥ�ⶨʱ����
	void Slot_Timeout_Predict72h();

signals:
	// ֹͣ��ʱ���ź�
	void Signal_StopSignal();

private:
	QTimer* m_pTime_4h;		// ������Ԥ�ⶨʱ��
	QTimer* m_pTime_72h;	// ����Ԥ�ⶨʱ��

	QThread* m_pThread;

	// �ೡվ����
	std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo;

	// �����㷨(�����վ�ļ���)
	std::vector<std::shared_ptr<CProportionAlgorithm>> m_vecProPortionAlgorm;

	std::atomic<bool> m_isInit;

	// ��׼ʱ�䣬�����㷨��ʱ��� (����)��15�ı�������1���ӿ�ʼԤ�⡣
	const static quint16 m_usBaseTime4h = 1;
	// �������㷨�����ļ��
	const static quint16 m_usInterval4h = 15;

	// ��׼ʱ�䣬�����㷨��ʱ��� (����)��15�ı�������1���ӿ�ʼԤ�⡣
	const static quint16 m_usBaseTime72h = 1;
	// �����㷨�����ļ��
	const static quint16 m_usInterval72h = 15;
};

#endif // ALGORITHM_FACTORY_H
