#include "timer.h"

Timer::Timer()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));

    timer->start(5000);
}


void Timer::timer_slot(){
    qDebug() << "Timer executed";
}
