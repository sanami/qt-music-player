#include "player_page.h"
#include "media.h"

PlayerPage::PlayerPage(Media *media)
	: m_media(media)
{
	setObjectName("PlayerPage");
	setTitle("Player");

	connect(m_media, SIGNAL(sig_messages(QString)), SLOT(on_media_messages(QString)));

	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	// Кнопки
	QGraphicsLinearLayout *layout2 = new QGraphicsLinearLayout(Qt::Horizontal);
	{
		MButton *btn = new MButton("close");
		connect(btn, SIGNAL(clicked()), SLOT(on_close_clicked()));
		layout2->addItem(btn);
	}
	{
		MButton *btn = new MButton("play");
		connect(btn, SIGNAL(clicked()), SLOT(on_play_clicked()));
		layout2->addItem(btn);
	}
	layout->addItem(layout2);

	m_station_btn = new MButton("[station]");
	connect(m_station_btn, SIGNAL(clicked()), SLOT(on_station_clicked()));
	layout->addItem(m_station_btn);

	// Сообщение плеера
	m_messages = new MTextEdit(MTextEditModel::MultiLine);
	setObjectName("messages");
	m_messages->setReadOnly(true);
	m_messages->setMaxLength(1000);
	layout->addItem(m_messages);
}

PlayerPage::~PlayerPage()
{
	delete m_messages;
	delete m_station_btn;
}

void PlayerPage::showStationInfo(Station station)
{
	m_station = station;

	QString info = station.name();
	m_station_btn->setText(info);
}

void PlayerPage::on_media_messages(QString text)
{
	m_messages->setText(text  + "\n" + m_messages->text());
}

void PlayerPage::on_play_clicked()
{
	// Pause / play
	m_media->switch_play_state();
}

void PlayerPage::on_close_clicked()
{
	m_media->close_player();
}

void PlayerPage::on_station_clicked()
{
	emit sig_showStationPage(m_station);
}
