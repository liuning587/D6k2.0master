/*! @file aerograph.h
<PRE>
********************************************************************************
模块名       :
文件名       :  aerograph.h
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
#ifndef AEROGRAPH_DEF_H
#define AEROGRAPH_DEF_H

#include "tag.h"

/*! \struct  CAerograph
*   \brief 气象仪基本信息 */
class CAerograph
{
public:
	CAerograph(void);
	~CAerograph(void);
public:
	bool GetRealTimeData();
protected:
	void LogMsg(const char *szLogTxt, int nLevel);
private:
	//!  总辐射
	CAinData m_TotalRadiation;
	//!  垂直辐射
	CAinData m_DirectRadiation;
	//!  散射辐射
	CAinData m_ScattRadiation;
	//!  平均风向
	CAinData m_AvergWindDirect;
	//!  平均风速
	CAinData m_AvergWindSpeed;
	//!  大气温度
	CAinData m_AirTemperature;
	//!  相对湿度
	CAinData m_RelativeHumdty;
	//!  大气压
	CAinData m_AirPressure;
};

#endif // AEROGRAPH_DEF_H