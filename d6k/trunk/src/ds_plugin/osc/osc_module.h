#ifndef OSC_MODULE_H
#define OSC_MODULE_H


#include "ds_global.h"
#include "devicestudio/plugin_module.h"

#include <QMap>

#include <memory>
#include <QToolBar>
#include <QAction>
#include <QSplitter>
#include <QToolButton>

class IMainWindow;

class CComtrade;

class DS_SHARED_EXPORT COscillogramModule : public IPluginModule
{
	Q_OBJECT
public:
	COscillogramModule();
	~COscillogramModule();
	//往树结构中插入数据
	void CreateTreeItem();

	bool LogString(const char *pszMoudleName, const char *szLogTxt, int nLevel);
	bool CommMsgLog(const char *pszModuleName, const char *pMsg, int nMsgLen, unsigned int nMsgType, const char * pMsgSubType, const char *pSource, const char *pDesc);

public:
	virtual void Init(IMainModule *pCore);
	virtual void UnInit();
	//设置加载的插件的名称
	virtual void SetLoadDeviceName(const QString &strDeviceName);
protected:
	void ShowMenu(QModelIndex &index);
	void ShowRootMenu(QModelIndex &index);

public slots:
	void Slot_ClickLeftTreeItem(const QModelIndex &mIndex);
	void Slot_CloseOneTabWidget(int iIndex);
	void showMouseRightButton(const QPoint &point);


	void SlotOpenFile();
	void SlotCloseCurFile();

	void SlotScreenShot();
	void SlotAbout();

	void OpenDataFile(const QString &szFileName);
	void LoadDataFile(const QString &szFileName);

	void CreateToolBar();

	void CreateActions();

	void Destroy();
private:
	//模块加载工具
	IMainModule *m_pMainModule;
	//主界面加载控件对象
	IMainWindow *m_pMainWindow;
	//树结构 编号  对应的tab页
	QMap<int, QWidget*> m_mapIndexTab;
	//树结构 tab页  编号
	QMap<QWidget*, int> m_mapTabIndex; 

	QString m_szCurFile;
	QStringList m_lstOpenedFileList;

	QToolBar *m_pToolBar;

	QAction *m_pOpenAct;
	QAction *m_pCloseAct;
	QAction *m_pOpenSpcAct;
	 
	QAction *m_pSaveAsAct;
	QAction *m_pPrintAct;
	QAction *m_pPrintHdrAct;
	QAction* m_pPrintActionInforAct;
	QAction* m_pActPdf;
	QAction* m_pActAIPdf;
	QAction* m_pActABPdf;
	QAction* m_pPrintActionBehaviorAct;
	QAction *m_pPrintPreviewAct;
	QAction *m_pPrintSetAct;
	//QAction *recentFilesAct;
	QAction *m_pQuitAct;

	//actions for view menu
	QAction *m_pToolBarAct;
	QAction *m_pStatusBarAct;
	QAction *m_pSeparateAct;

	//actions for diagram operation menu
	QAction *m_pZoomInAct;
	QAction *m_pZoomOutAct;
	QAction *m_pRecoverAct;
	QAction *m_pHStretchAct;
	QAction *m_pVZoomAct;
	QAction *m_pCopyCurveAct;
	QAction *m_pExchangeCurveAct;
	QAction *m_pAmplitudeAct;
	QAction *m_pCurveColorAct;

	//actions for fault analysis menu
	QAction *m_pVectorAnalyseAct;
	QAction *m_pHarmonicAnalyseAct;
	QAction *m_pTelemeterAct;
	QAction *m_pViewFileParamAct;
	QAction *m_pViewChannelParamAct;

	//actions for user manage menu
// 	QAction *m_pReLogonAct;
// 	QAction *manageUserAct;

	//actions for help menu
	QAction *m_pUsageAct;
	QAction *m_pAboutAct;

	//extra actions
	QAction *m_pCurveCascadeAct;
	QAction *m_pSequenceAnalyseAct;
	QAction *m_pCurveAct;
	QAction *m_pCommunicationAct;

	QToolButton* m_pToolButton;

private:

	std::vector<std::shared_ptr<CComtrade>> m_arrWaves;

};


#endif // OSC_MODULE_H
