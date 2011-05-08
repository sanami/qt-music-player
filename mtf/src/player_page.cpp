#include "player_page.h"
#include "media.h"

struct PlayerPageUi
{
	MTextEdit *messages; //!< Лог плеера
	MButton *station_btn; //!< Кнопка перехода на страницу станции

	~PlayerPageUi()
	{
		delete messages;
		delete station_btn;
	}
};

PlayerPage::PlayerPage(Media *media)
	: ui(new PlayerPageUi)
	, m_media(media)
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

	ui->station_btn = new MButton("[station]");
	connect(ui->station_btn, SIGNAL(clicked()), SLOT(on_station_clicked()));
	layout->addItem(ui->station_btn);

	// Сообщение плеера
	ui->messages = new MTextEdit(MTextEditModel::MultiLine);
	setObjectName("messages");
	ui->messages->setReadOnly(true);
	ui->messages->setMaxLength(1000);
	layout->addItem(ui->messages);
}

PlayerPage::~PlayerPage()
{
	delete ui;
}

void PlayerPage::showStationInfo(Station station)
{
	m_station = station;

	QString info = station.name();
	ui->station_btn->setText(info);
}

void PlayerPage::on_media_messages(QString text)
{
	ui->messages->setText(text  + "\n" + ui->messages->text());
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
