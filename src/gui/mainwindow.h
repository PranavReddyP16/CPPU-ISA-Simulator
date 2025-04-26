#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QTableWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setRegisterValue(char* reg, int value);
    void setRegisterValue(char* reg, float value);

    QLineEdit *cycleCount;
    QTableWidget *pipelineTable;
    QTableWidget *registerTable;
    QTableWidget *memoryTable;
    QTableWidget *cacheTable;
};