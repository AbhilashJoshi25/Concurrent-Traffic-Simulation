#ifndef TRAFFICLIGHT_H
#define TRAFFICLIGHT_H

#include <mutex>
#include <deque>
#include <condition_variable>
#include "TrafficObject.h"

// forward declarations to avoid include cycle
class Vehicle;


// FP.3 Define a class âMessageQueueâ which has the public methods send and receive. 
// Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type. 
// Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase. 
// Also, there should be an std::condition_variable as well as an std::mutex as private members. 

template <class T>
class MessageQueue
{
public:

    //Define a class âMessageQueueâ which has the public methods send and receive.
    T receive();

    // Send should take an rvalue reference of type TrafficLightPhase whereas receive should return this type.
    void send(T &&msg);

private:
    // Also, the class should define an std::dequeue called _queue, which stores objects of type TrafficLightPhase.
    std::deque<T> _queue;
    // Also, there should be an std::condition_variable as well as an std::mutex as private members. 
    std::condition_variable _condition;
    std::mutex _mutex;
};

// FP.1 : Define a class âTrafficLightâ which is a child class of TrafficObject. 
// The class shall have the public methods âvoid waitForGreen()â and âvoid simulate()â 
// as well as âTrafficLightPhase getCurrentPhase()â, where TrafficLightPhase is an enum that 
// can be either âredâ or âgreenâ. Also, add the private method âvoid cycleThroughPhases()â. 
// Furthermore, there shall be the private member _currentPhase which can take âredâ or âgreenâ as its value. 

// as well as âTrafficLightPhase getCurrentPhase()â, where TrafficLightPhase is an enum that 
// can be either âredâ or âgreenâ. Also, add the private method âvoid cycleThroughPhases()â.
enum TrafficLightPhase {red, green};

//Define a class âTrafficLightâ which is a child class of TrafficObject.
class TrafficLight : public TrafficObject
{
public:
    // constructor / desctructor
    TrafficLight();

    // getters / setters
    TrafficLightPhase getCurrentPhase();

    // typical behaviour methods
    // The class shall have the public methods âvoid waitForGreen()â and âvoid simulate()â 
    void waitForGreen();
    void simulate();

private:
    // typical behaviour methods
    // can be either âredâ or âgreenâ. Also, add the private method âvoid cycleThroughPhases()â.
    void cycleThroughPhases();

    // Furthermore, there shall be the private member _currentPhase which can take âredâ or âgreenâ as its value. 
    TrafficLightPhase _currentPhase;

    // FP.4b : create a private member of type MessageQueue for messages of type TrafficLightPhase 
    // and use it within the infinite loop to push each new TrafficLightPhase into it by calling 
    // send in conjunction with move semantics.
    //MessageQueue
    MessageQueue<TrafficLightPhase> mq;

    std::condition_variable _condition;
    std::mutex _mutex;
};

#endif
