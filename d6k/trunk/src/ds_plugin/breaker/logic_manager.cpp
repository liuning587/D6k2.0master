#include "logic_manager.h"
#include <QFile>
#include <QMessageBox>

CLogicManager::CLogicManager(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle(QStringLiteral("登录管理"));
	ui.lineEdit_2->setEchoMode(QLineEdit::Password);
	ui.lineEdit_3->setEchoMode(QLineEdit::Password);
	ui.lineEdit_5->setEchoMode(QLineEdit::Password);
	connect(ui.pushButton,SIGNAL(clicked()),this,SLOT(Slot_Conform()));
	InitData();
}

void CLogicManager::InitData()
{
	QFile tt(qApp->applicationDirPath() + LOGIC_MANAGER_FILE);

	if (tt.open(QIODevice::ReadOnly))
	{
		QByteArray btdata = tt.readAll();
		QString strUser = QByteArray::fromBase64(btdata);

		QStringList lstUsers = strUser.split("\r\n");

		for (auto struser : lstUsers)
		{
			m_mapUserPassWd.insert(struser.split(":").first(), struser.split(":").last());
		}

		tt.close();

	}
}

CLogicManager::~CLogicManager()
{
}

void CLogicManager::Slot_Conform()
{
	//正确性检查
	if (!m_mapUserPassWd.contains(ui.lineEdit_6->text()))
	{
		QMessageBox::warning(this, QStringLiteral("告警"),QStringLiteral("用户名错误!"));
		return;
	}

	if (m_mapUserPassWd.contains(ui.lineEdit_6->text()))
	{
		if (m_mapUserPassWd[ui.lineEdit_6->text()] != ui.lineEdit_2->text())
		{
			QMessageBox::warning(this, QStringLiteral("告警"), QStringLiteral("密码错误!"));
			return;
		}
	}

	if (ui.lineEdit_4->text().isEmpty() || ui.lineEdit_3->text().isEmpty() || ui.lineEdit_5->text().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("告警"), QStringLiteral("用户名密码不能为空!"));
		return;

	}

	if (ui.lineEdit_3->text() != ui.lineEdit_5->text())
	{
		QMessageBox::warning(this, QStringLiteral("告警"), QStringLiteral("两次密码输入不同!"));
		return;
	}


	//更新用户文件
	QFile tt(qApp->applicationDirPath() + LOGIC_MANAGER_FILE);

	if (tt.open(QIODevice::WriteOnly| QIODevice::Text))
	{
		QByteArray tttt = ui.lineEdit_4->text().toLocal8Bit() + ":" + ui.lineEdit_5->text().toLocal8Bit();

		tt.write(tttt.toBase64());
		tt.close();

		QMessageBox::information(0, tr("成功"), QStringLiteral("登录信息修改成功"));
		accept();
	}
}
