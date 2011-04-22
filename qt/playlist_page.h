#ifndef PLAYLIST_PAGE_H
#define PLAYLIST_PAGE_H

#include <QWidget>

namespace Ui {
    class PlaylistPage;
}

//! Иерархический список станций
class PlaylistPage : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistPage(QWidget *parent = 0);
    ~PlaylistPage();

	//! Показать список
	void showPlaylist(QVariantMap playlist);

	//! Очистить все списки
	void reset();
	const QMap<int, QVariantMap> &all() const { return m_all_playlists; }

signals:
	//! Данные станции
	void sig_requestStation(int station_id);

	//! Данные списка
	void sig_requestPlaylist(int playlist_id);
	//! Удалить список на сервере
	void sig_destroyPlaylist(int playlist_id);
	//! Создать новый плейлист
	void sig_createPlaylist(QString name, int parent_id);

private slots:
	//! Двойной клик на запись в списке
	void on_playlist_itemDoubleClicked(QListWidgetItem* item);

	//! Удалить запись из списка
	void on_actionDeletePlaylist_triggered();

	//! Создать новый плейлист
	void on_actionCreatePlaylist_triggered();

private:
    Ui::PlaylistPage *ui;

	QVariantMap m_current_playlist; //!< Текущий список
	QMap<int, QVariantMap> m_all_playlists; //!< Все избранные id => список
};

#endif // PLAYLIST_PAGE_H
