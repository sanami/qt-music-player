#ifndef FORM_H
#define FORM_H
#include <QMainWindow>
#include "task.h"
#include "settings.h"

namespace Ui {
    class Form;
}

class QListWidgetItem;
class InfoPage;
class PlayerPage;
class LogPage;
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

	//! Показать список
	void showPlaylist(QVariantMap playlist);

	//! Отладочные сообщения
	void log(QString text);

	//! Индикатор загрузки
	void toggleBusy(int check);

signals:
	//! Сообщения в окно лога
	void sig_log(QString str);

private slots:
	void on_actionLog_triggered();
//	void on_actionPlayer_triggered();

	//! Поиск станций по фильтру
	void on_pushButton_filter_apply_clicked();

	//! Перейти на страницу станции
	void on_stations_itemDoubleClicked(QListWidgetItem* item);

	//! Выбрана страна, запросить список городов
	void on_comboBox_country_currentIndexChanged(int index);

	//! Установить адрес сервера
	void on_comboBox_server_currentIndexChanged(QString );

	//! Открыть станцию
	void on_actionOpenStation_triggered();

	//! Следующая страница
	void on_pushButton_2_clicked();
	//! Предыдущая страница
	void on_pushButton_clicked();

	//! Получен результат запроса
	void on_web_finished(Task *task);

	//! Начать проигрывание аудиопотока
	void on_openStream(QVariantMap station, QString stream);

	//! Показать детали станции
	void on_showStationPage(QVariantMap station);

	void on_playlist_itemDoubleClicked(QListWidgetItem* item);

private:
	Ui::Form *ui;

	Web *m_web; //!< Веб запросы
	QVariantMap m_filter; //!< Параметры запроса
	int m_current_page;   //!< Текущая страница
	int m_num_pages;      //!< Общее кол-во страниц

	InfoPage *m_station_view;  //!< Информация о станции
	PlayerPage *m_player_page; //!< Страница плеера
	LogPage *m_log_page; //!< Страница сообщений
	Media *m_media; //!< Плеер
	Settings m_settings;
};

#endif // FORM_H
