#pragma once

#include <QMainWindow>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTableWidget>
#include <QTimer>
#include <QStringList>

#include "memory/Memory.h"
#include "memory/Cache.h"
#include "assembler/assembler.h"
#include "pipeline/five_stage_pipeline.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void actionLoadProgram();
    void tickOnce();
    void refreshGui();

private:
    // Mode controls
    QCheckBox    *chkCache;
    QCheckBox    *chkPipeline;

    // Control buttons
    QPushButton  *btnRun;
    QPushButton  *btnHalt;
    QPushButton  *btnStep;
    QPushButton  *btnReset;

    // Cycle counter display
    QLineEdit    *cycleCount;

    // State views
    QTableWidget *registerTable;
    QTableWidget *memoryTable;
    QTableWidget *cacheTable;

    // Back-end objects
    Memory               *memory;
    Cache                *cache;
    Assembler            *assembler;
    five_stage_pipeline  *pipeline;

    // Timer for continuous run
    QTimer               *runTimer;

    // Internal counters
    int                   cycles = 0;
    int                   memRows;
};
