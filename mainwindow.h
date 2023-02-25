#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cpuview.h"
#include "memoryview.h"
#include "system.h"
#include "vgaview.h"

#include <QMainWindow>
#include <QMdiArea>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(System *system, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    System *system;
    QMdiArea *mdiArea;
    MemoryView *lowerMemoryView;
    MemoryView *upperMemoryView;
    CpuView *cpuView;
    VgaView *vgaView;
    QDockWidget* addToDock(Qt::DockWidgetArea area, QWidget *widget, const QString &title);
    void onKeyEvent(QKeyEvent *event);
};
#endif // MAINWINDOW_H
