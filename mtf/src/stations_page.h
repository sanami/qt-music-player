#pragma once

class StationsPage : public MApplicationPage
{
	Q_OBJECT

public:
    StationsPage();

	//! Показать список с текущей страницы
	void showStations(QVariantMap stations);

	//! Есть ли записи в списке
	bool isEmpty() const;

signals:
	void sig_showStation(QVariantMap station);
	void sig_requestPage(int page);

protected:
	virtual void createContent();

private:
	//! Обновить состояние контролов
	void updateControls(bool enable);

	void requestPage();

private:
	int m_current_page;   //!< Текущая страница
	int m_num_pages;      //!< Общее кол-во страниц
};
