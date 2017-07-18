#ifndef AICUMULATIVESTATISTICSMODEL_H
#define AICUMULATIVESTATISTICSMODEL_H

#include <QAbstractTableModel>

class CAICumulativeStatisticsModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum COLUMN
	{
		ID, TagName, Desc, OccNo, BlockOccNo/*, DataType*/, InitQua, Address, PinLabel, InitValue, /*Format,*/ Unit, Enable,
		MaxRaw, MinRaw, MaxConvert, MinConvert, ScaleTagName, /*ScaleType*//*, ScaleDesc,*/ RangeL, RangeH, HighQua, LowQua,
		SaveDisk, SaveDiskPeriod, SaveDB, SaveDBPeriod, Sensitivity, SensitivityType, AlarmTagName, /*AlarmType*//*, AlarmDesc,*/
		ChannelOccNo, DeviceOccNo, ReferenceCount, DataSource, Express,
	};

public:
	CAICumulativeStatisticsModel(QObject *parent);
	~CAICumulativeStatisticsModel();

	int rowCount(const QModelIndex &parent = QModelIndex()) const;

	int columnCount(const QModelIndex &parent = QModelIndex()) const;

	QVariant data(const QModelIndex &index, int role) const;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

	Qt::ItemFlags flags(const QModelIndex &index) const;

	void RefrushModel();

	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);

	//void sort(int column, Qt::SortOrder order = Qt::AscendingOrder);

	bool InsertRows(int position, int rows, const QModelIndex &parent = QModelIndex());
	bool RemoveRows(int position, int rows, const QModelIndex &parent = QModelIndex());

private:
	void InitPara();

private:
	QStringList m_lstHorizontalHeader;
};

#endif // AICUMULATIVESTATISTICSMODEL_H
