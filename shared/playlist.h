#pragma once

class Playlist
{
public:
	//! Типы списков
	enum Type {
		Root      = 1,  // Корневой список
		Item      = 2,  // Обычный объект, ссылается на станцию
		History   = 3,  // Список история
		Favorites = 4,  // Список избранных пользователя
	};
};
