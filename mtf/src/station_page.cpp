#include "station_page.h"

StationPage::StationPage()
{
	setObjectName("StationPage");
	setTitle("Station");

	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	// Общая информация
	m_label = new MLabel("[info]");
	layout->addItem(m_label);

	// Кнопки
	m_streams = new MButtonGroup();
	connect(m_streams, SIGNAL(buttonClicked(MButton *)), SLOT(on_streams_clicked(MButton *)));
	m_streams_layout = new QGraphicsLinearLayout(Qt::Vertical);
	layout->addItem(m_streams_layout);

	// Сообщение плеера
	m_messages = new MTextEdit(MTextEditModel::MultiLine);
	setObjectName("messages");
	m_messages->setReadOnly(true);
	m_messages->setMaxLength(1000);
	layout->addItem(m_messages);
}

StationPage::~StationPage()
{
	delete m_label;
	delete m_messages;
	delete m_streams;
	delete m_streams_layout;
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
	qDeleteAll(m_streams->buttons());

	//Ошибка? foreach(Stream stream, m_station.streams())

	// Данные в список
	Stream::List streams = m_station.streams();
	foreach(Stream stream, streams)
	{
		// Информация об аудиопотоке
		MButton *btn = new MButton(stream.url());
		m_streams->addButton(btn);
		m_streams_layout->addItem(btn);
	}

	// Текстом
	QStringList info;
	info << QString("Id: %1").arg(m_station.id());
	info << QString("Name: %1").arg(m_station.name());
	m_label->setText(info.join(". "));
}

void StationPage::on_streams_clicked(MButton *btn)
{
	QString url = btn->text(); // URL текстом
	emit sig_openStream(m_station, url);
}

void StationPage::on_media_messages(QString text)
{
	m_messages->setText(text  + "\n" + m_messages->text());
}

//void StationPage::on_addToFavorites_clicked()
//{
//	emit sig_addToFavorites(m_station);
//}
