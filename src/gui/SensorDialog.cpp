#include "SensorDialog.h"
#include <QFormLayout>
#include <QIntValidator>
#include <QMessageBox>
#include <QLabel>
#include "MainWindow.h"

SensorDialog::SensorDialog(ContainerSensorList* containerList, QWidget* parent)
    : QDialog(parent), containerList(containerList) {
    setWindowTitle("Create Sensor");

    idEdit = new QLineEdit(this);
    idEdit->setValidator(new QIntValidator(0, 99999, this));
    idEdit->setPlaceholderText("up to 5 digits");

    descriptionEdit = new QLineEdit(this);
    descriptionEdit->setMaxLength(200);
    descriptionEdit->setPlaceholderText("up to 200 characters");


    sensorType = new QComboBox(this);
    sensorType->addItem("");
    sensorType->addItem("Speed Sensor");
    sensorType->addItem("Cadence Sensor");
    sensorType->addItem("Heart Rate Sensor");

    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);

    QFormLayout *formLayout = new QFormLayout;
    formLayout->addRow("ID:(*)", idEdit);
    formLayout->addRow("Sensor Type:(*)", sensorType);
    formLayout->addRow("Description:", descriptionEdit);
    QLabel *requiredFields = new QLabel("(*) indicates a required field");
    requiredFields->setAlignment(Qt::AlignRight);
    formLayout->addWidget(requiredFields);

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    QVBoxLayout *labelLayout = new QVBoxLayout(this);
    labelLayout->addLayout(formLayout);
    labelLayout->addLayout(buttonLayout);

    connect(okButton, &QPushButton::clicked, this, &SensorDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    setLayout(labelLayout);
}

SensorDialog::~SensorDialog() = default;

QString SensorDialog::getSensorType() const {
    return sensorType->currentText();
}

QString SensorDialog::getDescription() const {
    return descriptionEdit->text();
}

unsigned int SensorDialog::getIdentifier() const {
    return idEdit->text().toUInt();
}

void SensorDialog::onAccept() {
    if (!idEdit->text().isEmpty() && !sensorType->currentText().isEmpty()) {
        unsigned int id = getIdentifier();
        if (validateUniqueID(id)) {
            // Crea e aggiungi il sensore al ContainerSensorList
            std::string type = getSensorType().toStdString();
            std::string description = getDescription().toStdString();
            AbstractSensor* newSensor = nullptr;

            if (type == "Speed Sensor") {
                newSensor = new SpeedSensor(id, type, description);
            } else if (type == "Cadence Sensor") {
                newSensor = new CadenceSensor(id, type, description);
            } else if (type == "Heart Rate Sensor") {
                newSensor = new HeartRateSensor(id, type, description);
            }

            if (newSensor) {
                containerList->addSensor(newSensor); 
                QDialog::accept(); 
            }
        }
    } else {
        QMessageBox::warning(this, "Incomplete Form", "Please fill in the required fields.");
    }
}

bool SensorDialog::validateUniqueID(unsigned int id) {
    if (!containerList->isIDUnique(id)) {
        QMessageBox::warning(this, "Invalid ID", "The ID must be unique.");
        return false;
    }
    return true;
}