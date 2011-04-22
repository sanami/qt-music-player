#ifndef APP_H
#define APP_H
#include <QObject>
#include "settings.h"

class Web;
class Media;
class Task;
class MainPage;

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

	//! Получен результат запроса
	void on_web_finished(Task *task);

	//! Результат попытки подключения к станции
	void on_media_status(QVariantMap station, QString url, bool ok);

private:
	MApplicationWindow *m_app_window;
	MainPage *m_main_page;

	Web *m_web; //!< Веб запросы
	Media *m_media; //!< Плеер
	Settings m_settings;
};

#endif // APP_H
