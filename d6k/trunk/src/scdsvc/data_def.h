#ifndef _DATA_DEF_H
#define _DATA_DEF_H
#include "fesapi/fesdatadef.h"
#include <QObject>
#include <memory>
#include <vector>

struct  NODE;
class CNodeGroup;
class CNodeGroupMgr;
typedef std::shared_ptr<NODE> NODE_DEF;
typedef std::shared_ptr<CNodeGroup> NODE_GROUP_DEF;
typedef std::shared_ptr<CNodeGroupMgr> NODE_GRP_MGR_DEF;

#define MAGIC_HEAD 0X4C49

enum  NODE_ENUM
{
	MAX_NAME_LEN=256,
	MAX_MEM_LENGTH=512,
	MAX_PAD_LENGTH=12,
};

struct NODE_MEM
{
	int32u OccNo;                           //�ڵ�OccNo��
	int32u Type;                            //�ڵ�����
	char NodeTagName[MAX_NAME_LEN];      //�ڵ�tagname
	int32u  NodeOffSet;                     //�ڵ�ƫ��
	int32u  NodeSize;                       //�ڵ��ڴ��С
};


struct NODE_APP_MAGIC                   
{
	int32u MagicNum1;
	int32u MagicNum2;
	int32u nCount;                          //APP ���� ��Ŀ
	int32u nOccNo;
	int32u ShmSize;                         //�ڵ�ƫ��
};

struct NODE_SCD_MAGIC
{
	int32u MagicNum1;
	int32u MagicNum2;
	int32u NodeServerCount;                 //server�ڵ���Ŀ
	int32u NodeFesCount;                    //fes�ڵ���Ŀ
	int32u NodeClientCount;				    //client�ڵ���Ŀ
	int32u AllNodeCount;                    //���нڵ���Ŀ
	int32u ShmSize;                         //�ڵ�ƫ��
};

struct APP_INFO
{
	int nIndex;
	char appName[MAX_NAME_LEN];
	char loadMode[MAX_NAME_LEN];
	char agru[MAX_NAME_LEN];
	char desc[MAX_NAME_LEN];
	int32u nDescOccNo;
};

// ������������
enum
{
	//INVALID_TYPE = 0,
	AIN_TYPE = 1,
	DIN_TYPE = 2,
	USERVAR_TYPE = 3 ,
	SYSVAR_TYPE = 4 ,
	SYNC_CHANNEL_TYPE = 5,
	SYNC_DEVICE_TYPE = 6,
	MAX_DATA_TYPE = 100,
};

//������ֽ���
enum
{
	MAX_NETBUS_PAK_LENGTH = 398 * 1024,
};

//����ԭ��
enum
{
	COT_NORMAL_ASYNC = 1,      //��������ͬ��
	COT_INCIDENT_ENENT = 2,    //ͻ���¼�
};

struct DATA_BASE
{ 
	int32u m_nType;                      //!<������������
	int32u m_nCount;                     //!<����������Ŀ
	int32u m_nNodeOccNo;                 //!<�ڵ�OccNo
	int32u m_nStartOccNo;                //!<����������ʼ��ַ
	int32u m_nTransReason;               //!<���ݴ���ԭ��
	int32u m_nExtraLen;
};
 

//�ڵ���
class CNodeGroup
{
public:
	CNodeGroup(std::string& szName)
	{
		Q_ASSERT(!szName.empty() && szName.size() > 0);
		m_szGrpName = szName;
	}
	~CNodeGroup()
	{
		m_arrNodes.clear();
	}
public:
	void AddNode(NODE_DEF pNode)
	{
		Q_ASSERT(pNode);
		m_arrNodes.push_back(pNode);
	}
	NODE_DEF GetNodeByOccNo(int nOccNo)
	{
		Q_ASSERT(nOccNo!=INVALID_OCCNO && nOccNo<MAX_NODE_OCCNO);

		if (nOccNo==INVALID_OCCNO || nOccNo >=MAX_NODE_OCCNO)
		{
			return nullptr;
		}
		//�ȴӵ�ǰ�ڵ��б���Ѱ��
		for (auto i:m_arrNodes)
		{
			if (i->OccNo = nOccNo)
			{
				return i;
			}
		}
		//�ٴ���������Ѱ��
		for (auto i:m_arrGrps)
		{
			for (auto j:i->m_arrNodes)
			{
				if (j->OccNo==nOccNo)
				{
					return j;
				}
			}
		}
		return nullptr;
	}
	NODE_DEF GetNode(int i)
	{
		Q_ASSERT(i < m_arrNodes.size());
		if (i>=m_arrNodes.size())
		{
			return nullptr;
		}
		return m_arrNodes.at(i);
	}
	NODE_GROUP_DEF GetGrp(int i)
	{
		Q_ASSERT(i<m_arrGrps.size());
		if (i>=m_arrGrps.size())
		{
			return nullptr;
		}
		return m_arrGrps.at(i);
	}
	void AddGrp(NODE_GROUP_DEF pGrp)
	{
		Q_ASSERT(pGrp);
		m_arrGrps.push_back(pGrp);
		/*for (auto i:pGrp->m_arrNodes)
		{
			this->m_arrNodes.push_back(i);
		}*/
	}	
	//��ȡ��ɢ�ڵ���Ŀ
	size_t GetNodeSize()
	{
		return m_arrNodes.size();
	}
	//��ȡ�ڵ�����Ŀ
	size_t GetGrpSize()
	{
		return m_arrGrps.size();
	}
	//��ȡ��ǰǰ�������нڵ���Ŀ,һ����һ��ǰ�ã����ࣩ
	size_t GetTotalNodeSize()
	{
		return m_arrGrps.size()+m_arrNodes.size();
	}
private:
	std::string m_szGrpName;
	std::vector <std::shared_ptr < NODE > > m_arrNodes;
	std::vector <std::shared_ptr < CNodeGroup > >m_arrGrps;
};

//�����ڵ������
class CNodeGroupMgr
{
public:
	CNodeGroupMgr()
	{
	
	}
	~CNodeGroupMgr()
	{
		
	}
public:
	void AddGroup(NODE_TYPE nodeType,NODE_GROUP_DEF pGrp)
	{
		Q_ASSERT(pGrp);
		m_MapNode.insert(std::make_pair(nodeType, pGrp));
	}
	NODE_GROUP_DEF GetGroup(NODE_TYPE nodeType)
	{
		auto it = m_MapNode.find(nodeType);
		if (it==m_MapNode.end())
		{
			return nullptr;
		}
		return it->second;
	}
private:
	std::map<NODE_TYPE,NODE_GROUP_DEF>m_MapNode;
};



#endif