#include "carmodel.h"
/*
CarModel::CarModel()
{
    this->current_floor = 1;
    this->direction = 1;
    this->isMoving = false;
}
*/

CarModel::CarModel(int curr_floor, int dir):QObject(NULL)
{
    this->current_floor = curr_floor;
    this->direction = dir;
   // this->isMoving = moving;
}


QString CarModel::toString(){
    QString ret_string;

    ret_string = QString("Current floor: %1, Direction: %2 \n").arg(this->current_floor).arg(this->direction);

    ret_string.append("Elevator's upcoming passengers:\n");
    std::list<Passenger*>:: iterator it_u = upcoming_passengers.begin();


    while(it_u != upcoming_passengers.end()){
        ret_string.append("  ").append((*it_u)->toString()).append("\n");
        ++it_u;
    }

    ret_string.append("Current passengers: \n");

    std::list<Passenger*>:: iterator it_p = passengers.begin();

    while(it_p != passengers.end()){
        ret_string.append("  ").append((*it_p)->toString()).append("\n");
        ++it_p;
    }

    ret_string.append("destinations: ");
    for(auto i = destinations.begin(); i != destinations.end(); ++i){
        QString ret_d;
        ret_d = QString("%1 ").arg(*i);
        ret_string.append(ret_d);
    }

    return ret_string;
}


void CarModel::add_to_upcoming_passengers(Passenger* upcoming_passenger){
    // add pickup to floor requests stack

    if(upcoming_passenger->current_floor == this->current_floor){
        // we can skip adding them to upcoming passenger and we they can get in the elevator now
        add_to_passengers(upcoming_passenger);
    }else{
        // we need to travel to pickup the passenger
        upcoming_passengers.push_front(upcoming_passenger);
        destinations.insert(upcoming_passenger->current_floor);
        if(this->direction == 0){
            qInfo("Calling calculate direction");
            calculate_direction();
        }
    }
}

void CarModel::add_to_passengers(Passenger* new_passenger){
    passengers.push_front(new_passenger);
    destinations.insert(new_passenger->destination_floor);

    connect(this, SIGNAL(send_location_update(int)), new_passenger, SLOT(update_current_floor(int)));
}

void CarModel::pickup_passengers(){
    std::list<Passenger*>:: iterator it = upcoming_passengers.begin();
    
    while(it != upcoming_passengers.end()){
        qInfo("Picking up");
        if((*it)->current_floor == current_floor){
            add_to_passengers((*it));
            it = upcoming_passengers.erase(it);
        }else{
            ++it;
        }
    }
}

void CarModel::dropoff_passengers(){
    std::list<Passenger*>:: iterator it = passengers.begin();

    while (it != passengers.end()){
        if((*it)->destination_floor == current_floor){
            disconnect(this, SIGNAL(send_location_update(int)), *it, SLOT(update_current_floor(int)));
            it = passengers.erase(it);
        }else{
            ++it;
        }
    }
}

void CarModel::calculate_direction(){

    // TWO SCENARIOS WHERE YOU SHOULD CHANGE THE DIRECTION

    // 1: Currently not moving && have a destination to go to
    // 2: Currently ARE moving && have no destinations

    if (this->direction == 0 && destinations.size()){ // Scenario 1

        // Need to go up


        if(*destinations.begin() > current_floor){
            qInfo("Going up");
            this->direction = 1;
            qDebug() << "THIS IS THE DIRECTION " << this->direction;

        // Need to go down
        }else if(*destinations.begin() < current_floor){
            this->direction = -1;
        }

    }else if(this->direction != 0 && destinations.size() == 0){ // Scenario 2
        this->direction = 0;
    }
}



void CarModel::move(){

    // CHECK ALL THE HAZARDS

    // Update the current floor
    current_floor += direction;

    emit send_location_update(current_floor);

    // Is the current floor in our list of destinations?
    for(auto it = destinations.begin(); it != destinations.end(); ){
        if((*it) == this->current_floor){
            qDebug() << " PICKING UP / DROPPING OFF AT " << current_floor;
            dropoff_passengers();
            pickup_passengers();
            destinations.erase(it);
            break;
        }else{
            ++it;
        }
    }

    // See if we need to change direction since everyone may have gotten off
    calculate_direction();

}
