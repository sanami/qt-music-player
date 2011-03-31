#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegExp>
//#include "../qjson/src/parser.h"
#include "qjson/parser.h"
#include "web.h"

Web::Web()
	: QObject(NULL)
{
	m_json_parser = new QJson::Parser();

	m_network = new QNetworkAccessManager(this);
	connect(m_network, SIGNAL(finished(QNetworkReply *)), this, SLOT(on_replyFinished(QNetworkReply *)));
}

Web::~Web()
{
	delete m_json_parser;
	delete m_network;
}

void Web::requestStations(int page)
{
	QString url = QString("http://music.heroku.com/stations.json?page=%1").arg(page);
	request(url);
}

Task *Web::request(QUrl url, QVariantMap params)
{
	qDebug() << Q_FUNC_INFO << url;
	Task *task = new Task();
	task->url = url;
	task->params = params;
	task->ok = false;

	QNetworkReply *reply = m_network->get(QNetworkRequest(url));
	//connect(reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(on_replyProgress(qint64, qint64)));
	m_reply[reply] = task;
	return task;
}

void Web::on_replyFinished(QNetworkReply *reply)
{
	qDebug() << Q_FUNC_INFO << reply->bytesAvailable();
	if (m_reply.contains(reply))
	{
		Task *task = m_reply.take(reply);

		QByteArray json = reply->readAll();

		bool ok;
		task->result = m_json_parser->parse(json, &ok);
		task->ok = ok;
		emit sig_finished(task);
	}
	else
	{
		Q_ASSERT( 0 );
		qDebug() << "!!uknown replay";
	}
	// Do not directly delete it inside the slot
	reply->deleteLater();
}

void Web::on_replyProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	qDebug() << Q_FUNC_INFO << bytesReceived << bytesTotal;
}
