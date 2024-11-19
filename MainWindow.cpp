#include "MainWindow.h"
#include <QApplication>
#include <thread>
#include <iostream>
#include <QHeaderView>

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
        peValue = text.contains("PE") ? text.split(" ")[1].toInt() : 0;
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
        }
    });

    connect(button2, &QPushButton::clicked, this, [this]() {
        if (schemeValue == 1) {
            std::thread([this]() {
                RoundRobinScheme scheme(instructionMemories, pes);
                scheme.executeInstructions();
                //caches[0]->displayCache();
            }).detach();
        } else if (schemeValue == 2) {
            std::thread([this]() {
                StaticPriorityScheme scheme(instructionMemories, priorities, pes);
                scheme.executeInstructions();
            }).detach();
        }
    });

    // Crear un cuadro de texto fijo (QPlainTextEdit)
    auto *textFixed = new QPlainTextEdit(this);
    textFixed->setReadOnly(true); // Hacerlo no editable
    textFixed->setPlaceholderText("Executed Instruction");
    textFixed->setFixedHeight(button1->height());

    auto *buttonReset = new QPushButton("Reset", this);

    connect(buttonReset, &QPushButton::clicked, this, [this]() {
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
    auto *cacheList = new QListWidget(this);
    cacheList->addItem("Address \t Data");

    auto *buttonUpdate = new QPushButton("Update", this);

    // Estadísticas
    auto *statsTable = new QTableWidget(1, 3, this);
    statsTable->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transferred"});
    statsTable->setItem(0, 0, new QTableWidgetItem(""));

    connect(buttonUpdate, &QPushButton::clicked, this, [this, cacheList, statsTable]()
    {
        cacheList->clear(); // Limpiar la lista antes de actualizar

        cacheList->addItem("Address \t Data");

        std::vector<CacheBlock> blocks = caches[0]->getCacheBlocks();

        int index = 0;
        for (CacheBlock block : blocks) {
            cacheList->addItem(QString("\nBlock [%1]: (%2)").arg(index)
                .arg(block.state == MESIState::Modified ? "Modified" :
                        block.state == MESIState::Exclusive ? "Exclusive" :
                        block.state == MESIState::Shared ? "Shared" : "Invalid"));
            for (int i = 0;  i < 4; ++i) {
                cacheList->addItem(QString("[0x%1] \t %2")
                    .arg(block.tag == 0 ? 0 : block.tag*4 + i, 0, 16).toUpper()
                    .arg(block.data[block.tag*4%4 + i]));
            }
            ++index;
        }
        statsTable->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(caches[0]->getCacheMisses())));
        statsTable->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(caches[0]->getInvalidations())));
        statsTable->setItem(0, 2, new QTableWidgetItem(QString("%1 bytes").arg(caches[0]->getDataTransferred())));

        std::cout << "Cache 1:" << std::endl;
        caches[0]->displayCache();
    });

    statsTable->setFixedHeight(statsTable->verticalHeader()->length() + statsTable->horizontalHeader()->height() + statsTable->horizontalHeader()->height());


    columnLayout->addWidget(new QLabel("Cache 1", this));
    columnLayout->addWidget(cacheList);
    columnLayout->addWidget(buttonUpdate);
    columnLayout->addWidget(new QLabel("Statistics:", this));
    columnLayout->addWidget(statsTable);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn3() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la caché
    auto *cacheList = new QListWidget(this);
    cacheList->addItem("Address \t Data");

    auto *buttonUpdate = new QPushButton("Update", this);

    // Estadísticas
    auto *statsTable = new QTableWidget(1, 3, this);
    statsTable->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transferred"});
    statsTable->setItem(0, 0, new QTableWidgetItem(""));

    connect(buttonUpdate, &QPushButton::clicked, this, [this, cacheList, statsTable]()
    {
        cacheList->clear(); // Limpiar la lista antes de actualizar

        cacheList->addItem("Address \t Data");

        std::vector<CacheBlock> blocks = caches[1]->getCacheBlocks();

        int index = 0;
        for (CacheBlock block : blocks) {
            cacheList->addItem(QString("\nBlock [%1]: (%2)").arg(index)
                .arg(block.state == MESIState::Modified ? "Modified" :
                        block.state == MESIState::Exclusive ? "Exclusive" :
                        block.state == MESIState::Shared ? "Shared" : "Invalid"));
            for (int i = 0;  i < 4; ++i) {
                cacheList->addItem(QString("[0x%1] \t %2")
                    .arg(block.tag == 0 ? 0 : block.tag*4 + i, 0, 16).toUpper()
                    .arg(block.data[block.tag*4%4 + i]));
            }
            ++index;
        }
        statsTable->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(caches[1]->getCacheMisses())));
        statsTable->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(caches[1]->getInvalidations())));
        statsTable->setItem(0, 2, new QTableWidgetItem(QString("%1 bytes").arg(caches[1]->getDataTransferred())));

        std::cout << "Cache 2:" << std::endl;
        caches[1]->displayCache();
    });

    statsTable->setFixedHeight(statsTable->verticalHeader()->length() + statsTable->horizontalHeader()->height() + statsTable->horizontalHeader()->height());


    columnLayout->addWidget(new QLabel("Cache 2", this));
    columnLayout->addWidget(cacheList);
    columnLayout->addWidget(buttonUpdate);
    columnLayout->addWidget(new QLabel("Statistics:", this));
    columnLayout->addWidget(statsTable);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn4() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la caché
    auto *cacheList = new QListWidget(this);
    cacheList->addItem("Address \t Data");

    auto *buttonUpdate = new QPushButton("Update", this);

    // Estadísticas
    auto *statsTable = new QTableWidget(1, 3, this);
    statsTable->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transferred"});
    statsTable->setItem(0, 0, new QTableWidgetItem(""));

    connect(buttonUpdate, &QPushButton::clicked, this, [this, cacheList, statsTable]()
    {
        cacheList->clear(); // Limpiar la lista antes de actualizar

        cacheList->addItem("Address \t Data");

        std::vector<CacheBlock> blocks = caches[2]->getCacheBlocks();

        int index = 0;
        for (CacheBlock block : blocks) {
            cacheList->addItem(QString("\nBlock [%1]: (%2)").arg(index)
                .arg(block.state == MESIState::Modified ? "Modified" :
                        block.state == MESIState::Exclusive ? "Exclusive" :
                        block.state == MESIState::Shared ? "Shared" : "Invalid"));
            for (int i = 0;  i < 4; ++i) {
                cacheList->addItem(QString("[0x%1] \t %2")
                    .arg(block.tag == 0 ? 0 : block.tag*4 + i, 0, 16).toUpper()
                    .arg(block.data[block.tag*4%4 + i]));
            }
            ++index;
        }
        statsTable->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(caches[2]->getCacheMisses())));
        statsTable->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(caches[2]->getInvalidations())));
        statsTable->setItem(0, 2, new QTableWidgetItem(QString("%1 bytes").arg(caches[2]->getDataTransferred())));

        std::cout << "Cache 3:" << std::endl;
        caches[2]->displayCache();
    });

    statsTable->setFixedHeight(statsTable->verticalHeader()->length() + statsTable->horizontalHeader()->height() + statsTable->horizontalHeader()->height());


    columnLayout->addWidget(new QLabel("Cache 3", this));
    columnLayout->addWidget(cacheList);
    columnLayout->addWidget(buttonUpdate);
    columnLayout->addWidget(new QLabel("Statistics:", this));
    columnLayout->addWidget(statsTable);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn5() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la caché
    auto *cacheList = new QListWidget(this);
    cacheList->addItem("Address \t Data");

    auto *buttonUpdate = new QPushButton("Update", this);

    // Estadísticas
    auto *statsTable = new QTableWidget(1, 3, this);
    statsTable->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transferred"});
    statsTable->setItem(0, 0, new QTableWidgetItem(""));

    connect(buttonUpdate, &QPushButton::clicked, this, [this, cacheList, statsTable]()
    {
        cacheList->clear(); // Limpiar la lista antes de actualizar

        cacheList->addItem("Address \t Data");

        std::vector<CacheBlock> blocks = caches[3]->getCacheBlocks();

        int index = 0;
        for (CacheBlock block : blocks) {
            cacheList->addItem(QString("\nBlock [%1]: (%2)").arg(index)
                .arg(block.state == MESIState::Modified ? "Modified" :
                        block.state == MESIState::Exclusive ? "Exclusive" :
                        block.state == MESIState::Shared ? "Shared" : "Invalid"));
            for (int i = 0;  i < 4; ++i) {
                cacheList->addItem(QString("[0x%1] \t %2")
                    .arg(block.tag == 0 ? 0 : block.tag*4 + i, 0, 16).toUpper()
                    .arg(block.data[block.tag*4%4 + i]));
            }
            ++index;
        }
        statsTable->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(caches[3]->getCacheMisses())));
        statsTable->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(caches[3]->getInvalidations())));
        statsTable->setItem(0, 2, new QTableWidgetItem(QString("%1 bytes").arg(caches[3]->getDataTransferred())));

        std::cout << "Cache 4:" << std::endl;
        caches[3]->displayCache();
    });

    statsTable->setFixedHeight(statsTable->verticalHeader()->length() + statsTable->horizontalHeader()->height() + statsTable->horizontalHeader()->height());


    columnLayout->addWidget(new QLabel("Cache 4", this));
    columnLayout->addWidget(cacheList);
    columnLayout->addWidget(buttonUpdate);
    columnLayout->addWidget(new QLabel("Statistics:", this));
    columnLayout->addWidget(statsTable);

    return columnLayout;
}

QVBoxLayout* MainWindow::createColumn6() {
    auto *columnLayout = new QVBoxLayout();

    // Lista de datos de la memoria
    auto *sharedMemList = new QListWidget(this);
    sharedMemList->addItem("Address \t Data");

    auto *buttonUpdate = new QPushButton("Update", this);



    auto *statsTable = new QTableWidget(1, 4, this);
    statsTable->setHorizontalHeaderLabels({"Read Requests", "Write Requests", "Invalidations", "Data Transferred"});

    connect(buttonUpdate, &QPushButton::clicked, this, [this, sharedMemList, statsTable]()
    {
        sharedMemList->clear(); // Limpiar la lista antes de actualizar

        sharedMemList->addItem("Address \t Data");

        std::vector<uint64_t> memory = sharedMem.getWholeMemory();
        int index = 0;
        for (uint64_t mem : memory) {
            sharedMemList->addItem(QString("[%1] \t %2 ")
                .arg(index, 0, 16).toUpper()
                .arg(mem));
            ++index;
        }
        statsTable->setItem(0, 0, new QTableWidgetItem(QString("%1").arg(bus.getReadRequests())));
        statsTable->setItem(0, 1, new QTableWidgetItem(QString("%1").arg(bus.getWriteRequests())));
        statsTable->setItem(0, 2, new QTableWidgetItem(QString("%1").arg(bus.getInvalidations())));
        statsTable->setItem(0, 3, new QTableWidgetItem(QString("%1 bytes").arg(bus.getDataTransfered())));

        std::cout << "Shared Memory:" << std::endl;
        sharedMem.displayMemory();
    });

    statsTable->setFixedHeight(statsTable->verticalHeader()->length() + statsTable->horizontalHeader()->height() + statsTable->horizontalHeader()->height());

    columnLayout->addWidget(new QLabel("Shared Memory", this));
    columnLayout->addWidget(sharedMemList);
    columnLayout->addWidget(buttonUpdate);
    columnLayout->addWidget(new QLabel("Bus Interconnect Statistics:", this));
    columnLayout->addWidget(statsTable);

    return columnLayout;
}
