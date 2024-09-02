#include "EditSensorDialog.h"
#include "SensorDisplayVisitor.h"
#include "../logic_model/AbstractSensor.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QIntValidator>

EditSensorDialog::EditSensorDialog(AbstractSensor* sensor, ContainerSensorList* containerList, QWidget* parent)
    : QDialog(parent), sensor(sensor), containerList(containerList) { 

    QVBoxLayout* layout = new QVBoxLayout(this);
    setLayout(layout);
    setWindowTitle("Edit Sensor Parameters");

    layout->addWidget(new QLabel("ID:"));
    idEdit = new QLineEdit(QString::number(sensor->getIdentifier()));
    idEdit->setPlaceholderText("up to 5 digits");
    idEdit->setValidator(new QIntValidator(0, 99999, this));
    layout->addWidget(idEdit);

    layout->addWidget(new QLabel("Type:"));
    typeEdit = new QLineEdit(QString::fromStdString(sensor->getType()),this);
    typeEdit->setDisabled(true);
    layout->addWidget(typeEdit);

    layout->addWidget(new QLabel("Description:"));
    descriptionEdit = new QTextEdit(QString::fromStdString(sensor->getDescription()));
    descriptionEdit->setPlaceholderText("up to 200 characters");
    layout->addWidget(descriptionEdit);

    connect(descriptionEdit, &QTextEdit::textChanged, this, [this]() {
    QString text = descriptionEdit->toPlainText();
    if (text.length() > 200) {

        text = text.left(200);
        descriptionEdit->setPlainText(text);

        QTextCursor cursor = descriptionEdit->textCursor();
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        descriptionEdit->setTextCursor(cursor);
    }
    });

    SensorDisplayVisitor visitor(layout, this);
    sensor->acceptForDialog(visitor);

    applyButton = new QPushButton("Apply", this);
    layout->addWidget(applyButton);
    connect(applyButton, &QPushButton::clicked, this, &EditSensorDialog::applyChanges);
}

bool EditSensorDialog::validateUniqueID(unsigned int id) {
    if (sensor->getIdentifier() != id && !containerList->isIDUnique(id)) {
        QMessageBox::warning(this, "Invalid ID", "The ID must be unique.");
        return false;
    }
    return true;
}

void EditSensorDialog::applyChanges() {
    unsigned int id = idEdit->text().toUInt();
    if (!validateUniqueID(id)) {
        return;
    }

    sensor->setIdentifier(id);
    sensor->setDescription(descriptionEdit->toPlainText().toStdString());
    sensor->notifyAllObservers();
    accept();  
}

