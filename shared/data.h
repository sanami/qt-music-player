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
	FIELD_INT(status);
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

	Location::List locations() const
	{
		return Location::List(data["locations"]);
	}

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_STRING(name);

	//! Все данные
	QVariantMap data;
};
Q_DECLARE_METATYPE(Station);
