/*! @file dbg_svc.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  dbg_svc.cpp
文件实现功能 :  调试代理
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   调试代理
*  @author  LiJin
*  @version 1.0
*  @date    2016.09.01
*******************************************************************************/
#include "dbg_svc.h"
#include "log/log.h"
#include <QObject> 
#include <QString> 
#include "libzmq/zmq.h"
// #include "fesapi\fesapi.h"
// #include "mail\mail.h"

CDbgSvc::CDbgSvc(CScadaSvc* pServer, const std::string & szMailBoxName, int &nMailBoxID) : CBaseModule(pServer, szMailBoxName, nMailBoxID)
{
	m_bStopFlag = false;
}

CDbgSvc::~CDbgSvc(void)
{
	DestroyLog("SCADA");
}

/*! \fn bool CDbgSvc::Initialize(RUN_MODE mode)
********************************************************************************************************* 
** \brief CDbgSvc::Initialize 
** \details 初始化
** \return bool 
** \author LiJin 
** \date 2016年9月1日 
** \note  
********************************************************************************************************/
bool CDbgSvc::Initialize(const char *pszDataPath, unsigned int nMode)
{
// 	if (Mail::CreateMailBox(SMKEY_MAIL_DBGSVR)==K_FALSE)
// 		return false;

//	StartModule();
	// 第一步先启动log模块
	CreateLog("SCADA");

	QString szLog;
	if (pszDataPath && strlen((pszDataPath)))
	{
		szLog = QString(QObject::tr("Start project [%1]...")).arg(pszDataPath);
	}
	else
	{
		szLog = QObject::tr("Start project...");
	}
	
	LogMsg(szLog.toStdString().c_str(), 0);

	return true;
}

void CDbgSvc::Run()
{
 
}

void CDbgSvc::Shutdown()
{
 
}



/** @}*/
