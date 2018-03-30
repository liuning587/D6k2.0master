
#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <memory>
#include <QRect>
#include <QWidget>

class CScreen;
class OERect;
class CAmplifier;
class QTimer;
class QMenu;


/**
 * @class : OEScreenshot
 * @brief : �������ܵ���Ҫ���,
 *          ����ȫ���ȼ�,��Դ�Ļ������ͷ�.
 * @remark: ����ʾ��( CScreenshot::Instance(); )
*/
class CScreenshot : public QWidget
{
    Q_OBJECT

signals:

    /**
     * @brief : ����ƶ����źţ�
     * @param : int x�������
     * @param : int y�������
     * @date  : 2017��04��18��
     */
    void cursorPosChange(int, int);

    /**
     * @brief : ˫�����źţ�
     * @date  : 2017��04��18��
     */
    void doubleClick(void);

    /**
     * @brief : ��굱ǰλ����С���Ӵ��ڣ��źţ�
     * @param : QRect ��ǰ���ڵľ�������
     * @date  : 2017��04��18��
     */
    void findChildWind(QRect);

public:
    /**
     * @brief : ���캯��
     * @note  : ��ǰ�����ĸ����ڣ�һ�㲻�������ڣ�
     * @date  : 2017��04��16��
     */
    explicit CScreenshot(QWidget *parent = 0);
    ~CScreenshot(void);

    /**
     * @brief : ����ʵ��
     * @note  : ͨ�����������ý�ͼ��������ʵ��
     * @return: ���� OEScreenshot ʵ��ָ��
     * @date  : 2017��04��15��
     */
    static CScreenshot *Instance(void);

    /**
     * @brief : �ݻٽ�ͼ����
     * @note  : ͨ������������Դݻ�������ͼ����
     * @date  : 2017��04��30��
     */
    static void Destroy(void);

protected:
    /**
     * @brief : ���ش����¼�
     */
    virtual void hideEvent(QHideEvent *);
    /**
     * @brief : �رմ����¼�
     */
    virtual void closeEvent(QCloseEvent *);
    /**
     * @brief : ˫���¼�
     */
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    /**
     * @brief : ��갴���¼�
     */
    virtual void mousePressEvent(QMouseEvent *);
    /**
     * @brief : ����ͷ��¼�
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);
    /**
     * @brief : ����ƶ��¼�
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * @brief : ���������¼�
     */
    virtual void keyPressEvent(QKeyEvent *e);
    /**
     * @brief : �Ի��¼�
     */
    virtual void paintEvent(QPaintEvent *);

    /**
     * @brief : ���µ�ǰ���ѡ���Ĵ���
     */
    void updateMouse(void);

private:
    /**
     * @brief : ��ʼ���Ŵ� (ɫ�ʲɼ���)
     * @note  : ����Ҫ��Ļ��ԭ����Ϊ�Ŵ����ķŴ�Ԫ��
     * @param : originPainting �Ŵ�����Ҫ��Ԫ�أ���Ϊ�գ���Ĭ����originPainting_ԭ��
     * @date  : 2017��04��15��
     * @remark: �����е���getGlobalScreen��
     */
    void InitAmplifier(std::shared_ptr<QPixmap> pOriginPainting = nullptr);
    /**
     * @brief : �����ؼ� (��С��֪��)
     * @date  : 2017��04��27��
     */
    void InitMeasureWidget(void);
    /**
     * @brief : ��ʼ����������
     * @return: QPixmap ������ɫ�������Ļͼ
     * @date  : 2017��04��15��
     */
    std::shared_ptr<QPixmap> InitGlobalScreen(void);
    /**
     * @brief : ��ʼ�����
     * @note  : Ϊ�������Ĭ��״̬�µ�ͼ����ʽ
     * @param : ico ���ͼƬ����Դ�ļ�·��
     * @date  : 2017��04��15��
     * @remark: ������δ��д����ʹ�ñ�����Ĭ�ϵ����Logo
     */
    void InitCursor(const QString& ico = "");

    /**
     * @brief : ������ͼ��
     * @note  : ����ͼ���Ѵ��ڣ��򷵻ؽ�ͼ��ʾ���������ظ�������
     * @param : pos ��ͼ������ʼλ�� ������ǰ���λ�ü��ɣ�
     * @date  : 2017��04��16��
     * @remark: ������ͼ��ǰ����Ҫ������ص������(������С��֪�����Ŵ�ȡɫ��)
     */
    std::shared_ptr<CScreen> CreateScreen(const QPoint &pos);

    /**
     * @brief : �ݻٽ�ͼ��
     * @note  : ����ͼ���Ѵ��ڣ���ݻ�ʾ����������ʾ��������������Դ
     * @date  : 2017��04��16��
     */
    void DestroyScreen(void);
    /**
     * @brief : ��õ�ǰ��Ļ�Ĵ�С
     * @note  : ���������֧�ֶ���Ļ�ģ�ʾ����˫��Ļ QRect��-1920, 0, 3840, 1080��
     * @return: ���� QRect ����
     * @date  : 2017��04��15��
     */
    const QRect& GetScreenRect(void);
    /**
     * @brief : �����Ļ��ԭ��
     * @note  : �������ظ������Ļԭ��������У��򷵻�ԭ�е�ԭ��
     * @return: QPixmap* ָ��
     * @date  : 2017��04��15��
     * @remark: �������»����Ļԭ������Ҫ����ԭ����Դ
     */
    std::shared_ptr<QPixmap> GetGlobalScreen(void);
private:
    /// ���������Ƿ��Ѿ�չʾ
    bool                        m_bIsLeftPressed;
    /// ���ڼ�������
    QPoint                      m_ptStart;
    /// ��ǰ������Ļ�ľ�������
    QRect m_rcDesktop;
    /// ��Ļ��ɫ����ͼ
    std::shared_ptr<QPixmap>    m_pBackgroundScreen;
    /// ��Ļԭ��
    std::shared_ptr<QPixmap>    m_pOriginPainting;
    /// ��ͼ��Ļ
    std::shared_ptr<CScreen>   m_pScreenTool;
    /// ��ͼ����С��֪��
    std::shared_ptr<OERect>     m_pRectTool;
    /// �Ŵ�ȡɫ��
    std::shared_ptr<CAmplifier> m_pAmplifierTool;
    /// ��ǰ���ѡ����С�ľ��δ���
    QRect                       m_rcWindow;
    /// ����ʵ������
    static CScreenshot         *m_pSelf;
    /// �ö���ʱ��
    QTimer                      *m_pEgoisticTimer;
    /// �����
    static bool                 m_bIsActivity;

	class CGarbo
	{
	public:
		~CGarbo()
		{
			if (CScreenshot::m_pSelf)
			{
// 				delete CScreenshot::m_pSelf;
// 				CScreenshot::m_pSelf = nullptr;
			}
		}
	};
	static CGarbo s_Garbo;
private slots:

    /**
     * @brief : Window�°Ե��ö���Ψ�Ҷ���
     * @date  : 2017��04��28��
     * @remark: ʹ�øú���ʱ������ֹ�Ҽ��˵�����Ϊ������ʹ�ã�����BUG
     */
    void SlotOnEgoistic(void);
};




/**
 * @class : OERect
 * @brief : ��С��֪��
 * @note  : ��Ҫ�غ���ͼ�����Ϸ��Ĵ�С��֪�ؼ�
*/
class OERect : public QWidget
{
    Q_OBJECT

signals:


public:

    explicit OERect(QWidget *parent = 0);

protected:

    /**
     * @brief : �Ի溯��
     */
    void paintEvent(QPaintEvent *);

public slots:

    /**
     * @brief : �ⲿ���λ���޸ģ��ۣ�
     * @note  : ��֪���޸�����λ��
     * @param : x ����λ��
     * @param : y ����λ��
     * @date  : 2017��04��15��
     */
    void OnPostionChange(int x, int y);

    /**
     * @brief : �ⲿ�����С�޸� ���ۣ�
     * @note  : ��֪���޸���ʾ�Ĵ�С����
     * @param : w ���
     * @param : h �߶�
     * @date  : 2017��04��15��
     */
    void OnSizeChange(int w, int h);

private:
    /// ����ɫ
    std::shared_ptr<QPixmap>    m_pBackgroundPixmap;
    /// ��ʾ��������Ϣ
    QString                     m_szInfo;
};


/**
 * @class : CScreen
 * @brief : ��ͼ��
 * @note  : ��Ҫ�غ�ͼƬ�ı༭�뱣��
*/
class CScreen : public QWidget 
{

    Q_OBJECT

signals:


    /**
     * @brief : ��ͼ����С�޸ģ��źţ�
     * @param : int ���
     * @param : int �߶�
     * @date  : 2017��04��17��
     */
    void sizeChange(int,int);

    /**
     * @brief : ��ͼ�����ڵ�λ�ã��źţ�
     * @param : int ���ڵĺ���λ��
     * @param : int ���ڵ�����λ��
     * @date  : 2017��04��17��
     */
    void postionChange(int,int);

    /**
     * @brief : ˫�� ���źţ�
     * @date  : 2017��04��17��
     */
    void doubleClick(void);

protected:

    /// �ڱ߾࣬������ק�Ĵ�����
    const int INNER_PADDING = 6;

    /// ��λö��
    enum DIRECTION 
	{
        UPPER=0,
        LOWER=1,
        LEFT,
        RIGHT,
        LEFTUPPER,
        LEFTLOWER,
        RIGHTLOWER,
        RIGHTUPPER,
        NONE
    };

public:
    explicit CScreen(std::shared_ptr<QPixmap> originPainting, QPoint pos, QWidget *parent = 0);

	~CScreen();

    /**
     * @brief : ��õ�ǰ��ͼ���Ƿ����
     * @return: true : ����
     * @date  : 2017��04��17��
     */
    static bool GetState(void) 
	{
		return m_bIsInit; 
	}

protected:

    /**
     * @brief : ��õ�ǰ�����ı߽緽λ
     * @param : cursor ��ǰ����λ��
     * @return: DIRECTION ���ķ�λö��
     * @date  : 2017��04��17��
     */
    DIRECTION GetRegion(const QPoint &cursor);


    /**
     * @brief : �����˵��¼�
     */
    virtual void contextMenuEvent(QContextMenuEvent *);

    /**
     * @brief : ˫���¼�
     */
    virtual void mouseDoubleClickEvent(QMouseEvent *e);

    /**
     * @brief : ��갴���¼�
     */
    virtual void mousePressEvent(QMouseEvent *e);

    /**
     * @brief : ����ͷ��¼�
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);
    /**
     * @brief : ����ƶ��¼�
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * @brief : �����ƶ��¼�
     */
    virtual void moveEvent(QMoveEvent *);

    /**
     * @brief : ���ڴ�С�޸��¼�
     */
    virtual void resizeEvent(QResizeEvent *);


    /**
     * @brief : ������ʾ�¼�
     */
    virtual void showEvent(QShowEvent *);

    /**
     * @brief : ���������¼�
     */
    virtual void hideEvent(QHideEvent *);

    /**
     * @brief : �����봰���¼�
     */
    virtual void enterEvent(QEvent *e);

    /**
     * @brief : ����뿪�����¼�
     */
    virtual void leaveEvent(QEvent *e);

    /**
     * @brief : ���ڹر��¼�
     */
    virtual void closeEvent(QCloseEvent *);

    /**
     * @brief : �����Ի��¼�
     */
    virtual void paintEvent(QPaintEvent *);

private:

    /**
     * @brief : ���һ����ʱ���ʽ�������ļ���
     * @return: QString �ļ���
     * @date  : 2017��04��16��
     */
    virtual const QString GetFileName(void)const;

public slots:


    /**
     * @brief : �������λ���޸Ĵ��ڴ�С
     * @param : x ���ĺ���λ��
     * @param : y ��������λ��
     * @date  : 2017��04��16��
     */
    void SlotOnMouseChange(int x,int y);

    /**
     * @brief : ������Ļ�����а���
     * @date  : 2017��04��16��
     */
    void SlotOnSaveScreen(void);

protected slots:

    /**
     * @brief : ����༭��Ļ������·����
     * @note  : �����·��ѡ��Ĵ���
     * @date  : 2017��04��16��
     */
    void SlotOnSaveScreenOther(void);

    /**
     * @brief : �˳���ǰ��ͼ����
     * @date  : 2017��04��16��
     */
    void SlotQuitScreenshot(void);

private:

    /// �Ƿ��Ѿ����ó�ʼ��С
    static bool     m_bIsInit;
    /// ���ڴ�С�ı�ʱ����¼�ı䷽��
    DIRECTION       m_direction;
    /// ���
    QPoint          m_ptOrigin;
    /// ����Ƿ���
    bool            m_bIsPressed;
    /// �϶��ľ���
    QPoint          m_ptMovePos;
    /// ���ê��
    QPolygon        m_listMarker;
    /// ��Ļԭ��
    std::shared_ptr<QPixmap> m_pOriginPainting;
    /// ��ǰ���ڼ������� ���ڻ��ƽ�ͼ����
    QRect           m_rcCurrent;
    /// �Ҽ��˵�����
    QMenu           *m_pMenu;

};



#endif /// SCREENSHOT_H
