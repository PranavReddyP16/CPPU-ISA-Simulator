#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

#include "system_constants.h"
#include "memory/Memory.h"
#include "memory/utils.h"
#include "assembler/assembler.h"
#include "pipeline/five_stage_pipeline.h"

// Helpers for hex formatting
static inline QString hex2(uint8_t  v) { return QString("%1").arg(v, 2, 16, QChar('0')).toUpper(); }
static inline QString hex4(uint32_t v) { return QString("%1").arg(v, 4, 16, QChar('0')).toUpper(); }
static inline QString hex8(uint32_t v) { return QString("0x%1").arg(v, 8, 16, QChar('0')).toUpper(); }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      memory  (new Memory()),
      cache   (new Cache(*memory, ReplacementPolicy::LRU)),
      assembler(new Assembler(memory)),
      pipeline(new five_stage_pipeline(*cache)),
      runTimer(new QTimer(this))
{
    setWindowTitle("ISA Simulator");

    // ─── Menus ───
    auto *mb = menuBar();
    auto *file = mb->addMenu("File");
    file->addAction("Load Program…", this, &MainWindow::actionLoadProgram);
    file->addAction("Exit", this, &QWidget::close);
    mb->addMenu("Help")->addAction("About", [=] {
        QMessageBox::information(this, "About", "ISA Simulator GUI");
    });

    // ─── Layout ───
    auto *cw = new QWidget(this);
    auto *v = new QVBoxLayout(cw);
    cw->setLayout(v);
    setCentralWidget(cw);

    // Mode checkboxes
    auto *hMode = new QHBoxLayout;
    chkCache    = new QCheckBox("Cache Enable");
    chkPipeline = new QCheckBox("Pipeline Enable");
    chkCache->setChecked(true);
    chkPipeline->setChecked(true);
    hMode->addWidget(chkCache);
    hMode->addWidget(chkPipeline);
    hMode->addStretch();
    v->addLayout(hMode);

    // Cycle count
    cycleCount = new QLineEdit("0");
    cycleCount->setReadOnly(true);
    v->addWidget(cycleCount);

    // Control buttons
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

    // Register table
    registerTable = new QTableWidget(22, 2, this);
    registerTable->setHorizontalHeaderLabels({"Register","Value"});
    QStringList regs = {"PC","IR","SP","BP","RA","SR"};
    for (int i=0;i<8;++i) regs << QString("GPR[%1]").arg(i);
    for (int i=0;i<8;++i) regs << QString("FPR[%1]").arg(i);
    for (int r=0;r<22;++r)
        registerTable->setItem(r,0,new QTableWidgetItem(regs[r]));
    registerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v->addWidget(registerTable);
    // Initialize column 1 items to avoid nullptr in refreshGui()
    for(int r=0; r<registerTable->rowCount(); ++r)
        registerTable->setItem(r, 1, new QTableWidgetItem(""));

    // Memory table (first 256 rows)
    memRows = MEMORY_SIZE/16;
    memoryTable = new QTableWidget(memRows,17,this);
    QStringList hdr{"Addr"};
    for(int c=0;c<16;++c) hdr << QString::number(c,16).toUpper();
    memoryTable->setHorizontalHeaderLabels(hdr);
    memoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v->addWidget(memoryTable);
    // Initialize all cells
    for(int row=0; row<memoryTable->rowCount(); ++row)
        for(int col=0; col<memoryTable->columnCount(); ++col)
            memoryTable->setItem(row, col, new QTableWidgetItem(""));

    // Cache table (one row per set)
    cacheTable = new QTableWidget(Cache::numSets(),3,this);
    cacheTable->setHorizontalHeaderLabels({"Set","Tag","Data"});
    cacheTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    v->addWidget(cacheTable);
    // Initialize all cells
    for(int row=0; row<cacheTable->rowCount(); ++row)
        for(int col=0; col<cacheTable->columnCount(); ++col)
            cacheTable->setItem(row, col, new QTableWidgetItem(""));

    // ─── Signals ───
    connect(btnRun,   &QPushButton::clicked, [=]{ if(!runTimer->isActive()) runTimer->start(0); });
    connect(btnHalt,  &QPushButton::clicked, [=]{ runTimer->stop(); });
    connect(btnStep,  &QPushButton::clicked, this, &MainWindow::tickOnce);
    connect(btnReset, &QPushButton::clicked, [=]{
        runTimer->stop();
        delete pipeline;
        pipeline = new five_stage_pipeline(*cache);
        cycles = 0;
        cycleCount->setText("0");
        refreshGui();
    });
    connect(runTimer, &QTimer::timeout, this, &MainWindow::tickOnce);

    refreshGui();
}

MainWindow::~MainWindow() {
    delete pipeline;
    delete assembler;
    delete cache;
    delete memory;
    exit(0);
}

void MainWindow::actionLoadProgram() {
    QString fn = QFileDialog::getOpenFileName(this,"Load Program","","Assembly (*.s *.asm)");
    if(fn.isEmpty()) return;
    assembler->loadProgram(fn.toStdString());
    delete pipeline;
    pipeline = new five_stage_pipeline(*cache);
    cycles = 0;
    cycleCount->setText("0");
    refreshGui();
}

void MainWindow::tickOnce() {
    if(pipeline->halted) { runTimer->stop(); return; }
    pipeline->clock_cycle();
    ++cycles;
    cycleCount->setText(QString::number(cycles));
    refreshGui();
}

void MainWindow::refreshGui() {
    // Registers
    auto &R = pipeline->registers;
    QString vals[22] = {
        hex8(R.PC), hex8(R.IR), hex8(R.SP), hex8(R.BP),
        hex8(R.RA), hex8(R.SR),
        hex8(R.GPR[0]), hex8(R.GPR[1]), hex8(R.GPR[2]), hex8(R.GPR[3]),
        hex8(R.GPR[4]), hex8(R.GPR[5]), hex8(R.GPR[6]), hex8(R.GPR[7]),
        QString::number(R.FPR[0]), QString::number(R.FPR[1]),
        QString::number(R.FPR[2]), QString::number(R.FPR[3]),
        QString::number(R.FPR[4]), QString::number(R.FPR[5]),
        QString::number(R.FPR[6]), QString::number(R.FPR[7])
    };
    for(int r=0;r<22;++r)
        registerTable->item(r,1)->setText(vals[r]);

    // Memory
    for(int row=0;row<memRows;++row){
        uint32_t base = row*16;
        memoryTable->item(row,0)->setText(hex4(base));
        for(int col=0;col<16;++col){
            uint8_t b = static_cast<uint8_t>(memory->read_data(base+col)&0xFF);
            memoryTable->item(row,col+1)->setText(hex2(b));
        }
    }

    // Cache
    int sets = Cache::numSets();
    cacheTable->setRowCount(sets);
    for(int s=0;s<sets;++s){
        cacheTable->item(s,0)->setText(QString::number(s));
        const auto &cs = cache->getSet(s);
        QString tag = cs[0].valid ?
            QString::number(cs[0].tag,16).toUpper() : "-";
        cacheTable->item(s,1)->setText(tag);

        QStringList elems;
        for(auto datum: cs[0].data)
            elems << QString::number(mem_dtype_to_int(datum));
        cacheTable->item(s,2)->setText(elems.join(","));
    }
}
