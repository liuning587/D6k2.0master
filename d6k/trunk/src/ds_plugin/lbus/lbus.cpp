
#if _MSC_VER >= 1600
#include "vld.h"
#endif

#include "lbus_module.h"
#define MODULE_LIB
#include "devicestudio/module_export.h"
#include "devicestudio/imainwindow.h"
#include "devicestudio/main_module.h"
#include "devicestudio/treewidget.h" 

#if 0
#include <QStandardItemModel>
#include <QModelIndex>
#include <QDebug>
#include <QMenu>
#endif


static CLowBusModule *s_ptrModule = nullptr;

extern "C" DS_EXPORT IPluginModule *CreateModule()
{
	s_ptrModule = new CLowBusModule;
	return s_ptrModule;
//	return  s_ptrModule.get();
}

bool LogString(const char *szLogTxt, int nLevel)
{
	if (s_ptrModule == nullptr)
		return false;
	 
	return s_ptrModule->LogString("LBUS",  szLogTxt, nLevel);
}

bool CommMsgLog( const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc)
{
	if (s_ptrModule == nullptr)
		return false;

	return s_ptrModule->CommMsgLog("LBUS", pMsg, nMsgLen,nMsgType,pMsgSubType,pSource,pDesc);
}
 