#include "filter_page.h"
#include "ui_filter_page.h"

FilterPage::FilterPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FilterPage)
{
    ui->setupUi(this);

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

FilterPage::~FilterPage()
{
    delete ui;
}

void FilterPage::showCountries(QVariantList countries)
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

void FilterPage::showCities(QVariantList cities)
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

void FilterPage::showGenres(QVariantList genres)
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

void FilterPage::on_comboBox_server_currentIndexChanged(QString server)
{
	m_settings.setServer(server);
	emit sig_setServer(server);
}

void FilterPage::on_comboBox_country_currentIndexChanged(int index)
{
	ui->comboBox_city->clear();
	ui->comboBox_city->setEnabled(false);

	int country_id = ui->comboBox_country->itemData(index).toInt();
	if (country_id > 0)
	{
		// Список городов в фильтр
		ui->comboBox_city->addItem("Loading...", 0);
		emit sig_requestCities(country_id);
	}
	else
	{
		ui->comboBox_city->addItem("All", 0);
	}
}

void FilterPage::on_pushButton_filter_apply_clicked()
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

	// Новый поиск с 1 страницы
	emit sig_requestPage(1);
}