#pragma once
#include "playlist.h"

//! Управление всеми избранными
class PlaylistManager : public QObject
{
    Q_OBJECT

public:
    explicit PlaylistManager(QObject *parent = 0);

	//! Обработать пришедшие в JSON данные
	void process(QVariant json, Playlist::Action action);

	//! Очистить список избранных
	void clear()
	{
		m_playlists.clear();
//		m_root = 0;
//		m_history = 0;
	}

	bool contains(int id) const
	{
		return m_playlists.contains(id);
	}

	Playlist &playlist(int id)
	{
		return m_playlists[id];
	}

	Playlist &parent(const Playlist &pl)
	{
		return playlist(pl.parent_id());
	}

	Playlist &parent(int id)
	{
		return parent(playlist(id));
	}

//	//! Корневой элемент
//	Playlist &root()
//	{
//		return playlist(m_root);
//	}

//	//! История
//	Playlist &history()
//	{
//		return playlist(m_history);
//	}

	//! Все избранные, в которые можно добавлять элементы
	Playlist::List favorites() const;

public slots:
	void on_openPlaylist(int id);

signals:
	//! Запросить данные станции
	void sig_requestStation(int station_id);
	//! Запросить данные списка
	void sig_requestPlaylist(int playlist_id);

	void sig_clear();
	void sig_show(Playlist pl);
	void sig_add(Playlist pl);
	void sig_remove(int id);

private:
	//! Создать плейлист из JSON данных
	int create(const QVariantMap &var);

	//! Удалить плейлист из JSON данных
	void destroy(int id);

	//! Показать содержимое списка
	void show(int id);

private:
	QMap<int, Playlist> m_playlists; //! Глобальное хранилище
//	int m_root;
//	int m_history;
};
