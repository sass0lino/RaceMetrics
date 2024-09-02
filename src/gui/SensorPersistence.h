#ifndef SENSORPERSISTENCE_H
#define SENSORPERSISTENCE_H

#include <QString>
#include "../logic_model/AbstractSensor.h"
#include "../logic_model/SpeedSensor.h"
#include "../logic_model/CadenceSensor.h"
#include "../logic_model/HeartRateSensor.h"
#include "../logic_model/ContainerSensorList.h"

class SensorPersistence {
public:
    bool saveToFile(const QString& fileName, ContainerSensorList* containerList);
    bool loadFromFile(const QString& fileName, ContainerSensorList* containerList);
};

#endif