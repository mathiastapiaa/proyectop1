/**
 * @file main.cpp
 * @brief Punto de entrada del sistema de citas medicas.
 */
    
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#undef byte
#include "GestorCitas.h"
#include <iostream>
#include <memory>

using namespace std;

typedef void (__stdcall *IngresarDatoFunc)(const char*, char*, int);

int main() {
    try {
        auto gestor = make_unique<GestorCitas>("data/citas.dat");
        gestor->ejecutar();
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        std::cout << "Presione ENTER para salir...";
        std::cin.ignore();
        std::cin.get();
        return 1;
    } catch (...) {
        std::cerr << "[ERROR] Excepción desconocida." << std::endl;
        std::cout << "Presione ENTER para salir...";
        std::cin.ignore();
        std::cin.get();
        return 2;
    }
    return 0;
}