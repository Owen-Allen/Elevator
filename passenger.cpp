#include "passenger.h"



Passenger::Passenger(int curr, int dest):QObject(NULL)
{
    this->current_floor = curr;
    this->destination_floor = dest;

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


void Passenger::send_request(){
    qInfo("In send_request");
    emit request(this);
}

