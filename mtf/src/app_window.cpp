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

void AppWindow::showPageForAction(QAction *action)
{
	qDebug() << Q_FUNC_INFO << action->isChecked();
	m_pages[action]->appear(this);
//	action->setChecked(true);
}
