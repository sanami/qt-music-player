#include <QtGui/QApplication>
#include "form.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

    Form w;
#if defined(Q_WS_S60)
    w.showMaximized();
#else
    w.show();
#endif

    return a.exec();
}
