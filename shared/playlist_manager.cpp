#include "playlist_manager.h"

PlaylistManager::PlaylistManager(QObject *parent)
	: QObject(parent)
	, m_root(0)
	, m_history(0)
{
}

void PlaylistManager::process(QVariant json)
{
	int id = create(json.toMap());
	emit sig_showPlaylist(id);
//	Playlist pl = playlist(id);

//	if (pl.parent_id() == 0)
//	{

//	}
}

Playlist::List PlaylistManager::favorites()
{
	Playlist::List all;
	foreach (Playlist pl, m_playlists)
	{
		if (pl.type() == Playlist::Favorites)
			all << pl.id();
	}

	return all;
}
