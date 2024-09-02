#include "AbstractSensor.h"

AbstractSensor::AbstractSensor(const unsigned int & identifier_, const std::string & type_, const std::string & description_): identifier(identifier_), type(type_), description(description_){};
AbstractSensor::AbstractSensor(const AbstractSensor & ob): identifier(ob.identifier), type(ob.type), description(ob.description){};
AbstractSensor::~AbstractSensor() {
    for (auto observer : observers) {
        delete observer;
    }
}
const unsigned int & AbstractSensor::getIdentifier() const {
    return identifier;
};

const std::string & AbstractSensor::getType() const {
    return type;
};

const std::string & AbstractSensor::getDescription() const {
    return description;
};

void AbstractSensor::setIdentifier(const unsigned int & id_){
    identifier= id_;
};
void AbstractSensor::setType(const std::string & type_){
    type= type_;
};
void AbstractSensor::setDescription(const std::string & description_){
    description= description_;
};

void AbstractSensor::registerObserver(SensorObserver* obs){
    observers.push_back(obs);
}

void AbstractSensor::unregisterObserver(SensorObserver* obs) {
    for (auto it = observers.begin(); it != observers.end(); ++it) {
        if (*it == obs) {
            observers.erase(it);
            break;
        }
    }
}

void AbstractSensor::notifyAllObservers() {
    for (size_t i = 0; i < observers.size(); ++i) {
        if (observers[i]) {
            observers[i]->notify(*this);
        }
    }
}