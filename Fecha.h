#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <ctime>
#include <stdexcept>
#include <string>

class Fecha {
private:
    int dia, mes, anio;

    void validarFecha() const;
    tm getFechaActual() const;

public:
    Fecha(int d = 0, int m = 0, int a = 0);
    Fecha(const Fecha& otra);

    static Fecha fechaActual();
    static bool esBisiesto(int anio);
    static int diasEnMesStatic(int mes, int anio);

    int getDia() const;
    int getMes() const;
    int getAnio() const;

    void setDia(int d);
    void setMes(int m);
    void setAnio(int a);

    bool operator<(const Fecha& otra) const;
    bool operator>(const Fecha& otra) const;
    bool operator==(const Fecha& otra) const;

    int calcularEdad() const;
    bool esFechaValida() const;
    bool esFechaPasada() const;
    bool esFechaFutura() const;
    bool esMayorQue(const Fecha& otra, int anos) const;

    std::string getMesStr() const; // <-- Método para obtener el nombre del mes

    friend std::ostream& operator<<(std::ostream& os, const Fecha& fecha);
    friend std::istream& operator>>(std::istream& is, Fecha& fecha);
};

#endif // FECHA_H
