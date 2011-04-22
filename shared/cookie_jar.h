#pragma once
#include <QNetworkCookieJar>

class CookieJar : public QNetworkCookieJar
{
    Q_OBJECT
public:
    CookieJar(QObject *parent);
    ~CookieJar();

	QList<QNetworkCookie> all() const
	{
		return allCookies();
	}

    void clear();
	void save() const;
    void load();
};
