#include "singlecontrol.h"
#include "apdurecver.h"
#include "apdusender.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);



	CSingleControl::GetInstance()->GetWgt()->show();
    return a.exec();
}
