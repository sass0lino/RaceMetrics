#include "SensorWidget.h"

SensorWidget::SensorWidget(AbstractSensor* sensor, QWidget *parent)
    : QWidget(parent), sensor(sensor) {

    frame = new QFrame(this);
    frame->setFrameShape(QFrame::Panel);
    frame->setLineWidth(2);
    frame->setFrameShadow(QFrame::Raised);

    typeLabel = new QLabel("Type: " + QString::fromStdString(sensor->getType()), this);
    identifierLabel = new QLabel("ID: " + QString::number(sensor->getIdentifier()), this);
    descriptionLabel = new QLabel("Description: " + QString::fromStdString(sensor->getDescription()), this);
    descriptionLabel->setWordWrap(true);
    descriptionLabel->setMaximumHeight(descriptionLabel->fontMetrics().height() * 1.5);
    descriptionLabel->setToolTip("<p style='white-space: pre-wrap;'>" + descriptionLabel->text() + "</p>");

    QString type = QString::fromStdString(sensor->getType()).replace(" ", "_");
    QString iconPath = ":/assets/" + type + "_icon.png";
    iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(iconPath).scaled(64, 64, Qt::KeepAspectRatio));
    iconLabel->setFixedWidth(64);
    iconLabel->setAlignment(Qt::AlignCenter);

    infoLayout = new QHBoxLayout;
    infoLayout->addWidget(iconLabel);

    QVBoxLayout *textLayout = new QVBoxLayout;
    textLayout->setAlignment(Qt::AlignLeft);
    textLayout->addWidget(typeLabel);
    textLayout->addWidget(identifierLabel);
    textLayout->addWidget(descriptionLabel);

    infoLayout->addLayout(textLayout);
    frame->setLayout(infoLayout);

    mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(frame);

    setLayout(mainLayout);

    sensor->registerObserver(this);
}

SensorWidget::~SensorWidget() {
    sensor->unregisterObserver(this);
}

void SensorWidget::mousePressEvent(QMouseEvent *event) {
    emit clicked(this);
    QWidget::mousePressEvent(event);
}

void SensorWidget::notify(const AbstractSensor& sensor) {
    typeLabel->setText("Type: " + QString::fromStdString(sensor.getType()));
    identifierLabel->setText("ID: " + QString::number(sensor.getIdentifier()));
    descriptionLabel->setText("Description: " + QString::fromStdString(sensor.getDescription()));
}

AbstractSensor* SensorWidget::getSensor() const {
    return sensor;
}