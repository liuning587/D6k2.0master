
#include "power_api.h"

#include "power_prediction.h"

extern "C"
{
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

		CPowerPrediction* pPowerPredict = new CPowerPrediction(nAppOccNo);

		if (!pPowerPredict->Init(pszName))
		{
			delete pPowerPredict;
			pPowerPredict = nullptr;

			return 0;
		}

		bool bRet = false;

		bRet = CExtSvcContainer::GetInstance()->RegisterService(pPowerPredict);
		if (bRet == false)
		{
			delete pPowerPredict;
			pPowerPredict = nullptr;

			// LOG 登记一下

			return 0;
		}
		// 启动起来
		pPowerPredict->Run();

		return 1;
	}
}

SCDAPP int StopScadaApp(const char * pszName, int32u nAppOccNo)
{
	Q_ASSERT(pszName);
	if (pszName == 0)
	{
		return 0;
	}

	CExtService* pSvc = CExtSvcContainer::GetInstance()->FindService(nAppOccNo);
	Q_ASSERT(pSvc);
	if (pSvc == nullptr)
	{
		return 0;
	}

	CPowerPrediction* pPowerPredict = dynamic_cast<CPowerPrediction*>(pSvc);
	pPowerPredict->Shutdown();

	delete pPowerPredict;

	return 1;
}
