#ifndef INFO_PAGE_H
#define INFO_PAGE_H

#include <QWidget>
#include <QVariant>

namespace Ui {
    class InfoPage;
}

class QTreeWidgetItem;

class InfoPage : public QWidget
{
    Q_OBJECT

public:
    explicit InfoPage(QWidget *parent = 0);
    ~InfoPage();

	//! Показать данные станции
	void setStation(QVariantMap station);

private:
    Ui::InfoPage *ui;

	class Media *m_media; //!< Плеер

private slots:
	void on_streams_itemDoubleClicked(QTreeWidgetItem* item);
};

#endif // INFO_PAGE_H
