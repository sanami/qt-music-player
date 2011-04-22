#include <QGraphicsLinearLayout>
#include "main_page.h"

MainPage::MainPage(QGraphicsItem *parent)
	: MApplicationPage(parent)
{
	setObjectName("MainPage");
	setTitle("Main Page");
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
	actions << "Stations" << "Filter";

	MButton *btn;
	for (int i = 0; i < actions.count(); i++) {
		btn = new MButton();
		btn->setText(actions[i]);

		layout->addItem(btn);
		buttonGroup->addButton(btn, i);
    }
	layout->addStretch();

    // A MButtonGroup is used because it provides a signal that is emitted
    // whenever any of the buttons added to it gets clicked.
    // MButtonGroup groups the buttons logically only. Their graphical
    // positioning is still handled by the layout.
	connect(buttonGroup, SIGNAL(buttonClicked(int)), SIGNAL(sig_showPage(int)));
}

//void MainPage::displayArtist(int artistIndex)
//{
//    Artist *artist = artistsList[artistIndex];

//    ArtistPage *artistPage = new ArtistPage(artist);

//    // When the back button is pressed, the page gets dismissed.
//    // By setting MSceneWindow::DestroyWhenDismissed we don't have to
//    // keep tabs on this page since it will be automatically destroyed
//    // after the dismissal.
//    artistPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
//}
