#include "mainwindow.h"
#include "ps2.h"

#include <QApplication>
#include <QDockWidget>
#include <QMdiSubWindow>
#include <QKeyEvent>
#include <iostream>

MainWindow::MainWindow(System *system, QWidget *parent)
    : QMainWindow(parent),
      system(system),
      mdiArea(new QMdiArea),
      lowerMemoryView(new MemoryView(system->lowerMemory, 0x0000)),
      upperMemoryView(new MemoryView(system->upperMemory, 0x8000)),
      cpuView(new CpuView(system->cpu)),
      vgaView(new VgaView(system->video.nameMemory, system->video.attrMemory,
                          system->video.pattMemory, system->video.paleMemory,
                          system->video.vscrx, system->video.vscry, system->video.vscrh))
{
    mdiArea->setOption(QMdiArea::DontMaximizeSubWindowOnActivation);
    setCentralWidget(mdiArea);

    addToDock(Qt::RightDockWidgetArea, lowerMemoryView, "Lower Memory");
    addToDock(Qt::RightDockWidgetArea, upperMemoryView, "Upper Memory");
    QDockWidget *cpuDock = addToDock(Qt::LeftDockWidgetArea, cpuView, "CPU");
    resizeDocks({cpuDock}, {1}, Qt::Horizontal);

    QMdiSubWindow *vgaWindow = mdiArea->addSubWindow(vgaView);
    vgaWindow->setWindowTitle("VGA Output");
        connect(vgaWindow, &QMdiSubWindow::windowStateChanged, this, [this, vgaWindow](Qt::WindowStates oldState, Qt::WindowStates newState){
        if (newState & Qt::WindowMaximized) {
            vgaWindow->setWindowFlag(Qt::FramelessWindowHint);
            mdiArea->setMinimumSize(640,480);
        }
    });

    // See eventFilter(): the emulated keyboard has to see keys before Qt does.
    qApp->installEventFilter(this);

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

// Every key event goes to the emulated keyboard, before Qt can spend it on
// focus navigation. Without this the cursor keys never reach the machine at
// all: QMainWindow::keyPressEvent only sees what the focused widget ignored,
// and the docks and MDI area consume the arrows and Tab to move focus.
// Nothing here takes text input -- the register fields are read-only -- so the
// guest can have the whole keyboard.
bool MainWindow::eventFilter(QObject *watched, QEvent *ev)
{
    if (ev->type() == QEvent::KeyPress || ev->type() == QEvent::KeyRelease) {
        onKeyEvent(static_cast<QKeyEvent *>(ev));
        return true;
    }
    return QMainWindow::eventFilter(watched, ev);
}

void MainWindow::keyPressEvent(QKeyEvent *ev)
{
   onKeyEvent(ev);
}

void MainWindow::keyReleaseEvent(QKeyEvent *ev)
{
    onKeyEvent(ev);
}

void MainWindow::onKeyEvent(QKeyEvent *ev) {
    ps2Code code = ps2CodeFromEvent(ev);
    if (code.nbytes == 0 || (code.nbytes == 1 && code.bytes[0] == 0xF0)) {
        // unknown key
        std::cerr << "Key not mapped: " << std::hex << ev->key() << std::endl;
    } else {
        for (int i = 0; i < code.nbytes; ++i) {
            system->sio.receive(Sio::Channel_A, code.bytes[i]);
        }
    }
}
