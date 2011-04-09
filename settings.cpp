#include "settings.h"

Settings::Settings()
	: m_settings("player_qt", "settings")
{
}

QString Settings::server()
{
	m_settings.value("server").toString();
}

void Settings::setServer(QString server)
{
	m_settings.setValue("server", server);
}
