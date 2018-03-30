
#include "common_helper.h"

#include <QFile>
#include <QTranslator>
#include <QApplication>
#include <QWidget>
#include <QDesktopWidget>

#ifdef Q_OS_WIN32
#include <windows.h>
#endif

#ifndef QT_NO_DEBUG
#include <QDebug>
#endif


#define WINDOW_BASESIZE_WIDTH (1920)
#define WINDOW_BASESIZE_HEIGHT (1080)

float CCommonHelper::m_fWidthMultiplyingPower = 0;
float CCommonHelper::m_fHeightMultiplyingPower = 0;

void CCommonHelper::SetStyle(const QString &style)
{
    QFile qss(style);
    qss.open(QFile::ReadOnly);
    qApp->setStyleSheet(qss.readAll());
    qss.close();
}

void CCommonHelper::SetLanguagePack(const QString &szLanguage) 
{
    // 加载中文包
    QTranslator translator;
    translator.load(szLanguage);
    qApp->installTranslator(&translator);
}

void CCommonHelper::MoveCenter(QWidget *pWidget, QRect parentRect)
{
    if (parentRect.isEmpty())
	{
        parentRect = QApplication::desktop()->rect();
    }
    pWidget->move (((parentRect.width() - pWidget->width()) >> 1),
          ((parentRect.height() - pWidget->height()) >> 1));
}

const float &CCommonHelper::GetWindowWidthMultiplyingPower()
{
    if (m_fWidthMultiplyingPower == 0) 
	{
        UpWindowSizeMultiplyingPower();
    }
    return m_fWidthMultiplyingPower;
}

const float & CCommonHelper::GetWindowHeightMultiplyingPower() 
{
    if (m_fHeightMultiplyingPower == 0)
	{
        UpWindowSizeMultiplyingPower();
    }
    return m_fHeightMultiplyingPower;
}

void CCommonHelper::UpWindowSizeMultiplyingPower()
{
	QSize temp_size = QApplication::desktop()->size();
	m_fWidthMultiplyingPower = (float)temp_size.width()
		/ (float)WINDOW_BASESIZE_WIDTH;
	m_fHeightMultiplyingPower = (float)temp_size.height()
		/ (float)WINDOW_BASESIZE_HEIGHT;
}
/*! \fn bool CCommonHelper::GetSmallestWindowFromCursor(QRect& rcOut)
********************************************************************************************************* 
** \brief CCommonHelper::GetSmallestWindowFromCursor 
** \details 获得当前鼠标位置最小的子窗口
** \param rcOut 
** \return bool 
** \author LiJin 
** \date 2018年3月5日 
** \note 
********************************************************************************************************/
bool CCommonHelper::GetSmallestWindowFromCursor(QRect& rcOut)
{
#ifdef Q_OS_WIN32
    HWND hwnd;
    POINT pt;
    // 获得当前鼠标位置
    ::GetCursorPos(&pt);
    // 获得当前位置桌面上的子窗口
    hwnd = ::ChildWindowFromPointEx(::GetDesktopWindow(),
                        pt, CWP_SKIPDISABLED | CWP_SKIPINVISIBLE);
    if (hwnd != nullptr) 
	{
        HWND temp_hwnd;
        temp_hwnd = hwnd;
        while (true) 
		{
            ::GetCursorPos(&pt);
            ::ScreenToClient(temp_hwnd, &pt);
            temp_hwnd = ::ChildWindowFromPointEx(temp_hwnd,
                                     pt, CWP_SKIPINVISIBLE);
            if (temp_hwnd == nullptr || temp_hwnd == hwnd)
                break;
            hwnd = temp_hwnd;
        }
        RECT rcTempWindow;
        ::GetWindowRect(hwnd, &rcTempWindow);
        rcOut.setRect(rcTempWindow.left,rcTempWindow.top,
                         rcTempWindow.right - rcTempWindow.left,
                         rcTempWindow.bottom - rcTempWindow.top);
        return true;
    }
#endif
    return false;
}
/*! \fn bool CCommonHelper::GetCurrentWindowFromCursor(QRect &rcOut)
********************************************************************************************************* 
** \brief CCommonHelper::GetCurrentWindowFromCursor 
** \details 获得当前鼠标位置的窗口
** \param out_rect 
** \return bool 
** \author LiJin 
** \date 2018年3月5日 
** \note 
********************************************************************************************************/
bool CCommonHelper::GetCurrentWindowFromCursor(QRect &rcOut)
{
#ifdef Q_OS_WIN32
    HWND hwnd;
    POINT pt;
    // 获得当前鼠标位置
    ::GetCursorPos(&pt);
    // 获得当前位置桌面上的子窗口
    hwnd = ::ChildWindowFromPointEx(::GetDesktopWindow(), pt,
                        CWP_SKIPDISABLED | CWP_SKIPINVISIBLE);
    if (hwnd != nullptr) 
	{
        RECT rcTempWindow;
        ::GetWindowRect(hwnd, &rcTempWindow);
        rcOut.setRect(rcTempWindow.left, rcTempWindow.top,
                         rcTempWindow.right - rcTempWindow.left,
                         rcTempWindow.bottom - rcTempWindow.top);
        return true;
    }
#endif
    return false;
}
