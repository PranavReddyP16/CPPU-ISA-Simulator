#include "mainwindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Set up the main window
    setWindowTitle("ISA Simulator");

    // Create a menu bar
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // Add a File menu
    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // Add a Help menu
    QMenu *helpMenu = menuBar->addMenu("Help");
    QAction *aboutAction = helpMenu->addAction("About");
    connect(aboutAction, &QAction::triggered, [this]() {
        QMessageBox::information(this, "About", "ISA Simulator\nVersion 1.0");
    });

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    centralWidget->setLayout(layout);

    // Horizontal layout for checkboxes
    QHBoxLayout *checkboxLayout = new QHBoxLayout();

        // Cache Enable
        QCheckBox *cacheEnableCheckbox = new QCheckBox("Cache Enable", this);
        cacheEnableCheckbox->setChecked(true);
        checkboxLayout->addWidget(cacheEnableCheckbox);
        
        // Pipeline Enable
        QCheckBox *pipelineEnableCheckbox = new QCheckBox("Pipeline Enable", this);
        pipelineEnableCheckbox->setChecked(false);
        checkboxLayout->addWidget(pipelineEnableCheckbox);
        
    checkboxLayout->addStretch();
    checkboxLayout->setAlignment(Qt::AlignRight);
    layout->addLayout(checkboxLayout);

    // Add a text field to display the current cycle count
    cycleCount = new QLineEdit(this);
    cycleCount->setReadOnly(true);
    cycleCount->setText("0");
    layout->addWidget(cycleCount);

    // Horizontal layout for control buttons

    QHBoxLayout *controlButtonsLayout = new QHBoxLayout();
        // Add Run button
        QPushButton *runButton = new QPushButton("Run", this);
        connect(runButton, &QPushButton::clicked, [this]() {
            // Placeholder for run functionality
            QMessageBox::information(this, "Run", "Run button clicked. Implement the run logic here.");
        });
        controlButtonsLayout->addWidget(runButton);

        // Add Pause button
        QPushButton *pauseButton = new QPushButton("Pause", this);
        connect(pauseButton, &QPushButton::clicked, [this]() {
            // Placeholder for pause functionality
            QMessageBox::information(this, "Pause", "Pause button clicked. Implement the pause logic here.");
        });
        controlButtonsLayout->addWidget(pauseButton);

        // Add Step button
        QPushButton *stepButton = new QPushButton("Step", this);
        connect(stepButton, &QPushButton::clicked, [this]() {
            // Placeholder for step functionality
            QMessageBox::information(this, "Step", "Step button clicked. Implement the step logic here.");
        });
        controlButtonsLayout->addWidget(stepButton);

        // Add Restart button
        QPushButton *restartButton = new QPushButton("Reset", this);
        connect(restartButton, &QPushButton::clicked, [this]() {
            // Placeholder for restart functionality
            QMessageBox::information(this, "Restart", "Restart button clicked. Implement the restart logic here.");
        });
        controlButtonsLayout->addWidget(restartButton);

    layout->addLayout(controlButtonsLayout);

    

    pipelineTable = new QTableWidget(this);
    pipelineTable->setColumnCount(5);
    pipelineTable->setHorizontalHeaderLabels({"fetch", "decode", "execute", "mem_stage", "writeback"});
    pipelineTable->setRowCount(1);
    layout->addWidget(pipelineTable);

    registerTable = new QTableWidget(this);
    registerTable->setColumnCount(2);
    registerTable->setHorizontalHeaderLabels({"Register", "Value"});
    registerTable->setRowCount(22);
    registerTable->setItem(0, 0, new QTableWidgetItem("PC"));
    registerTable->setItem(0, 1, new QTableWidgetItem("0"));
    registerTable->setItem(1, 0, new QTableWidgetItem("IR"));
    registerTable->setItem(1, 1, new QTableWidgetItem("0"));
    registerTable->setItem(2, 0, new QTableWidgetItem("SP"));
    registerTable->setItem(2, 1, new QTableWidgetItem("0"));
    registerTable->setItem(3, 0, new QTableWidgetItem("BP"));
    registerTable->setItem(3, 1, new QTableWidgetItem("0"));
    registerTable->setItem(4, 0, new QTableWidgetItem("RA"));
    registerTable->setItem(4, 1, new QTableWidgetItem("0"));
    registerTable->setItem(5, 0, new QTableWidgetItem("SR"));
    registerTable->setItem(5, 1, new QTableWidgetItem("0"));
    registerTable->setItem(6, 0, new QTableWidgetItem("GPR[0]"));
    registerTable->setItem(6, 1, new QTableWidgetItem("0"));
    registerTable->setItem(7, 0, new QTableWidgetItem("GPR[1]"));
    registerTable->setItem(7, 1, new QTableWidgetItem("0"));
    registerTable->setItem(8, 0, new QTableWidgetItem("GPR[2]"));
    registerTable->setItem(8, 1, new QTableWidgetItem("0"));
    registerTable->setItem(9, 0, new QTableWidgetItem("GPR[3]"));
    registerTable->setItem(9, 1, new QTableWidgetItem("0"));
    registerTable->setItem(10, 0, new QTableWidgetItem("GPR[4]"));
    registerTable->setItem(10, 1, new QTableWidgetItem("0"));
    registerTable->setItem(11, 0, new QTableWidgetItem("GPR[5]"));
    registerTable->setItem(11, 1, new QTableWidgetItem("0"));
    registerTable->setItem(12, 0, new QTableWidgetItem("GPR[6]"));
    registerTable->setItem(12, 1, new QTableWidgetItem("0"));
    registerTable->setItem(13, 0, new QTableWidgetItem("GPR[7]"));
    registerTable->setItem(13, 1, new QTableWidgetItem("0"));
    registerTable->setItem(14, 0, new QTableWidgetItem("FPR[0]"));
    registerTable->setItem(14, 1, new QTableWidgetItem("0.0"));
    registerTable->setItem(15, 0, new QTableWidgetItem("FPR[1]"));
    registerTable->setItem(15, 1, new QTableWidgetItem("0.0"));
    registerTable->setItem(16, 0, new QTableWidgetItem("FPR[2]"));
    registerTable->setItem(16, 1, new QTableWidgetItem("0.0"));
    registerTable->setItem(17, 0, new QTableWidgetItem("FPR[3]"));
    registerTable->setItem(17, 1, new QTableWidgetItem("0.0"));
    registerTable->setItem(18, 0, new QTableWidgetItem("FPR[4]"));
    registerTable->setItem(18, 1, new QTableWidgetItem("0.0"));
    registerTable->setItem(19, 0, new QTableWidgetItem("FPR[5]"));
    registerTable->setItem(19, 1, new QTableWidgetItem("0.0"));
    registerTable->setItem(20, 0, new QTableWidgetItem("FPR[6]"));
    registerTable->setItem(20, 1, new QTableWidgetItem("0.0"));
    registerTable->setItem(21, 0, new QTableWidgetItem("FPR[7]"));
    registerTable->setItem(21, 1, new QTableWidgetItem("0.0"));
    layout->addWidget(registerTable);

    memoryTable = new QTableWidget(this);
    memoryTable->setColumnCount(2);
    memoryTable->setHorizontalHeaderLabels({"Address", "Data"});
    layout->addWidget(memoryTable);

    cacheTable = new QTableWidget(this);
    cacheTable->setColumnCount(3);
    cacheTable->setHorizontalHeaderLabels({"Set", "Tag", "Block Data"});
    layout->addWidget(cacheTable);

    QPushButton *tickButton = new QPushButton("Tick", this);
    connect(tickButton, &QPushButton::clicked, [this]() {
        // Placeholder for tick functionality
        QMessageBox::information(this, "Tick", "Tick button clicked. Implement the tick logic here.");
    });
    layout->addWidget(tickButton);
}

MainWindow::~MainWindow() {}

void MainWindow::setRegisterValue(char* reg, int value) {
    for (int i = 0; i < registerTable->rowCount(); ++i) {
        QTableWidgetItem *item = registerTable->item(i, 0);
        if (item && QString(item->text()).compare(QString(reg)) == 0) {
            registerTable->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(value)));
            return;
        }
    }
    // If we reach here, the register was not found, you may want to handle this case
    QMessageBox::warning(this, "Error", QString("Register %1 not found.").arg(QString(reg)));
    return;
}

void MainWindow::setRegisterValue(char* reg, float value) {
    for (int i = 0; i < registerTable->rowCount(); ++i) {
        QTableWidgetItem *item = registerTable->item(i, 0);
        if (item && QString(item->text()).compare(QString(reg)) == 0) {
            registerTable->setItem(i, 1, new QTableWidgetItem(QString("%1").arg(value)));
            return;
        }
    }
    // If we reach here, the register was not found, you may want to handle this case
    QMessageBox::warning(this, "Error", QString("Register %1 not found.").arg(QString(reg)));
    return;
}