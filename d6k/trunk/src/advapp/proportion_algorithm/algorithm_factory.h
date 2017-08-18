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
	// 运行超短期预测
	void RunTimer4h();

	// 运行短期预测
	void RunTimer72h();

private:

	// 停止定时器
	void StopTimer();

	// 是否到达超短期算法启动时间
	bool IsPredictTime4h(const QTime &time);

	// 是否到达短期算法启动时间
	bool IsPredictTime72h(const QTime &time);

public slots:
	// 超短期预测定时处理
	void Slot_Timeout_Predict4h();

	// 短期预测定时处理
	void Slot_Timeout_Predict72h();

signals:
	// 停止定时器信号
	void Signal_StopSignal();

private:
	QTimer* m_pTime_4h;		// 超短期预测定时器
	QTimer* m_pTime_72h;	// 短期预测定时器

	QThread* m_pThread;

	// 多场站数据
	std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo;

	// 线性算法(多个场站的集合)
	std::vector<std::shared_ptr<CProportionAlgorithm>> m_vecProPortionAlgorm;

	std::atomic<bool> m_isInit;

	// 基准时间，启动算法的时间点 (分钟)。15的倍数过后1分钟开始预测。
	const static quint16 m_usBaseTime4h = 1;
	// 超短期算法启动的间隔
	const static quint16 m_usInterval4h = 15;

	// 基准时间，启动算法的时间点 (分钟)。15的倍数过后1分钟开始预测。
	const static quint16 m_usBaseTime72h = 1;
	// 短期算法启动的间隔
	const static quint16 m_usInterval72h = 15;
};

#endif // ALGORITHM_FACTORY_H
