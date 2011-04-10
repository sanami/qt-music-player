#include <QList>
#include <QDataStream>
#include "cookie_jar.h"

static const unsigned int JAR_VERSION = 11;

QT_BEGIN_NAMESPACE
QDataStream &operator<<(QDataStream &stream, const QList<QNetworkCookie> &list)
{
    stream << JAR_VERSION;
    stream << quint32(list.size());
    for (int i = 0; i < list.size(); ++i)
        stream << list.at(i).toRawForm();
    return stream;
}

QDataStream &operator>>(QDataStream &stream, QList<QNetworkCookie> &list)
{
    list.clear();

    quint32 version;
    stream >> version;

    if (version != JAR_VERSION)
        return stream;

    quint32 count;
    stream >> count;
    for (quint32 i = 0; i < count; ++i) {
        QByteArray value;
        stream >> value;
        QList<QNetworkCookie> newCookies = QNetworkCookie::parseCookies(value);
        if (newCookies.count() == 0 && value.length() != 0) {
            //qWarning() << "CookieJar: Unable to parse saved cookie:" << value;
        }
        for (int j = 0; j < newCookies.count(); ++j)
            list.append(newCookies.at(j));
        if (stream.atEnd())
            break;
    }
    return stream;
}
QT_END_NAMESPACE

CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent)
{
    qRegisterMetaTypeStreamOperators<QList<QNetworkCookie> >("QList<QNetworkCookie>");
    load();
}

CookieJar::~CookieJar()
{
    save();
}

void CookieJar::clear()
{
    QList<QNetworkCookie> cl;
    setAllCookies(cl);
    save();
}

void CookieJar::load()
{
    QSettings settings;
	QList<QNetworkCookie> cookies = qvariant_cast<QList<QNetworkCookie> >(settings.value("Web/Cookies"));
    setAllCookies(cookies);

	qDebug() << Q_FUNC_INFO << cookies;
}

void CookieJar::save() const
{
    QSettings settings;
    QList<QNetworkCookie> cookies = allCookies();
    for (int i = cookies.count() - 1; i >= 0; --i) {
        //if (cookies.at(i).isSessionCookie())
        //cookies.removeAt(i);
    }
	settings.setValue("Web/Cookies", qVariantFromValue<QList<QNetworkCookie> >(cookies));

	qDebug() << Q_FUNC_INFO << cookies;
}
