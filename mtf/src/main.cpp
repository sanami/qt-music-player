#include <MApplication>
#include "logger.h"
#include "app.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("playermtf");
    QCoreApplication::setApplicationName("playermtf");

    MApplication a(argc, argv, QString("playermtf"));

	Logger::init();

    App app;


    return a.exec();
}
