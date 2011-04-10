#include <QtGui/QApplication>
#include "form.h"

int main(int argc, char *argv[])
{
	QCoreApplication::setOrganizationName("playerqt");
	QCoreApplication::setApplicationName("playerqt");
//    QCoreApplication::setApplicationVersion("0.0.1");

	QApplication a(argc, argv);

    Form w;
#if defined(Q_WS_S60)
    w.showMaximized();
#else
    w.show();
#endif

    return a.exec();
}
