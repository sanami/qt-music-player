#pragma once
#include <MApplicationPage>

//! Главное меню
class MainPage : public MApplicationPage
{
    Q_OBJECT
public:
	MainPage(QGraphicsItem *parent = 0);
    virtual ~MainPage();

signals:
	void sig_showPage(int);

protected:
	//! This is a virtual method that gets called only once by the framework,
	//! right before your page begins to appear on the scene.
	virtual void createContent();
};
