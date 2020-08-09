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
    this->setWindowTitle("Photomaton");

    ui->preview->setVisible(false);
    ui->effect1->setVisible(false);
    ui->effect2->setVisible(false);
    ui->effect3->setVisible(false);
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

    m_currentState = STATE_PREVIEW;

}

void MainWindow::handleImage(QImage &image)
{
    if (STATE_PREVIEW == m_currentState)
    {
        ui->image->setPixmap(QPixmap::fromImage(image));

        m_rawImage.setQImage(image);

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

    ui->image->setPixmap(QPixmap::fromImage(m_rawImage.getQImage()));

    ui->helpText->setText("Choisissez un effet :");
    ui->effect1->setVisible(true);
    ui->effect1->setPixmap(QPixmap::fromImage(QImage("/home/pi/photomaton2/misc/sepia_effect.png")));
    ui->effect2->setVisible(true);
    ui->effect2->setPixmap(QPixmap::fromImage(m_rawImage.getQImage()));
    ui->effect3->setVisible(true);
    ui->effect3->setPixmap(QPixmap::fromImage(QImage("/home/pi/photomaton2/misc/bw_effect.png")));

    this->repaint();
}

void MainWindow::resumePreview()
{
    m_cameraWorker->resume();
    m_cameraRunning = true;
    m_currentState = STATE_PREVIEW;

    ui->helpText->setText("Appuyez sur le bouton pour prendre une photo");
    ui->preview->setVisible(false);
    ui->effect1->setVisible(false);
    ui->effect2->setVisible(false);
    ui->effect3->setVisible(false);

    QApplication::processEvents();
}

void MainWindow::startSlideShow()
{
    m_currentState = STATE_SLIDESHOW;
    ui->preview->setVisible(true);
    ui->helpText->setText("Appuyez sur un bouton pour quitter le mode diaporama");
    ui->effect1->setVisible(false);
    ui->effect2->setVisible(false);
    ui->effect3->setVisible(false);
}

void MainWindow::applyFilter(eFilter filter)
{
    switch(filter)
    {
    case BLACK_AND_WHITE:
	    toGray(m_rawImage, m_image);
	    break;
    case SEPIA:
	    toSepia(m_rawImage, m_image);
	    break;
    default:
	    break;
    }
    ui->image->setPixmap(QPixmap::fromImage(m_image.getQImage()));

    this->repaint();
}

void MainWindow::okBtnPressed()
{
    switch(m_currentState)
    {
    case STATE_OFF:
        break;
    case STATE_SLIDESHOW:
	resumePreview();
        break;
    case STATE_PREVIEW:
        captureImage();
        break;
    case STATE_CAPTURESETTINGS:
	// todo validation
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
    case STATE_SLIDESHOW:
	resumePreview();
        break;
    case STATE_PREVIEW:
	startSlideShow();
        break;
    case STATE_CAPTURESETTINGS:
        resumePreview();
        break;
    case STATE_FINAL:
	resumePreview();
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
    case STATE_SLIDESHOW:
	resumePreview();
        break;
    case STATE_PREVIEW:
        break;
    case STATE_CAPTURESETTINGS:
	applyFilter(BLACK_AND_WHITE);
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
    case STATE_SLIDESHOW:
	resumePreview();
        break;
    case STATE_PREVIEW:
        break;
    case STATE_CAPTURESETTINGS:
	applyFilter(SEPIA);
        break;
    case STATE_FINAL:
        break;
    default:
        assert(false);
        break;
    }
}
