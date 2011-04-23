#pragma once

//! Просмотр отладочных сообщений
class LogPage : public MApplicationPage
{
	Q_OBJECT

public:
    LogPage();
	~LogPage();

public slots:
	void addLog(QString text);

protected:
	virtual void createContent();

private:
	MTextEdit *m_messages;
};
