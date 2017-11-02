
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
	//IO ��������ʵ�ֵĽӿ�
	IODRV int StartIoDriver(const char * pszName,int32u nNodeOccNo, int32u nChannelOccNo);

	IODRV int StopIoDriver(const char * pszName, int32u nNodeOccNo, int32u nChannelOccNo);

	// ������������
	IODRV char * ConfigDriverParam(const char * szOldConfData);
	// Ĭ�ϵ�ͨ������
	IODRV const char * GetChannelDefaultParam();
	IODRV const char * GetChannelDefaultExtParam();
 
	// ָ��ͨ�������ò���
	IODRV const char * GetChannelParam(const char * pszTagName);
	IODRV const char * GetChannelExtParam(const char * pszTagName);


	IODRV const char * GetDeviceDefaultlParam(const char * pszAddr);
	IODRV const char * GetDeviceDefaultExtParam(const char * pszAddr);

	// ����װ�õ�ͨ�ϣ�0 �ɹ�����0���������
	IODRV int TestDevice(const char * pszAddr);


}


#endif // IO_DRIVER_H

