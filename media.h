#ifndef MEDIA_H
#define MEDIA_H

#include <QObject>
#include <Phonon>

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(QObject *parent = 0);
	~Media();

	void open(QString url);

private slots:
	void on_stateChanged(Phonon::State newstate, Phonon::State oldstate);
	void on_bufferStatus(int percentFilled);

private:
	Phonon::AudioOutput *m_audioOutput;
	Phonon::MediaObject *m_mediaObject;
	Phonon::MediaSource *m_source;
};

#endif // MEDIA_H
