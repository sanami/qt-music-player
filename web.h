#pragma once
#include <QObject>
#include <QMap>
#include <QString>
#include "task.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace QJson
{
	class Parser;
}

//! Работа с веб запросами
class Web : public QObject
{
	Q_OBJECT
	friend class Test;

public:
	Web();
	~Web();

	//! Сервер для отправки запросов
	QString server() const { return m_server; }
	void setServer(QString server) { m_server = server; }

	//! Запрос на HTML страницу, это установит cookies
	void requestCookies();
	//! Добавить запись в плейлист пользователя
	void addStationToPlaylist(QVariantMap station, QVariantMap playlist = QVariantMap());
	//! Запрос информации о списке
	void requestPlaylist(int playlist_id);
	//! Удалить список
	void destroyPlaylist(int playlist_id);

	//! Список стран
	void requestCountries();
	//! Список городов в стране
	void requestCities(int country_id);
	//! Список жанров
	void requestGenres();
	//! Список станций на странице
	void requestStations(int page, QVariantMap params = QVariantMap());
	//! Запрос данных одной станции
	void requestStation(int station_id);

signals:
	//! Завершенная задача
	void sig_finished(Task *task);

private slots:
	//! Получен результат запроса
	void on_replyFinished(QNetworkReply *reply);
	void on_replyProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
	//! Общие запросы
	Task *request(Task::Type type, QUrl url, Task::Op op = Task::Get,  QVariantMap params = QVariantMap());

	//! Преобразовать QVariantMap в данные для POST
	QByteArray toParams(QVariantMap params) const;

private:
	QNetworkAccessManager *m_network;

	QString m_server; //!< Адрес сервера
	QJson::Parser *m_json_parser;
	QMap<QNetworkReply *, Task *> m_reply;
};
