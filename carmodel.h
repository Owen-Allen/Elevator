#ifndef CARMODEL_H
#define CARMODEL_H

#include "passenger.h"
#include <iostream>

using namespace std;
#include <list>
#include <unordered_set>


class CarModel: public QObject
{
    Q_OBJECT
public:
    CarModel(int curr_floor, int dir);

    int current_floor;
    int direction; //-1 for down, 1 for up, 0 for not moving
    //bool isMoving;

    std::list<Passenger*> passengers;
    std::list<Passenger*> upcoming_passengers;
    
    std::unordered_set<int> destinations;

    // Add the passenger to upcoming_passengers when they
    // request an elevator
    void add_to_upcoming_passengers(Passenger*);
    
    // Add the passenger to passengers when we pick them up
    void add_to_passengers(Passenger*);
    
    // When on destination floor, see which passengers should get off
    void pickup_passengers();
    void dropoff_passengers();
    
    
    void move();



private slots:
    void add_to_destinations(int); // Called when Passenger requests pickup AND when they get on

};



#endif // CARMODEL_H
