#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>

class Settings
{
public:
    Settings();

	QString server();
	void setServer(QString server);

private:
	QSettings m_settings;
};

#endif // SETTINGS_H
