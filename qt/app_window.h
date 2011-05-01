#pragma once
#include <QMainWindow>

namespace Ui {
    class AppWindow;
}

class AppWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit AppWindow(QWidget *parent = 0);
    ~AppWindow();

	void addPage(QWidget *page);
	void showPage(QWidget *page);
	QWidget *currentPage() const;

signals:
	void sig_showLogPage();

protected:
#ifdef Q_WS_WIN
	virtual bool winEvent(MSG *msg, long *result);
#endif

private:
	void initTray();

private slots:
	void on_trayActivated(QSystemTrayIcon::ActivationReason reason);

private:
    Ui::AppWindow *ui;

	QSystemTrayIcon m_tray;
	QMenu m_trayMenu;
};
