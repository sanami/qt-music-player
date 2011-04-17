#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegExp>
//#include "../qjson/src/parser.h"
#include "qjson/parser.h"
#include "web.h"
#include "cookie_jar.h"
#include "playlist.h"

Web::Web()
	: QObject(NULL)
	, m_server("http://192.168.2.14:3000") // Для разработки
{
	m_json_parser = new QJson::Parser();

	m_network = new QNetworkAccessManager(this);
	connect(m_network, SIGNAL(finished(QNetworkReply *)), this, SLOT(on_replyFinished(QNetworkReply *)));

	m_network->setCookieJar(new CookieJar(this));
//	m_network->setCookieJar(new QNetworkCookieJar(this));

}

Web::~Web()
{
	delete m_json_parser;
	delete m_network;
}

void Web::requestCookies()
{
//	QString url = QString("%1/info/cookies").arg(m_server);
	QString url = QString("%1/me.json").arg(m_server);
	request(Task::Cookies, url);
}

void Web::requestPlaylist(int playlist_id)
{
	QString url = QString("%1/playlists/%2.json").arg(m_server).arg(playlist_id);
	request(Task::Playlist, url);
}

void Web::destroyPlaylist(int playlist_id)
{
	QString url = QString("%1/playlists/%2.json").arg(m_server).arg(playlist_id);
	request(Task::PlaylistDestroy, url, Task::Delete);
}

void Web::addStationToPlaylist(QVariantMap station, QVariantMap playlist)
{
	QString url = QString("%1/playlists.json").arg(m_server);

	QVariantMap params;
	{
		QVariantMap e;
		e["playlist_type_id"] = Playlist::Item;
		e["station_id"] = station["id"];
		e["parent_id"] = playlist["id"];
		e["name"] = station["name"];
		params["playlist"] = e;
	}

	request(Task::AddToPlaylist, url, Task::Post, params);
}

void Web::requestCountries()
{
	QString url = QString("%1/locations.json?location_type=1").arg(m_server);
	request(Task::Countries, url);
}

void Web::requestGenres()
{
	QString url = QString("%1/genres.json").arg(m_server);
	request(Task::Genres, url);
}

void Web::requestCities(int country_id)
{
	QString url = QString("%1/locations/%2/related_locations.json").arg(m_server).arg(country_id);
	request(Task::Cities, url);
}

void Web::requestStation(int station_id)
{
	QString url = QString("%1/stations/%2.json").arg(m_server).arg(station_id);
	request(Task::Station, url);
}

void Web::requestStations(int page, QVariantMap params)
{
	QString url = QString("%1/stations.json?page=%2").arg(m_server).arg(page);
	foreach(QString name, params.keys())
	{
		url += QString("&%1=%2").arg(name, params[name].toString());
	}

	request(Task::Stations, url);
}


Task *Web::request(Task::Type type, QUrl url, Task::Op op, QVariantMap params)
{
	qDebug() << Q_FUNC_INFO << url;
	Task *task = new Task();
	task->type = type;
	task->url = url;
	task->params = params;
	task->ok = false;

	QNetworkReply *reply;
	QNetworkRequest request(url);

//	request.setAttribute(QNetworkRequest::CookieLoadControlAttribute, QNetworkRequest::Manual);
//	CookieJar *jar = qobject_cast<CookieJar *>(m_network->cookieJar());
//	qDebug() << "***************************************************";
//	qDebug() << jar->all();
//	request.setHeader(QNetworkRequest::CookieHeader, QVariant::fromValue(jar->all()));

	switch (op)
	{
	case Task::Get:
		reply = m_network->get(request);
		break;
	case Task::Post:
		Q_ASSERT( !params.isEmpty() );
		reply = m_network->post(request, toParams(params));
		break;
	case Task::Delete:
		reply = m_network->deleteResource(request);
		break;
	default:
		Q_ASSERT( 0 );
	}

	//connect(reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(on_replyProgress(qint64, qint64)));
	m_reply[reply] = task;
	return task;
}

QByteArray Web::toParams(QVariantMap params) const
{
	QStringList args;
	foreach(QString key, params.keys())
	{
		QVariant value = params[key];
		switch (value.type())
		{
		case QVariant::Map:
			{
				QVariantMap sub_params = value.toMap();
				foreach(QString sub_key, sub_params.keys())
				{
					QString sub_value = sub_params[sub_key].toString();
					args << QString("%1[%2]=%3").arg(key, sub_key, sub_value);
				}
			}
			break;
		default:
			args << QString("%1=%2").arg(key, value.toString());
		}
	}

	return args.join("&").toUtf8();
}

void Web::on_replyFinished(QNetworkReply *reply)
{
	qDebug() << Q_FUNC_INFO << reply->error() << reply->bytesAvailable();

//	qDebug() << "replay" << reply->header(QNetworkRequest::CookieHeader);
//	qDebug() << m_network->cookieJar()->cookiesForUrl(QUrl(m_server));

//	CookieJar *jar = qobject_cast<CookieJar *>(m_network->cookieJar());
//	qDebug() << "jar" << jar->all();

	if (m_reply.contains(reply))
	{
		Task *task = m_reply.take(reply);

		// Нет ошибок
		if (reply->error() == QNetworkReply::NoError)
		{
			task->ok = true;
			task->result = reply->readAll();
			task->json = m_json_parser->parse(task->result, &task->json_ok);
		}
		else
		{
			// Ошибка загрузки данных
			task->ok = false;
			task->error_code = (int)reply->error();
		}
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
