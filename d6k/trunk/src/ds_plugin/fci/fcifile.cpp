#include "fcifile.h"

#include <QFile>
CFciFile* CFciFile::m_nGetInstance = nullptr ;

CFciFile::CFciFile()
{
}


CFciFile::~CFciFile()
{
}

bool CFciFile::ReadPrgFile(const QString& fileName)
{
	Q_ASSERT(!fileName.isEmpty());
	if (fileName.isEmpty())
	{
		return false;
	}

	QFile file(fileName);
	if (!file.exists())
	{
		return false;
	}

	bool bRet=ParaSysInfo(fileName);

	bRet = ParaUartInfo(fileName);

	bRet = ParaGPRSInfo(fileName);

	bRet = ParaGpsInfo(fileName);

	bRet = ParaSmsInfo(fileName);

	bRet = ParaProtocolInfo(fileName);

	bRet = ParaIndAddrTabInfo(fileName);

	bRet = ParaYXInfo(fileName);

	bRet = ParaYCInfo(fileName);	

	bRet = ParaYKInfo(fileName);

	bRet = ParaFixInfo(fileName);

	bRet = ParaFLInfo(fileName);

	bRet = ParaYPInfo(fileName);

	return bRet;
}

void CFciFile::WritePrgFile(const QString& fileName)
{
	Q_ASSERT(!fileName.isEmpty() && fileName.length()>0);
}

bool CFciFile::GetSysParaInfo(SYSPARA_DEF& sysPara)
{
	Q_ASSERT(m_nSysparaNum == 1);

	if (m_nSysparaNum != 1)
	{
		return false;
	}

	sysPara = m_arrSysParas.front();

	Q_ASSERT(sysPara);
	if (sysPara == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetUartInfo(int nIndex, UART_DEF& pUart)
{
	Q_ASSERT(nIndex >= 0 && nIndex <= m_nUartNum);
	if (nIndex < 0 || nIndex > m_nUartNum)
	{
		return false;
	}
	pUart = m_arrUartds.at(nIndex);
	Q_ASSERT(pUart);
	if (pUart == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetGprsInfo(int nIndex, GPRS_DEF& pGprs)
{
	Q_ASSERT(nIndex >= 0 && nIndex <= m_nGprsNum);
	if (nIndex < 0 || nIndex > m_nGprsNum)
	{
		return false;
	}
	pGprs = m_arrGprss.at(nIndex);
	Q_ASSERT(pGprs);
	if (pGprs == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetGpsInfo(int nIndex, GPS_DEF& pGps)
{
	Q_ASSERT(nIndex >= 0 && nIndex <= m_nGpsNum);
	if (nIndex < 0 || nIndex > m_nGpsNum)
	{
		return false;
	}
	pGps = m_arrGpss.at(nIndex);

	Q_ASSERT(pGps);
	if (pGps == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetSmsInfo(int nIndex, SMS_DEF& pSms)
{
	Q_ASSERT(nIndex >= 0 && nIndex <= m_nSmsNum);
	if (nIndex < 0 || nIndex > m_nSmsNum)
	{
		return false;
	}
	pSms = m_arrSmss.at(nIndex);
	Q_ASSERT(pSms);
	if (pSms == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetProtocolInfo(int nIndex, PROTOCOL_DEF& pProtocol)
{
	Q_ASSERT(nIndex >= 0 && nIndex <= m_nProtocolNum);
	if (nIndex < 0 || nIndex > m_nProtocolNum)
	{
		return false;
	}
	pProtocol = m_arrProtocols.at(nIndex);
	Q_ASSERT(pProtocol);
	if (pProtocol == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetIndAddrTabInfo(int nIndex, IND_ADDR_TAB_DEF& pIndAddr)
{
	Q_ASSERT(nIndex >= 0 && nIndex <= m_nIndAddrTabNum);
	if (nIndex < 0 || nIndex > m_nIndAddrTabNum)
	{
		return false;
	}
	pIndAddr = m_arrIndTabs.at(nIndex);
	Q_ASSERT(pIndAddr);
	if (pIndAddr == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetYXInfo(int nIndex, RPT_DEF& pYx)
{
	Q_ASSERT(nIndex >= 0 && nIndex <= m_nYXNum);
	if (nIndex < 0 || nIndex > m_nYXNum)
	{
		return false;
	}
	pYx = m_arrYxs.at(nIndex);
	Q_ASSERT(pYx);
	if (pYx == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetYcInfo(int nIndex, RPT_DEF& pYC)
{
	Q_ASSERT(nIndex > 0 && nIndex <= m_nYcNum);
	if (nIndex == 0 || nIndex > m_nYcNum)
	{
		return false;
	}
	pYC = m_arrYcs.at(nIndex);
	Q_ASSERT(pYC);
	if (pYC == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetYKInfo(int nIndex, RPT_DEF& pYK)
{
	Q_ASSERT(nIndex > 0 && nIndex <= m_nYkNum);
	if (nIndex == 0 || nIndex > m_nYkNum)
	{
		return false;
	}
	pYK = m_arrYks.at(nIndex);
	Q_ASSERT(pYK);
	if (pYK == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetFixInfo(int nIndex, RPT_DEF& pFix)
{
	Q_ASSERT(nIndex > 0 && nIndex <= m_nFixNum);
	if (nIndex == 0 || nIndex > m_nFixNum)
	{
		return false;
	}
	pFix = m_arrFixs.at(nIndex);
	Q_ASSERT(pFix);
	if (pFix == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetFFLInfo(int nIndex, RPT_DEF& pFl)
{
	Q_ASSERT(nIndex > 0 && nIndex <= m_nFlNum);
	if (nIndex == 0 || nIndex > m_nFlNum)
	{
		return false;
	}
	pFl = m_arrFfls.at(nIndex);
	Q_ASSERT(pFl);
	if (pFl == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetYpInfo(int nIndex, RPT_DEF& pFp)
{
	Q_ASSERT(nIndex > 0 && nIndex <= m_nYPnum);
	if (nIndex == 0 || nIndex > m_nYPnum)
	{
		return false;
	}
	pFp = m_arrYps.at(nIndex);
	Q_ASSERT(pFp);
	if (pFp == nullptr)
	{
		return false;
	}
	return true;
}

bool CFciFile::GetUartArryInfo(std::vector<UART_DEF>& vecs)
{
	Q_ASSERT(m_arrUartds.size());
	if (m_arrUartds.size() <= 0)
	{
		return false;
	}
	vecs = m_arrUartds;
	return true;
}

bool CFciFile::GetGprsArryInfo(std::vector<GPRS_DEF>& vecs)
{
	Q_ASSERT(m_arrGprss.size());
	if (m_arrGprss.size() <= 0)
	{
		return false;
	}
	vecs = m_arrGprss;
	return true;
}

bool CFciFile::GetGpsArryInfo(std::vector<GPS_DEF>& vecs)
{
	Q_ASSERT(m_arrGpss.size());
	if (m_arrGpss.size() <= 0)
	{
		return false;
	}
	vecs = m_arrGpss;
	return true;
}

bool CFciFile::GetSmsArryInfo(std::vector<SMS_DEF>& vecs)
{
	Q_ASSERT(m_arrSmss.size());
	if (m_arrSmss.size() <= 0)
	{
		return false;
	}
	vecs = m_arrSmss;
	return true;
}

bool CFciFile::GetProtocolArryInfo(std::vector<PROTOCOL_DEF>& vecs)
{
	Q_ASSERT(m_arrProtocols.size());
	if (m_arrProtocols.size() <= 0)
	{
		return false;
	}
	vecs = m_arrProtocols;
	return true;
}

bool CFciFile::GetIndAddrTabArryInfo(std::vector<IND_ADDR_TAB_DEF>& vecs)
{
	Q_ASSERT(m_arrIndTabs.size());
	if (m_arrIndTabs.size() <= 0)
	{
		return false;
	}
	vecs = m_arrIndTabs;
	return true;
}

bool CFciFile::GetYXArryInfo(std::vector<RPT_DEF>& vecs)
{
	Q_ASSERT(m_arrYxs.size());
	if (m_arrYxs.size() <= 0)
	{
		return false;
	}
	vecs = m_arrYxs;
	return true;
}

bool CFciFile::GetYcArryInfo(std::vector<RPT_DEF>& vecs)
{
	Q_ASSERT(m_arrYcs.size());
	if (m_arrYcs.size() <= 0)
	{
		return false;
	}
	vecs = m_arrYcs;
	return true;
}

bool CFciFile::GetYKArryInfo(std::vector<RPT_DEF>& vecs)
{
	Q_ASSERT(m_arrYks.size());
	if (m_arrYks.size() <= 0)
	{
		return false;
	}
	vecs = m_arrYks;
	return true;
}

bool CFciFile::GetFixArryInfo(std::vector<RPT_DEF>& vecs)
{
	Q_ASSERT(m_arrFixs.size());
	if (m_arrFixs.size() <= 0)
	{
		return false;
	}
	vecs = m_arrFixs;
	return true;
}

bool CFciFile::GetFFLArryInfo(std::vector<RPT_DEF>& vecs)
{
	Q_ASSERT(m_arrFfls.size());
	if (m_arrFfls.size() <= 0)
	{
		return false;
	}
	vecs = m_arrFfls;
	return true;
}

bool CFciFile::GetYpArryInfo(std::vector<RPT_DEF>& vecs)
{
	Q_ASSERT(m_arrYps.size());
	if (m_arrYps.size() <= 0)
	{
		return false;
	}
	vecs = m_arrYps;
	return true;
}

bool CFciFile::ParaSysInfo(const QString& fileName)
{
	QSettings settings(fileName, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("SYSPARA");
	int nNum = settings.value("num").toString().remove(';').toInt();
	
	m_nSysparaNum = 1;
	//记录序号和对应的内容

	QMap<int, QString>mapKeys;
	for (int i = 1; i <= nNum; ++i)
	{
		QString str =settings.value(QString("Para%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	SYSPARA_DEF pSys = std::make_shared<SYSPARA>();

	pSys->StationAddr = mapKeys[1].section(',', 0, 0).toUInt();
	pSys->StationAddrDesc=mapKeys[1].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->IsSaveSoe = mapKeys[2].section(',', 0, 0).toUInt();
	pSys->IsSaveSoeDesc=mapKeys[2].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->SoeNum = mapKeys[3].section(',', 0, 0).toUInt();
	pSys->SoeNumDesc=mapKeys[3].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->IsSaveYCData = mapKeys[4].section(',', 0, 0).toUInt();
	pSys->IsSaveYCDataDesc=mapKeys[4].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->SaveYCFRQ = mapKeys[5].section(',', 0, 0).toUInt();
	pSys->SaveYCFRQDesc=mapKeys[5].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->SaveYcDays = mapKeys[6].section(',', 0, 0).toUInt();
	pSys->SaveYcDaysDesc=mapKeys[6].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->DataType = mapKeys[7].section(',', 0, 0).toUInt();
	pSys->DataTypeDesc=mapKeys[7].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->UpLoadYCFRQ = mapKeys[8].section(',', 0, 0).toUInt();
	pSys->UpLoadYCFRQDesc= mapKeys[8].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->CallYCFRQ = mapKeys[9].section(',', 0, 0).toUInt();
	pSys->CallYCFRQDesc=mapKeys[9].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->SyncFRQ = mapKeys[10].section(',', 0, 0).toUInt();
	pSys->SyncFRQDesc = mapKeys[10].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->GPRSTimeOut = mapKeys[11].section(',', 0, 0).toUInt();
	pSys->GPRSTimeOutDesc= mapKeys[11].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->VBatLimit = mapKeys[12].section(',', 0, 0).toUInt();
	pSys->VBatLimitDesc=mapKeys[12].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->LbFileNum = mapKeys[13].section(',', 0, 0).toUInt();
	pSys->LbFileNumDesc= mapKeys[13].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->ComNum = mapKeys[14].section(',', 0, 0).toUInt();
	pSys->ComNumDesc= mapKeys[14].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->NetNum = mapKeys[15].section(',', 0, 0).toUInt();
	pSys->NetNumDesc= mapKeys[15].section(',', 1, 1).simplified().toStdString().c_str();

	pSys->IndNum = mapKeys[16].section(',', 0, 0).toUInt();
	pSys->IndNumDesc= mapKeys[16].section(',', 1, 1).simplified().toStdString().c_str();

	m_arrSysParas.push_back(pSys);

	return true;
}

bool CFciFile::ParaUartInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("UART");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nUartNum = nNum;

	QMap<int, QString> mapKeys;

	UART_DEF pUart = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("Para%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pUart = std::make_shared<UART>();
		pUart->OccNO = var;
		QString szUart = mapKeys[var].section(';', 0, 0);
		pUart->ComID   = szUart.section(',',0,0).simplified().toInt();
		pUart->UseType = szUart.section(',',1,1).simplified().toInt();
		pUart->Band    = szUart.section(',',2,2).simplified().toUInt();
		pUart->ParityBit = szUart.section(',',3, 3).simplified().toInt();
		pUart->DataBit = szUart.section(',', 4, 4).simplified().toInt();
		pUart->StopBit = szUart.section(',', 5, 5).simplified().toInt();
		pUart->ProtocolID = szUart.section(',', 6, 6).simplified().toInt();

		m_arrUartds.push_back(pUart);
	}
	return true;
}

bool CFciFile::ParaGPRSInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("GPRS");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nGprsNum = nNum;

	QMap<int, QString> mapKeys;

	GPRS_DEF pGprs = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("Para%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	settings.endGroup();

	for (auto var : mapKeys.keys())
	{
		pGprs = std::make_shared<GPRS>();
		QString szGprs = mapKeys[var].section(';', 0, 0);

		pGprs->OccNO = var;
		pGprs->WorkMode = szGprs.section(',', 0, 0).toInt();

		QString szIPA = szGprs.section(',', 1, 1).simplified();
		if (szIPA.contains(':'))
		{
			pGprs->IPA = szIPA.section(':', 0, 0).simplified();
			pGprs->PortA = szIPA.section(':', 1, 1).simplified().toUInt();
		}
		
		QString szIPB = szGprs.section(',', 2, 2).simplified();
		if (szIPB.contains(':'))
		{
			pGprs->IPB = szIPB.section(':', 0, 0).simplified();
			pGprs->PortB = szIPB.section(':', 1, 1).simplified().toInt();
		}
		
		pGprs->APN= szGprs.section(',', 3, 3).simplified();

		pGprs->UserName = szGprs.section(',', 4, 4).simplified();

		pGprs->UserPwd = szGprs.section(',', 5, 5).simplified();
	
		pGprs->ProtocolID = szGprs.section(',', 6, 6).simplified().toInt();

		m_arrGprss.push_back(pGprs);
	}

	return true;
}

bool CFciFile::ParaGpsInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("GPS");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nGpsNum = nNum;

	QMap<int, QString> mapKeys;

	GPS_DEF pGps = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("Para%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pGps = std::make_shared<GPS>();
		QString szGps = mapKeys[var].section(';', 0, 0);
		pGps->OccNO = var;
		pGps->Enable = szGps.section(',', 0, 0).toInt();
		pGps->SysInterval = szGps.section(',', 1, 1).toUInt();
		pGps->MaxRecvTime = szGps.section(',', 2, 2).toUInt();
		
		m_arrGpss.push_back(pGps);
	}

	return true;
}

bool CFciFile::ParaSmsInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("SMS");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nSmsNum = nNum;

	QMap<int, QString> mapKeys;

	SMS_DEF pSms = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("Para%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pSms = std::make_shared<SMS>();
		QString szSms = mapKeys[var].section(';', 0, 0);
		pSms->OccNO = var;
		pSms->StatationNum = szSms.section(',', 0, 0).simplified();
		pSms->StatationNumDesc = szSms.section(',', 1, 1).simplified();
		
		m_arrSmss.push_back(pSms);
	}

	return true;
}

bool CFciFile::ParaProtocolInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("PROTCOL");

	int nNum = settings.value("num").toString().remove(';').toInt();
	m_nProtocolNum = nNum;
	QMap<int, QString> mapKeys;

	PROTOCOL_DEF pProtocol = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("Para%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pProtocol = std::make_shared<PROTOCOL>();

		QString szProtcol = mapKeys[var].section(';', 0, 0);
		pProtocol->OccNO = var;
		pProtocol->ProtocolName = szProtcol.section(',', 0, 0);
		pProtocol->ProtocolID = szProtcol.section(',', 1, 1).toUInt();
		pProtocol->LinkAddrLen = szProtcol.section(',', 2, 2).toUInt();
		pProtocol->CotLen = szProtcol.section(',', 3, 3).toUInt();
		pProtocol->CommLen = szProtcol.section(',', 4, 4).toUInt();
		pProtocol->InfoAddrLen = szProtcol.section(',', 5, 5).toUInt();
		pProtocol->LinkAddr = szProtcol.section(',', 6, 6).toUInt();
		pProtocol->CommonAddr = szProtcol.section(',', 7, 7).toUInt();
		pProtocol->ResendTimes = szProtcol.section(',', 8, 8).toUInt();
		pProtocol->T1 = szProtcol.section(',', 9, 9).toUInt();
		pProtocol->T2 = szProtcol.section(',', 10, 10).toUInt();
		pProtocol->T3 = szProtcol.section(',', 11, 11).toUInt();
		pProtocol->T4 = szProtcol.section(',', 12, 12).toUInt();

		m_arrProtocols.push_back(pProtocol);
	}

	return true;
}

bool CFciFile::ParaIndAddrTabInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("IND_ADDR_TAB");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nIndAddrTabNum = nNum;

	QMap<int, QString> mapKeys;

	IND_ADDR_TAB_DEF pIndTab = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("Para%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pIndTab = std::make_shared<IND_ADDR_TAB>();
		QString szIndTab = mapKeys[var].section(';', 0, 0);
		pIndTab->OccNO = var;

		pIndTab->GrpNo = szIndTab.section(',', 0, 0).toInt();
		pIndTab->Address1 = szIndTab.section(',', 1, 1).simplified();
		pIndTab->Address2 = szIndTab.section(',', 2, 2).simplified();
		pIndTab->Address3 = szIndTab.section(',', 3, 3).simplified();
		pIndTab->Desription = szIndTab.section(',', 4, 4).simplified();

		m_arrIndTabs.push_back(pIndTab);
	}

	return true;
}

bool CFciFile::ParaYCInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("HUJI_YC");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nYcNum = nNum;

	QMap<int, QString> mapKeys;

	RPT_DEF pYC = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("point%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pYC = std::make_shared<RPT>();

		QString szYC = mapKeys[var].section(';', 0, 0);
		pYC->OccNO = var;
		pYC->InnerAddr = szYC.section(',', 0, 0).simplified();
		pYC->Address = szYC.section(',', 1, 1).simplified();
		pYC->MinValue = szYC.section(',', 2, 2).toDouble();
		pYC->MaxValue = szYC.section(',', 3, 3).toDouble();
		pYC->KRate = szYC.section(',', 4, 4).toDouble();
		pYC->Unit = szYC.section(',', 5, 5).simplified();
		pYC->Desccription = szYC.section(',', 6, 6).simplified();

		m_arrYcs.push_back(pYC);
	}
	return true;
}

bool CFciFile::ParaYXInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("HUJI_YX");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nYXNum = nNum;

	QMap<int, QString> mapKeys;

	RPT_DEF pYX = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("point%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pYX = std::make_shared<RPT >();

		QString szYX = mapKeys[var].section(';', 0, 0);
		pYX->OccNO = var;
		pYX->InnerAddr = szYX.section(',', 0, 0).simplified();
		pYX->Address = szYX.section(',', 1, 1).simplified();
		pYX->Name = szYX.section(',', 2, 2).simplified();
		pYX->Desccription = szYX.section(',', 3, 3).simplified();

		m_arrYxs.push_back(pYX);
	}

	return true;
}

bool CFciFile::ParaYKInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("HUJI_YK");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nYkNum = nNum;

	QMap<int, QString> mapKeys;

	RPT_DEF pYK = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("point%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pYK = std::make_shared<RPT>();

		QString szYX = mapKeys[var].section(';', 0, 0);
		pYK->OccNO = var;
		pYK->InnerAddr = szYX.section(',', 0, 0).simplified();
		pYK->Address = szYX.section(',', 1, 1).simplified();
		pYK->Desccription = szYX.section(',', 2, 2).simplified();

		m_arrYks.push_back(pYK);
	}

	return true;
}

bool CFciFile::ParaFixInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("HUJI_FIX");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nFixNum = nNum;

	QMap<int, QString> mapKeys;

	RPT_DEF pFix = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("point%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pFix = std::make_shared<RPT >();

		QString szFix = mapKeys[var].section(';', 0, 0);
		pFix->OccNO = var;
		pFix->InnerAddr = szFix.section(',', 0, 0).simplified();
		pFix->Address = szFix.section(',', 1, 1).simplified();
		pFix->Desccription = szFix.section(',', 2, 2).simplified();

		m_arrFixs.push_back(pFix);
	}

	return true;
}

bool CFciFile::ParaFLInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("HUJI_FL");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nFlNum = nNum;

	QMap<int, QString> mapKeys;

	RPT_DEF pFl = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("point%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pFl = std::make_shared<RPT >();

		QString szFl = mapKeys[var].section(';', 0, 0);
		pFl->OccNO = var;
		pFl->InnerAddr = szFl.section(',', 0, 0).simplified();
		pFl->Address = szFl.section(',', 1, 1).simplified();
		pFl->Desccription = szFl.section(',', 2, 2).simplified();

		m_arrFfls.push_back(pFl);
	}
	return true;
}

bool CFciFile::ParaYPInfo(const QString& filename)
{
	QSettings settings(filename, QSettings::IniFormat);
	settings.setIniCodec("GB2312");
	settings.beginGroup("HUJI_YP");

	int nNum = settings.value("num").toString().remove(';').toInt();

	m_nYPnum = nNum;

	QMap<int, QString> mapKeys;

	RPT_DEF pYp = nullptr;

	for (int i = 1; i <= nNum; ++i)
	{
		QString str = settings.value(QString("point%1").arg(i)).toString();
		str = str.section(';', 0, 0);
		mapKeys.insert(i, str);
	}

	for (auto var : mapKeys.keys())
	{
		pYp = std::make_shared<RPT> ();

		QString szFl = mapKeys[var].section(';', 0, 0);
		pYp->OccNO = var;
		pYp->InnerAddr = szFl.section(',', 0, 0).simplified();
		pYp->Address = szFl.section(',', 1, 1).simplified();
		pYp->Desccription = szFl.section(',', 2, 2).simplified();

		m_arrYps.push_back(pYp);
	}

	return true;
}
