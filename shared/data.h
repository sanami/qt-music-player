#pragma once

#define FIELD_STRING(key) QString key() const { return data.value(#key).toString(); }
#define FIELD_INT(key) int key() const { return data.value(#key).toInt(); }

//! Список объектов создаваемый из QVariant
template<class T>
struct AsList : public QList<T>
{
public:
	AsList(QVariant var_list)
	{
		foreach(QVariant var, var_list.toList())
		{
			append( T(var) );
		}
	}
};

//! Жанры
struct Genre
{
	typedef AsList<Genre> List;

	QVariantMap data;
	Genre(QVariant var)
	{
		data = var.toMap().value("genre").toMap();
	}

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_STRING(name);
};

//! Локации (Страна, город)
struct Location
{
	typedef AsList<Location> List;

	QVariantMap data;
	Location(QVariant var)
	{
		data = var.toMap().value("location").toMap();
	}

	//! Определения рабочих полей
	FIELD_INT(id);
	FIELD_STRING(name);
};
