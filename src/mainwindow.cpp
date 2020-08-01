#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPixmap>
#include <unistd.h>
#include <QDebug>

#include "imgprocessing.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_cameraRunning(false),
    m_currentState(STATE_OFF)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startPreview()
{
    m_cameraWorkerThread = new QThread;
    m_cameraWorker = new VideoStream;

    m_cameraWorker->openCamera();

    if (m_cameraRunning)
    {
        return;
    }

    m_cameraWorker->moveToThread(m_cameraWorkerThread);

    connect(m_cameraWorkerThread, SIGNAL(started()), m_cameraWorker, SLOT(grabImages()));
    connect(m_cameraWorker, SIGNAL(finished()), m_cameraWorkerThread, SLOT(quit()));
    connect(m_cameraWorker, SIGNAL(finished()), m_cameraWorker, SLOT(deleteLater()));
    connect(m_cameraWorkerThread, SIGNAL(finished()), m_cameraWorkerThread, SLOT(deleteLater()));
    connect(m_cameraWorker, SIGNAL(finished()), this, SLOT(cameraFinished()));
    connect(m_cameraWorker, SIGNAL(handleImage(QImage &)), this, SLOT(handleImage(QImage &)));

    connect(&GPIO::Instance(), SIGNAL(okBtnPressed()), this, SLOT(okBtnPressed()));
    connect(&GPIO::Instance(), SIGNAL(cancelBtnPressed()), this, SLOT(cancelBtnPressed()));
    connect(&GPIO::Instance(), SIGNAL(leftBtnPressed()), this, SLOT(leftBtnPressed()));
    connect(&GPIO::Instance(), SIGNAL(rightBtnPressed()), this, SLOT(rightBtnPressed()));

    m_cameraWorkerThread->start();

    m_cameraRunning = true;

    GPIO::Instance().init();

    ui->helpText->setText("Appuyez sur le bouton pour prendre une photo");
    ui->choicesGroupBox->setVisible(false);

    m_currentState = STATE_PREVIEW;

}

void MainWindow::handleImage(QImage &image)
{
    if (STATE_PREVIEW == m_currentState)
    {
        ui->image->setPixmap(QPixmap::fromImage(image));

        m_image.setQImage(image);

        QApplication::processEvents();
        this->repaint();
    }
}

void MainWindow::cameraFinished()
{
    m_cameraRunning = false;
    m_currentState = STATE_OFF;

    //disable buttons

    ui->image->setText("Appareil photo désactivé");
}

void MainWindow::captureImage()
{
    m_cameraWorker->pause();
    QApplication::processEvents();
    m_cameraRunning = false;
    m_currentState = STATE_CAPTURESETTINGS;

    toGray(m_image);
    ui->image->setPixmap(QPixmap::fromImage(m_image.getQImage()));

    this->repaint();
}

void MainWindow::resumePreview()
{
    m_cameraWorker->resume();
    m_cameraRunning = true;
    m_currentState = STATE_PREVIEW;
    QApplication::processEvents();
}

void MainWindow::okBtnPressed()
{
    switch(m_currentState)
    {
    case STATE_OFF:
        break;
    case STATE_PREVIEW:
        captureImage();
        break;
    case STATE_CAPTURESETTINGS:
        break;
    case STATE_FINAL:
        break;
    default:
        assert(false);
        break;
    }
}

void MainWindow::cancelBtnPressed()
{
    switch(m_currentState)
    {
    case STATE_OFF:
        break;
    case STATE_PREVIEW:
        break;
    case STATE_CAPTURESETTINGS:
        resumePreview();
        break;
    case STATE_FINAL:
        break;
    default:
        assert(false);
        break;
    }
}

void MainWindow::leftBtnPressed()
{
    switch(m_currentState)
    {
    case STATE_OFF:
        break;
    case STATE_PREVIEW:
        break;
    case STATE_CAPTURESETTINGS:
	//todo apply previous filter
        break;
    case STATE_FINAL:
        break;
    default:
        assert(false);
        break;
    }
}

void MainWindow::rightBtnPressed()
{
    switch(m_currentState)
    {
    case STATE_OFF:
        break;
    case STATE_PREVIEW:
        break;
    case STATE_CAPTURESETTINGS:
	//todo apply next filter
        break;
    case STATE_FINAL:
        break;
    default:
        assert(false);
        break;
    }
}
