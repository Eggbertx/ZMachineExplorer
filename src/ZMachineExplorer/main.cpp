#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	a.setOrganizationName("Eggbertx");
	a.setOrganizationDomain("eggbertx.github.io");
	a.setApplicationName("Z-Machine Explorer");
	a.setApplicationVersion(ZME_VERSION);
    MainWindow w;
    w.show();
    return a.exec();
}
