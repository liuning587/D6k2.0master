/*! @file aerograph.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  aerograph.cpp
�ļ�ʵ�ֹ��� :  �����Ƕ���
����         :  GuoHaiJun
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   �����Ƕ���
*  @author  GuoHaiJun
*  @version 1.0
*  @date    2017.07.04
*******************************************************************************/

#include "aerograph.h" 
#include "log/log.h"
#include <QDebug>

CAerograph::CAerograph()
{

}

CAerograph::~CAerograph()
{

}

void CAerograph::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("PPGF", szLogTxt, nLevel, nullptr);
}
/*! \fn bool CAerograph::GetRealTimeData()
*********************************************************************************************************
** \brief CAerograph::GetRealTimeData
** \details ��ȡʵʱ����
** \return bool
** \author GuoHaiJun
** \date 2017��7��4��
** \note
********************************************************************************************************/
bool CAerograph::GetRealTimeData()
{
	bool bRet = false;
	bRet |= m_TotalRadiation.GetRealTimeData();
	bRet |= m_DirectRadiation.GetRealTimeData();
	bRet |= m_ScattRadiation.GetRealTimeData();
	bRet |= m_AvergWindDirect.GetRealTimeData();
	bRet |= m_AvergWindSpeed.GetRealTimeData();
	bRet |= m_AirTemperature.GetRealTimeData();
	bRet |= m_RelativeHumdty.GetRealTimeData();
	bRet |= m_AirPressure.GetRealTimeData();
	return bRet;
}

/** @}*/