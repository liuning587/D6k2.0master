


#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include <memory>
#include <QWidget>


/**
 * @class : OEAmplifier
 * @brief : 放大取色器
 * @note  : 关于鼠标位置局部放大的操作以及色卡取值的操作
 */
class CAmplifier : public QWidget
{
    Q_OBJECT
public:
    explicit CAmplifier(std::shared_ptr<QPixmap> originPainting, QWidget *parent = 0);

signals:

public slots:

    /**
     * @brief : 大小修改
     * @param : w 宽度
     * @param : h 高度
     * @date  : 2017年4月29日
     */
    void OnSizeChange(int w, int h);
    /**
     * @brief : 大小修改
     * @param : w 宽度
     * @param : h 高度
     * @date  : 2017年4月29日
     */
    void OnPostionChange(int x, int y);
protected:
    /**
     * @brief : 窗口自绘事件
     * @date  : 2017年4月29日
     */
    virtual void paintEvent(QPaintEvent *);
private:
    /// 外部组件的大小信息
    QSize m_sizeScreen;
    /// 鼠标的位置
    QPoint m_ptCursor;
    /// 取色放大器的边长
    int m_nSideLength;
    /// 放大区的高度
    int m_nImageHeight;
    /// 屏幕原画
    std::shared_ptr<QPixmap> m_pOriginPainting;

};

#endif /// AMPLIFIER_H
