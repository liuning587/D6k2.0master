/*! @file predict_output.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  predict_output.cpp
�ļ�ʵ�ֹ��� :  Ԥ���������
����         :  GuoHaiJun
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   Ԥ���������
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
** \details ��ȡʵʱ����
** \return bool
** \author GuoHaiJun
** \date 2017��7��4��
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