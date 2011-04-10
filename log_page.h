#ifndef LOG_PAGE_H
#define LOG_PAGE_H

#include <QWidget>

namespace Ui {
    class LogPage;
}

class LogPage : public QWidget
{
    Q_OBJECT

public:
    explicit LogPage(QWidget *parent = 0);
    ~LogPage();

public slots:
	void addLog(QString text);

private:
    Ui::LogPage *ui;
};

#endif // LOG_PAGE_H
