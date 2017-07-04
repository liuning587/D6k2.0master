#pragma once

#include <QDialog>
#include <memory>
#include <QXmlStreamReader>
#include "ui_browsescreenwgt.h"

class CBrowseScreenWgt : public QDialog
{
	Q_OBJECT

public:
	enum 
	{
		E_CATALOG,   //目录
		E_FILENAME ,   //文件
	};
	CBrowseScreenWgt(QWidget *parent = Q_NULLPTR);
	~CBrowseScreenWgt();
	//加载街结构
	void LoadWindowData(const QString &strProjectFileName);
	//解析文件
	void AnalyseObjectFile(std::shared_ptr<QXmlStreamReader> xmlReader);
	//解析所有节点
	void AnalyseAllNode(std::shared_ptr<QXmlStreamReader> xmlReader);
	//解析group节点
	void AnalyseGroupNode(std::shared_ptr<QXmlStreamReader> xmlReader, QString strFatherName, QTreeWidgetItem *pFather);
public slots:
    void Slot_Conform();
public:
	const QString &GetFilename()
	{
		return m_strFilename;
	}

private:
	Ui::CBrowseScreenWgt ui;
	//文件名
	QString m_strFilename;
};
