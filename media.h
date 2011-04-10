#pragma once
#include <QObject>

class MPlayerControl;

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(QObject *parent = 0);
	~Media();

	//! Начать проигрывание
	void open(QString url);

	//! Переключить состояние проигрывания
	void switch_play_state();

	//! Закрыть плеер
	void close_player();

signals:
	//! Сообщения от плееров
	void sig_messages(QString str);

private:
	MPlayerControl *m_mplayer; //!< Управление внешним mplayer
};
