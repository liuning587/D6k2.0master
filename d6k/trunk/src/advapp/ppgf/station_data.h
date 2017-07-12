/*! @file station_data.h
<PRE>
********************************************************************************
模块名       :
文件名       :  station_data.h
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
#ifndef STATION_DATA_DEF_H
#define STATION_DATA_DEF_H

#include "tag.h"

/*! \struct  CStationTotalData
*   \brief 整站数据基本信息 */
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
	//!  总有功
	CAinData m_ActPower;
	//!  总无功
	CAinData m_ReactPower;
};

#endif // STATION_DATA_DEF_H
