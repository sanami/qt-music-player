#pragma once
#include <QDebug>
#include <QObject>

class Test : public QObject
{
    Q_OBJECT

public:
    explicit Test(QObject *parent = 0);

signals:

private slots:
//	initTestCase() will be called before the first testfunction is executed.
//	cleanupTestCase() will be called after the last testfunction was executed.
//	init() will be called before each testfunction is executed.
//	cleanup() will be called after every testfunction.

	void test_settings();

	void test_mplayer();

	void test_cookie_jar();
};
