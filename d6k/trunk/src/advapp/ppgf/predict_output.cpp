/*! @file predict_output.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  predict_output.cpp
文件实现功能 :  预测输出定义
作者         :  GuoHaiJun
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   预测输出定义
*  @author  GuoHaiJun
*  @version 1.0
*  @date    2017.07.04
*******************************************************************************/

#include "predict_output.h" 
#include "log/log.h"
#include <QDebug>

CPRredictOutput::CPRredictOutput()
{

}

CPRredictOutput::~CPRredictOutput()
{

}

void CPRredictOutput::LogMsg(const char *szLogTxt, int nLevel)
{
	::LogMsg("PPGF", szLogTxt, nLevel, nullptr);
}
/*! \fn bool CPRredictOutput::GetRealTimeData()
*********************************************************************************************************
** \brief CPRredictOutput::GetRealTimeData
** \details 获取实时数据
** \return bool
** \author GuoHaiJun
** \date 2017年7月4日
** \note
********************************************************************************************************/
bool CPRredictOutput::GetRealTimeData()
{
	bool bRet = false;
	bRet |= m_4Hour.GetRealTimeData();
	bRet |= m_72Hour.GetRealTimeData();
	return bRet;
}

/** @}*/