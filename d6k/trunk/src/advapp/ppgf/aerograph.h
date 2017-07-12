/*! @file aerograph.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  aerograph.h
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
#ifndef AEROGRAPH_DEF_H
#define AEROGRAPH_DEF_H

#include "tag.h"

/*! \struct  CAerograph
*   \brief �����ǻ�����Ϣ */
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
	//!  �ܷ���
	CAinData m_TotalRadiation;
	//!  ��ֱ����
	CAinData m_DirectRadiation;
	//!  ɢ�����
	CAinData m_ScattRadiation;
	//!  ƽ������
	CAinData m_AvergWindDirect;
	//!  ƽ������
	CAinData m_AvergWindSpeed;
	//!  �����¶�
	CAinData m_AirTemperature;
	//!  ���ʪ��
	CAinData m_RelativeHumdty;
	//!  ����ѹ
	CAinData m_AirPressure;
};

#endif // AEROGRAPH_DEF_H