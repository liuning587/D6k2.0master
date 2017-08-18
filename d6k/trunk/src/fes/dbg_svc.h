/*! @file dbg_svc.h
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  dbg_svc.h
�ļ�ʵ�ֹ��� :  ���Դ���
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ���Դ���
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#ifndef _DBGSVR_MODULE_H
#define _DBGSVR_MODULE_H

#include "base_module.h"

class CServer;
class CDbgSvc : public CBaseModule
{
public:
	CDbgSvc(CFesSvc* pServer, const std::string& szMailBoxName, int &MailID);
	virtual ~CDbgSvc(void);

public:
	virtual bool Initialize(const char *pszDataPath, unsigned int nMode);
	virtual void Run();
	virtual void Shutdown();

protected:
	// ң��
	bool DoDout();
	// ң��
	bool DoAout();
	// �ڵ�Ͷ��
	bool DoNodeScanEnable();

	bool DoChannelScanEnable();

	bool DoDeviceScanEnable();

	bool DoSetVar();

private:
	void * m_pCtx;
	void * m_pSender;

	bool InitMQ(unsigned int nPortNo);

	void CloseMQ();
};

#endif // _DBGSVR_MODULE_H


/** @}*/

