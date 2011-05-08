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

QString AppWindow::getItem(QString title, QString prompt, QStringList items)
{
	MWidget *centralWidget = new MWidget;
	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget->setLayout(layout);

	MComboBox *combobox = new MComboBox(centralWidget);
	combobox->setTitle(prompt);
	combobox->addItems(items);

	layout->addItem(combobox);

	MDialog* dialog = new MDialog(title, M::OkButton | M::CancelButton);
	dialog->setCentralWidget(centralWidget);

	connect(dialog, SIGNAL(disappeared()), SLOT(processDialogResult()));
	if (dialog->exec(this) == M::OkButton)
		return combobox->currentText();
	return "";
}

QString AppWindow::getText(QString title, QString prompt, QString text)
{
	MWidget *centralWidget = new MWidget;
	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget->setLayout(layout);

	MLabel *label = new MLabel(prompt, centralWidget);
	label->setStyleName("CommonTitleInverted");

	MTextEdit *textEdit = new MTextEdit(MTextEditModel::SingleLine, text, centralWidget);

	layout->addItem(label);
	layout->addItem(textEdit);

	MDialog* dialog = new MDialog(title, M::OkButton | M::CancelButton);
	dialog->setCentralWidget(centralWidget);

	connect(dialog, SIGNAL(disappeared()), SLOT(processDialogResult()));
	if (dialog->exec(this) == M::OkButton)
		return textEdit->text();
	return "";
}

bool AppWindow::question(QString title, QString prompt)
{
	MMessageBox box(title, prompt, M::OkButton | M::CancelButton);
	return box.exec(this) == M::OkButton;
}
