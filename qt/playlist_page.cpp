#include "playlist_page.h"
#include "ui_playlist_page.h"
#include "playlist.h"

#define PlaylistRole (Qt::UserRole + 2)

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

void PlaylistPage::showPlaylist(QVariantMap result)
{
	qDebug() << Q_FUNC_INFO << result;
	ui->playlist->clear();

	// Данные самого списка
	QVariantMap playlist = result["playlist"].toMap();
	m_current_playlist = playlist;

	if (!playlist["parent_id"].isNull())
	{
		// Переход на родителя
		int parent_id = playlist["parent_id"].toInt();
		QVariantMap parent;
		parent["id"] = parent_id;
		QListWidgetItem *it = new QListWidgetItem("[..]");
		it->setData(PlaylistRole, parent);
		ui->playlist->addItem(it);
	}

	// Данные о под-списках
	QVariantList children = result["children"].toList();
	foreach(QVariant item_var, children)
	{
		QVariantMap item = item_var.toMap();
		QVariantMap info = item["playlist"].toMap();

		QString name = info["id"].toString() + " ";
		switch (info["playlist_type_id"].toInt())
		{
		case Playlist::Item:
			name += info["name"].toString();
			break;
		default:
			name += "[" + info["name"].toString() + "]";

			// Сохранить в список всех избранных
			m_all_playlists[info["id"].toInt()] = info;
		}

		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(PlaylistRole, info);
		ui->playlist->addItem(it);
	}
}

void PlaylistPage::on_playlist_itemDoubleClicked(QListWidgetItem* it)
{
	QVariantMap playlist = it->data(PlaylistRole).toMap();
	qDebug() << Q_FUNC_INFO << playlist;
	switch (playlist["playlist_type_id"].toInt())
	{
	case Playlist::Item:
		emit sig_requestStation(playlist["station_id"].toInt());
		break;
	default:
		emit sig_requestPlaylist(playlist["id"].toInt());
	}
}

void PlaylistPage::on_actionDeletePlaylist_triggered()
{
	QListWidgetItem* it = ui->playlist->currentItem();
	if (it)
	{
		QVariantMap playlist = it->data(PlaylistRole).toMap();
		int playlist_id = playlist["id"].toInt();
		emit sig_destroyPlaylist(playlist_id);
	}
}

void PlaylistPage::on_actionCreatePlaylist_triggered()
{
	QString name = QInputDialog::getText(this, "Create playlist", "Name:");
	if (!name.isEmpty())
	{
		// Текущий список будет родителем
		int parent_id = m_current_playlist["id"].toInt();

		// Отправить запрос на создание
		emit sig_createPlaylist(name, parent_id);
	}
}
