#ifndef ABSTRACTSENSOR_H
#define ABSTRACTSENSOR_H


#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <iomanip>
#include "SensorObserver.h"
#include "SensorVisitor.h"
#include "../gui/SensorDisplayVisitor.h"

class AbstractSensor {
private:
    unsigned int identifier;
    std::string type;
    std::string description;
    std::vector<SensorObserver*>observers;

public:
    AbstractSensor(const unsigned int &, const std::string &, const std::string &);
    AbstractSensor(const AbstractSensor &);
    virtual ~AbstractSensor();

    const unsigned int & getIdentifier() const;
    const std::string & getType() const;
    const std::string & getDescription() const;


    void setIdentifier(const unsigned int &);
    void setType(const std::string &);
    void setDescription(const std::string &);
    void registerObserver(SensorObserver*);
    void unregisterObserver(SensorObserver*);  
    virtual AbstractSensor * clonePattern () const = 0;
    virtual std::vector<double> simulateData () = 0;
    virtual void accept ( SensorVisitor &) const  =0;
    virtual void acceptForDialog( SensorVisitor &) =0;
    
    void notifyAllObservers();
  


};

#endif