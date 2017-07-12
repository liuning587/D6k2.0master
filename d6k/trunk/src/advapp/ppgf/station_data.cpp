/*! @file station_data.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  station_data.cpp
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

#include "station_data.h" 
#include "log/log.h"
#include <QDebug>

CStationTotalData::CStationTotalData()
{

}

CStationTotalData::~CStationTotalData()
{

}

void CStationTotalData::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("PPGF", szLogTxt, nLevel, nullptr);
}
/*! \fn bool CStationTotalData::GetRealTimeData()
*********************************************************************************************************
** \brief CStationTotalData::GetRealTimeData
** \details ��ȡʵʱ����
** \return bool
** \author GuoHaiJun
** \date 2017��7��4��
** \note
********************************************************************************************************/
bool CStationTotalData::GetRealTimeData()
{
	bool bRet = false;
	bRet |= m_ActPower.GetRealTimeData();
	bRet |= m_ReactPower.GetRealTimeData();
	return bRet;
}

/** @}*/