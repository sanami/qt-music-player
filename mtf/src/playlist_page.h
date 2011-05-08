#pragma once
#include "playlist.h"

class IdListModel;
class PlaylistManager;

//! Иерархический список станций
class PlaylistPage : public MApplicationPage
{
    Q_OBJECT

public:
	PlaylistPage(PlaylistManager *manager);
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
	//! Поворот экрана
	void on_orientationChanged(M::Orientation orientation);

	//! Клик на запись в списке
	void on_playlist_itemClicked(const QModelIndex &index);

	//! Удалить запись из списка
	void on_actionDeletePlaylist_triggered();

	//! Создать новый плейлист
	void on_actionCreatePlaylist_triggered();

	//! Переименовать плейлист
	void on_actionRenamePlaylist_triggered();

	void on_up_clicked();

private:
	//! Выбранный плейлист, или 0
	int currentPlaylist() const;

protected:
	virtual void createContent();

private:
	struct PlaylistPageUi *ui;

	IdListModel *m_model; //!< Модель для отображения
	PlaylistManager *m_manager; //!< Менеджер списками из App

	int m_current_playlist_id; //!< Текущий список
	int m_parent_playlist_id;
};
