#ifndef FORM_H
#define FORM_H
#include <QVariant>
#include <QMainWindow>

namespace Ui {
    class Form;
}

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

private slots:
	//! Следующая страница
	void on_pushButton_2_clicked();
	//! Предыдущая страница
	void on_pushButton_clicked();
	//! Получен результат запроса
	void on_web_replyFinished(class QNetworkReply *replay);

private:
	Ui::Form *ui;
	class QNetworkAccessManager *m_web; //!< Веб запросы

	int m_page; //!< Текущая страница
	QVariantList m_stations; //!< Распарсенный список станций
};

#endif // FORM_H
