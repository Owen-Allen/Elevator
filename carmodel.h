#ifndef CARMODEL_H
#define CARMODEL_H

#include "passenger.h"
#include <iostream>

using namespace std;
#include <list>
#include <unordered_set>

#include <QDebug>
#include <QString>


class CarModel: public QObject
{
    Q_OBJECT
public:
    CarModel(int curr_floor, int dir);

    int current_floor;
    int direction; //-1 for down, 1 for up, 0 for not moving

    std::list<Passenger*> passengers;
    std::list<Passenger*> upcoming_passengers;
    
    std::unordered_set<int> destinations;

    // Add the passenger to upcoming_passengers when they
    // request an elevator
    //void add_to_upcoming_passengers(Passenger*);
    
    // Add the passenger to passengers when we pick them up
    void add_to_passengers(Passenger*);
    
    // When on destination floor, see which passengers should get off
    void pickup_passengers();
    void dropoff_passengers();
    
    void calculate_direction();
    void move();

    QString toString();

private:
    bool isOverload;
    bool isFire;
    bool isPowerOut;
    bool isDoorBlocked;
    bool needHelp;
    int safeFloor;

    void emergency_destination();

signals:
    //void update_passenger_floor(int);
    void send_location_update(int);
    void change_destination(int);

private slots:
    //void add_to_passengers(Passenger*);
    void add_to_upcoming_passengers(Passenger*);

    void receive_overload();
    void receive_fire();
    void receive_power();
    void receive_door();
    void receive_help();
};



#endif // CARMODEL_H
