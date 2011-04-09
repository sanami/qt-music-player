#pragma once
#include <QObject>

class MPlayerControl;

class Media : public QObject
{
    Q_OBJECT
public:
    explicit Media(QObject *parent = 0);
	~Media();

	void open(QString url);

signals:
	//! Сообщения от плееров
	void sig_messages(QString str);

private:
	MPlayerControl *m_mplayer; //!< Управление внешним mplayer
};
