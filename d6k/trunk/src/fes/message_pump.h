
#ifndef FES_MSGPUMP_H
#define FES_MSGPUMP_H

#include <memory>
  

class CMessageExecutor;
class CDbSvc;

class CMessagePump
{
public:
	CMessagePump(void);
	explicit CMessagePump(int nRecvMailBoxID);
	~CMessagePump(void);
public:

	bool RunInDbSvc(CDbSvc *pDbSvc);

public:
	 

private:
	//! 消息执行器
	std::shared_ptr<CMessageExecutor> m_pExecutor;

	//! DB_SVC的邮箱ID
	int m_nDBSvcMailBoxID; 
};


#endif // FES_MSGPUMP_H

