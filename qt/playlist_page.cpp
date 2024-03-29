#include "playlist_page.h"
#include "ui_playlist_page.h"

#define PlaylistRole (Qt::UserRole + 2)

PlaylistPage::PlaylistPage()
	: ui(new Ui::PlaylistPage)
{
    ui->setupUi(this);

	connect(ui->create, SIGNAL(clicked()), ui->actionCreatePlaylist, SLOT(trigger()));

	// Контекстное меню списка
	ui->playlist->addAction(ui->actionDeletePlaylist);
	ui->playlist->addAction(ui->actionCreatePlaylist);
	ui->playlist->addAction(ui->actionRenamePlaylist);

	clearItems();
}

PlaylistPage::~PlaylistPage()
{
    delete ui;
}

void PlaylistPage::clearItems()
{
	ui->playlist->clear();
	m_current_playlist_id = 0;

	ui->name->clear();
	ui->up->setEnabled(false);
	ui->create->setEnabled(false);
}

void PlaylistPage::showPlaylist(Playlist pl)
{
	m_current_playlist_id = pl.id();
	m_parent_playlist_id = pl.parent_id();

	ui->name->setText("["+pl.name()+"]");

	// Переход наверх
	//	if (m_parent_playlist_id > 0)
	//	{
	//		QString name = "[..]";
	//		QListWidgetItem *it = new QListWidgetItem(name);
	//		it->setData(PlaylistRole, m_parent_playlist_id);
	//		ui->playlist->addItem(it);
	//	}
	ui->up->setEnabled(m_parent_playlist_id > 0);

	ui->create->setEnabled(pl.type() == Playlist::Favorites || pl.type() == Playlist::Root);
}

void PlaylistPage::addItem(Playlist pl)
{
	// Только для элементов этого списка
	if (pl.parent_id() == m_current_playlist_id && !findItem(pl.id()))
	{
		QString name = QString::number(pl.id()) + " ";
		switch (pl.type())
		{
		case Playlist::Item:
			// Обычный элемент
			name += pl.name();
			break;
		default:
			// Список
			name += "[" + pl.name() + "]";
		}

		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(PlaylistRole, pl.id());
		ui->playlist->addItem(it);
	}
}

void PlaylistPage::updateItem(Playlist pl)
{
	// Найти среди показываемых и удалить
	QListWidgetItem* it = findItem(pl.id());
	if (it)
	{
		QString name = QString::number(pl.id()) + " ";
		switch (pl.type())
		{
		case Playlist::Item:
			// Обычный элемент
			name += pl.name();
			break;
		default:
			// Список
			name += "[" + pl.name() + "]";
		}

		it->setText(name);
	}
}

void PlaylistPage::removeItem(int playlist_id)
{
	// Найти среди показываемых и удалить
	QListWidgetItem* it = findItem(playlist_id);
	if (it)
	{
		delete it;
	}
}

QListWidgetItem *PlaylistPage::findItem(int playlist_id) const
{
	for(int i=0; i<ui->playlist->count(); ++i)
	{
		QListWidgetItem* it = ui->playlist->item(i);
		int id = it->data(PlaylistRole).toInt();

		if (playlist_id == id)
		{
			return it;
		}
	}
	return NULL;
}

int PlaylistPage::currentPlaylist() const
{
	QListWidgetItem* it = ui->playlist->currentItem();
	if (it)
	{
		return it->data(PlaylistRole).toInt();
	}
	return 0;
}

void PlaylistPage::on_playlist_itemDoubleClicked(QListWidgetItem* it)
{
	int playlist_id = it->data(PlaylistRole).toInt();
	emit sig_openPlaylist(playlist_id);
}

void PlaylistPage::on_actionCreatePlaylist_triggered()
{
	// Текущий список будет родителем
	if (m_current_playlist_id)
		emit sig_createPlaylist(m_current_playlist_id);
}

void PlaylistPage::on_actionDeletePlaylist_triggered()
{
	int playlist_id = currentPlaylist();
	if (playlist_id > 0)
	{
		emit sig_destroyPlaylist(playlist_id);
	}
}

void PlaylistPage::on_actionRenamePlaylist_triggered()
{
	int playlist_id = currentPlaylist();
	if (playlist_id > 0)
	{
		emit sig_renamePlaylist(playlist_id);
	}
}

void PlaylistPage::on_up_clicked()
{
	emit sig_openPlaylist(m_parent_playlist_id);
}
