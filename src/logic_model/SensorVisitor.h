#ifndef SENSOR_VISITOR_H
#define SENSOR_VISITOR_H


class SpeedSensor;
class CadenceSensor;
class HeartRateSensor;

class SensorVisitor{
public:
    virtual void visit(const SpeedSensor&) = 0;
    virtual void visit(const CadenceSensor&) = 0;
    virtual void visit(const HeartRateSensor&) = 0;

    virtual void visitForDialog( SpeedSensor&) = 0;
    virtual void visitForDialog( CadenceSensor&) = 0;
    virtual void visitForDialog( HeartRateSensor&) = 0;

    virtual ~SensorVisitor() = default;
};


#endif