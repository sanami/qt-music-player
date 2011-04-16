#include <QDebug>
#include <QFile>
#include "form.h"
#include "web.h"
#include "ui_form.h"
#include "info_page.h"
#include "player_page.h"
#include "log_page.h"
#include "media.h"
#include "logger.h"
#include "playlist.h"

#define StationRole  (Qt::UserRole + 1)
#define PlaylistRole (Qt::UserRole + 2)

Form::Form(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Form)
	, m_current_page(0)
	, m_num_pages(0)
{
#ifdef Q_WS_MAEMO_5
//	setAttribute(Qt::WA_Maemo5AutoOrientation, true);
	setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
	setAttribute(Qt::WA_Maemo5StackedWindow); // Слайд между окнами
#endif
    ui->setupUi(this);

//	menuBar()->addAction(ui->actionPlayer);
	menuBar()->addAction(ui->actionLog);

	m_log_page = new LogPage(this);
	m_log_page->hide();
	connect(this, SIGNAL(sig_log(QString)), m_log_page, SLOT(addLog(QString)));
	connect(Logger::logger(), SIGNAL(sig_debug(QString)), m_log_page, SLOT(addLog(QString)));

	m_station_view = new InfoPage(this);
	m_station_view->hide();
	connect(m_station_view, SIGNAL(sig_openStream(QVariantMap, QString)), SLOT(on_openStream(QVariantMap, QString)));

	m_media = new Media(this);
	connect(m_media, SIGNAL(sig_messages(QString)), m_station_view, SLOT(on_media_messages(QString)));

	m_player_page = new PlayerPage(m_media, this);
	connect(m_player_page, SIGNAL(sig_showStationPage(QVariantMap)), SLOT(on_showStationPage(QVariantMap)));
	ui->tabWidget->addTab(m_player_page, "Player");
//	m_player_page->hide();

	ui->stations->addAction(ui->actionOpenStation);

	m_web = new Web();
	connect(m_web, SIGNAL(sig_finished(Task *)), this, SLOT(on_web_finished(Task *)));

	int server_index = ui->comboBox_server->findText(m_settings.server());
	if (server_index >= 0)
	{
		ui->comboBox_server->setCurrentIndex(server_index);

		// Запрос первой страницы
//		m_current_page = 1;
//		requestPage();
	}

//	QString server = ui->comboBox_server->currentText();
//	on_comboBox_server_currentIndexChanged(server);
}

Form::~Form()
{
    delete ui;
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

	// Блокировать если занят
	ui->stations->setDisabled(busy);

	// Управление навигацией
	ui->pushButton->setEnabled(!busy && (m_current_page > 1));
	ui->pushButton_2->setEnabled(!busy && (m_current_page < m_num_pages));

	// Информация о страницах
	ui->page->setText(QString("%1 / %2").arg(m_current_page).arg(m_num_pages));
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
			qDebug() << "task->result" << task->result;
			break;

		case Task::Station:
			// Данные одной станции
			{
				QVariantMap station = task->json.toMap()["station"].toMap();
				on_showStationPage(station);
			}
			break;
		case Task::Stations:
			// Получены данные о списке станций
			showStations(task->json.toMap());
			break;
		case Task::Countries:
			// Список стран
			showCountries(task->json.toList());
			break;
		case Task::Cities:
			// Список городов
			showCities(task->json.toList());
			break;
		case Task::Genres:
			// Список жанров
			showGenres(task->json.toList());
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

void Form::showStations(QVariantMap result)
{
	m_current_page = result["current_page"].toInt();
	m_num_pages = result["num_pages"].toInt();

	toggleBusy(Qt::Unchecked);

	// Отобразить список
	ui->stations->clear();
	QVariantList stations = result["stations"].toList();
	foreach(QVariant station_var, stations)
	{
		QVariantMap station = station_var.toMap().value("station").toMap();
		//qDebug() << station;
		QString name = station["name"].toString();

		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(StationRole, station);
		ui->stations->addItem(it);
	}
	// Пустой список
	if (stations.isEmpty())
	{
		ui->stations->addItem("Not found");
	}
}

void Form::showCountries(QVariantList countries)
{
	ui->comboBox_country->clear();
	// Первый элемент, все страны
	ui->comboBox_country->addItem("All", 0);

	foreach(QVariant country_var, countries)
	{
		QVariantMap country = country_var.toMap().value("location").toMap();
		QString name = country["name"].toString();

		// С привязкой к id
		ui->comboBox_country->addItem(name, country["id"]);
	}
}

void Form::showCities(QVariantList cities)
{
	ui->comboBox_city->clear();
	// Первый элемент, все города
	ui->comboBox_city->addItem("All", 0);
	ui->comboBox_city->setEnabled(true);

	foreach(QVariant city_var, cities)
	{
		QVariantMap city = city_var.toMap().value("location").toMap();
		QString name = city["name"].toString();

		// С привязкой к id
		ui->comboBox_city->addItem(name, city["id"]);
	}
}

void Form::showGenres(QVariantList genres)
{
	ui->comboBox_genre->clear();
	// Первый элемент, все жанры
	ui->comboBox_genre->addItem("All", 0);

	foreach(QVariant genre_var, genres)
	{
		QVariantMap genre = genre_var.toMap().value("genre").toMap();
		QString name = genre["name"].toString();

		// С привязкой к id
		ui->comboBox_genre->addItem(name, genre["id"]);
	}
}

void Form::on_pushButton_clicked()
{
	// <<
	if (m_current_page > 1)
	{
		m_current_page--;
		requestPage();
	}
}

void Form::on_pushButton_2_clicked()
{
	// >>
	if (m_current_page < m_num_pages)
	{
		m_current_page++;
		requestPage();
	}
}

void Form::on_actionOpenStation_triggered()
{
	qDebug() << Q_FUNC_INFO;

	QListWidgetItem* it = ui->stations->currentItem();
	if (it)
	{
//		QVariantMap station = it->data(StationRole).toMap();
//		QDesktopServices::openUrl(firstUrl(station));
	}
}

void Form::on_stations_itemDoubleClicked(QListWidgetItem* it)
{
	QVariantMap station = it->data(StationRole).toMap();

	// Показать детали станции
	on_showStationPage(station);
}

void Form::on_showStationPage(QVariantMap station)
{
	m_station_view->setStation(station);
	m_station_view->show();
}

void Form::on_comboBox_server_currentIndexChanged(QString server)
{
	m_settings.setServer(server);

	m_web->setServer(server);

	// Первый запрос установит cookies с сервера
	m_web->requestCookies();
}

void Form::on_comboBox_country_currentIndexChanged(int index)
{
	ui->comboBox_city->clear();
	ui->comboBox_city->setEnabled(false);

	int country_id = ui->comboBox_country->itemData(index).toInt();
	if (country_id > 0)
	{
		// Список городов в фильтр
		m_web->requestCities(country_id);
		ui->comboBox_city->addItem("Loading...", 0);
	}
	else
	{
		ui->comboBox_city->addItem("All", 0);
	}
}

void Form::on_pushButton_filter_apply_clicked()
{
	m_filter.clear();

	// Строка поиска
	QStringList search = ui->comboBox_search->currentText().split(QRegExp("\\s+"), QString::SkipEmptyParts);
	if (!search.isEmpty())
	{
		m_filter["name"] = search.join("+");
	}

	// Локация
	int city_id = ui->comboBox_city->itemData(ui->comboBox_city->currentIndex()).toInt();
	if (city_id > 0)
	{
		m_filter["location"] = city_id;
	}
	else
	{
		int country_id = ui->comboBox_country->itemData(ui->comboBox_country->currentIndex()).toInt();

		if (country_id > 0)
			m_filter["location"] = country_id;
	}

	// Жанр
	int genre_id = ui->comboBox_genre->itemData(ui->comboBox_genre->currentIndex()).toInt();
	if (genre_id > 0)
	{
		m_filter["genre"] = genre_id;
	}

	m_current_page = 1;
	requestPage();

	// Переключится на список станций
	ui->tabWidget->setCurrentIndex(0);
}

void Form::on_openStream(QVariantMap station, QString stream)
{
	log(stream);
	m_player_page->showStationInfo(station);
	m_media->open(stream);

	m_web->addStationToPlaylist(station);
}

//void Form::on_actionPlayer_triggered()
//{
//	m_player_page->show();
//}

void Form::on_actionLog_triggered()
{
	m_log_page->show();
}

void Form::log(QString text)
{
	emit sig_log(text);
}

void Form::requestPage()
{
	// Отправить запроса на текущую страницу
	m_web->requestStations(m_current_page, m_filter);

	// В списке сообщение о работе
	ui->stations->clear();
	ui->stations->addItem("Loading...");
	toggleBusy(Qt::Checked);

	log(QString("showPage %1").arg(m_current_page));
}
