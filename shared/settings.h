#pragma once
#include <QSettings>
#include "web_proxy.h"

//! Сохраняется в $HOME/.config/player_qt/settings.conf
class Settings
{
public:
    Settings();

	QString server() const
	{
		return m_settings.value("server", "http://music.heroku.com").toString();
	}
	void setServer(QString server)
	{
		m_settings.setValue("server", server);
	}

	WebProxy::Type proxyType() const
	{
		return (WebProxy::Type)m_settings.value("proxy_type", WebProxy::AutoProxy).toInt();
	}

	QUrl proxyUrl() const
	{
		return m_settings.value("proxy", QUrl("http://host:8080")).toUrl();
	}

	WebProxy proxy() const
	{
		WebProxy p;
		p.type = (WebProxy::Type)m_settings.value("proxy_type", WebProxy::AutoProxy).toInt();
		p.url = m_settings.value("proxy", QUrl("http://host:8080")).toUrl();
		return p;
	}

	void setProxyType(WebProxy::Type type)
	{
		m_settings.setValue("proxy_type", type);
	}

	void setProxyUrl(QUrl url)
	{
		m_settings.setValue("proxy", url);
	}

private:
	QSettings m_settings;
};
