#pragma once
#include "playlist.h"

//! Управление всеми избранными
class PlaylistManager : public QObject
{
    Q_OBJECT

public:
    explicit PlaylistManager(QObject *parent = 0);

	//! Обработать пришедшие в JSON данные
	void process(QVariant json);

	//! Очистить список избранных
	void clear()
	{
		m_playlists.clear();
	}

	void add(const Playlist &pl)
	{
		m_playlists[pl.id()] = pl;
	}

	void remove(const Playlist &pl)
	{
		m_playlists.remove(pl.id());
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

	//! Корневой элемент
	Playlist &root()
	{
		return playlist(m_root);
	}

	//! История
	Playlist &history()
	{
		return playlist(m_history);
	}

	//! Все избранные, в которые можно добавлять элементы
	Playlist::List favorites();

signals:
	void sig_showPlaylist(int id);

public slots:

private:
	//! Создать плейлист из JSON данных
	int create(const QVariantMap &var)
	{
		Playlist pl;
		pl.data = var.value("playlist").toMap();
		foreach (QVariant child_var, var.value("children").toList())
		{
			// Рекурсивно создаст для всей иерархии
			pl.children << create(child_var.toMap());
		}
		// Добавить с хранилище
		add(pl);

		// Вернуть только ID;
		return pl.id();
	}

private:
	QMap<int, Playlist> m_playlists; //! Глобальное хранилище
	int m_root;
	int m_history;
};
