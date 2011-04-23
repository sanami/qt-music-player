#include <QGraphicsLinearLayout>
#include "main_page.h"

MainPage::MainPage(QGraphicsItem *parent)
	: MApplicationPage(parent)
{
	setObjectName("MainPage");
	setTitle("Main");
}

MainPage::~MainPage()
{
}

void MainPage::createContent()
{
    // We want to organize our items in a single column. A vertical
    // layout enables us to easily achieve this arrangement.
    // Layout classes take care of setting the correct geometry (size and position)
    // of the items added to it.
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget()->setLayout(layout);

    // Items in a vertical layout are arranged from top to bottom.
	layout->addItem(new MLabel("Actions:"));

    MButtonGroup *buttonGroup = new MButtonGroup(this);

	QStringList actions;
	actions << "Stations" << "Filter" << "Log";

	for (int i = 0; i < actions.count(); i++)
	{
		MButton *btn = new MButton();
		btn->setText(actions[i]);

		layout->addItem(btn);
		buttonGroup->addButton(btn, i);
    }
	layout->addStretch();

	connect(buttonGroup, SIGNAL(buttonClicked(int)), SIGNAL(sig_showPage(int)));
}
