#include "mainwindow.h"

#include <QDockWidget>

MainWindow::MainWindow(System *system, QWidget *parent)
    : QMainWindow(parent),
      mdiArea(new QMdiArea),
      lowerMemoryView(new MemoryView(system->lowerMemory, 0x0000)),
      upperMemoryView(new MemoryView(system->upperMemory, 0x8000))
{
    setCentralWidget(mdiArea);
    addToDock(lowerMemoryView, "Lower Memory");
    addToDock(upperMemoryView, "Upper Memory");
}

MainWindow::~MainWindow()
{
}

void MainWindow::addToDock(QWidget *widget, const QString &title)
{
    QDockWidget *dock = new QDockWidget(title, this);
    dock->setWidget(widget);
    addDockWidget(Qt::RightDockWidgetArea, dock);
}
