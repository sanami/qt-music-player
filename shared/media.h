#pragma once
#include <QObject>
#include <QVariant>

class MPlayerControl;

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(QObject *parent = 0);
	~Media();

	//! Начать проигрывание
	void open(QVariantMap station, QString url);

	//! Переключить состояние проигрывания
	void switch_play_state();

	//! Закрыть плеер
	void close_player();

signals:
	//! Сообщения от плееров
	void sig_messages(QString str);

	//! Результат открытия аудиопотока
	void sig_status(QVariantMap station, QString url, bool ok);

private slots:
	//! Обработка сообщений от плеера
	void on_mplayer_status(QString url, bool ok);

private:
	MPlayerControl *m_mplayer; //!< Управление внешним mplayer
	QMap<QString, QVariantMap> m_stations; //!< Открытые станции
};
