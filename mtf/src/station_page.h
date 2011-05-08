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

private slots:
	void on_streams_clicked(MButton *btn);

	//! Сообщения от плееров
	void on_media_messages(QString str);

	//! Добавить в избранное
	void on_addToFavorites_clicked();

protected:
	virtual void createContent();

private:
	struct StationPageUi *ui;

	Station m_station; //!< Текущая станция
};
