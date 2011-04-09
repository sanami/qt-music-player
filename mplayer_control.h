#pragma once
#include <QProcess>
#include <QPointer>
#include <QTimer>

class MPlayerControl : public QObject
{
    Q_OBJECT

private:
    enum PlayState {PlayUnknown, Pause, Play} m_playState;
    enum SoundState {SoundUnknown, Mute, On} m_soundState;
    int m_soundLevel;
    bool m_fullScreen;

    QString m_winId;
    QTimer m_timer;
	QPointer<QProcess> m_proc;

public:
    MPlayerControl(QObject *parent);
    ~MPlayerControl();
    void setWinId(QString winId) { m_winId = winId; }

    bool isPlaying() const { return m_playState == Play; }
    bool isMute() const { return m_soundState == Mute; }

    void open(QString url = QString());
    void play(bool on);
    void mute(bool on);
    void seek(int pos);
    void cmd(QString str, bool show = true);

    void close();

private:
    void setPlayState(PlayState state);
    void setSoundState(SoundState state);

    void startProcess(QString url);

    void setFullScreen(bool on);

private slots:
    void on_stateChanged(QProcess::ProcessState newState);
    void on_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void on_error(QProcess::ProcessError error);
    void on_readyReadStandardError();
    void on_readyReadStandardOutput();
    void on_updateStatus();

signals:
    void sig_videoPos(float pos);
    void sig_videoLength(float length);
    void sig_videoOutput(QString text);
};
