


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
	/// ���ó��ޱ߿�Ի���
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
	/// �������ʵʱ׷��
	setMouseTracking(true);

	/// ����Ĭ�ϴ�С
	m_nSideLength = 122 * CCommonHelper::GetWindowHeightMultiplyingPower();
	m_nImageHeight = 90 * CCommonHelper::GetWindowHeightMultiplyingPower();
	setFixedSize(m_nSideLength, m_nSideLength);

	/// Ĭ������
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

	/// ������Ļ���
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


/// ���������קʱѡ�����ε����¶���ķŴ�ͼ;
void CAmplifier::paintEvent(QPaintEvent *) 
{
    QPainter painter(this);

    /// ���Ʊ���
    painter.fillRect(rect(), QColor(0, 0, 0, 160));

    QPixmap endPointImage;
    /// ���ƷŴ�ͼ;
    const QSize& parent_size = parentWidget()->size();
    /**
     * @bug   : ����Ļ��Ե���ƷŴ�ͼʱ�����ͼƬ����
     *          ������ʱ���˱�Ե��⣬������Ļ��Ե�򲻽��зŴ�ͼ�Ļ��ƣ���QQ��ͼ�Ĳ�ȡ��ʽ��һ�µġ�
     *
     * @marker: ��ɫ��������ʶ�𣬵��Ǿֲ��Ŵ��Ч����ʱ����
     *
     * @note  : ������������Է��ֱ�Եʱ�������ܷŴ�ĵط�������棬������ɫ���Ա���ͼƬ����Ԥ�ڵ��������⡣
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
    /// ����ʮ��
	painter.setPen(QPen(QColor(0, 180, 255, 180), 4));
	// ����;
	painter.drawLine(QPoint(m_nSideLength >> 1, 0), QPoint(m_nSideLength >> 1, m_nImageHeight - 4));
	// ����;
	painter.drawLine(QPoint(0, m_nImageHeight >> 1), QPoint(m_nSideLength, m_nImageHeight >> 1));

	/// ���ƴ�ͼ�ڱ߿�
    painter.setPen(QPen(Qt::white, 2));
    painter.drawRect(2,2,width()-4, m_nImageHeight-4);

    /// ������߿�
    painter.setPen(QPen(Qt::black, 1));
    painter.drawRect(0,0,width()-1,height()-1);

    /// ��ǰѡ�о��εĿ����Ϣ;
    QString szSelectScreenInfo = QString("%1 x %2")
            .arg(m_sizeScreen.width()).arg(m_sizeScreen.height());

    /// ��ǰ�������ֵ��RGB��Ϣ
    QImage image = m_pOriginPainting->toImage();
    QColor cursor_pixel = image.pixel(m_ptCursor);
    QString szSelectPtRgb = QString("RGB:(%1,%2,%3)")
                                    .arg(cursor_pixel.red())
                                    .arg(cursor_pixel.green())
                                    .arg(cursor_pixel.blue());

    /// �����������������
    painter.setPen(Qt::white);
    painter.drawText(QPoint(6, m_nImageHeight+14),szSelectScreenInfo);
    painter.drawText(QPoint(6, m_nImageHeight+27),szSelectPtRgb);
}
