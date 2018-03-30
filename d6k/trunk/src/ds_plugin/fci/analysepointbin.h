#ifndef ANALYSEPOINTBIN_H
#define ANALYSEPOINTBIN_H

#include <QObject>
#include <QMap>
#include "datadefine.h"
class CAnalysePointBin : public QObject
{
    Q_OBJECT

public:
    CAnalysePointBin(QObject *parent=0);
    void InitData(const QString &strFileName);
    //解析数据
    ~CAnalysePointBin();

	const QList<int> GetAnaloglst()
	{
		return m_lstAnalog;
	}

	const QList<int> GetBinarylst()
	{
		return m_lstBinary;
	}

	const QList<int> GetDoubleBinarylst()
	{
		return m_lstDoubleBinary;
	}

	const QList<int> GetKwhlst()
	{
		return m_lstKwh;
	}

	const QList<int> GetControllst()
	{
		return m_lstControl;
	}

	const QMap<int, RPT> &GetDoorDatas()
	{
		return m_mapAnalogChooseInfo;
	}
    
private:
    //需要测的点   类型标识  该类型下需要测的点
   // QMap<unsigned int, QList<int>> m_mapNeedpoints;
	//遥测
	QList<int> m_lstAnalog;
	//遥信
	QList<int> m_lstBinary;
	//双点遥信
	QList<int> m_lstDoubleBinary;
	//遥控
	QList<int> m_lstControl;
	//遥脉
	QList<int> m_lstKwh;
	// 遥测 选点 门槛等信息
	QMap<int, RPT> m_mapAnalogChooseInfo;

};

#endif // ANALYSEPOINTBIN_H
