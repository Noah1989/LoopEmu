#pragma once

#include <QObject>
#include <QThread>
#include <iostream>

#ifdef Q_OS_WIN
#include <QWinEventNotifier>
#include <windows.h>
#else
#include <QSocketNotifier>
#endif

class ConsoleListener : public QObject
{
    Q_OBJECT

public:
    ConsoleListener();
    ~ConsoleListener();

Q_SIGNALS:
    void newChar(char c);
    void finishedGetChar(char c);

private:
#ifdef Q_OS_WIN
    QWinEventNotifier *m_notifier;
#else
    QSocketNotifier *m_notifier;
#endif

private Q_SLOTS:
	void on_finishedGetChar(char c);

private:
	QThread m_thread;
};
