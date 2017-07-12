#include "aicurvesavemodel.h"

CAICurveSaveModel::CAICurveSaveModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	InitPara();
}

CAICurveSaveModel::~CAICurveSaveModel()
{

}

int CAICurveSaveModel::rowCount(const QModelIndex & parent) const
{
	return 0;
}

int CAICurveSaveModel::columnCount(const QModelIndex & parent) const
{
	return 0;
}

QVariant CAICurveSaveModel::data(const QModelIndex & index, int role) const
{
	return QVariant();
}

QVariant CAICurveSaveModel::headerData(int section, Qt::Orientation orientation, int role) const
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

Qt::ItemFlags CAICurveSaveModel::flags(const QModelIndex & index) const
{
	return Qt::ItemFlags();
}

bool CAICurveSaveModel::InsertRows(int position, int rows, const QModelIndex & parent)
{
	return false;
}

bool CAICurveSaveModel::RemoveRows(int position, int rows, const QModelIndex & parent)
{
	return false;
}

void CAICurveSaveModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

void CAICurveSaveModel::InitPara()
{
	m_lstHorizontalHeader.clear();

	m_lstHorizontalHeader.append(tr("ID"));
}
