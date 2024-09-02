#include "SensorDisplayVisitor.h"
#include "../logic_model/AbstractSensor.h"
#include "../logic_model/SpeedSensor.h"
#include "../logic_model/CadenceSensor.h"
#include "../logic_model/HeartRateSensor.h"
#include "../logic_model/ContainerSensorList.h"
#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>
#include <QValidator>
#include <QVBoxLayout>
#include <QObject>
#include <QComboBox>

SensorDisplayVisitor::SensorDisplayVisitor(QLabel* label, QObject* parent)
    : label(label), layout(nullptr), parent(parent) {}

SensorDisplayVisitor::SensorDisplayVisitor(QVBoxLayout* layout, QObject* parent)
    : label(nullptr), layout(layout), parent(parent) {}

void SensorDisplayVisitor::visit(const SpeedSensor& sensor) {
    label->setText(
        "Meters: " + QString::number(sensor.meters) + 
        "\nAverage Speed: " + (sensor.averageSpeed == 0 ? "Random Value" : QString::number(sensor.averageSpeed)) +
        "\nSpeed Drift: " + QString::number(sensor.speedDrift) +
        "\nInjury Probability: " + QString::number(sensor.injuryProbability)
    );
}

void SensorDisplayVisitor::visit(const CadenceSensor& sensor) {
    label->setText(
        "Meters: " + QString::number(sensor.meters) +
        "\nAverage Cadence: " + (sensor.averageCadence == 0 ? "Random Value" : QString::number(sensor.averageCadence)) +
        "\nCadence Drift: " + QString::number(sensor.cadenceDrift) +
        "\nInjury Probability: " + QString::number(sensor.injuryProbability)
    );
}

void SensorDisplayVisitor::visit(const HeartRateSensor& sensor) {
    label->setText(
        "Meters: " + QString::number(sensor.meters) +
        "\nResting Heart Rate: " + (sensor.restingHeartRate == 0 ? "Random Value" : QString::number(sensor.restingHeartRate)) +
        "\nMaximum Heart Rate: " + (sensor.maximumHeartRate == 0 ? "Random Value" : QString::number(sensor.maximumHeartRate)) +
        "\nHeart Rate Drift: " + QString::number(sensor.heartRateDrift) +
        "\nInjury Probability: " + QString::number(sensor.injuryProbability)
    );
}

void SensorDisplayVisitor::visitForDialog(SpeedSensor& sensor) {
    if (!layout) return;

    QComboBox* metersComboBox = new QComboBox();
    metersComboBox->addItems({"100", "200", "400", "800", "1500", "3000", "10000"});
    metersComboBox->setCurrentText(QString::number(sensor.meters));
    layout->addWidget(new QLabel("Meters:"));
    layout->addWidget(metersComboBox);
    QObject::connect(metersComboBox, &QComboBox::currentTextChanged, [&sensor](const QString& text) {
        sensor.meters = text.toInt();
    });

    QLineEdit* averageSpeedEdit = new QLineEdit(QString::number(sensor.averageSpeed));
    layout->addWidget(new QLabel("Average Speed (km/h) ( 0 = random value):"));
    layout->addWidget(averageSpeedEdit);
    connectValidation(averageSpeedEdit, 0.0, 40.0, "value between 0 and 40");
    QObject::connect(averageSpeedEdit, &QLineEdit::editingFinished, [edit = averageSpeedEdit, &sensor]() {
        sensor.averageSpeed = edit->text().toDouble();
    });

    QLineEdit* speedDriftEdit = new QLineEdit(QString::number(sensor.speedDrift));
    layout->addWidget(new QLabel("Speed Drift (%):"));
    layout->addWidget(speedDriftEdit);
    connectValidation(speedDriftEdit, 0.0, 20.0, "value between 0 and 20");
    QObject::connect(speedDriftEdit, &QLineEdit::editingFinished, [edit = speedDriftEdit, &sensor]() {
        sensor.speedDrift = edit->text().toDouble();
    });

    QLineEdit* injuryProbabilityEdit = new QLineEdit(QString::number(sensor.injuryProbability));
    layout->addWidget(new QLabel("Injury Probability (%):"));
    layout->addWidget(injuryProbabilityEdit);
    connectValidation(injuryProbabilityEdit, 0.0, 100.0, "value between 0 and 100");
    QObject::connect(injuryProbabilityEdit, &QLineEdit::editingFinished, [edit = injuryProbabilityEdit, &sensor]() {
        sensor.injuryProbability = edit->text().toDouble();
    });
}

void SensorDisplayVisitor::visitForDialog(CadenceSensor& sensor) {
    if (!layout) return;

    QComboBox* metersComboBox = new QComboBox();
    metersComboBox->addItems({"100", "200", "400", "800", "1500", "3000", "10000"});
    metersComboBox->setCurrentText(QString::number(sensor.meters));
    layout->addWidget(new QLabel("Meters:"));
    layout->addWidget(metersComboBox);
    QObject::connect(metersComboBox, &QComboBox::currentTextChanged, [&sensor](const QString& text) {
        sensor.meters = text.toInt();
    });

    QLineEdit* averageCadenceEdit = new QLineEdit(QString::number(sensor.averageCadence));
    layout->addWidget(new QLabel("Average Cadence (rpm) ( 0 = random value):"));
    layout->addWidget(averageCadenceEdit);
    connectValidation(averageCadenceEdit, 0.0, 200.0, "value between 0 and 200");
    QObject::connect(averageCadenceEdit, &QLineEdit::editingFinished, [edit = averageCadenceEdit, &sensor]() {
        sensor.averageCadence = edit->text().toDouble();
    });

    QLineEdit* cadenceDriftEdit = new QLineEdit(QString::number(sensor.cadenceDrift));
    layout->addWidget(new QLabel("Cadence Drift (%):"));
    layout->addWidget(cadenceDriftEdit);
    connectValidation(cadenceDriftEdit, 0.0, 20.0, "value between 0 and 20");
    QObject::connect(cadenceDriftEdit, &QLineEdit::editingFinished, [edit = cadenceDriftEdit, &sensor]() {
        sensor.cadenceDrift = edit->text().toDouble();
    });

    QLineEdit* injuryProbabilityEdit = new QLineEdit(QString::number(sensor.injuryProbability));
    layout->addWidget(new QLabel("Injury Probability (%):"));
    layout->addWidget(injuryProbabilityEdit);
    connectValidation(injuryProbabilityEdit, 0.0, 100.0, "value between 0 and 100");
    QObject::connect(injuryProbabilityEdit, &QLineEdit::editingFinished, [edit = injuryProbabilityEdit, &sensor]() {
        sensor.injuryProbability = edit->text().toDouble();
    });
}

void SensorDisplayVisitor::visitForDialog(HeartRateSensor& sensor) {
    if (!layout) return;

    QComboBox* metersComboBox = new QComboBox();
    metersComboBox->addItems({"100", "200", "400", "800", "1500", "3000", "10000"});
    metersComboBox->setCurrentText(QString::number(sensor.meters));
    layout->addWidget(new QLabel("Meters:"));
    layout->addWidget(metersComboBox);
    QObject::connect(metersComboBox, &QComboBox::currentTextChanged, [&sensor](const QString& text) {
        sensor.meters = text.toInt();
    });

    QLineEdit* restingHeartRateEdit = new QLineEdit(QString::number(sensor.restingHeartRate));
    layout->addWidget(new QLabel("Resting Heart Rate (bpm) ( 0 = random value):"));
    layout->addWidget(restingHeartRateEdit);
    connectValidation(restingHeartRateEdit, 30.0, 100.0, "value between 30 and 100");
    QObject::connect(restingHeartRateEdit, &QLineEdit::editingFinished, [edit = restingHeartRateEdit, &sensor]() {
        sensor.restingHeartRate = edit->text().toDouble();
    });

    QLineEdit* maximumHeartRateEdit = new QLineEdit(QString::number(sensor.maximumHeartRate));
    layout->addWidget(new QLabel("Maximum Heart Rate (bpm) ( 0 = random value):"));
    layout->addWidget(maximumHeartRateEdit);
    connectValidation(maximumHeartRateEdit, 100.0, 220.0, "value between 100 and 220");
    QObject::connect(maximumHeartRateEdit, &QLineEdit::editingFinished, [edit = maximumHeartRateEdit, &sensor]() {
        sensor.maximumHeartRate = edit->text().toDouble();
    });

    QLineEdit* heartRateDriftEdit = new QLineEdit(QString::number(sensor.heartRateDrift));
    layout->addWidget(new QLabel("Heart Rate Drift (%):"));
    layout->addWidget(heartRateDriftEdit);
    connectValidation(heartRateDriftEdit, 0.0, 20.0, "value between 0 and 20");
    QObject::connect(heartRateDriftEdit, &QLineEdit::editingFinished, [edit = heartRateDriftEdit, &sensor]() {
        sensor.heartRateDrift = edit->text().toDouble();
    });

    QLineEdit* injuryProbabilityEdit = new QLineEdit(QString::number(sensor.injuryProbability));
    layout->addWidget(new QLabel("Injury Probability (%):"));
    layout->addWidget(injuryProbabilityEdit);
    connectValidation(injuryProbabilityEdit, 0.0, 100.0, "value between 0 and 100");
    QObject::connect(injuryProbabilityEdit, &QLineEdit::editingFinished, [edit = injuryProbabilityEdit, &sensor]() {
        sensor.injuryProbability = edit->text().toDouble();
    });
}

void SensorDisplayVisitor::connectValidation(QLineEdit* edit, double minValue, double maxValue, const QString& placeholderText) {
    edit->setPlaceholderText(placeholderText);

    QObject::connect(edit, &QLineEdit::editingFinished, parent, [edit, minValue, maxValue]() {
        bool ok;
        double value = edit->text().toDouble(&ok);

        if (!ok) {
            QMessageBox::warning(edit, "Invalid Input", "Please enter a valid number.");
            edit->clear();
            return;
        }

        if (value != 0 && (value < minValue || value > maxValue)) {
            QMessageBox::warning(edit, "Invalid Input", QString("Please enter a value between %1 and %2, or 0.").arg(minValue).arg(maxValue));
            edit->clear();
        }
    });
}