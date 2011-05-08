#pragma once
#include "data.h"

class StationPage : public MApplicationPage
{
    Q_OBJECT

public:
	StationPage();
	~StationPage();

	//! Показать данные станции
	void setStation(Station station);

signals:
	void sig_openStream(Station station, QString stream);
	void sig_addToFavorites(Station station);

public slots:

private slots:
	//! Сообщения от плееров
	void on_media_messages(QString str);

	void on_streams_clicked(MButton *btn);

protected:
	virtual void createContent();

private:
	struct StationPageUi *ui;

	Station m_station; //!< Текущая станция
};
