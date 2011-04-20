#include <QDebug>
#include <QFile>
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

#define PlaylistRole (Qt::UserRole + 2)

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

	menuBar()->addAction(ui->actionLog);

	m_log_page = new LogPage(this);
	m_log_page->hide();
	connect(this, SIGNAL(sig_log(QString)), m_log_page, SLOT(addLog(QString)));
	connect(Logger::logger(), SIGNAL(sig_debug(QString)), m_log_page, SLOT(addLog(QString)));

	m_stations_page = new StationsPage(this);
	connect(m_stations_page, SIGNAL(sig_requestPage(int)), SLOT(on_requestPage(int)));
	connect(m_stations_page, SIGNAL(sig_showStation(QVariantMap)), SLOT(on_showStationPage(QVariantMap)));
	ui->tabWidget->addTab(m_stations_page, "Stations");

	m_filter_page = new FilterPage(this);
	connect(m_filter_page, SIGNAL(sig_setServer(QString)), SLOT(on_setServer(QString)));
	connect(m_filter_page, SIGNAL(sig_requestCities(int)), SLOT(on_requestCities(int)));
	connect(m_filter_page, SIGNAL(sig_requestPage(int)), SLOT(on_requestPage(int)));
//	connect(m_filter_page, SIGNAL(), SLOT());
	ui->tabWidget->addTab(m_filter_page, "Filter");

	m_station_view = new InfoPage(this);
	m_station_view->hide();
	connect(m_station_view, SIGNAL(sig_openStream(QVariantMap, QString)), SLOT(on_openStream(QVariantMap, QString)));

	m_media = new Media(this);
	connect(m_media, SIGNAL(sig_messages(QString)), m_station_view, SLOT(on_media_messages(QString)));
	connect(m_media, SIGNAL(sig_status(QVariantMap, QString, bool)), SLOT(on_media_status(QVariantMap, QString, bool)));

	m_player_page = new PlayerPage(m_media, this);
	connect(m_player_page, SIGNAL(sig_showStationPage(QVariantMap)), SLOT(on_showStationPage(QVariantMap)));
	ui->tabWidget->addTab(m_player_page, "Player");
//	m_player_page->hide();

	ui->playlist->addAction(ui->actionDeletePlaylist);

	m_web = new Web();
	connect(m_web, SIGNAL(sig_finished(Task *)), this, SLOT(on_web_finished(Task *)));
	on_setServer(m_filter_page->server());
}

Form::~Form()
{
    delete ui;
	delete m_stations_page;
	delete m_filter_page;
	delete m_station_view;
	delete m_player_page;
	delete m_web;
	delete m_media;
}

void Form::toggleBusy(int check)
{
	bool busy = (check == Qt::Checked);
#ifdef Q_WS_MAEMO_5
	// Индикатор работы
	setAttribute(Qt::WA_Maemo5ShowProgressIndicator, busy);
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
				showPlaylist(playlist);
			}
			// Список стран в фильтр
			m_web->requestCountries();
			// Список жанров в фильтр
			m_web->requestGenres();
			break;
		case Task::Playlist:
			showPlaylist(task->json.toMap());
			break;
		case Task::AddToPlaylist:
		case Task::PlaylistDestroy:
			qDebug() << "task->result" << task->result;
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

			//TODO Переключится на список станций
			//ui->tabWidget->setCurrentIndex(0);

			break;
		case Task::Countries:
			// Список стран
			m_filter_page->showCountries(task->json.toList());
			break;
		case Task::Cities:
			// Список городов
			m_filter_page->showCities(task->json.toList());
			break;
		case Task::Genres:
			// Список жанров
			m_filter_page->showGenres(task->json.toList());
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

void Form::showPlaylist(QVariantMap result)
{
	qDebug() << Q_FUNC_INFO << result;
	ui->playlist->clear();

	// Данные самого списка
	QVariantMap playlist = result["playlist"].toMap();

	if (!playlist["parent_id"].isNull())
	{
		// Переход на родителя
		int parent_id = playlist["parent_id"].toInt();
		QVariantMap parent;
		parent["id"] = parent_id;
		QListWidgetItem *it = new QListWidgetItem("[..]");
		it->setData(PlaylistRole, parent);
		ui->playlist->addItem(it);
	}

	// Данные о под-списках
	QVariantList children = result["children"].toList();
	foreach(QVariant item_var, children)
	{
		QVariantMap item = item_var.toMap();
		QVariantMap info = item["playlist"].toMap();

		QString name = info["id"].toString() + " ";
		switch (info["playlist_type_id"].toInt())
		{
		case Playlist::Item:
			name += info["name"].toString();
			break;
		default:
			name += "[" + info["name"].toString() + "]";
		}

		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(PlaylistRole, info);
		ui->playlist->addItem(it);
	}
}

void Form::on_playlist_itemDoubleClicked(QListWidgetItem* it)
{
	QVariantMap playlist = it->data(PlaylistRole).toMap();
	qDebug() << Q_FUNC_INFO << playlist;
	switch (playlist["playlist_type_id"].toInt())
	{
	case Playlist::Item:
		m_web->requestStation(playlist["station_id"].toInt());
		break;
	default:
		m_web->requestPlaylist(playlist["id"].toInt());
	}
}

void Form::on_showStationPage(QVariantMap station)
{
	m_station_view->setStation(station);
	m_station_view->show();
}

void Form::on_openStream(QVariantMap station, QString stream)
{
	log(stream);
	m_player_page->showStationInfo(station);
	m_media->open(station, stream);
}

void Form::on_media_status(QVariantMap station, QString url, bool ok)
{
	qDebug() << Q_FUNC_INFO << ok << url;
	if (ok)
	{
		//qDebug() << station;
		// Удалось подключится, добавить в историю
		m_web->addStationToPlaylist(station);
	}
}

void Form::on_actionLog_triggered()
{
	m_log_page->show();
}

void Form::log(QString text)
{
	emit sig_log(text);
}

void Form::on_actionDeletePlaylist_triggered()
{
	QListWidgetItem* it = ui->playlist->currentItem();
	if (it)
	{
		QVariantMap playlist = it->data(PlaylistRole).toMap();
		int playlist_id = playlist["id"].toInt();
		m_web->destroyPlaylist(playlist_id);
	}
}

void Form::on_requestPage(int page)
{
	// Отправить запроса на текущую страницу
	m_web->requestStations(page, m_filter_page->filter());
}

void Form::on_setServer(QString server)
{
	m_web->setServer(server);

	// Первый запрос установит cookies с сервера
	m_web->requestCookies();
}

void Form::on_requestCities(int country_id)
{
	// Список городов в фильтр
	m_web->requestCities(country_id);
}
