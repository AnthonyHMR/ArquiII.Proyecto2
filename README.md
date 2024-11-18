ArquiII.Proyecto2: Simulador de Sistema Multiprocesador con Protocolo MESI

Descripción del Proyecto
Este proyecto implementa un simulador de un sistema multiprocesador que utiliza el protocolo MESI para la coherencia de caché.
Se enfoca en modelar procesadores (PEs) con cachés privadas y un bus de interconexión que administra la memoria compartida, cumpliendo con las características especificadas en el ámbito de arquitectura de computadores.

Requisitos del Sistema
1. Sistema Operativo: Windows, macOS o Linux.
2. Compilador: Compatible con C++17 (GCC, Clang, o MSVC).
3. CLion IDE: Instalado con soporte para proyectos basados en CMake.
4. CMake: Instalado como parte de CLion (versión 3.10 o superior).
5. Qt: Veriosn 6.0.8, framework instalado desde la pagina oficial de Qt.
6. Bibliotecas Adicionales: Ninguna

Instrucciones para Compilar
1. Abre CLion.
2. Selecciona File -> Open y navega al directorio raíz del proyecto (ArquiII.Proyecto2/).
3. CLion detectará automáticamente el archivo CMakeLists.txt y configurará el entorno de compilación.
4. Haz clic en el botón Build.
5. Haz clic en el botón Run.
6. El programa ejecutará las instrucciones definidas en el archivo main.cpp y mostrará los resultados en la consola integrada de CLion y la GUI hecha con Qt.

Opciones de Ejecución
Ya desplegada la interfaz grafica:
1. Escriba el código que desee en el cuadro de texto que dice "Instructions".
2. Tome en cuenta la estructura en la que se debe escribir las instrucciones, ejemplo:
    - LOAD REG0 8
    - INC REG1
    - STORE REG1 12
3. Cuando hayas terminado de escribir el codigo, elija un procesador PE al que le quiera cargar las instrucciones en memoria.
4. Seguidamente presione el boton "Compilar", para que se compilen las instrucciones escritas y se carguen en la memoria de instrucciones.
5. *Como dato relevante, puede escribir cuatro codigos distintos a cargar, uno para cada PE. Solamente debe elegir el PE correspondiente y darle al botón "Compilar".
6. Seguidamente seleccione un esquema de arbitraje abajo del cuadro de texto en el que escribió, para que se ejecuten las instrucciones de la forma que prefiera.
7. Por último, presione el boton "Execute" o el botón "Stepping" para ejecutar las instrucciones cargadas.
8. *Como segundo dato relevante, puede notar que hay un boton abajo de las listas de cache. Al presionar este, los datos en cada una de las tablas y listas se actualizara, para que así pueda observar el proceso de una manera más gráfica.


Responsable: Anthony Montero Román
Carné: 2019275097