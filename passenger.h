#ifndef PASSENGER_H
#define PASSENGER_H

#include <QObject>

class Passenger: public QObject
{
     Q_OBJECT
public:
    //passenger();
    Passenger(int curr, int dest);
    int current_floor;
    int destination_floor;
    int direction;

    ~Passenger();

    void send_request();
    
/*
private slots:
    void board_car();
*/

signals:
    void request(Passenger*);


};

#endif // PASSENGER_H
