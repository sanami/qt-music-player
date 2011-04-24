#include "stations_page.h"
#include "ui_stations_page.h"

#define StationRole  (Qt::UserRole + 1)

StationsPage::StationsPage(QWidget *parent)
	:QWidget(parent)
	, ui(new Ui::StationsPage)
	, m_current_page(0)
	, m_num_pages(0)
{
    ui->setupUi(this);
}

StationsPage::~StationsPage()
{
    delete ui;
}

void StationsPage::showStations(Station::List stations, QVariantMap result)
{
	m_current_page = result["current_page"].toInt();
	m_num_pages = result["num_pages"].toInt();

	updateControls(true);

	// Отобразить список
	ui->stations->clear();
	foreach(Station station, stations)
	{
		QListWidgetItem *it = new QListWidgetItem(station.name());
		it->setData(StationRole, QVariant::fromValue(station));
		ui->stations->addItem(it);
	}
	// Пустой список
	if (stations.isEmpty())
	{
		ui->stations->addItem("Not found");
	}
}

bool StationsPage::isEmpty() const
{
	return m_num_pages == 0;
}

void StationsPage::on_pushButton_clicked()
{
	// <<
	if (m_current_page > 1)
	{
		m_current_page--;
		requestPage();
	}
}

void StationsPage::on_pushButton_2_clicked()
{
	// >>
	if (m_current_page < m_num_pages)
	{
		m_current_page++;
		requestPage();
	}
}

void StationsPage::on_stations_itemDoubleClicked(QListWidgetItem* it)
{
	Station station = it->data(StationRole).value<Station>();

	// Показать детали станции
	emit sig_showStation(station);
}

void StationsPage::requestPage()
{
	// В списке сообщение о работе
	ui->stations->clear();
	ui->stations->addItem("Loading...");
	updateControls(false);

	// Отправить запрос на текущую страницу
	emit sig_requestPage(m_current_page);
}

void StationsPage::updateControls(bool enable)
{
	// Блокировать если занят
	ui->stations->setEnabled(enable);

	// Управление навигацией
	ui->pushButton->setEnabled(enable && (m_current_page > 1));
	ui->pushButton_2->setEnabled(enable && (m_current_page < m_num_pages));

	// Информация о страницах
	ui->page->setText(QString("%1 / %2").arg(m_current_page).arg(m_num_pages));

}
