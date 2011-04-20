#ifndef FORM_H
#define FORM_H
#include <QMainWindow>
#include "task.h"

namespace Ui {
    class Form;
}

class QListWidgetItem;
class StationsPage;
class FilterPage;
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
	void on_requestCities(int country_id);
	void on_setServer(QString server);
	void on_requestPage(int page);

	//! Показать окно с детальным логом
	void on_actionLog_triggered();

	//! Получен результат запроса
	void on_web_finished(Task *task);

	//! Начать проигрывание аудиопотока
	void on_openStream(QVariantMap station, QString stream);

	//! Показать детали станции
	void on_showStationPage(QVariantMap station);

	//! Двойной клик на запись в списке
	void on_playlist_itemDoubleClicked(QListWidgetItem* item);

	//! Удалить запись из списка
	void on_actionDeletePlaylist_triggered();

	//! Результат попытки подключения к станции
	void on_media_status(QVariantMap station, QString url, bool ok);

private:
	Ui::Form *ui;

	Web *m_web; //!< Веб запросы

	StationsPage *m_stations_page; //!< Список станций
	FilterPage *m_filter_page; //!< Фильтрация станций
	InfoPage *m_station_view;  //!< Информация о станции
	PlayerPage *m_player_page; //!< Страница плеера
	LogPage *m_log_page; //!< Страница сообщений
	Media *m_media; //!< Плеер
};

#endif // FORM_H
