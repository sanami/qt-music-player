#pragma once
#include "data.h"

class StationsModel : public QAbstractListModel
{
    Q_OBJECT

public:
	StationsModel();
	~StationsModel();

	void setStations(const Station::List &stations);
	void reset(QString message);

	const Station *station(const QModelIndex &index) const;

signals:

public slots:

protected:
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;

private:
	Station::List *m_stations;
	QString m_message; //!< Сообщение, если список пустой
};
