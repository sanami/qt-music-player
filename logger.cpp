#include "logger.h"

bool g_use_console = true;
bool g_use_log4cxx = true;

QMutex g_console_mutex;
QRegExp g_rxMethodParams("\\(.*\\)"); //!< Параметры метода

//TODO Перейти на UTF-8
//Добавлена поддержка руских букв: проходит через toAscii, в raw_msg 1251
//TODO выбор каталога для файла лога, настройки
//TODO возможно в текущий каталог нельзя писать, использовать UserData и т.п.
//TODO показывать только названия функций, без описания параметров
static void conMessageOutput(QtMsgType type, const char *raw_msg)
{
//	QMutexLocker locker(&g_console_mutex);

	static int line = 0;
	++line;
	QString msg = QString("%1 %2: %3")
		.arg(line, 4, 10, QChar('0'))
		.arg(QThread::currentThreadId(), 0, 16, QChar('0'))
		.arg(raw_msg);

	// Удаление излишней информации
	msg.remove("__thiscall ");
	if (msg.contains("::") && g_rxMethodParams.indexIn(msg) > 0) // Параметры метода (в скобках)
	{
		int pos = g_rxMethodParams.pos(0)+1;
		int length = g_rxMethodParams.matchedLength() - 2;
		//msg.remove(pos, length);
		msg.replace(pos, length, "|");
	}

	//QTextCodec *codec = QTextCodec::codecForName("IBM 866");
	//_cprintf(codec->fromUnicode(msg).constData());

	//_cwprintf(L"%s", L"Проверка"); //работает

//	wprintf((const wchar_t *)msg.constData());
//	printf((const char *)msg.constData());
//	wprintf(L"%S\n", msg.data());
//	wprintf(L"%s\n", raw_msg);
//	wprintf(L"%s\n", msg.constData());
//	printf("%s\n", msg.toUtf8().constData());

	switch (type)
	{
	case QtDebugMsg:
		printf(" %s\n", msg.toUtf8().constData());
		break;
	case QtWarningMsg:
		printf("!%s\n", msg.toUtf8().constData());
		break;
	case QtCriticalMsg:
		printf("-%s\n", msg.toUtf8().constData());
		break;
	case QtFatalMsg:
		printf("#%s\n", msg.toUtf8().constData());
		//TODO abort();
	}
	fflush(stdout);

	Logger::logger()->debug(msg);
}

void setCodec(const QString &name)
{
	QTextCodec *codec = QTextCodec::codecForName(qPrintable(name));

	QTextCodec::setCodecForCStrings(codec);
	QTextCodec::setCodecForTr(codec);
	QTextCodec::setCodecForLocale(codec);
}

void Logger::init()
{
	setCodec("UTF-8");

	qInstallMsgHandler(conMessageOutput);
	qDebug() << "йцук";
//	qDebug() << "123";
}

Logger *Logger::logger()
{
	static Logger *s_logger = NULL;
	if (!s_logger)
	{
		s_logger = new Logger();
	}

	return s_logger;
}

Logger::Logger(QObject *parent) :
    QObject(parent)
{
}
