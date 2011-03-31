#include <QDebug>
#include <QFile>
#include <QDesktopServices>
#include <QLabel>
#include "form.h"
#include "web.h"
#include "ui_form.h"

#define StationRole (Qt::UserRole + 1)

Form::Form(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Form)
{
#ifdef Q_WS_MAEMO_5
//	setAttribute(Qt::WA_Maemo5AutoOrientation, true);
	setAttribute(Qt::WA_Maemo5PortraitOrientation, true);
	setAttribute(Qt::WA_Maemo5StackedWindow);
#endif
    ui->setupUi(this);

	m_station_view = new QLabel(this);
	m_station_view->setWindowFlags(m_station_view->windowFlags() | Qt::Window);
#ifdef Q_WS_MAEMO_5
	m_station_view->setAttribute(Qt::WA_Maemo5StackedWindow);
#endif

	ui->stations->addAction(ui->actionOpenStation);

	m_web = new Web();
	connect(m_web, SIGNAL(sig_finished(Task *)), this, SLOT(on_web_finished(Task *)));

	m_page = 1;
	requestPage();
}

Form::~Form()
{
    delete ui;
	delete m_web;
}

void Form::toggleBusy(int check)
{
	bool busy = (check == Qt::Checked);
#ifdef Q_WS_MAEMO_5
	setAttribute(Qt::WA_Maemo5ShowProgressIndicator, busy);
#endif
	ui->pushButton->setDisabled(busy);
	ui->pushButton_2->setDisabled(busy);
	ui->pushButton_3->setDisabled(busy);
	ui->stations->setDisabled(busy);
}

void Form::log(QString text)
{
	qDebug() << Q_FUNC_INFO << text;
	ui->plainTextEdit->appendPlainText(text);
}

void Form::requestPage()
{
	m_web->requestStations(m_page);

	ui->stations->clear();
	ui->stations->addItem("Loading...");
	toggleBusy(Qt::Checked);

	log(QString("showPage %1").arg(m_page));
}

void Form::on_web_finished(Task *task)
{
	qDebug() << Q_FUNC_INFO;

	m_stations = task->result.toList();
	showStations();
}

void Form::showStations()
{
	ui->stations->clear();
	toggleBusy(Qt::Unchecked);

	foreach(QVariant station_var, m_stations)
	{
		QVariantMap station = station_var.toMap().value("station").toMap();
		qDebug() << station;
		QString name = station["name"].toString();

		QListWidgetItem *it = new QListWidgetItem(name);
		it->setData(StationRole, station);
		ui->stations->addItem(it);
	}
}

void Form::on_pushButton_clicked()
{
	// <<
	if (m_page > 1)
	{
		m_page--;
		requestPage();
	}

}

void Form::on_pushButton_2_clicked()
{
	// >>
	m_page++;
	requestPage();
}

QUrl Form::firstUrl(QVariantMap station) const
{
	QUrl url;
	QList<QVariant> streams = station["streams"].toList();
	if (!streams.isEmpty())
	{
		url = streams[0].toMap()["stream"].toMap()["url"].toString();
	}
	return url;
}

void Form::on_stations_currentItemChanged(QListWidgetItem* it, QListWidgetItem* previous)
{
	if (it)
	{
		QVariantMap station = it->data(StationRole).toMap();
		//qDebug() << station;
		//qDebug() << station["streams"].toList()[0].toMap()["stream"].toMap()["url"];
		log(firstUrl(station).toString());
	}
}

void Form::on_actionOpenStation_triggered()
{
	qDebug() << Q_FUNC_INFO;

	QListWidgetItem* it = ui->stations->currentItem();
	if (it)
	{
		QVariantMap station = it->data(StationRole).toMap();
		QDesktopServices::openUrl(firstUrl(station));
	}
}

void Form::on_pushButton_3_clicked()
{
	on_actionOpenStation_triggered();
}

void Form::on_stations_itemDoubleClicked(QListWidgetItem* it)
{
	QVariantMap station = it->data(StationRole).toMap();

	QString info;
	foreach(QString name, station.keys())
	{
		info += QString("%1: %2\n").arg(name).arg(station[name].toString());

		if (name == "streams")
		{
			QList<QVariant> streams = station["streams"].toList();
			foreach(QVariant stream_var, streams)
			{
				QString url = stream_var.toMap()["stream"].toMap()["url"].toString();
				info += QString("  %1\n").arg(url);
			}
		}
	}
	m_station_view->setText(info);

	m_station_view->show();
}
