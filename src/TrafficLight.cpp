#include <iostream>
#include <random>
#include "TrafficLight.h"

#include<chrono>
#include<future>
#include<queue>

/* Implementation of class "MessageQueue" */

template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 

    //The method receive should use std::unique_lock<std::mutex> and _condition.wait()
    std::unique_lock<std::mutex>curr_lock(_mutex);
    _condition.wait(curr_lock, [this]{
        return !_queue.empty();
    });

    // to wait for and receive new messages and pull them from the queue using move semantics.
    T message_queue = std::move(_queue.back());
    _queue.pop_back();

    // The received object should then be returned by the receive function. 
    return message_queue;

}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.

    //The method send should use the mechanisms std::lock_guard<std::mutex>
    std::lock_guard<std::mutex>curr_lock(_mutex);
    _queue.push_back(std::move(msg));

    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
    _condition.notify_one();
}

/* Implementation of class "TrafficLight" */

TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.

    //Add the implementation of the method waitForGreen, in which an infinite while-loop
    while(true){
        //receiving message to traffic light phase
        TrafficLightPhase received_message = mq.receive();
        // Once it receives TrafficLightPhase::green, the method returns.
        if(received_message == TrafficLightPhase::green){
            return;
        }
    }
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method âcycleThroughPhasesâ should be started in a thread when the public method âsimulateâ is called. To do this, use the thread queue in the base class.
    threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.

    //To generate a random number between min and max, use:
    //int randNum = rand()%(max-min + 1) + min;

    int cd_randNum = rand()%(6000 - 4000 + 1) + 4000;

    //Returning the ID for curr Thread
    auto prev_time_update = std::chrono::system_clock::now();

    //Implement the function with an infinite loop that measures the time between two loop cycles
    while(true){
        //compute the difference of time updated so far
        auto timesofar = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - prev_time_update);

        //keep a count for the timesofar
        auto temp_time = timesofar.count();

        // and toggles the current phase of the traffic light between red and green and sends an update method 
        // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds.

        if(temp_time >= cd_randNum){
            if(_currentPhase == TrafficLightPhase::red){
                _currentPhase = TrafficLightPhase::green;
            }
            else{
                _currentPhase = TrafficLightPhase::red;
            }
            mq.send(std::move(_currentPhase));
            prev_time_update = std::chrono::system_clock::now();
        }
        // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
        //make a thread sleep for 1 msec
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
