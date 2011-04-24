#pragma once

class AppWindow : public MApplicationWindow
{
    Q_OBJECT
public:
	AppWindow();

	void addPage(MApplicationPage *page);

signals:
	void sig_showLogPage();

public slots:

private slots:
	void showPageForAction(QAction *action);

private:
	MApplicationPage *createPage(const QString &name);

private:
	QAction *currentAction;
	QActionGroup *actionGroup;
	QMap<QAction *, MApplicationPage *> m_pages;
};
