#pragma once

#include <QTextOption>
#include "base_widget.h"


class CDyncEventData;
class CPushBtnWidget : public CBaseWidget
{
	Q_OBJECT

public:
	CPushBtnWidget();
	virtual ~CPushBtnWidget();

public:
	enum BTN_TYPE
	{
		NORMAL = 0,        //!< 一般
		BTN_3D1,           //3d按钮1
		BTN_3D2,
		BTN_3D3,
		BTN_3D4,
		EMGRCY_STOP_A,              //!< 急停A
		EMGRCY_STOP_B,            //!< 急停B
		EMGRCY_STOP_C,            //!< 急停C
		YELLOW_LIGHT,        //!< 黄灯
		BLUE_LIGHT,              //!< 蓝灯
		GREEN_LIGHT,     //绿灯
		RED_NIGHT, //红灯
		YELLOW_LED,                //!< 黄LED
		BLUE_LED,             //!< 蓝LED
		GREEN_LED,               //绿LED
		RED_LED,                //红LED
	};

	enum BTN_OPERATOR
	{
		ACTION_CLICK = 0, //!< 单击
		ACTION_PRESSED,   //!< 按下
		ACTION_RELEASE,   //!< 释放
		ACTION_FOCUSED,   //!< 激活
		ACTION_UNFOCUSED,
		MAX_ACTION_NUM

	};

	//命令执行类型
	enum BTN_EXEC_TYPE
	{
		//执行命令
		BTN_EXEC_ORDER = 0,
		//on-off
		BTN_ON_OFF,
		//脉冲
		BTN_PLUS,
	};

	//遥控预制  和  执行
	enum BTN_REMOTE_CONTROL
	{
		//预制
		BTN_PRESET = 0,
		//执行
		BTN_EXEC = 1,
	};

public:

	CPushBtnWidget(QRectF rcPos, CPushBtnWidget::BTN_TYPE shape);
public:
	virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	virtual QRectF boundingRect() const;

	// 旋转的中心点 
	virtual QPointF GetRotationCenterPoint()const;

	virtual void MoveHandleTo(int nHandle, const QPointF &point);
	// 	virtual void DrawSingleSelectedTracker(QPainter *pPainter);
	// 	virtual void DrawMultiSelectedTracker(QPainter *pPainter, bool bFirstSelected);
	// 移动中的图元
	virtual void DrawMovingWidget(QPainter *pPainter, bool bSingleSelected = false);

	virtual void GetPath(QPainterPath& path);

	virtual void GetMovingPath(QPainterPath& path);

	virtual void FillPath(QPainter *pPainter, QPainterPath &p, CBrushInfo &info);


	//基本按钮path
	void GetNormalPath(QPainterPath& path);
	//基本按钮补充path
	void GetNormalAddPath(QPainterPath& path);
	//基本按钮补充path press
	void GetNormalRressAddPath(QPainterPath& path);
	//基本按钮移动path
	void GetNormalMovingPath(QPainterPath& path);

	//3d1path
	void Get3D1Path(QPainterPath& path);
	//3d1  movingpath
	void Get3D1MovingPath(QPainterPath& path);

	//3d2path
	void Get3D2Path(QPainterPath& path);
	//3d2  movingpath
	void Get3D2MovingPath(QPainterPath& path);

	//3d3path
	void Get3D3Path(QPainterPath& path);
	//3d1  movingpath
	void Get3D3MovingPath(QPainterPath& path);

	//3d4path
	void Get3D4Path(QPainterPath& path);
	//3d4  movingpath
	void Get3D4MovingPath(QPainterPath& path);

	//设置字体位置
	void SetFontLayout(QTextOption &txtOpt);

	//获取命令信息
	const std::vector<CDyncEventData*>&  GetEventIntent()
	{
		return m_arrEventIntent;
	}

	//获取命令
	CDyncEventData* GetEventActionData(BTN_OPERATOR eBtnOper);
public: 
	virtual bool SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);
	//保存按钮执行属性
	void SaveBtnExecXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter);
	//保存order
	void SaveOrderXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter, CDyncEventData* pOrder);
	//加载数据

	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader> pXmlReader);
	//创建单个事件
	void LoadOrderInfo(std::shared_ptr<QXmlStreamReader> pXmlReader, CDyncEventData* pOrder);


	void SetBtnBindData(const QString &strValue)
	{
		m_strBtnBindValue = strValue;
	}

	const QString &GetBtnBindData()
	{
		return m_strBtnBindValue;
	}

	int GeteExecType()
	{
		return m_eExecType;
	}

	void SetExecType(BTN_EXEC_TYPE nType)
	{
		m_eExecType = nType;
	}

	int GetRemoteControlType()
	{
		return m_eRemoteControyType;
	}

	void SetRemoteControl(BTN_REMOTE_CONTROL nType)
	{
		m_eRemoteControyType = nType;
	}

private:
	BTN_TYPE m_Shape;
	//命令状态变量
	QString m_strBtnBindValue;
	//命令类型
	BTN_EXEC_TYPE m_eExecType;
	//预置或执行
	BTN_REMOTE_CONTROL m_eRemoteControyType;
	//命令
	std::vector<CDyncEventData*> m_arrEventIntent;
};
