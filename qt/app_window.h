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

public slots:
	//! Индикатор загрузки
	void showBusy(bool busy);

	//! Всплывающее сообщение
	void showMessage(QString msg, int timeout = 3000);
	//! Удалить сообщение
	void clearMessage();

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
	void on_busyAnimation();

private:
    Ui::AppWindow *ui;

	QSystemTrayIcon m_tray;
	QMenu m_trayMenu;

	QTimer *timer;
};
