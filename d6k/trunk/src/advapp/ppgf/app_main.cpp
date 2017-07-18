/*! @file app_main.cpp
<PRE>
********************************************************************************
ģ����       :
�ļ���       :  app_main.cpp
�ļ�ʵ�ֹ��� :  ������
����         :  LiJin
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         :  <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief   ������
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
	** \details ����Ӧ��ģ��
	** \param pszName ����ʾ�����ƣ����Ϊ�յĻ������bin�������scada.xml�������ж�ȡ����ʵ������
	** \param nAppOccNo Ӧ�õ�OccNo�����Ϊ0��������ͬ
	** \param nExtraArgc �������
	** \param pszExtraArgv �������
	** \return SCDAPP int 
	** \author LiJin 
	** \date 2017��3��24�� 
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

			// LOG �Ǽ�һ��


			return 0;
		}
		// ��������
		pPpgf->Run();

		return 1;
	}
	/*! \fn SCDAPP int StopScadaApp(const char * pszName, int32u nAppOccNo)
	********************************************************************************************************* 
	** \brief StopScadaApp 
	** \details �˳�Ӧ��ģ��
	** \param pszName 
	** \param nAppOccNo 
	** \return  int 
	** \author LiJin 
	** \date 2017��3��23�� 
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
	//����ʷ���ȡģ����ֵ

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
