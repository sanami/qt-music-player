#include "stations_page.h"
#include "stations_model.h"

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
	: m_model(NULL)
	, m_current_page(0)
	, m_num_pages(0)
{
	setObjectName("StationsPage");
	setTitle("Stations");

	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	// Кнопки
	QGraphicsLinearLayout *layout2 = new QGraphicsLinearLayout(Qt::Horizontal);
	{
		MButton *btn = new MButton("<<");
		connect(btn, SIGNAL(clicked()), SLOT(on_prevPage_clicked()));
		layout2->addItem(btn);
	}
	{
		MButton *btn = new MButton(">>");
		connect(btn, SIGNAL(clicked()), SLOT(on_nextPage_clicked()));
		layout2->addItem(btn);
	}
	layout->addItem(layout2);

	//layout->addItem(new MLabel("Stations:"));
	m_list = new MList();
	MContentItemCreator *cellCreator = new MContentItemCreator();
	m_list->setCellCreator(cellCreator);
	m_model = new StationsModel;
	m_list->setItemModel(m_model);
	m_list->setColumns(2); //TODO
	connect(m_list, SIGNAL(itemClicked(QModelIndex)), SLOT(on_list_itemClicked(QModelIndex)));
	layout->addItem(m_list);
}

StationsPage::~StationsPage()
{
	delete m_model;
}

void StationsPage::createContent()
{
	qDebug() << Q_FUNC_INFO;
}

void StationsPage::showStations(Station::List stations, QVariantMap result)
{
	qDebug() << Q_FUNC_INFO << stations.size();
	m_current_page = result["current_page"].toInt();
	m_num_pages = result["num_pages"].toInt();

	updateControls(true);

	// Отобразить список
	m_model->setStations(stations);
	m_list->update();
}

bool StationsPage::isEmpty() const
{
	return m_num_pages == 0;
}

void StationsPage::on_prevPage_clicked()
{
	// <<
	if (m_current_page > 1)
	{
		m_current_page--;
		requestPage();
	}
}

void StationsPage::on_nextPage_clicked()
{
	// >>
	if (m_current_page < m_num_pages)
	{
		m_current_page++;
		requestPage();
	}
}

void StationsPage::on_list_itemClicked(const QModelIndex &index)
{
	const Station *station = m_model->station(index);
	if (station)
	{
		emit sig_showStation(*station);
	}
}

void StationsPage::requestPage()
{
	// В списке сообщение о работе
//	ui->stations->clear();
//	ui->stations->addItem("Loading...");

	m_model->reset("Loading...");
	updateControls(false);

	// Отправить запрос на текущую страницу
	emit sig_requestPage(m_current_page);
}

void StationsPage::updateControls(bool enable)
{
//	// Блокировать если занят
//	ui->stations->setEnabled(enable);

//	// Управление навигацией
//	ui->pushButton->setEnabled(enable && (m_current_page > 1));
//	ui->pushButton_2->setEnabled(enable && (m_current_page < m_num_pages));

//	// Информация о страницах
//	ui->page->setText(QString("%1 / %2").arg(m_current_page).arg(m_num_pages));

}

void StationsPage::on_orientationChanged(M::Orientation orientation)
{
	qDebug() << Q_FUNC_INFO << orientation;
	if (orientation == M::Landscape)
		m_list->setColumns(2);
	else
		m_list->setColumns(1);
}
