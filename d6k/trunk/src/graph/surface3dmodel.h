#ifndef CSURFACE3DMODEL_H
#define CSURFACE3DMODEL_H

#include <QAbstractItemModel>
#include <memory>

//单个数据结构
class CItemSurface
{
public:
    CItemSurface()
    {
        m_fxData = 0;
        m_fyData = 0;
        m_fzData = 0;
    }

    ~CItemSurface()
    {

    }

public:
    double m_fxData;
    double m_fyData;
    double m_fzData;
};


class CSurface3dModel : public QAbstractTableModel
{
    Q_OBJECT

    enum
    {
        E_XROLE = Qt::UserRole+1,
        E_YROLE = Qt::UserRole+2,
        E_ZROLE = Qt::UserRole+3,
    };

public:
    explicit CSurface3dModel(QObject *parent = 0);
    ~CSurface3dModel();

    //添加数据
    void AddItem(std::shared_ptr<CItemSurface> pItem);
    //
    void AddItem(double x,double y,double z);
    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const;

private:
    //数据
    std::vector<std::shared_ptr<CItemSurface> > m_arrData;
    //
    int m_nRowCount;
    int m_nColCount;

};

#endif // CSURFACE3DMODEL_H
