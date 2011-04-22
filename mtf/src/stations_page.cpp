#include "stations_page.h"

StationsPage::StationsPage()
{
	setObjectName("StationsPage");
	setTitle("Stations");
}

void StationsPage::createContent()
{
	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	// Items in a vertical layout are arranged from top to bottom.
	layout->addItem(new MLabel("Stations:"));
}
