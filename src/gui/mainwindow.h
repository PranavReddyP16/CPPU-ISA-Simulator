#pragma once

#include <QMainWindow>
#include <QTableWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTabWidget>
#include <QTimer>

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
    QCheckBox            *chkCache;
    QCheckBox            *chkPipeline;
    QCheckBox            *chkMemDelay;

    QPushButton          *btnRun;
    QPushButton          *btnHalt;
    QPushButton          *btnStep;
    QPushButton          *btnReset;
    QPushButton          *btnLoadProgram;

    QLineEdit            *cycleCount;
    QLineEdit            *clockSpeed;

    QTableWidget         *programTable;
    QTableWidget         *pipelineTable;
    QTableWidget         *registerTable;
    QTableWidget         *memoryTable;
    QTableWidget         *cacheTable;

    Memory               *memory;
    Cache                *cache;
    Assembler            *assembler;
    five_stage_pipeline  *pipeline;

    QTimer               *runTimer;
    bool                  breakpoints[MEMORY_SIZE];
    int                   cycles = 0;
    int                   memRows = 0;

private slots:
    void memoryAccessed() { cycles += 100; }
};
