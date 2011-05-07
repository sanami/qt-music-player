#include "player_page.h"
#include "ui_player_page.h"
#include "media.h"

PlayerPage::PlayerPage(Media *media, QWidget *parent)
	: QWidget(parent, Qt::Window)
	, ui(new Ui::PlayerPage)
	, m_media(media)
{
    ui->setupUi(this);

#ifdef Q_WS_MAEMO_5
	setAttribute(Qt::WA_Maemo5StackedWindow);
#endif

	connect(m_media, SIGNAL(sig_messages(QString)), SLOT(on_media_messages(QString)));
	connect(m_media, SIGNAL(sig_volume(int)), SLOT(on_media_volume(int)));
}

PlayerPage::~PlayerPage()
{
    delete ui;
}

void PlayerPage::showStationInfo(Station station)
{
	m_station = station;

	QString info = station.name();
	ui->station->setText(info);
}

void PlayerPage::on_media_messages(QString str)
{
	ui->messages->appendPlainText(str);
	ui->messages->ensureCursorVisible();
}

void PlayerPage::on_pushButton_play_clicked()
{
	// Pause / play
	m_media->switch_play_state();
}

void PlayerPage::on_pushButton_close_clicked()
{
	m_media->close_player();
}

void PlayerPage::on_station_clicked()
{
	emit sig_showStationPage(m_station);
}

void PlayerPage::on_media_volume(int value)
{
	ui->volume->blockSignals(true);
	ui->volume->setValue(value);
	ui->volume->blockSignals(false);
}

void PlayerPage::on_volume_valueChanged(int value)
{
	m_media->set_volume(value);
}
