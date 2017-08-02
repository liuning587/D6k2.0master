#include "predicition_wgt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CPredicitionWgt w;
	w.show();
	return a.exec();
}
