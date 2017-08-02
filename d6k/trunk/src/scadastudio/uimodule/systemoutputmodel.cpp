/*! @file
<PRE>
********************************************************************************
ģ����       :  ϵͳ���view
�ļ���       :  outputmessagerserver.cpp
�ļ�ʵ�ֹ��� :  ϵͳ���view
����         :  gw
�汾         :  V1.00
--------------------------------------------------------------------------------
��ע         : <����˵��>
--------------------------------------------------------------------------------
�޸ļ�¼ :
�� ��        �汾     �޸���              �޸�����

********************************************************************************
</PRE>
*  @brief
*  @author gw
*  @version 1.0
*  @date    2015.11.30
*/

#include "systemoutputmodel.h"
#include "logdefine.h"
#include "uimodule/systemoutputview.h"
#include "log/logimpl.h"
#include "log/msglog.h"
#include <QDebug>

/*! \fn CSystemOutputModel::CSystemOutputModel(QObject *parent, int rowCount, int colCount)
********************************************************************************************************* 
** \brief CSystemOutputModel::CSystemOutputModel 
** \details 
** \param parent 
** \param rowCount 
** \param colCount 
** \return  
** \author gw
** \date 2016��4��12�� 
** \note 
********************************************************************************************************/
CSystemOutputModel::CSystemOutputModel(QObject *parent, int rowCount, int colCount)
	: QAbstractTableModel(parent)
{
	Q_UNUSED(parent);
	Q_UNUSED(rowCount);
	Q_UNUSED(colCount);
	
	//QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GBK"));
	this->m_nRowCount = 0;
	this->m_nColCount = colCount;

	m_lstHeader.append(("!"));
	m_lstHeader.append((tr("Index")));
	m_lstHeader.append(tr("Date"));
	m_lstHeader.append(tr("Time"));
	m_lstHeader.append(tr("Module Name"));

	m_lstHeader.append(tr("Comment"));
	m_lstHeader.append(tr("Process ID"));
	m_lstHeader.append(tr("Thread ID"));

// 	m_lstHeader.append(tr("date"));
// 	m_lstHeader.append(QStringLiteral("time"));
// 	m_lstHeader.append(QStringLiteral("desc"));
// 	m_lstHeader.append(QStringLiteral("module"));
// 	m_lstHeader.append(QStringLiteral("thread id"));
// 	m_lstHeader.append(QStringLiteral("process id"));
}
/*! \fn CSystemOutputModel::~CSystemOutputModel()
********************************************************************************************************* 
** \brief CSystemOutputModel::~CSystemOutputModel 
** \details 
** \return  
** \author gw
** \date 2016��4��12�� 
** \note 
********************************************************************************************************/
CSystemOutputModel::~CSystemOutputModel()
{
	ClearAll();
}
/*! \fn int	CSystemOutputModel::columnCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CSystemOutputModel::columnCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2016��4��12�� 
** \note 
********************************************************************************************************/
int	CSystemOutputModel::columnCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);

	return m_nColCount;
}

/*! \fn int CSystemOutputModel::rowCount(const QModelIndex &parent) const
********************************************************************************************************* 
** \brief CSystemOutputModel::rowCount 
** \details 
** \param parent 
** \return int 
** \author gw
** \date 2016��4��12�� 
** \note 
********************************************************************************************************/
int CSystemOutputModel::rowCount(const QModelIndex &parent) const
{
	Q_UNUSED(parent);
	return  static_cast <int>  (m_arrLogs.size());
}

/*! \fn QVariant CSystemOutputModel::data(const QModelIndex &index, int role) const
********************************************************************************************************* 
** \brief CSystemOutputModel::data 
** \details 
** \param index 
** \param role 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2016��4��12�� 
** \note 
********************************************************************************************************/
QVariant CSystemOutputModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) 
	{
		return QVariant(); 
	}


// 	if (Qt::BackgroundColorRole == role)
// 	{
// 		//return  QVariant(QColor(Qt::black));
// 	}

	/*
	if(Qt::FontRole == role)
	{
	return QVariant(CConfig::Instance().GetUIFont());
	}
	*/
	if (Qt::DisplayRole == role)
	{
		int nRow = index.row();
		int nCol = index.column();

		if (nRow >= m_arrLogs.size() || nRow < 0)
			return QVariant();

		std::shared_ptr<CMsgLog> pLog = m_arrLogs[nRow];
		Q_ASSERT(pLog != nullptr);
		if (pLog != nullptr)
		{
			if (nCol == COL_LEVEL)
			{
				return pLog->m_nLevel;
			}
			else if (nCol == COL_INDEX)
			{
				return pLog->m_nIndex;
			}
			else if (nCol == COL_DATE)
			{
				return pLog->m_tmLog.date().toString("yyyy-MM-dd");
			}
			else if (nCol == COL_TIME)
			{
				return pLog->m_tmLog.time().toString("hh:mm:ss.zzz");
			}
			else if (nCol == COL_MODULE)
			{
				return QString::fromLocal8Bit(pLog->m_szModuleName.c_str());
			}
			else if (nCol == COL_COMMENT)
			{
				return QString::fromLocal8Bit(pLog->m_szLogText.c_str());
			}
			else if (nCol == COL_PROCESS)
			{
				return pLog->m_nProcessId;
			}
			else if (nCol == COL_THREAD)
			{
				return pLog->m_nThreadId;
			}
			return QVariant();
		}
	}
	if (Qt::TextColorRole == role)
	{

	}
 
	return QVariant();
} 

void CSystemOutputModel::AddItem(std::shared_ptr<CMsgLog> pLog)
{
	Q_ASSERT(pLog);
	if (pLog == nullptr)
		return;

//	std::shared_ptr<CMsgLog>ptrLog = std::make_shared<CMsgLog>(pLog);
	size_t nRow = m_arrLogs.size();

	beginInsertRows(QModelIndex(), (int)nRow, (int)nRow);
	m_arrLogs.push_back(pLog);
	endInsertRows();
}
#if 0
/*! \fn int  CSystemOutputModel::AddItem(MSG_LOG &log, QColor color)
********************************************************************************************************* 
** \brief CSystemOutputModel::AddItem 
** \details �����
** \param log 
** \param color 
** \return int 
** \author gw
** \date 2016��4��12�� 
** \note 
********************************************************************************************************/
int  CSystemOutputModel::AddItem(const MSG_LOG &log, QColor& color)
{
	//int nRow = m_items.size();

	//if (nRow >= m_rowCount)
		//ClearAll();


	CDataItem* pItem = new CDataItem(log, color);

	int row = (int)m_items.size();
	beginResetModel();
	//beginInsertRows(QModelIndex(), row, row);

	m_items.push_back(pItem);

	endResetModel();
	//endInsertRows();

	//beginResetModel();

	//endResetModel();

	((CSystemOutputView *)this->parent())->scrollToBottom();

	return (int)m_items.size();
}

#endif

/*! \fn void CSystemOutputModel::ClearAll()
********************************************************************************************************* 
** \brief CSystemOutputModel::ClearAll 
** \details 
** \return void 
** \author gw
** \date 2016��4��12�� 
** \note 
********************************************************************************************************/
void CSystemOutputModel::ClearAll()
{
	beginResetModel();
	m_arrLogs.clear();
	endResetModel();
}

/*! \fn QVariant CSystemOutputModel::headerData(int section, Qt::Orientation orientation, int role) const
********************************************************************************************************* 
** \brief CSystemOutputModel::headerData 
** \details 
** \param section 
** \param orientation 
** \param role 
** \return QT_NAMESPACE::QVariant 
** \author gw
** \date 2016��4��12�� 
** \note 
********************************************************************************************************/
QVariant CSystemOutputModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // ˮƽ��ͷ  
		{
			if (m_lstHeader.size() > section)
				return m_lstHeader[section];
			else
				return section + 1;
		}
		else
		{
			return section + 1;
		}
	}

	return QVariant();
}