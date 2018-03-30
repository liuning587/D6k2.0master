

#ifndef COMMONHELPER_H
#define COMMONHELPER_H
#include <QString>
#include <QRect>

class QWidget;

/**
 * @class : CCommonHelper
 * @brief : ͨ������
 * @note  : ���һЩ�Ƚϳ�����ͨ�õĹ���
 */
class CCommonHelper
{
public:

    /**
    * @brief : ����QSS�ļ�
    * @param : style �ļ���
    * @date  : 2017��04��10��
    * @remark: �����qrc·��������� qrc:/
    **/
    static void SetStyle(const QString &style);

    /**
    * @brief : �������԰�
    * @param : language ���԰����ļ���
    * @date  : 2017��04��10��
    **/
    static void SetLanguagePack(const QString& szLanguage);

    /**
    * @brief : �������ƶ�������
    * @param : widget Ҫ�ƶ��Ĵ���
    * @param : parentRect ���μ�������
    * @date  : 2017��04��10��
    **/
    static void MoveCenter(QWidget* pWidget, QRect parentRect = {});


    /**
    * @brief : ��õ�ǰ�����뿪��ʱ�Ľ���֮��ĺ�����
    * @return: float ����
    * @date  : 2017��04��10��
    **/
    static const float& GetWindowWidthMultiplyingPower(void);


    /**
    * @brief : ��õ�ǰ�����뿪��ʱ�Ľ���֮���������
    * @return: float ����
    * @date  : 2017��04��10��
    **/
    static const float& GetWindowHeightMultiplyingPower(void);


    /**
    * @brief : ��õ�ǰ���λ����С���Ӵ���
    * @param : out_rect ���� (����)
    * @return: �ɹ�:true
    * 
    * @date  : 2017��04��10��
    **/
    static bool GetSmallestWindowFromCursor(QRect &out_rect);

    /**
    * @brief : ��õ�ǰ���λ�õĴ���
    * @param : out_rect ���� (����)
    * @return: �ɹ�:true
    * 
    * @date  : 2017��04��10��
    **/
    static bool GetCurrentWindowFromCursor(QRect &out_rect);

protected:

    /**
    * @brief : ���´��ڱ���
    * 
    * @date  : 2017��04��10��
    **/
    static void UpWindowSizeMultiplyingPower(void);


private:
    /// ���ں�����
    static float m_fWidthMultiplyingPower;
    /// ����������
    static float m_fHeightMultiplyingPower;
};

#endif /// COMMONHELPER_H
