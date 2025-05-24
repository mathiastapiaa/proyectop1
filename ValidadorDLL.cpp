#define VALIDADOR_DLL_EXPORTS
#include "ValidadorDLL.h"
#include <cstring>

extern "C" {
    bool validarClave(const char* clave) {
        static const char* claves[] = {"ABC123", "DEF456", "GHI789", "JKL012", "MNO345"};
        for (int i = 0; i < 5; ++i) {
            if (strcmp(clave, claves[i]) == 0) return true;
        }
        return false;
    }
}
