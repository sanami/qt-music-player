#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QFile>
#include <QUrl>
#include "../qjson/src/parser.h"
#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::Form)
	, m_page(1)
{
    ui->setupUi(this);

	m_web = new QNetworkAccessManager(this);
	connect(m_web, SIGNAL(finished(QNetworkReply*)), this, SLOT(on_web_replyFinished(QNetworkReply*)));

	requestPage();
}

Form::~Form()
{
    delete ui;
}

void Form::log(QString text)
{
	qDebug() << Q_FUNC_INFO << text;
	ui->plainTextEdit->appendPlainText(text);
}

void Form::requestPage()
{
	QString url = QString("http://localhost:3000/stations.json?page=%1").arg(m_page);
	m_web->get(QNetworkRequest(QUrl(url)));

	log(QString("showPage %1").arg(url));
}

void Form::on_web_replyFinished(QNetworkReply *replay)
{
	qDebug() << Q_FUNC_INFO << replay->bytesAvailable();

//	QFile file("../1/0329/stations.json");
//	file.open(QFile::ReadOnly | QFile::Text);
//	QByteArray json = file.readAll();
//	qDebug() << json;

	QByteArray json = replay->readAll();

	bool ok;
	QJson::Parser parser;
	QVariant result = parser.parse(json, &ok);
	if (ok)
	{
		m_stations = result.toList();
		showStations();
	}
	else
	{
		log("Bad replay");
	}

	// Do not directly delete it inside the slot
	replay->deleteLater();
}

void Form::showStations()
{
	ui->stations->clear();

	foreach(QVariant station_var, m_stations)
	{
		QVariantMap station = station_var.toMap().value("station").toMap();
		qDebug() << station;
		QString name = station["name"].toString();
		ui->stations->addItem(name);
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

