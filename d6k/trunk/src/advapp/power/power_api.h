#pragma once

#include "scadaapi/scdsvcapi.h"
#include "scadaapi/scdapp_api.h"
#include "scadaapi/scdapp_def.h"

extern "C"
{

	SCDAPP int StartScadaApp(const char * pszName, int32u nAppOccNo, int nExtraArgc, char *pszExtraArgv[]);

	SCDAPP int StopScadaApp(const char * pszName, int32u nAppOccNo);
}