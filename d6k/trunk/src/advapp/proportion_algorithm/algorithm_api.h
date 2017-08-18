
#include "proportion_algorithm.h"

#include <vector>
#include <memory>
#include "station_info.h"

#include "algorithm_define.h"

extern "C"
{
	// �����㷨ģ��
	PROPORTION_ALGORITHM_EXPORT int StartAlgorithm(std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo);

	// ֹͣ�㷨ģ��
	PROPORTION_ALGORITHM_EXPORT void StopAlgorithm();

}
