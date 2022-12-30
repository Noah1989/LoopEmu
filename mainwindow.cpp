#include "mainwindow.h"

#include <QDockWidget>
#include <QMdiSubWindow>

MainWindow::MainWindow(System *system, QWidget *parent)
    : QMainWindow(parent),
      mdiArea(new QMdiArea),
      lowerMemoryView(new MemoryView(system->lowerMemory, 0x0000)),
      upperMemoryView(new MemoryView(system->upperMemory, 0x8000)),
      cpuView(new CpuView(system->cpu)),
      vgaView(new VgaView(system->nameMemory, system->attrMemory,
                          system->pattMemory, system->paleMemory))
{
    setCentralWidget(mdiArea);
    addToDock(Qt::RightDockWidgetArea, lowerMemoryView, "Lower Memory");
    addToDock(Qt::RightDockWidgetArea, upperMemoryView, "Upper Memory");
    QDockWidget *cpuDock = addToDock(Qt::LeftDockWidgetArea, cpuView, "CPU");
    resizeDocks({cpuDock}, {1}, Qt::Horizontal);

    mdiArea->addSubWindow(vgaView)->setWindowTitle("VGA Output");

    connect(system, &System::frame, lowerMemoryView, &MemoryView::update);
    connect(system, &System::frame, upperMemoryView, &MemoryView::update);
    connect(system, &System::frame, cpuView, &CpuView::update);
    connect(system, &System::frame, vgaView, &VgaView::update);
}

MainWindow::~MainWindow()
{
}

QDockWidget* MainWindow::addToDock(Qt::DockWidgetArea area, QWidget *widget, const QString &title)
{
    QDockWidget *dock = new QDockWidget(title);
    dock->setWidget(widget);
    addDockWidget(area, dock);
    return dock;
}
