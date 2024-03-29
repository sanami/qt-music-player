#include "stations_page.h"
#include "stations_model.h"

class StationsItemCreator : public MAbstractCellCreator<MContentItem>
{
public:
	virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
	{
		MContentItem *contentItem = new MContentItem(MContentItem::TwoTextLabels);
		updateCell(index, contentItem);
		return contentItem;
	}

	virtual void updateCell(const QModelIndex& index, MWidget *cell) const
	{
		MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
		QVariant data = index.data(Qt::DisplayRole);
		QStringList rowData = data.value<QStringList>();
		contentItem->setTitle(rowData[0]);
		contentItem->setSubtitle(rowData[1]);
	}
};

struct StationsPageUi
{
	MButton *prev;
	MButton *next;
	MLabel *page;
	MList *stations;

	~StationsPageUi()
	{
		delete prev;
		delete next;
		delete page;
		delete stations;
	}
};

StationsPage::StationsPage()
	: ui(new StationsPageUi)
	, m_model(NULL)
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
		ui->prev = btn;
	}
	{
		MButton *btn = new MButton(">>");
		connect(btn, SIGNAL(clicked()), SLOT(on_nextPage_clicked()));
		layout2->addItem(btn);
		ui->next = btn;
	}
	{
		MLabel *lab = new MLabel("[page]");
		layout2->addItem(lab);
		ui->page = lab;
	}
	layout->addItem(layout2);

	//layout->addItem(new MLabel("Stations:"));
	ui->stations = new MList();
	StationsItemCreator *cellCreator = new StationsItemCreator();
	ui->stations->setCellCreator(cellCreator);
	m_model = new StationsModel;
	ui->stations->setItemModel(m_model);
	connect(ui->stations, SIGNAL(itemClicked(QModelIndex)), SLOT(on_list_itemClicked(QModelIndex)));
	layout->addItem(ui->stations);

	layout->addStretch(100);
}

StationsPage::~StationsPage()
{
	delete ui;
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
	ui->stations->update();
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
	m_model->reset("Loading...");
	updateControls(false);

	// Отправить запрос на текущую страницу
	emit sig_requestPage(m_current_page);
}

void StationsPage::updateControls(bool enable)
{
	// Блокировать если занят
	ui->stations->setEnabled(enable);

	// Управление навигацией
	ui->prev->setEnabled(enable && (m_current_page > 1));
	ui->next->setEnabled(enable && (m_current_page < m_num_pages));

	// Информация о страницах
	ui->page->setText(QString("%1 / %2").arg(m_current_page).arg(m_num_pages));
}

void StationsPage::on_orientationChanged(M::Orientation orientation)
{
	qDebug() << Q_FUNC_INFO << orientation;
	if (orientation == M::Landscape)
		ui->stations->setColumns(2);
	else
		ui->stations->setColumns(1);
}
