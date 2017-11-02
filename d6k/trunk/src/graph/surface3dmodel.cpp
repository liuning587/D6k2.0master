#include "surface3dmodel.h"
#include <QDebug>

CSurface3dModel::CSurface3dModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    this->m_nRowCount = 0;
    this->m_nColCount = 3;

}

QVariant CSurface3dModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    // FIXME: Implement me!

    return QVariant();

}

CSurface3dModel::~CSurface3dModel()
{
    m_arrData.clear();
}

void CSurface3dModel::AddItem(std::shared_ptr<CItemSurface> pItem)
{
    Q_ASSERT(pItem);
    if (pItem == nullptr)
        return;

   // std::shared_ptr<CMsgLog>ptrLog = std::make_shared<CMsgLog>(pLog);
    size_t nRow = m_arrData.size();

    beginInsertRows(QModelIndex(), (int)nRow, (int)nRow);
    m_arrData.push_back(pItem);
    endInsertRows();
}

void CSurface3dModel::AddItem(double x,double y,double z)
{
    std::shared_ptr<CItemSurface>pItem = std::make_shared<CItemSurface>();
    pItem->m_fxData = x;
    pItem->m_fyData = y;
    pItem->m_fzData = z;

    size_t nRow = m_arrData.size();

    beginInsertRows(QModelIndex(), (int)nRow, (int)nRow);
    m_arrData.push_back(pItem);
    endInsertRows();
}


int CSurface3dModel::rowCount(const QModelIndex &parent) const
{

    return  static_cast<int>(m_arrData.size());

}

int CSurface3dModel::columnCount(const QModelIndex &parent) const
{

    return 3;
}

QVariant CSurface3dModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    int nRow = index.row();
    int nCol = index.column();

    if (nRow >= m_arrData.size() || nRow < 0)
    {
        return QVariant();
    }

    std::shared_ptr<CItemSurface> pItem = m_arrData[nRow];

    if (role == E_XROLE)
    {
        return pItem->m_fxData;
    }
    else if (role == E_YROLE)
    {
        qDebug()<<"KKKKKKKKKKK:"<<pItem->m_fyData;

        return pItem->m_fyData;
    }
    else if (role == E_ZROLE)
    {
        return pItem->m_fzData;
    }


    return QVariant();
}


QHash<int,QByteArray> CSurface3dModel::roleNames() const
{
    QHash<int, QByteArray> names(QAbstractItemModel::roleNames());
    names[Qt::UserRole+1] = "xdata";
    names[Qt::UserRole+2] = "ydata";
    names[Qt::UserRole+3] = "zdata";
    return names;
}

