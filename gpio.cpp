#include "gpio.h"
#include <wiringPi.h>
#include <QDebug>

GPIO::GPIO()
{
}

GPIO& GPIO::Instance()
{
    static GPIO instance;
    return instance;
}

bool GPIO::init()
{
    if (0 != wiringPiSetupSys())
    {
        qDebug() << "Error in GPIO init";
        return false;
    }

    // wirintPiISR
    wiringPiISR(BUTTON0, INT_EDGE_FALLING, &GPIO::okInterrupt);
    wiringPiISR(BUTTON1, INT_EDGE_FALLING, &GPIO::cancelInterrupt);
    wiringPiISR(BUTTON2, INT_EDGE_FALLING, &GPIO::rightInterrupt);
    wiringPiISR(BUTTON3, INT_EDGE_FALLING, &GPIO::leftInterrupt);

    return true;
}

void GPIO::okInterrupt()
{
    emit GPIO::Instance().okBtnPressed();
}

void GPIO::cancelInterrupt()
{
    emit GPIO::Instance().cancelBtnPressed();
}

void GPIO::leftInterrupt()
{
    emit GPIO::Instance().leftBtnPressed();
}

void GPIO::rightInterrupt()
{
    emit GPIO::Instance().rightBtnPressed();
}
