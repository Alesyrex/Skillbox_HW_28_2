#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <mutex>

enum ID
{
    A = 1,
    B,
    C
};

std::mutex mtx;
static bool stationBusy = false;

class Train
{
    int timeTravel;
    ID trainID;

public:

    Train(int inID)
         : timeTravel(0), trainID(static_cast<ID>(inID))
    {
        std::cout << "Enter the travel time of the train " << getTrainID() << ":";
        std::cin >> timeTravel;
        if (timeTravel < 0) timeTravel = 1;
    }
    int getTimeTravel(){return timeTravel;}
    char getTrainID ()
    {
        if (trainID == A) return 'A';
        else if (trainID == B) return 'B';
        else if (trainID == C) return 'C';
    }

};

void arrival(Train &train)
{
    std::this_thread::sleep_for(std::chrono::seconds(train.getTimeTravel()));
    if (stationBusy) std::cout << "Train " << train.getTrainID() << " is waiting for a free seat." << std::endl;
    mtx.lock();
    stationBusy = true;
    std::cout << "Train " << train.getTrainID() << " arrived at the station." << std::endl << std::endl;
    std::string depart;
    std::cout << "Enter \"depart\" to send a train " << train.getTrainID() << " from the station." << std::endl;
    do
    {
        std::cin >> depart;
    } while (depart != "depart");
    stationBusy = false;
    std::cout << "Train " << train.getTrainID() << " departed from the station." << std::endl;
    mtx.unlock();
}

int main() {

    int amountTrains = 3;
    std::vector<Train> trains;
    std::vector<std::thread> trainThr;

    for (int i=0;i < amountTrains;++i)
    {
        trains.push_back(Train(i+1));
    }

    for (int i=0;i < amountTrains;++i)
    {
        trainThr.push_back(std::thread (arrival,std::ref(trains[i])));
    }

    for (auto& i : trainThr)
    {
        i.join();
    }
    
    return 0;
}
