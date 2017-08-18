
#include "proportion_algorithm.h"

#include <vector>
#include <memory>
#include "station_info.h"

#include "algorithm_define.h"

extern "C"
{
	// 启动算法模块
	PROPORTION_ALGORITHM_EXPORT int StartAlgorithm(std::vector<std::shared_ptr<CStationInfo>> m_vecStationInfo);

	// 停止算法模块
	PROPORTION_ALGORITHM_EXPORT void StopAlgorithm();

}
