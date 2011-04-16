#include <QTest>
#include <QNetworkAccessManager>
#include "test.h"
#include "../../settings.h"
#include "../../mplayer_control.h"
#include "../../cookie_jar.h"
#include "../../web.h"


Test::Test(QObject *parent)
	: QObject(parent)
{
	QCoreApplication::setOrganizationName("playerqt");
	QCoreApplication::setApplicationName("test");
}

void Test::test_settings()
{
	Settings settings;
	qDebug() << settings.server();

#if 0
	{
		// Сохраняется в $HOME/.config/sas/test_settings.conf
		Settings settings("player_qt", "test_settings");
		settings.setValue("editor/wrapMargin", 68);
	}
	{
		QSettings settings("player_qt", "test_settings");
		int margin = settings.value("editor/wrapMargin").toInt();

		QVERIFY( margin == 68 );
	}
#endif

}

void Test::test_mplayer()
{
	MPlayerControl *player = new MPlayerControl(NULL);

	player->open("http://broadcast.infomaniak.net:80/funradiobe-high.mp3");

	qDebug() << "sleep 3";
	sleep(3);

	player->close();

	qDebug() << "sleep 1";
	sleep(1);

	delete player;
}

void Test::test_cookie_jar()
{
//	QNetworkAccessManager *network = new QNetworkAccessManager();
//	network->setCookieJar(new CookieJar(this));

//	network->get("http://localhost:3000/stations/1440");


//	delete network;

}

void Test::test_convert_variant_map_to_post_params()
{
	Web web;

	QVariantMap params;
	QVariantMap playlist;
	playlist["playlist_type_id"] = 2;
	playlist["station_id"] = 11;
	params["playlist"] = playlist;
	QByteArray result = web.toParams(params);

	qDebug() << result;
	QVERIFY( result == "playlist[playlist_type_id]=2&playlist[station_id]=11" );
}

QTEST_MAIN(Test);
