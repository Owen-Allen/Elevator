#include "mainwindow.h"
#include "ui_mainwindow.h"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //connect(sender, signal (what they send), reciever, slot (how they recieve it))
     connect(ui->pushButton, SIGNAL(released()), this, SLOT(overload_sensor()));
     connect(ui->pushButton_2, SIGNAL(released()), this, SLOT(fire_sensor()));
     connect(ui->pushButton_3, SIGNAL(released()), this, SLOT(power_sensor()));
     connect(ui->pushButton_4, SIGNAL(released()), this, SLOT(door_sensor()));
     connect(ui->pushButton_5, SIGNAL(released()), this, SLOT(help()));
     connect(ui->pushButton_6, SIGNAL(released()), this, SLOT(normal()));


    // Initialize Passengers
    QString names[5] = {"Shrek", "Donkey", "Fiona", "Puss in Boots", "Lord Farquad"};
    int start[5] =  {1, 4, 5, 4, 5};
    int end[5] =    {5, 1, 1, 7, 2};

    // Connect the passengers to the Main Window so they can call an elevator
    for(int i = 0; i < NUM_PASSENGERS; i++){
        people[i] = new Passenger(start[i], end[i], names[i]);
        pickup_requests.push_back(people[i]);
    }



    // Initialize Cars
    int curr_floor[2] =  {1, 4};
    int dir[2] =    {0, 0};


    for (int i = 0; i < NUM_CARS; i++){
        cars[i] = new CarModel(curr_floor[i], dir[i]);
        connect(this, SIGNAL(send_overload()), cars[i], SLOT(receive_overload()));
        connect(this, SIGNAL(send_fire()), cars[i], SLOT(receive_fire()));
        connect(this, SIGNAL(send_powerout()), cars[i], SLOT(receive_power()));
        connect(this, SIGNAL(send_doorblocked()), cars[i], SLOT(receive_door()));
        connect(this, SIGNAL(send_help()), cars[i], SLOT(receive_help()));
    }

    QString display_string = QString();
    for(int e = 0; e < NUM_CARS; e++){
        cars[e]->move();
        QString car_num = QString("Elevator %1 \n").arg(e);
        display_string.append(car_num);
        display_string.append(cars[e]->toString()).append("\n");
    }
    ui->label->setText(display_string);

    control_clock = new QTimer(this);
    connect(control_clock, SIGNAL(timeout()), this, SLOT(normal()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::overload_sensor(){ //
    qInfo("Sending overload simulation");
    this->send_overload();}

void MainWindow::fire_sensor(){
    qInfo("Sending fire signal simulation");
    this->send_fire();
}

void MainWindow::power_sensor(){
    qInfo("Sending power simulation");
    this->send_powerout();
}

void MainWindow::door_sensor(){
    if(cars[0] -> isDoorBlocked == false){
        qDebug() << "BLOCKING DOORS";
        this->send_doorblocked();
    }else{
        qDebug() << "UNBLOCKING DOORS";
        this->send_doorblocked();
    }
}

void MainWindow::help(){
    if(cars[0] -> needHelp == false){
        qDebug() << "sending need help signal";
        this->send_help();
    }else{
        qDebug() << "setting needHelp back to false";
        this->send_doorblocked();
    }
}



void MainWindow::normal(){
    std::list<Passenger*>:: iterator it = pickup_requests.begin();
    while(it != pickup_requests.end()){
        if(determine_elevator(*it)){
            qDebug() << "added " << (*it) -> name << " to an upcoming_pickup";
                // the Passenger has been moved to an upcoming_request for an elevator,
                // so we can remove them from pickup_requests
            it = pickup_requests.erase(it);
        }else{
            qDebug() << "could not add " << (*it)->name << " to an upcoming_pickup, we will try again later";
            ++it;
        }
    }

    QString display_string = QString();
    for(int e = 0; e < NUM_CARS; e++){
        cars[e]->move();
        QString car_num = QString("Elevator %1 \n").arg(e);
        display_string.append(car_num);
        display_string.append(cars[e]->toString()).append("\n");
    }
    ui->label->setText(display_string);
    //control_clock->start(5000);
}


void MainWindow::create_pickup_request(){
    // Create a random new request
    // MAKE SURE TO PUSH_BACK of pickup_requests
}

bool MainWindow::determine_elevator(Passenger* p){
    // Try to add this passenger to an elevators upcoming_requests
    // if they are added, return true, else return false

    // PASSENGER CAN BE ADDED TO AN ELEVATORS upcoming_passengers UNDER 2 CONDITIONS
    // 1: the car is not currently moving
    // 2: the car is heading toward the passenger(as if they hit the "up" arrow)
    //   2a: if the elevators current floor is HIGHER than p, then their direction must be -1
    //   2b: if the elevators current floor is LOWER than p, then their direction must be 1

    qDebug() << p->toString();

    for(int e = 0; e < NUM_CARS; e++){
        qDebug() << cars[e]->toString();

        if(!cars[e] -> isOverload && !cars[e] -> isFire && !cars[e] -> needHelp){ // If the elevator is having any of these issues, it cannot be called
            if(cars[e]-> direction == 0){
                connect(p, SIGNAL(request_elevator(Passenger*)), cars[e], SLOT(add_to_upcoming_passengers(Passenger*)));
                p->send_elevator_request();
                disconnect(p, SIGNAL(request_elevator(Passenger*)), cars[e], SLOT(add_to_upcoming_passengers(Passenger*)));
                //qDebug() << "Car after update " << cars[e]->toString();
                return true;
            }else if(cars[e]->direction == 1 && p->direction == 1 && p->current_floor > cars[e]->current_floor){ // elevator can head up toward the person
                connect(p, SIGNAL(request_elevator(Passenger*)), cars[e], SLOT(add_to_upcoming_passengers(Passenger*)));
                p->send_elevator_request();
                disconnect(p, SIGNAL(request_elevator(Passenger*)), cars[e], SLOT(add_to_upcoming_passengers(Passenger*)));
                //qDebug() << "Car after update " << cars[e]->toString();
                return true;
            }else if(cars[e]->direction == -1 && p->direction == -1 && p->current_floor < cars[e]->current_floor){ // elevator can head down to pickup the person
                connect(p, SIGNAL(request_elevator(Passenger*)), cars[e], SLOT(add_to_upcoming_passengers(Passenger*)));
                p->send_elevator_request();
                disconnect(p, SIGNAL(request_elevator(Passenger*)), cars[e], SLOT(add_to_upcoming_passengers(Passenger*)));
                //qDebug() << "Car after update " << cars[e]->toString();
                return true;
            }
        }
    }
    return false; // Could not find a suitable elevator, try again next iteration
}


















