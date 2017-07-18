#include "aicumulativestatisticsmodel.h"

CAICumulativeStatisticsModel::CAICumulativeStatisticsModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	InitPara();
}

CAICumulativeStatisticsModel::~CAICumulativeStatisticsModel()
{

}

int CAICumulativeStatisticsModel::rowCount(const QModelIndex & parent) const
{
	return 0;
}

int CAICumulativeStatisticsModel::columnCount(const QModelIndex & parent) const
{
	return 0;
}

QVariant CAICumulativeStatisticsModel::data(const QModelIndex & index, int role) const
{
	return QVariant();
}

QVariant CAICumulativeStatisticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // 水平表头  
		{
			if (m_lstHorizontalHeader.size() > section && section > -1)
				return m_lstHorizontalHeader[section];
			else
				return QVariant();
		}
		else
		{
			//if(vertical_header_list.size() > section)
			//	return vertical_header_list[section]; // 垂直表头  
			//else
			//return QVariant();

			return QString(" ");
		}
	}

	return QVariant();
}

Qt::ItemFlags CAICumulativeStatisticsModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // 设置单元格可编辑,此处注释,单元格无法被编辑  
	return flag;
}

void CAICumulativeStatisticsModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CAICumulativeStatisticsModel::setData(const QModelIndex & index, const QVariant & value, int role)
{
	return false;
}

bool CAICumulativeStatisticsModel::InsertRows(int position, int rows, const QModelIndex & parent)
{
	return false;
}

bool CAICumulativeStatisticsModel::RemoveRows(int position, int rows, const QModelIndex & parent)
{
	return false;
}

void CAICumulativeStatisticsModel::InitPara()
{
	m_lstHorizontalHeader.clear();

	m_lstHorizontalHeader.append(tr("ID"));
}
