
//后台接收到从装置发来的报文

#include "101recver.h"
#include "commplugin.h"
#include "devicestudio/main_module.h"
#include "ftu_module.h"
#include "101sender.h"
#include <algorithm>



std::string FormatBuffer(char* pBuff, int nMsgLen)
{
	std::string strDebugData("");
	char szByte[10] = { '\0' };

	for (int i = 0; i < nMsgLen; i++)
	{
		sprintf(szByte, "%02X ", (unsigned char)pBuff[i]);
		strDebugData += szByte;
	}
	return strDebugData;
}



//固定帧：来自装置
bool C101Recver::OnRecvFfromMonitor(char* pBuff, int nLen)
{
	AFCI* pAfci = (AFCI*)pBuff;
	//1验证校验码

	//2解析链路地址
	int nDeviceAddr = pAfci->m_addr.GetAddr();

	CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
	Q_ASSERT(pDeviceData);
	if (pDeviceData == NULL)
		return false;

	//解析功能码，调整链路状态
	int nFuncCode = pAfci->m_ctrl.m_FUNCCODE;
	switch (nFuncCode)
	{
	case 0://认可或者复位远方链路
		if (pAfci->m_ctrl.m_PRM == 1 &&pAfci->m_ctrl.m_Dir== DIRECTION_DOWN)
		{
			m_p101Impl->getSender()->SendReplyLinkReset();
		}
		else
		{
			pDeviceData->m_nDeviceState = DEVICE_STATE_NONE;
		}
		break;
	case 1://否定
		break;
	case 9://回复请求链路响应
	{
		m_p101Impl->getSender()->SendReplyLinkState();
		break;

	}
		
	case 11://响应链路状态
	{
		switch (pDeviceData->m_nDeviceState)
		{
		case DEVICE_STATE_UNKNOWN:
			pDeviceData->m_nDeviceState = DEVICE_STATE_LINKSTATE;
			break;
		case DEVICE_STATE_LINKSTATE:
			pDeviceData->m_nDeviceState = DEVICE_STATE_LINKRESET;
			break;
		}
	}
	break;
	}

	//保存ACD值
	pDeviceData->m_nACD = pAfci->m_ctrl.m_DATA;

	return true;
}

//可变帧：总召确认
bool C101Recver::OnRecvGeneralCallConfirm(char* pBuff, int nLen)
{
	std::string strBuffer = FormatBuffer(pBuff, nLen);

	ASDU100* pAsdu100 = (ASDU100*)pBuff;

	CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
	if (pDeviceData == NULL)
		return false;

	int nCot = pAsdu100->cot.GetCot();
	if (nCot == COT_ACTTERM)//总召结束
	{
		QString szGI = tr("[GI COMPLETE]");
		m_p101Impl->GetFCIModule()->m_pMainModule->LogString("FCI", szGI.toLocal8Bit(), szGI.length());

		pDeviceData->m_nDeviceState = DEVICE_STATE_NONE;
	}
	if (nCot == COT_ACTCON)//总召确认
	{
		//激活总招
		QString szGI = tr("[GI CONFIRM]");
		m_p101Impl->GetFCIModule()->m_pMainModule->LogString("FCI", szGI.toLocal8Bit(), szGI.length());
		pDeviceData->m_nDeviceState = DEVICE_STATE_NONE;
	}

	return true;
}

//可变帧：对时确认
bool C101Recver::OnRecvSyncTimeConfirm(char* pBuff, int nLen)
{
	std::string strBuffer = FormatBuffer(pBuff, nLen);

	ASDU103* pAsdu103 = (ASDU103*)pBuff;

	CDeviceData* pDeviceData = m_p101Impl->GetFCIModule()->GetDeviceInfo();
	if (pDeviceData == NULL)
		return false;

	int nCot = pAsdu103->cot.GetCot();

	if (nCot == COT_ACTCON)//对时结束
	{
		pDeviceData->m_nDeviceState = DEVICE_STATE_SYNCTIME_RDTIME;

		pDeviceData->m_nLastCallTime = QDateTime::currentDateTime().toTime_t();
		
		pDeviceData->m_nLastSyncTime = QDateTime::currentDateTime().toTime_t();		
	}
	if (nCot==COT_REQ)//读取对时
	{
		//读取到装置的时间为………………
		QString szRecvTime = tr("[DEVTIME:]")+ pAsdu103->m_time.Dump();
		m_p101Impl->GetFCIModule()->m_pMainModule->LogString("FCI", szRecvTime.toStdString().c_str(), szRecvTime.length());
	}

	return true;
}

//归一化测量值
bool C101Recver::OnRecvAnalogNormalized(char* pBuff, int nLen)
{
	Q_ASSERT(pBuff && nLen > 0);
	if (nLen <= 0)
	{
		return false;
	}
	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;
	QMap<int, float> mapPointValue;

	if (pAsduBase->IsSQ1())
	{
		ASDU9_SQ1* pAsdu9SQ1 = (ASDU9_SQ1*)pBuff;

		int nDeviceAddr = pAsdu9SQ1->asduAddr.GetAddr();

		int nCount = pAsdu9SQ1->GetItemCount();

		int nPointID = pAsdu9SQ1->infoaddr.GetAddr();


		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu9SQ1->GetValue(i);

			QDS qds = pAsdu9SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			emit Signal_AnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);
			//定值修改
			mapPointValue[nPointID] = fValue;


			nPointID++;
		}
	}
	else
	{
		ASDU9_SQ0* pAsdu9SQ0 = (ASDU9_SQ0*)pBuff;

		int nDeviceAddr = pAsdu9SQ0->asduAddr.GetAddr();

		int nCount = pAsdu9SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu9SQ0->m_data[i].infoaddr.GetAddr();

			float fValue = pAsdu9SQ0->GetValue(i);

			QDS qds = pAsdu9SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			emit Signal_AnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);

			//定值修改
			mapPointValue[nPointID] = fValue;

		}
	}

	return true;
}
//带短时标的归一化测量值
bool C101Recver::OnRecvAnalogNormalizedShortTime(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return false;
	}
	ASDU10_SQ0* pAsdu10SQ0 = (ASDU10_SQ0*)pBuff;
	QMap<int, float> mapPointValue;


	int nCount = pAsdu10SQ0->GetItemCount();
	int nDeviceAddr = pAsdu10SQ0->asduAddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu10SQ0->m_data[i].m_infoaddr.GetAddr();

		float fValue = pAsdu10SQ0->GetValue(i);

		QDS qds = pAsdu10SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;

		emit Signal_AnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);

		mapPointValue[nPointID] = fValue;
	}
	return true;
}
//标度化测量值
bool C101Recver::OnRecvAnalogScaled(char* pBuff, int nLen)
{
	Q_ASSERT(pBuff && nLen > 0);
	if (nLen <= 0)
	{
		return false;
	}
	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;

	QMap<int, float> mapPointValue;

	if (pAsduBase->IsSQ1())
	{
		ASDU11_SQ1* pAsdu11SQ1 = (ASDU11_SQ1*)pBuff;

		int nDeviceAddr = pAsdu11SQ1->asduAddr.GetAddr();

		int nCount = pAsdu11SQ1->GetItemCount();

		int nPointID = pAsdu11SQ1->infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu11SQ1->GetValue(i);

			QDS qds = pAsdu11SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;
			//通过同一信号发出去  这边没必要进行区分
			emit Signal_ScaledAnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);

			//定值修改
			mapPointValue[nPointID] = fValue;

			nPointID++;
		}
	}
	else
	{
		ASDU11_SQ0* pAsdu11SQ0 = (ASDU11_SQ0*)pBuff;

		int nDeviceAddr = pAsdu11SQ0->asduAddr.GetAddr();

		int nCount = pAsdu11SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu11SQ0->m_data[i].infoaddr.GetAddr();

			float fValue = pAsdu11SQ0->GetValue(i);

			QDS qds = pAsdu11SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;
			//通过同一信号发出去  这边没必要进行区分
			emit Signal_AnalogNormalShortTime(nDeviceAddr, nPointID, fValue, nQuality);

			//定值修改
			mapPointValue[nPointID] = fValue;
		}
	}

	return true;
}
//带短时标的标度化测量值
bool C101Recver::OnRecvAnalogScaledShortTime(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return false;
	}
	ASDU12_SQ0* pAsdu12SQ0 = (ASDU12_SQ0*)pBuff;

	QMap<int, float> mapPointValue;

	int nCount = pAsdu12SQ0->GetItemCount();
	int nDeviceAddr = pAsdu12SQ0->asduAddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu12SQ0->m_data[i].m_infoaddr.GetAddr();

		float fValue = pAsdu12SQ0->GetValue(i);

		QDS qds = pAsdu12SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;
		//通过同一信号发出去  这边没必要进行区分
		emit Signal_ScaledAnalogNormal(nDeviceAddr, nPointID, fValue, nQuality);

		//定值修改
		mapPointValue[nPointID] = fValue;
	}

	return true;
}
//短浮点数测量值
bool C101Recver::OnRecvAnalogShortFloat(char* pBuff, int nLen)
{
	Q_ASSERT(pBuff && nLen > 0);
	if (nLen <= 0)
	{
		return false;
	}
	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;

	QMap<int, float> mapPointValue;

	if (pAsduBase->IsSQ1())
	{
		ASDU13_SQ1* pAsdu13SQ1 = (ASDU13_SQ1*)pBuff;

		int nDeviceAddr = pAsdu13SQ1->asduAddr.GetAddr();

		int nCount = pAsdu13SQ1->GetItemCount();

		int nPointID = pAsdu13SQ1->infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu13SQ1->GetValue(i);

			QDS qds = pAsdu13SQ1->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			float tt = 0.0;
			*(uint32_t *)(&tt) = (unsigned char)(pAsdu13SQ1->m_data[i].m_fValue) + (unsigned char)pAsdu13SQ1->m_data[i].m_fValue[1] * 0x100 + (unsigned char)pAsdu13SQ1->m_data[i].m_fValue[2] * 0x10000 + (unsigned char)pAsdu13SQ1->m_data[i].m_fValue[3] * 0x1000000;

			emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, tt, nQuality);

			//定值修改
			mapPointValue[nPointID] = tt;

			nPointID++;
		}
	}
	else
	{
		ASDU13_SQ0* pAsdu13SQ0 = (ASDU13_SQ0*)pBuff;

		int nDeviceAddr = pAsdu13SQ0->asduAddr.GetAddr();

		int nCount = pAsdu13SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu13SQ0->m_data[i].infoaddr.GetAddr();

			float fValue = pAsdu13SQ0->GetValue(i);

			QDS qds = pAsdu13SQ0->GetQDS(i);

			int nQuality = QUALITY_NORMAL;

			if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
			if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
			if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
			if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
			if (qds.IV == 1) nQuality |= QUALITY_INVALID;

			float tt = 0.0;
			*(uint32_t *)(&tt) = (unsigned char)(pAsdu13SQ0->m_data[i].m_fValue) + (unsigned char)pAsdu13SQ0->m_data[i].m_fValue[1] * 0x100 + (unsigned char)pAsdu13SQ0->m_data[i].m_fValue[2] * 0x10000 + (unsigned char)pAsdu13SQ0->m_data[i].m_fValue[3] * 0x1000000;


			emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, tt, nQuality);

			//定值修改
			mapPointValue[nPointID] = tt;
		}
	}

	return true;
}

//带短时标的短浮点数测量值
bool C101Recver::OnRecvAnalogShortFloatShortTime(char* pBuff, int nLen)
{
	Q_ASSERT(pBuff && nLen > 0);

	if (nLen <= 0 || !pBuff)
	{
		return false;
	}
	ASDU14_SQ0* pAsdu14SQ0 = (ASDU14_SQ0*)pBuff;

	QMap<int, float> mapPointValue;

	int nCount = pAsdu14SQ0->GetItemCount();
	int nDeviceAddr = pAsdu14SQ0->asduAddr.GetAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu14SQ0->m_data[i].m_infoaddr.GetAddr();

		float fValue = pAsdu14SQ0->GetValue(i);

		QDS qds = pAsdu14SQ0->GetQDS(i);

		int nQuality = QUALITY_NORMAL;

		if (qds.OV == 1) nQuality |= QUALITY_OVERFLOW;
		if (qds.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (qds.SB == 1) nQuality |= QUALITY_REPLACED;
		if (qds.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (qds.IV == 1) nQuality |= QUALITY_INVALID;
		//通过同一信号发出去  这边没必要进行区分

		float tt = 0.0;
		*(uint32_t *)(&tt) = (unsigned char)(pAsdu14SQ0->m_data[i].m_std) + (unsigned char)pAsdu14SQ0->m_data[i].m_std[1] * 0x100 + (unsigned char)pAsdu14SQ0->m_data[i].m_std[2] * 0x10000 + (unsigned char)pAsdu14SQ0->m_data[i].m_std[3] * 0x1000000;

		emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, tt, nQuality);

		//定值修改
		mapPointValue[nPointID] = tt;
	}

	return true;
}

//不带品质描述词的归一化测量值
bool C101Recver::OnRecvAnalogNormalizedNoQuality(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return false;
	}
	ASDU_BASE* pAsduBase = (ASDU_BASE*)pBuff;

	if (pAsduBase->IsSQ1())
	{
		ASDU21_SQ1* pAsdu21SQ1 = (ASDU21_SQ1*)pBuff;

		int nCount = pAsdu21SQ1->GetItemCount();

		int nPointID = pAsdu21SQ1->m_infoaddr.GetAddr();

		int nDeviceAddr = pAsdu21SQ1->asduAddr.GetAddr();
		for (int i = 0; i < nCount; i++)
		{
			float fValue = pAsdu21SQ1->GetValue(i);
			//通过同一信号发出去  这边没必要进行区分
			emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, fValue, 0);

			nPointID++;
		}
	}
	else
	{
		ASDU21_SQ0* pAsdu21SQ0 = (ASDU21_SQ0*)pBuff;

		int nCount = pAsdu21SQ0->GetItemCount();

		int nDeviceAddr = pAsdu21SQ0->asduAddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu21SQ0->m_data[i].m_infoaddr.GetAddr();

			float fValue = pAsdu21SQ0->GetValue(i);
			//通过同一信号发出去  这边没必要进行区分
			emit Signal_AnalogShortFloat(nDeviceAddr, nPointID, fValue, 0);
		}
	}

	return true;
}

//带长时标的归一化测量值
bool C101Recver::OnRecvAnalogNormalizedLongTime(char* pBuff, int nLen)
{
	/*IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->getGroupID();
	int nChannelID = m_pHost->getChannelID();
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU34_SQ0* pAsdu34SQ0 = (ASDU34_SQ0*)pBuff;

	int nDeviceAddr = pAsdu34SQ0->asduAddr.GetAddr();
	int nDeviceID = m_pImpl->GetDevMgr()->GetDeviceID(nDeviceAddr);

	int nCount = pAsdu34SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu34SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nAnalogStartAddr;

		float fValue = pAsdu34SQ0->GetValue(i);
		pKernel->SetAnalogValue(nGroupID, nChannelID, nDeviceID, nPointID, fValue);
	}*/

	return true;
}

//带长时标的标度化测量值
bool C101Recver::OnRecvAnalogScaledLongTime(char* pBuff, int nLen)
{
	/*IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->getGroupID();
	int nChannelID = m_pHost->getChannelID();
	int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

	ASDU35_SQ0* pAsdu35SQ0 = (ASDU35_SQ0*)pBuff;

	int nDeviceAddr = pAsdu35SQ0->asduAddr.GetAddr();
	int nDeviceID = m_pImpl->GetDevMgr()->GetDeviceID(nDeviceAddr);

	int nCount = pAsdu35SQ0->GetItemCount();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu35SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nAnalogStartAddr;

		float fValue = pAsdu35SQ0->GetValue(i);
		pKernel->SetAnalogValue(nGroupID, nChannelID, nDeviceID, nPointID, fValue);
	}
*/


	return true;
}

//带长时标的短浮点数测量值
bool C101Recver::OnRecvAnalogShortFloatLongTime(char* pBuff, int nLen)
{
	/*(
	IKernel* pKernel = m_pHost->getKernel();
		int nGroupID = m_pHost->getGroupID();
		int nChannelID = m_pHost->getChannelID();
		int nAnalogStartAddr = m_pImpl->GetParam()->m_nAnalogStartAddr;

		ASDU36_SQ0* pAsdu36SQ0 = (ASDU36_SQ0*)pBuff;

		int nDeviceAddr = pAsdu36SQ0->asduAddr.GetAddr();
		int nDeviceID = m_pImpl->GetDevMgr()->GetDeviceID(nDeviceAddr);

		int nCount = pAsdu36SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu36SQ0->m_data[i].m_infoaddr.GetAddr();
			nPointID = nPointID - nAnalogStartAddr;

			float fValue = pAsdu36SQ0->GetValue(i);
			pKernel->SetAnalogValue(nGroupID, nChannelID, nDeviceID, nPointID, fValue);
		})
	*/


	return true;
}
bool C101Recver::OnRecvBinarySinglePoint(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return false;
	}

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;

	if (pBase->IsSQ1())
	{
		//连续单点遥信
		ASDU1_SQ1* pAsdu1SQ1 = (ASDU1_SQ1*)pBuff;

		int nDeviceAddr = pAsdu1SQ1->asduAddr.GetAddr();

		int nCount = pAsdu1SQ1->GetItemCount();

		int nPointID = pAsdu1SQ1->infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu1SQ1->GetValue(i);

			//发出信号   或者存入共享内存  待定
			emit Signal_OnePointRemote(nDeviceAddr, nPointID, nValue);

			nPointID++;
		}
	}
	else
	{
		ASDU1_SQ0* pAsdu1SQ0 = (ASDU1_SQ0*)pBuff;

		int nDeviceAddr = pAsdu1SQ0->asduAddr.GetAddr();

		int nCount = pAsdu1SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu1SQ0->m_data[i].infoaddr.GetAddr();

			int nValue = pAsdu1SQ0->GetValue(i);
			//发出信号   或者存入共享内存  待定
			emit Signal_onePointDisRemote(nDeviceAddr, nPointID, nValue);

		}
	}

	return true;
}
bool C101Recver::OnRecvBinaryDoublePoint(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return false;
	}

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU3_SQ1* pAsdu3SQ1 = (ASDU3_SQ1*)pBuff;

		int nDeviceAddr = pAsdu3SQ1->asduAddr.GetAddr();

		int nCount = pAsdu3SQ1->GetItemCount();

		int nPointID = pAsdu3SQ1->infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu3SQ1->GetValue(i);//>m_siq[i];

			emit Signal_DoublePointRemote(nDeviceAddr, nPointID, nValue);

			nPointID++;
		}
	}
	else
	{
		ASDU3_SQ0* pAsdu3SQ0 = (ASDU3_SQ0*)pBuff;

		int nDeviceAddr = pAsdu3SQ0->asduAddr.GetAddr();

		int nCount = pAsdu3SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu3SQ0->m_data[i].infoaddr.GetAddr();

			int nValue = pAsdu3SQ0->GetValue(i);//>m_siq[i].siq;

			emit Signal_DoublePointDisRemote(nDeviceAddr, nPointID, nValue);

		}
	}
	return true;
}

//带变位检出的成组单点 ASDU20
bool C101Recver::OnRecvBinaryGroupPoint(char* pBuff, int nLen)
{
	if (nLen <= 0)
	{
		return false;
	}

	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU20_SQ1* pAsdu20SQ1 = (ASDU20_SQ1*)pBuff;

		int nDeviceAddr = pAsdu20SQ1->asduAddr.GetAddr();

		int nCount = pAsdu20SQ1->GetItemCount();

		int nPointID = pAsdu20SQ1->m_infoaddr.GetAddr();

		for (int i = 0; i < nCount; i++)
		{
			nPointID += 16 * i;

			int nValue = pAsdu20SQ1->GetValue(i);

			for (int k = 0; k < 16; k++)
			{
				int nBitValue = (nValue & (1 << k)) >> k;
				emit Signal_BinaryGroupPoint(nDeviceAddr, nPointID + k, nBitValue);
			}
		}
	}
	else
	{
		ASDU20_SQ0* pAsdu20SQ0 = (ASDU20_SQ0*)pBuff;

		int nCount = pAsdu20SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nPointID = pAsdu20SQ0->m_data[i].m_infoaddr.GetAddr();

			int nDeviceAddr = pAsdu20SQ0->asduAddr.GetAddr();

			int nValue = pAsdu20SQ0->GetValue(i);

			for (int k = 0; k < 16; k++)
			{
				int nBitValue = (nValue & (1 << k)) >> k;
				emit Signal_BinaryGroupDisPoint(nDeviceAddr, nPointID + k, nBitValue);
			}
		}
	}
	return true;
}

//收到单点命令确认
void C101Recver::OnRecvSetBinarySPAck(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}
	ASDU45* pAsdu45 = (ASDU45*)(pBuff);
	int nDataID = pAsdu45->m_infoaddr.GetAddr();
	int nRecvAckCot = pAsdu45->cot.GetCot();
	int nRecvAckType = pAsdu45->m_sco.SelectOrExcute;

	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
	telectrl.m_nDeviceID = 0;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_BINARY;
	telectrl.m_fValue = pAsdu45->m_sco.SCS;


	if (nRecvAckCot == COT_DEACTCON)
	{
		telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
		int ionFloag = 0;

		if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
		{
			//on
			ionFloag = 0;
		}
		else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
		{
			//on
			ionFloag = 1;
		}
		else
		{
			//error
			ionFloag = -1;
		}
		emit Signal_ControlFeedBack(0, telectrl.m_nDataID, ionFloag, QString::number(nRecvAckCot));
	}
	else if (nRecvAckType == 0 && nRecvAckCot == COT_ACTCON) //执行
	{
		telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
		int ionFloag = 0;

		if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
		{
			//on
			ionFloag = 0;
		}
		else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
		{
			//on
			ionFloag = 1;
		}
		else
		{
			//error
			ionFloag = -1;
		}
		emit Signal_ControlFeedBack(0, telectrl.m_nDataID, ionFloag, "Exec Response");
	}
	else if (nRecvAckType == 1 && nRecvAckCot == COT_ACTCON)//选择
	{
		telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
		int ionFloag = 0;

		if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
		{
			//on
			ionFloag = 0;
		}
		else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
		{
			//off
			ionFloag = 1;
		}
		else
		{
			//error
			ionFloag = -1;
		}
		emit Signal_ControlFeedBack(0, telectrl.m_nDataID, ionFloag, "Preset Response");

	}
	else
	{
		int ionFloag = 0;

		if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
		{
			//on
			ionFloag = 0;
		}
		else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
		{
			//off
			ionFloag = 1;
		}
		else
		{
			//error
			ionFloag = -1;
		}

		emit Signal_ControlFeedBack(0, telectrl.m_nDataID, ionFloag, QString::number(nRecvAckCot));
		telectrl.m_nCtrlType = -1;
	}
}

//收到双点命令确认
void C101Recver::OnRecvSetBinaryDPAck(char* pBuff, int nLength)
{
	if (nLength <= 0)
	{
		return;
	}

	ASDU46* pAsdu46 = (ASDU46*)(pBuff);
	int nDataID = pAsdu46->m_infoaddr.GetAddr();
	int nRecvAckCot = pAsdu46->cot.GetCot();
	int nRecvAckType = pAsdu46->m_dco.SelectOrExcute;

	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = 0;
	telectrl.m_nDeviceID = 0;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_BINARY;
	telectrl.m_fValue = pAsdu46->m_dco.DCS - 1;



	if (nRecvAckCot == COT_DEACTCON)
	{
		telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;

		int ionFloag = 0;

		if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
		{
			//on
			ionFloag = 0;
		}
		else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
		{
			//off
			ionFloag = 1;
		}
		else
		{
			//error
			ionFloag = -1;
		}
		emit Signal_ControlFeedBack(1, telectrl.m_nDataID, ionFloag, QString::number(nRecvAckCot));

	}
	else if (nRecvAckType == 0 && nRecvAckCot == COT_ACTCON) //执行
	{
		telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
		int ionFloag = 0;

		if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
		{
			//on
			ionFloag = 0;
		}
		else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
		{
			//off
			ionFloag = 1;
		}
		else
		{
			//error
			ionFloag = -1;
		}
		emit Signal_ControlFeedBack(1, telectrl.m_nDataID, ionFloag, "Exec Response");

	}
	else if (nRecvAckType == 1 && nRecvAckCot == COT_ACTCON)//选择
	{
		telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
		int ionFloag = 0;

		if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
		{
			//on
			ionFloag = 0;
		}
		else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
		{
			//off
			ionFloag = 1;
		}
		else
		{
			//error
			ionFloag = -1;
		}
		emit Signal_ControlFeedBack(1, telectrl.m_nDataID, ionFloag, "Preset Response");
	}
	else
	{

		int ionFloag = 0;

		if (telectrl.m_fValue > 0.99999 && telectrl.m_fValue < 1.00001)
		{
			//on
			ionFloag = 0;
		}
		else if (telectrl.m_fValue > -0.000001 && telectrl.m_fValue < 0.00001)
		{
			//off
			ionFloag = 1;
		}
		else
		{
			//error
			ionFloag = -1;
		}
		emit Signal_ControlFeedBack(1, telectrl.m_nDataID, ionFloag, QString::number(nRecvAckCot));
		telectrl.m_nCtrlType = -1;
	}

}
//收到归一化设点命令确认
void C101Recver::OnRecvSetAnalogNormalizeAck(char* pBuff, int nLength)
{
	/*IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->getGroupID();
	int nChannelID = m_pHost->getChannelID();

	ASDU48* pAsdu48 = (ASDU48*)(pBuff);

	int nDeviceAddr = pAsdu48->asduAddr.GetAddr();
	int nDeviceID = m_pImpl->GetDevMgr()->GetDeviceID(nDeviceAddr);

	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;

	int nDataID = pAsdu48->m_infoaddr.GetAddr() - nAnalogTelectrlStartAddr;
	int nRecvAckCot = pAsdu48->cot.GetCot();
	int nRecvAckType = pAsdu48->m_qos.SelectOrExcute;

	//1、上送遥控确认至前置内核
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_ANALOG;

	if (nRecvAckCot == COT_DEACTCON)
		telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
	else if (nRecvAckType == 0) //执行
		telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
	else if (nRecvAckType == 1)//选择
		telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
	else
		telectrl.m_nCtrlType = -1;

	if (!pKernel->PushCommand(-1, -1, -1, telectrl))//GroupID=-1,ChannelID=-1,DeviceID=-1表示前置内核遥控队列
	{
		char buf[256];
		sprintf(buf, "OnRecvSetAnalogNormalizeAck PushCommand failed: GroupID%d  DeviceID:%d DataID:%d CtrlType:%d", nGroupID, nDeviceID, nDataID, telectrl.m_nCtrlType);
		m_pHost->LogCallback101(MCB_ERROR, TYPE_V, buf);
	}*/
}

//收到标度化设点命令确认
void C101Recver::OnRecvSetAnalogScaleAck(char* pBuff, int nLength)
{
	/*IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->getGroupID();
	int nChannelID = m_pHost->getChannelID();

	ASDU49* pAsdu49 = (ASDU49*)(pBuff);

	int nDeviceAddr = pAsdu49->asduAddr.GetAddr();
	int nDeviceID = m_pImpl->GetDevMgr()->GetDeviceID(nDeviceAddr);

	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;
	int nDataID = pAsdu49->m_infoaddr.GetAddr() - nAnalogTelectrlStartAddr;
	int nRecvAckCot = pAsdu49->cot.GetCot();
	int nRecvAckType = pAsdu49->m_qos.SelectOrExcute;

	//1、上送遥控确认至前置内核
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_ANALOG;

	if (nRecvAckCot == COT_DEACTCON)
		telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
	else if (nRecvAckType == 0) //执行
		telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
	else if (nRecvAckType == 1)//选择
		telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
	else
		telectrl.m_nCtrlType = -1;

	if (!pKernel->PushCommand(-1, -1, -1, telectrl))//GroupID=-1,ChannelID=-1,DeviceID=-1表示前置内核遥控队列
	{
		char buf[256];
		sprintf(buf, "OnRecvSetAnalogScaleAck PushCommand failed: GroupID%d  DeviceID:%d DataID:%d CtrlType:%d", nGroupID, nDeviceID, nDataID, telectrl.m_nCtrlType);
		m_pHost->LogCallback101(MCB_ERROR, TYPE_V, buf);
	}*/
}

//收到短浮点数设点命令确认
void C101Recver::OnRecvSetAnalogShortFloatAck(char* pBuff, int nLength)
{
	/*IKernel* pKernel = m_pHost->getKernel();
	int nGroupID = m_pHost->getGroupID();
	int nChannelID = m_pHost->getChannelID();

	ASDU50* pAsdu50 = (ASDU50*)(pBuff);

	int nDeviceAddr = pAsdu50->asduAddr.GetAddr();
	int nDeviceID = m_pImpl->GetDevMgr()->GetDeviceID(nDeviceAddr);

	int nAnalogTelectrlStartAddr = m_pImpl->GetParam()->m_nAnalogTelectrlStartAddr;
	int nDataID = pAsdu50->m_infoaddr.GetAddr() - nAnalogTelectrlStartAddr;
	int nRecvAckCot = pAsdu50->cot.GetCot();
	int nRecvAckType = pAsdu50->m_qos.SelectOrExcute;

	//1、上送遥控确认至前置内核
	NBM_TELECTRL telectrl;
	telectrl.m_nStationID = nGroupID;
	telectrl.m_nDeviceID = nDeviceID;
	telectrl.m_nDataID = nDataID;
	telectrl.m_nDataType = DATATYPE_ANALOG;

	if (nRecvAckCot == COT_DEACTCON)
		telectrl.m_nCtrlType = TELECTRL_ACK_UNSELECT;
	else if (nRecvAckType == 0) //执行
		telectrl.m_nCtrlType = TELECTRL_ACK_EXECUTE;
	else if (nRecvAckType == 1)//选择
		telectrl.m_nCtrlType = TELECTRL_ACK_SELECT;
	else
		telectrl.m_nCtrlType = -1;

	if (!pKernel->PushCommand(-1, -1, -1, telectrl))//GroupID=-1,ChannelID=-1,DeviceID=-1表示前置内核遥控队列
	{
		char buf[256];
		sprintf(buf, "OnRecvSetAnalogShortFloatAck PushCommand failed: GroupID%d  DeviceID:%d DataID:%d CtrlType:%d", nGroupID, nDeviceID, nDataID, telectrl.m_nCtrlType);
		m_pHost->LogCallback101(MCB_ERROR, TYPE_V, buf);
	}*/
}


void C101Recver::OnRecvKwh(char* pBuff, int nLength)
{
	ASDU_BASE* pBase = (ASDU_BASE*)pBuff;
	if (pBase->IsSQ1())
	{
		ASDU15_SQ1* pAsdu15SQ1 = (ASDU15_SQ1*)pBuff;
		int nDeviceID = pAsdu15SQ1->asduAddr.GetAddr();
		int nCount = pAsdu15SQ1->GetItemCount();
		int nPointID = pAsdu15SQ1->infoaddr.GetAddr();


		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu15SQ1->m_data[i].m_nValue;
			emit Signal_KwhNormal(nDeviceID, nPointID, nValue);
			nPointID++;
		}
	}
	else
	{
		ASDU15_SQ0* pAsdu15SQ0 = (ASDU15_SQ0*)pBuff;
		int nDeviceID = pAsdu15SQ0->asduAddr.GetAddr();
		int nCount = pAsdu15SQ0->GetItemCount();

		for (int i = 0; i < nCount; i++)
		{
			int nValue = pAsdu15SQ0->m_data[i].m_nValue;
			int nPointID = pAsdu15SQ0->m_data[i].m_infoaddr.GetAddr();
			emit Signal_KwhDisNormal(nDeviceID, nPointID, nValue);
		}
	}
}

void C101Recver::OnRecvKwhShortTime(char* pBuff, int nLength)
{
	ASDU16_SQ0* pAsdu16SQ0 = (ASDU16_SQ0*)pBuff;

	int nCount = pAsdu16SQ0->GetItemCount();

	int nKwhStartAddr = m_p101Impl->GetFCIModule()->GetKwhStartAddr();

	for (int i = 0; i < nCount; i++)
	{
		float fValue = pAsdu16SQ0->m_data[i].m_fValue;
		int nPointID = pAsdu16SQ0->m_data[i].m_infoaddr.GetAddr();
		//nPointID = nPointID - nKwhStartAddr;
		emit Signal_KwhShortTimeNormal(1, nPointID, fValue);
	}
}

void C101Recver::OnRecvKwhLongTime(char* pBuff, int nLength)
{
	ASDU37_SQ0* pAsdu37SQ0 = (ASDU37_SQ0*)pBuff;

	int nDeviceAddr = pAsdu37SQ0->asduAddr.GetAddr();

	int nCount = pAsdu37SQ0->GetItemCount();

	int nKwhStartAddr = m_p101Impl->GetFCIModule()->GetKwhStartAddr();

	for (int i = 0; i < nCount; i++)
	{
		float fValue = pAsdu37SQ0->m_data[i].m_bcr.number;
		int nPointID = pAsdu37SQ0->m_data[i].m_infoaddr.GetAddr();
		//nPointID = nPointID - nKwhStartAddr;

		emit Signal_KwhLongTimeNormal(1, nPointID, fValue);

	}
}

void C101Recver::OnRecvBinarySinglePointTime(char* pBuff, int nLength)
{
	ASDU30_SQ0* pAsdu30SQ0 = (ASDU30_SQ0*)pBuff;

	int nCount = pAsdu30SQ0->GetItemCount();

	int nBinaryStartAddr = m_p101Impl->GetFCIModule()->GetBinaryStartAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu30SQ0->m_data[i].m_infoaddr.GetAddr();
		nPointID = nPointID - nBinaryStartAddr;

		int nValue = pAsdu30SQ0->GetValue(i);//>m_data[i].m_siq;

		CP56Time2a time = pAsdu30SQ0->GetTime(i);
		SIQ siq = pAsdu30SQ0->GetSIQ(i);
		int nQuality = QUALITY_NORMAL;

		if (siq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (siq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (siq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (siq.IV == 1) nQuality |= QUALITY_INVALID;

		emit Signal_SoeBinarySinglePointTime(1, nPointID, nValue, nQuality, time.Dump());
	}
}

void C101Recver::OnRecvBinaryDoublePointTime(char* pBuff, int nLength)
{
	ASDU31_SQ0* pAsdu31SQ0 = (ASDU31_SQ0*)pBuff;

	int nCount = pAsdu31SQ0->GetItemCount();

	//int nBinaryStartAddr = m_p101Impl->GetFCIModule()->GetBinaryStartAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu31SQ0->m_data[i].m_infoaddr.GetAddr();
		//nPointID = nPointID - nBinaryStartAddr;

		int nValue = pAsdu31SQ0->GetValue(i);
		CP56Time2a time = pAsdu31SQ0->m_data[i].m_time;
		DIQ diq = pAsdu31SQ0->GetDIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (diq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (diq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (diq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (diq.IV == 1) nQuality |= QUALITY_INVALID;

		emit Signal_BinaryDoublePointShortTime(1, nPointID, nValue, 1, time.Dump());
	}
}

void C101Recver::OnRecvBinarySinglePointShortTime(char* pBuff, int nLength)
{
	ASDU2_SQ0* pAsdu2SQ0 = (ASDU2_SQ0*)pBuff;

	int nCount = pAsdu2SQ0->GetItemCount();

	//	int nBinaryStartAddr = m_p101Impl->GetFCIModule()->GetBinaryStartAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu2SQ0->m_data[i].m_infoaddr.GetAddr();
		//	nPointID = nPointID - nBinaryStartAddr;

		int nValue = pAsdu2SQ0->GetValue(i);
		CP24Time2a time = pAsdu2SQ0->GetTime(i);

		SIQ siq = pAsdu2SQ0->GetSIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (siq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (siq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (siq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (siq.IV == 1) nQuality |= QUALITY_INVALID;

		emit Signal_BinarySignalPointShortTime(1, nPointID, nValue, 0, time.Dump());
	}
}

void C101Recver::OnRecvBinaryDoublePointShortTime(char* pBuff, int nLength)
{
	ASDU4_SQ0* pAsdu4SQ0 = (ASDU4_SQ0*)pBuff;

	int nDeviceAddr = pAsdu4SQ0->asduAddr.GetAddr();

	int nCount = pAsdu4SQ0->GetItemCount();

	//int nBinaryStartAddr = m_p101Impl->GetFCIModule()->GetBinaryStartAddr();

	for (int i = 0; i < nCount; i++)
	{
		int nPointID = pAsdu4SQ0->m_data[i].m_infoaddr.GetAddr();
		//	nPointID = nPointID - nBinaryStartAddr;

		int nValue = pAsdu4SQ0->GetValue(i);
		CP24Time2a time = pAsdu4SQ0->m_data[i].m_time;

		DIQ diq = pAsdu4SQ0->GetDIQ(i);

		int nQuality = QUALITY_NORMAL;

		if (diq.BL == 1) nQuality |= QUALITY_BLOCKED;
		if (diq.SB == 1) nQuality |= QUALITY_REPLACED;
		if (diq.NT == 1) nQuality |= QUALITY_UNCURRENT;
		if (diq.IV == 1) nQuality |= QUALITY_INVALID;

		emit Signal_BinaryDoublePointShortTime(1, nPointID, nValue, nQuality, time.Dump());

	}
}

