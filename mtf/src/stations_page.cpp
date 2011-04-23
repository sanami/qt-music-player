#include "stations_page.h"

QStringList g_stations;

class StationsModel : public QAbstractListModel
{
public:
	StationsModel()
	{
	}

protected:
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const
	{
		return g_stations.size();
	}
	QVariant data(const QModelIndex &index, int role) const
	{
		if (role == Qt::DisplayRole)
		{
			QStringList rowData;
			rowData << g_stations[index.row()];
//			rowData << "Angelina"; // first name
//			rowData << "Joli"; // last name
			return QVariant(rowData);
		}
		return QVariant();
	}
};

class MContentItemCreator : public MAbstractCellCreator<MContentItem>
{
public:
	void updateCell(const QModelIndex& index, MWidget *cell) const
	{
		MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
		QVariant data = index.data(Qt::DisplayRole);
		QStringList rowData = data.value<QStringList>();
		contentItem->setTitle(rowData[0]);
//		contentItem->setSubtitle(rowData[1]);
	}
};

StationsPage::StationsPage()
	: m_current_page(0)
	, m_num_pages(0)
{
	setObjectName("StationsPage");
	setTitle("Stations");
}

void StationsPage::createContent()
{
	qDebug() << Q_FUNC_INFO;
	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	// Items in a vertical layout are arranged from top to bottom.
	layout->addItem(new MLabel("Stations:"));


	MList * list = new MList();
	MContentItemCreator *cellCreator = new MContentItemCreator;
	list->setCellCreator(cellCreator);
	StationsModel *model = new StationsModel;
	list->setItemModel(model);

	list->setColumns(2);


	layout->addItem(list);

}

void StationsPage::showStations(QVariantMap result)
{
	m_current_page = result["current_page"].toInt();
	m_num_pages = result["num_pages"].toInt();

//	updateControls(true);

	g_stations.clear();

//	// Отобразить список
//	ui->stations->clear();
	QVariantList stations = result["stations"].toList();
	foreach(QVariant station_var, stations)
	{
		QVariantMap station = station_var.toMap().value("station").toMap();
		//qDebug() << station;
		QString name = station["name"].toString();

		g_stations << name;

//		QListWidgetItem *it = new QListWidgetItem(name);
//		it->setData(StationRole, station);
//		ui->stations->addItem(it);
	}
//	// Пустой список
//	if (stations.isEmpty())
//	{
//		ui->stations->addItem("Not found");
//	}
}

bool StationsPage::isEmpty() const
{
	return m_num_pages == 0;
}

//void StationsPage::on_pushButton_clicked()
//{
//	// <<
//	if (m_current_page > 1)
//	{
//		m_current_page--;
//		requestPage();
//	}
//}

//void StationsPage::on_pushButton_2_clicked()
//{
//	// >>
//	if (m_current_page < m_num_pages)
//	{
//		m_current_page++;
//		requestPage();
//	}
//}

//void StationsPage::on_stations_itemDoubleClicked(QListWidgetItem* it)
//{
//	QVariantMap station = it->data(StationRole).toMap();

//	// Показать детали станции
//	emit sig_showStation(station);
//}

//void StationsPage::requestPage()
//{
//	// В списке сообщение о работе
//	ui->stations->clear();
//	ui->stations->addItem("Loading...");
//	updateControls(false);

//	// Отправить запрос на текущую страницу
//	emit sig_requestPage(m_current_page);
//}

//void StationsPage::updateControls(bool enable)
//{
//	// Блокировать если занят
//	ui->stations->setEnabled(enable);

//	// Управление навигацией
//	ui->pushButton->setEnabled(enable && (m_current_page > 1));
//	ui->pushButton_2->setEnabled(enable && (m_current_page < m_num_pages));

//	// Информация о страницах
//	ui->page->setText(QString("%1 / %2").arg(m_current_page).arg(m_num_pages));

//}
