#include "stations_model.h"

StationsModel::StationsModel()
	: m_stations(NULL)
{
}

StationsModel::~StationsModel()
{
	delete m_stations;
}

void StationsModel::setStations(const Station::List &stations)
{
	if (!stations.isEmpty())
	{
		if (!m_stations)
			m_stations = new Station::List(stations);
		else
			*m_stations = stations;
		emit layoutChanged();
	}
	else
	{
		reset("Not found");
	}
}

void StationsModel::reset(QString message)
{
	m_message = message;
	delete m_stations;
	m_stations = NULL;
	emit layoutChanged();
}

const Station *StationsModel::station(const QModelIndex &index) const
{
	if (m_stations && m_stations->size() > 0)
	{
		const Station &station = m_stations->at(index.row());
		return &station;
	}
	return NULL;
}

int StationsModel::rowCount(const QModelIndex &/*parent*/) const
{
	if (m_stations && m_stations->size() > 0)
		return m_stations->size();

	// Пустой список
	return 1;
}

QVariant StationsModel::data(const QModelIndex &index, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (m_stations && m_stations->size() > 0)
		{
			const Station &station = m_stations->at(index.row());

			QStringList rowData;
			rowData << station.name();

			QStringList all;
			foreach(Location loc, station.locations())
				all << loc.name();
			rowData << all.join(", ");
			return QVariant(rowData);
		}

		// Пустой список
		return QVariant(QStringList(m_message) << "");
	}
	return QVariant();
}
