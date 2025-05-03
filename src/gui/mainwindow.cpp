#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QLabel>
#include <QHeaderView>
#include <QTabWidget>
#include <QTableWidget>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>
#include <QTimer>

#include "system_constants.h"
#include "memory/utils.h"
#include "memory/Memory.h"
#include "memory/Cache.h"
#include "assembler/assembler.h"
#include "pipeline/five_stage_pipeline.h"

// Helpers to format hex in the tables
static inline QString hex2(uint8_t v)  { return QString("%1").arg(v, 2, 16, QChar('0')); }
static inline QString hex4(uint32_t v) { return QString("%1").arg(v, 4, 16, QChar('0')); }
static inline QString hex8(uint64_t v) { return QString("0x%1").arg(v, 16, 16, QChar('0')); }

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      memory   (new Memory()),
      cache    (new Cache(*memory, ReplacementPolicy::LRU)),
      assembler(new Assembler(memory)),
      pipeline (new five_stage_pipeline(*cache)),
      runTimer (new QTimer(this)),
      cycles   (0)
{
    setWindowTitle("ISA Simulator");

    // ─── Menu Bar ───
    auto *mb = menuBar();
    auto *fileMenu = mb->addMenu("File");
    fileMenu->addAction("Load Program…", this, &MainWindow::actionLoadProgram);
    fileMenu->addAction("Exit", this, &QWidget::close);
    mb->addMenu("Help")->addAction("About", [=] {
        QMessageBox::information(this, "About", "ISA Simulator");
    });

    // ─── Main Layout ───
    auto *cw = new QWidget(this);
    auto *mainLayout = new QVBoxLayout(cw);
    setCentralWidget(cw);
    cw->setLayout(mainLayout);

    // Mode toggles
    auto *modeLayout = new QHBoxLayout;
    chkCache    = new QCheckBox("Cache Enable");
    chkPipeline = new QCheckBox("Pipeline Enable");
    chkCache->setChecked(true);
    chkPipeline->setChecked(true);
    connect(chkCache, &QCheckBox::stateChanged, [this](int state) {
        if (state == Qt::Checked)
            cache->enabled = true;
        else
            cache->enabled = false;
    });
    modeLayout->addWidget(chkCache);
    modeLayout->addWidget(chkPipeline);
    modeLayout->addStretch();
    mainLayout->addLayout(modeLayout);

    // Cycle count header + field
    mainLayout->addWidget(new QLabel("Clock Cycles:", this));
    cycleCount = new QLineEdit("0", this);
    cycleCount->setReadOnly(true);
    mainLayout->addWidget(cycleCount);

    // Control buttons
    auto *ctl = new QHBoxLayout;
    btnRun         = new QPushButton("Run", this);
    btnHalt        = new QPushButton("Halt", this);
    btnStep        = new QPushButton("Step", this);
    btnReset       = new QPushButton("Reset", this);
    btnLoadProgram = new QPushButton("Load Program", this);
    ctl->addWidget(btnRun);
    ctl->addWidget(btnHalt);
    ctl->addWidget(btnStep);
    ctl->addWidget(btnReset);
    ctl->addWidget(btnLoadProgram);
    mainLayout->addLayout(ctl);

    // Tabs: Overview + Pipeline
    tabWidget = new QTabWidget(this);
    mainLayout->addWidget(tabWidget);

    // ── Overview Tab ──
    overviewTab = new QWidget(this);
    {
        auto *ov = new QVBoxLayout(overviewTab);

        // Program listing
        ov->addWidget(new QLabel("Program", this));
        programTable = new QTableWidget(0, 2, this);
        programTable->setHorizontalHeaderLabels({"Addr","Instr"});
        programTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        programTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        ov->addWidget(programTable);

        // Registers
        ov->addWidget(new QLabel("Registers", this));
        registerTable = new QTableWidget(22, 2, this);
        registerTable->setHorizontalHeaderLabels({"Register","Value"});
        registerTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        registerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        // Fill names
        QStringList regs = {"PC","IR","SP","BP","RA","SR"};
        for(int i=0;i<8;++i) regs << QString("GPR[%1]").arg(i);
        for(int i=0;i<8;++i) regs << QString("FPR[%1]").arg(i);
        for(int r=0;r<22;++r) {
            registerTable->setItem(r,0,new QTableWidgetItem(regs[r]));
            registerTable->setItem(r,1,new QTableWidgetItem(""));
        }
        ov->addWidget(registerTable);

        // Memory
        ov->addWidget(new QLabel("Memory", this));
        memRows = std::min(MEMORY_SIZE/16, 256);
        memoryTable = new QTableWidget(memRows, 17, this);
        QStringList memHdr = {"Addr"};
        for(int c=0;c<16;++c) memHdr << QString::number(c,16).toUpper();
        memoryTable->setHorizontalHeaderLabels(memHdr);
        memoryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        // create all items
        for(int r=0;r<memRows;++r)
            for(int c=0;c<17;++c)
                memoryTable->setItem(r,c,new QTableWidgetItem(""));
        ov->addWidget(memoryTable);

        // Cache
        ov->addWidget(new QLabel("Cache", this));
        cacheTable = new QTableWidget(Cache::numSets(), 5, this);
        cacheTable->setHorizontalHeaderLabels({"Set","Tag","Valid","Dirty","Data"});
        cacheTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        cacheTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        for(int r=0;r<Cache::numSets();++r)
            for(int c=0;c<5;++c)
                cacheTable->setItem(r,c,new QTableWidgetItem(""));
        ov->addWidget(cacheTable);

        tabWidget->addTab(overviewTab, "Overview");
    }

    // ── Pipeline Tab ──
    pipelineTab = new QWidget(this);
    {
        auto *pl = new QVBoxLayout(pipelineTab);
        pl->addWidget(new QLabel("Pipeline Stages", this));
        pipelineTable = new QTableWidget(1,5,this);
        pipelineTable->setHorizontalHeaderLabels({"IF","ID","EX","MEM","WB"});
        pipelineTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        pipelineTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
        for(int c=0;c<5;++c)
            pipelineTable->setItem(0,c,new QTableWidgetItem(""));
        pl->addWidget(pipelineTable);
        tabWidget->addTab(pipelineTab, "Pipeline");
    }

    // Fullscreen on start
    showMaximized();

    // ── Signals ──
    connect(btnRun,   &QPushButton::clicked, [=](){
        if (!runTimer->isActive()) runTimer->start(0);
    });
    connect(btnHalt,  &QPushButton::clicked, [=](){
        runTimer->stop();
    });
    connect(btnStep,  &QPushButton::clicked, this, &MainWindow::tickOnce);
    connect(btnReset, &QPushButton::clicked, [=](){
        runTimer->stop();
        memory->reset();
        cache->reset();
        delete pipeline;
        pipeline = new five_stage_pipeline(*cache);
        cycles = 0;
        cycleCount->setText("0");
        refreshGui();
    });
    connect(btnLoadProgram, &QPushButton::clicked, this, &MainWindow::actionLoadProgram);
    connect(runTimer,       &QTimer::timeout,      this, &MainWindow::tickOnce);

    // Initial display
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
    QString fn = QFileDialog::getOpenFileName(this, "Load Program", "", "Assembly (*.s *.asm)");
    if (fn.isEmpty()) return;

    // reset BEFORE loading so we start with clean memory,
    // but preserve the loaded instructions in memory
    memory->reset();
    cache->reset();

    assembler->loadProgram(fn.toStdString());
    const auto &prog = assembler->getProgram();

    programTable->setRowCount(prog.size());
    for(int i=0;i<prog.size();++i){
        programTable->setItem(i,0,new QTableWidgetItem(hex8(i)));
        programTable->setItem(i,1,new QTableWidgetItem(hex8(prog[i])));
    }

    // restart pipeline only
    runTimer->stop();
    delete pipeline;
    pipeline = new five_stage_pipeline(*cache);
    cycles = 0;
    cycleCount->setText("0");
    refreshGui();
}

void MainWindow::tickOnce() {
    pipeline->clock_cycle();
    ++cycles;
    cycleCount->setText(QString::number(cycles));
    refreshGui();

    // auto-stop after HALT drains
    if (pipeline->halted
     && !(pipeline->if_valid||pipeline->id_valid
       ||pipeline->ex_valid||pipeline->mem_valid
       ||pipeline->wb_valid)) {
        runTimer->stop();
    }
}

void MainWindow::refreshGui() {
    // Pipeline registers
    QStringList insts = {
        hex8(pipeline->ifr.instruction),
        hex8(pipeline->idr.instruction),
        hex8(pipeline->exr.instruction),
        hex8(pipeline->memr.instruction),
        hex8(pipeline->wbr.instruction)
    };
    bool valid[5] = {
        pipeline->if_valid,
        pipeline->id_valid,
        pipeline->ex_valid,
        pipeline->mem_valid,
        pipeline->wb_valid
    };
    for(int c=0;c<5;++c)
        pipelineTable->item(0,c)
                     ->setText(valid[c] ? insts[c] : QString());

    // Registers
    auto &R = pipeline->regs;
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

    // Memory view (with centered text!)
    for(int row=0; row<memRows; ++row) {
        uint32_t base = row*16;
        auto *addrItem = memoryTable->item(row, 0);
        addrItem->setText(hex4(base));
        addrItem->setTextAlignment(Qt::AlignCenter);

        for(int col=0; col<16; ++col) {
            uint64_t b = memory->read_data(base+col);
            auto *it = memoryTable->item(row, col+1);
            it->setText(hex8(b));
            it->setTextAlignment(Qt::AlignCenter);
        }
    }
    memoryTable->resizeColumnsToContents();

    // Cache view
    int sets = Cache::numSets();
    cacheTable->setRowCount(sets);
    for(int s=0; s<sets; ++s){
        cacheTable->item(s,0)->setText(QString::number(s));
        cacheTable->item(s,0)->setTextAlignment(Qt::AlignCenter);

        const auto &line = cache->getSet(s)[0];
        cacheTable->item(s,1)->setText(QString::number(line.tag,16).toUpper());
        cacheTable->item(s,1)->setTextAlignment(Qt::AlignCenter);

        cacheTable->item(s,2)->setText(line.valid ? "1":"0");
        cacheTable->item(s,2)->setTextAlignment(Qt::AlignCenter);

        cacheTable->item(s,3)->setText(line.dirty ? "1":"0");
        cacheTable->item(s,3)->setTextAlignment(Qt::AlignCenter);

        QStringList data;
        for(auto d: line.data)
            data << QString::number(mem_dtype_to_int(d));
        cacheTable->item(s,4)->setText(data.join(","));
        cacheTable->item(s,4)->setTextAlignment(Qt::AlignCenter);
    }
}
