#ifndef DETAILEDSENSORWIDGET_H
#define DETAILEDSENSORWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QChartView>
#include <QScrollArea>
#include <QLineSeries>
#include <QChart>
#include "SensorDisplayVisitor.h"
#include "../logic_model/AbstractSensor.h"
#include "../logic_model/SensorObserver.h"

class ContainerSensorList;

class DetailedSensorWidget : public QWidget, public SensorObserver {
    Q_OBJECT

public:
    explicit DetailedSensorWidget(AbstractSensor* sensor, ContainerSensorList* containerList, QWidget *parent = nullptr);
    ~DetailedSensorWidget();

    void notify(const AbstractSensor&) override;

signals:
    void sensorDeleted(AbstractSensor* sensor);

private slots:
    void onSimulateButtonClicked();
    void onEditButtonClicked();
    void onDeleteButtonClicked();

private:
    void updateChart(const std::vector<double>& data);

    AbstractSensor* sensor;
    ContainerSensorList* containerList;

    QLabel *iconLabel;
    QLabel *typeLabel;
    QLabel *identifierLabel;
    QLabel *descriptionLabel;
    QLabel *parametersLabel;
    QVBoxLayout *mainLayout;
    QHBoxLayout *topLayout;
    QChartView *chartView;

    QPushButton* editButton;
    QPushButton* deleteButton;
    QPushButton* simulateButton;
};

#endif