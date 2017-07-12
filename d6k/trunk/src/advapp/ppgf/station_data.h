/*! @file station_data.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  station_data.h
�ļ�ʵ�ֹ��� :  ��վ���ݶ���
����         :  GuoHaiJun
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ��վ���ݶ���
*  @author  GuoHaiJun
*  @version 1.0
*  @date    2017.07.04
*******************************************************************************/
#ifndef STATION_DATA_DEF_H
#define STATION_DATA_DEF_H

#include "tag.h"

/*! \struct  CStationTotalData
*   \brief ��վ���ݻ�����Ϣ */
class CStationTotalData
{
public:
	CStationTotalData(void);
	~CStationTotalData(void);
public:
	bool GetRealTimeData();
protected:
	void LogMsg(const char *szLogTxt, int nLevel);
private:
	//!  ���й�
	CAinData m_ActPower;
	//!  ���޹�
	CAinData m_ReactPower;
};

#endif // STATION_DATA_DEF_H
