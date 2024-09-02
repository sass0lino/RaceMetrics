#include "DetailedSensorWidget.h"
#include "EditSensorDialog.h"
#include "../logic_model/ContainerSensorList.h"
#include <QMessageBox>

DetailedSensorWidget::DetailedSensorWidget(AbstractSensor* sensor, ContainerSensorList* containerList, QWidget *parent)
    : QWidget(parent), sensor(sensor), containerList(containerList) { 

typeLabel = new QLabel("Type: " + QString::fromStdString(sensor->getType()), this);
identifierLabel = new QLabel("ID: " + QString::number(sensor->getIdentifier()), this);

descriptionLabel = new QLabel("Description: " + QString::fromStdString(sensor->getDescription()), this);
descriptionLabel->setWordWrap(true);

QString type = QString::fromStdString(sensor->getType()).replace(" ", "_");
QString iconPath = ":/assets/" + type + "_icon.png";
iconLabel = new QLabel(this);
iconLabel->setPixmap(QPixmap(iconPath).scaled(64, 64, Qt::KeepAspectRatio));
iconLabel->setFixedWidth(64);
iconLabel-> setAlignment(Qt::AlignCenter);

parametersLabel = new QLabel(this);
SensorDisplayVisitor visitor(parametersLabel);
parametersLabel-> setAlignment(Qt::AlignCenter);
sensor->accept(visitor);

topLayout = new QHBoxLayout;

QVBoxLayout* topLeftLayout = new QVBoxLayout;

topLeftLayout->setSizeConstraint(QLayout::SetMinimumSize);

topLeftLayout->addWidget(typeLabel);
topLeftLayout->addWidget(identifierLabel);
topLeftLayout->addWidget(descriptionLabel);

topLayout->addWidget(iconLabel);
topLayout->addStretch(); 
topLayout->addLayout(topLeftLayout);
topLayout->addStretch(); 
topLayout->addWidget(parametersLabel);

mainLayout = new QVBoxLayout(this);
mainLayout->addLayout(topLayout);

editButton = new QPushButton("Edit Sensor/Simulation Parameters", this);
deleteButton = new QPushButton("Delete Sensor", this);
simulateButton = new QPushButton("Simulate", this);

connect(editButton, &QPushButton::clicked, this, &DetailedSensorWidget::onEditButtonClicked);
connect(deleteButton, &QPushButton::clicked, this, &DetailedSensorWidget::onDeleteButtonClicked);
connect(simulateButton, &QPushButton::clicked, this, &DetailedSensorWidget::onSimulateButtonClicked);

mainLayout->addWidget(editButton);
mainLayout->addWidget(deleteButton);
mainLayout->addWidget(simulateButton);

chartView = new QChartView(this);
chartView->setMinimumHeight(200);
chartView->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
mainLayout->addWidget(chartView);

setLayout(mainLayout);

sensor->registerObserver(this);
}

DetailedSensorWidget::~DetailedSensorWidget() {
    sensor->unregisterObserver(this);
}

void DetailedSensorWidget::onSimulateButtonClicked() {
    std::vector<double> data = sensor->simulateData();
    updateChart(data);
    sensor->notifyAllObservers();
}

void DetailedSensorWidget::onEditButtonClicked() {
    EditSensorDialog dialog(sensor, containerList, this);
    if (dialog.exec() == QDialog::Accepted) {
        sensor->notifyAllObservers();
        containerList->setIsModified(true);
    }
}

void DetailedSensorWidget::onDeleteButtonClicked() {
    emit sensorDeleted(sensor);
}

void DetailedSensorWidget::notify(const AbstractSensor& sensor) {
    typeLabel->setText("Type: " + QString::fromStdString(sensor.getType()));
    identifierLabel->setText("ID: " + QString::number(sensor.getIdentifier()));
    descriptionLabel->setText("Description: " + QString::fromStdString(sensor.getDescription()));

    SensorDisplayVisitor visitor(parametersLabel);
    sensor.accept(visitor);
}

void DetailedSensorWidget::updateChart(const std::vector<double>& data) {
    if (data.empty()) {
        return;
    }

    QLineSeries *series = new QLineSeries();
    for (unsigned int i = 0; i < data.size(); ++i) {
        series->append(static_cast<qreal>(i), static_cast<qreal>(data[i]));
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->createDefaultAxes();
    chart->setTitle("Simulation Data");
    chartView->setChart(chart);

    if (data.back() == 0.0) {
        QMessageBox::warning(this, "Athlete Injury", "The athlete has been injured, and data recording has stopped.");
    }
}