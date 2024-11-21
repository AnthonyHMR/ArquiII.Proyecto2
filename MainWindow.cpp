#include "MainWindow.h"
#include <QApplication>
#include <thread>
#include <iostream>
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(
    SharedMemory& sharedMem,
    const std::vector<InstructionMemory>& instructionMemories,
    const std::vector<int>& priorities,
    const std::vector<ProcessingElement*>& pes,
    std::vector<Cache*>& caches,
    Compiler& compiler,
    BusInterconnect& bus,
    QWidget* parent
    )
    : sharedMem(sharedMem), instructionMemories(instructionMemories), priorities(priorities), pes(pes), caches(caches), compiler(compiler), bus(bus), QWidget(parent) {

    auto *mainLayout = new QHBoxLayout(this);

    // Crear columnas
    mainLayout->addLayout(createColumn1());
    mainLayout->addLayout(createColumn2());
    mainLayout->addLayout(createColumn3());
    mainLayout->addLayout(createColumn4());
    mainLayout->addLayout(createColumn5());
    mainLayout->addLayout(createColumn6());

    updateSystem();

    setLayout(mainLayout);
    setWindowTitle("Computer Architecture II, Project 2");
    resize(1100, 500);
}

QVBoxLayout* MainWindow::createColumn1() {
    auto *columnLayout = new QVBoxLayout();
    // Crear widgets y conexiones específicas
    auto *comboBox = new QComboBox(this);
    comboBox->addItems({"Select a PE", "PE 1", "PE 2", "PE 3", "PE 4"});
    connect(comboBox, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        peValue = text == "PE 1" ? 1 : (text == "PE 2" ? 2 : (text == "PE 3" ? 3 : (text == "PE 4" ? 4 : 0)));
    });

    auto *textEdit = new QTextEdit(this);
    textEdit->setPlaceholderText(">>");

    auto *comboBox2 = new QComboBox(this);
    comboBox2->addItems({"Select an arbitration scheme", "Round-Robin", "Static Priority"});
    connect(comboBox2, &QComboBox::currentTextChanged, this, [this](const QString &text) {
        schemeValue = text == "Round-Robin" ? 1 : (text == "Static Priority" ? 2 : 0);
    });

    auto *buttonLayout = new QHBoxLayout();

    auto *button1 = new QPushButton("Compile", this);
    auto *button2 = new QPushButton("Execute", this);
    auto *button3 = new QPushButton("Stepping", this);

    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);
    buttonLayout->addWidget(button3);

    // Crear un cuadro de texto fijo (QPlainTextEdit)
    textFixed->setReadOnly(true); // Hacerlo no editable
    textFixed->setPlaceholderText("Executed Instruction");
    textFixed->setFixedHeight(button1->height());

    connect(button1, &QPushButton::clicked, this, [textEdit, this]() {
        if (peValue > 0) {
            std::string input = textEdit->toPlainText().toStdString();
            compiler.compile(input);
            switch (peValue) {
                case 1: instructionMemories[0].addInstructionsVector(compiler.getInstructionsVector()); break;
                case 2: instructionMemories[1].addInstructionsVector(compiler.getInstructionsVector()); break;
                case 3: instructionMemories[2].addInstructionsVector(compiler.getInstructionsVector()); break;
                case 4: instructionMemories[3].addInstructionsVector(compiler.getInstructionsVector()); break;
            }
            compiler.clearInstructions();
            QMessageBox::information(this, "Notification!", QString("PE %1 code has been compiled successfully!").arg(peValue));
        } else {QMessageBox::information(this, "Warning!", "Choose a PE to compile the code.");}
    });

    connect(button2, &QPushButton::clicked, this, [this]() {
        if (schemeValue == 1) {
            std::thread([this]() {
                RoundRobinScheme scheme(instructionMemories, pes);
                scheme.executeInstructions();
                QMetaObject::invokeMethod(this, [this]() {
                    updateSystem();
                }, Qt::QueuedConnection);
            }).detach();
            QMessageBox::information(this, "Notification!", "The instructions have been executed successfully!");
        } else if (schemeValue == 2) {
            std::thread([this]() {
                StaticPriorityScheme scheme(instructionMemories, priorities, pes);
                scheme.executeInstructions();
                QMetaObject::invokeMethod(this, [this]() {
                    updateSystem();
                }, Qt::QueuedConnection);
            }).detach();
            QMessageBox::information(this, "Notification!", "The instructions have been executed successfully!");
        } else {QMessageBox::information(this, "Warning!", "Choose a scheme scheduling to execute the instructions.");}
    });

    connect(button3, &QPushButton::clicked, this, [this, button2, button1]()
    {
        button2->setEnabled(false);
        button1->setEnabled(false);
        if (instructionMemories[0].getInstructionCount() == indexInst[0] &&
                    instructionMemories[1].getInstructionCount() == indexInst[1] &&
                    instructionMemories[2].getInstructionCount() == indexInst[2] &&
                    instructionMemories[3].getInstructionCount() == indexInst[3])
        {
            std::cout << "All the instructions have been executed" << std::endl;
            QMetaObject::invokeMethod(this, [this]() {
                QMessageBox::information(this, "Notification!", "All the instructions in memory have been executed successfully!");
                textFixed->clear();
                textFixed->setPlaceholderText("Executed Instruction");
            }, Qt::QueuedConnection);
            button2->setEnabled(true);
            button1->setEnabled(true);
            indexMem = 0;
            indexInst = {0, 0, 0, 0};
        } else
        {
            if (schemeValue == 1) {
                roundRobinSteppingExecute();
                ++indexMem;
            } else if (schemeValue == 2) {
                staticPrioritySteppingExecute();
            }
        }
        updateSystem();
    });

    auto *buttonReset = new QPushButton("Reset", this);

    connect(buttonReset, &QPushButton::clicked, this, [this]() {
        clearSystem();
        QMessageBox::information(this, "Alert!", "The system has been reset!");
    });

    columnLayout->addWidget(comboBox);
    columnLayout->addWidget(new QLabel("Instructions:", this));
    columnLayout->addWidget(textEdit);
    columnLayout->addWidget(comboBox2);
    columnLayout->addLayout(buttonLayout);
    columnLayout->addWidget(textFixed);
    columnLayout->addWidget(buttonReset);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn2() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la caché
    cacheList1->addItem("Address \t Data");

    // Estadísticas
    statsTable1->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transferred"});
    statsTable1->setItem(0, 0, new QTableWidgetItem(""));

    statsTable1->setFixedHeight(statsTable1->verticalHeader()->length() + statsTable1->horizontalHeader()->height() + statsTable1->horizontalHeader()->height());


    columnLayout->addWidget(new QLabel("Cache 1", this));
    columnLayout->addWidget(cacheList1);
    columnLayout->addWidget(new QLabel("Statistics:", this));
    columnLayout->addWidget(statsTable1);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn3() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la caché
    cacheList2->addItem("Address \t Data");

    // Estadísticas
    statsTable2->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transferred"});
    statsTable2->setItem(0, 0, new QTableWidgetItem(""));

    statsTable2->setFixedHeight(statsTable2->verticalHeader()->length() + statsTable2->horizontalHeader()->height() + statsTable2->horizontalHeader()->height());

    columnLayout->addWidget(new QLabel("Cache 2", this));
    columnLayout->addWidget(cacheList2);
    columnLayout->addWidget(new QLabel("Statistics:", this));
    columnLayout->addWidget(statsTable2);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn4() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la caché
    cacheList3->addItem("Address \t Data");

    // Estadísticas
    statsTable3->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transferred"});
    statsTable3->setItem(0, 0, new QTableWidgetItem(""));

    statsTable3->setFixedHeight(statsTable3->verticalHeader()->length() + statsTable3->horizontalHeader()->height() + statsTable3->horizontalHeader()->height());

    columnLayout->addWidget(new QLabel("Cache 3", this));
    columnLayout->addWidget(cacheList3);
    columnLayout->addWidget(new QLabel("Statistics:", this));
    columnLayout->addWidget(statsTable3);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn5() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la caché
    cacheList4->addItem("Address \t Data");

    // Estadísticas
    statsTable4->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transferred"});
    statsTable4->setItem(0, 0, new QTableWidgetItem(""));

    statsTable4->setFixedHeight(statsTable4->verticalHeader()->length() + statsTable4->horizontalHeader()->height() + statsTable4->horizontalHeader()->height());

    columnLayout->addWidget(new QLabel("Cache 4", this));
    columnLayout->addWidget(cacheList4);
    columnLayout->addWidget(new QLabel("Statistics:", this));
    columnLayout->addWidget(statsTable4);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn6() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la memoria
    sharedMemList->addItem("Address \t Data");

    statsTable->setHorizontalHeaderLabels({"Read Requests", "Write Requests", "Invalidations", "Data Transferred"});

    statsTable->setFixedHeight(statsTable->verticalHeader()->length() + statsTable->horizontalHeader()->height() + statsTable->horizontalHeader()->height());

    columnLayout->addWidget(new QLabel("Shared Memory", this));
    columnLayout->addWidget(sharedMemList);
    columnLayout->addWidget(new QLabel("Bus Interconnect Statistics:", this));
    columnLayout->addWidget(statsTable);

    return columnLayout;
}

void MainWindow::roundRobinSteppingExecute() {
    if (indexMem < instructionMemories.size()) {
        if (indexInst[indexMem] < instructionMemories[indexMem].getInstructionCount()){
            stepping(indexMem, indexInst[indexMem]);
            ++indexInst[indexMem];
        } else {
            ++indexMem;
            roundRobinSteppingExecute();
        }
    } else {
        indexMem = 0;
        roundRobinSteppingExecute();
    }
}

void MainWindow::staticPrioritySteppingExecute() {
    if(indexMem < priorityIndex.size()) {
        if (indexInst[priorityIndex[indexMem]] < instructionMemories[priorityIndex[indexMem]].getInstructionCount()){
            stepping(priorityIndex[indexMem], indexInst[priorityIndex[indexMem]]);
            ++indexInst[priorityIndex[indexMem]];
        } else {
            ++indexMem;
            staticPrioritySteppingExecute();
        }
    }

}

void MainWindow::stepping(int indexMemory, int indexInstruction) {
    InstructionMemory& currentInstMem  = instructionMemories[indexMemory];
    ProcessingElement& currentPE = *pes[indexMemory];

    if (indexInstruction < currentInstMem.getInstructionCount()) {
        // Obtener la instrucción actual
        switch (currentInstMem.getInstruction(indexInstruction).inst){
        case InstructionType::LOAD:
            currentPE.load(currentInstMem.getInstruction(indexInstruction).reg, currentInstMem.getInstruction(indexInstruction).address);
            QMetaObject::invokeMethod(this, [this, currentPE, currentInstMem, indexInstruction]() {
                textFixed->clear();
                textFixed->setPlaceholderText(QString("PE %1: LOAD REG%2 <- [0x%3]").arg(currentPE.getId()).arg(currentInstMem.getInstruction(indexInstruction).reg).arg(QString("%1").arg(currentInstMem.getInstruction(indexInstruction).address, 0, 16).toUpper()));
            }, Qt::QueuedConnection);
            break;
        case InstructionType::STORE:
            currentPE.store(currentInstMem.getInstruction(indexInstruction).reg, currentInstMem.getInstruction(indexInstruction).address);
            QMetaObject::invokeMethod(this, [this, currentPE, currentInstMem, indexInstruction]() {
                textFixed->clear();
                textFixed->setPlaceholderText(QString("PE %1: STORE REG%2 -> [0x%3]").arg(currentPE.getId()).arg(currentInstMem.getInstruction(indexInstruction).reg).arg(QString("%1").arg(currentInstMem.getInstruction(indexInstruction).address, 0, 16).toUpper()));
            }, Qt::QueuedConnection);
            break;
        case InstructionType::INC:
            currentPE.increment(currentInstMem.getInstruction(indexInstruction).reg);
            QMetaObject::invokeMethod(this, [this, currentPE, currentInstMem, indexInstruction]() {
                textFixed->clear();
                textFixed->setPlaceholderText(QString("PE %1: INC  REG%2").arg(currentPE.getId()).arg(currentInstMem.getInstruction(indexInstruction).reg));
            }, Qt::QueuedConnection);
            break;
        case InstructionType::DEC:
            currentPE.decrement(currentInstMem.getInstruction(indexInstruction).reg);
            QMetaObject::invokeMethod(this, [this, currentPE, currentInstMem, indexInstruction]() {
                textFixed->clear();
                textFixed->setPlaceholderText(QString("PE %1: DEC  REG%2").arg(currentPE.getId()).arg(currentInstMem.getInstruction(indexInstruction).reg));
            }, Qt::QueuedConnection);
            break;
        case InstructionType::JNZ:
            std::cout << "PE " << currentPE.getId() << " ejecutando instruccion: " << "JNZ" << std::endl;
            break;
        default:
            break;
        }
        std::cout << std::endl;
    }
}

void MainWindow::updateSystem() {
    cacheList1->clear(); // Limpiar la lista antes de actualizar

    cacheList1->addItem("Address \t Data");

    std::vector<CacheBlock> blocks = caches[0]->getCacheBlocks();

    int index = 0;
    for (CacheBlock block : blocks) {
        cacheList1->addItem(QString("\nBlock [%1]: (%2)").arg(index)
            .arg(block.state == MESIState::Modified ? "Modified" :
                    block.state == MESIState::Exclusive ? "Exclusive" :
                    block.state == MESIState::Shared ? "Shared" : "Invalid"));
        for (int i = 0;  i < 4; ++i) {
            cacheList1->addItem(QString("[0x%1] \t %2")
                .arg(QString("%1").arg(block.tag == 0 ? 0 : block.tag*4 + i, 0, 16).toUpper())
                .arg(block.data[block.tag*4%4 + i]));
        }
        ++index;
    }
    statsTable1->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(caches[0]->getCacheMisses())));
    statsTable1->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(caches[0]->getInvalidations())));
    statsTable1->setItem(0, 2, new QTableWidgetItem(QString("%1 bytes").arg(caches[0]->getDataTransferred())));

    cacheList2->clear(); // Limpiar la lista antes de actualizar

    cacheList2->addItem("Address \t Data");

    blocks = caches[1]->getCacheBlocks();

    index = 0;
    for (CacheBlock block : blocks) {
        cacheList2->addItem(QString("\nBlock [%1]: (%2)").arg(index)
            .arg(block.state == MESIState::Modified ? "Modified" :
                    block.state == MESIState::Exclusive ? "Exclusive" :
                    block.state == MESIState::Shared ? "Shared" : "Invalid"));
        for (int i = 0;  i < 4; ++i) {
            cacheList2->addItem(QString("[0x%1] \t %2")
                .arg(QString("%1").arg(block.tag == 0 ? 0 : block.tag*4 + i, 0, 16).toUpper())
                .arg(block.data[block.tag*4%4 + i]));
        }
        ++index;
    }
    statsTable2->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(caches[1]->getCacheMisses())));
    statsTable2->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(caches[1]->getInvalidations())));
    statsTable2->setItem(0, 2, new QTableWidgetItem(QString("%1 bytes").arg(caches[1]->getDataTransferred())));


    cacheList3->clear(); // Limpiar la lista antes de actualizar

    cacheList3->addItem("Address \t Data");

    blocks = caches[2]->getCacheBlocks();

    index = 0;
    for (CacheBlock block : blocks) {
        cacheList3->addItem(QString("\nBlock [%1]: (%2)").arg(index)
            .arg(block.state == MESIState::Modified ? "Modified" :
                    block.state == MESIState::Exclusive ? "Exclusive" :
                    block.state == MESIState::Shared ? "Shared" : "Invalid"));
        for (int i = 0;  i < 4; ++i) {
            cacheList3->addItem(QString("[0x%1] \t %2")
                .arg(QString("%1").arg(block.tag == 0 ? 0 : block.tag*4 + i, 0, 16).toUpper())
                .arg(block.data[block.tag*4%4 + i]));
        }
        ++index;
    }
    statsTable3->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(caches[2]->getCacheMisses())));
    statsTable3->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(caches[2]->getInvalidations())));
    statsTable3->setItem(0, 2, new QTableWidgetItem(QString("%1 bytes").arg(caches[2]->getDataTransferred())));

    cacheList4->clear(); // Limpiar la lista antes de actualizar

    cacheList4->addItem("Address \t Data");

    blocks = caches[3]->getCacheBlocks();

    index = 0;
    for (CacheBlock block : blocks) {
        cacheList4->addItem(QString("\nBlock [%1]: (%2)").arg(index)
            .arg(block.state == MESIState::Modified ? "Modified" :
                    block.state == MESIState::Exclusive ? "Exclusive" :
                    block.state == MESIState::Shared ? "Shared" : "Invalid"));
        for (int i = 0;  i < 4; ++i) {
            cacheList4->addItem(QString("[0x%1] \t %2")
                .arg(QString("%1").arg(block.tag == 0 ? 0 : block.tag*4 + i, 0, 16).toUpper())
                .arg(block.data[block.tag*4%4 + i]));
        }
        ++index;
    }
    statsTable4->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(caches[3]->getCacheMisses())));
    statsTable4->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(caches[3]->getInvalidations())));
    statsTable4->setItem(0, 2, new QTableWidgetItem(QString("%1 bytes").arg(caches[3]->getDataTransferred())));

    sharedMemList->clear(); // Limpiar la lista antes de actualizar

    sharedMemList->addItem("Address \t Data");

    std::vector<uint64_t> memory = sharedMem.getWholeMemory();
    index = 0;
    for (uint64_t mem : memory) {
        sharedMemList->addItem(QString("[0x%1] \t %2 ")
            .arg(QString("%1").arg(index, 0, 16).toUpper())
            .arg(mem));
        ++index;
    }
    statsTable->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(bus.getReadRequests())));
    statsTable->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(bus.getWriteRequests())));
    statsTable->setItem(0, 2, new QTableWidgetItem(QString("%1").arg(bus.getInvalidations())));
    statsTable->setItem(0, 3, new QTableWidgetItem(QString("%1 bytes").arg(bus.getDataTransfered())));
}

void MainWindow::clearSystem() {
    sharedMem.clear();
    caches[0]->clear();
    caches[1]->clear();
    caches[2]->clear();
    caches[3]->clear();
    instructionMemories[0].clear();
    instructionMemories[1].clear();
    instructionMemories[2].clear();
    instructionMemories[3].clear();
    bus.clear();
    pes[0]->clear();
    pes[1]->clear();
    pes[2]->clear();
    pes[3]->clear();

    indexMem = 0;
    indexInst = {0, 0, 0, 0};

    textFixed->clear();
    textFixed->setPlaceholderText("Executed Instruction");

    updateSystem();
}