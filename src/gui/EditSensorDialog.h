#ifndef EDITSENSORDIALOG_H
#define EDITSENSORDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QValidator>
#include "../logic_model/AbstractSensor.h"
#include "../logic_model/ContainerSensorList.h"

class EditSensorDialog : public QDialog {
    Q_OBJECT

public:
    explicit EditSensorDialog(AbstractSensor* sensor, ContainerSensorList* containerList, QWidget* parent = nullptr);

private slots:
    void applyChanges();

private:
    AbstractSensor* sensor;
    QLineEdit* idEdit;
    QTextEdit* descriptionEdit;
    QPushButton* applyButton;
    ContainerSensorList* containerList; 
    QLineEdit* typeEdit;

    bool validateUniqueID(unsigned int id);
};

#endif