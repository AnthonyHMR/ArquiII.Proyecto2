#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QLabel>
#include <QTableWidget>
#include <vector>
#include "InstructionMemory.h"
#include "ProcessingElement.h"
#include "Cache.h"
#include "BusInterconnect.h"
#include "SharedMemory.h"
#include "StaticPriorityScheme.h"
#include "Compiler.h"
#include "RoundRobinScheme.h"

class MainWindow : public QWidget {
    Q_OBJECT

public:
    explicit MainWindow(SharedMemory& sharedMem, const std::vector<InstructionMemory>& instructionMemories, const std::vector<int>& priorities, const std::vector<ProcessingElement*>& pes, std::vector<Cache*>& caches, Compiler& compiler, BusInterconnect& bus, QWidget *parent = nullptr);

private:
    MainWindow(SharedMemory& sharedMem, const std::vector<InstructionMemory>& instructionMemories, const std::vector<int>& priorities,
               const std::vector<ProcessingElement*>& pes, const std::vector<Cache*>& caches, const Compiler& compiler, BusInterconnect& bus,
               QWidget* parent);
    QVBoxLayout* createColumn1();
    QVBoxLayout* createColumn2();
    QVBoxLayout* createColumn3();
    QVBoxLayout* createColumn4();
    QVBoxLayout* createColumn5();
    QVBoxLayout* createColumn6();

    SharedMemory& sharedMem;
    std::vector<InstructionMemory> instructionMemories;
    std::vector<int> priorities;
    std::vector<ProcessingElement*> pes;
    std::vector<Cache*> caches;
    Compiler compiler;
    BusInterconnect& bus;

    int peValue = 0;
    int schemeValue;
    int indexMem = 0;
    std::vector<int> indexInst = {0, 0, 0, 0};
    std::vector<int> priorityIndex = {2, 0, 1, 3};

    void roundRobinSteppingExecute();
    void staticPrioritySteppingExecute();
    void stepping(int indexMemory, int indexInstruction);

    void updateSystem();
    void clearSystem();

    QPlainTextEdit *textFixed = new QPlainTextEdit(this);


    QPushButton *button1 = new QPushButton("Compile", this);
    QPushButton *button2 = new QPushButton("Execute", this);

    QListWidget *cacheList1 = new QListWidget(this);
    QTableWidget *statsTable1 = new QTableWidget(1, 3, this);
    QListWidget *cacheList2 = new QListWidget(this);
    QTableWidget *statsTable2 = new QTableWidget(1, 3, this);
    QListWidget *cacheList3 = new QListWidget(this);
    QTableWidget *statsTable3 = new QTableWidget(1, 3, this);
    QListWidget *cacheList4 = new QListWidget(this);
    QTableWidget *statsTable4 = new QTableWidget(1, 3, this);

    QListWidget *sharedMemList = new QListWidget(this);
    QTableWidget *statsTable = new QTableWidget(1, 4, this);
};

#endif // MAINWINDOW_H
