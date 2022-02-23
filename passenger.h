#ifndef PASSENGER_H
#define PASSENGER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <iostream>

#include <string>

class Passenger: public QObject
{
     Q_OBJECT
public:
    //passenger();
    Passenger(int curr, int dest, QString name);
    int current_floor;
    int destination_floor;
    int direction;
    QString name;

    ~Passenger();

    // Requests the main window to be added to the pickup_requests
    void send_elevator_request();

    QString toString();



private slots:
    void update_current_floor(int); // Slot for CarModel signal


signals:
    //void request(Passenger*);
    void request_elevator(Passenger*); // Request from passenger to elevator
};

#endif // PASSENGER_H
