#include <QApplication>
#include "MainWindow.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    SharedMemory sharedMem(256);
    Cache cache1(8, 1);
    Cache cache2(8, 2);
    Cache cache3(8, 3);
    Cache cache4(8, 4);

    std::vector<Cache*> caches = {&cache1, &cache2, &cache3, &cache4};
    BusInterconnect bus(sharedMem, caches);

    ProcessingElement pe1(1, &cache1, &bus);
    ProcessingElement pe2(2, &cache2, &bus);
    ProcessingElement pe3(3, &cache3, &bus);
    ProcessingElement pe4(4, &cache4, &bus);

    std::vector<ProcessingElement*> pes = {&pe1, &pe2, &pe3, &pe4};

    InstructionMemory mem1(1);
    InstructionMemory mem2(1);
    InstructionMemory mem3(1);
    InstructionMemory mem4(1);

    std::vector<InstructionMemory> instructionMemories = {mem1, mem2, mem3, mem4};
    std::vector<int> priorities = {3, 2, 4, 1};

    Compiler compiler(1);

    MainWindow mainWindow(sharedMem, instructionMemories, priorities, pes, caches, compiler, bus);
    mainWindow.show();

    return app.exec();
}
