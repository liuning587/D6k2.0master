
//后台发送报文到装置

#include "101sender.h"
#include "commplugin.h"
#include <QDateTime>

bool C101Sender::SendRequireLinkReset()
{
        CDeviceData* pDeviceData=m_p101Impl->GetFCIModule()->GetDeviceInfo() ;
		Q_ASSERT(pDeviceData);
		if (!pDeviceData)
		{
			return false;
		}
        
        AFCI afci;
        
        //链路控制域
		afci.m_ctrl.m_Dir = 0;
        afci.m_ctrl.m_PRM = DIRECTION_DOWN;
        afci.m_ctrl.m_DATA = 0;
        afci.m_ctrl.m_FCV= 0;
        afci.m_ctrl.m_FUNCCODE = 0;
		
		pDeviceData->m_nFCB = !pDeviceData->m_nFCB;

        //链路地址
        afci.m_addr.SetAddr(pDeviceData->m_nDeviceAddr);
            
		return Send_F((char*)(&afci),sizeof(afci));
}

bool C101Sender::SendRequireLinkState()
{
		AFCI afci;

        //链路控制域
		afci.m_ctrl.m_Dir = 0;
        afci.m_ctrl.m_PRM = DIRECTION_DOWN;
        afci.m_ctrl.m_DATA = 0;
        afci.m_ctrl.m_FCV= 0;
        afci.m_ctrl.m_FUNCCODE = 9;

		//pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
		
        //链路地址
        afci.m_addr.SetAddr(m_p101Impl->GetFCIModule()->GetDeviceAddr());

        return Send_F((char*)(&afci),sizeof(afci));
}

bool C101Sender::SendRequireAllData()
{
	return true;
}
bool C101Sender::SendRequireLinkData()
{
        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
		Q_ASSERT(pDeviceData);
		if (!pDeviceData)
		{
			return false;
		}

        AFCI afci;

        //链路控制域
        afci.m_ctrl.m_PRM = DIRECTION_DOWN;
        afci.m_ctrl.m_DATA = pDeviceData->m_nFCB;
        pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
        afci.m_ctrl.m_FCV= 1;
        afci.m_ctrl.m_FUNCCODE = (pDeviceData->m_nACD == 1)?10:11;//请求一级或二级数据

        //链路地址
        afci.m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

        return Send_F((char*)(&afci),sizeof(afci));
}

bool C101Sender::SendGeneralCall()
{
        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
		Q_ASSERT(pDeviceData);
		if (!pDeviceData)
		{
			return false;
		}
        char buf[BUFFER_SIZE_101];

        //链路控制域
        AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
        pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
        pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB; 
        pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
        pHead->m_ctrl.m_FCV = 1;
        pHead->m_ctrl.m_FUNCCODE = 3;
        //链路地址
        pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);
        
        //应用服务数据单元：组织ASDU100
        ASDU100* pAsdu100 = (ASDU100*)((char*)buf+sizeof(AVDU_HEAD));
        pAsdu100->type   = C_IC_NA_1;
        pAsdu100->vsq    = 0x01;
        pAsdu100->cot.SetCot(COT_ACT);
        pAsdu100->asduAddr.SetAddr( pDeviceData->m_nDeviceAddr );
        pAsdu100->m_infoaddr.SetAddr(0x00);
        pAsdu100->m_qoi = 0x14;

        return Send_V((char*)(buf),sizeof(ASDU100));
}

bool C101Sender::SendSyncTime()
{
        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
		Q_ASSERT(pDeviceData);
		if (!pDeviceData)
		{
			return false;
		}
        char buf[BUFFER_SIZE_101];

        //链路控制域
        AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
        pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
        pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB; 
        pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
        pHead->m_ctrl.m_FCV = 1;
        pHead->m_ctrl.m_FUNCCODE = 3;
        //链路地址
        pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

        //组织ASDU103
        ASDU103* pAsdu103 = (ASDU103*)((char*)buf+sizeof(AVDU_HEAD));

        pAsdu103->type = C_CS_NA_1;
        pAsdu103->vsq  = 0x01;

        pAsdu103->cot.SetCot(COT_ACT);
        pAsdu103->asduAddr.SetAddr( pDeviceData->m_nDeviceAddr );
        pAsdu103->m_infoaddr.SetAddr(0x00);

        struct CP56Time2a* cp56time2a = ( struct CP56Time2a*)( &(pAsdu103->m_time) );

		QDateTime tvTime = QDateTime::currentDateTime();
        cp56time2a->Initialize(tvTime.date().year(),tvTime.date().month(),tvTime.date().day(),tvTime.time().hour(),tvTime.time().hour(),tvTime.time().second(),tvTime.time().msec());

        return Send_V((char*)(buf),sizeof(ASDU103));
}



bool C101Sender::OnSendSetAnalogNormalizeRequest(NBM_TELECTRL& command)
{
        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
		Q_ASSERT(pDeviceData);
		if (!pDeviceData)
		{
			return false;
		}
        char buf[BUFFER_SIZE_101];

        //链路控制域
        AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
        pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
        pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB; 
        pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
        pHead->m_ctrl.m_FCV = 1;
        pHead->m_ctrl.m_FUNCCODE = 3;
        //链路地址
        pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

        //组织ASDU48
        ASDU48* pAsdu48 = (ASDU48*)((char*)buf+sizeof(AVDU_HEAD));

        pAsdu48->type = C_SE_NA_1;
        pAsdu48->vsq  = 0x01;

        int nDeviceAddr =m_p101Impl->GetFCIModule()->GetDeviceAddr();
        pAsdu48->asduAddr.SetAddr( nDeviceAddr );

        int nAnalogTelectrlStartAddr = m_p101Impl->GetFCIModule()->GetAnalogStartAddr();
        pAsdu48->m_infoaddr.SetAddr(command.m_nDataID+ nAnalogTelectrlStartAddr);

        pAsdu48->SetValue(command.m_fValue);

        pAsdu48->cot.SetCot((command.m_nCtrlType == TELECTRL_REQUEST_UNSELECT)?COT_DEACT:COT_ACT);
        pAsdu48->m_qos.SelectOrExcute = (command.m_nCtrlType==TELECTRL_REQUEST_SELECT)?1:0;
        pAsdu48->m_qos.QL = 0;

        return Send_V((char*)(buf),sizeof(ASDU48));
}

bool C101Sender::OnSendSetAnalogScaleRequest(NBM_TELECTRL& command)
{
       /* CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();

        char buf[BUFFER_SIZE_101];

        //链路控制域
        AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
        pHead->m_ctrl.m_DIRECTION = DIRECTION_DOWN;
        pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB; 
        pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
        pHead->m_ctrl.m_DFC = 1;
        pHead->m_ctrl.m_FUNCCODE = 3;
        //链路地址
        pHead->m_addr = pDeviceData->m_nDeviceAddr;

        //组织ASDU49
        ASDU49* pAsdu49 = (ASDU49*)((char*)buf+sizeof(AVDU_HEAD));

        pAsdu49->type = C_SE_NB_1;
        pAsdu49->vsq  = 0x01;

        int nDeviceAddr = m_p101Impl->GetFCIModule()->GetDeviceAddr();
        pAsdu49->asduAddr.SetAddr( nDeviceAddr );

        int nAnalogTelectrlStartAddr = m_p;
        pAsdu49->m_infoaddr.SetAddr(command.m_nDataID+ nAnalogTelectrlStartAddr);

        pAsdu49->SetValue(command.m_fValue);

        pAsdu49->cot.SetCot((command.m_nCtrlType == TELECTRL_REQUEST_UNSELECT)?COT_DEACT:COT_ACT);
        pAsdu49->m_qos.SelectOrExcute = (command.m_nCtrlType==TELECTRL_REQUEST_SELECT)?1:0;
        pAsdu49->m_qos.QL = 0;

        return Send_V((char*)(buf),sizeof(ASDU49));*/
	return true;
}


bool C101Sender::OnSendSetAnalogShortFloatRequest(NBM_TELECTRL& command)
{
      /*  CDeviceData* pDeviceData = m_pImpl->GetDevMgr()->GetCurDeviceData();

        char buf[BUFFER_SIZE_101];

        //链路控制域
        AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
        pHead->m_ctrl.m_DIRECTION = DIRECTION_DOWN;
        pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB; 
        pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
        pHead->m_ctrl.m_DFC = 1;
        pHead->m_ctrl.m_FUNCCODE = 3;
        //链路地址
        pHead->m_addr = pDeviceData->m_nDeviceAddr;

        //组织ASDU50
        ASDU50* pAsdu50 = (ASDU50*)((char*)buf+sizeof(AVDU_HEAD));

        pAsdu50->type = C_SE_NC_1;
        pAsdu50->vsq  = 0x01;

        int nDeviceAddr = m_pHost->getKernel()->GetDeviceAddress(command.m_nDeviceID);
        pAsdu50->asduAddr.SetAddr( nDeviceAddr );

        int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;
        pAsdu50->m_infoaddr.SetAddr(command.m_nDataID+ nAnalogTelectrlStartAddr);

        pAsdu50->SetValue(command.m_fValue);

        pAsdu50->cot.SetCot((command.m_nCtrlType == TELECTRL_REQUEST_UNSELECT)?COT_DEACT:COT_ACT);
        pAsdu50->m_qos.SelectOrExcute = (command.m_nCtrlType==TELECTRL_REQUEST_SELECT)?1:0;
        pAsdu50->m_qos.QL = 0;

        return Send_V((char*)(buf),sizeof(ASDU50));*/

	return true;
}

bool C101Sender::OnSendSetBinarySPRequest(NBM_TELECTRL& command)
{
        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();

        char buf[BUFFER_SIZE_101];

        //链路控制域
        AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
        pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
        pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB; 
        pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
        pHead->m_ctrl.m_FCV = 1;
        pHead->m_ctrl.m_FUNCCODE = 3;
        //链路地址
        pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

        //组织ASDU45
        ASDU45* pAsdu45 = (ASDU45*)((char*)buf+sizeof(AVDU_HEAD));

        pAsdu45->type = C_SC_NA_1;
        pAsdu45->vsq  = 0x01;

        int nDeviceAddr = m_p101Impl->GetFCIModule()->GetDeviceAddr();
        pAsdu45->asduAddr.SetAddr( nDeviceAddr );
        
		int nBinaryTelectrlStartAddr = 0;// m_p101Impl->GetFCIModule()->GetBinaryStartAddr();
        pAsdu45->m_infoaddr.SetAddr(command.m_nDataID+ nBinaryTelectrlStartAddr);


        pAsdu45->cot.SetCot((command.m_nCtrlType == TELECTRL_REQUEST_UNSELECT)?COT_DEACT:COT_ACT);
        pAsdu45->m_sco.SelectOrExcute = (command.m_nCtrlType==TELECTRL_REQUEST_SELECT)?1:0;
        pAsdu45->m_sco.QU=0;
        pAsdu45->m_sco.RES =0;
        pAsdu45->m_sco.SCS = command.m_fValue;
        
        return Send_V((char*)(buf),sizeof(ASDU45));
}


bool C101Sender::OnSendSetBinaryDPRequest(NBM_TELECTRL& command)
{
        CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
		Q_ASSERT(pDeviceData);
        char buf[BUFFER_SIZE_101];

        //链路控制域
        AVDU_HEAD* pHead = (AVDU_HEAD*)buf;
        pHead->m_ctrl.m_PRM = DIRECTION_DOWN;
        pHead->m_ctrl.m_DATA = pDeviceData->m_nFCB; 
        pDeviceData->m_nFCB = !pDeviceData->m_nFCB;
        pHead->m_ctrl.m_FCV = 1;
        pHead->m_ctrl.m_FUNCCODE = 3;
        //链路地址
        pHead->m_addr.SetAddr(pDeviceData->m_nDeviceAddr);

        //组织ASDU46
        ASDU46* pAsdu46 = (ASDU46*)((char*)buf+sizeof(AVDU_HEAD));

        pAsdu46->type = C_DC_NA_1;
        pAsdu46->vsq  = 0x01;

        int nDeviceAddr = m_p101Impl->GetFCIModule()->GetDeviceAddr();
        pAsdu46->asduAddr.SetAddr( nDeviceAddr );

		int nBinaryTelectrlStartAddr = 0;// m_pImpl->GetParam()->m_nBinaryTelectrlStartAddr;
        pAsdu46->m_infoaddr.SetAddr(command.m_nDataID+ nBinaryTelectrlStartAddr);


        pAsdu46->cot.SetCot((command.m_nCtrlType == TELECTRL_REQUEST_UNSELECT)?COT_DEACT:COT_ACT);
        pAsdu46->m_dco.SelectOrExcute = (command.m_nCtrlType==TELECTRL_REQUEST_SELECT)?1:0;
        pAsdu46->m_dco.QU=0;
        pAsdu46->m_dco.DCS = command.m_fValue+1;

        return Send_V((char*)(buf),sizeof(ASDU46));
}


