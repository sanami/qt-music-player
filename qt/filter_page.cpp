#include "filter_page.h"
#include "ui_filter_page.h"

FilterPage::FilterPage(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::FilterPage)
{
    ui->setupUi(this);

	ui->comboBox_search->addItem("stac");
}

FilterPage::~FilterPage()
{
    delete ui;
}

void FilterPage::showCountries(Location::List countries)
{
	ui->comboBox_country->clear();
	// Первый элемент, все страны
	ui->comboBox_country->addItem("All", 0);

	foreach(Location country, countries)
	{
		// С привязкой к id
		ui->comboBox_country->addItem(country.name(), country.id());
	}
}

void FilterPage::showCities(Location::List cities)
{
	ui->comboBox_city->clear();
	// Первый элемент, все города
	ui->comboBox_city->addItem("All", 0);
	ui->comboBox_city->setEnabled(true);

	foreach(Location city, cities)
	{
		// С привязкой к id
		ui->comboBox_city->addItem(city.name(), city.id());
	}
}

void FilterPage::showGenres(Genre::List genres)
{
	ui->comboBox_genre->clear();
	// Первый элемент, все жанры
	ui->comboBox_genre->addItem("All", 0);

	foreach(Genre g, genres)
	{
		// С привязкой к id
		ui->comboBox_genre->addItem(g.name(), g.id());
	}
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
		m_filter["name"] = search.join(" ");
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
