#pragma once

class WebProxy
{
public:
	enum Type {NoProxy, AutoProxy, ManualProxy};

	WebProxy()
		: type(NoProxy)
	{
	}

	//! Брать из http_proxy
	static QUrl autoProxy();

	Type type;
	QUrl url;
};

