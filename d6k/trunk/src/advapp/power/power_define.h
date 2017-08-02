#pragma once

#ifndef POWER_DEFINE_H
#define POWER_DEFINE_H

#include <string>

#define LOG_FILENAME  "power"

#define HISTORY_DATA_SAVE_PERIOD		 5 * 1000

#define PREDICT_DATA_SAVE_PERIOD		 15 * 1000

#define WEATHER_FILE_DATA_SAVE_PERIOD    1 * 1000

#define APP_NAME	"POWER"

#define POWER_PREDICT_PATH_NAME "/powerpredict/station.xml"

struct DATA_INFO
{
	int nStationId;
	int nDeviceId;
	int nPointId;
	std::string strTagName;
};

#endif // POWER_DEFINE_H

