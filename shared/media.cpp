#include <QDebug>
#include <QUrl>
#include <QDesktopServices>
#include "media.h"
#include "mplayer_control.h"

Media::Media(QObject *parent)
	: QObject(parent)
{
	m_mplayer = new MPlayerControl(this);
	connect(m_mplayer, SIGNAL(sig_videoOutput(QString)), SIGNAL(sig_messages(QString)));
	connect(m_mplayer, SIGNAL(sig_urlStatus(QString, bool)), SLOT(on_mplayer_status(QString, bool)));
}

Media::~Media()
{
	delete m_mplayer;
}

void Media::open(QVariantMap station, QString url)
{
	qDebug() << Q_FUNC_INFO << url;
	// Открыть в системе
	//QDesktopServices::openUrl(QUrl(url));

	m_stations[url] = station;
	// Открыть в mplayer
	m_mplayer->open(url);
}

void Media::switch_play_state()
{
	qDebug() << Q_FUNC_INFO;

	bool play_state = m_mplayer->isPlaying();
	m_mplayer->play(!play_state);
}

void Media::close_player()
{
	m_mplayer->close();
}

void Media::on_mplayer_status(QString url, bool ok)
{
	emit sig_status(m_stations[url], url, ok);
}
