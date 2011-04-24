#include <QDebug>
#include <QFile>
#ifdef Q_WS_MAEMO_5
#include <QMaemo5InformationBox>
#endif
#include "form.h"
#include "web.h"
#include "ui_form.h"
#include "stations_page.h"
#include "filter_page.h"
#include "info_page.h"
#include "player_page.h"
#include "log_page.h"
#include "media.h"
#include "logger.h"
#include "playlist.h"
#include "playlist_page.h"

Form::Form(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Form)
{
#ifdef Q_WS_MAEMO_5
//	setAttribute(Qt::WA_Maemo5AutoOrientation, true);
	setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
	setAttribute(Qt::WA_Maemo5StackedWindow); // Слайд между окнами
#endif
    ui->setupUi(this);

	// Веб-запросы
	m_web = new Web();
	connect(m_web, SIGNAL(sig_finished(Task *)), SLOT(on_web_finished(Task *)));
	connect(m_web, SIGNAL(sig_busy(bool)), SLOT(showBusy(bool)));

	// Отладочные сообщения
	m_log_page = new LogPage(this);
	m_log_page->hide();
	connect(Logger::logger(), SIGNAL(sig_debug(QString)), m_log_page, SLOT(addLog(QString)));
	menuBar()->addAction(ui->actionLog);

	// Добавить страницы
	m_stations_page = new StationsPage(this);
	connect(m_stations_page, SIGNAL(sig_requestPage(int)), SLOT(on_requestPage(int)));
	connect(m_stations_page, SIGNAL(sig_showStation(QVariantMap)), SLOT(on_showStationPage(QVariantMap)));
	ui->tabWidget->addTab(m_stations_page, "Stations");

	m_filter_page = new FilterPage(this);
	connect(m_filter_page, SIGNAL(sig_setServer(QString)), SLOT(on_setServer(QString)));
	connect(m_filter_page, SIGNAL(sig_requestCities(int)), m_web, SLOT(requestCities(int)));
	connect(m_filter_page, SIGNAL(sig_requestPage(int)), SLOT(on_requestPage(int)));
//	connect(m_filter_page, SIGNAL(), SLOT());
	ui->tabWidget->addTab(m_filter_page, "Filter");

	m_station_view = new InfoPage(this);
	m_station_view->hide();
	connect(m_station_view, SIGNAL(sig_openStream(QVariantMap, QString)), SLOT(on_openStream(QVariantMap, QString)));
	connect(m_station_view, SIGNAL(sig_addToFavorites(QVariantMap)), SLOT(on_addStationToFavorites(QVariantMap)));

	m_media = new Media(this);
	connect(m_media, SIGNAL(sig_messages(QString)), m_station_view, SLOT(on_media_messages(QString)));
	connect(m_media, SIGNAL(sig_status(QVariantMap, QString, bool)), SLOT(on_media_status(QVariantMap, QString, bool)));

	m_player_page = new PlayerPage(m_media, this);
	connect(m_player_page, SIGNAL(sig_showStationPage(QVariantMap)), SLOT(on_showStationPage(QVariantMap)));
	ui->tabWidget->addTab(m_player_page, "Player");

	m_playlist_page = new PlaylistPage(this);
	connect(m_playlist_page, SIGNAL(sig_requestStation(int)), m_web, SLOT(requestStation(int)));
	connect(m_playlist_page, SIGNAL(sig_requestPlaylist(int)), m_web, SLOT(requestPlaylist(int)));
	connect(m_playlist_page, SIGNAL(sig_destroyPlaylist(int)), m_web, SLOT(destroyPlaylist(int)));
	connect(m_playlist_page, SIGNAL(sig_createPlaylist(QString, int)), m_web, SLOT(createPlaylist(QString, int)));
//	connect(m_playlist_page, SIGNAL(), m_web, SLOT());
	ui->tabWidget->addTab(m_playlist_page, "Playlist");

	ui->tabWidget->setCurrentIndex(1);

	// Сделать запрос на сервер
	on_setServer(m_filter_page->server());
}

Form::~Form()
{
    delete ui;
	delete m_stations_page;
	delete m_filter_page;
	delete m_station_view;
	delete m_player_page;
	delete m_playlist_page;
	delete m_log_page;
	delete m_web;
	delete m_media;
}

void Form::showBusy(bool busy)
{
#ifdef Q_WS_MAEMO_5
	// Индикатор работы
	setAttribute(Qt::WA_Maemo5ShowProgressIndicator, busy);
#else
	Q_UNUSED(busy);
#endif
}

void Form::showMessage(QString msg, int timeout)
{
#ifdef Q_WS_MAEMO_5
	QMaemo5InformationBox::information(this, msg, timeout);
#else
	Q_UNUSED(msg);
	Q_UNUSED(timeout);
#endif
}

void Form::on_web_finished(Task *task)
{
	qDebug() << Q_FUNC_INFO << task->type << task->ok << task->json_ok;

	if (task->ok)
	{
		switch(task->type)
		{
		case Task::Cookies:
			{
				QVariantMap playlist = task->json.toMap()["root"].toMap();
				m_playlist_page->showPlaylist(playlist);
			}
			// Список стран в фильтр
			m_web->requestCountries();
			// Список жанров в фильтр
			m_web->requestGenres();
			break;
		case Task::Playlist:
			m_playlist_page->showPlaylist(task->json.toMap());
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
				m_station_view->setStation(station);
				m_station_view->show();
			}
			break;
		case Task::Stations:
			// Получены данные о списке станций
			m_stations_page->showStations(task->json.toMap());

			if (ui->tabWidget->currentWidget() != m_stations_page)
			{
				// Пустой результат поиска
				if (m_stations_page->isEmpty())
				{
					showMessage("Not found", 1000);
				}
				else
				{
					// Переключится на список станций
					ui->tabWidget->setCurrentWidget(m_stations_page);
				}
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


void Form::on_showStationPage(QVariantMap station)
{
	m_station_view->setStation(station);
	m_station_view->show();
}

void Form::on_openStream(QVariantMap station, QString stream)
{
	m_player_page->showStationInfo(station);
	m_media->open(station, stream);
}

void Form::on_media_status(QVariantMap station, QString url, bool ok)
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

void Form::on_actionLog_triggered()
{
	m_log_page->show();
}

void Form::on_requestPage(int page)
{
	// Отправить запроса на текущую страницу
	m_web->requestStations(page, m_filter_page->filter());
}

void Form::on_setServer(QString server)
{
	m_web->setServer(server);

	// Очистить список избранных
	m_playlist_page->reset();

	// Первый запрос установит cookies с сервера
	m_web->requestCookies();
}

void Form::on_addStationToFavorites(QVariantMap station)
{
	// Список избранных
	QStringList items;
	foreach(QVariantMap favorite, m_playlist_page->all())
	{
		items << favorite["id"].toString() + " " + favorite["name"].toString();
	}

	bool ok;
	QString parent = QInputDialog::getItem(this, "Add to Favorites", "Choose:", items, 0, false, &ok);
	if (ok)
	{
		int parent_id = parent.section(' ', 0, 0).toInt();
		m_web->addStationToPlaylist(station, parent_id);
	}
}
