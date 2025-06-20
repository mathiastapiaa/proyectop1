#include "Hora.h"
#include <stdexcept>

using namespace std;

Hora::Hora(int h, int m, int s) : hora(h), minuto(m), segundo(s) {
    // Solo validar si los valores son positivos
    if (h >= 0 && m >= 0 && s >= 0)
        validarHora();
}

Hora::Hora(const Hora& otra) : hora(otra.hora), minuto(otra.minuto), segundo(otra.segundo) {}

void Hora::serializar(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&hora), sizeof(hora));
    os.write(reinterpret_cast<const char*>(&minuto), sizeof(minuto));
    os.write(reinterpret_cast<const char*>(&segundo), sizeof(segundo));
}

void Hora::deserializar(std::istream& is) {
    is.read(reinterpret_cast<char*>(&hora), sizeof(hora));
    is.read(reinterpret_cast<char*>(&minuto), sizeof(minuto));
    is.read(reinterpret_cast<char*>(&segundo), sizeof(segundo));
}

int Hora::getHora() const { return hora; }
int Hora::getMinuto() const { return minuto; }
int Hora::getSegundo() const { return segundo; }

void Hora::setHora(int h) { hora = h; validarHora(); }
void Hora::setMinuto(int m) { minuto = m; validarHora(); }
void Hora::setSegundo(int s) { segundo = s; validarHora(); }

void Hora::validarHora() const {
    if (hora < 0 || hora > 23) throw invalid_argument("Hora no valida");
    if (minuto < 0 || minuto > 59) throw invalid_argument("Minuto no valido");
    if (segundo < 0 || segundo > 59) throw invalid_argument("Segundo no valido");
}

bool Hora::operator<(const Hora& otra) const {
    if (hora != otra.hora) return hora < otra.hora;
    if (minuto != otra.minuto) return minuto < otra.minuto;
    return segundo < otra.segundo;
}

bool Hora::operator>(const Hora& otra) const {
    return otra < *this;
}

bool Hora::operator==(const Hora& otra) const {
    return hora == otra.hora && minuto == otra.minuto && segundo == otra.segundo;
}

bool Hora::esHoraValida() const {
    if (hora < 0 || minuto < 0 || segundo < 0) return false;
    try {
        validarHora();
        return true;
    } catch (...) {
        return false;
    }
}

ostream& operator<<(ostream& os, const Hora& hora) {
    os << hora.hora << ":" << hora.minuto << ":" << hora.segundo;
    return os;
}

istream& operator>>(istream& is, Hora& hora) {
    char sep;
    is >> hora.hora >> sep >> hora.minuto >> sep >> hora.segundo;
    hora.validarHora();
    return is;
}