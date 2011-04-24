#include "log_page.h"
#include "logger.h"

LogPage::LogPage()
	: m_messages(NULL)
{
	setObjectName("LogPage");
	setTitle("Log");

	createContent();
}

LogPage::~LogPage()
{
	delete m_messages;
}

void LogPage::createContent()
{
	if (!m_messages)
	{
		QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
		centralWidget()->setLayout(layout);

		m_messages = new MTextEdit(MTextEditModel::MultiLine);
		setObjectName("messages");
		m_messages->setReadOnly(true);
		layout->addItem(m_messages);
	}
}

void LogPage::addLog(QString text)
{
	if (m_messages)
	{
//		m_messages->insert(text);
		// Чтобы не было цикла
		Logger::logger()->blockSignals(true);
		m_messages->setText(text  + "\n" + m_messages->text());
		Logger::logger()->blockSignals(false);
	}
}
