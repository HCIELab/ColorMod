#include "ColorFabPainting.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ColorFabPainting w;
	w.show();
	return a.exec();
}
