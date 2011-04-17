#ifndef TASK_H
#define TASK_H
#include <QUrl>
#include <QVariant>

//! Задача на выполнение
struct Task
{
	enum Op {Get, Post, Delete};
	enum Type {Unknown, Cookies, Station, Stations, Countries, Cities, Genres, Playlist, AddToPlaylist, PlaylistDestroy} type; //!< Тип запроса
	QUrl url;           //!< Адрес запроса
	QVariantMap params; //!< Доп. информация
	QByteArray result;  //!< Результат, без обработки
	QVariant json;      //!< Результат в формате JSON
	bool json_ok;       //!< Распарсен в JSON без ошибок
	bool ok;
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
