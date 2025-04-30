#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QFileDialog>
#include <QScrollBar>
#include <QTimer>

#include "system_constants.h"
#include "memory/Memory.h"
#include "memory/Cache.h"
#include "assembler/assembler.h"
#include "pipeline/five_stage_pipeline.h"

// ────────────────────────────────────────────────────────────────────────────────
//  Helper: hex-format helpers
// ────────────────────────────────────────────────────────────────────────────────
static inline QString hex8  (uint32_t v) { return QString("0x%1").arg(v, 8, 16, QChar('0')).toUpper(); }
static inline QString hex2  (uint8_t  v) { return QString("%1").arg(v, 2, 16, QChar('0')).toUpper();  }
static inline QString hexAdr(uint32_t v) { return QString("%1").arg(v, 4, 16, QChar('0')).toUpper();  }

// ────────────────────────────────────────────────────────────────────────────────
//  Constructor
// ────────────────────────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      /* back-end objects */
      memory   (new Memory(MEMORY_SIZE)),
      cache    (new Cache(*memory, ReplacementPolicy::LRU, this)),
      assembler(new Assembler(memory)),
      pipeline (new five_stage_pipeline(*cache, this)),
      runTimer (new QTimer(this))
{
    setWindowTitle("ISA Simulator");

    // ───────── menus ─────────
    auto *mb = menuBar();
    auto *file = mb->addMenu("File");
    auto *actLoad = file->addAction("Load Program…");
    auto *actQuit = file->addAction("Quit");
    connect(actQuit, &QAction::triggered, this, &QWidget::close);

    auto *help = mb->addMenu("Help");
    help->addAction("About", [this]{
        QMessageBox::information(this,"About","ISA Simulator demo – GUI core");
    });

    // ───────── central layout ─────────
    auto *cw = new QWidget(this);
    auto *v  = new QVBoxLayout(cw);
    cw->setLayout(v);
    setCentralWidget(cw);

    // ─── top-row check-boxes ───
    auto *hMode = new QHBoxLayout;
    chkCache    = new QCheckBox("Cache Enable");
    chkPipeline = new QCheckBox("Pipeline Enable");
    chkCache   ->setChecked(true);
    chkPipeline->setChecked(true);
    hMode->addWidget(chkCache);
    hMode->addWidget(chkPipeline);
    hMode->addStretch();
    v->addLayout(hMode);

    // ─── cycle counter ───
    cycleCount = new QLineEdit("0");
    cycleCount->setReadOnly(true);
    v->addWidget(cycleCount);

    // ─── control buttons ───
    auto *hCtl = new QHBoxLayout;
    btnRun   = new QPushButton("Run");
    btnHalt  = new QPushButton("Halt");
    btnStep  = new QPushButton("Step");
    btnReset = new QPushButton("Reset");
    hCtl->addWidget(btnRun);
    hCtl->addWidget(btnHalt);
    hCtl->addWidget(btnStep);
    hCtl->addWidget(btnReset);
    v->addLayout(hCtl);

    // ─── register table ───
    registerTable = new QTableWidget(this);
    registerTable->setColumnCount(2);
    registerTable->setHorizontalHeaderLabels({"Register", "Value"});
    registerTable->setRowCount(22);
    const char* rowNames[22] = {
        "PC","IR","SP","BP","RA","SR",
        "GPR[0]","GPR[1]","GPR[2]","GPR[3]","GPR[4]","GPR[5]","GPR[6]","GPR[7]",
        "FPR[0]","FPR[1]","FPR[2]","FPR[3]","FPR[4]","FPR[5]","FPR[6]","FPR[7]"
    };
    for (int r=0;r<22;++r){
        registerTable->setItem(r,0,new QTableWidgetItem(rowNames[r]));
        registerTable->setItem(r,1,new QTableWidgetItem("0"));
    }
    registerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v->addWidget(registerTable);

    // ─── memory table (show first 256 rows = 4 KiB max) ───
    memoryTable = new QTableWidget(this);
    memRows = std::min(MEMORY_SIZE/16, 256);
    memoryTable->setRowCount(memRows);
    memoryTable->setColumnCount(17);
    QStringList hdr{"Addr"};
    for(char c='0';c<='9';++c) hdr<<QString(c);
    for(char c='A';c<='F';++c) hdr<<QString(c);
    memoryTable->setHorizontalHeaderLabels(hdr);
    memoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v->addWidget(memoryTable);

    // ─── cache summary ───
    cacheTable = new QTableWidget(this);
    cacheTable->setColumnCount(3);
    cacheTable->setHorizontalHeaderLabels({"Set","Tag","Block Data"});
    cacheTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v->addWidget(cacheTable);

    // ───────── actions wiring ─────────
    connect(actLoad,&QAction::triggered,this,&MainWindow::actionLoadProgram);

    connect(btnRun ,&QPushButton::clicked,[this]{ if(!runTimer->isActive()) runTimer->start(0); });
    connect(btnHalt,&QPushButton::clicked,[this]{ runTimer->stop(); });
    connect(btnStep,&QPushButton::clicked,this,&MainWindow::tickOnce);
    connect(btnReset,&QPushButton::clicked,[this]{
        runTimer->stop();
        *pipeline = five_stage_pipeline(*cache,this);   // re-construct
        cycles = 0;
        cycleCount->setText("0");
        refreshGui();
    });

    connect(runTimer,&QTimer::timeout,this,&MainWindow::tickOnce);

    refreshGui();
}

// ────────────────────────────────────────────────────────────────────────────────
//  Destructor  – Qt cleans children, no exit(0)
// ────────────────────────────────────────────────────────────────────────────────
MainWindow::~MainWindow() = default;

// ────────────────────────────────────────────────────────────────────────────────
//  Load program action
// ────────────────────────────────────────────────────────────────────────────────
void MainWindow::actionLoadProgram()
{
    QString fname = QFileDialog::getOpenFileName(this,"Load Program","","Assembly (*.s *.asm);;All (*)");
    if(fname.isEmpty()) return;

    assembler->loadProgram(fname.toStdString());
    /* assembler already wrote into Memory – PC = 0 */
    *pipeline = five_stage_pipeline(*cache,this);   // fresh pipeline
    cycles = 0;
    cycleCount->setText("0");
    refreshGui();
}

// ────────────────────────────────────────────────────────────────────────────────
//  One simulator cycle
// ────────────────────────────────────────────────────────────────────────────────
void MainWindow::tickOnce()
{
    if(pipeline->halted){
        runTimer->stop();
        return;
    }
    pipeline->clock_cycle();
    ++cycles;
    cycleCount->setText(QString::number(cycles));
    refreshGui();
}

// ────────────────────────────────────────────────────────────────────────────────
//  GUI refresh – registers, memory window, cache
// ────────────────────────────────────────────────────────────────────────────────
void MainWindow::refreshGui()
{
    /* ─ registers ─ */
    const Register& R = pipeline->registers;
    QString regVals[22] = {
        hex8(R.PC), hex8(R.IR), hex8(R.SP), hex8(R.BP), hex8(R.RA), hex8(R.SR),
        hex8(R.GPR[0]), hex8(R.GPR[1]), hex8(R.GPR[2]), hex8(R.GPR[3]),
        hex8(R.GPR[4]), hex8(R.GPR[5]), hex8(R.GPR[6]), hex8(R.GPR[7]),
        QString::number(R.FPR[0]), QString::number(R.FPR[1]), QString::number(R.FPR[2]), QString::number(R.FPR[3]),
        QString::number(R.FPR[4]), QString::number(R.FPR[5]), QString::number(R.FPR[6]), QString::number(R.FPR[7])
    };
    for(int r=0;r<22;++r) registerTable->item(r,1)->setText(regVals[r]);

    /* ─ memory window (first memRows rows) ─ */
    for(int row=0; row<memRows; ++row){
        uint32_t base = row*16;
        if(!memoryTable->item(row,0))
            memoryTable->setItem(row,0,new QTableWidgetItem);
        memoryTable->item(row,0)->setText(hexAdr(base));
        for(int col=0;col<16;++col){
            uint8_t byte = static_cast<uint8_t>( cache->read_data(base+col) & 0xFF );
            if(!memoryTable->item(row,col+1))
                memoryTable->setItem(row,col+1,new QTableWidgetItem);
            memoryTable->item(row,col+1)->setText(hex2(byte));
        }
    }

    /* ─ cache view ─ */
    cacheTable->setRowCount(CACHE_NUM_SETS);
    for(int s=0;s<CACHE_NUM_SETS;++s){
        if(!cacheTable->item(s,0)) cacheTable->setItem(s,0,new QTableWidgetItem);
        cacheTable->item(s,0)->setText(QString::number(s));

        if(!cacheTable->item(s,1)) cacheTable->setItem(s,1,new QTableWidgetItem);
        cacheTable->item(s,1)->setText(QString::number(cache->cache_sets[s].tag,16));

        if(!cacheTable->item(s,2)) cacheTable->setItem(s,2,new QTableWidgetItem);
        cacheTable->item(s,2)->setText("…");     // replace with block-dump if desired
    }
}