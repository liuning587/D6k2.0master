#ifndef REALTIMEBINARY_H
#define REALTIMEBINARY_H

#include <QWidget>
#include <QMap>

class CCommThread;
class QTableView;
class QStandardItemModel;
class QStandardItem;
class CPointInfo;
class CInfoConfigWgt;

class CRealTimeBinary : public QWidget
{
	Q_OBJECT

public:
    CRealTimeBinary(QWidget *parent, CCommThread *pCommunicate, CPointInfo *pPointInfo, CInfoConfigWgt *pConfgWgt);
	~CRealTimeBinary();
	void AnalyseRealBinaryData(int iDeviceID, int nPointID, int nValue, const QString &strType);
	void InitWidget();
	//初始化数据
	void InitData();

	enum
	{
		eSignal = 1,
		eDouble = 2,
		eSignalGroup = 3,
	};
public slots:
    void Slot_AnalyseoneBinaryData(int iDeviceID, int nPointID, int nValue);
    void Slot_AnalyseDoubleBinaryData(int iDeviceID, int nPointID, int nValue);

	void Slot_AnalyseGroupBinaryData(int iDeviceID, int nPointID, int nValue);

private:
	//通信插件
	CCommThread *m_pCommunicate;
	//id item
	QMap<int, QStandardItem*> m_mapIdItemObj;
	//双点  item  id
	QMap<int, QStandardItem*> m_mapDoubleIdItemObj;
	//tableview
	QTableView *m_pTableView;
	//双点遥信
	QTableView *m_pDoubleTableView;
	//双点
	QStandardItemModel *m_pDoubleTableModel;
	//model
	QStandardItemModel *m_pTableModel;
	//当前个数
	int m_iCurrentPointsNum;
	//当前double个数
	int m_nICurrentDoubelPointNum;
	//点表数据
	CPointInfo *m_pPointInfo;
    //
    CInfoConfigWgt *m_pConfgWgt;

};

#endif // REALTIMEBINARY_H
