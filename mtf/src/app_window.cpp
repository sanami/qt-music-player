#include "app_window.h"

AppWindow::AppWindow()
{
	setObjectName("AppWindow");

	// We want the toolbar to show actions as tabs. I.e., be a tab bar.
	setToolbarViewType(MToolBar::tabType);

	actionGroup = new QActionGroup(this);
	actionGroup->setExclusive(true);
	connect(actionGroup, SIGNAL(triggered(QAction*)), SLOT(showPageForAction(QAction*)));

	// Пункты меню
	{
		MAction *action = new MAction("Options", this);
		action->setLocation(MAction::ApplicationMenuLocation);
		connect(action, SIGNAL(triggered()), SIGNAL(sig_showOptionsPage()));
		addAction(action);
	}
	{
		MAction *action = new MAction("Log", this);
		action->setLocation(MAction::ApplicationMenuLocation);
		connect(action, SIGNAL(triggered()), SIGNAL(sig_showLogPage()));
		addAction(action);
	}
	{
		MAction *action = new MAction("Quit", this);
		action->setLocation(MAction::ApplicationMenuLocation);
		connect(action, SIGNAL(triggered()), SLOT(close()));
		addAction(action);
	}

	show();
}

AppWindow::~AppWindow()
{
}

void AppWindow::addPage(MApplicationPage *page)
{
	MAction *action = new MAction(page->title(), this);
	action->setLocation(MAction::ToolBarLocation);
//	action->setCheckable(true);
	action->setActionGroup(actionGroup);
	addAction(action);

	m_pages[action] = page;
}

void AppWindow::showPage(MApplicationPage *page)
{
	page->appear(this);
}

void AppWindow::showBusy(bool busy)
{
	Q_UNUSED(busy);
	//TODO индикатор рабочих запросов
	//MProgressIndicator *spinner = new MProgressIndicator(NULL, MProgressIndicator::spinnerType);
	//spinner->setUnknownDuration(true);
}

void AppWindow::showMessage(QString msg, int timeout)
{
	Q_UNUSED(timeout);

	MBanner *infoBanner = new MBanner();
	infoBanner->setStyleName("InformationBanner");
	infoBanner->setTitle(msg);
	infoBanner->appear(this, MSceneWindow::DestroyWhenDone);
	// Не работает, делается через CSS для #InformationBanner
	//infoBanner->style()->setProperty("disappear-timeout", 100);
}

void AppWindow::clearMessage()
{
//	statusBar()->clearMessage();
}

void AppWindow::showPageForAction(QAction *action)
{
	qDebug() << Q_FUNC_INFO << action->isChecked();
	m_pages[action]->appear(this);
//	action->setChecked(true);
}
