#ifndef AIMAXMINSTATISTICSMODEL_H
#define AIMAXMINSTATISTICSMODEL_H

#include <QAbstractTableModel>

class CAIMaxMinStatisticsModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CAIMaxMinStatisticsModel(QObject *parent);
	~CAIMaxMinStatisticsModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

	void RefrushModel();

private:
	void InitPara();

private:
	QStringList m_lstHorizontalHeader;
};

#endif // AIMAXMINSTATISTICSMODEL_H
