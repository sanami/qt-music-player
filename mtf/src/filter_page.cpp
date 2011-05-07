#include "filter_page.h"

struct FilterPageUi
{
	MComboBox *comboBox_country;
	QVector<QVariant> comboBox_country_itemData;
	MComboBox *comboBox_city;
	QVector<QVariant> comboBox_city_itemData;
	MComboBox *comboBox_genre;
	QVector<QVariant> comboBox_genre_itemData;

	MTextEdit *search;
};

FilterPage::FilterPage()
	: ui(new FilterPageUi)
{
	setObjectName("FilterPage");
	setTitle("Filter");
}

FilterPage::~FilterPage()
{
	delete ui;
}


void FilterPage::createContent()
{
	//QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	QGraphicsGridLayout *layout = new QGraphicsGridLayout;
	layout->setSpacing(10);

	int row = 0;
	//layout->addItem(new MLabel("Country"), row, 0);
	{
		MComboBox *combobox = new MComboBox();
		combobox->setTitle("Country");
		connect(combobox, SIGNAL(currentIndexChanged(int)), SLOT(on_comboBox_country_currentIndexChanged(int)));
		layout->addItem(combobox, row, 0);
		ui->comboBox_country = combobox;
	}
	++row;
	//layout->addItem(new MLabel("City"), row, 0);
	{
		MComboBox *combobox = new MComboBox();
		combobox->setTitle("City");
		layout->addItem(combobox, row, 0);
		ui->comboBox_city = combobox;
	}
	++row;
	//layout->addItem(new MLabel("Genre"), row, 0);
	{
		MComboBox *combobox = new MComboBox();
		combobox->setTitle("Genre");
		layout->addItem(combobox, row, 0);
		ui->comboBox_genre = combobox;
	}
	++row;
	//layout->addItem(new MLabel("Search"), row, 0);
	{
		MTextEdit *edit = new MTextEdit();
		edit->setText("stac");
		layout->addItem(edit, row, 0);
		ui->search = edit;
	}
	++row;
	{
		MButton *btn = new MButton("Apply");
		connect(btn, SIGNAL(clicked()), SLOT(on_pushButton_filter_apply_clicked()));
		layout->addItem(btn, row, 0);
	}

	centralWidget()->setLayout(layout);
}

void FilterPage::showCountries(Location::List countries)
{
	ui->comboBox_country->clear();
	// Первый элемент, все страны
	ui->comboBox_country->addItem("All");

	// Данные элементов
	ui->comboBox_country_itemData.fill(0, countries.size()+1);
	int i = 1; // Первый 'All'

	foreach(Location country, countries)
	{
		// С привязкой к id
//		ui->comboBox_country->addItem(name, country["id"]);
		ui->comboBox_country->addItem(country.name());
		ui->comboBox_country_itemData[i++] = country.id();
	}
}

void FilterPage::showCities(Location::List cities)
{
	ui->comboBox_city->clear();
	// Первый элемент, все города
	ui->comboBox_city->addItem("All");
	ui->comboBox_city->setEnabled(true);

	// Данные элементов
	ui->comboBox_city_itemData.fill(0, cities.size()+1);
	int i = 1; // Первый 'All'

	foreach(Location city, cities)
	{
		// С привязкой к id
//		ui->comboBox_city->addItem(name, city["id"]);
		ui->comboBox_city->addItem(city.name());
		ui->comboBox_city_itemData[i++] = city.id();
	}
}

void FilterPage::showGenres(Genre::List genres)
{
	ui->comboBox_genre->clear();
	// Первый элемент, все жанры
	ui->comboBox_genre->addItem("All");

	// Данные элементов
	ui->comboBox_genre_itemData.fill(0, genres.size()+1);
	int i = 1; // Первый 'All'

	foreach(Genre g, genres)
	{
		// С привязкой к id
		ui->comboBox_genre->addItem(g.name());
		ui->comboBox_genre_itemData[i++] = g.id();
	}
}

void FilterPage::on_comboBox_country_currentIndexChanged(int index)
{
	ui->comboBox_city->clear();
	ui->comboBox_city->setEnabled(false);

	int country_id = ui->comboBox_country_itemData[index].toInt();
	if (country_id > 0)
	{
		// Список городов в фильтр
		ui->comboBox_city->addItem("Loading...");
		emit sig_requestCities(country_id);
	}
	else
	{
		ui->comboBox_city->addItem("All");
	}
}

void FilterPage::on_pushButton_filter_apply_clicked()
{
	m_filter.clear();

	// Строка поиска
	QStringList search = ui->search->text().split(QRegExp("\\s+"), QString::SkipEmptyParts);
	if (!search.isEmpty())
	{
		m_filter["name"] = search.join(" ");
	}

	// Локация
	int city_id = ui->comboBox_city->currentIndex() >= 0 ? ui->comboBox_city_itemData[ui->comboBox_city->currentIndex()].toInt() : 0;
	if (city_id > 0)
	{
		m_filter["location"] = city_id;
	}
	else
	{
		int country_id = ui->comboBox_country->currentIndex() >= 0 ? ui->comboBox_country_itemData[ui->comboBox_country->currentIndex()].toInt() : 0;

		if (country_id > 0)
			m_filter["location"] = country_id;
	}

	// Жанр
	int genre_id = ui->comboBox_genre->currentIndex() >= 0 ? ui->comboBox_genre_itemData[ui->comboBox_genre->currentIndex()].toInt() : 0;
	if (genre_id > 0)
	{
		m_filter["genre"] = genre_id;
	}

	// Новый поиск с 1 страницы
	emit sig_requestPage(1);
}
