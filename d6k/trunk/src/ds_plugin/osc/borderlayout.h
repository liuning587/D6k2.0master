#ifndef BORDERLAYOUT_H
#define BORDERLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QWidgetItem>

#include <QtGui>

#if QT_VERSION >= 0x050000 
#include <QtWidgets>
#endif//QT_VERSION


 class CBorderLayout : public QLayout
 {
 public:
     enum Position { West, North, South, East, Center };

     CBorderLayout(QWidget *parent, int margin = 0, int spacing = -1);
     CBorderLayout(int spacing = -1);
     ~CBorderLayout();

	 virtual void addItem(QLayoutItem *item);
     void addWidget(QWidget *widget, Position position);

     Qt::Orientations expandingDirections() const;
     bool hasHeightForWidth() const;
	 virtual int count() const;

     QLayoutItem *itemAt(int index) const;
     QSize minimumSize() const;
     void setGeometry(const QRect &rect);
     QSize sizeHint() const;

	 virtual QLayoutItem *takeAt(int index);

     void add(QLayoutItem *item, Position position);

 private:
     struct ItemWrapper
     {
         ItemWrapper(QLayoutItem *i, Position p)
		 {
             pItem = i;
             position = p;
         }

         QLayoutItem *pItem;
         Position position;
     };

     enum SizeType { MinimumSize, SizeHint };
     QSize CalculateSize(SizeType sizeType) const;

     QList<ItemWrapper *> m_list;

	 std::vector<QWidgetItem*> m_arrWidgetItems;
 };

 #endif