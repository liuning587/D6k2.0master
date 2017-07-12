#include "ailimitstatisticsmodel.h"

CAILimitStatisticsModel::CAILimitStatisticsModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	InitPara();
}

CAILimitStatisticsModel::~CAILimitStatisticsModel()
{

}

int CAILimitStatisticsModel::rowCount(const QModelIndex & parent) const
{
	return 0;
}

int CAILimitStatisticsModel::columnCount(const QModelIndex & parent) const
{
	return 0;
}

QVariant CAILimitStatisticsModel::data(const QModelIndex & index, int role) const
{
	return QVariant();
}

QVariant CAILimitStatisticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal) // ˮƽ��ͷ  
		{
			if (m_lstHorizontalHeader.size() > section && section > -1)
				return m_lstHorizontalHeader[section];
			else
				return QVariant();
		}
		else
		{
			//if(vertical_header_list.size() > section)
			//	return vertical_header_list[section]; // ��ֱ��ͷ  
			//else
			//return QVariant();

			return QString(" ");
		}
	}

	return QVariant();
}

Qt::ItemFlags CAILimitStatisticsModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	Qt::ItemFlags flag = QAbstractItemModel::flags(index);

	flag |= Qt::ItemIsEditable; // ���õ�Ԫ��ɱ༭,�˴�ע��,��Ԫ���޷����༭  
	return flag;
}

void CAILimitStatisticsModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

bool CAILimitStatisticsModel::InsertRows(int position, int rows, const QModelIndex & parent)
{
	return false;
}

bool CAILimitStatisticsModel::RemoveRows(int position, int rows, const QModelIndex & parent)
{
	return false;
}

void CAILimitStatisticsModel::InitPara()
{
	m_lstHorizontalHeader.clear();

	m_lstHorizontalHeader.append(tr("ID"));
}
