#ifndef FILTER_PAGE_H
#define FILTER_PAGE_H
#include <QWidget>
#include "settings.h"
#include "data.h"

namespace Ui {
    class FilterPage;
}

class FilterPage : public QWidget
{
    Q_OBJECT

public:
    explicit FilterPage(QWidget *parent = 0);
    ~FilterPage();

	const QVariantMap &filter() const { return m_filter; }

	//! Заполнить список стран на странице фильтров
	void showCountries(Location::List countries);
	//! Заполнить список стран на странице фильтров
	void showCities(Location::List cities);
	//! Заполнить список жанров на странице фильтров
	void showGenres(Genre::List genres);

	QString server() const;

signals:
	void sig_setServer(QString server);
	void sig_requestCities(int country_id);
	void sig_requestPage(int page);

private slots:
	//! Поиск станций по фильтру
	void on_pushButton_filter_apply_clicked();

	//! Выбрана страна, запросить список городов
	void on_comboBox_country_currentIndexChanged(int index);

	//! Установить адрес сервера
	void on_comboBox_server_currentIndexChanged(QString );

private:
    Ui::FilterPage *ui;

	QVariantMap m_filter; //!< Параметры запроса
	Settings m_settings;
};

#endif // FILTER_PAGE_H
