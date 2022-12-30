#include <curses.h>
#include "consolelistener.h"

ConsoleListener::ConsoleListener()
{
	QObject::connect(
		this, &ConsoleListener::finishedGetChar,
		this, &ConsoleListener::on_finishedGetChar, 
		Qt::QueuedConnection
	);
#ifdef Q_OS_WIN
	m_notifier = new QWinEventNotifier(GetStdHandle(STD_INPUT_HANDLE));
#else
    m_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read);
#endif 
	// NOTE : move to thread because std::getline blocks, 
	//        then we sync with main thread using a QueuedConnection with finishedGetLine
	m_notifier->moveToThread(&m_thread);
	QObject::connect(
		&m_thread , &QThread::finished,
		m_notifier, &QObject::deleteLater
	);
#ifdef Q_OS_WIN
	QObject::connect(m_notifier, &QWinEventNotifier::activated,
#else
	QObject::connect(m_notifier, &QSocketNotifier::activated,
#endif 
	[this]() {
		int c;
		c = getch();
		Q_EMIT this->finishedGetChar((char)c);
	});
	m_thread.start();
}

void ConsoleListener::on_finishedGetChar(char c)
{
	Q_EMIT this->newChar(c);
}

ConsoleListener::~ConsoleListener()
{
	m_thread.quit();
	m_thread.wait();
}
