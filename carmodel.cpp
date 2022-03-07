#include "carmodel.h"


CarModel::CarModel(int curr_floor, int dir):QObject(NULL)
{
    this->current_floor = curr_floor;
    this->direction = dir;

    isOverload = false;
    isFire = false;
    isPowerOut = false;
    isDoorBlocked = false;
    needHelp = false;

    blockedCounter = 0;
    helpCounter = 0;

    safeFloor = 1;
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
        }
    }
     calculate_direction();
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

    // 1: have a destination to go to
    // 2: Currently ARE moving && have no destinations

    if (destinations.size()){
        // Need to go up
        if(*destinations.begin() > current_floor){
            this->direction = 1;
        // Need to go down
        }else if(*destinations.begin() < current_floor){
            this->direction = -1;
        }
    }else if(this->direction != 0 && destinations.size() == 0){ // Scenario 2
        this->direction = 0;
    }
}


void CarModel::emergency_destination(){
    // CLEAR ALL DESTINATIONS
    destinations.clear();

    // add the safe floor to destinations
    destinations.insert(safeFloor);

    std::list<Passenger*>:: iterator it = passengers.begin();
    while (it != passengers.end()){
        connect(this, SIGNAL(change_destination(int)), *it, SLOT(change_destination(int)));
        ++it;
    }
    emit change_destination(safeFloor);

    calculate_direction();

    //current_floor += direction;

    /*
    emit send_location_update(current_floor);

    if (current_floor == safeFloor){
        dropoff_passengers();
    }
    */
}



void CarModel::move(){

    // CHECK ALL THE HAZARDS

    //if(isOverload){

    //}else if(isFire){

    //}else if(isPowerOut){

    if(needHelp){
            helpCounter += 1;
            qDebug() << "Someone needs help " << helpCounter;

            if(helpCounter > 3){
                qDebug() << "DID NOT RECIEVE RESPONSE FROM BUILDING SAFETY, CALLING 911";
            }
    }

    if(isDoorBlocked){
        blockedCounter += 1;
        qDebug() << "Elevator did not move, door is blocked";
        if(blockedCounter > 3){
            qDebug() << "WARNING: PLEASE UNBLOCK DOOR";
        }
    }else if(isOverload){
        qDebug() << "Elevator is overloaded, please exit the elevator";
    }else{

        // If we reached this case, then we can reset the blockedCounter
        current_floor += direction;

        emit send_location_update(current_floor);

        // Is the current floor in our list of destinations?
        for(auto it = destinations.begin(); it != destinations.end(); ){
            if((*it) == this->current_floor){
                qDebug() << " PICKING UP / DROPPING OFF AT " << current_floor;
                dropoff_passengers();
                if(!isOverload && !isFire && !isPowerOut && !isDoorBlocked )
                    pickup_passengers();
                destinations.erase(it);
                break;
            }else{
                ++it;
            }
        }
    }
    // See if we need to change direction since everyone may have gotten off
    calculate_direction();
}

void CarModel::receive_overload(){
    // Flip the value, so that if we recieve the signal again, we've simulated it becoming
    // normal again
    isOverload = !isOverload;
}

void CarModel::receive_fire(){
    isFire = true;
    emergency_destination();
}

void CarModel::receive_power(){
    isPowerOut = true;
    emergency_destination();
}

void CarModel::receive_door(){
    // Flip the value, so that if we recieve the signal again, we've simulated it becoming
    // normal again
    if(isDoorBlocked == false){
        isDoorBlocked = true;
    }else{
        isDoorBlocked = false;
        blockedCounter = 0;
    }
}

void CarModel::receive_help(){
    if(needHelp == false){
        needHelp = true;
    }else{
        // reset
        needHelp = false;
        helpCounter = 0;
    }
}


