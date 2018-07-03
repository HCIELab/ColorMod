#include "ColorFab.h"
#include "Config.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ColorFab w;

	// -- Setup window geometry
	w.setGeometry(M_APP_QT_POS_X,
				  M_APP_QT_POS_Y,
				  w.size().width(),
				  w.size().height());

	// -- Show the application
	w.show();

	return a.exec();
}
