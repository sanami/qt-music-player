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
class PlaylistPage;
class LogPage;
class Web;
class Media;

class Form : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

public slots:
	//! Индикатор загрузки
	void showBusy(bool busy);

	//! Всплывающее сообщение
	void showMessage(QString msg);

private:
	//! Запросить текущую страницу
	void requestPage();

private slots:
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

	//! Результат попытки подключения к станции
	void on_media_status(QVariantMap station, QString url, bool ok);

private:
	Ui::Form *ui;

	Web *m_web; //!< Веб запросы
	Media *m_media; //!< Плеер

	StationsPage *m_stations_page; //!< Список станций
	FilterPage *m_filter_page; //!< Фильтрация станций
	InfoPage *m_station_view;  //!< Информация о станции
	PlayerPage *m_player_page; //!< Страница плеера
	PlaylistPage *m_playlist_page; //!< Списки
	LogPage *m_log_page; //!< Страница сообщений
};

#endif // FORM_H
