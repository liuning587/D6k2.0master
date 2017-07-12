/*! @file aerograph.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  aerograph.cpp
文件实现功能 :  气象仪定义
作者         :  GuoHaiJun
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   气象仪定义
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
** \details 获取实时数据
** \return bool
** \author GuoHaiJun
** \date 2017年7月4日
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