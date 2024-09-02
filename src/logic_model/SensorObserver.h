#ifndef SENSOR_OBSERVER
#define SENSOR_OBSERVER

class AbstractSensor;

class SensorObserver{
public:
    virtual void notify(const AbstractSensor&) = 0;
    virtual ~SensorObserver() = default;

};



#endif