#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>

class Logger : public QObject
{
    Q_OBJECT
public:
    explicit Logger(QObject *parent = 0);

	static void init();
	static Logger *logger();

	void debug(QString str)
	{
		emit sig_debug(str);
	}

signals:
	void sig_debug(QString str);

public slots:

};

#endif // LOGGER_H
