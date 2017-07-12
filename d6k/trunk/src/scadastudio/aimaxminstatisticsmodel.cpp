#include "aimaxminstatisticsmodel.h"

CAIMaxMinStatisticsModel::CAIMaxMinStatisticsModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	InitPara();
}

CAIMaxMinStatisticsModel::~CAIMaxMinStatisticsModel()
{

}

int CAIMaxMinStatisticsModel::rowCount(const QModelIndex & parent) const
{
	return 0;
}

int CAIMaxMinStatisticsModel::columnCount(const QModelIndex & parent) const
{
	return 0;
}

QVariant CAIMaxMinStatisticsModel::data(const QModelIndex & index, int role) const
{
	return QVariant();
}

QVariant CAIMaxMinStatisticsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

Qt::ItemFlags CAIMaxMinStatisticsModel::flags(const QModelIndex & index) const
{
	return Qt::ItemFlags();
}

bool CAIMaxMinStatisticsModel::InsertRows(int position, int rows, const QModelIndex & parent)
{
	return false;
}

bool CAIMaxMinStatisticsModel::RemoveRows(int position, int rows, const QModelIndex & parent)
{
	return false;
}

void CAIMaxMinStatisticsModel::RefrushModel()
{
	beginResetModel();
	endResetModel();
}

void CAIMaxMinStatisticsModel::InitPara()
{
	m_lstHorizontalHeader.clear();

	m_lstHorizontalHeader.append(tr("ID"));
}
