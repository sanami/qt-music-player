#include "playlist_page.h"
#include "playlist_manager.h"
#include "id_list_model.h"

#define PlaylistRole (Qt::UserRole + 2)

class PlaylistItemCreator : public MAbstractCellCreator<MContentItem>
{
public:
	PlaylistItemCreator(PlaylistManager *manager)
		: m_manager(manager)
	{
	}

	virtual MWidget *createCell(const QModelIndex &index, MWidgetRecycler &recycler) const
	{
		MContentItem *contentItem = new MContentItem(MContentItem::SingleTextLabel);
		updateCell(index, contentItem);
		return contentItem;
	}

	void updateCell(const QModelIndex& index, MWidget *cell) const
	{
		MContentItem *contentItem = qobject_cast<MContentItem *>(cell);
		QVariant data = index.data(Qt::DisplayRole);

		int id = data.toInt();
		Playlist &pl = m_manager->playlist(id);

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

		contentItem->setTitle(name);

//		contentItem->setTitle(data.toString());
//		QStringList rowData = data.value<QStringList>();
//		contentItem->setTitle(rowData[0]);
//		contentItem->setSubtitle(rowData[1]);
	}

private:
	PlaylistManager *m_manager;
};

struct PlaylistPageUi
{
	MLabel *name;
	MButton *up;
	MButton *create;
	MList *playlist;
	MObjectMenu *playlist_menu;
};

PlaylistPage::PlaylistPage(PlaylistManager *manager)
	: ui(new PlaylistPageUi)
	, m_manager(manager)
	, m_selected_playlist_id(0)
{
	setObjectName("PlaylistPage");
	setTitle("Playlist");

	connect(m_manager, SIGNAL(sig_clear()), this, SLOT(clearItems()));
	connect(m_manager, SIGNAL(sig_show(Playlist)), this, SLOT(showPlaylist(Playlist)));
	connect(m_manager, SIGNAL(sig_add(Playlist)), this, SLOT(addItem(Playlist)));
	connect(m_manager, SIGNAL(sig_remove(int)), this, SLOT(removeItem(int)));
	connect(m_manager, SIGNAL(sig_update(Playlist)), this, SLOT(updateItem(Playlist)));

	QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
	centralWidget()->setLayout(layout);

	// Верхние кнопки
	QGraphicsLinearLayout *layout2 = new QGraphicsLinearLayout(Qt::Horizontal);
	{
		MButton *btn = new MButton("..");
		connect(btn, SIGNAL(clicked()), SLOT(on_up_clicked()));
		layout2->addItem(btn);
		ui->up = btn;
	}
	{
		MButton *btn = new MButton("Create");
		connect(btn, SIGNAL(clicked()), SLOT(on_actionCreatePlaylist_triggered()));
		layout2->addItem(btn);
		ui->create = btn;
	}
	{
		MLabel *lab = new MLabel("[name]");
		layout2->addItem(lab);
		ui->name = lab;
	}
	layout->addItem(layout2);

	// Плейлист
	ui->playlist = new MList();
	PlaylistItemCreator *cellCreator = new PlaylistItemCreator(m_manager);
	ui->playlist->setCellCreator(cellCreator);

	m_model = new IdListModel();
	ui->playlist->setItemModel(m_model);

	connect(ui->playlist, SIGNAL(itemClicked(QModelIndex)), SLOT(on_playlist_itemClicked(QModelIndex)));
	connect(ui->playlist, SIGNAL(itemLongTapped(QModelIndex)), SLOT(on_playlist_itemLongTapped(QModelIndex)));
	layout->addItem(ui->playlist);

	layout->addStretch(100);

	// Контекстное меню списка
	ui->playlist_menu = new MObjectMenu(this);
	{
		MAction *action = new MAction("Delete", this);
		ui->playlist_menu->addAction(action);
		connect(action, SIGNAL(triggered()), SLOT(on_actionDeletePlaylist_triggered()));
	}
	{
		MAction *action = new MAction("Create", this);
		ui->playlist_menu->addAction(action);
		connect(action, SIGNAL(triggered()), SLOT(on_actionCreatePlaylist_triggered()));
	}
	{
		MAction *action = new MAction("Rename", this);
		ui->playlist_menu->addAction(action);
		connect(action, SIGNAL(triggered()), SLOT(on_actionRenamePlaylist_triggered()));
	}

	clearItems();
}

PlaylistPage::~PlaylistPage()
{
    delete ui;
	delete m_model;
}

void PlaylistPage::createContent()
{
}

void PlaylistPage::on_orientationChanged(M::Orientation orientation)
{
	qDebug() << Q_FUNC_INFO << orientation;
	if (orientation == M::Landscape)
		ui->playlist->setColumns(2);
	else
		ui->playlist->setColumns(1);
}

void PlaylistPage::clearItems()
{
	m_model->clear();
	m_current_playlist_id = 0;
	m_selected_playlist_id = 0;

	ui->name->setText("");
	ui->up->setEnabled(false);
	ui->create->setEnabled(false);
}

void PlaylistPage::showPlaylist(Playlist pl)
{
	m_current_playlist_id = pl.id();
	m_parent_playlist_id = pl.parent_id();

	ui->name->setText("["+pl.name()+"]");

	// Переход наверх
	ui->up->setEnabled(m_parent_playlist_id > 0);

	ui->create->setEnabled(pl.type() == Playlist::Favorites || pl.type() == Playlist::Root);
}

void PlaylistPage::addItem(Playlist pl)
{
	// Только для элементов этого списка
	if (pl.parent_id() == m_current_playlist_id)
	{
		m_model->add(pl.id());
	}
}

void PlaylistPage::updateItem(Playlist pl)
{
	// Найти среди показываемых и удалить
	m_model->update(pl.id());
}

void PlaylistPage::removeItem(int playlist_id)
{
	m_model->remove(playlist_id);
}

int PlaylistPage::selectedPlaylist() const
{
	return m_selected_playlist_id;
}

void PlaylistPage::on_playlist_itemClicked(const QModelIndex &index)
{
	emit sig_openPlaylist(m_model->id(index));
}

void PlaylistPage::on_playlist_itemLongTapped(const QModelIndex &index)
{
	Playlist &pl = m_manager->playlist(m_model->id(index));
	ui->playlist_menu->setTitle(pl.name());
	m_selected_playlist_id = pl.id();
	sceneManager()->appearSceneWindow(ui->playlist_menu);
}

void PlaylistPage::on_actionCreatePlaylist_triggered()
{
	// Текущий список будет родителем
	if (m_current_playlist_id)
		emit sig_createPlaylist(m_current_playlist_id);
}

void PlaylistPage::on_actionDeletePlaylist_triggered()
{
	int playlist_id = selectedPlaylist();
	if (playlist_id > 0)
	{
		emit sig_destroyPlaylist(playlist_id);
	}
}

void PlaylistPage::on_actionRenamePlaylist_triggered()
{
	int playlist_id = selectedPlaylist();
	if (playlist_id > 0)
	{
		emit sig_renamePlaylist(playlist_id);
	}
}

void PlaylistPage::on_up_clicked()
{
	emit sig_openPlaylist(m_parent_playlist_id);
}
