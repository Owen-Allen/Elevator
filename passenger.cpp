#include "passenger.h"



Passenger::Passenger(int curr, int dest, QString name):QObject(NULL)
{
    this->current_floor = curr;
    this->destination_floor = dest;
    this->name = name;

    if(curr < dest){
        direction = 1;
    }else if(curr > dest){
        direction = -1;
    }else{
        // This should never happen
        direction = 0;
    }
}


Passenger::~Passenger()
{

}

QString Passenger::toString(){
   return QString("%1 cur: %2 dest: %3 ").arg(this->name).arg(this->current_floor).arg(this->destination_floor);
}

void Passenger::send_elevator_request(){
    qInfo("sending_elevator_request");
    emit request_elevator(this);
}


void Passenger::update_current_floor(int new_floor){    
    this->current_floor = new_floor;
    qInfo("passengers current floor is %d", this->current_floor);
}
