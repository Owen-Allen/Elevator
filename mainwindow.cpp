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
     connect(ui->pushButton_5, SIGNAL(released()), this, SLOT(normal()));


    // Initialize Passengers
    int start[5] =  {1, 2, 3, 4, 5};
    int end[5] =    {5, 4, 1, 7, 10};

    for(int i = 0; i < NUM_PASSENGERS; i++){
        people[i] = new Passenger(start[i], end[i]);
        connect(people[i], SIGNAL(request(int, int)), this, SLOT(add_request(int, int)));
    }

    // Initialize Cars
    int curr_floor[2] =  {1, 4};
    int dir[2] =    {0, 0};
    bool moving[2] = {false, false};

    for (int i = 0; i < NUM_CARS; i++){
        cars[i] = new CarModel(curr_floor[i], dir[i]);
    }

    //Connect Passengers and Cars
    
    //for(int j = 0; j < NUM_CARS; j++){
    //    connect(this, SIGNAL(add_passenger_to_car()), cars[j], SLOT(add_passenger()));
    //}





    // Declare the passengers and the elevators
    // Tests can be ran in whatever order the user desires through the GUI


    // Is the array of elevators created here?
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
    people[0]->send_request();
    qInfo("Back from send request");
    determine_elevator();

}



void MainWindow::determine_elevator(){
    // Loop through the queue and try and add the passenger to an elevators next queue
    qInfo("determine elevator")
    int r_dir;
    
    for (std::list<Passenger*>::iterator r = pickup_requests.begin(); r != pickup_requests.end(); ++r){
        
        //bool found_elevator = false;
        // Which way does the requester want to go?
        if(r->current_floor < r->destination_floor){
            r_dir = 1; //up
        }else{
            r_dir = -1;
        }
        
        // Now see if there are any available elevators, or elevators that are going in the same direction
        
        for(int e = 0; e < NUM_CARS; e++){
            if(!elevators[e]->isMoving || elevators[e]->direction == r_dir){
                // MAYBE WE SHOULD CALL A FUNCTION IN ELEVATOR??
                    // we need to add the passenger, update the elevators list of to do
                    // and check if we need to re-evaluate its final destination
                
                // Add the passenger to the elevators pickup list
                elevators[e]->next_passengers.push(r);
                if(elevators[e]->destination >)
                
                // remove the passenger from the list of passengers to be assigned
                pickup_requests.erase(r++);
                break;
            }
        }
        
        
        
    }
}


// SLOTS
void MainWindow::add_request(Passenger* p){
    pickup_requests.push(p);
}



















