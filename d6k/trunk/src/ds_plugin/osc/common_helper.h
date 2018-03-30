

#ifndef COMMONHELPER_H
#define COMMONHELPER_H
#include <QString>
#include <QRect>

class QWidget;

/**
 * @class : CCommonHelper
 * @brief : 通用助手
 * @note  : 完成一些比较常用又通用的功能
 */
class CCommonHelper
{
public:

    /**
    * @brief : 设置QSS文件
    * @param : style 文件名
    * @date  : 2017年04月10日
    * @remark: 如果是qrc路径，请带上 qrc:/
    **/
    static void SetStyle(const QString &style);

    /**
    * @brief : 设置语言包
    * @param : language 语言包的文件名
    * @date  : 2017年04月10日
    **/
    static void SetLanguagePack(const QString& szLanguage);

    /**
    * @brief : 将窗口移动到中心
    * @param : widget 要移动的窗口
    * @param : parentRect 矩形几何数据
    * @date  : 2017年04月10日
    **/
    static void MoveCenter(QWidget* pWidget, QRect parentRect = {});


    /**
    * @brief : 获得当前界面与开发时的界面之间的横向倍率
    * @return: float 倍率
    * @date  : 2017年04月10日
    **/
    static const float& GetWindowWidthMultiplyingPower(void);


    /**
    * @brief : 获得当前界面与开发时的界面之间的纵向倍率
    * @return: float 倍率
    * @date  : 2017年04月10日
    **/
    static const float& GetWindowHeightMultiplyingPower(void);


    /**
    * @brief : 获得当前鼠标位置最小的子窗口
    * @param : out_rect 矩形 (返回)
    * @return: 成功:true
    * 
    * @date  : 2017年04月10日
    **/
    static bool GetSmallestWindowFromCursor(QRect &out_rect);

    /**
    * @brief : 获得当前鼠标位置的窗口
    * @param : out_rect 矩形 (返回)
    * @return: 成功:true
    * 
    * @date  : 2017年04月10日
    **/
    static bool GetCurrentWindowFromCursor(QRect &out_rect);

protected:

    /**
    * @brief : 更新窗口倍率
    * 
    * @date  : 2017年04月10日
    **/
    static void UpWindowSizeMultiplyingPower(void);


private:
    /// 窗口横向倍率
    static float m_fWidthMultiplyingPower;
    /// 窗口纵向倍率
    static float m_fHeightMultiplyingPower;
};

#endif /// COMMONHELPER_H
