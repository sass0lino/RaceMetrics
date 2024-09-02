#ifndef CONTAINERSENSORLIST_H
#define CONTAINERSENSORLIST_H

#include <vector>
#include "AbstractSensor.h"

class ContainerSensorList {
    std::vector<AbstractSensor*> sensors;
    bool isModified;
public:
    ContainerSensorList();
    ~ContainerSensorList();

    void addSensor(AbstractSensor* sensor);
    void removeSensor(unsigned int id);
    void clearSensors();

    AbstractSensor* getSensorById(unsigned int id) const;
    const std::vector<AbstractSensor*>& getSensors() const;
    unsigned int count() const;
    AbstractSensor* getSensorAt(unsigned int index) const;
    
    bool isIDUnique(unsigned int id) const;
    bool getIsModified() const;
    void setIsModified(bool value);
};

#endif