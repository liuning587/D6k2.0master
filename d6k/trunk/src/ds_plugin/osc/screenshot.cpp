
#include "screenshot.h"

#include "amplifier.h"
#include "common_helper.h"


#include <QDesktopWidget>
#include <QApplication>
#include <QMouseEvent>
#include <QFileDialog>
#include <QClipboard>
#include <QDateTime>
#include <QPainter>
#include <QScreen>
#include <QCursor>
#include <QMutex>
#include <QMenu>
#include <QPen>
#ifndef QT_NO_DEBUG
#include <QDebug>
#endif

#ifdef Q_OS_WIN32
#include <windows.h>
#endif


/// ��갴ťͼƬ��ʮ����������
static const unsigned char uc_mouse_image[] = 
{
    0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A,  0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52
    ,0x00, 0x00, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x2D,  0x08, 0x06, 0x00, 0x00, 0x00, 0x52, 0xE9, 0x60
    ,0xA2, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59,  0x73, 0x00, 0x00, 0x0B, 0x13, 0x00, 0x00, 0x0B
    ,0x13, 0x01, 0x00, 0x9A, 0x9C, 0x18, 0x00, 0x00,  0x01, 0x40, 0x49, 0x44, 0x41, 0x54, 0x58, 0x85
    ,0xED, 0xD5, 0x21, 0x6E, 0xC3, 0x30, 0x14, 0xC6,  0xF1, 0xFF, 0x9B, 0xC6, 0x36, 0x30, 0x38, 0xA9
    ,0x05, 0x01, 0x05, 0x81, 0x05, 0x03, 0x39, 0xCA,  0x60, 0x8F, 0xD2, 0x03, 0xEC, 0x10, 0x3B, 0x46
    ,0xC1, 0xC0, 0xC6, 0x0A, 0x3B, 0x96, 0xB1, 0x80,  0x82, 0xC1, 0x56, 0x2A, 0xFF, 0x06, 0xE2, 0x36
    ,0x75, 0x9A, 0xB4, 0xCA, 0xEC, 0x4E, 0x9A, 0xE4,  0x2F, 0xB2, 0x42, 0x22, 0xFF, 0xF2, 0xFC, 0x9C
    ,0x18, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52, 0x52,  0x52, 0x52, 0x52, 0xFE, 0x55, 0xE4, 0xC6, 0xA0
    ,0xDC, 0xC4, 0x71, 0x87, 0xC1, 0xC1, 0x68, 0x01,  0xCC, 0x06, 0xC2, 0x51, 0xD0, 0x29, 0xB0, 0x18
    ,0x00, 0xDF, 0xC6, 0x40, 0x33, 0x37, 0x84, 0x30,  0x4C, 0x80, 0x85, 0xCE, 0x7B, 0x2E, 0x2A, 0x91
    ,0x84, 0x24, 0xBE, 0x25, 0xDE, 0x25, 0x5E, 0x2F,  0x6E, 0xAE, 0xD0, 0x37, 0x92, 0x10, 0xF0, 0x09
    ,0x54, 0x40, 0xE9, 0xEE, 0x15, 0xC6, 0xA2, 0x77,  0xFE, 0xE0, 0xE5, 0x85, 0x8F, 0x16, 0x58, 0xDF
    ,0x35, 0x06, 0x5B, 0xD3, 0xB9, 0xD4, 0x11, 0xD0,  0xA5, 0x8F, 0xDE, 0x57, 0x75, 0x83, 0x73, 0x50
    ,0x06, 0xF6, 0x72, 0x0A, 0x47, 0x40, 0x57, 0x0D,  0x38, 0xDE, 0xC0, 0x04, 0x6F, 0x68, 0x05, 0x36
    ,0xF5, 0xE1, 0x08, 0x3D, 0xCD, 0xEA, 0xEA, 0x5A,  0xD8, 0xBE, 0x5A, 0x46, 0xB0, 0x05, 0x1E, 0xAC
    ,0xF1, 0xC2, 0xD1, 0xCC, 0x01, 0x6D, 0x74, 0x02,  0xDB, 0x3B, 0xBF, 0xD3, 0x73, 0x07, 0x87, 0x2F
    ,0xEF, 0x53, 0x07, 0x38, 0x82, 0x2F, 0xF6, 0xFB,  0xB8, 0x81, 0x73, 0x41, 0x69, 0x28, 0x3A, 0x7A
    ,0x5C, 0xDD, 0x73, 0xCF, 0x3A, 0x86, 0xA3, 0x05,  0x87, 0xEA, 0xCC, 0x60, 0xA1, 0x06, 0x75, 0x89
    ,0xFE, 0x77, 0x92, 0x76, 0x68, 0x23, 0xEF, 0x88,  0xD3, 0x4C, 0xA8, 0x10, 0x7A, 0xD4, 0xEF, 0x8E
    ,0xBE, 0x8B, 0x68, 0x79, 0x3A, 0xB1, 0x72, 0xE1,  0xAE, 0xBC, 0x13, 0x0D, 0xDE, 0xBD, 0x3D, 0xF3
    ,0x08, 0x15, 0xD4, 0xDF, 0x4C, 0x06, 0x36, 0xF7,  0x9E, 0x09, 0xED, 0xE9, 0x99, 0x97, 0x3E, 0x42
    ,0xFF, 0x30, 0x42, 0x4B, 0xA1, 0x8D, 0xD8, 0xE9,  0x2A, 0xBD, 0xED, 0x41, 0x25, 0x2A, 0x89, 0x37
    ,0x1F, 0xBD, 0xEA, 0x61, 0x8B, 0x5F, 0xDD, 0xC1,  0xFA, 0x01, 0xD8, 0xA3, 0x8F, 0xFB, 0xCA, 0x70
    ,0x16, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45,  0x4E, 0x44, 0xAE, 0x42, 0x60, 0x82
};



CScreenshot * CScreenshot::m_pSelf = nullptr;

CScreenshot::CGarbo CScreenshot::s_Garbo;

bool CScreenshot::m_bIsActivity = false;
bool CScreen::m_bIsInit = false;

CScreenshot::CScreenshot(QWidget *parent) : QWidget(parent),
    m_bIsLeftPressed (false), m_pBackgroundScreen(nullptr),
    m_pOriginPainting(nullptr), m_pScreenTool(nullptr)
{
    /// ��ʼ�����
    InitCursor();
    /// ��ȡ��Ļ��Ϣ
    InitGlobalScreen();
    /// ��ʼ�����Ŵ���
    InitAmplifier();
    /// ��ʼ����С��֪��
    InitMeasureWidget();
    /// ȫ������
    showFullScreen();
    /// ��������ʾ������
    setGeometry(GetScreenRect());
    /// �Ե��ö�
   // onEgoistic();
    /// �������ʵʱ׷��
    setMouseTracking(true);
    /// ��������λ��
    emit cursorPosChange(cursor().pos().x(), cursor().pos().y());
    /// ����������򴰿�
    updateMouse();
    /// չʾ����
    show();
}

CScreenshot::~CScreenshot(void) 
{

}

/**
 * @brief������ʵ��
 * @return��OEScreenshot
 */
CScreenshot *CScreenshot::Instance(void)
{
    if (!m_bIsActivity && m_pSelf) 
	{
        Destroy();
    }
    static QMutex mutex;
    if (!m_pSelf) 
	{
       QMutexLocker locker(&mutex);
       if (!m_pSelf) 
	   {
           m_bIsActivity = true;
           m_pSelf = new CScreenshot;
       }
    }
    return m_pSelf;
}

void CScreenshot::Destroy(void) 
{
    if (!m_bIsActivity && m_pSelf)
	{
        delete m_pSelf;
        m_pSelf = nullptr;
    }
}

void CScreenshot::hideEvent(QHideEvent *) 
{
    m_bIsActivity = false;
}


void CScreenshot::closeEvent(QCloseEvent *) 
{
    m_bIsActivity = false;
}

void CScreenshot::mouseDoubleClickEvent(QMouseEvent *)
{
    emit doubleClick();
}

/**
 * ��ʼ���Ŵ� (ɫ�ʲɼ���)
 */
void CScreenshot::InitAmplifier(std::shared_ptr<QPixmap> pOriginPainting)
{
    std::shared_ptr<QPixmap>  pTempPm = pOriginPainting;
    if (pTempPm == nullptr) 
	{
        pTempPm = m_pOriginPainting;
    }
    m_pAmplifierTool.reset(new CAmplifier(pTempPm, this));

    connect(this,SIGNAL(cursorPosChange(int,int)), m_pAmplifierTool.get(), SLOT(OnPostionChange(int,int)));

    m_pAmplifierTool->show();
    m_pAmplifierTool->raise();
}

void CScreenshot::InitMeasureWidget(void)
{
    m_pRectTool.reset(new OERect(this));
    m_pRectTool->raise();
}

/**
 * ���ܣ���õ�ǰ��Ļ�Ĵ�С
 */
const QRect &CScreenshot::GetScreenRect(void) 
{
    if (!m_rcDesktop.isEmpty())
	{
        return m_rcDesktop;
    }
    /// ���ݶ����Ļ������
    m_rcDesktop = QRect(QApplication::desktop()->pos(), QApplication::desktop()->size());
    return m_rcDesktop;
}

std::shared_ptr<QPixmap> CScreenshot::InitGlobalScreen(void) 
{
    if (m_pBackgroundScreen.get() != nullptr) 
	{
        return m_pBackgroundScreen;
    }
    /// �����Ļԭ��
    std::shared_ptr<QPixmap> pTempScreen = GetGlobalScreen();

    /// ������ɫ��Ļ����
    QPixmap temp_dim_pix(pTempScreen->width(), pTempScreen->height());
    temp_dim_pix.fill((QColor(0, 0, 0, 160)));
    m_pBackgroundScreen.reset(new QPixmap(*pTempScreen));

    QPainter p(m_pBackgroundScreen.get());
    p.drawPixmap(0, 0, temp_dim_pix);

    return m_pBackgroundScreen;
}

/*
 * �����Ļ��ԭ��
 * ���أ�QPixmap* ָ��
 */
std::shared_ptr<QPixmap> CScreenshot::GetGlobalScreen(void)
{
    if (m_pOriginPainting.get() == nullptr) 
	{
		/// ��ȡ��ǰ���棬��Ϊ�����ı���ͼ
		QScreen *pScreen = QGuiApplication::primaryScreen();
		const QRect& rcScreen = GetScreenRect();
		m_pOriginPainting.reset(new QPixmap(pScreen->grabWindow(0, rcScreen.x(), rcScreen.y(), rcScreen.width(), rcScreen.height())));
	}
	return m_pOriginPainting;
}

void CScreenshot::SlotOnEgoistic(void)
{
    /// �����ö�
#ifdef Q_OS_WIN32
    SetWindowPos((HWND)this->winId(),HWND_TOPMOST,this->pos().x(),this->pos().y(),this->width(),this->height(),SWP_SHOWWINDOW);
#else
    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    setWindowFlags(flags);
#endif
}

/*
 * ��ʼ�����
 * ������_ico ���ͼƬ����Դ�ļ�
 */
void CScreenshot::InitCursor(const QString& ico)
{
    QPixmap pixmap;
    if (ico.isEmpty())
	{
        pixmap.loadFromData(uc_mouse_image, sizeof(uc_mouse_image));
    }
    else 
	{
        pixmap.load(ico);
    }
    QCursor cursor;
    cursor = QCursor(pixmap, 15, 23);
    setCursor(cursor);
}

std::shared_ptr<CScreen> CScreenshot::CreateScreen(const QPoint &pos) 
{
    if (m_pScreenTool.get() == nullptr) 
	{
		/// ������ͼ��
		m_pScreenTool.reset(new CScreen(m_pOriginPainting, pos, this));
		/// �����ź�����
		connect(this, SIGNAL(cursorPosChange(int, int)), m_pScreenTool.get(), SLOT(SlotOnMouseChange(int, int)));
		/// ����������˫�������źŹ���
		connect(this, SIGNAL(doubleClick()), m_pScreenTool.get(), SLOT(SlotOnSaveScreen()));
		/// ������ͼ����С����
		connect(m_pScreenTool.get(), SIGNAL(sizeChange(int, int)), m_pRectTool.get(), SLOT(OnSizeChange(int, int)));
		connect(m_pScreenTool.get(), SIGNAL(sizeChange(int, int)), m_pAmplifierTool.get(), SLOT(OnSizeChange(int, int)));
		/// ������ͼ�����֪����λ�ù���
		connect(m_pScreenTool.get(), SIGNAL(postionChange(int, int)), m_pRectTool.get(), SLOT(OnPostionChange(int, int)));

        /// ��ý�ͼ����ǰ��ʼλ��
        m_ptStart = pos;
        m_bIsLeftPressed = true;
    }
    return m_pScreenTool;
}

void CScreenshot::DestroyScreen() 
{
	if (m_pScreenTool.get() != nullptr)
	{
		/// �Ͽ��ź���Դ
		disconnect(this, SIGNAL(doubleClick()), m_pScreenTool.get(), SLOT(SlotOnSaveScreen()));
		disconnect(m_pScreenTool.get(), SIGNAL(sizeChange(int, int)), m_pRectTool.get(), SLOT(OnSizeChange(int, int)));
		disconnect(m_pScreenTool.get(), SIGNAL(postionChange(int, int)), m_pRectTool.get(), SLOT(OnPostionChange(int, int)));
		/// ������
        m_pScreenTool.reset();
        m_pScreenTool = nullptr;
        m_bIsLeftPressed = false;
        update();
        return;
    }
}

void CScreenshot::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) 
	{
        CreateScreen(e->pos());
        return ;
    }
}

void CScreenshot::mouseReleaseEvent(QMouseEvent *e) 
{
    if (e->button() == Qt::RightButton)
	{
        if (m_pScreenTool.get() != nullptr)
		{
            m_pRectTool->hide();
            m_pAmplifierTool->OnPostionChange(e->x(), e->y());
            m_pAmplifierTool->show();
            /// ���µ�ǰ���ѡ�еĴ���
            updateMouse();
            return DestroyScreen();
        }
        close();
        return ;
    }
    else if (m_bIsLeftPressed == true && e->button() == Qt::LeftButton) 
	{
        /// ѡ�񴰿�ѡ��
        if (m_ptStart == e->pos() && !m_rcWindow.isEmpty()) 
		{
            m_pScreenTool->setGeometry(m_rcWindow);
            m_pScreenTool->show();
            m_rcWindow = {};
        }
		/// �Ͽ�����ƶ����ź�
		disconnect(this, SIGNAL(cursorPosChange(int, int)), m_pScreenTool.get(), SLOT(SlotOnMouseChange(int, int)));
		/// ���طŴ���
		m_pAmplifierTool->hide();
		/// �Ͽ���ͼ���Ĵ�С�޸��ź�
		disconnect(m_pScreenTool.get(), SIGNAL(sizeChange(int, int)), m_pAmplifierTool.get(), SLOT(OnSizeChange(int, int)));
		m_bIsLeftPressed = false;
    }
    QWidget::mouseReleaseEvent(e);
}

void CScreenshot::mouseMoveEvent(QMouseEvent *e)
{
    emit cursorPosChange(e->x(), e->y());
    if (m_bIsLeftPressed) 
	{
        m_pAmplifierTool->raise();
        m_rcWindow = {};
        update();
    }
    else if (m_bIsLeftPressed == false  && false == CScreen::GetState())
	{
        /// �Ե��ö�
       // onEgoistic();

        /// ���µ�ǰ���ѡ�еĴ���
        updateMouse();
    }
    QWidget::mouseMoveEvent(e);
}

void CScreenshot::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    /// ��ȫ��ͼ
    painter.drawPixmap(0,0,m_rcDesktop.width(), m_rcDesktop.height(), *m_pBackgroundScreen);

    if (!m_rcWindow.isEmpty())
	{
        /// ����ѡ��
        QPen pen = painter.pen();
        pen.setColor(QColor(0,175,255));
        pen.setWidth(2);
		painter.setPen(pen);
		painter.drawRect(m_rcWindow.x(), m_rcWindow.y(), m_rcWindow.width(), m_rcWindow.height());
		painter.drawPixmap(QPoint(m_rcWindow.x(), m_rcWindow.y()), *m_pOriginPainting, m_rcWindow);
    }
}

void CScreenshot::updateMouse(void)
{
#ifdef Q_OS_WIN32
    /// ��ȡ��ǰ���ѡ�еĴ���
    ::EnableWindow((HWND)winId(), FALSE);
#endif
    /// @marker: ֻ����һ��,�����޸��û���������µĲ��Ҵ�����ʶ����洰�ڲ�һ��.
    CCommonHelper::GetSmallestWindowFromCursor(m_rcWindow);
	QPoint temp_pt = mapFromGlobal(QPoint(m_rcWindow.x(), m_rcWindow.y()));
	m_rcWindow = QRect(temp_pt.x(), temp_pt.y(), m_rcWindow.width(), m_rcWindow.height());

#ifdef Q_OS_WIN32
    ::EnableWindow((HWND)winId(), TRUE);
#endif
    m_pAmplifierTool->OnSizeChange(m_rcWindow.width(),m_rcWindow.height());

    emit findChildWind(m_rcWindow);
    update();
}

void CScreenshot::keyPressEvent(QKeyEvent *e) 
{
    /// Esc ���˳���ͼ;
    if (e->key() == Qt::Key_Escape) 
	{
        close();
    }
    else
	{
        e->ignore();
    }
}

///////////////////////////////////////////////////////////




OERect::OERect(QWidget *parent) : QWidget(parent)
{

    /// ���ø�֪��Ĭ�ϴ�С
    setFixedSize(95 * CCommonHelper::GetWindowHeightMultiplyingPower(),
                 20 * CCommonHelper::GetWindowHeightMultiplyingPower());

    /// ���Ĭ�ϱ���
    m_pBackgroundPixmap.reset(new QPixmap(width(),height()));
    m_pBackgroundPixmap->fill((QColor(8, 8, 8, 160)));

    /// Ĭ������
    hide();
}


void OERect::paintEvent(QPaintEvent *) 
{
    QPainter painter(this);
    painter.drawPixmap(rect(),*m_pBackgroundPixmap);
    painter.setPen(QPen(QColor(Qt::white)));
    painter.drawText(rect(), Qt::AlignCenter, m_szInfo);
}

void OERect::OnPostionChange(int x, int y)
{
    if (x < 0)
		x = 0;
    if (y < 0)
		y = 0;

    const int& ry = y - height() - 1;
    if (ry < 0) 
	{
        this->raise();
    }

    move(x, ((ry < 0) ? y : ry));
    show();
}

void OERect::OnSizeChange(int w, int h)
{
    m_szInfo = QString("%1 x %2").arg(w).arg(h);
}

///////////////////////////////////////////////////////////

CScreen::CScreen(std::shared_ptr<QPixmap> originPainting, QPoint pos, QWidget *parent)
	: QWidget(parent), m_direction(NONE), m_ptOrigin(pos), m_bIsPressed(false), m_pOriginPainting(originPainting)
{
    m_pMenu = new QMenu(this);
    m_pMenu->addAction(QStringLiteral("��ɽ�ͼ"), this, SLOT(SlotOnSaveScreen()));
    m_pMenu->addAction(QStringLiteral("����"), this, SLOT(SlotOnSaveScreenOther()));
    m_pMenu->addSeparator();
    m_pMenu->addAction(QStringLiteral("�˳���ͼ"), this, SLOT(SlotQuitScreenshot()));
	
	/// ˫�������
    connect(this, SIGNAL(doubleClick()), this, SLOT(SlotOnSaveScreen()));

    /// �������ʵʱ׷��
    setMouseTracking(true);
    /// Ĭ������
    hide();
}

CScreen::~CScreen()
{
	m_bIsInit = false;
//	m_pMenu->deleteLater();
}

CScreen::DIRECTION CScreen::GetRegion(const QPoint &cursor) 
{
    if (!m_bIsInit)
	{
        return NONE;
    }
    CScreen::DIRECTION ret_dir = NONE;
    // left upper
    QPoint pt_lu = mapToParent(rect().topLeft());
    // right lower
    QPoint pt_rl = mapToParent(rect().bottomRight());

    int x = cursor.x();
    int y = cursor.y();

	/// �����굱ǰ�������ڵı߽緽��
	if (pt_lu.x() + INNER_PADDING >= x && pt_lu.x() <= x
		&& pt_lu.y() + INNER_PADDING >= y && pt_lu.y() <= y)
	{
        // ���Ͻ�
        ret_dir = LEFTUPPER;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
    } 
	else if (x >= pt_rl.x() - INNER_PADDING && x <= pt_rl.x()
		&& y >= pt_rl.y() - INNER_PADDING && y <= pt_rl.y())
	{
        // ���½�
        ret_dir = RIGHTLOWER;
        this->setCursor(QCursor(Qt::SizeFDiagCursor));
	}
	else if (x <= pt_lu.x() + INNER_PADDING && x >= pt_lu.x()
		&& y >= pt_rl.y() - INNER_PADDING && y <= pt_rl.y())
	{
        // ���½�
        ret_dir = LEFTLOWER;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    } 
	else if(x <= pt_rl.x()
           && x >= pt_rl.x() - INNER_PADDING
           && y >= pt_lu.y()
           && y <= pt_lu.y() + INNER_PADDING)
	{
        // ���Ͻ�
        ret_dir = RIGHTUPPER;
        this->setCursor(QCursor(Qt::SizeBDiagCursor));
    }
	else if(x <= pt_lu.x() + INNER_PADDING && x >= pt_lu.x()) 
	{
        // ���
        ret_dir = LEFT;
        this->setCursor(QCursor(Qt::SizeHorCursor));
    } 
	else if( x <= pt_rl.x()   && x >= pt_rl.x() - INNER_PADDING)
	{
        // �ұ�
        ret_dir = RIGHT;
		this->setCursor(QCursor(Qt::SizeHorCursor));
	}
	else if (y >= pt_lu.y() && y <= pt_lu.y() + INNER_PADDING)
	{
        // �ϱ�
        ret_dir = UPPER;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    } 
	else if(y <= pt_rl.y() && y >= pt_rl.y() - INNER_PADDING) 
	{
        // �±�
        ret_dir = LOWER;
        this->setCursor(QCursor(Qt::SizeVerCursor));
    }
	else 
	{
        // Ĭ��
        ret_dir = NONE;
        this->setCursor(QCursor(Qt::SizeAllCursor));
    }
    return ret_dir;
}


void CScreen::contextMenuEvent(QContextMenuEvent *) 
{
    /// �����λ�õ�����˵���
    m_pMenu->exec(cursor().pos());
}

void CScreen::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) 
	{
        emit doubleClick();
        e->accept();
    }
}

void CScreen::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) 
	{
        m_bIsPressed = true;
        if(m_direction != NONE) 
		{
            this->mouseGrabber();
        }
        /// @bug :������ܴ�������, ��Ӧ��ʹ��globalPos
        m_ptMovePos = e->globalPos() - pos();
    }
}

void CScreen::mouseReleaseEvent(QMouseEvent * e) 
{
    if (e->button() == Qt::LeftButton) 
	{
        m_bIsPressed = false;
        if(m_direction != NONE)
		{
            setCursor(QCursor(Qt::SizeAllCursor));
        }
    }
}

void CScreen::mouseMoveEvent(QMouseEvent * e)
{
    QPoint gloPoint = mapToParent(e->pos());
    // left upper
    QPoint pt_lu = mapToParent(rect().topLeft());
    // left lower
    QPoint pt_ll = mapToParent(rect().bottomLeft());
    // right lower
    QPoint pt_rl = mapToParent(rect().bottomRight());
    // right upper
    QPoint pt_ru = mapToParent(rect().topRight());
	if (!m_bIsPressed)
	{
		/// ��������귽��
		m_direction = GetRegion(gloPoint);

		/// ���ݷ�λ�ж�������Ӧ֧��
		switch (m_direction)
		{
		case NONE:
		case RIGHT:
		case RIGHTLOWER:
			m_ptOrigin = pt_lu;
			break;
		case RIGHTUPPER:
			m_ptOrigin = pt_ll;
			break;
		case LEFT:
        case LEFTLOWER:
            m_ptOrigin = pt_ru;
            break;
        case LEFTUPPER:
        case UPPER:
            m_ptOrigin = pt_rl;
            break;
        case LOWER:
            m_ptOrigin = pt_lu;
            break;
        }
    }
    else 
	{
        if(m_direction != NONE)
		{
            const int& global_x = gloPoint.x();
#ifndef QT_NO_DEBUG
            qDebug() << "CScreen::" << __FUNCTION__ << "call onMouseChange()";
#endif
            /// ����������ק
            switch(m_direction) 
			{
            case LEFT:
                return SlotOnMouseChange(global_x, pt_ll.y() + 1);
            case RIGHT:
                return SlotOnMouseChange(global_x, pt_rl.y() + 1);
            case UPPER:
                return SlotOnMouseChange(pt_lu.x(), gloPoint.y());
            case LOWER:
                return SlotOnMouseChange(pt_rl.x() + 1, gloPoint.y());
            case LEFTUPPER:
            case RIGHTUPPER:
            case LEFTLOWER:
            case RIGHTLOWER:
                return SlotOnMouseChange(global_x, gloPoint.y());
            default:
                break;
            }
        }
        else 
		{
            /// ���ڵ��ƶ�
            /// @bug :������ܴ�������, ��Ӧ��ʹ��globalPos
            move(e->globalPos() - m_ptMovePos);
            m_ptMovePos = e->globalPos() - pos();
#ifndef QT_NO_DEBUG
            qDebug() << "CScreen::" << __FUNCTION__ << pos();
#endif
        }
    }
    m_rcCurrent = geometry();
}

void CScreen::moveEvent(QMoveEvent *)
{
    emit postionChange(x(), y());
}

void CScreen::resizeEvent(QResizeEvent *)
{
	m_listMarker.clear();

	/// ���¼���˸�ê��
	// �ǵ�
	m_listMarker.push_back(QPoint(0, 0));
	m_listMarker.push_back(QPoint(width(), 0));
	m_listMarker.push_back(QPoint(0, height()));
	m_listMarker.push_back(QPoint(width(), height()));

	// �е�
	m_listMarker.push_back(QPoint((width() >> 1), 0));
	m_listMarker.push_back(QPoint((width() >> 1), height()));
	m_listMarker.push_back(QPoint(0, (height() >> 1)));
	m_listMarker.push_back(QPoint(width(), (height() >> 1)));

	emit sizeChange(width(), height());
}

void CScreen::showEvent(QShowEvent *) 
{
    m_bIsInit = true;
}

void CScreen::hideEvent(QHideEvent *) 
{
    m_rcCurrent = {};
    m_ptMovePos = {};
    m_ptOrigin = {};
    m_bIsInit = false;
}

void CScreen::enterEvent(QEvent *e) 
{
    setCursor(Qt::SizeAllCursor);
    QWidget::enterEvent(e);
}

void CScreen::leaveEvent(QEvent *e) 
{
    setCursor(Qt::ArrowCursor);
    QWidget::leaveEvent(e);
}

void CScreen::closeEvent(QCloseEvent *)
{
    m_bIsInit = false;
}

void CScreen::paintEvent(QPaintEvent *) 
{
    QPainter painter(this);
    /// ���ƽ����༭����
    painter.drawPixmap(QPoint(0,0), *m_pOriginPainting, m_rcCurrent);

    /// ���Ʊ߿���
    QPen pen(QColor(0,174,255),2);
    painter.setPen(pen);
    painter.drawRect(rect());

    /// ���ư˸���
    //�ı��Ŀ��
    pen.setWidth(4);
    pen.setColor(Qt::red);
    painter.setPen(pen);
    painter.drawPoints(m_listMarker);
}


const QString CScreen::GetFileName(void) const
{
	QString file_name = QDateTime::currentDateTime().toString(("wave-yyyy-MM-dd-HH-mm-ss"));
    return file_name;
}


void CScreen::SlotOnSaveScreenOther(void)
{
    QString fileName = QFileDialog::getSaveFileName(this,
                              tr("Save picture"),
                              GetFileName(), "JPEG Files (*.jpg)");

    if (fileName.length() > 0 && m_pOriginPainting)
	{
        m_pOriginPainting->copy(m_rcCurrent).save(fileName, "jpg");
        SlotQuitScreenshot();
    }
}

void CScreen::SlotOnSaveScreen(void)
{
    /// ��ͼƬ������а�
    QClipboard *pBoard = QApplication::clipboard();
    pBoard->setPixmap(m_pOriginPainting->copy(m_rcCurrent));
    /// �˳���ǰ��ͼ����
    SlotQuitScreenshot();
}


void CScreen::SlotQuitScreenshot(void) 
{
    close();
    parentWidget()->close();
}

void CScreen::SlotOnMouseChange(int x, int y) 
{
    show();
    if (x < 0 || y < 0) 
	{
        return;
    }
    const int& rx = (x >= m_ptOrigin.x()) ? m_ptOrigin.x() : x;
    const int& ry = (y >= m_ptOrigin.y()) ? m_ptOrigin.y() : y;
    const int& rw = abs(x - m_ptOrigin.x());
    const int& rh = abs(y - m_ptOrigin.y());

    /// �ı��С
    m_rcCurrent = QRect(rx, ry, rw, rh);

    this->setGeometry(m_rcCurrent);
    /// �ı��С����¸����ڣ���ֹ������δ��ʱˢ�¶����µ�����
    parentWidget()->update();
}


extern "C" void CScreenshot(void) 
{
    CScreenshot::Instance();
}
