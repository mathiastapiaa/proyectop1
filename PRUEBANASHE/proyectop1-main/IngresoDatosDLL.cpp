#define INGRESODATOSDLL_EXPORTS
#include "IngresoDatosDLL.h"
#include <iostream>
#include <cstring>

extern "C" {
    INGRESODATOSDLL_API void __stdcall ingresarDato(const char* mensaje, char* buffer, int bufferSize) {
        std::cout << mensaje;
        std::cin.getline(buffer, bufferSize);
    }
}