#ifndef CHILD_FRM_H
#define CHILD_FRM_H
 
#include <QMainWindow>
#include <QWidget>
#include <QSplitter>
#include <memory>
 

class CLeftView;
class CRightView;
class CComtrade;
class CChannelViewInfo;
 
class CChildFrm : public QMainWindow
{
	Q_OBJECT
public:
	CChildFrm(std::shared_ptr<CComtrade> pWave, QWidget *parent);
	~CChildFrm();

public:
	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	std::shared_ptr<CLeftView> GetLeftView()
	{
		return m_pLeftView;
	}

	std::shared_ptr<CRightView> GetRightView()
	{
		return m_pRightView;
	}
//signals:

//public slots :
//	void OnCoilDoubleClicked(const QModelIndex & index);
protected:
	void Destroy();
 

private:
	QSplitter * m_pMainSplitter;
// 	QSplitter * m_pLeftSplitter;
// 	QSplitter * m_pRightSplitter;

	std::shared_ptr<CLeftView>   m_pLeftView;
	std::shared_ptr<CRightView>  m_pRightView;

	std::shared_ptr<CComtrade>  m_pWave;

	std::shared_ptr<CChannelViewInfo> m_pChannelViewInfo;
};

#endif // CHILD_FRM_H