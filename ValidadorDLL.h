#pragma once

#ifdef VALIDADOR_DLL_EXPORTS
#define VALIDADOR_API __declspec(dllexport)
#else
#define VALIDADOR_API __declspec(dllimport)
#endif

extern "C" {
    VALIDADOR_API bool validarClave(const char* clave);
}
