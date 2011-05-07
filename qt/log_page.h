#pragma once

namespace Ui {
    class LogPage;
}

//! Просмотр отладочных сообщений
class LogPage : public QWidget
{
	Q_OBJECT

public:
	explicit LogPage(QWidget *parent = 0);
	~LogPage();

public slots:
	void addLog(QString text);

private:
    Ui::LogPage *ui;
};
