/*! @file dync_file_op_event.cpp
<PRE>
********************************************************************************
模块名       :
文件名       :  dync_file_op_event.cpp
文件实现功能 :  文件操作命令
作者         :  LiJin
版本         :  V1.00
--------------------------------------------------------------------------------
备注         :  <其它说明>
--------------------------------------------------------------------------------
修改记录 :
日 期        版本     修改人              修改内容

********************************************************************************
</PRE>
*  @brief   基本的命令事件定义
*  @author  LiJin
*  @version 1.0
*  @date    2017.06.11
*******************************************************************************/

#include "dync_file_op_event.h"

#include "fesapi/fesdatadef.h"

#ifdef HMI_SDK_LIB
#include "log/log.h"
#include "scadaapi/scdsvcapi.h"
#else
#include "graph_module.h"
#endif
#include <memory>
#include <array>
#include <QObject>
#include <QActionGroup>
#include "../scdpub/variant_data.h"
 


std::array<QString, CDyncFileOpEventItem::MAX_FILE_OP_NUM> CDyncFileOpEventItem::m_arrFileOpTypeTxt =
{
	QObject::tr("Open Normal (scree change)"),
	QObject::tr("Open Modal (pop-up screen)"),
	QObject::tr("Open Frame (multi-monitor)"),
	QObject::tr("Open in other process (Safe Mode)"),
	QObject::tr("Print"),
	QObject::tr("Close and Return Back"),
	QObject::tr("Execute Synapses"),
	QObject::tr("Open Next"),
	QObject::tr("Open Prev"),
	QObject::tr("Capture and Print"),
	QObject::tr("Capture and Save")
};

CDyncFileOpEventItem::CDyncFileOpEventItem() :CBaseDyncEventItem()
{
	m_FileOpType = FILE_OPEN_NORMAL;

	m_nXPos = 100;
	m_nYPos = 100;

	//监视器
	m_nMontor = 0;

	//宽度
	m_nWidth = 0;
	//高度
	m_nHeight = 0;
	//标题
	m_bTitle = true;
	//边框
	m_bFrame = true;
	//可变边框
	m_bChangeableFrame = false;
	//系统菜单
	m_bSystemMenu = false;
	//最大化图标
	m_bMaxIcon = false;
	//最小化图标
	m_bMinIcon = false;
	//保持比例打印
	m_bScalePrint = false;
	//打印页宽
	m_nPrintWidth = -1;
	//打印页高
	m_nPrintHeight = -1;
	//顶边界
	m_nTopMargin = -1;
	//底边界
	m_nBottomMargin = -1;
	//左边界
	m_nLeftMargin = -1;
	//右边界
	m_nRightMaring = -1;
}

CDyncFileOpEventItem::CDyncFileOpEventItem(const CDyncFileOpEventItem *pSrc) :CBaseDyncEventItem()
{
	Q_ASSERT(pSrc);
	Q_ASSERT(pSrc != this);
	if (pSrc && pSrc != this)
	{
		Q_ASSERT(pSrc->m_OpEventType == DYNC_FILE_OP);
		m_bEnable = pSrc->m_bEnable;
		m_OpEventType = pSrc->m_OpEventType;
		m_FileOpType = pSrc->m_FileOpType;
		m_szGraphFileName = pSrc->m_szGraphFileName;
		m_nXPos = pSrc->m_nXPos;
		m_nYPos = pSrc->m_nYPos;
	}
}

CDyncFileOpEventItem::~CDyncFileOpEventItem()
{

}

CDyncFileOpEventItem& CDyncFileOpEventItem::operator=(const CDyncFileOpEventItem& src)
{
	if (this != &src)
	{
		Q_ASSERT(src.m_OpEventType == DYNC_FILE_OP);

		this->m_bEnable = src.m_bEnable;
		this->m_OpEventType = src.m_OpEventType;

		//! 文件操作类型
		m_FileOpType = src.m_FileOpType;;
		//! 文件名，格式： 目录\文件 不包含后缀
		m_szGraphFileName = src.m_szGraphFileName;;
		//监视器
		int m_nMontor = src.m_nMontor;;
		//参数文件
		QString m_szParaFile = src.m_szParaFile;;
		//宽度
		m_nWidth = src.m_nWidth;;
		//高度
		m_nHeight = src.m_nHeight;;
		//标题
		m_bTitle = src.m_bTitle;;
		//边框
		m_bFrame = src.m_bFrame;;
		//可变边框
		m_bChangeableFrame = src.m_bChangeableFrame;;
		//系统菜单
		m_bSystemMenu = src.m_bSystemMenu;;
		//最大化图标
		m_bMaxIcon = src.m_bMaxIcon;;
		//最小化图标
		m_bMinIcon = src.m_bMinIcon;;
		//保持比例打印
		m_bScalePrint = src.m_bScalePrint;;
		//打印页宽
		m_nPrintWidth = src.m_nPrintWidth;;
		//打印页高
		m_nPrintHeight = src.m_nPrintHeight;;
		//顶边界
		m_nTopMargin = src.m_nTopMargin;;
		//底边界
		m_nBottomMargin = src.m_nBottomMargin;;
		//左边界
		m_nLeftMargin = src.m_nLeftMargin;;
		//右边界
		m_nRightMaring = src.m_nRightMaring;;

		m_nXPos = src.m_nXPos;;
		m_nYPos = src.m_nYPos;;
	}

	return *this;
}
bool CDyncFileOpEventItem::operator==(const CDyncFileOpEventItem& src) const
{
	if (this->m_bEnable == src.m_bEnable &&
		this->m_OpEventType == src.m_OpEventType &&
		this->m_FileOpType == src.m_FileOpType &&
		m_szGraphFileName == src.m_szGraphFileName &&
		m_nXPos == src.m_nXPos &&
		m_nYPos == src.m_nYPos)
	{
		return true;
	}
	return false;
}

std::string CDyncFileOpEventItem::GetEventDescript() const
{
	std::string szRet;
	if (m_FileOpType == FILE_CLOSE_FILE || m_FileOpType == FILE_OPEN_NEXT_FILE || m_FileOpType == FILE_OPEN_PREV_FILE)
	{
		// 单击 关闭
	//	szRet.Format(("%s %s"), m_arrActionTypeTxt[m_ActionType], m_arrFileOpTypeTxt[m_FileOpType]);
	}
	else
	{
		// 单击 打开文件 ...
	//	szRet.Format(("%s %s 画面名称:%s"), m_arrActionTypeTxt[m_ActionType], m_arrFileOpTypeTxt[m_FileOpType], m_szGraphFileName);
	}
	return szRet;
}
#if 0
void CDyncFileOpEventItem::SaveXml(JWXml::CXmlNodePtr  pDyncPtr) const
{
	ASSERT(!pDyncPtr->IsNull());
	if (pDyncPtr->IsNull())
		return;
	// 	JWXml::CXmlNodePtr pChild = pDyncPtr->NewChild(_T("EVENT_TYPE"));
	// 	ASSERT(pChild);
	// 	if (pChild)
	// 	{
	// 		pChild->SetValue(static_cast<DWORD>(m_OpEventType) );
	// 	}
	pDyncPtr->SetAttribute(_T("EVENT_TYPE"), static_cast<DWORD>(m_OpEventType));

	JWXml::CXmlNodePtr pChild = pDyncPtr->NewChild(_T("ACTION_TYPE"));
	ASSERT(!pChild->IsNull());
	if (!pChild->IsNull())
	{
		pChild->SetValue(static_cast<DWORD>(m_ActionType));
	}
	pChild = pDyncPtr->NewChild(_T("FILE_NAME"));
	ASSERT(!pChild->IsNull());
	if (!pChild->IsNull())
	{
		pChild->SetValue(m_szGraphFileName);
	}
	pChild = pDyncPtr->NewChild(_T("GRAPH_POS"));
	ASSERT(!pChild->IsNull());
	if (!pChild->IsNull())
	{
		JWXml::CXmlNodePtr pPos = pChild->NewChild(_T("X_POS"));
		pPos->SetValue(static_cast<DWORD>(m_nXPos));
		pPos = pChild->NewChild(_T("Y_POS"));
		pPos->SetValue(static_cast<DWORD>(m_nYPos));
	}
}

bool CDyncFileOpEventItem::LoadXml(JWXml::CXmlNodePtr  pDyncPtr)
{
	ASSERT(!pDyncPtr->IsNull());
	if (pDyncPtr->IsNull())
		return false;

	using namespace JWXml;

	DWORD dwRet = pDyncPtr->GetAttribute(_T("EVENT_TYPE"), 0);
	ASSERT(dwRet <= DYNC_USER_OP);

	CXmlNodes child_nodes = pDyncPtr->GetChildren();
	CString szName;

	for (long i = 0; i < child_nodes.GetCount(); i++)
	{
		szName = child_nodes[i]->GetName();
		if (szName == _T("ACTION_TYPE"))
		{
			dwRet = child_nodes[i]->GetValue(0);
			ASSERT(dwRet < MAX_ACTION_NUM);
			if (dwRet >= MAX_ACTION_NUM)
				dwRet = 0;
			m_ActionType = static_cast<ACTION_TYPE>(dwRet);
		}
		else if (szName == _T("FILE_NAME"))
		{
			m_szGraphFileName = child_nodes[i]->GetValue(_T(""));
		}
		else if (szName == _T("GRAPH_POS"))
		{
			CXmlNodePtr  pSub = child_nodes[i]->GetChild(_T("X_POS"), FALSE);
			ASSERT(!pSub->IsNull());
			if (!pSub->IsNull())
			{
				dwRet = pSub->GetValue(0);
				m_nXPos = dwRet;
			}
			pSub = child_nodes[i]->GetChild(_T("Y_POS"), FALSE);
			ASSERT(!pSub->IsNull());
			if (!pSub->IsNull())
			{
				dwRet = pSub->GetValue(0);
				m_nYPos = dwRet;
			}
		}
	}
	m_bEnable = true;
	return true;
}
#endif

const QString & CDyncFileOpEventItem::GetGraphFileName() const
{
	return m_szGraphFileName;
}

void CDyncFileOpEventItem::SetFileOpType(const QString & szTxt)
{
	unsigned int i = 0;
	for (i = 0; i < MAX_FILE_OP_NUM; i++)
	{
		if (szTxt == m_arrFileOpTypeTxt[i])
		{
			m_FileOpType = static_cast<FILE_OP_TYPE>(i);
			return;
		}
	}
	Q_ASSERT(false);
}

QString  CDyncFileOpEventItem::GetFileOpTypeTxt() const
{
	if (m_FileOpType < MAX_FILE_OP_NUM)
	{
		return m_arrFileOpTypeTxt[m_FileOpType];
	}
	else
	{
		Q_ASSERT(false);
		return ("错误");
	}
}

void CDyncFileOpEventItem::SetGraphPos(const std::string & szX, const std::string & szY)
{
	int nVal = 0; // _ttoi(szX);
	if (nVal < 0)
	{
		nVal = 0;
	}
	m_nXPos = static_cast<unsigned int>(nVal);

	nVal = 0; // _ttoi(szY);
	if (nVal < 0)
	{
		nVal = 0;
	}
	m_nYPos = static_cast<unsigned int>(nVal);
}

void CDyncFileOpEventItem::SetGraphXPos(const std::string & szX)
{
	int nVal = 0;// _ttoi(szX);
	if (nVal < 0)
	{
		nVal = 0;
	}
	m_nXPos = static_cast<unsigned int>(nVal);
}
void CDyncFileOpEventItem::SetGraphYPos(const std::string & szY)
{
	int nVal = 0;// _ttoi(szY);
	if (nVal < 0)
	{
		nVal = 0;
	}
	m_nYPos = static_cast<unsigned int>(nVal);
}

void CDyncFileOpEventItem::SetFileName(const QString & szName)
{
	//	ASSERT(!szName.IsEmpty());
	m_szGraphFileName = szName;
}

void CDyncFileOpEventItem::DoAction(CEventIntent &intent)
{
	switch (m_FileOpType)
	{
	case FILE_OPEN_NORMAL:
		if (!m_szGraphFileName.isEmpty())
		{
// 			if (intent.m_fnOpenNewGraph)
// 			{
// 				intent.m_fnOpenNewGraph(m_szGraphFileName);
// 			}
		}
		break;
	case FILE_CLOSE_FILE:
		break;
	}
}


#if 0
bool CDyncEventData::AddFileOpEvent(std::shared_ptr <CDyncFileOpEventItem> & ptr)
{
	if (ptr == nullptr)
		return false;

	auto it = m_arrFileEvents.begin();
	for (; it != m_arrFileEvents.end(); ++it)
	{
		if ((*it) == ptr)
		{
			return false;
		}
	}
	m_arrFileEvents.push_back(ptr);
	return true;
}

bool CDyncEventData::SaveFileOp(CDyncFileOpEventItem * pFileOp)
{
	Q_ASSERT(pFileOp);
	if (pFileOp == nullptr)
		return false;

	if (pFileOp->m_bEnable == true)
	{// 增加
		auto it = m_arrFileEvents.begin();
		for (; it != m_arrFileEvents.end(); ++it)
		{
			if ((*it).get() == pFileOp)
			{
				*(*it) = *pFileOp;
				return true;
			}
		}
		// 没有找到，新增加
		m_arrFileEvents.push_back(std::make_shared<CDyncFileOpEventItem>(pFileOp));
	}
	else
	{// 删除
		auto it = m_arrFileEvents.begin();
		for (; it != m_arrFileEvents.end(); ++it)
		{
			if ((*it).get() == pFileOp)
			{
				m_arrFileEvents.erase(it);
				return true;
			}
		}
	}
	return false;
}

bool CDyncEventData::SaveFileOp(std::shared_ptr <CDyncFileOpEventItem> & ptr)
{
	if (ptr == nullptr)
		return false;

	if (ptr->m_bEnable == true)
	{// 增加
		auto it = m_arrFileEvents.begin();
		for (; it != m_arrFileEvents.end(); ++it)
		{
			if ((*it) == ptr)
			{
				*(*it) = *ptr;
				return true;
			}
		}
		// 没有找到，新增加
		m_arrFileEvents.push_back(ptr);
	}
	else
	{// 删除
		auto it = m_arrFileEvents.begin();
		for (; it != m_arrFileEvents.end(); ++it)
		{
			if ((*it) == ptr)
			{
				m_arrFileEvents.erase(it);
				return true;
			}
		}
	}
	return false;
}

void CDyncEventData::GetFileOpEvents(std::vector <CDyncFileOpEventItem*> &data) const
{
	// 	data.clear();
	// 	auto iter = m_arrEvents.begin();
	// 	for ( ; iter != m_arrEvents.end(); ++iter)
	// 	{
	// 		if ((*iter)->GetEventType() == CBaseDyncEventItem::DYNC_FILE_OP )
	// 		{
	// 			data.push_back( (*iter).get() );
	// 		}
	// 	}
	// 	return;
}

// void  CDyncEventData::GetFileOpEvents(std::vector<CBaseDyncEventItem*> &arrEvents)
// {
// 
// }
#endif

/** @}*/
