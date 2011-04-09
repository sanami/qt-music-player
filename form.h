#ifndef FORM_H
#define FORM_H
#include <QMainWindow>
#include "task.h"

namespace Ui {
    class Form;
}

class QListWidgetItem;
class InfoPage;
class Web;
class Media;

class Form : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
	//! Запросить текущую страницу
	void requestPage();
	//! Показать список с текущей страницы
	void showStations(QVariantMap stations);
	//! Заполнить список стран на странице фильтров
	void showCountries(QVariantList countries);
	//! Заполнить список стран на странице фильтров
	void showCities(QVariantList cities);
	//! Заполнить список жанров на странице фильтров
	void showGenres(QVariantList genres);

	//! Отладочные сообщения
	void log(QString text);

	//! Индикатор загрузки
	void toggleBusy(int check);

	//! Первая ссылка из данных станции
	QUrl firstUrl(QVariantMap station) const;

private slots:
	void on_pushButton_filter_apply_clicked();
	void on_stations_itemDoubleClicked(QListWidgetItem* item);

	//! Выбрана страна, запросить список городов
	void on_comboBox_country_currentIndexChanged(int index);

	//! Установить адрес сервера
	void on_comboBox_server_currentIndexChanged(QString );

	//! Открыть станцию
	void on_actionOpenStation_triggered();
	//! Выбрана станция
	void on_stations_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

	//! Следующая страница
	void on_pushButton_2_clicked();
	//! Предыдущая страница
	void on_pushButton_clicked();

	//! Получен результат запроса
	void on_web_finished(Task *task);

private:
	Ui::Form *ui;

	Web *m_web; //!< Веб запросы

	int m_current_page; //!< Текущая страница
	int m_num_pages;    //!< Общее кол-во страниц

	QVariantMap m_filter; //!< Параметры запроса

	InfoPage *m_station_view; //!< Информация о станции
};

#endif // FORM_H
