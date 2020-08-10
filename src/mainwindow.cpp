#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "imgprocessing.h"

#include <QtGui/QPixmap>
#include <QtCore/QDebug>
#include <QtGui/QFontDatabase>

#include <unistd.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_cameraRunning(false),
    m_slideShowRunning(false),
    m_currentState(STATE_OFF)
{
    ui->setupUi(this);
    this->setWindowTitle("Photomaton");

    ui->preview->setVisible(false);
    ui->effect1->setVisible(false);
    ui->effect2->setVisible(false);
    ui->effect3->setVisible(false);

    int id = QFontDatabase::addApplicationFont("/home/pi/photomaton2/misc/CaviarDreams.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    font = QFont(family, 32);

    ui->helpText->setFont(font);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startPhotomaton()
{
    qDebug() << Q_FUNC_INFO;
    m_cameraWorkerThread = new QThread;
    m_slideShowWorkerThread = new QThread;
    m_cameraWorker = new VideoStream;
    m_slideShowWorker = new SlideShow;

    if (true != m_cameraWorker->openCamera())
    {
        assert(false);
    }
    m_slideShowWorker->init();

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
    connect(m_cameraWorker, SIGNAL(handleImage(QImage &)), this, SLOT(handleCameraImage(QImage &)));

    m_slideShowWorker->moveToThread(m_slideShowWorkerThread);

    connect(m_slideShowWorkerThread, SIGNAL(started()), m_slideShowWorker, SLOT(grabImages()));
    connect(m_slideShowWorker, SIGNAL(finished()), m_slideShowWorkerThread, SLOT(quit()));
    connect(m_slideShowWorker, SIGNAL(finished()), m_slideShowWorker, SLOT(deleteLater()));
    connect(m_slideShowWorkerThread, SIGNAL(finished()), m_slideShowWorkerThread, SLOT(deleteLater()));
    connect(m_slideShowWorker, SIGNAL(handleImage(QImage &)), this, SLOT(handleSSImage(QImage &)));

    connect(&GPIO::Instance(), SIGNAL(okBtnPressed()), this, SLOT(okBtnPressed()));
    connect(&GPIO::Instance(), SIGNAL(cancelBtnPressed()), this, SLOT(cancelBtnPressed()));
    connect(&GPIO::Instance(), SIGNAL(leftBtnPressed()), this, SLOT(leftBtnPressed()));
    connect(&GPIO::Instance(), SIGNAL(rightBtnPressed()), this, SLOT(rightBtnPressed()));

    m_cameraWorkerThread->start();
    m_slideShowWorkerThread->start();

    m_cameraRunning = true;

    GPIO::Instance().init();

    ui->helpText->setText("Appuyez sur le bouton pour prendre une photo");

    m_currentState = STATE_PREVIEW;

}

void MainWindow::handleCameraImage(QImage &image)
{
    if (true == m_cameraRunning)
    {
        if (STATE_PREVIEW == m_currentState)
        {
            ui->image->setPixmap(QPixmap::fromImage(image));
        }
        else if (STATE_SLIDESHOW == m_currentState)
        {
            ui->preview->setPixmap(QPixmap::fromImage(image));
        }
        m_rawImage.setQImage(image);
        QApplication::processEvents();
        this->repaint();
    }
}

void MainWindow::handleSSImage(QImage &image)
{
    if (true == m_slideShowRunning)
    {
        ui->image->setPixmap(QPixmap::fromImage(image));
        m_rawImage.setQImage(image);
        QApplication::processEvents();
        this->repaint();
    }
}

void MainWindow::cameraFinished()
{
    qDebug() << Q_FUNC_INFO;
    m_cameraRunning = false;
    m_currentState = STATE_OFF;

    //disable buttons

    ui->image->setText("Appareil photo désactivé");
}

void MainWindow::captureImage()
{
    qDebug() << Q_FUNC_INFO;
    m_cameraWorker->pause();
    QApplication::processEvents();
    m_cameraRunning = false;
    m_currentState = STATE_CAPTURESETTINGS;

    ui->image->setPixmap(QPixmap::fromImage(m_rawImage.getQImage()));
    m_image = m_rawImage;

    ui->helpText->setText("Choisissez un effet et validez");

    ui->effect1->setPixmap(QPixmap::fromImage(QImage("/home/pi/photomaton2/misc/sepia_effect.png")));
    ui->effect2->setPixmap(QPixmap::fromImage(m_rawImage.getQImage()));
    ui->effect2->setScaledContents(true);
    ui->effect3->setPixmap(QPixmap::fromImage(QImage("/home/pi/photomaton2/misc/bw_effect.png")));

    ui->effect1->setVisible(true);
    ui->effect2->setVisible(true);
    ui->effect3->setVisible(true);

    this->repaint();
}

void MainWindow::resumePreview()
{
    qDebug() << Q_FUNC_INFO;
    m_cameraWorker->resume();
    m_cameraRunning = true;
    m_slideShowWorker->resume();
    m_slideShowRunning = false;
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
    qDebug() << Q_FUNC_INFO;
    m_cameraWorker->resume();
    m_cameraRunning = true;

    m_slideShowWorker->resume();
    m_slideShowRunning = true;

    m_currentState = STATE_SLIDESHOW;
    ui->preview->setVisible(true);
    ui->helpText->setText("Appuyez sur un bouton\npour quitter le mode diaporama");
    ui->effect1->setVisible(false);
    ui->effect2->setVisible(false);
    ui->effect3->setVisible(false);
}

void MainWindow::applyFilter(eFilter filter)
{
    qDebug() << Q_FUNC_INFO << "Filter:" << QString(filter);
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

void MainWindow::saveImage()
{
    qDebug() << Q_FUNC_INFO;
    m_image.save();

    resumePreview();
}

void MainWindow::okBtnPressed()
{
    qDebug() << Q_FUNC_INFO << "in state" << QString(m_currentState);
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
        saveImage();
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
    qDebug() << Q_FUNC_INFO << "in state" << QString(m_currentState);
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
    qDebug() << Q_FUNC_INFO << "in state" << QString(m_currentState);
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
    qDebug() << Q_FUNC_INFO << "in state" << QString(m_currentState);
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
