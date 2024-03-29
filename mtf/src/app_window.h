#pragma once

//! Управляет всеми страницами
class AppWindow : public MApplicationWindow
{
    Q_OBJECT

public:
    AppWindow();
    ~AppWindow();

	void addPage(MApplicationPage *page);
	void showPage(MApplicationPage *page);

	QString getItem(QString title, QString prompt, QStringList items);
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

	MApplicationPage *m_current_page; //!< Текущая активная страница
	bool m_busy; //!< Показывать индикатор загрузки
};
