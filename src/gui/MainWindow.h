#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QLineEdit> 
#include "SensorWidget.h"
#include "DetailedSensorWidget.h"
#include "SensorDialog.h"
#include "SensorPersistence.h"
#include "EditSensorDialog.h"
#include "SensorDisplayVisitor.h"
#include "../logic_model/AbstractSensor.h"
#include "../logic_model/SpeedSensor.h"
#include "../logic_model/CadenceSensor.h"
#include "../logic_model/HeartRateSensor.h"
#include "../logic_model/ContainerSensorList.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void createSensor();
    void showDetailedSensor(SensorWidget* widget);
    void deleteSensor(AbstractSensor* sensor);
    void save();
    void saveAs();
    void openFile();
    void closeFile();
    void updateSearchResults(const QString& text);
    void clearSearchBar();

private:
    void setupMenuBar();
    void updateWindowTitle();
    void clearSensors();
    
    QVBoxLayout* sensorListLayout;

    void addSensorWidget(AbstractSensor* sensor);
    void deleteSensorWidget(SensorWidget* sensorWidget);

    QWidget *centralWidget;
    QVBoxLayout *leftLayout;
    QVBoxLayout *rightLayout;
    QScrollArea *scrollArea;

    QPushButton *createSensorButton;
    QLineEdit* searchBar; 
    QPushButton* clearSearchButton;
    ContainerSensorList *containerList;
    SensorPersistence *sensorPersistence; 
    QString currentFileName;
    bool isModified;
};

#endif