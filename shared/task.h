#ifndef TASK_H
#define TASK_H
#include <QUrl>
#include <QVariant>

//! Задача на выполнение
struct Task
{
	enum Op {Get, Post, Put, Delete};
	enum Type {Unknown, Cookies, Station, Stations, Countries, Cities, Genres, Playlist, PlaylistCreate, PlaylistRename, PlaylistDestroy};

	Type type; //!< Тип запроса
	QUrl url;           //!< Адрес запроса
	QVariantMap params; //!< Доп. информация
	QByteArray result;  //!< Результат, без обработки
	QVariant json;      //!< Результат в формате JSON
	bool ok;
	bool json_ok;       //!< Распарсен в JSON без ошибок
	int error_code;
	//TODO filter

	Task()
		: ok(false)
		, json_ok(false)
		, error_code(0)
	{
	}
};

#endif // TASK_H
