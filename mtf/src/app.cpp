#include "app.h"
#include "app_window.h"
#include "web.h"
#include "media.h"
#include "logger.h"
#include "log_page.h"
#include "stations_page.h"
#include "station_page.h"
#include "filter_page.h"

App::App(QObject *parent)
	: QObject(parent)
{
	// Отладочные сообщения
	m_log_page = new LogPage();
	connect(Logger::logger(), SIGNAL(sig_debug(QString)), m_log_page, SLOT(addLog(QString)));

	// Веб-запросы
	m_web = new Web();
	connect(m_web, SIGNAL(sig_finished(Task *)), SLOT(on_web_finished(Task *)));
	connect(m_web, SIGNAL(sig_busy(bool)), SLOT(showBusy(bool)));

	// Управление плеером
	m_media = new Media(this);
	connect(m_media, SIGNAL(sig_status(QVariantMap, QString, bool)), SLOT(on_media_status(QVariantMap, QString, bool)));

	// Окно приложения
	m_app_window = new AppWindow();

	// Добавить страницы
	m_filter_page = new FilterPage();
	connect(m_filter_page, SIGNAL(sig_setServer(QString)), SLOT(on_setServer(QString)));
	connect(m_filter_page, SIGNAL(sig_requestCities(int)), m_web, SLOT(requestCities(int)));
	connect(m_filter_page, SIGNAL(sig_requestPage(int)), SLOT(on_requestPage(int)));

	m_stations_page = new StationsPage();
	connect(m_app_window, SIGNAL(orientationChanged(M::Orientation)), m_stations_page, SLOT(on_orientationChanged(M::Orientation)));
	connect(m_stations_page, SIGNAL(sig_requestPage(int)), SLOT(on_requestPage(int)));
	connect(m_stations_page, SIGNAL(sig_showStation(Station)), SLOT(on_showStationPage(Station)));

	m_station_page = new StationPage();
	connect(m_station_page, SIGNAL(sig_openStream(Station, QString)), SLOT(on_openStream(Station, QString)));
//	connect(m_station_page, SIGNAL(sig_addToFavorites(QVariantMap)), SLOT(on_addStationToFavorites(QVariantMap)));
	connect(m_media, SIGNAL(sig_messages(QString)), m_station_page, SLOT(on_media_messages(QString)));

	m_app_window->addPage(m_stations_page);
	m_app_window->addPage(m_filter_page);
	m_app_window->addPage(m_log_page);
	m_app_window->show();

//	m_settings.server();
	// Сделать запрос на сервер
//	on_setServer("http://localhost:3000");

	m_filter_page->appear(m_app_window);
	on_requestPage(1);
}

App::~App()
{
	delete m_web;
	delete m_media;

	delete m_log_page;
	delete m_filter_page;
	delete m_stations_page;
	delete m_station_page;

	delete m_app_window;
}

void App::showBusy(bool busy)
{
	Q_UNUSED(busy);
	//TODO индикатор рабочих запросов
	//MProgressIndicator *spinner = new MProgressIndicator(NULL, MProgressIndicator::spinnerType);
	//spinner->setUnknownDuration(true);
}

void App::showMessage(QString msg, int timeout)
{
	Q_UNUSED(timeout);

	MBanner *infoBanner = new MBanner();
	infoBanner->setStyleName("InformationBanner");
	infoBanner->setTitle(msg);
	infoBanner->appear(m_app_window, MSceneWindow::DestroyWhenDone);
	// Не работает, делается через CSS для #InformationBanner
	//infoBanner->style()->setProperty("disappear-timeout", 100);
}

void App::on_web_finished(Task *task)
{
	qDebug() << Q_FUNC_INFO << task->type << task->ok << task->json_ok;

	if (task->ok)
	{
		switch(task->type)
		{
		case Task::Cookies:
			{
				QVariantMap playlist = task->json.toMap()["root"].toMap();
//				m_playlist_page->showPlaylist(playlist);
			}
			// Список стран в фильтр
			m_web->requestCountries();
			// Список жанров в фильтр
			m_web->requestGenres();
			break;
		case Task::Playlist:
//			m_playlist_page->showPlaylist(task->json.toMap());
			break;
		case Task::AddToPlaylist:
		case Task::PlaylistDestroy:
			qDebug() << "task->result" << task->result;
			if (task->result == "ok")
				showMessage("OK", 500);
			else
				showMessage("FAIL", 500);
			break;

		case Task::Station:
			// Данные одной станции
			{
				QVariantMap station = task->json.toMap()["station"].toMap();
//				m_station_view->setStation(station);
//				m_station_view->show();
			}
			break;
		case Task::Stations:
			// Получены данные о списке станций
			{
				QVariantMap result = task->json.toMap();
				m_stations_page->showStations(Station::List(result["stations"]), result);
			}

			// Пустой результат поиска
			if (m_stations_page->isEmpty())
			{
				showMessage("Not found", 1000);
			}
			else
			{
				// Переключится на список станций
				m_stations_page->appear(m_app_window);
			}

			break;
		case Task::Countries:
			// Список стран
			m_filter_page->showCountries(Location::List(task->json));
			break;
		case Task::Cities:
			// Список городов
			m_filter_page->showCities(Location::List(task->json));
			break;
		case Task::Genres:
			// Список жанров
			m_filter_page->showGenres(Genre::List(task->json));
			break;

		default:
			qDebug() << "Unknown task type:" << task->type << task->result;
			Q_ASSERT( 0 );
		}
	}
	else
	{
		switch(task->type)
		{
		case Task::Cookies:
			// Ошибка подключения к серверу
			qDebug() << "Bad server:" << m_web->server();
			break;
		default:
			qDebug() << "Error code:" << task->error_code << "for task type:" << task->type;
		}
	}

	delete task;
}

void App::on_showStationPage(Station station)
{
	m_station_page->setStation(station);
	m_station_page->appear(m_app_window);
}

void App::on_openStream(Station station, QString stream)
{
//	m_player_page->showStationInfo(station);
	m_media->open(station, stream);
}

void App::on_media_status(QVariantMap station, QString url, bool ok)
{
	qDebug() << Q_FUNC_INFO << ok << url;
	if (ok)
	{
		// Удалось подключится, добавить в историю
		m_web->addStationToPlaylist(station);

		QString msg = QString("Playing: %1").arg(station["name"].toString());
		showMessage(msg);
	}
}

void App::on_requestPage(int page)
{
	// Отправить запроса на текущую страницу
	m_web->requestStations(page, m_filter_page->filter());
}

void App::on_setServer(QString server)
{
	m_web->setServer(server);

	// Очистить список избранных
//	m_playlist_page->reset();

	// Первый запрос установит cookies с сервера
	m_web->requestCookies();
}
