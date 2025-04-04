#include "mainwindow.h"
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QTableWidget>
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

    QTableWidget *instructionTable = new QTableWidget(this);
    instructionTable->setColumnCount(3);
    instructionTable->setHorizontalHeaderLabels({"Address", "Instruction", "Pipeline Stage"});
    layout->addWidget(instructionTable);

    QTableWidget *memoryTable = new QTableWidget(this);
    memoryTable->setColumnCount(2);
    memoryTable->setHorizontalHeaderLabels({"Address", "Data"});
    layout->addWidget(memoryTable);

    QTableWidget *cacheTable = new QTableWidget(this);
    cacheTable->setColumnCount(3);
    cacheTable->setHorizontalHeaderLabels({"Cache Address", "Data", "Status"});
    layout->addWidget(cacheTable);

    QPushButton *tickButton = new QPushButton("Tick", this);
    connect(tickButton, &QPushButton::clicked, [this]() {
        // Placeholder for tick functionality
        QMessageBox::information(this, "Tick", "Tick button clicked. Implement the tick logic here.");
    });
    layout->addWidget(tickButton);
}

MainWindow::~MainWindow() {}