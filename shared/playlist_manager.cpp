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
			if (id)
			{
				show(id);
			}
		}
		break;
	case Playlist::Create:
		{
			int id = create(json.toMap());
			Playlist pl = playlist(id);
			if (pl.type() == Playlist::Root)
			{
				// Создан корень
				show(id);
			}
			else
			{
				// Обычный элемент
				emit sig_add(pl);
			}
		}
		break;
	case Playlist::Rename:
		{
			int id = update(json.toMap());
			Playlist pl = playlist(id);
			// Обычный элемент
			emit sig_update(pl);
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
	if (pl.id() > 0)
	{
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
	return 0; // Ошибка
}

int PlaylistManager::update(const QVariantMap &var)
{
	Playlist pl;
	pl.data = var.value("playlist").toMap();
	if (pl.id() > 0)
	{
		// Обновить только атрибуты, без children
		m_playlists[pl.id()].data = pl.data;

		// Вернуть только ID;
		return pl.id();
	}
	return 0; // Ошибка
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

