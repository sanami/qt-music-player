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

public:
	Web();
	~Web();

	//! Сервер для отправки запросов
	void setServer(QString server)
	{
		m_server = server;
	}

	//! Список стран
	void requestCountries();
	//! Список городов в стране
	void requestCities(int country_id);
	//! Список жанров
	void requestGenres();
	//! Список станций на странице
	void requestStations(int page, QVariantMap params = QVariantMap());

	//! Общие запросы
	Task *request(Task::Type type, QUrl url, QVariantMap params = QVariantMap());

private slots:
	//! Получен результат запроса
	void on_replyFinished(QNetworkReply *reply);
	void on_replyProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
	//! Завершенная задача
	void sig_finished(Task *task);

private:
	QNetworkAccessManager *m_network;
	QString m_server; //!< Адрес сервера
	QJson::Parser *m_json_parser;
	QMap<QNetworkReply *, Task *> m_reply;
};
