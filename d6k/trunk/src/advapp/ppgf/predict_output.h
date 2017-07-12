/*! @file predict_output.h
<PRE>
********************************************************************************
模块名       :
文件名       :  predict_output.h
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
#ifndef PREDICT_OUTPUT_DEF_H
#define PREDICT_OUTPUT_DEF_H

#include "tag.h"

/*! \struct  CAerograph
*   \brief 气象仪基本信息 */
class CPRredictOutput
{
public:
	CPRredictOutput(void);
	~CPRredictOutput(void);
public:
	bool GetRealTimeData();
protected:
	void LogMsg(const char *szLogTxt, int nLevel);
private:
	//!  超短期预测输出
	CAinData m_4Hour;
	//!  短期预测输出
	CAinData m_72Hour;

};

#endif // PREDICT_OUTPUT_DEF_H