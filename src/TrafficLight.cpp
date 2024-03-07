#include <iostream>
#include <random>
#include "TrafficLight.h"

/* Implementation of class "MessageQueue" */

/* 
template <typename T>
T MessageQueue<T>::receive()
{
    // FP.5a : The method receive should use std::unique_lock<std::mutex> and _condition.wait() 
    // to wait for and receive new messages and pull them from the queue using move semantics. 
    // The received object should then be returned by the receive function. 
}

template <typename T>
void MessageQueue<T>::send(T &&msg)
{
    // FP.4a : The method send should use the mechanisms std::lock_guard<std::mutex> 
    // as well as _condition.notify_one() to add a new message to the queue and afterwards send a notification.
}
*/

/* Implementation of class "TrafficLight" */

/* 
TrafficLight::TrafficLight()
{
    _currentPhase = TrafficLightPhase::red;
}

void TrafficLight::waitForGreen()
{
    // FP.5b : add the implementation of the method waitForGreen, in which an infinite while-loop 
    // runs and repeatedly calls the receive function on the message queue. 
    // Once it receives TrafficLightPhase::green, the method returns.
}

TrafficLightPhase TrafficLight::getCurrentPhase()
{
    return _currentPhase;
}

*/

void TrafficLight::simulate()
{
    // FP.2b : Finally, the private method „cycleThroughPhases“ should be started in a thread when the public method „simulate“ is called. To do this, use the thread queue in the base class. 
  threads.emplace_back(std::thread(&TrafficLight::cycleThroughPhases, this));
}

// virtual function which is executed in a thread
void TrafficLight::cycleThroughPhases()
{
    // FP.2a : Implement the function with an infinite loop that measures the time between two loop cycles 
    // and toggles the current phase of the traffic light between red and green and sends an update method 
    // to the message queue using move semantics. The cycle duration should be a random value between 4 and 6 seconds. 
    // Also, the while-loop should use std::this_thread::sleep_for to wait 1ms between two cycles.
  std::random_device os_seed;
  const uint_least32_t seed = os_seed();

  std::mt19937 generator(seed);
  std::uniform_real_distribution<double> distribute(4.0,6.0);
  
  double random_wait;
  
  auto t1 = std::chrono::high_resolution_clock::now();
  auto t2 = std::chrono::high_resolution_clock::now();
  while (true){
    
    // Update the current time and compute the cycle time
    t2 = std::chrono::high_resolution_clock::now();
    double dt = (std::chrono::duration<double>(t2-t1)).count();
    
    // Compute and sleep for random wait time to wait for 4-6 s
    random_wait = distribute(generator) - dt;
    std::this_thread::sleep_for(std::chrono::duration<double>(random_wait));
    
    // Toggle from red to green or from green to red
    if (_currentPhase==TrafficLightPhase::red){
      _currentPhase = TrafficLightPhase::green;
    } else if (_currentPhase==TrafficLightPhase::green) {
      _currentPhase = TrafficLightPhase::red;
    }
    
    // Send light phase to queue
    _light_singals.send(std::move(_currentPhase));
    
    // Sleep for 1 millisecond
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    t1 = t2;
  }
}