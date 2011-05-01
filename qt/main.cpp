#include <QtGui/QApplication>
#include "app.h"
#include "logger.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("playerqt");
	QCoreApplication::setApplicationName("playerqt");
//    QCoreApplication::setApplicationVersion("0.0.1");

	QApplication a(argc, argv);

	Logger::init();

	App app;

    return a.exec();
}
