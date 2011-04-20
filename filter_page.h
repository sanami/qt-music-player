#ifndef FILTER_PAGE_H
#define FILTER_PAGE_H
#include <QWidget>
#include "settings.h"

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
	void showCountries(QVariantList countries);
	//! Заполнить список стран на странице фильтров
	void showCities(QVariantList cities);
	//! Заполнить список жанров на странице фильтров
	void showGenres(QVariantList genres);

	QString server() const { return m_settings.server(); }

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
