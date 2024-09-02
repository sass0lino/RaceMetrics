#ifndef SENSORDISPLAYVISITOR_H
#define SENSORDISPLAYVISITOR_H

#include "../logic_model/SensorVisitor.h"
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QObject>


class SensorDisplayVisitor : public SensorVisitor {
public:
    explicit SensorDisplayVisitor(QLabel* label, QObject* parent = nullptr);
    explicit SensorDisplayVisitor(QVBoxLayout* layout, QObject* parent = nullptr);


    void visit(const SpeedSensor& sensor) override;
    void visit(const CadenceSensor& sensor) override;
    void visit(const HeartRateSensor& sensor) override;

    void visitForDialog(SpeedSensor& sensor) override;
    void visitForDialog(CadenceSensor& sensor) override;
    void visitForDialog(HeartRateSensor& sensor) override;

private:
    QLabel* label;
    QVBoxLayout* layout;
    QObject* parent;
    void connectValidation(QLineEdit* edit, double minValue, double maxValue, const QString& placeholderText);
};

#endif