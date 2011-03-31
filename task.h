#ifndef TASK_H
#define TASK_H
#include <QUrl>
#include <QVariant>

//! Задача на выполнение
struct Task
{
	enum Type {General, Stations, Streams, Favorites, Filters} type;
	QUrl url; //!< Адрес запроса
	QVariantMap params; //!< Доп. информация
	QVariant result; //!< Результат
	bool ok;
	//TODO filter
};

#endif // TASK_H
