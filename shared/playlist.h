#pragma once
#include "data.h"

//! Плейлист: список или элемент
struct Playlist
{
	typedef QList<int> List; //!< Список ID, а не самих объектов

	//! Типы списков
	enum Type {
		UnknownType = 0, // Ошибка
		Root      = 1,  // Корневой список
		Item      = 2,  // Обычный объект, ссылается на станцию
		History   = 3,  // Список история
		Favorites = 4   // Список избранных пользователя
	};

	enum Action {
		UnknownAction = 0, // Ошибка
		AllList   = 1,  // Весь список
		Create    = 2,  // Добавить
		Destroy   = 3   // Удалить
	};

	//! Стандартные конструкторы и оператор
	Playlist()
	{
//		qDebug() << Q_FUNC_INFO;
	}
	Playlist(const Playlist &that)
		: data(that.data)
		, children(that.children)
	{
//		qDebug() << Q_FUNC_INFO;
	}
	Playlist &operator=(const Playlist &that)
	{
//		qDebug() << Q_FUNC_INFO;
		data = that.data;
		children = that.children;
		return *this;
	}

//	//! Создается менеджером
//	explicit Playlist(const QVariantMap &data)
//		: data(data)
//	{
//	}

//	explicit Playlist(const QVariantMap &data, const Playlist::List &children)
//		: data(data)
//		, children(children)
//	{
//	}

	//! Тип
	Type type() const
	{
		return (Type)(data["playlist_type_id"].toInt());
	}

	bool isNull() const { return data.isEmpty(); }

	void add(int child_id)
	{
		if (!children.contains(child_id))
		{
			children << child_id;
		}
	}

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_INT(parent_id);
	FIELD_STRING(name);
	FIELD_INT(station_id);

	//! Все данные
	QVariantMap data;
	Playlist::List children;
};
