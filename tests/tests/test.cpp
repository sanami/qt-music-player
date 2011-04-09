#include <QTest>
#include <QSettings>
#include "test.h"
#include "../../player_control.h"


Test::Test(QObject *parent)
	: QObject(parent)
{
}

void Test::test_settings()
{
	{
		// Сохраняется в $HOME/.config/sas/test_settings.conf
		QSettings settings("player_qt", "test_settings");
		settings.setValue("editor/wrapMargin", 68);
	}
	{
		QSettings settings("player_qt", "test_settings");
		int margin = settings.value("editor/wrapMargin").toInt();

		QVERIFY( margin == 68 );
	}

}

void Test::test_mplayer()
{
	PlayerControl *player = new PlayerControl(NULL);

	player->open("http://broadcast.infomaniak.net:80/funradiobe-high.mp3");

	qDebug() << "sleep 3";
	sleep(3);

	player->close();

	qDebug() << "sleep 1";
	sleep(1);

	delete player;
}

QTEST_MAIN(Test);
