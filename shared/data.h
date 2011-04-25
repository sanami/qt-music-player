#pragma once

#define FIELD_STRING(key) QString key() const { return data.value(#key).toString(); }
#define FIELD_INT(key) int key() const { return data.value(#key).toInt(); }

//! Список объектов создаваемый из QVariant
template<class T>
struct AsList : public QList<T>
{
public:
	AsList()
	{
//		qDebug() << Q_FUNC_INFO;
	}
//	AsList(const AsList& that)
//	{
//		qDebug() << Q_FUNC_INFO;
//	}
	explicit AsList(QVariant var_list)
	{
//		qDebug() << Q_FUNC_INFO;
		foreach(QVariant var, var_list.toList())
		{
			append( T(var) );
		}
	}
//	AsList &operator=(const AsList &that)
//	{
//		qDebug() << Q_FUNC_INFO;
//		data = that.data; children = that.children;
//		return *this;
//	}
};

//! Жанры
struct Genre
{
	typedef AsList<Genre> List;

	QVariantMap data;
	explicit Genre(QVariant var)
	{
		data = var.toMap().value("genre").toMap();
	}

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_STRING(name);
};
//Q_DECLARE_METATYPE(Genre);


//! Локации (Страна, город)
struct Location
{
	typedef AsList<Location> List;

	QVariantMap data;
	explicit Location(QVariant var)
	{
		data = var.toMap().value("location").toMap();
	}

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_STRING(name);
};
//Q_DECLARE_METATYPE(Location);

//! Аудиопотоки
struct Stream
{
	typedef AsList<Stream> List;

	QVariantMap data;
	explicit Stream(QVariant var)
	{
		data = var.toMap().value("stream").toMap();
	}

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_STRING(url);
	FIELD_STRING(bitrate);
	FIELD_STRING(codec);
};
//Q_DECLARE_METATYPE(Stream);

//! Станция
struct Station
{
	typedef AsList<Station> List;

	Station() {}
	Station(const Station &that) : data(that.data) {}
	Station &operator=(const Station &that) { data = that.data; return *this; }

	explicit Station(QVariant var)
	{
		data = var.toMap().value("station").toMap();
	}

	Stream::List streams() const
	{
		return Stream::List(data["streams"]);
	}

	Genre::List genres() const
	{
		return Genre::List(data["genres"]);
	}

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_STRING(name);

	//! Все данные
	QVariantMap data;
};
Q_DECLARE_METATYPE(Station);

//! Плейлист: список или элемент
struct Playlist
{
	typedef AsList<Playlist> List;

	//! Типы списков
	enum Type {
		Unknown   = 0,  // Ошибка
		Root      = 1,  // Корневой список
		Item      = 2,  // Обычный объект, ссылается на станцию
		History   = 3,  // Список история
		Favorites = 4  // Список избранных пользователя
	};

	Playlist()
	{
//		qDebug() << Q_FUNC_INFO;
	}
	Playlist(const Playlist &that) : data(that.data)
	{
//		qDebug() << Q_FUNC_INFO;
	}
	Playlist &operator=(const Playlist &that)
	{
//		qDebug() << Q_FUNC_INFO;
		data = that.data; children = that.children; return *this;
	}

	explicit Playlist(QVariant var)
		: children(var.toMap().value("children"))
	{
		qDebug() << Q_FUNC_INFO;
		data = var.toMap().value("playlist").toMap();

		add_to_all(*this);
	}

	//! Тип
	Type type() const
	{
		return (Type)(data["playlist_type_id"].toInt());
	}

	bool isNull() const { return data.isEmpty(); }

	Playlist &parent() { return Playlist::playlist(parent_id()); }

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_INT(parent_id);
	FIELD_STRING(name);
	FIELD_INT(station_id);

	//! Все данные
	QVariantMap data;
	Playlist::List children;

	//! Глобальное хранилище
	static Playlist &playlist(int id)
	{
		return g_all_playlists[id];
	}

private:
	static void add_to_all(const Playlist &pl)
	{
		g_all_playlists[pl.id()] = pl;
	}
	static QMap<int, Playlist> g_all_playlists;
};
