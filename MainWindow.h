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

    size_t peValue;
    int schemeValue;
};

#endif // MAINWINDOW_H
