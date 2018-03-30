


#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include <memory>
#include <QWidget>


/**
 * @class : OEAmplifier
 * @brief : �Ŵ�ȡɫ��
 * @note  : �������λ�þֲ��Ŵ�Ĳ����Լ�ɫ��ȡֵ�Ĳ���
 */
class CAmplifier : public QWidget
{
    Q_OBJECT
public:
    explicit CAmplifier(std::shared_ptr<QPixmap> originPainting, QWidget *parent = 0);

signals:

public slots:

    /**
     * @brief : ��С�޸�
     * @param : w ���
     * @param : h �߶�
     * @date  : 2017��4��29��
     */
    void OnSizeChange(int w, int h);
    /**
     * @brief : ��С�޸�
     * @param : w ���
     * @param : h �߶�
     * @date  : 2017��4��29��
     */
    void OnPostionChange(int x, int y);
protected:
    /**
     * @brief : �����Ի��¼�
     * @date  : 2017��4��29��
     */
    virtual void paintEvent(QPaintEvent *);
private:
    /// �ⲿ����Ĵ�С��Ϣ
    QSize m_sizeScreen;
    /// ����λ��
    QPoint m_ptCursor;
    /// ȡɫ�Ŵ����ı߳�
    int m_nSideLength;
    /// �Ŵ����ĸ߶�
    int m_nImageHeight;
    /// ��Ļԭ��
    std::shared_ptr<QPixmap> m_pOriginPainting;

};

#endif /// AMPLIFIER_H
