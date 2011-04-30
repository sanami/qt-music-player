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
#include "playlist_page.h"
#include "playlist_manager.h"

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

	// Плеер
	m_media = new Media(this);
	connect(m_media, SIGNAL(sig_status(Station, QString, bool)), SLOT(on_media_status(Station, QString, bool)));

	// Менеджер избранных
	m_manager = new PlaylistManager(this);

	// Отладочные сообщения
	m_log_page = new LogPage(this);
	connect(Logger::logger(), SIGNAL(sig_debug(QString)), m_log_page, SLOT(addLog(QString)));
	m_log_page->hide();
	menuBar()->addAction(ui->actionLog);

	// Добавить страницы
	m_stations_page = new StationsPage(this);
	connect(m_stations_page, SIGNAL(sig_requestPage(int)), SLOT(on_requestPage(int)));
	connect(m_stations_page, SIGNAL(sig_showStation(Station)), SLOT(on_showStationPage(Station)));
	ui->tabWidget->addTab(m_stations_page, "Stations");

	m_filter_page = new FilterPage(this);
	connect(m_filter_page, SIGNAL(sig_setServer(QString)), SLOT(on_setServer(QString)));
	connect(m_filter_page, SIGNAL(sig_requestCities(int)), m_web, SLOT(requestCities(int)));
	connect(m_filter_page, SIGNAL(sig_requestPage(int)), SLOT(on_requestPage(int)));
//	connect(m_filter_page, SIGNAL(), SLOT());
	ui->tabWidget->addTab(m_filter_page, "Filter");

	m_station_view = new InfoPage(this);
	connect(m_station_view, SIGNAL(sig_openStream(Station, QString)), SLOT(on_openStream(Station, QString)));
	connect(m_station_view, SIGNAL(sig_addToFavorites(Station)), SLOT(on_addStationToFavorites(Station)));
	connect(m_media, SIGNAL(sig_messages(QString)), m_station_view, SLOT(on_media_messages(QString)));
	m_station_view->hide();

	m_player_page = new PlayerPage(m_media, this);
	connect(m_player_page, SIGNAL(sig_showStationPage(Station)), SLOT(on_showStationPage(Station)));
	ui->tabWidget->addTab(m_player_page, "Player");

	m_playlist_page = new PlaylistPage();
	connect(m_playlist_page, SIGNAL(sig_openPlaylist(int)), SLOT(on_openPlaylist(int)));
	connect(m_playlist_page, SIGNAL(sig_createPlaylist(int)), SLOT(on_createPlaylist(int)));
	connect(m_playlist_page, SIGNAL(sig_destroyPlaylist(int)), SLOT(on_destroyPlaylist(int)));
	connect(m_manager, SIGNAL(sig_clear()), m_playlist_page, SLOT(clearItems()));
	connect(m_manager, SIGNAL(sig_show(Playlist)), m_playlist_page, SLOT(showPlaylist(Playlist)));
	connect(m_manager, SIGNAL(sig_add(Playlist)), m_playlist_page, SLOT(addItem(Playlist)));
	connect(m_manager, SIGNAL(sig_remove(int)), m_playlist_page, SLOT(removeItem(int)));
	ui->tabWidget->addTab(m_playlist_page, "Playlist");

	// Показать FilterPage
	ui->tabWidget->setCurrentIndex(3);

	// Сделать запрос на сервер
	on_setServer(m_filter_page->server());

#ifndef Q_WS_MAEMO_5
	int x = 100, y = 100;
	move(x, y);
	m_station_view->move(x+width(), y);
#endif
}

Form::~Form()
{
    delete ui;

	delete m_web;
	delete m_media;
	delete m_manager;

	delete m_stations_page;
	delete m_filter_page;
	delete m_station_view;
	delete m_player_page;
	delete m_playlist_page;
	delete m_log_page;
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
	setWindowTitle("Heroku " + msg);
	QTimer::singleShot(timeout+500, this, SLOT(clearMessage()));
#endif
}

void Form::clearMessage()
{
	setWindowTitle("Heroku");
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
				QVariant root_playlist = task->json.toMap()["root"];
				m_manager->process(root_playlist, Playlist::AllList);
//				m_playlist_page->showPlaylist(Playlist(root_playlist));
			}
			// Список стран в фильтр
			m_web->requestCountries();
			// Список жанров в фильтр
			m_web->requestGenres();
			break;
//		case Task::Playlist:
//			m_manager->process(task->json);
//			m_playlist_page->showPlaylist(Playlist(task->json));
//			break;
		case Task::PlaylistCreate:
			m_manager->process(task->json, Playlist::Create);
			showMessage("OK", 500);
			break;
		case Task::PlaylistDestroy:
			m_manager->process(task->json, Playlist::Destroy);
			showMessage("OK", 500);
			break;
		case Task::Station:
			// Данные одной станции
			{
				m_station_view->setStation(Station(task->json));
				m_station_view->show();
			}
			break;
		case Task::Stations:
			// Получены данные о списке станций
			{
				QVariantMap result = task->json.toMap();
				m_stations_page->showStations(Station::List(result["stations"]), result); //TODO вынести в мета-информацию к списку
			}

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
		showMessage("FAIL", 500);
	}

	delete task;
}

void Form::on_showStationPage(Station station)
{
	m_station_view->setStation(station);
	m_station_view->show();
}

void Form::on_openStream(Station station, QString stream)
{
	m_player_page->showStationInfo(station);
	m_media->open(station, stream);
}

void Form::on_media_status(Station station, QString url, bool ok)
{
	qDebug() << Q_FUNC_INFO << ok << url;
	if (ok)
	{
		// Удалось подключится, добавить в историю
		m_web->addStationToPlaylist(station);

		QString msg = QString("Playing: %1").arg(station.name());
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
	m_manager->clear();

	// Первый запрос установит cookies с сервера
	m_web->requestCookies();
}

void Form::on_addStationToFavorites(Station station)
{
	if (!m_manager->favorites().isEmpty())
	{
		// Список избранных
		QStringList items;
		foreach(int favorite_id, m_manager->favorites())
		{
			Playlist favorite = m_manager->playlist(favorite_id);
			items << QString("%1 %2").arg(favorite.id()).arg(favorite.name());
		}

		bool ok;
		QString parent = QInputDialog::getItem(this, "Add to Favorites", "Choose:", items, 0, false, &ok);
		if (ok)
		{
			int parent_id = parent.section(' ', 0, 0).toInt();
			m_web->addStationToPlaylist(station, parent_id);
		}
	}
	else
	{
		showMessage("First create playlist");
	}
}

void Form::on_openPlaylist(int playlist_id)
{
	Playlist pl = m_manager->playlist(playlist_id);
	switch (pl.type())
	{
	case Playlist::Item:
		m_web->requestStation(pl.station_id());
		break;
	default:
		if (m_manager->contains(playlist_id))
			m_manager->show(playlist_id);
		else
			m_web->requestPlaylist(playlist_id);
	}
}

void Form::on_createPlaylist(int parent_id)
{
	QString name = QInputDialog::getText(this, "Create playlist", "Name:");
	if (!name.isEmpty())
	{
		// Отправить запрос на создание
		m_web->createPlaylist(name, parent_id);
	}
}

void Form::on_destroyPlaylist(int playlist_id)
{
	Playlist pl = m_manager->playlist(playlist_id);
	switch (pl.type())
	{
	case Playlist::Item:
		// Удалить станцию
		m_web->destroyPlaylist(playlist_id);
		break;
	default:
		// Каталог удалять с запросом
		if (QMessageBox::question(this, "Delete playlist", "Are you sure?", QMessageBox::Ok|QMessageBox::Cancel) == QMessageBox::Ok)
		{
			m_web->destroyPlaylist(playlist_id);
		}
	}
}
