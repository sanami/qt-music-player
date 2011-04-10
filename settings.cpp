#include "settings.h"

Settings::Settings()
	: m_settings()
{
}

QString Settings::server()
{
	return m_settings.value("server", "").toString();
}

void Settings::setServer(QString server)
{
	m_settings.setValue("server", server);
}
