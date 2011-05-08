#pragma once
#include "data.h"

namespace Ui {
    class StationPage;
}

class QTreeWidgetItem;

class StationPage : public QWidget
{
    Q_OBJECT

public:
	explicit StationPage(QWidget *parent = 0);
	~StationPage();

	//! Показать данные станции
	void setStation(Station station);

signals:
	void sig_openStream(Station station, QString stream);
	void sig_addToFavorites(Station station);

private slots:
	void on_streams_itemDoubleClicked(QTreeWidgetItem* item);

	//! Сообщения от плееров
	void on_media_messages(QString str);

	//! Добавить в избранное
	void on_addToFavorites_clicked();

private:
	Ui::StationPage *ui;

	Station m_station; //!< Текущая станция
};

