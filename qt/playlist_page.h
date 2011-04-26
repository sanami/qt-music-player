#pragma once
#include <QWidget>
#include "playlist.h"

namespace Ui {
    class PlaylistPage;
}

class PlaylistManager;

//! Иерархический список станций
class PlaylistPage : public QWidget
{
    Q_OBJECT

public:
	PlaylistPage(PlaylistManager *manager);
    ~PlaylistPage();

public slots:
	//! Показать список
	void showPlaylist(int playlist_id);

signals:
	//! Показать данные станции
	void sig_requestStation(int station_id);

	//! Открыть список
	void sig_requestPlaylist(int playlist_id);
	//! Удалить список на сервере
	void sig_destroyPlaylist(int playlist_id);
	//! Создать новый плейлист
	void sig_createPlaylist(QString name, int parent_id);

private slots:
	//! Двойной клик на запись в списке
	void on_playlist_itemDoubleClicked(QListWidgetItem* item);

	//! Удалить запись из списка
	void on_actionDeletePlaylist_triggered();

	//! Создать новый плейлист
	void on_actionCreatePlaylist_triggered();

private:
    Ui::PlaylistPage *ui;

	PlaylistManager *m_manager; //!< Все операции со списками, через менеджера
	int m_current_playlist_id; //!< Текущий список
};
