#include <iostream>
#include <QApplication>
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
#include <QTableWidgetItem>
#include <QHeaderView>

#include <vector>
#include "InstructionMemory.h"
#include "ProcessingElement.h"
#include "Cache.h"
#include "BusInterconnect.h"
#include "SharedMemory.h"
#include "StaticPriorityScheme.h"
#include "Compiler.h"
#include "InstructionMemory.h"

class MainWindow : public QWidget {
public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {
        // Crear un layout principal horizontal
        auto *mainLayout = new QHBoxLayout(this);

        // Función para crear una columna
        auto createColumn1 = [this]() -> QVBoxLayout* {
            auto *columnLayout = new QVBoxLayout();

            // Crear un cuadro desplegable para los PEs
            auto *comboBox = new QComboBox(this);
            comboBox->addItem("Select a Processing Element");
            comboBox->addItem("PE 1");
            comboBox->addItem("PE 2");
            comboBox->addItem("PE 3");
            comboBox->addItem("PE 4");

            size_t peValue = 0;

            // Conectar el QComboBox con una lambda para manejar los cambios
            connect(comboBox, &QComboBox::currentTextChanged, this, [&peValue](const QString &text) {
                if (text == "PE 1") {
                    peValue = 1;
                } else if (text == "PE 2") {
                    peValue = 2;
                } else if (text == "PE 3") {
                    peValue = 3;
                } else if (text == "PE 4") {
                    peValue = 4;
                } else {
                    peValue = 0;
                }
            });

            // Crear un cuadro para escribir texto (QTextEdit)
            auto *textEdit = new QTextEdit(this);
            textEdit->setPlaceholderText(">>");
            textEdit->setTabChangesFocus(false); // Permitir usar Tab para insertar tabs en el texto

            // Crear un cuadro desplegable para los PEs
            auto *comboBox2 = new QComboBox(this);
            comboBox2->addItem("Round-Robin");
            comboBox2->addItem("Static Priority");

            // Crear botones
            auto *buttonLayout = new QHBoxLayout();
            auto *button1 = new QPushButton("Compile", this);
            auto *button2 = new QPushButton("Execute", this);
            auto *button3 = new QPushButton("Stepping", this);
            buttonLayout->addWidget(button1);
            buttonLayout->addWidget(button2);
            buttonLayout->addWidget(button3);

            // Conectar el botón para copiar el texto
            connect(button1, &QPushButton::clicked, this, [&, textEdit]() {
                if (peValue >= 1 && peValue <= 4)
                {
                    Compiler compiler(peValue);

                    std::string input = textEdit->toPlainText().toStdString();
                    std::cout << input << std::endl;
                    compiler.compile(input);

                    InstructionMemory mem(peValue);
                    std::cout << "PE elegido: " << peValue << std::endl;

                    mem.addInstructionsVector(compiler.getInstructionsVector());

                    compiler.clearInstructions();

                    for (size_t i = 0; i < mem.getInstructionCount(); ++i) {
                        std::cout << "Instruccion compilada: ";
                        switch (mem.getInstruction(i).inst) {
                            case InstructionType::LOAD: std::cout << "LOAD"; break;
                            case InstructionType::STORE: std::cout << "STORE"; break;
                            case InstructionType::INC: std::cout << "INC"; break;
                            case InstructionType::DEC: std::cout << "DEC"; break;
                        }
                        std::cout << " Registro " << mem.getInstruction(i).reg << " Direccion " << mem.getInstruction(i).address << std::endl;
                    }
                }else
                {
                    std::cout << " PE igual a -1" << std::endl;
                }
            });

            // Conectar el botón para copiar el texto
            /*connect(button1, &QPushButton::clicked, this, [textEdit, textFixed]() {
                textFixed->setPlainText(textEdit->toPlainText());
            });*/

            // Crear un cuadro de texto fijo (QPlainTextEdit)
            auto *textFixed = new QPlainTextEdit(this);
            textFixed->setReadOnly(true); // Hacerlo no editable
            textFixed->setPlaceholderText("Executed Instruction");
            textFixed->setFixedHeight(button1->height());

            // Añadir widgets al layout de la columna
            columnLayout->addWidget(comboBox);
            columnLayout->addWidget(new QLabel("Instructions:", this));
            columnLayout->addWidget(textEdit);
            columnLayout->addWidget(comboBox2);
            columnLayout->addLayout(buttonLayout);
            columnLayout->addWidget(textFixed);

            return columnLayout;
        };

        // Función para crear una columna
        auto createColumn2 = [this]() -> QVBoxLayout* {
            auto *columnLayout = new QVBoxLayout();

            // Crear lista enumerada con scroll
            auto *listWidget1 = new QListWidget(this);
            listWidget1->addItem("Address \t Data \t State");
            QList<int> numeros = {101, 202, 303, 404, 505, 606, 707, 808};
            int index = 1;
            for (int numero : numeros) {
                listWidget1->addItem(QString("[%1] \t %2 \t %3").arg(index*4).arg(numero).arg("M"));
                index++;
            }

            // Crear una tabla con 2 filas y 3 columnas
            auto *tableWidget = new QTableWidget(1, 3, this);

            // Establecer encabezados de las columnas
            tableWidget->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transfered"});

            // Rellenar la tabla con datos
            tableWidget->setItem(0, 0, new QTableWidgetItem(QString("%1").arg("Ejemplo")));
            tableWidget->setItem(0, 1, new QTableWidgetItem(""));
            tableWidget->setItem(0, 2, new QTableWidgetItem(""));

            tableWidget->setFixedHeight(tableWidget->verticalHeader()->length() + tableWidget->horizontalHeader()->height() + tableWidget->horizontalHeader()->height());

            // Añadir widgets al layout de la columna
            columnLayout->addWidget(new QLabel("Cache 1", this));
            columnLayout->addWidget(listWidget1);
            columnLayout->addWidget(new QLabel("Statistics:", this));
            columnLayout->addWidget(tableWidget);

            return columnLayout;
        };

        // Función para crear una columna
        auto createColumn3 = [this]() -> QVBoxLayout* {
            auto *columnLayout = new QVBoxLayout();

            // Crear lista enumerada con scroll
            auto *listWidget1 = new QListWidget(this);
            listWidget1->addItem("Address \t Data \t State");
            QList<int> numeros = {101, 202, 303, 404, 505, 606, 707, 808};
            int index = 1;
            for (int numero : numeros) {
                listWidget1->addItem(QString("[%1] \t %2 \t %3").arg(index*4).arg(numero).arg("M"));
                index++;
            }

            // Crear una tabla con 2 filas y 3 columnas
            auto *tableWidget = new QTableWidget(1, 3, this);

            // Establecer encabezados de las columnas
            tableWidget->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transfered"});

            // Rellenar la tabla con datos
            tableWidget->setItem(0, 0, new QTableWidgetItem(QString("%1").arg("Ejemplo")));
            tableWidget->setItem(0, 1, new QTableWidgetItem(""));
            tableWidget->setItem(0, 2, new QTableWidgetItem(""));

            tableWidget->setFixedHeight(tableWidget->verticalHeader()->length() + tableWidget->horizontalHeader()->height() + tableWidget->horizontalHeader()->height());

            // Añadir widgets al layout de la columna
            columnLayout->addWidget(new QLabel("Cache 2", this));
            columnLayout->addWidget(listWidget1);
            columnLayout->addWidget(new QLabel("Statistics:", this));
            columnLayout->addWidget(tableWidget);

            return columnLayout;
        };

        // Función para crear una columna
        auto createColumn4 = [this]() -> QVBoxLayout* {
            auto *columnLayout = new QVBoxLayout();

            // Crear lista enumerada con scroll
            auto *listWidget1 = new QListWidget(this);
            listWidget1->addItem("Address \t Data \t State");
            QList<int> numeros = {101, 202, 303, 404, 505, 606, 707, 808};
            int index = 1;
            for (int numero : numeros) {
                listWidget1->addItem(QString("[%1] \t %2 \t %3").arg(index*4).arg(numero).arg("M"));
                index++;
            }

            // Crear una tabla con 2 filas y 3 columnas
            auto *tableWidget = new QTableWidget(1, 3, this);

            // Establecer encabezados de las columnas
            tableWidget->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transfered"});

            // Rellenar la tabla con datos
            tableWidget->setItem(0, 0, new QTableWidgetItem(QString("%1").arg("Ejemplo")));
            tableWidget->setItem(0, 1, new QTableWidgetItem(""));
            tableWidget->setItem(0, 2, new QTableWidgetItem(""));

            tableWidget->setFixedHeight(tableWidget->verticalHeader()->length() + tableWidget->horizontalHeader()->height() + tableWidget->horizontalHeader()->height());

            // Añadir widgets al layout de la columna
            columnLayout->addWidget(new QLabel("Cache 3", this));
            columnLayout->addWidget(listWidget1);
            columnLayout->addWidget(new QLabel("Statistics:", this));
            columnLayout->addWidget(tableWidget);

            return columnLayout;
        };

        // Función para crear una columna
        auto createColumn5 = [this]() -> QVBoxLayout* {
            auto *columnLayout = new QVBoxLayout();

            // Crear lista enumerada con scroll
            auto *listWidget1 = new QListWidget(this);
            listWidget1->addItem("Address \t Data \t State");
            QList<int> numeros = {101, 202, 303, 404, 505, 606, 707, 808};
            int index = 1;
            for (int numero : numeros) {
                listWidget1->addItem(QString("[%1] \t %2 \t %3").arg(index*4).arg(numero).arg("M"));
                index++;
            }

            // Crear una tabla con 2 filas y 3 columnas
            auto *tableWidget = new QTableWidget(1, 3, this);

            // Establecer encabezados de las columnas
            tableWidget->setHorizontalHeaderLabels({"Misses", "Invalidations", "Data Transfered"});

            // Rellenar la tabla con datos
            tableWidget->setItem(0, 0, new QTableWidgetItem(QString("%1").arg("Ejemplo")));
            tableWidget->setItem(0, 1, new QTableWidgetItem(""));
            tableWidget->setItem(0, 2, new QTableWidgetItem(""));

            tableWidget->setFixedHeight(tableWidget->verticalHeader()->length() + tableWidget->horizontalHeader()->height() + tableWidget->horizontalHeader()->height());

            // Añadir widgets al layout de la columna
            columnLayout->addWidget(new QLabel("Cache 4", this));
            columnLayout->addWidget(listWidget1);
            columnLayout->addWidget(new QLabel("Statistics:", this));
            columnLayout->addWidget(tableWidget);

            return columnLayout;
        };

        // Función para crear una columna
        auto createColumn6 = [this]() -> QVBoxLayout* {
            auto *columnLayout = new QVBoxLayout();

            // Crear lista enumerada con scroll
            auto *listWidget1 = new QListWidget(this);
            listWidget1->addItem("Address \t Data");
            QList<int> numeros = {101, 202, 303, 404, 505, 606, 707, 808, 909, 1010};
            int index = 1;
            for (int numero : numeros) {
                listWidget1->addItem(QString("[%1] \t %2").arg(index).arg(numero));
                index++;
            }

            // Crear una tabla con 2 filas y 3 columnas
            auto *tableWidget = new QTableWidget(1, 4, this);

            // Establecer encabezados de las columnas
            tableWidget->setHorizontalHeaderLabels({"Read Requests", "Write Requests", "Invalidations", "Data Transfered"});

            // Rellenar la tabla con datos
            tableWidget->setItem(0, 0, new QTableWidgetItem(""));
            tableWidget->setItem(0, 1, new QTableWidgetItem(""));
            tableWidget->setItem(0, 2, new QTableWidgetItem(""));

            tableWidget->setFixedHeight(tableWidget->verticalHeader()->length() + tableWidget->horizontalHeader()->height() + tableWidget->horizontalHeader()->height());

            // Añadir widgets al layout de la columna
            columnLayout->addWidget(new QLabel("Shared Memory", this));
            columnLayout->addWidget(listWidget1);
            columnLayout->addWidget(new QLabel("Bus Interconnect Statistics:", this));
            columnLayout->addWidget(tableWidget);

            return columnLayout;
        };

        // Crear dos columnas y añadirlas al layout principal
        mainLayout->addLayout(createColumn1());
        mainLayout->addLayout(createColumn2());
        mainLayout->addLayout(createColumn3());
        mainLayout->addLayout(createColumn4());
        mainLayout->addLayout(createColumn5());
        mainLayout->addLayout(createColumn6());

        setLayout(mainLayout);
        setWindowTitle("Computer Arquitecture II, Project 2");
        resize(1385, 500);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}
