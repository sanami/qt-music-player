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

Web::Web(QObject *parent)
	: QObject(parent)
{
	m_json_parser = new QJson::Parser();

	m_network = new QNetworkAccessManager(this);
	connect(m_network, SIGNAL(finished(QNetworkReply *)), this, SLOT(on_replyFinished(QNetworkReply *)));

	m_network->setCookieJar(new CookieJar(this));
}

Web::~Web()
{
	delete m_json_parser;
	delete m_network;
//TODO	qDeleteAll(m_reply);
}

void Web::setProxy(WebProxy p)
{
	switch(p.type)
	{
		case WebProxy::ManualProxy:
		{
			QNetworkProxy proxy;
			proxy.setType(QNetworkProxy::HttpProxy);
			proxy.setHostName(p.url.host());
			proxy.setPort(p.url.port());
			proxy.setUser(p.url.userName());
			proxy.setPassword(p.url.password());
			m_network->setProxy(proxy);
		}
		break;
	case WebProxy::AutoProxy:
#ifdef Q_WS_WIN
		//TODO Только под Windows
		QNetworkProxyFactory::setUseSystemConfiguration(true);
		m_network->setProxy(QNetworkProxy(QNetworkProxy::DefaultProxy));
#else
		{
			QUrl url = WebProxy::autoProxy();
			if (url.isValid())
			{
				QNetworkProxy proxy;
				proxy.setType(QNetworkProxy::HttpProxy);
				proxy.setHostName(url.host());
				proxy.setPort(url.port());
				proxy.setUser(url.userName());
				proxy.setPassword(url.password());
				m_network->setProxy(proxy);
			}
		}
#endif
		break;
	case WebProxy::NoProxy:
		m_network->setProxy(QNetworkProxy(QNetworkProxy::NoProxy));
		break;
	}

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

void Web::renamePlaylist(int playlist_id, QString name)
{
	QString url = QString("%1/playlists/%2.json").arg(m_server).arg(playlist_id);

	QVariantMap params;
	{
		QVariantMap e;
		e["name"] = name;
		params["playlist"] = e;
	}

	request(Task::PlaylistRename, url, Task::Put, params);
}

void Web::createPlaylist(QString name, int parent_id)
{
	QString url = QString("%1/playlists.json").arg(m_server);

	QVariantMap params;
	{
		QVariantMap e;
		e["playlist_type_id"] = Playlist::Favorites;
		e["parent_id"] = parent_id;
		e["name"] = name;
		params["playlist"] = e;
	}

	request(Task::PlaylistCreate, url, Task::Post, params);
}

void Web::addStationToPlaylist(Station station, int parent_id)
{
	QString url = QString("%1/playlists.json").arg(m_server);

	QVariantMap params;
	{
		QVariantMap e;
		e["playlist_type_id"] = Playlist::Item;
		e["station_id"] = station.id();
		e["name"] = station.name();

		if (parent_id > 0)
			e["parent_id"] = parent_id;

		params["playlist"] = e;
	}

	request(Task::PlaylistCreate, url, Task::Post, params);
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

	QNetworkReply *reply = FALSE;
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
	case Task::Put:
		Q_ASSERT( !params.isEmpty() );
		//TODO Не работает
		//reply = m_network->put(request, toParams(params));
		params["_method"]="PUT";
		reply = m_network->post(request, toParams(params));
		break;
	case Task::Delete:
		reply = m_network->deleteResource(request);
		break;
	default:
		Q_ASSERT( 0 );
	}

	if (reply)
	{
		// Занят
		emit sig_busy(true);

		//connect(reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(on_replyProgress(qint64, qint64)));
		m_reply[reply] = task;
		return task;
	}

	// Ошибка
	delete task;
	return NULL;
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

	QByteArray result = args.join("&").toUtf8();
	qDebug() << Q_FUNC_INFO << result;
	return result;
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

	if (m_reply.isEmpty())
	{
		// Задач в запросе больше нет
		emit sig_busy(false);
	}

	// Do not directly delete it inside the slot
	reply->deleteLater();
}

void Web::on_replyProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	qDebug() << Q_FUNC_INFO << bytesReceived << bytesTotal;
}
