
#ifndef IO_DRIVER_H
#define IO_DRIVER_H

#include "fesdatadef.h"

#include <QtCore/QtGlobal>

#if defined(IO_DRIVER)
#  define IODRV Q_DECL_EXPORT
#else
#  define IODRV Q_DECL_IMPORT
#endif


extern "C"
{
	//IO 驱动必须实现的接口
	IODRV int StartIoDriver(const char * pszName,int32u nNodeOccNo, int32u nChannelOccNo);

	IODRV int StopIoDriver(const char * pszName, int32u nNodeOccNo, int32u nChannelOccNo);

	// 配置驱动参数
	IODRV char * ConfigDriverParam(const char * szOldConfData);
	// 默认的通道配置
	IODRV const char * GetChannelDefaultParam();
	IODRV const char * GetChannelDefaultExtParam();
 
	// 指定通道的配置参数
	IODRV const char * GetChannelParam(const char * pszTagName);
	IODRV const char * GetChannelExtParam(const char * pszTagName);


	IODRV const char * GetDeviceDefaultlParam(const char * pszAddr);
	IODRV const char * GetDeviceDefaultExtParam(const char * pszAddr);

	// 测试装置的通断，0 成功，非0，错误代码
	IODRV int TestDevice(const char * pszAddr);


}


#endif // IO_DRIVER_H

