#pragma once

#ifdef INGRESODATOSDLL_EXPORTS
#define INGRESODATOSDLL_API __declspec(dllexport)
#else
#define INGRESODATOSDLL_API __declspec(dllimport)
#endif

// Exportar con stdcall para evitar decoraciones y asegurar compatibilidad con LoadLibrary/GetProcAddress
extern "C" {
    // Solicita un dato al usuario y lo devuelve como string (buffer)
    INGRESODATOSDLL_API void __stdcall ingresarDato(const char* mensaje, char* buffer, int bufferSize);
}