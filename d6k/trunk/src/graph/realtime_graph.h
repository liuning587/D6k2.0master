
#ifndef REAL_TIME_GRAPH_H
#define REAL_TIME_GRAPH_H

#include "base_module.h" 

class CGraphFile;

class CRealTimeTask : public CBaseModule
{
public:
	CRealTimeTask();
	explicit CRealTimeTask(CGraphFile* pFile);
	~CRealTimeTask();
 
public:
	bool Initialize(CGraphFile* pFile, unsigned int nMode);
	virtual bool Initialize(const char*, unsigned int) { return true; }
	virtual void Run() ;
	virtual void Shutdown() ;

protected:
	virtual void MainLoop(void *pImpl);

private:
	CGraphFile* m_pGraphFile;
};

  

#endif // REAL_TIME_GRAPH_H


