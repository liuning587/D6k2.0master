#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H
#include <QMainWindow> 
#include <QWidget>
#include <memory>
#include <vector>
#include <functional>
class CGraphFile;
class QTreeWidget;
class CRealTimeTask;
class CGraphScene;
class CRealTimeView;
class QTreeWidgetItem;
class QDockWidget;
class QComboBox;
#define USER_ROLE_ROOT Qt::UserRole + 1
#define USER_GRP_ROLE  Qt::UserRole + 2
#define USER_FILE_ROLE Qt::UserRole + 3

#define PREDICT_SHORT_CURVE_ROLE       Qt::UserRole+10   //短期预测曲线
#define PREDICT_Ultra_SHORT_CURVE_ROLE Qt::UserRole+11   //超短期预测曲线
#define PREDICT_HIS_CURVE_ROLE         Qt::UserRole+12   //历史曲线
#define PREDICT_LimitPower_ROLE        Qt::UserRole+13   //限电设置
#define PREDICT_STATICS_ROLE           Qt::UserRole+14   //统计
#define PREDICT_VARIANCE_ROLE          Qt::UserRole+15   //方差等

enum PredDataType
{
	PREDICT_SHORT_CURVE = 100,
	PREDICT_Ultra_SHORT_CURVE,
	PREDICT_HIS_CURVE,
	PREDICT_LimitPower,
	PREDICT_STATICS,
	PREDICT_VARIANCE
};

typedef struct
{
	std::string szFileName;
	std::string szFilePath;

}GraphFileDataUnit;

using SIM_GRAPH_DEF = std::shared_ptr < GraphFileDataUnit >;


class CMainFrame : public QMainWindow 
{
	Q_OBJECT
public:
	CMainFrame(QWidget *pParent=nullptr);
	virtual ~CMainFrame();

	void closeEvent(QCloseEvent * event);

public:
	bool Initalize(std::vector < SIM_GRAPH_DEF >& arrFiles);
	void Run();
	void ShutDown();


public:
	void AddView(CGraphFile * pGraphFile);
	void SetProjPath(const QString& szProjPath)
	{
		m_szProjPath = szProjPath;
	}
	QString& GetProjPath()  
	{
		return  m_szProjPath;
	}
protected:
	//加载文件由谁来实现?!
	void InitTreeWidget();
	void InitPredictTreeView();
	void InitMainWindow();
	void InitMenuBar();
	void InitToolBar();
	bool LoadAppDlls();
private:
	QAction * m_pShowViewAction;
	QAction * m_pHideViewAction;
	QAction * m_pFullScreenAction;
	QAction * m_pNomalScreenAction;
protected:
    void resizeEvent(QResizeEvent *event);
	void SetCurrentFile(const QString& szName);
	//CBaseWidget* HitTest(QPoint & pos, unsigned int nLayerIdx);
public slots: 
	void OnDelete();
	void slot_OnTreeDbClicked(QTreeWidgetItem *item, int column);
	void slot_OnPredictTreeDbClicked(QTreeWidgetItem *item, int column);
	void slot_OnShowView();
	void slot_OnHideView();
	void slot_FullScreenView();
	void slot_NormalScreenView();
	void slot_ComBoBoxChanged(int nIndex);
	void slot_BtnOrder(const QString& tagName,int nType);
private:
	QTabWidget  *m_pTabWidget;
	QTabWidget  *m_pLeftTabWidget;
	QTreeWidget* m_pTreeWidget;
	QTreeWidget* m_pPredictTreeWidget;
	QDockWidget * m_pDockWidget;
	QComboBox* m_pComBox;
private:
	QString m_szProjPath; 	
	//当前页面
	CGraphFile * m_pGraphFile;
	CGraphScene *  m_pCurScene;
	CRealTimeView * m_pCurView;
	std::vector < SIM_GRAPH_DEF > m_arrFiles;

	std::vector<CRealTimeTask*> m_arrTasks;
	//图层
	int m_nIndexLayer;
	//是否正在执行命令
	bool m_bIsOrder;

private:
	//预测曲线
	std::function< QWidget* () > m_funPrecictMainWidget;
	std::function<void()> m_funcPredictCloseWindow; 
};

#endif // GRAPH_TAB_VIEW_H
