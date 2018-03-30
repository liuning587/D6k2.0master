

#include "borderlayout.h"
#include "stl_util-inl.h"


CBorderLayout::CBorderLayout(QWidget *parent, int margin, int spacing)
	: QLayout(parent)
{
	setMargin(margin);
	setSpacing(spacing);
}

CBorderLayout::CBorderLayout(int spacing)
{
	setSpacing(spacing);
}

CBorderLayout::~CBorderLayout()
{
	//      QLayoutItem *pItem = nullptr;
	//      while ((pItem = takeAt(0)))
	//          delete pItem;
	/*
	auto begin = m_list.begin();
	auto end = m_list.end();

	while (begin != end)
	{
		auto temp = begin;
		++begin;
		delete *temp;
	}
	m_list.clear();
	*/
	STLDeleteElements(&m_list);
	STLDeleteElements(&m_arrWidgetItems);
}

void CBorderLayout::addItem(QLayoutItem *pItem)
{
	Q_ASSERT(pItem);
	if (pItem == nullptr)
		return;

	add(pItem, West);
}

void CBorderLayout::addWidget(QWidget *pWidget, Position position)
{
	Q_ASSERT(pWidget);
	if (pWidget == nullptr)
		return;

	QWidgetItem *pWidgetItem = new QWidgetItem(pWidget);

	m_arrWidgetItems.emplace_back(pWidgetItem);

	add(pWidgetItem, position);
}

void CBorderLayout::add(QLayoutItem *pItem, Position position)
{
	Q_ASSERT(pItem);
	if (pItem == nullptr)
		return;

	ItemWrapper *pWrapper = new ItemWrapper(pItem, position);

	m_list.append(pWrapper);
}

Qt::Orientations CBorderLayout::expandingDirections() const
{
	return Qt::Horizontal | Qt::Vertical;
}

bool CBorderLayout::hasHeightForWidth() const
{
	return false;
}

int CBorderLayout::count() const
{
	return m_list.size();
}

QLayoutItem *CBorderLayout::itemAt(int index) const
{
	ItemWrapper *pWrapper = m_list.value(index);
	if (pWrapper)
		return pWrapper->pItem;
	else
		return 0;
}

QSize CBorderLayout::minimumSize() const
{
	return CalculateSize(SizeHint);
}

void CBorderLayout::setGeometry(const QRect &rect)
{
	ItemWrapper *center = 0;
	int eastWidth = 0;
	int westWidth = 0;
	int northHeight = 0;
	int southHeight = 0;
	int centerHeight = 0;
	int i;

	QLayout::setGeometry(rect);

	for (i = 0; i < m_list.size(); ++i)
	{
		ItemWrapper *wrapper = m_list.at(i);
		QLayoutItem *item = wrapper->pItem;
		Position position = wrapper->position;

		if (position == North)
		{
			item->setGeometry(QRect(rect.x(), northHeight, rect.width(),
				item->sizeHint().height()));

			// northHeight += item->geometry().height() + spacing();
			northHeight += item->geometry().height();
		}
		else if (position == South)
		{
			item->setGeometry(QRect(item->geometry().x(),
				item->geometry().y(), rect.width(),
				item->sizeHint().height()));

			//southHeight += item->geometry().height() + spacing();
			southHeight += item->geometry().height();

			item->setGeometry(QRect(rect.x(),
				//rect.y() + rect.height() - southHeight + spacing(),
				rect.y() + rect.height() - southHeight,
				item->geometry().width(),
				item->geometry().height()));
		}
		else if (position == Center)
		{
			center = wrapper;
		}
	}

	centerHeight = rect.height() - northHeight - southHeight;

	for (i = 0; i < m_list.size(); ++i)
	{
		ItemWrapper *wrapper = m_list.at(i);
		QLayoutItem *item = wrapper->pItem;
		Position position = wrapper->position;

		if (position == West)
		{
			item->setGeometry(QRect(rect.x() + westWidth, northHeight,
				item->sizeHint().width(), centerHeight));

			//westWidth += item->geometry().width() + spacing();
			westWidth += item->geometry().width();
		}
		else if (position == East)
		{
			item->setGeometry(QRect(item->geometry().x(), item->geometry().y(),
				item->sizeHint().width(), centerHeight));

			//eastWidth += item->geometry().width() + spacing();
			eastWidth += item->geometry().width();

			item->setGeometry(QRect(
				//rect.x() + rect.width() - eastWidth + spacing(),
				rect.x() + rect.width() - eastWidth,
				northHeight, item->geometry().width(),
				item->geometry().height()));
		}
	}

	if (center)
		center->pItem->setGeometry(QRect(westWidth, northHeight,
			rect.width() - eastWidth - westWidth,
			centerHeight));
}

QSize CBorderLayout::sizeHint() const
{
	return CalculateSize(SizeHint);
}

QLayoutItem *CBorderLayout::takeAt(int index)
{
	if (index >= 0 && index < m_list.size())
	{
		ItemWrapper *pLayoutItem = m_list.takeAt(index);
		return pLayoutItem->pItem;
	}
	return 0;
}


QSize CBorderLayout::CalculateSize(SizeType sizeType) const
{
	QSize totalSize;

	for (int i = 0; i < m_list.size(); ++i)
	{
		ItemWrapper *wrapper = m_list.at(i);
		Position position = wrapper->position;
		QSize itemSize;

		if (sizeType == MinimumSize)
			itemSize = wrapper->pItem->minimumSize();
		else // (sizeType == SizeHint)
			itemSize = wrapper->pItem->sizeHint();

		if (position == North || position == South || position == Center)
			totalSize.rheight() += itemSize.height();

		if (position == West || position == East || position == Center)
			totalSize.rwidth() += itemSize.width();
	}
	return totalSize;
}