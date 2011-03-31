#ifndef FORM_H
#define FORM_H
#include <QMainWindow>
#include "task.h"

namespace Ui {
    class Form;
}

class QListWidgetItem;
class QLabel;
class Web;

class Form : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();

private:
	//! Запросить текущую страницу
	void requestPage();
	//! Показать список с текущей страницы
	void showStations();

	//! Отладочные сообщения
	void log(QString text);

	//! Индикатор загрузки
	void toggleBusy(int check);

	//! Первая ссылка из данных станции
	QUrl firstUrl(QVariantMap station) const;

private slots:
	void on_stations_itemDoubleClicked(QListWidgetItem* item);
 void on_pushButton_3_clicked();

	//! Открыть станцию
	void on_actionOpenStation_triggered();
	//! Выбрана станция
	void on_stations_currentItemChanged(QListWidgetItem* current, QListWidgetItem* previous);

	//! Следующая страница
	void on_pushButton_2_clicked();
	//! Предыдущая страница
	void on_pushButton_clicked();

	//! Получен результат запроса
	void on_web_finished(Task *task);

private:
	Ui::Form *ui;
	Web *m_web; //!< Веб запросы

	int m_page; //!< Текущая страница
	QVariantList m_stations; //!< Распарсенный список станций
	QLabel *m_station_view;
};

#endif // FORM_H
