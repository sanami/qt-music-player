#include "web_proxy.h"

QUrl WebProxy::autoProxy()
{
	// Брать из http_proxy=http://nokes.nokia.com:8080/
	QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
	QString httpProxy = env.value("http_proxy");
	if (httpProxy.isEmpty())
	{
		httpProxy = env.value("HTTP_PROXY");
	}
	return QUrl(httpProxy);
}
