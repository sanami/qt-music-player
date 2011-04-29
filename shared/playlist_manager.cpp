#include "playlist_manager.h"

PlaylistManager::PlaylistManager(QObject *parent)
	: QObject(parent)
{
	clear();
}

void PlaylistManager::process(QVariant json, Playlist::Action action)
{
	switch(action)
	{
	case Playlist::AllList:
		{
			// Весь список
			clear();
			int id = create(json.toMap());
			show(id);
		}
		break;
	case Playlist::Create:
		{
			int id = create(json.toMap());
			emit sig_add(playlist(id));
		}
		break;
	case Playlist::Destroy:
		{
			QVariantMap var = json.toMap();
			int id = var.value("playlist_id").toInt();
			destroy(id);
			emit sig_remove(id);
		}
		break;
	default:
		Q_ASSERT( 0 );
	}
}

int PlaylistManager::create(const QVariantMap &var)
{
	Playlist pl;
	pl.data = var.value("playlist").toMap();
	// Добавить в хранилище
	m_playlists[pl.id()] = pl;

//	if (!m_root && pl.type() == Playlist::Root)
//		m_root = pl.id();
//	if (!m_history && pl.type() == Playlist::History)
//		m_history = pl.id();

	parent(pl).add(pl.id());

	foreach (QVariant child_var, var.value("children").toList())
	{
		// Рекурсивно создаст для всей иерархии
		create(child_var.toMap());
	}

	// Вернуть только ID;
	return pl.id();
}

void PlaylistManager::destroy(int id)
{
	//Удалить children
	foreach(int child_id, playlist(id).children)
	{
		destroy(child_id);
	}

	m_playlists.remove(id);
}

Playlist::List PlaylistManager::favorites() const
{
	Playlist::List all;
	foreach (Playlist pl, m_playlists)
	{
		if (pl.type() == Playlist::Favorites)
			all << pl.id();
	}

	return all;
}

void PlaylistManager::show(int id)
{
	qDebug() << Q_FUNC_INFO << id;
	emit sig_clear();

	// Данные самого списка
	Playlist pl = playlist(id);
	emit sig_show(pl);

	// Данные о дочерних списках/элементах
	foreach(int child_id, pl.children)
	{
		Playlist child_pl = playlist(child_id);
		emit sig_add(child_pl);
	}
}

void PlaylistManager::on_openPlaylist(int id)
{
	Playlist pl = playlist(id);
	switch (pl.type())
	{
	case Playlist::Item:
		emit sig_requestStation(pl.station_id());
		break;
	default:
		if (contains(id))
			show(id);
		else
			emit sig_requestPlaylist(pl.id());
	}
}
