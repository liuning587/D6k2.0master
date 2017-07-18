/*! @file app_main.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  app_main.cpp
文件实现功能 :  插件入口
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   插件入口
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.23
*******************************************************************************/

#include "scadaapi/scdsvcapi.h"
#include "scadaapi/scdapp_api.h"
#include "scadaapi/scdapp_def.h"

#include "app_main.h"

#include <QObject> 
#include <QDateTime> 

extern "C"
{
	/*! \fn SCDAPP int StartScadaApp(const char * pszName, int32u nAppOccNo, int nExtraArgc,  char *pszExtraArgv[])
	********************************************************************************************************* 
	** \brief StartScadaApp 
	** \details 启动应用模块
	** \param pszName 工程示例名称，如果为空的话，则从bin下面加载scada.xml，从其中读取工程实例名称
	** \param nAppOccNo 应用的OccNo，如果为0，则与上同
	** \param nExtraArgc 额外参数
	** \param pszExtraArgv 额外参数
	** \return SCDAPP int 
	** \author LiJin 
	** \date 2017年3月24日 
	** \note 
	********************************************************************************************************/
	SCDAPP int StartScadaApp(const char * pszName, int32u nAppOccNo, int nExtraArgc, char *pszExtraArgv[])
	{
		Q_ASSERT(pszName);
		if (pszName == nullptr)
		{
			return 0;
		}

		Q_ASSERT(nAppOccNo != INVALID_OCCNO && nAppOccNo <= MAX_OCCNO);
		if (nAppOccNo == INVALID_OCCNO || nAppOccNo > MAX_OCCNO)
		{
			return 0;
		}

		CPpgf *pPpgf = new CPpgf(nAppOccNo);

		if (!pPpgf->Initalize(pszName))
		{
			delete pPpgf;
			pPpgf = nullptr;
			return 0;
		}

		bool bRet = false;

		bRet = CExtSvcContainer::GetInstance()->RegisterService(pPpgf);
		if (bRet == false)
		{
			delete pPpgf;
			pPpgf = nullptr;

			// LOG 登记一下


			return 0;
		}
		// 启动起来
		pPpgf->Run();

		return 1;
	}
	/*! \fn SCDAPP int StopScadaApp(const char * pszName, int32u nAppOccNo)
	********************************************************************************************************* 
	** \brief StopScadaApp 
	** \details 退出应用模块
	** \param pszName 
	** \param nAppOccNo 
	** \return  int 
	** \author LiJin 
	** \date 2017年3月23日 
	** \note 
	********************************************************************************************************/
	SCDAPP int StopScadaApp(const char * pszName, int32u nAppOccNo)
	{
		Q_ASSERT(pszName);
		if (pszName == 0)
		{
			return 0;
		}

		CExtService *pSvc = CExtSvcContainer::GetInstance()->FindService(nAppOccNo);
		Q_ASSERT(pSvc);
		if (pSvc == nullptr)
			return 0;

		pSvc->Shutdown();


		return 1;
	}
}


CPpgf::CPpgf()
{

}

CPpgf::CPpgf(int32u nOccNo):CExtService(nOccNo)
{

}


CPpgf::~CPpgf()
{


}



void CPpgf::GetDbData()
{
	//从历史库获取模拟量值

}

void CPpgf::GetRealData()
{
	Q_ASSERT(m_pPvStation);
	if (m_pPvStation)
	{
		m_pPvStation->GetRealTimeData();
	}
}

void CPpgf::ReadXml(const QString& szFilePath)
{
	Q_ASSERT(m_pReadXml);
	if (m_pReadXml)
	{
		m_pReadXml->ReadGroupInfo(szFilePath);
	}
	
}




/** @}*/
