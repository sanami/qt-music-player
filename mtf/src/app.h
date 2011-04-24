#ifndef APP_H
#define APP_H
#include <QObject>
#include "data.h"

class Web;
class Media;
class Task;
class AppWindow;
class LogPage;
class FilterPage;
class StationsPage;
class StationPage;

class App : public QObject
{
    Q_OBJECT
public:
    explicit App(QObject *parent = 0);
	~App();

signals:

public slots:
	//! Индикатор загрузки
	void showBusy(bool busy);

	//! Всплывающее сообщение
	void showMessage(QString msg, int timeout = 3000);

private slots:
	void on_setServer(QString server);
	void on_requestPage(int page);

	//! Получен результат запроса
	void on_web_finished(Task *task);

	//! Начать проигрывание аудиопотока
	void on_openStream(Station station, QString stream);

	//! Показать детали станции
	void on_showStationPage(Station station);

	//! Результат попытки подключения к станции
	void on_media_status(Station station, QString url, bool ok);

private:
	AppWindow *m_app_window;
	LogPage *m_log_page;
	FilterPage *m_filter_page;
	StationsPage *m_stations_page;
	StationPage *m_station_page;

	Web *m_web; //!< Веб запросы
	Media *m_media; //!< Плеер
};

#endif // APP_H
