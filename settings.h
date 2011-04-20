#ifndef SETTINGS_H
#define SETTINGS_H
#include <QSettings>

//! Сохраняется в $HOME/.config/player_qt/settings.conf
class Settings
{
public:
    Settings();

	QString server() const;
	void setServer(QString server);

private:
	QSettings m_settings;
};

#endif // SETTINGS_H
