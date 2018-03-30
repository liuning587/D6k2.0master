#ifndef ANALYSEPOINTBIN_H
#define ANALYSEPOINTBIN_H

#include <QObject>
#include <QMap>

#include "remoteponittableanalyse.h"

class CAnalysePointBin : public QObject
{
    Q_OBJECT

public:
    CAnalysePointBin(QObject *parent=0);
    void InitData(const QString &strFileName);
    //解析数据
    //void AnalyseBinData(const QByteArray & byData);
    //const QByteArray & GetBtData();
    ~CAnalysePointBin();

	const QMap<int, QList<int>> GetAnaloglst()
	{
		return m_lstAnalog;
	}

	const QMap<int, QList<int>> GetBinarylst()
	{
		return m_lstBinary;
	}

	const QMap<int, QList<int>> GetDoubleBinarylst()
	{
		return m_lstDoubleBinary;
	}

	const QMap<int, QList<int>> GetKwhlst()
	{
		return m_lstKwh;
	}

	const QMap<int, QList<int>> GetControllst()
	{
		return m_lstControl;
	}

	const QMap<int, QMap<int, RPT> > &GetDoorDatas()
	{
		return m_mapAnalogChooseInfo;
	}

	//
	const QList<int> GetAnalogTableSort()
	{
		return m_lstTableRemote;
	}
	//
	const QList<int> GetBinaryTableSort()
	{
		return m_lstBinaryTable;
	}
	//
	const QList<int> GetDoubleBinaryTableSort()
	{
		return m_lstDoubleBinaryTable;
	}
	//
	const QList<int> GetControlTableSort()
	{
		return m_lstRemoteControlTable;
	}
	//
	const QList<int> GetKwhTableSort()
	{
		return m_lstKwhTable;
	}

    
private:
    //需要测的点   类型标识  该类型下需要测的点
   // QMap<unsigned int, QList<int>> m_mapNeedpoints;
	//遥测
	QMap<int, QList<int>> m_lstAnalog;
	//遥测点表顺序
	QList<int> m_lstTableRemote;

	//遥信
	QMap<int, QList<int>> m_lstBinary;
	//遥信点表顺序
	QList<int> m_lstBinaryTable;

	//双点遥信
	QMap<int, QList<int>> m_lstDoubleBinary;
	//双点遥信表顺序
	QList<int> m_lstDoubleBinaryTable;
	//遥控
	QMap<int, QList<int>> m_lstControl;
	//遥控表顺序
	QList<int> m_lstRemoteControlTable;
	//遥脉
	QMap<int, QList<int>> m_lstKwh;
	//遥脉表顺序
	QList<int>m_lstKwhTable;

	// 遥测 选点 门槛等信息
	QMap<int, QMap<int, RPT> > m_mapAnalogChooseInfo;

};

#endif // ANALYSEPOINTBIN_H
