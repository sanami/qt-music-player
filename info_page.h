#ifndef INFO_PAGE_H
#define INFO_PAGE_H

#include <QWidget>
#include <QVariant>

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
	void setStation(QVariantMap station);

signals:
	void sig_openStream(QVariantMap station, QString stream);

private:
    Ui::InfoPage *ui;
	QVariantMap m_station; //!< Текущая станция

private slots:
	void on_streams_itemDoubleClicked(QTreeWidgetItem* item);

	//! Сообщения от плееров
	void on_media_messages(QString str);
};

#endif // INFO_PAGE_H
