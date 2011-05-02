#pragma once
#include "task.h"
#include "data.h"
#include "playlist.h"

class AppWindow;
class StationsPage;
class FilterPage;
class InfoPage;
class PlayerPage;
class PlaylistPage;
class LogPage;
class Web;
class Media;
class PlaylistManager;

class App : public QObject
{
    Q_OBJECT

public:
	App();
	~App();

private:
	//! Запросить текущую страницу
	void requestPage();
	//! Всплывающее сообщение
	void showMessage(QString msg, int timeout = 3000);

private slots:
	void on_setServer(QString server);
	void on_requestPage(int page);

	//! Показать окно с детальным логом
	void on_actionLog_triggered();

	//! Получен результат запроса
	void on_web_finished(Task *task);

	//! Начать проигрывание аудиопотока
	void on_openStream(Station station, QString stream);

	//! Показать детали станции
	void on_showStationPage(Station station);

	//! Добавление станции в избранное
	void on_addStationToFavorites(Station station);
	//! Открыть список или станцию
	void on_openPlaylist(int playlist_id);
	//! Создать новый список
	void on_createPlaylist(int parent_id);
	//! Удаление записи в списке
	void on_destroyPlaylist(int playlist_id);
	//! Переименовать избранное
	void on_renamePlaylist(int playlist_id);

	//! Результат попытки подключения к станции
	void on_media_status(Station station, QString url, bool ok);

private:
	Web *m_web; //!< Веб запросы
	Media *m_media; //!< Плеер
	PlaylistManager *m_manager; //!< Менеджер списками избранных/истории

	AppWindow *m_app_window; //! Окно приложения
	StationsPage *m_stations_page; //!< Список станций
	FilterPage *m_filter_page; //!< Фильтрация станций
	InfoPage *m_station_view;  //!< Информация о станции
	PlayerPage *m_player_page; //!< Страница плеера
	PlaylistPage *m_playlist_page; //!< Списки
	LogPage *m_log_page; //!< Страница сообщений
};
