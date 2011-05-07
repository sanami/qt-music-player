#pragma once
#include "playlist.h"

//! Иерархический список станций
class PlaylistPage : public MApplicationPage
{
    Q_OBJECT

public:
	PlaylistPage();
    ~PlaylistPage();

public slots:
	//! Показать список
	void showPlaylist(Playlist pl);

	//! Добавить элемент в список
	void addItem(Playlist pl);

	//! Обновить элемент в список
	void updateItem(Playlist pl);

	//! Удалить элемент из списка
	void removeItem(int playlist_id);

	//! Очистить весь список
	void clearItems();

signals:
	//! Открыть список
	void sig_openPlaylist(int playlist_id);

	//! Создать новый плейлист
	void sig_createPlaylist(int parent_id);

	//! Удалить список на сервере
	void sig_destroyPlaylist(int playlist_id);

	//! Переименовать плейлист
	void sig_renamePlaylist(int playlist_id);

private slots:
	//! Двойной клик на запись в списке
//	void on_playlist_itemDoubleClicked(QListWidgetItem* item);

	//! Удалить запись из списка
	void on_actionDeletePlaylist_triggered();

	//! Создать новый плейлист
	void on_actionCreatePlaylist_triggered();

	//! Переименовать плейлист
	void on_actionRenamePlaylist_triggered();

	void on_up_clicked();

private:
	//! Найти запись или NULL
//	QListWidgetItem *findItem(int playlist_id) const;

	//! Выбранный плейлист, или 0
	int currentPlaylist() const;

protected:
	virtual void createContent();

private:
	struct PlaylistPageUi *ui;

	int m_current_playlist_id; //!< Текущий список
	int m_parent_playlist_id;
};
