#include "id_list_model.h"

IdListModel::IdListModel()
{
}

IdListModel::~IdListModel()
{
}

void IdListModel::clear()
{
	m_list.clear();
	emit layoutChanged();
}

void IdListModel::add(int id)
{
	if (!m_list.contains(id))
	{
		m_list << id;
		emit layoutChanged();
	}
	else
	{
		Q_ASSERT( 0 );
	}
}

void IdListModel::remove(int id)
{
	Q_ASSERT( m_list.contains(id) );
	m_list.removeAll(id);
	emit layoutChanged();
}

void IdListModel::update(int id)
{
	if (m_list.contains(id))
		emit layoutChanged(); //TODO dataChanged(index)
}

int IdListModel::id(const QModelIndex &index) const
{
	return m_list.at(index.row());
}

int IdListModel::rowCount(const QModelIndex &/*parent*/) const
{
	return m_list.size();
}

QVariant IdListModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		return id(index);
	}
	return QVariant();
}
