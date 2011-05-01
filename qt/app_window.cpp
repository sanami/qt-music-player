#include "app_window.h"
#include "ui_app_window.h"

AppWindow::AppWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::AppWindow)
{
#ifdef Q_WS_MAEMO_5
//	setAttribute(Qt::WA_Maemo5AutoOrientation, true);
	setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
	setAttribute(Qt::WA_Maemo5StackedWindow); // Слайд между окнами
#endif
	ui->setupUi(this);

	statusBar()->hide();
	menuBar()->addAction(ui->actionLog);

	initTray();

	connect(ui->actionLog, SIGNAL(triggered()), SIGNAL(sig_showLogPage()));
	connect(ui->actionExit, SIGNAL(triggered()), SLOT(close()));

#if defined(Q_WS_S60)
	showMaximized();
#else
	show();
#endif
}

AppWindow::~AppWindow()
{
    delete ui;
}

void AppWindow::addPage(QWidget *page)
{
	ui->tabWidget->addTab(page, page->windowTitle());
}

void AppWindow::showPage(QWidget *page)
{
	ui->tabWidget->setCurrentWidget(page);
}

QWidget *AppWindow::currentPage() const
{
	ui->tabWidget->currentWidget();
}

void AppWindow::initTray()
{
	m_tray.setIcon(QIcon(":/resources/tray.ico"));
	m_tray.setToolTip("Heroku");
//	m_trayMenu.addAction(ui.actionHidePopup);
//	m_trayMenu.addSeparator();
	m_trayMenu.addAction(ui->actionExit);
	m_tray.setContextMenu(&m_trayMenu);

	connect(&m_tray, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), SLOT(on_trayActivated(QSystemTrayIcon::ActivationReason)));

	m_tray.show();
}

#ifdef Q_WS_WIN
bool AppWindow::winEvent(MSG *msg, long *result)
{
	if (msg->message == WM_SYSCOMMAND && msg->wParam == SC_MINIMIZE)
	{
		hide();
// 		if (g_options.minimizeToTray)
// 		{
// 			m_trayIcon.show();
// 		}
		*result = 0;
		return true;
	}
	return false;
}
#endif

void AppWindow::on_trayActivated(QSystemTrayIcon::ActivationReason reason)
{
	if (reason == QSystemTrayIcon::Trigger)
	{
		bool visible = !isVisible();
		if (!visible && (windowState() & Qt::WindowMinimized))
		{
			setWindowState(windowState() ^ Qt::WindowMinimized);
			activateWindow();
			//m_tray.show();
		}
		else
		{
			setVisible(visible);
			if (visible)
			{
				activateWindow();
				//m_tray.hide();
			}
			else
			{
				if (windowState() & Qt::WindowMinimized)
				{
					setWindowState(windowState() ^ Qt::WindowMinimized);
					//m_tray.show();
				}
			}
		}
	}
}
