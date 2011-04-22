#ifndef STATIONS_PAGE_H
#define STATIONS_PAGE_H

#include <QWidget>

namespace Ui {
    class StationsPage;
}

//! Список станций
class StationsPage : public QWidget
{
    Q_OBJECT

public:
    explicit StationsPage(QWidget *parent = 0);
    ~StationsPage();

	//! Показать список с текущей страницы
	void showStations(QVariantMap stations);

signals:
	void sig_showStation(QVariantMap station);
	void sig_requestPage(int page);

private slots:
	//! Следующая страница
	void on_pushButton_2_clicked();
	//! Предыдущая страница
	void on_pushButton_clicked();

	//! Перейти на страницу станции
	void on_stations_itemDoubleClicked(QListWidgetItem* item);

private:
	//! Обновить состояние контролов
	void updateControls(bool enable);

	void requestPage();

private:
    Ui::StationsPage *ui;

	int m_current_page;   //!< Текущая страница
	int m_num_pages;      //!< Общее кол-во страниц
};

#endif // STATIONS_PAGE_H
