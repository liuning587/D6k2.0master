/*! @file ext_service.h
<PRE>
********************************************************************************
模块名       :
文件名       :  ext_service.h
文件实现功能 :  扩展服务入口
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   扩展服务入口
*  @author  LiJin
*  @version 1.0
*  @date    2017.03.24
*******************************************************************************/
#ifndef EXT_SVC_H
#define EXT_SVC_H

#include "datatypes.h"
#include "singleton.h"
#include <vector>
#include <memory>
 
class CExtService
{
public:
	CExtService(void);
	explicit CExtService(int32u nOccNo);

	~CExtService(void);
public:
	virtual bool Initalize(const char * pszProjectName);

	virtual void Run();

	virtual void Shutdown();

	void LogMsg(const char* logMsg, int nLevel);

	int32u GetOccNo() const
	{
		return m_nOccNo;
	}
protected:
	//! APP的排行号
	int32u m_nOccNo;
	//! APP的标签
	std::string m_szTagName;

private:
	 
};

/*! \struct  CExtSvcContainer
*   \brief   扩展服务的容器，一个进程内可能存在多个服务 */
class CExtSvcContainer
{
public:
	static CExtSvcContainer *GetInstance()
	{
		return Singleton<CExtSvcContainer, DefaultSingletonTraits<CExtSvcContainer>>::instance();
	}
public:
	bool RegisterService(CExtService *pSvc);

	bool UnRegisterService(int32u nOccNo);

	CExtService * FindService(int32u nOccNo);
	//! 启动 
	bool Start(int32u nOccNo);
	//! 停止 
	bool Stop(int32u nOccNo);
private:
	CExtSvcContainer();
	~CExtSvcContainer();
private:
	friend DefaultSingletonTraits<CExtSvcContainer>;
	std::vector<CExtService*> m_arrExtSvcs;
};

#endif // EXT_SVC_H



/** @}*/

