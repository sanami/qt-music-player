#include <QDebug>
#include <QUrl>
#include <QDesktopServices>
#include "media.h"

Media::Media(QObject *parent)
	: QObject(parent)
	, m_source(NULL)
{
	m_mediaObject = new Phonon::MediaObject(this);
	connect(m_mediaObject, SIGNAL(stateChanged(Phonon::State, Phonon::State)), this, SLOT(on_stateChanged(Phonon::State, Phonon::State)));
	connect(m_mediaObject, SIGNAL(bufferStatus(int)), this, SLOT(on_bufferStatus(int)));

	m_audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	Phonon::createPath(m_mediaObject, m_audioOutput);

	m_mediaObject->play();
}

Media::~Media()
{
	delete m_source;
	delete m_mediaObject;
	delete m_audioOutput;
}

void Media::open(QString url)
{
	qDebug() << Q_FUNC_INFO << url;
	QDesktopServices::openUrl(QUrl(url));

	return;
	url = "http://broadcast.infomaniak.net:80/funradiobe-high.mp3";

	m_mediaObject->setCurrentSource(QUrl(url));
//	delete m_source;
//	m_source = new Phonon::MediaSource(QUrl(url));

//	m_mediaObject->setCurrentSource(*m_source);
	m_mediaObject->play();
}

void Media::on_stateChanged(Phonon::State newstate, Phonon::State oldstate)
{
	qDebug() << Q_FUNC_INFO << newstate;
}


void Media::on_bufferStatus(int percentFilled)
{
	qDebug() << Q_FUNC_INFO << percentFilled;
}
