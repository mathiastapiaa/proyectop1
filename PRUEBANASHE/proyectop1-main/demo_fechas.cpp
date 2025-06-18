#include <iostream>
#include <vector>
#include <algorithm>
#include "Fecha.h"
#include "GestorCitas.h"

int main() {
    Fecha hoy = Fecha::fechaActual();
    std::cout << "Fecha de hoy: " << hoy << std::endl;

    // Instanciar GestorCitas para acceder a feriados
    GestorCitas gestor("data/citas.dat");
    std::vector<Fecha> feriadosAntes, feriadosDespues;

    // Buscar feriados antes y después de hoy
    for (int offset = -1; offset <= 1; ++offset) {
        int anio = hoy.getAnio() + offset;
        for (const auto& f : gestor.getFeriados(anio)) {
            Fecha feriado(f.first, f.second, anio);
            if (feriado < hoy) feriadosAntes.push_back(feriado);
            else if (feriado > hoy) feriadosDespues.push_back(feriado);
        }
    }

    // Ordenar y mostrar el feriado más cercano antes y después de hoy
    if (!feriadosAntes.empty()) {
        std::sort(feriadosAntes.begin(), feriadosAntes.end());
        std::cout << "Feriado antes de hoy: " << feriadosAntes.back() << std::endl;
    } else {
        std::cout << "No hay feriado antes de hoy." << std::endl;
    }
    if (!feriadosDespues.empty()) {
        std::sort(feriadosDespues.begin(), feriadosDespues.end());
        std::cout << "Feriado despues de hoy: " << feriadosDespues.front() << std::endl;
    } else {
        std::cout << "No hay feriado despues de hoy." << std::endl;
    }
    std::cout << "\nPresione ENTER para salir...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}
