#ifndef SENSORDIALOG_H
#define SENSORDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "../logic_model/ContainerSensorList.h"

class SensorDialog : public QDialog {
    Q_OBJECT

public:
    explicit SensorDialog(ContainerSensorList* containerList, QWidget* parent = nullptr);
    ~SensorDialog();

    QString getSensorType() const;
    QString getDescription() const;
    unsigned int getIdentifier() const;

private slots:
    void onAccept();
    bool validateUniqueID(unsigned int id);

private:
    ContainerSensorList* containerList;
    QLineEdit *idEdit;
    QLineEdit *descriptionEdit;
    QComboBox *sensorType;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif