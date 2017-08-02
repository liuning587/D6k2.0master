#include "logic_manager.h"
#include <QFile>
#include <QMessageBox>

CLogicManager::CLogicManager(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	setWindowTitle(QStringLiteral("��¼����"));
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
	//��ȷ�Լ��
	if (!m_mapUserPassWd.contains(ui.lineEdit_6->text()))
	{
		QMessageBox::warning(this, QStringLiteral("�澯"),QStringLiteral("�û�������!"));
		return;
	}

	if (m_mapUserPassWd.contains(ui.lineEdit_6->text()))
	{
		if (m_mapUserPassWd[ui.lineEdit_6->text()] != ui.lineEdit_2->text())
		{
			QMessageBox::warning(this, QStringLiteral("�澯"), QStringLiteral("�������!"));
			return;
		}
	}

	if (ui.lineEdit_4->text().isEmpty() || ui.lineEdit_3->text().isEmpty() || ui.lineEdit_5->text().isEmpty())
	{
		QMessageBox::warning(this, QStringLiteral("�澯"), QStringLiteral("�û������벻��Ϊ��!"));
		return;

	}

	if (ui.lineEdit_3->text() != ui.lineEdit_5->text())
	{
		QMessageBox::warning(this, QStringLiteral("�澯"), QStringLiteral("�����������벻ͬ!"));
		return;
	}


	//�����û��ļ�
	QFile tt(qApp->applicationDirPath() + LOGIC_MANAGER_FILE);

	if (tt.open(QIODevice::WriteOnly| QIODevice::Text))
	{
		QByteArray tttt = ui.lineEdit_4->text().toLocal8Bit() + ":" + ui.lineEdit_5->text().toLocal8Bit();

		tt.write(tttt.toBase64());
		tt.close();

		QMessageBox::information(0, tr("�ɹ�"), QStringLiteral("��¼��Ϣ�޸ĳɹ�"));
		accept();
	}
}
