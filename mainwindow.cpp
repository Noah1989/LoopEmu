#include "mainwindow.h"

#include <QDockWidget>

MainWindow::MainWindow(System *system, QWidget *parent)
    : QMainWindow(parent),
      mdiArea(new QMdiArea),
      lowerMemoryView(new MemoryView(system->lowerMemory)),
      upperMemoryView(new MemoryView(system->upperMemory))
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
