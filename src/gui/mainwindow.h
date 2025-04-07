#pragma once

#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setRegisterValue(char* reg, int value);
    void setRegisterValue(char* reg, float value);

private:
    QTableWidget *instructionTable;
    QTableWidget *registerTable;
    QTableWidget *memoryTable;
    QTableWidget *cacheTable;
};