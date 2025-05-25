#include "Fecha.h"
#include <sstream>
#include <ctime>

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {
    // Solo validar si los valores son positivos
    if (d > 0 && m > 0 && a > 0)
        validarFecha();
}

Fecha::Fecha(const Fecha& otra) : dia(otra.dia), mes(otra.mes), anio(otra.anio) {}

void Fecha::serializar(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&dia), sizeof(dia));
    os.write(reinterpret_cast<const char*>(&mes), sizeof(mes));
    os.write(reinterpret_cast<const char*>(&anio), sizeof(anio));
}

void Fecha::deserializar(std::istream& is) {
    is.read(reinterpret_cast<char*>(&dia), sizeof(dia));
    is.read(reinterpret_cast<char*>(&mes), sizeof(mes));
    is.read(reinterpret_cast<char*>(&anio), sizeof(anio));
}

int Fecha::getDia() const { return dia; }
int Fecha::getMes() const { return mes; }
int Fecha::getAnio() const { return anio; }

void Fecha::setDia(int d) { dia = d; validarFecha(); }
void Fecha::setMes(int m) { mes = m; validarFecha(); }
void Fecha::setAnio(int a) { anio = a; validarFecha(); }

bool Fecha::esBisiesto(int anio) {
    return (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
}

int Fecha::diasEnMesStatic(int mes, int anio) {
    if (mes == 2) return esBisiesto(anio) ? 29 : 28;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
    return 31;
}

void Fecha::validarFecha() const {
    if (anio < 1900) throw std::invalid_argument("Año no válido");
    if (mes < 1 || mes > 12) throw std::invalid_argument("Mes no válido");
    if (dia < 1 || dia > diasEnMesStatic(mes, anio)) throw std::invalid_argument("Día no válido");
}

tm Fecha::getFechaActual() const {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return *now;
}

Fecha Fecha::fechaActual() {
    time_t t = time(nullptr);
    tm* now = localtime(&t);
    return Fecha(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
}

bool Fecha::operator<(const Fecha& otra) const {
    if (anio != otra.anio) return anio < otra.anio;
    if (mes != otra.mes) return mes < otra.mes;
    return dia < otra.dia;
}

bool Fecha::operator>(const Fecha& otra) const {
    return otra < *this;
}

bool Fecha::operator==(const Fecha& otra) const {
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}

int Fecha::calcularEdad() const {
    tm ahora = getFechaActual();
    int edad = ahora.tm_year + 1900 - anio;
    if (ahora.tm_mon + 1 < mes || (ahora.tm_mon + 1 == mes && ahora.tm_mday < dia)) {
        edad--;
    }
    return edad;
}

bool Fecha::esFechaValida() const {
    if (dia < 1 || mes < 1 || anio < 1) return false;
    try {
        validarFecha();
        return true;
    } catch (...) {
        return false;
    }
}

bool Fecha::esFechaPasada() const {
    return *this < fechaActual();
}

bool Fecha::esFechaFutura() const {
    return *this > fechaActual();
}

bool Fecha::esMayorQue(const Fecha& otra, int anos) const {
    Fecha temp = otra;
    temp.anio += anos;
    return *this > temp;
}

std::ostream& operator<<(std::ostream& os, const Fecha& fecha) {
    os << fecha.dia << "/" << fecha.mes << "/" << fecha.anio;
    return os;
}

std::istream& operator>>(std::istream& is, Fecha& fecha) {
    char sep;
    is >> fecha.dia >> sep >> fecha.mes >> sep >> fecha.anio;
    fecha.validarFecha();
    return is;
}
std::string Fecha::getMesStr() const {
    static const char* meses[] = {
        "Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio",
        "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre"
    };
    if (mes >= 1 && mes <= 12)
        return meses[mes - 1];
    else
        return "Mes inválido";
}
