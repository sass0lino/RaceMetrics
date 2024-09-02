#include "SensorPersistence.h"
#include "../logic_model/ContainerSensorList.h"
#include "../logic_model/SpeedSensor.h"
#include "../logic_model/CadenceSensor.h"
#include "../logic_model/HeartRateSensor.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMessageBox>

bool SensorPersistence::saveToFile(const QString& fileName, ContainerSensorList* containerList) {
    QJsonArray sensorArray;

    for (unsigned int i = 0; i < containerList->count(); ++i) {
        AbstractSensor* sensor = containerList->getSensorAt(i);
        if (sensor) {
            QJsonObject sensorObject;

            sensorObject["id"] = static_cast<int>(sensor->getIdentifier());
            sensorObject["type"] = QString::fromStdString(sensor->getType());
            sensorObject["description"] = QString::fromStdString(sensor->getDescription());

            if (auto speedSensor = dynamic_cast<SpeedSensor*>(sensor)) {
                sensorObject["meters"] = speedSensor->meters;
                sensorObject["averageSpeed"] = speedSensor->averageSpeed;
                sensorObject["speedDrift"] = speedSensor->speedDrift;
                sensorObject["injuryProbability"] = speedSensor->injuryProbability;
            } else if (auto cadenceSensor = dynamic_cast<CadenceSensor*>(sensor)) {
                sensorObject["meters"] = cadenceSensor->meters;
                sensorObject["averageCadence"] = cadenceSensor->averageCadence;
                sensorObject["cadenceDrift"] = cadenceSensor->cadenceDrift;
                sensorObject["injuryProbability"] = cadenceSensor->injuryProbability;
            } else if (auto heartRateSensor = dynamic_cast<HeartRateSensor*>(sensor)) {
                sensorObject["meters"] = heartRateSensor->meters;
                sensorObject["restingHeartRate"] = heartRateSensor->restingHeartRate;
                sensorObject["maximumHeartRate"] = heartRateSensor->maximumHeartRate;
                sensorObject["heartRateDrift"] = heartRateSensor->heartRateDrift;
                sensorObject["injuryProbability"] = heartRateSensor->injuryProbability;
            }

            sensorArray.append(sensorObject);
        }
    }

    QString finalFileName = fileName;
    if (!fileName.endsWith(".json", Qt::CaseInsensitive)) {
        finalFileName += ".json";
    }

    QJsonDocument doc(sensorArray);
    QFile file(finalFileName);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(nullptr, "Save Error", "Could not save the file.");
        return false;
    }
    file.write(doc.toJson());
    return true;
}

bool SensorPersistence::loadFromFile(const QString& fileName, ContainerSensorList* containerList) {
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, "Open Error", "Could not open the file.");
        return false;
    }

    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isArray()) {
        QMessageBox::warning(nullptr, "Open Error", "Invalid file format.");
        return false;
    }

    containerList->clearSensors();

    QJsonArray sensorArray = doc.array();
    for (const QJsonValue& value : sensorArray) {
        if (!value.isObject()) continue;

        QJsonObject sensorObject = value.toObject();

        unsigned int id = static_cast<unsigned int>(sensorObject["id"].toInt());
        QString type = sensorObject["type"].toString();
        QString description = sensorObject["description"].toString();

        AbstractSensor* newSensor = nullptr;

        if (type == "Speed Sensor") {
            newSensor = new SpeedSensor(id, type.toStdString(), description.toStdString());
            SpeedSensor* speedSensor = dynamic_cast<SpeedSensor*>(newSensor);
            speedSensor->meters = sensorObject["meters"].toInt();
            speedSensor->averageSpeed = sensorObject["averageSpeed"].toDouble();
            speedSensor->speedDrift = sensorObject["speedDrift"].toDouble();
            speedSensor->injuryProbability = sensorObject["injuryProbability"].toDouble();
        } else if (type == "Cadence Sensor") {
            newSensor = new CadenceSensor(id, type.toStdString(), description.toStdString());
            CadenceSensor* cadenceSensor = dynamic_cast<CadenceSensor*>(newSensor);
            cadenceSensor->meters = sensorObject["meters"].toInt();
            cadenceSensor->averageCadence = sensorObject["averageCadence"].toDouble();
            cadenceSensor->cadenceDrift = sensorObject["cadenceDrift"].toDouble();
            cadenceSensor->injuryProbability = sensorObject["injuryProbability"].toDouble();
        } else if (type == "Heart Rate Sensor") {
            newSensor = new HeartRateSensor(id, type.toStdString(), description.toStdString());
            HeartRateSensor* heartRateSensor = dynamic_cast<HeartRateSensor*>(newSensor);
            heartRateSensor->meters = sensorObject["meters"].toInt();
            heartRateSensor->restingHeartRate = sensorObject["restingHeartRate"].toDouble();
            heartRateSensor->maximumHeartRate = sensorObject["maximumHeartRate"].toDouble();
            heartRateSensor->heartRateDrift = sensorObject["heartRateDrift"].toDouble();
            heartRateSensor->injuryProbability = sensorObject["injuryProbability"].toDouble();
        } else {
            QMessageBox::warning(nullptr, "Open Error", "Unknown sensor type: " + type);
            continue;
        }

        if (newSensor) {
            containerList->addSensor(newSensor);
        }
    }
    return true;
}