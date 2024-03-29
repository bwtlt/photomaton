#include "gpio.h"

#include "utils.h"

#include <wiringPi.h>
#include <QtCore/QDebug>

unsigned int last_interrupt_time = 0;
static uint64_t epochMilli;

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
        qDebug() << Q_FUNC_INFO << "Error in GPIO init";
        return false;
    }

    // wirintPiISR
    wiringPiISR(BUTTON0, INT_EDGE_FALLING, &GPIO::okInterrupt);
    wiringPiISR(BUTTON1, INT_EDGE_FALLING, &GPIO::cancelInterrupt);
    wiringPiISR(BUTTON2, INT_EDGE_FALLING, &GPIO::rightInterrupt);
    wiringPiISR(BUTTON3, INT_EDGE_FALLING, &GPIO::leftInterrupt);

    epochMilli = getTimeInMillis();
    last_interrupt_time = epochMilli;

    return true;
}

void GPIO::okInterrupt()
{
    unsigned int interrupt_time = getTimeInMillis();
    qDebug() << Q_FUNC_INFO << "at" << interrupt_time << "ms";
    if (interrupt_time - last_interrupt_time > 200)
        emit GPIO::Instance().okBtnPressed();
    else
	    qDebug() << Q_FUNC_INFO << "ignored interrupt";
    last_interrupt_time = interrupt_time;
}

void GPIO::cancelInterrupt()
{
    unsigned int interrupt_time = getTimeInMillis();
    qDebug() << Q_FUNC_INFO << "at" << interrupt_time << "ms";
    if (interrupt_time - last_interrupt_time > 200)
	    emit GPIO::Instance().cancelBtnPressed();
    else
	    qDebug() << Q_FUNC_INFO << "ignored interrupt";
    last_interrupt_time = interrupt_time;
}

void GPIO::leftInterrupt()
{
    unsigned int interrupt_time = getTimeInMillis();
    qDebug() << Q_FUNC_INFO << "at" << interrupt_time << "ms";
    if (interrupt_time - last_interrupt_time > 200)
	    emit GPIO::Instance().leftBtnPressed();
    else
	    qDebug() << Q_FUNC_INFO << "ignored interrupt";
    last_interrupt_time = interrupt_time;
}

void GPIO::rightInterrupt()
{
    unsigned int interrupt_time = getTimeInMillis();
    qDebug() << Q_FUNC_INFO << "at" << interrupt_time << "ms";
    if (interrupt_time - last_interrupt_time > 200)
        emit GPIO::Instance().rightBtnPressed();
    else
	    qDebug() << Q_FUNC_INFO << "ignored interrupt";
    last_interrupt_time = interrupt_time;
}

