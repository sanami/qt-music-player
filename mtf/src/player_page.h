#pragma once
#include "data.h"

class Media;

//! Страница плеера
class PlayerPage : public MApplicationPage
{
	Q_OBJECT

public:
	PlayerPage(Media *media);
	~PlayerPage();

	void showStationInfo(Station station);

signals:
	void sig_showStationPage(Station station);

private slots:
	void on_station_clicked();
	void on_play_clicked();
	void on_close_clicked();

	//! Сообщения от плееров
	void on_media_messages(QString str);

private:
	Media *m_media; //!< Плеер из App
	Station m_station; //!< Текущая станция

	MTextEdit *m_messages; //!< Лог плеера
	MButton *m_station_btn; //!< Кнопка перехода на страницу станции
};
