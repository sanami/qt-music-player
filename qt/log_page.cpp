#include "log_page.h"
#include "ui_log_page.h"

LogPage::LogPage(QWidget *parent)
	: QWidget(parent, Qt::Window)
	, ui(new Ui::LogPage)
{
    ui->setupUi(this);

#ifdef Q_WS_MAEMO_5
	setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
}

LogPage::~LogPage()
{
    delete ui;
}

void LogPage::addLog(QString text)
{
//	qDebug() << Q_FUNC_INFO << text;
	ui->log->appendPlainText(text);
}
