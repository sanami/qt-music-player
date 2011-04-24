#ifndef INFO_PAGE_H
#define INFO_PAGE_H

#include <QWidget>
#include <QVariant>
#include "data.h"

namespace Ui {
    class InfoPage;
}

class QTreeWidgetItem;

class InfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit InfoPage(QWidget *parent = 0);
    ~InfoPage();

	//! Показать данные станции
	void setStation(Station station);

signals:
	void sig_openStream(Station station, QString stream);
	void sig_addToFavorites(Station station);

private slots:
	void on_streams_itemDoubleClicked(QTreeWidgetItem* item);

	//! Сообщения от плееров
	void on_media_messages(QString str);
	void on_addToFavorites_clicked();

private:
	Ui::InfoPage *ui;
	Station m_station; //!< Текущая станция
};

#endif // INFO_PAGE_H
