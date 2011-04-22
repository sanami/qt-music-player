#include "app.h"
#include "web.h"
#include "media.h"
#include "logger.h"
#include "main_page.h"
#include "stations_page.h"

App::App(QObject *parent) :
    QObject(parent)
{
	// Веб-запросы
	m_web = new Web();
	connect(m_web, SIGNAL(sig_finished(Task *)), SLOT(on_web_finished(Task *)));
	connect(m_web, SIGNAL(sig_busy(bool)), SLOT(showBusy(bool)));

	m_media = new Media(this);
//	connect(m_media, SIGNAL(sig_messages(QString)), m_station_view, SLOT(on_media_messages(QString)));
	connect(m_media, SIGNAL(sig_status(QVariantMap, QString, bool)), SLOT(on_media_status(QVariantMap, QString, bool)));

	m_app_window = new MApplicationWindow();
	m_app_window->setObjectName("AppWindow");
	m_app_window->show();

	// Добавить страницы
	m_main_page = new MainPage();
	connect(m_main_page, SIGNAL(sig_showPage(int)), SLOT(on_showPage(int)));
	m_main_page->appear();

//	m_settings.server();
	// Сделать запрос на сервер
	on_setServer("http://localhost:3000");
}

App::~App()
{
	delete m_web;
	delete m_media;

	delete m_main_page;
	delete m_app_window;
}

void App::showBusy(bool busy)
{
	Q_UNUSED(busy);
}

void App::showMessage(QString msg, int timeout)
{
	Q_UNUSED(msg);
	Q_UNUSED(timeout);
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
//			m_stations_page->showStations(task->json.toMap());

//			if (ui->tabWidget->currentWidget() != m_stations_page)
//			{
//				// Пустой результат поиска
//				if (m_stations_page->isEmpty())
//				{
//					showMessage("Not found", 1000);
//				}
//				else
//				{
//					// Переключится на список станций
//					ui->tabWidget->setCurrentWidget(m_stations_page);
//				}
//			}

			break;
		case Task::Countries:
			// Список стран
//			m_filter_page->showCountries(task->json.toList());
			break;
		case Task::Cities:
			// Список городов
//			m_filter_page->showCities(task->json.toList());
			break;
		case Task::Genres:
			// Список жанров
//			m_filter_page->showGenres(task->json.toList());
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

void App::on_setServer(QString server)
{
	m_web->setServer(server);

	// Очистить список избранных
//	m_playlist_page->reset();

	// Первый запрос установит cookies с сервера
	m_web->requestCookies();
}

void App::on_showPage(int page_id)
{
	qDebug() << Q_FUNC_INFO << page_id;
	StationsPage *page = new StationsPage();

	// When the back button is pressed, the page gets dismissed.
	// By setting MSceneWindow::DestroyWhenDismissed we don't have to
	// keep tabs on this page since it will be automatically destroyed
	// after the dismissal.
	page->appear();//, MSceneWindow::DestroyWhenDismissed);
}
