/*! @file station_data.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  station_data.cpp
文件实现功能 :  整站数据定义
作者         :  GuoHaiJun
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   整站数据定义
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
** \details 获取实时数据
** \return bool
** \author GuoHaiJun
** \date 2017年7月4日
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