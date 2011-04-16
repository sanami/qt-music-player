#include <QDebug>
#include <QFile>
#include "mplayer_control.h"

MPlayerControl::MPlayerControl(QObject *parent)
    : QObject(parent)
    , m_playState(PlayUnknown)
    , m_soundState(On)
    , m_soundLevel(50)
    , m_fullScreen(false)
    , m_timer(this)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(on_updateStatus()));
    m_timer.start(1000);
}

MPlayerControl::~MPlayerControl()
{
    m_timer.stop();
	close();
}

void MPlayerControl::close()
{
	if (!m_proc.isNull())
	{
		cmd("quit");
		sleep(1);
		m_proc->waitForBytesWritten();
		m_proc->close();

		m_proc->deleteLater();;
		m_proc = NULL;
	}
}

void MPlayerControl::open(QString url)
{
    qDebug() << Q_FUNC_INFO << url;

	if (m_proc.isNull())
    {
        startProcess(url);
        m_playState = Play;
    }
    else
    {
        cmd("loadfile " + url);
        setPlayState(Play);
    }

//    cmd("get_time_length");
}

void MPlayerControl::play(bool on)
{
    setPlayState(on ? Play : Pause);
}

void MPlayerControl::mute(bool on)
{
    setSoundState(on ? Mute : On);
}

void MPlayerControl::setPlayState(MPlayerControl::PlayState state)
{
    qDebug() << Q_FUNC_INFO << state;
    switch (state)
    {
    case Pause:
        cmd("pause");
        m_timer.stop();
        break;
    case Play:
        if (!isPlaying())
        {
            cmd("pause");
			m_timer.start();
		}
        break;
    default:
        Q_ASSERT( 0 );
    }

    m_playState = state;
}

void MPlayerControl::setSoundState(MPlayerControl::SoundState state)
{
    m_soundState = state;

    switch (m_soundState)
    {
    case On:
        cmd("mute 1");
        break;
    case Mute:
        cmd("mute 0");
        break;
    default:
        Q_ASSERT( 0 );
    }

}

void MPlayerControl::on_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << Q_FUNC_INFO << exitCode << exitStatus;
    m_proc->deleteLater();
	m_proc = NULL;
}

void MPlayerControl::on_error(QProcess::ProcessError error)
{
    qDebug() << Q_FUNC_INFO << error;
}

void MPlayerControl::on_stateChanged(QProcess::ProcessState newState)
{
    qDebug() << Q_FUNC_INFO << newState;
}

void MPlayerControl::on_readyReadStandardError()
{
    qDebug() << Q_FUNC_INFO;

	QString err_str = QString::fromUtf8(m_proc->readAllStandardError());
	err_str = err_str.trimmed();
	qDebug() << err_str;
	emit sig_videoOutput(err_str);
}

void MPlayerControl::on_readyReadStandardOutput()
{
    while (m_proc->canReadLine())
    {
        QString line = m_proc->readLine().trimmed();
        //if (!line.startsWith("A:") && !line.startsWith("V:"))
        {
            QString name = line.section("=",0,0);
            QString value = line.section("=",1,1);
            if (name == "ANS_TIME_POSITION")
            {
                emit sig_videoPos(value.toFloat());
            }
            else if (name == "ANS_LENGTH")
            {
                emit sig_videoLength(value.toFloat());
            }
            else
            {
                //qDebug() << Q_FUNC_INFO;
                qDebug() << "\t" << line;
                emit sig_videoOutput(line);
            }
        }
    }
}

void MPlayerControl::cmd(QString str, bool /*show*/)
{
    if (m_proc)
    {
        //if (show)
            qDebug() << Q_FUNC_INFO << str;

        m_proc->write(QString(str + "\n").toAscii());
        m_proc->waitForBytesWritten();
    }
}

void MPlayerControl::on_updateStatus()
{
//	cmd("get_time_pos", false);
//	cmd("get_time_length", false);
}

void MPlayerControl::seek(int pos)
{
    cmd(QString("seek %1 2").arg(pos));
    if (m_playState == Pause)
        cmd("pause");

    m_proc->readAllStandardOutput();
    cmd("get_time_pos", false);
}

void MPlayerControl::setFullScreen(bool on)
{
    m_fullScreen = on;
    if (m_fullScreen)
        cmd("vo_fullscreen 1");
    else
        cmd("vo_fullscreen 0");
}

void MPlayerControl::startProcess(QString url)
{
    QStringList arguments;
	arguments << "-quiet";
	arguments << "-slave";
	arguments << "-noconsolecontrols";
	arguments << "-nojoystick";
	arguments << "-nolirc";
	arguments << "-nomouseinput";
	arguments << "-idle";
	arguments << "-osdlevel" << "0";
	arguments << "-idx";
//	arguments << "-cache" << "256" << "-cache-min" << "50"; # Медленно
	arguments << "-identify";
	arguments << "-novideo";
	arguments << "-ao" << "pulse";
	arguments << "-loop" << "1"; // Пробовать только один раз

	//arguments << "-wid" << m_winId;
	//arguments << "-dr"; //direct rendering
	//arguments << "-nodouble"; // no double buffering because direct rendering is enabled
	//arguments << "-framedrop"; //Skip displaying some frames to maintain A/V sync on slow systems.
	//arguments << "-loop" << "1"; // Пробовать только один раз
	//arguments << "-msglevel" << "all=4";
	//arguments << "-vo" << "gl";
	//arguments << "-vo" << "null"; // Нет видео

	arguments << url; // the file name you want to play (with path)

	qDebug() << Q_FUNC_INFO << arguments.join(" ");
    emit sig_videoOutput(arguments.join("\n"));

    m_proc = new QProcess(this);
    connect( m_proc, SIGNAL( finished(int, QProcess::ExitStatus) ), this, SLOT( on_finished(int, QProcess::ExitStatus) ) );
    connect( m_proc, SIGNAL( error(QProcess::ProcessError) ), this, SLOT( on_error(QProcess::ProcessError) ) );
    connect( m_proc, SIGNAL( stateChanged(QProcess::ProcessState) ), this, SLOT( on_stateChanged(QProcess::ProcessState) ) );
    connect( m_proc, SIGNAL( readyReadStandardError() ), this, SLOT( on_readyReadStandardError() ) );
    connect( m_proc, SIGNAL( readyReadStandardOutput() ), this, SLOT( on_readyReadStandardOutput() ) );

	// Запуск в shell
//	QStringList args;
//	args << "-c";
//	args << "/usr/bin/mplayer" + " " + arguments.join(" ");
//	m_proc->start("/bin/sh", args);

	m_proc->start("mplayer", arguments);
	//m_proc->start("C:/Program Files/MPlayer for Windows/MPlayer.exe", arguments);
	//QProcess::startDetached("mplayer", arguments);
}
