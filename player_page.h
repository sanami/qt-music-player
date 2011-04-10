#ifndef PLAYER_PAGE_H
#define PLAYER_PAGE_H

#include <QWidget>

namespace Ui {
    class PlayerPage;
}

class Media;

class PlayerPage : public QWidget
{
    Q_OBJECT

public:
	explicit PlayerPage(Media *media, QWidget *parent = 0);
    ~PlayerPage();

	void showStationInfo(QVariantMap station);

signals:
	void sig_showStationPage(QVariantMap station);

private:
    Ui::PlayerPage *ui;
	Media *m_media;
	QVariantMap m_station; //!< Текущая станция

private slots:
	void on_station_clicked();
	void on_pushButton_play_clicked();
	void on_pushButton_close_clicked();

	//! Сообщения от плееров
	void on_media_messages(QString str);
};

#endif // PLAYER_PAGE_H
