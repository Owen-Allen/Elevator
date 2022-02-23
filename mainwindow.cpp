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
    QString names[5] = {"Owen", "Cole", "Mej", "Migu", "Lord Farquad"};
    int start[5] =  {1, 4, 5, 4, 5};
    int end[5] =    {5, 1, 1, 7, 2};

    // Connect the passengers to the Main Window so they can call an elevator
    for(int i = 0; i < NUM_PASSENGERS; i++){
        people[i] = new Passenger(start[i], end[i], names[i]);
        //connect(people[i], SIGNAL(request(Passenger*)), this, SLOT(add_request(Passenger*)));
    }


    // Initialize Cars
    int curr_floor[2] =  {1, 4};
    int dir[2] =    {0, 0};


    for (int i = 0; i < NUM_CARS; i++){
        cars[i] = new CarModel(curr_floor[i], dir[i]);
        connect(this, SIGNAL(request_upcoming_passenger(Passenger*)), cars[i], SLOT(add_to_upcoming_passengers(Passenger*)));
    }

    //control_clock = new Timer();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::overload_sensor(){ //
    qInfo("overload");
}

void MainWindow::fire_sensor(){
    qInfo("fire in da boof");
}

void MainWindow::power_sensor(){
    qInfo("eh wheres da power");
}

void MainWindow::door_sensor(){
    qInfo("Door sensor");
}

void MainWindow::help(){
    qInfo("Help");
}


void MainWindow::normal(){
    // GAME LOOP SHOULD OCCUR HERE
    qInfo("Normal");

    // Fill the pickup_requests
    for(int i = 0; i < NUM_PASSENGERS; i++){
        pickup_requests.push_back(people[i]);
    }
    

    //control_clock ->interval();

    std::list<Passenger*>:: iterator it = pickup_requests.begin();
    while(it != pickup_requests.end()){
        if(determine_elevator(*it)){
            qDebug() << "added " << (*it) -> name << " to an upcoming_pickup";
            // the Passenger has been moved to an upcoming_request for an elevator,
            // so we can remove them from pickup_requests
            it = pickup_requests.erase(it);
        }else{
            qDebug() << "could not add " << (*it)->name << " to an upcoming_pickup";
            ++it;
        }
    }
    
    /*
        qDebug() << "Front of pickup_requests " << pickup_requests.front()->toString();
        connect(pickup_requests.front(), SIGNAL(request_elevator(Passenger*)), cars[0], SLOT(add_to_upcoming_passengers(Passenger*)));
        pickup_requests.front() -> send_elevator_request();
    */
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
        if(cars[e]->direction == 0 || (cars[e]->direction == p->direction && cars[e]->current_floor < p->current_floor) ||  (cars[e]->direction == p->direction && cars[e]->current_floor > p->current_floor)){
            connect(p, SIGNAL(request_elevator(Passenger*)), cars[e], SLOT(add_to_upcoming_passengers(Passenger*)));
            p->send_elevator_request();
            disconnect(p, SIGNAL(request_elevator(Passenger*)), cars[e], SLOT(add_to_upcoming_passengers(Passenger*)));
            return true;
        }
    }
    return false; // Could not find a suitable elevator, try again next iteration
}


// SLOTS
void MainWindow::add_request(Passenger* p){
    pickup_requests.push_front(p);
}



















