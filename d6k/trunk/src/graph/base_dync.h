/*! @file base_dync.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  base_dync.h
�ļ�ʵ�ֹ��� :  �����Ķ�̬����
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����Ķ�̬����
*  @author  LiJin
*  @version 1.0
*  @date    2014.09.11
*******************************************************************************/
#ifndef BASE_DYNC_DATA_H
#define BASE_DYNC_DATA_H
 
#include <string>
#include <memory>

class QXmlStreamWriter;
class QXmlStreamReader;

struct TAGITEM;

class CVariant;

/*! \class CBaseDyncData  base_dync.h  "base_dync.h"
*  \brief ��̬���Ի���.
*/
class CBaseWidget;
class CBaseDyncData  
{
public:
	CBaseDyncData();
	virtual ~CBaseDyncData();
public:
public:
	enum  DYNC_TYPE
	{
		DYNC_NULL = 0,
		DYNC_VALUE,          //!< ֵ��ʾ
		DYNC_STRING,         //!< �ַ�����ʾ
		DYNC_PEN_COLOR,          //!< pen��ɫ
		DYNC_TEXT_COLOR,        //text��ɫ
		DYNC_VISIBLE,        //!< �ɼ�
		DYNC_HOR_MOVE,           //!ˮƽ�ƶ�
		DYNC_VIR_MOVE,           //��ֱ�ƶ� 
		DYNC_GROUP_MOVE,         //����ƶ�
		DYNC_SIZE,           //!< ��С�仯
		DYNC_ROTATION,       //!< ��ת
		DYNC_FLIP,           //!< ��ת
		DYNC_TRANSPARENCY,   //!< ͸��
		DYNC_FILLING,        //!< ���
		DYNC_SYMBOL,
		DYNC_DRAGGING,       //!< ����ʱͼԪ��λ���ƶ����ߴ�С�仯
		DYNC_GRID,           //!< ��̬����
		DYNC_EVENT,          //!< ��̬�¼���������굥�������£��ͷţ����ȡ������
		DYNC_MAP,            //ͼ��
		//	LineColor,FillColor,TextColor,Visible,Flicker,Rotate,Animation,
		//	SetValue,SetState,Graph,Operate,Logic,Menu,Ctrl
	};

     const CBaseDyncData& operator=(const CBaseDyncData& src);

	//��������
	 virtual void CopyBaseData(CBaseDyncData *src);
public:
	//! �Ƿ����� Ҳ������̬�ˣ����ǲ�������
	bool m_bEnable;
	//! ��̬��������
	DYNC_TYPE m_nDyncType;
	//! ��̬������صĲ��
	std::string m_szTagName;
	//! ��ִ̬��ʱ��ʱ�����
	//boost::posix_time::ptime  m_tmLast;


public:
	// �Ƚ�������̬�����Ƿ���һ��������
	virtual bool IsDyncTypeEqual(CBaseDyncData *pData) const;
	// ����Ƿ�Ϸ�
	virtual bool CheckDyncData();
	// Ԥ����̬
	virtual void PrepareWidgetDync(CBaseWidget *pWidget);
	// ��̬���ֵĴ���
	virtual bool ProcessWidgetDync(CBaseWidget *pWidget);
	// ������̬�¼�
//	virtual bool ProcessAction(CBaseWidget *pWidget, CEventIntent& intent);
	// ����ռ䲢�����Լ�
//	virtual CBaseDyncData *CloneDyncData() = 0;

	virtual void SaveXml(std::shared_ptr<QXmlStreamWriter>pXmlWriter) const;
	virtual bool LoadXml(std::shared_ptr<QXmlStreamReader>pXmlReader);
protected:
	void GetOccNoByTagName( );
	void LogMsg(const char *szLogTxt, int nLevel);

	bool GetRealTimeValue(std::shared_ptr<struct  TAGITEM > pInfo,CVariant *pValue );

	std::shared_ptr<struct TAGITEM> m_pTagInfo;	  
private:

};


#endif // BASE_DYNC_DATA_H


/** @}*/