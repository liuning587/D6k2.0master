
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
 * @brief : 截屏功能的主要入口,
 *          管理全局热键,资源的回收与释放.
 * @remark: 调用示例( CScreenshot::Instance(); )
*/
class CScreenshot : public QWidget
{
    Q_OBJECT

signals:

    /**
     * @brief : 鼠标移动（信号）
     * @param : int x轴的坐标
     * @param : int y轴的坐标
     * @date  : 2017年04月18日
     */
    void cursorPosChange(int, int);

    /**
     * @brief : 双击（信号）
     * @date  : 2017年04月18日
     */
    void doubleClick(void);

    /**
     * @brief : 鼠标当前位置最小的子窗口（信号）
     * @param : QRect 当前窗口的矩形数据
     * @date  : 2017年04月18日
     */
    void findChildWind(QRect);

public:
    /**
     * @brief : 构造函数
     * @note  : 当前依附的父窗口（一般不给父窗口）
     * @date  : 2017年04月16日
     */
    explicit CScreenshot(QWidget *parent = 0);
    ~CScreenshot(void);

    /**
     * @brief : 窗口实例
     * @note  : 通过这个函数获得截图器的整个实例
     * @return: 返回 OEScreenshot 实例指针
     * @date  : 2017年04月15日
     */
    static CScreenshot *Instance(void);

    /**
     * @brief : 摧毁截图窗口
     * @note  : 通过这个函数可以摧毁整个截图窗口
     * @date  : 2017年04月30日
     */
    static void Destroy(void);

protected:
    /**
     * @brief : 隐藏窗口事件
     */
    virtual void hideEvent(QHideEvent *);
    /**
     * @brief : 关闭窗口事件
     */
    virtual void closeEvent(QCloseEvent *);
    /**
     * @brief : 双击事件
     */
    virtual void mouseDoubleClickEvent(QMouseEvent*);
    /**
     * @brief : 鼠标按下事件
     */
    virtual void mousePressEvent(QMouseEvent *);
    /**
     * @brief : 鼠标释放事件
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);
    /**
     * @brief : 鼠标移动事件
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * @brief : 按键按下事件
     */
    virtual void keyPressEvent(QKeyEvent *e);
    /**
     * @brief : 自绘事件
     */
    virtual void paintEvent(QPaintEvent *);

    /**
     * @brief : 更新当前鼠标选区的窗口
     */
    void updateMouse(void);

private:
    /**
     * @brief : 初始化放大镜 (色彩采集器)
     * @note  : 他需要屏幕的原画作为放大器的放大元素
     * @param : originPainting 放大器必要的元素，若为空，则默认用originPainting_原画
     * @date  : 2017年04月15日
     * @remark: 需先行调用getGlobalScreen。
     */
    void InitAmplifier(std::shared_ptr<QPixmap> pOriginPainting = nullptr);
    /**
     * @brief : 测量控件 (大小感知器)
     * @date  : 2017年04月27日
     */
    void InitMeasureWidget(void);
    /**
     * @brief : 初始化截屏背景
     * @return: QPixmap 经过暗色处理的屏幕图
     * @date  : 2017年04月15日
     */
    std::shared_ptr<QPixmap> InitGlobalScreen(void);
    /**
     * @brief : 初始化鼠标
     * @note  : 为鼠标设置默认状态下的图标样式
     * @param : ico 鼠标图片的资源文件路径
     * @date  : 2017年04月15日
     * @remark: 若参数未填写，在使用本程序默认的鼠标Logo
     */
    void InitCursor(const QString& ico = "");

    /**
     * @brief : 创建截图器
     * @note  : 若截图器已存在，则返回截图器示例，不会重复创建。
     * @param : pos 截图器的起始位置 （给当前鼠标位置即可）
     * @date  : 2017年04月16日
     * @remark: 创建截图器前，需要创建相关的组件，(例：大小感知器，放大取色器)
     */
    std::shared_ptr<CScreen> CreateScreen(const QPoint &pos);

    /**
     * @brief : 摧毁截图器
     * @note  : 若截图器已存在，则摧毁示例，并清理示例创建的连带资源
     * @date  : 2017年04月16日
     */
    void DestroyScreen(void);
    /**
     * @brief : 获得当前屏幕的大小
     * @note  : 这个函数是支持多屏幕的，示例：双屏幕 QRect（-1920, 0, 3840, 1080）
     * @return: 返回 QRect 引用
     * @date  : 2017年04月15日
     */
    const QRect& GetScreenRect(void);
    /**
     * @brief : 获得屏幕的原画
     * @note  : 他不会重复获得屏幕原画，如果有，则返回原有的原画
     * @return: QPixmap* 指针
     * @date  : 2017年04月15日
     * @remark: 若想重新获得屏幕原画，需要清理原有资源
     */
    std::shared_ptr<QPixmap> GetGlobalScreen(void);
private:
    /// 截屏窗口是否已经展示
    bool                        m_bIsLeftPressed;
    /// 用于检测误操作
    QPoint                      m_ptStart;
    /// 当前桌面屏幕的矩形数据
    QRect m_rcDesktop;
    /// 屏幕暗色背景图
    std::shared_ptr<QPixmap>    m_pBackgroundScreen;
    /// 屏幕原画
    std::shared_ptr<QPixmap>    m_pOriginPainting;
    /// 截图屏幕
    std::shared_ptr<CScreen>   m_pScreenTool;
    /// 截图器大小感知器
    std::shared_ptr<OERect>     m_pRectTool;
    /// 放大取色器
    std::shared_ptr<CAmplifier> m_pAmplifierTool;
    /// 当前鼠标选区最小的矩形窗口
    QRect                       m_rcWindow;
    /// 截屏实例对象
    static CScreenshot         *m_pSelf;
    /// 置顶定时器
    QTimer                      *m_pEgoisticTimer;
    /// 活动窗口
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
     * @brief : Window下霸道置顶（唯我独尊）
     * @date  : 2017年04月28日
     * @remark: 使用该函数时，会终止右键菜单的行为，慎重使用，避免BUG
     */
    void SlotOnEgoistic(void);
};




/**
 * @class : OERect
 * @brief : 大小感知器
 * @note  : 主要关乎截图器左上方的大小感知控件
*/
class OERect : public QWidget
{
    Q_OBJECT

signals:


public:

    explicit OERect(QWidget *parent = 0);

protected:

    /**
     * @brief : 自绘函数
     */
    void paintEvent(QPaintEvent *);

public slots:

    /**
     * @brief : 外部组件位置修改（槽）
     * @note  : 感知器修改自身位置
     * @param : x 横向位置
     * @param : y 纵向位置
     * @date  : 2017年04月15日
     */
    void OnPostionChange(int x, int y);

    /**
     * @brief : 外部组件大小修改 （槽）
     * @note  : 感知器修改显示的大小数据
     * @param : w 宽度
     * @param : h 高度
     * @date  : 2017年04月15日
     */
    void OnSizeChange(int w, int h);

private:
    /// 背景色
    std::shared_ptr<QPixmap>    m_pBackgroundPixmap;
    /// 显示的文字信息
    QString                     m_szInfo;
};


/**
 * @class : CScreen
 * @brief : 截图器
 * @note  : 主要关乎图片的编辑与保存
*/
class CScreen : public QWidget 
{

    Q_OBJECT

signals:


    /**
     * @brief : 截图器大小修改（信号）
     * @param : int 宽度
     * @param : int 高度
     * @date  : 2017年04月17日
     */
    void sizeChange(int,int);

    /**
     * @brief : 截图器窗口的位置（信号）
     * @param : int 窗口的横向位置
     * @param : int 窗口的纵向位置
     * @date  : 2017年04月17日
     */
    void postionChange(int,int);

    /**
     * @brief : 双击 （信号）
     * @date  : 2017年04月17日
     */
    void doubleClick(void);

protected:

    /// 内边距，决定拖拽的触发。
    const int INNER_PADDING = 6;

    /// 方位枚举
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
     * @brief : 获得当前截图器是否存在
     * @return: true : 存在
     * @date  : 2017年04月17日
     */
    static bool GetState(void) 
	{
		return m_bIsInit; 
	}

protected:

    /**
     * @brief : 获得当前坐标点的边界方位
     * @param : cursor 当前鼠标的位置
     * @return: DIRECTION 鼠标的方位枚举
     * @date  : 2017年04月17日
     */
    DIRECTION GetRegion(const QPoint &cursor);


    /**
     * @brief : 呼出菜单事件
     */
    virtual void contextMenuEvent(QContextMenuEvent *);

    /**
     * @brief : 双击事件
     */
    virtual void mouseDoubleClickEvent(QMouseEvent *e);

    /**
     * @brief : 鼠标按下事件
     */
    virtual void mousePressEvent(QMouseEvent *e);

    /**
     * @brief : 鼠标释放事件
     */
    virtual void mouseReleaseEvent(QMouseEvent *e);
    /**
     * @brief : 鼠标移动事件
     */
    virtual void mouseMoveEvent(QMouseEvent *e);

    /**
     * @brief : 窗口移动事件
     */
    virtual void moveEvent(QMoveEvent *);

    /**
     * @brief : 窗口大小修改事件
     */
    virtual void resizeEvent(QResizeEvent *);


    /**
     * @brief : 窗口显示事件
     */
    virtual void showEvent(QShowEvent *);

    /**
     * @brief : 窗口隐藏事件
     */
    virtual void hideEvent(QHideEvent *);

    /**
     * @brief : 鼠标进入窗口事件
     */
    virtual void enterEvent(QEvent *e);

    /**
     * @brief : 鼠标离开窗口事件
     */
    virtual void leaveEvent(QEvent *e);

    /**
     * @brief : 窗口关闭事件
     */
    virtual void closeEvent(QCloseEvent *);

    /**
     * @brief : 界面自绘事件
     */
    virtual void paintEvent(QPaintEvent *);

private:

    /**
     * @brief : 获得一个以时间格式命名的文件名
     * @return: QString 文件名
     * @date  : 2017年04月16日
     */
    virtual const QString GetFileName(void)const;

public slots:


    /**
     * @brief : 根据鼠标位置修改窗口大小
     * @param : x 鼠标的横向位置
     * @param : y 鼠标的纵向位置
     * @date  : 2017年04月16日
     */
    void SlotOnMouseChange(int x,int y);

    /**
     * @brief : 保存屏幕到剪切板中
     * @date  : 2017年04月16日
     */
    void SlotOnSaveScreen(void);

protected slots:

    /**
     * @brief : 保存编辑屏幕到其他路径下
     * @note  : 会呼出路径选择的窗口
     * @date  : 2017年04月16日
     */
    void SlotOnSaveScreenOther(void);

    /**
     * @brief : 退出当前截图窗口
     * @date  : 2017年04月16日
     */
    void SlotQuitScreenshot(void);

private:

    /// 是否已经设置初始大小
    static bool     m_bIsInit;
    /// 窗口大小改变时，记录改变方向
    DIRECTION       m_direction;
    /// 起点
    QPoint          m_ptOrigin;
    /// 鼠标是否按下
    bool            m_bIsPressed;
    /// 拖动的距离
    QPoint          m_ptMovePos;
    /// 标记锚点
    QPolygon        m_listMarker;
    /// 屏幕原画
    std::shared_ptr<QPixmap> m_pOriginPainting;
    /// 当前窗口几何数据 用于绘制截图区域
    QRect           m_rcCurrent;
    /// 右键菜单对象
    QMenu           *m_pMenu;

};



#endif /// SCREENSHOT_H
