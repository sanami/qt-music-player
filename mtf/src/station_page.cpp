#include "station_page.h"

struct StationPageUi
{
	MLabel *label;
	MTextEdit *messages;
	MButtonGroup *streams;
	QGraphicsLinearLayout *streams_layout;

	~StationPageUi()
	{
		delete label;
		delete messages;
		delete streams;
		delete streams_layout;
	}
};

StationPage::StationPage()
	: ui(new StationPageUi)
{
	setObjectName("StationPage");
	setTitle("Station");

	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	// Общая информация
	ui->label = new MLabel("[info]");
	layout->addItem(ui->label);

	// Кнопки
	ui->streams = new MButtonGroup();
	connect(ui->streams, SIGNAL(buttonClicked(MButton *)), SLOT(on_streams_clicked(MButton *)));
	ui->streams_layout = new QGraphicsLinearLayout(Qt::Vertical);
	layout->addItem(ui->streams_layout);

	// Сообщение плеера
	ui->messages = new MTextEdit(MTextEditModel::MultiLine);
	setObjectName("messages");
	ui->messages->setReadOnly(true);
	ui->messages->setMaxLength(1000);
	layout->addItem(ui->messages);
}

StationPage::~StationPage()
{
	delete ui;
}

void StationPage::createContent()
{
	qDebug() << Q_FUNC_INFO;
}

void StationPage::setStation(Station station)
{
	// Сохранить данные
	m_station = station;

	// Удалить предыдущие кнопки
	qDeleteAll(ui->streams->buttons());

	//Ошибка? foreach(Stream stream, m_station.streams())

	// Данные в список
	Stream::List streams = m_station.streams();
	foreach(Stream stream, streams)
	{
		// Информация об аудиопотоке
		MButton *btn = new MButton(stream.bitrate() + " " + stream.url());
		btn->setProperty("url", stream.url());
		ui->streams->addButton(btn);
		ui->streams_layout->addItem(btn);
	}

	// Текстом
	QStringList info;
	info << QString("Id: %1").arg(m_station.id());
	info << QString("Name: %1").arg(m_station.name());
	ui->label->setText(info.join(". "));
}

void StationPage::on_streams_clicked(MButton *btn)
{
	QString url = btn->property("url").toString();
	emit sig_openStream(m_station, url);
}

void StationPage::on_media_messages(QString text)
{
	ui->messages->setText(text  + "\n" + ui->messages->text());
}

//void StationPage::on_addToFavorites_clicked()
//{
//	emit sig_addToFavorites(m_station);
//}
