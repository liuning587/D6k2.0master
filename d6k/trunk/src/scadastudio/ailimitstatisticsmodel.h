#ifndef AILIMITSTATISTICSMODEL_H
#define AILIMITSTATISTICSMODEL_H

#include <QAbstractTableModel>

class CAILimitStatisticsModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	enum COLUMN
	{
		ID, TagName, Description, TimingSavePeriod, LongTermAccuracy, ShortTermAccuracy
	};


public:
	CAILimitStatisticsModel(QObject *parent);
	~CAILimitStatisticsModel();

	void SetVerticalHeaderList(QStringList verticalHeaderList);

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void RefrushModel();

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

private:
	void InitPara();
private:
	QStringList m_lstHorizontalHeader;
};

#endif // AILIMITSTATISTICSMODEL_H
