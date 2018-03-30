


#include "amplifier.h"
#include "common_helper.h"

#include <QPixmap>
#include <QPainter>

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif


CAmplifier::CAmplifier(std::shared_ptr<QPixmap> pOriginPainting,	QWidget *parent) :QWidget(parent),
	m_pOriginPainting(pOriginPainting)
{
	/// 设置成无边框对话框
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
	/// 开启鼠标实时追踪
	setMouseTracking(true);

	/// 设置默认大小
	m_nSideLength = 122 * CCommonHelper::GetWindowHeightMultiplyingPower();
	m_nImageHeight = 90 * CCommonHelper::GetWindowHeightMultiplyingPower();
	setFixedSize(m_nSideLength, m_nSideLength);

	/// 默认隐藏
	hide();
}

void CAmplifier::OnSizeChange(int w, int h) 
{
    m_sizeScreen = QSize(w, h);
}

void CAmplifier::OnPostionChange(int x, int y)
{
	m_ptCursor = QPoint(x, y);
	raise();
	int dest_x = x + 4;
	int dest_y = y + 26;

	/// 超出屏幕检测
	const QSize& parent_size = parentWidget()->size();
	if (dest_y + height() > parent_size.height())
	{
		dest_y = y - 26 - height();
	}
	if (dest_x + width() > parent_size.width())
	{
		dest_x = x - 4 - width();
	}

	move(dest_x, dest_y);
}


/// 绘制鼠标拖拽时选区矩形的右下顶点的放大图;
void CAmplifier::paintEvent(QPaintEvent *) 
{
    QPainter painter(this);

    /// 绘制背景
    painter.fillRect(rect(), QColor(0, 0, 0, 160));

    QPixmap endPointImage;
    /// 绘制放大图;
    const QSize& parent_size = parentWidget()->size();
    /**
     * @bug   : 在屏幕边缘绘制放大图时会出现图片拉伸
     *          这里暂时做了边缘检测，若是屏幕边缘则不进行放大图的绘制，和QQ截图的采取方式是一致的。
     *
     * @marker: 颜色还是照样识别，但是局部放大的效果暂时禁用
     *
     * @note  : 解决方法，可以发现边缘时，将不能放大的地方，不描绘，或填充黑色，以避免图片被非预期的拉伸问题。
     */
    if ((m_ptCursor.x() + 15 < parent_size.width() && m_ptCursor.x() - 15 > 0)
		&& (m_ptCursor.y() + 11 < parent_size.height() && m_ptCursor.y() - 11 > 0))
	{
		endPointImage = m_pOriginPainting->copy(QRect(m_ptCursor.x() - 15, m_ptCursor.y() - 11, 30, 22)).scaled(m_nSideLength, m_nImageHeight);
		painter.drawPixmap(0, 0, endPointImage);
	}
	else
	{
		endPointImage = m_pOriginPainting->copy(QRect(m_ptCursor.x() - 15, m_ptCursor.y() - 11, 30, 22));
	}
    /// 绘制十字
	painter.setPen(QPen(QColor(0, 180, 255, 180), 4));
	// 竖线;
	painter.drawLine(QPoint(m_nSideLength >> 1, 0), QPoint(m_nSideLength >> 1, m_nImageHeight - 4));
	// 横线;
	painter.drawLine(QPoint(0, m_nImageHeight >> 1), QPoint(m_nSideLength, m_nImageHeight >> 1));

	/// 绘制大图内边框
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRect(2,2,width()-4, m_nImageHeight-4);

    /// 绘制外边框
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(0,0,width()-1,height()-1);

    /// 当前选中矩形的宽高信息;
    QString szSelectScreenInfo = QString("%1 x %2")
            .arg(m_sizeScreen.width()).arg(m_sizeScreen.height());

    /// 当前鼠标像素值的RGB信息
    QImage image = m_pOriginPainting->toImage();
    QColor cursor_pixel = image.pixel(m_ptCursor);
    QString szSelectPtRgb = QString("RGB:(%1,%2,%3)")
                                    .arg(cursor_pixel.red())
                                    .arg(cursor_pixel.green())
                                    .arg(cursor_pixel.blue());

    /// 绘制坐标轴相关数据
    painter.setPen(Qt::white);
    painter.drawText(QPoint(6, m_nImageHeight+14),szSelectScreenInfo);
    painter.drawText(QPoint(6, m_nImageHeight+27),szSelectPtRgb);
}
