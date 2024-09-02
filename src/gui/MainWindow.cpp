#include "MainWindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), containerList(new ContainerSensorList), sensorPersistence(new SensorPersistence), isModified(false) {

    setWindowTitle("RaceMetrics");
    centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);

    leftLayout = new QVBoxLayout;
    rightLayout = new QVBoxLayout;

    searchBar = new QLineEdit(this);
    searchBar->setPlaceholderText("Search by ID or Description...");
    connect(searchBar, &QLineEdit::textChanged, this, &MainWindow::updateSearchResults);

    clearSearchButton = new QPushButton(this);
    clearSearchButton->setIcon(QIcon(":assets/clear_search_bar_icon.svg"));
    clearSearchButton->setToolTip("Clear search bar");
    clearSearchButton->setFixedSize(26, 26); 
    connect(clearSearchButton, &QPushButton::clicked, this, &MainWindow::clearSearchBar);

    QHBoxLayout *searchLayout = new QHBoxLayout;
    searchLayout->addWidget(searchBar);
    searchLayout->addWidget(clearSearchButton);
    leftLayout->addLayout(searchLayout);

    QWidget* sensorListContainer = new QWidget(this);
    sensorListLayout = new QVBoxLayout(sensorListContainer);
    sensorListContainer->setLayout(sensorListLayout);
    sensorListLayout->setAlignment(Qt::AlignTop);

    scrollArea = new QScrollArea(this);
    scrollArea->setWidget(sensorListContainer);
    scrollArea->setWidgetResizable(true);
    leftLayout->addWidget(scrollArea);

    createSensorButton = new QPushButton("Add Sensor", this);
    leftLayout->addWidget(createSensorButton);
    connect(createSensorButton, &QPushButton::clicked, this, &MainWindow::createSensor);

    mainLayout->addLayout(leftLayout, 1);
    mainLayout->addLayout(rightLayout, 2);

    setCentralWidget(centralWidget);
    resize(1200, 600);
    setupMenuBar();
    updateWindowTitle();
}

MainWindow::~MainWindow() {
    delete containerList;
    delete sensorPersistence;
}

void MainWindow::addSensorWidget(AbstractSensor* sensor) {
    SensorWidget* sensorWidget = new SensorWidget(sensor, this);
    connect(sensorWidget, &SensorWidget::clicked, this, &MainWindow::showDetailedSensor);
    sensorListLayout->addWidget(sensorWidget);
}

void MainWindow::createSensor() {
    SensorDialog dialog(containerList, this);
    if (dialog.exec() == QDialog::Accepted) {
        AbstractSensor* newSensor = containerList->getSensorById(dialog.getIdentifier());
        if (newSensor) {
            addSensorWidget(newSensor);
            isModified = true;
            updateWindowTitle();
        }
    }
}

void MainWindow::deleteSensor(AbstractSensor* sensor) {
    QLayoutItem *child;
    while ((child = rightLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    while ((child = sensorListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    containerList->removeSensor(sensor->getIdentifier());


    std::vector<AbstractSensor*> sensors = containerList->getSensors();
    for (std::size_t i = 0; i < sensors.size(); ++i) {
        addSensorWidget(sensors[i]);
    }

    isModified = true;
    updateWindowTitle();
}

void MainWindow::updateSearchResults(const QString& text) {
    QLayoutItem *child;
    while ((child = sensorListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    std::vector<AbstractSensor*> sensors = containerList->getSensors();
    for (AbstractSensor* sensor : sensors) {
        if (QString::number(sensor->getIdentifier()).contains(text, Qt::CaseInsensitive) ||
            QString::fromStdString(sensor->getDescription()).contains(text, Qt::CaseInsensitive)) {
            addSensorWidget(sensor);
        }
    }
}

void MainWindow::clearSearchBar() {
    searchBar->clear();
    updateSearchResults("");
};

void MainWindow::showDetailedSensor(SensorWidget* widget) {
    QLayoutItem *child;
    while ((child = rightLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }

    DetailedSensorWidget* detailedWidget = new DetailedSensorWidget(widget->getSensor(), containerList, this);
    connect(detailedWidget, &DetailedSensorWidget::sensorDeleted, this, &MainWindow::deleteSensor);
    rightLayout->addWidget(detailedWidget);
    rightLayout->update();
}

void MainWindow::setupMenuBar() {
    QToolBar *toolBar = addToolBar("File Operations");
    toolBar->setMovable(false);

    QAction *openAction = new QAction(QIcon(":assets/open_file_icon.svg"), "Open File", this);
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);
    toolBar->addAction(openAction);

    QAction *saveAction = new QAction(QIcon(":assets/save_icon.svg"), "Save", this);
    connect(saveAction, &QAction::triggered, this, &MainWindow::save);
    toolBar->addAction(saveAction);

    QAction *saveAsAction = new QAction(QIcon(":assets/save_as_icon.svg"), "Save As", this);
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveAs);
    toolBar->addAction(saveAsAction);

    QAction *closeAction = new QAction(QIcon(":assets/close_file_icon.svg"), "Close File", this);
    connect(closeAction, &QAction::triggered, this, &MainWindow::closeFile);
    toolBar->addAction(closeAction);
};

void MainWindow::updateWindowTitle() {
    QString title = "RaceMetrics";
    if (!currentFileName.isEmpty()) {
        title += " - " + currentFileName;
    }
    setWindowTitle(title);
};

void MainWindow::clearSensors() {
    containerList->clearSensors();
    QLayoutItem *child;
    while ((child = rightLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            child->widget()->deleteLater();
        }
        delete child;
    }
    isModified = false;
    updateWindowTitle();
};

void MainWindow::saveAs() {
    QString fileName = QFileDialog::getSaveFileName(this, "Save As", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        if (sensorPersistence->saveToFile(fileName, containerList)) {
            currentFileName = fileName;
            isModified = false;
            updateWindowTitle();
        }
    }
};

void MainWindow::save() {
    if (currentFileName.isEmpty()) {
        saveAs();
    } else {
        if (sensorPersistence->saveToFile(currentFileName, containerList)) {
            isModified = false;
            updateWindowTitle();
        }
    }
};

void MainWindow::openFile() {
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "JSON Files (*.json)");
    if (!fileName.isEmpty()) {
        if (sensorPersistence->loadFromFile(fileName, containerList)) {
            currentFileName = fileName;
            isModified = false;
            updateWindowTitle();

            // Svuota il layout dei SensorWidget
            QLayoutItem *child;
            while ((child = sensorListLayout->takeAt(0)) != nullptr) {
                if (child->widget()) {
                    child->widget()->deleteLater();
                }
                delete child;
            }

            std::vector<AbstractSensor*> sensors = containerList->getSensors();
            for (std::size_t i = 0; i < sensors.size(); ++i) {
                addSensorWidget(sensors[i]);
            }
        } else {
            QMessageBox::warning(this, "Open Error", "Could not load the file.");
        }
    }
};

void MainWindow::closeFile() {
    if (isModified) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "Save Changes", "Do you want to save any possible changes?",
                                                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            save();
        } else if (reply == QMessageBox::Cancel) {
            return;
        }
    }
    clearSensors();
    currentFileName.clear();
    updateWindowTitle();
};