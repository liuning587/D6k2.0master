/*! @file predict_output.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  predict_output.h
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
#ifndef PREDICT_OUTPUT_DEF_H
#define PREDICT_OUTPUT_DEF_H

#include "tag.h"

/*! \struct  CAerograph
*   \brief �����ǻ�����Ϣ */
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
	//!  ������Ԥ�����
	CAinData m_4Hour;
	//!  ����Ԥ�����
	CAinData m_72Hour;

};

#endif // PREDICT_OUTPUT_DEF_H