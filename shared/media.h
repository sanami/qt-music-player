#pragma once
#include <QObject>
#include <QVariant>
#include "data.h"

class MPlayerControl;

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(QObject *parent = 0);
	~Media();

	//! Начать проигрывание
	void open(Station station, QString url);

	//! Переключить состояние проигрывания
	void switch_play_state();

	//! Закрыть плеер
	void close_player();

signals:
	//! Сообщения от плееров
	void sig_messages(QString str);

	//! Результат открытия аудиопотока
	void sig_status(Station station, QString url, bool ok);

private slots:
	//! Обработка сообщений от плеера
	void on_mplayer_status(QString url, bool ok);

private:
	MPlayerControl *m_mplayer; //!< Управление внешним mplayer
	QMap<QString, Station> m_stations; //!< Открытые станции
};
