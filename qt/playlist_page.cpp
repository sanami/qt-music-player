#include "playlist_page.h"
#include "ui_playlist_page.h"
#include "playlist_manager.h"

#define PlaylistRole (Qt::UserRole + 2)

PlaylistPage::PlaylistPage(PlaylistManager *manager)
	: m_manager(manager)
	, ui(new Ui::PlaylistPage)
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

void PlaylistPage::showPlaylist(int playlist_id)
{
	qDebug() << Q_FUNC_INFO << playlist_id;
	ui->playlist->clear();

	// Данные самого списка
	Playlist playlist = m_manager->playlist(playlist_id);
	m_current_playlist_id = playlist_id;

	// Переход наверх
	Playlist parent = m_manager->parent(playlist_id);
	if (!parent.isNull())
	{
		QString name = QString("[..](%1)").arg(parent.name());
		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(PlaylistRole, parent.id());
		ui->playlist->addItem(it);
	}

	// Данные о дочерних списках/элементах
	foreach(int child_id, playlist.children)
	{
		Playlist child_pl = m_manager->playlist(child_id);

		QString name = QString::number(child_pl.id()) + " ";
		switch (child_pl.type())
		{
		case Playlist::Item:
			// Обычный элемент
			name += child_pl.name();
			break;
		default:
			// Список
			name += "[" + child_pl.name() + "]";
		}

		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(PlaylistRole, child_pl.id());
		ui->playlist->addItem(it);
	}
}

void PlaylistPage::on_playlist_itemDoubleClicked(QListWidgetItem* it)
{
	Playlist playlist = m_manager->playlist(it->data(PlaylistRole).toInt());
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
		// Отправить запрос на создание
		emit sig_createPlaylist(name, m_current_playlist_id); // Текущий список будет родителем
	}
}
