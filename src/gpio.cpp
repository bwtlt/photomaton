#include "gpio.h"

#include <wiringPi.h>
#include <QtCore/QDebug>

#include <sys/time.h>

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
        qDebug() << "Error in GPIO init";
        return false;
    }

    // wirintPiISR
    wiringPiISR(BUTTON0, INT_EDGE_FALLING, &GPIO::okInterrupt);
    wiringPiISR(BUTTON1, INT_EDGE_FALLING, &GPIO::cancelInterrupt);
    wiringPiISR(BUTTON2, INT_EDGE_FALLING, &GPIO::rightInterrupt);
    wiringPiISR(BUTTON3, INT_EDGE_FALLING, &GPIO::leftInterrupt);

    struct timeval tv;

    gettimeofday(&tv, NULL);
    epochMilli = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);
    last_interrupt_time = epochMilli;

    return true;
}

void GPIO::okInterrupt()
{
    unsigned int interrupt_time = millis();
    qDebug() << __func__ << interrupt_time;
    if (interrupt_time - last_interrupt_time > 200)
        emit GPIO::Instance().okBtnPressed();
    else
	qDebug() << "ignored interrupt";
    last_interrupt_time = interrupt_time;
}

void GPIO::cancelInterrupt()
{
    unsigned int interrupt_time = millis();
    qDebug() << __func__ << interrupt_time;
    if (interrupt_time - last_interrupt_time > 200)
	emit GPIO::Instance().cancelBtnPressed();
    else
	qDebug() << "ignored interrupt";
    last_interrupt_time = interrupt_time;
}

void GPIO::leftInterrupt()
{
    unsigned int interrupt_time = millis();
    qDebug() << __func__ << interrupt_time;
    if (interrupt_time - last_interrupt_time > 200)
	emit GPIO::Instance().leftBtnPressed();
    else
	qDebug() << "ignored interrupt";
    last_interrupt_time = interrupt_time;
}

void GPIO::rightInterrupt()
{
    unsigned int interrupt_time = millis();
    qDebug() << __func__ << interrupt_time;
    if (interrupt_time - last_interrupt_time > 200)
        emit GPIO::Instance().rightBtnPressed();
    else
	qDebug() << "ignored interrupt";
    last_interrupt_time = interrupt_time;
}

unsigned int GPIO::millis(void)
{
    struct timeval tv;
    uint64_t now;

    gettimeofday(&tv, NULL);
    now = (uint64_t)tv.tv_sec * (uint64_t)1000 + (uint64_t)(tv.tv_usec / 1000);

    return (uint32_t)(now - epochMilli);
}
