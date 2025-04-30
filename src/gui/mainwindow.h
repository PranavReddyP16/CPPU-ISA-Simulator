#include <QTimer>
#include "memory/Memory.h"
#include "memory/Cache.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /* … existing methods … */

    QCheckBox *chkCache;
    QCheckBox *chkPipeline;

    QPushButton *btnRun;
    QPushButton *btnHalt;
    QPushButton *btnStep;
    QPushButton *btnReset;

    /* existing public members unchanged */
    QLineEdit    *cycleCount;
    QTableWidget *registerTable;
    QTableWidget *memoryTable;
    QTableWidget *cacheTable;

    Assembler          *assembler;
    five_stage_pipeline*pipeline;

private:
    Memory *memory;
    Cache  *cache;

    QTimer *runTimer;
    int     cycles = 0;
    int     memRows;

    void actionLoadProgram();
    void tickOnce();
    void refreshGui();
};
