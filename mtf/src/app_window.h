#pragma once

class AppWindow : public MApplicationWindow
{
    Q_OBJECT

public:
    AppWindow();
    ~AppWindow();

	void addPage(MApplicationPage *page);
	void showPage(MApplicationPage *page);

	QString getText(QString title, QString prompt, QString text = "");
	bool question(QString title, QString prompt);

public slots:
	//! Индикатор загрузки
	void showBusy(bool busy);
	//! Всплывающее сообщение
	void showMessage(QString msg, int timeout = 3000);
	//! Удалить сообщение
	void clearMessage();

signals:
	void sig_showLogPage();
	void sig_showOptionsPage();

private slots:
	void showPageForAction(QAction *action);

private:
	MApplicationPage *createPage(const QString &name);

private:
	QAction *currentAction;
	QActionGroup *actionGroup;
	QMap<QAction *, MApplicationPage *> m_pages;
};
