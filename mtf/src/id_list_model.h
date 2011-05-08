#pragma once

class IdListModel : public QAbstractListModel
{
public:
	IdListModel();
	~IdListModel();

	void clear();
	void add(int id);
	void remove(int id);
	void update(int id);

	int id(const QModelIndex &index) const;

protected:
	virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
	virtual QVariant data(const QModelIndex &index, int role) const;

private:
	QList<int> m_list;
};
