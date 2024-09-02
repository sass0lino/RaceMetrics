#ifndef SENSORWIDGET_H
#define SENSORWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include "../logic_model/AbstractSensor.h"
#include "../logic_model/SensorObserver.h"

class SensorWidget : public QWidget, public SensorObserver {
    Q_OBJECT

public:
    explicit SensorWidget(AbstractSensor* sensor, QWidget *parent = nullptr);
    ~SensorWidget();

    void notify(const AbstractSensor&) override;
    AbstractSensor* getSensor() const;

signals:
    void clicked(SensorWidget* widget);

protected:
    void mousePressEvent(QMouseEvent *event) override;

private:
    AbstractSensor* sensor;
    QLabel *typeLabel;
    QLabel *identifierLabel;
    QLabel *descriptionLabel;
    QLabel *iconLabel;
    QVBoxLayout *mainLayout;
    QHBoxLayout *infoLayout;
    QFrame *frame;
};

#endif