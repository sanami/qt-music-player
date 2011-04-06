#ifndef TASK_H
#define TASK_H
#include <QUrl>
#include <QVariant>

//! Задача на выполнение
struct Task
{
	enum Type {Unknown, Stations, Countries, Cities, Genres} type; //!< Тип запроса
	QUrl url; //!< Адрес запроса
	QVariantMap params; //!< Доп. информация
	QVariant result; //!< Результат
	bool ok;
	//TODO filter
};

#endif // TASK_H
