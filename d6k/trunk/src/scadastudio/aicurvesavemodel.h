#ifndef AICURVESAVEMODEL_H
#define AICURVESAVEMODEL_H

#include <QAbstractTableModel>

class CAICurveSaveModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	CAICurveSaveModel(QObject *parent);
	~CAICurveSaveModel();

public:
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

#endif // AICURVESAVEMODEL_H
