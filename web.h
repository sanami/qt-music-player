#pragma once
#include <QObject>
#include <QMap>
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

	// Список станций на странице
	void requestStations(int page);

	// Общие запросы
	Task *request(QUrl url, QVariantMap params = QVariantMap());

private slots:
	//! Получен результат запроса
	void on_replyFinished(QNetworkReply *reply);
	void on_replyProgress(qint64 bytesReceived, qint64 bytesTotal);

signals:
	//! Завершенная задача
	void sig_finished(Task *task);

private:
	QNetworkAccessManager *m_network;
	QJson::Parser *m_json_parser;
	QMap<QNetworkReply *, Task *> m_reply;
};
