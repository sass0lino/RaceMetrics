#include "ContainerSensorList.h"
#include <algorithm>

ContainerSensorList::ContainerSensorList() : isModified(false) {}

ContainerSensorList::~ContainerSensorList() {
    clearSensors();
}

void ContainerSensorList::addSensor(AbstractSensor* sensor) {
    sensors.push_back(sensor);
    isModified = true;
}

void ContainerSensorList::removeSensor(unsigned int id) {
    for (unsigned int i = 0; i < sensors.size(); ++i) {
        if (sensors[i]->getIdentifier() == id) {
            delete sensors[i];
            sensors.erase(sensors.begin() + i);
            isModified = true;
            break;
        }
    }
}

void ContainerSensorList::clearSensors() {
    for (unsigned int i = 0; i < sensors.size(); ++i) {
        delete sensors[i];
    }
    sensors.clear();
    isModified = true;
}

AbstractSensor* ContainerSensorList::getSensorById(unsigned int id) const {
    for (unsigned int i = 0; i < sensors.size(); ++i) {
        if (sensors[i]->getIdentifier() == id) {
            return sensors[i];
        }
    }
    return nullptr;
}

const std::vector<AbstractSensor*>& ContainerSensorList::getSensors() const {
    return sensors; 
}

unsigned int ContainerSensorList::count() const {
    return sensors.size();
}

AbstractSensor* ContainerSensorList::getSensorAt(unsigned int index) const {
    if (index < sensors.size()) {
        return sensors[index];
    }
    return nullptr;
}

bool ContainerSensorList::isIDUnique(unsigned int id) const {
    for (unsigned int i = 0; i < sensors.size(); ++i) {
        if (sensors[i]->getIdentifier() == id) {
            return false;
        }
    }
    return true;
}

bool ContainerSensorList::getIsModified() const {
    return isModified;
}

void ContainerSensorList::setIsModified(bool value) {
    isModified = value;
}
