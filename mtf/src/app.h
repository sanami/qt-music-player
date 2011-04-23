#ifndef APP_H
#define APP_H
#include <QObject>

class Web;
class Media;
class Task;
class AppWindow;
class MainPage;
class LogPage;
class FilterPage;
class StationsPage;

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
	void on_showPage(int page);
	void on_setServer(QString server);
	void on_requestPage(int page);

	//! Получен результат запроса
	void on_web_finished(Task *task);

	//! Результат попытки подключения к станции
	void on_media_status(QVariantMap station, QString url, bool ok);

private:
	AppWindow *m_app_window;
	MainPage *m_main_page;
	LogPage *m_log_page;
	FilterPage *m_filter_page;
	StationsPage *m_stations_page;

	Web *m_web; //!< Веб запросы
	Media *m_media; //!< Плеер
};

#endif // APP_H
