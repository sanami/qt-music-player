#ifndef PLAYLIST_PAGE_H
#define PLAYLIST_PAGE_H

#include <QWidget>

namespace Ui {
    class PlaylistPage;
}

class PlaylistPage : public QWidget
{
    Q_OBJECT

public:
    explicit PlaylistPage(QWidget *parent = 0);
    ~PlaylistPage();

	//! Показать список
	void showPlaylist(QVariantMap playlist);

signals:
	void sig_requestStation(int station_id);
	void sig_requestPlaylist(int playlist_id);
	void sig_destroyPlaylist(int playlist_id);

private slots:
	//! Двойной клик на запись в списке
	void on_playlist_itemDoubleClicked(QListWidgetItem* item);

	//! Удалить запись из списка
	void on_actionDeletePlaylist_triggered();

private:
    Ui::PlaylistPage *ui;
};

#endif // PLAYLIST_PAGE_H
