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


void CarModel::add_to_upcoming_passengers(Passenger* upcoming_passenger){
    // add pickup to floor requests stack
    upcoming_passengers.push(upcoming_passenger);

    destinations.insert(upcoming_passenger->current_floor);
}

void CarModel::add_to_passengers(Passenger* new_passenger){
    passengers.push(upcoming_passenger);

    destinations.insert(new_passenger->destination_floor);
}

void CarModel::pickup_passengers(){
    std::list<Passenger*>:: iterator it = upcoming_passengers.begin();
    
    while(it != upcoming_passengers.end()){
        if(it->current_floor == current_floor){
            add_to_passengers(it);
            upcoming_passengers.erase(it++);
        }
    }
}

void CarModel::dropoff_passengers(){
    std::list<Passenger*>:: iterator it = passengers.begin();

    while (it != passengers.end()){
        if(it->destination == current_floor){
            passengers.erase(i++);
        }
    }
}


void CarModel::move(){

    // CHECK ALL THE HAZARDS

    // Is the current floor in our list of destinations?
    if(destinations.find(current_floor) != destinations.end()){
        pickup_passengers();
        dropoff_passengers();
    }


    // Do we need to change direction?

    if (direction == 1){ // Currently Going Up
        bool continue_going_up = false;
        for (int i = 0; i < destinations.size(); i++){
            if(destinations[i] > current_floor){
                // We can keep going in this direction
                continue_going_up = true;
                break;
            }
        }




    // Do we need to change direction?








    // remove current floor from destinations



    // change the current floor based on direction



}
