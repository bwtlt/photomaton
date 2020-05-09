#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_cameraRunning(false)

{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startPreview()
{
    m_workerThread = new QThread;
    m_worker = new VideoStream;

    m_worker->openCamera();

    if (m_cameraRunning)
    {
        return;
    }

    m_worker->moveToThread(m_workerThread);

    connect(m_workerThread, SIGNAL(started()), m_worker, SLOT(grabImages()));
    connect(m_worker, SIGNAL(finished()), m_workerThread, SLOT(quit()));
    connect(m_worker, SIGNAL(finished()), m_worker, SLOT(deleteLater()));
    connect(m_workerThread, SIGNAL(finished()), m_workerThread, SLOT(deleteLater()));
    connect(m_worker, SIGNAL(finished()), this, SLOT(cameraFinished()));
    connect(m_worker, SIGNAL(handleImage(QImage &)), this, SLOT(handleImage(QImage &)));
    // todo connect buttons

    m_workerThread->start();

    m_cameraRunning = true;

    // enable/disable buttons

}

void MainWindow::handleImage(QImage &image)
{
    ui->image->setPixmap(QPixmap::fromImage(image));

    QApplication::processEvents();
    this->repaint();
}

void MainWindow::cameraFinished()
{
    m_cameraRunning = false;

    //disable buttons

    ui->image->setText("Camera Disabled");
}
