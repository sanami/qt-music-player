#include <QDebug>
#include <QFile>
#include "form.h"
#include "web.h"
#include "ui_form.h"
#include "info_page.h"

#define StationRole (Qt::UserRole + 1)

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

	m_station_view = new InfoPage(this);
	m_station_view->hide();

	ui->stations->addAction(ui->actionOpenStation);

	m_web = new Web();
	connect(m_web, SIGNAL(sig_finished(Task *)), this, SLOT(on_web_finished(Task *)));

	QString server = ui->comboBox_server->currentText();
	on_comboBox_server_currentIndexChanged(server);
//	ui->comboBox_server->setCurrentIndex(-1);

	// Запрос первой страницы
	m_current_page = 1;
	requestPage();
}

Form::~Form()
{
    delete ui;
	delete m_station_view;
	delete m_web;
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

void Form::log(QString text)
{
	qDebug() << Q_FUNC_INFO << text;
	ui->plainTextEdit->appendPlainText(text);
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

void Form::on_web_finished(Task *task)
{
	qDebug() << Q_FUNC_INFO;

	if (task->ok)
	{
		switch(task->type)
		{
		case Task::Stations:
			// Получены данные о списке станций
			showStations(task->result.toMap());
			break;
		case Task::Countries:
			// Список стран
			showCountries(task->result.toList());
			break;
		case Task::Cities:
			// Список городов
			showCities(task->result.toList());
			break;
		case Task::Genres:
			// Список жанров
			showGenres(task->result.toList());
			break;

		default:
			qDebug() << "Unknown task type:" << task->type;
			Q_ASSERT( 0 );
		}
	}
	else
	{
		qDebug() << "Error code:" << task->error_code << "for task type:" << task->type;
	}

	delete task;
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

QUrl Form::firstUrl(QVariantMap station) const
{
	QUrl url;
	QVariantList streams = station["streams"].toList();
	if (!streams.isEmpty())
	{
		url = streams[0].toMap()["stream"].toMap()["url"].toString();
	}
	return url;
}

void Form::on_stations_currentItemChanged(QListWidgetItem* it, QListWidgetItem* /*previous*/)
{
	if (it)
	{
		QVariantMap station = it->data(StationRole).toMap();
		//qDebug() << station;
		//qDebug() << station["streams"].toList()[0].toMap()["stream"].toMap()["url"];
		log(firstUrl(station).toString());
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
	m_station_view->setStation(station);
	m_station_view->show();
}

void Form::on_comboBox_server_currentIndexChanged(QString server)
{
	m_web->setServer(server);

	// Список стран в фильтр
	m_web->requestCountries();

	// Список жанров в фильтр
	m_web->requestGenres();
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
