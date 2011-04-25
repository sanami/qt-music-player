#include "playlist_page.h"
#include "ui_playlist_page.h"

#define PlaylistRole (Qt::UserRole + 2)

QMap<int, Playlist> Playlist::g_all_playlists;

PlaylistPage::PlaylistPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaylistPage)
{
    ui->setupUi(this);

	connect(ui->create, SIGNAL(clicked()), ui->actionCreatePlaylist, SLOT(trigger()));

	// Контекстное меню списка
	ui->playlist->addAction(ui->actionDeletePlaylist);
	ui->playlist->addAction(ui->actionCreatePlaylist);
}

PlaylistPage::~PlaylistPage()
{
    delete ui;
}

void PlaylistPage::reset()
{
	m_all_playlists.clear();
}

void PlaylistPage::showPlaylist(Playlist playlist)
{
//	qDebug() << Q_FUNC_INFO << playlist;
	ui->playlist->clear();

	// Данные самого списка
	m_current_playlist = playlist;

	if (playlist.parent_id() > 0)
	{
		// Переход на родителя
		QListWidgetItem *it = new QListWidgetItem("[..]");
		it->setData(PlaylistRole, playlist.parent_id());
		ui->playlist->addItem(it);
	}

	// Данные о под-списках
	foreach(Playlist item, playlist.children)
	{
		QString name = QString::number(item.id()) + " ";

		switch (item.type())
		{
		case Playlist::Item:
			name += item.name();
			break;
		default:
			name += "[" + item.name() + "]";

			// Сохранить в список всех избранных
			m_all_playlists[item.id()] = item;
		}

		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(PlaylistRole, item.id());
		ui->playlist->addItem(it);
	}
}

void PlaylistPage::on_playlist_itemDoubleClicked(QListWidgetItem* it)
{
	Playlist playlist = Playlist::playlist(it->data(PlaylistRole).toInt());
	qDebug() << Q_FUNC_INFO << playlist.data;
	switch (playlist.type())
	{
	case Playlist::Item:
		emit sig_requestStation(playlist.station_id());
		break;
	default:
		emit sig_requestPlaylist(playlist.id());
	}
}

void PlaylistPage::on_actionDeletePlaylist_triggered()
{
	QListWidgetItem* it = ui->playlist->currentItem();
	if (it)
	{
		int playlist_id = it->data(PlaylistRole).toInt();
		emit sig_destroyPlaylist(playlist_id);
	}
}

void PlaylistPage::on_actionCreatePlaylist_triggered()
{
	QString name = QInputDialog::getText(this, "Create playlist", "Name:");
	if (!name.isEmpty())
	{
		// Текущий список будет родителем
		int parent_id = m_current_playlist.id();

		// Отправить запрос на создание
		emit sig_createPlaylist(name, parent_id);
	}
}
