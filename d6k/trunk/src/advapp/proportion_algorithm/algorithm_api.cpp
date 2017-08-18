
#include "algorithm_api.h"
#include "algorithm_factory.h"
extern "C"
{
	PROPORTION_ALGORITHM_EXPORT int StartAlgorithm(std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo)
	{
		if (!CAlgorithmFactory::GetInstance().Init(m_vecStationInfo))
		{
			// log
			return 0;
		}

		bool bRet = false;

		CAlgorithmFactory::GetInstance().Run();

		return 1;
	}

	PROPORTION_ALGORITHM_EXPORT void StopAlgorithm()
	{
		CAlgorithmFactory::GetInstance().Shutdown();
	}
}

