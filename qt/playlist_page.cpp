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
}

PlaylistPage::~PlaylistPage()
{
    delete ui;
}

void PlaylistPage::clearItems()
{
	ui->playlist->clear();
}

void PlaylistPage::showPlaylist(Playlist pl)
{
	m_current_playlist_id = pl.id();
	// Переход наверх
	int parent_id = pl.parent_id();
	if (parent_id > 0)
	{
		QString name = "[..]";
		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(PlaylistRole, parent_id);
		ui->playlist->addItem(it);
	}
}

void PlaylistPage::addItem(Playlist pl)
{
	// Только для элементы этого списка
	if (pl.parent_id() == m_current_playlist_id)
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

void PlaylistPage::removeItem(int playlist_id)
{
	//найти среди показываемых и удалить
	QListWidgetItem* it = ui->playlist->currentItem();
	for(int i=0; i<ui->playlist->count(); ++i)
	{
		QListWidgetItem* it = ui->playlist->item(i);
		int id = it->data(PlaylistRole).toInt();

		if (playlist_id == id)
		{
			delete it;
			break;
		}
	}
}

void PlaylistPage::on_playlist_itemDoubleClicked(QListWidgetItem* it)
{
	int playlist_id = it->data(PlaylistRole).toInt();
	emit sig_openPlaylist(playlist_id);
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

void PlaylistPage::on_actionDeletePlaylist_triggered()
{
	QListWidgetItem* it = ui->playlist->currentItem();
	if (it)
	{
		int playlist_id = it->data(PlaylistRole).toInt();
		emit sig_destroyPlaylist(playlist_id);
	}
}
