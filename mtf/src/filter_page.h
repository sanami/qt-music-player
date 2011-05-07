#pragma once
#include "data.h"

//! Настройки поиска/фильтра станций
class FilterPage : public MApplicationPage
{
    Q_OBJECT
public:
	FilterPage();
	~FilterPage();

	const QVariantMap &filter() const { return m_filter; }
	//! Заполнить список стран на странице фильтров
	void showCountries(Location::List countries);
	//! Заполнить список стран на странице фильтров
	void showCities(Location::List cities);
	//! Заполнить список жанров на странице фильтров
	void showGenres(Genre::List genres);

signals:
	void sig_requestCities(int country_id);
	void sig_requestPage(int page);

private slots:
	//! Поиск станций по фильтру
	void on_pushButton_filter_apply_clicked();

	//! Выбрана страна, запросить список городов
	void on_comboBox_country_currentIndexChanged(int index);

protected:
	virtual void createContent();

private:
	struct FilterPageUi *ui;

	QVariantMap m_filter; //!< Параметры запроса
};
