
#ifndef SCADA_APP_API_H
#define SCADA_APP_API_H

#include "../datatypes.h"

#include <QtCore/QtGlobal>

#if defined(SCADA_APP)
#  define SCDAPP Q_DECL_EXPORT
#else
#  define SCDAPP Q_DECL_IMPORT
#endif


extern "C"
{
	// APP必须实现的接口  返回0 为成功 非0值为错误码
	SCDAPP int StartScadaApp(const char * pszName, int32u nAppOccNo, int nExtraArgc, char *pszExtraArgv[]);

	SCDAPP int StopScadaApp(const char * pszName, int32u nAppOccNo);
 
}


#endif // APP_API_H

