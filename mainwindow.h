#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cpuview.h"
#include "memoryview.h"
#include "system.h"

#include <QMainWindow>
#include <QMdiArea>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(System *system, QWidget *parent = nullptr);
    ~MainWindow();

private:
    QMdiArea *mdiArea;
    MemoryView *lowerMemoryView;
    MemoryView *upperMemoryView;
    CpuView *cpuView;
    QDockWidget* addToDock(Qt::DockWidgetArea area, QWidget *widget, const QString &title);
};
#endif // MAINWINDOW_H