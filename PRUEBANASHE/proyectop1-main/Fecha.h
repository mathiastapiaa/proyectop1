/**
 * @file Fecha.h
 * @brief Definicion de la clase Fecha.
 */

#ifndef FECHA_H
#define FECHA_H

#include <iostream>
#include <ctime>
#include <stdexcept>
#include <string>

/**
 * @class Fecha
 * @brief Representa una fecha.
 */
class Fecha {
private:
    int dia, mes, anio;

    void validarFecha() const;
    tm getFechaActual() const;

public:

    /**
     * @brief Constructor de la clase Fecha.
     * @param d Dia.
     * @param m Mes.
     * @param a Anio.
     */
    Fecha(int d = 0, int m = 0, int a = 0);

    /**
     * @brief Constructor de copia.
     * @param otra Otra fecha.
     */
    Fecha(const Fecha& otra);

    /**
     * @brief Serializa la fecha en un flujo de salida.
     * @param os Flujo de salida.
     */
    void serializar(std::ostream& os) const;

    /**
     * @brief Deserializa la fecha desde un flujo de entrada.
     * @param is Flujo de entrada.
     */
    void deserializar(std::istream& is);

    /**
     * @brief Obtiene la fecha actual del sistema.
     * @return Fecha actual.
     */
    static Fecha fechaActual();

    /**
     * @brief Indica si un anio es bisiesto.
     * @param anio Anio.
     * @return true si es bisiesto.
     */
    static bool esBisiesto(int anio);

    /**
     * @brief Devuelve la cantidad de dias de un mes.
     * @param mes Mes.
     * @param anio Anio.
     * @return Dias en el mes.
     */
    static int diasEnMesStatic(int mes, int anio);

    /**
     * @brief Obtiene el dia.
     * @return Dia.
     */
    int getDia() const;

    /**
     * @brief Obtiene el mes.
     * @return Mes.
     */
    int getMes() const;

    /**
     * @brief Obtiene el anio.
     * @return Anio.
     */
    int getAnio() const;

    /**
     * @brief Establece el dia.
     * @param d Dia.
     */
    void setDia(int d);

    /**
     * @brief Establece el mes.
     * @param m Mes.
     */
    void setMes(int m);

    /**
     * @brief Establece el anio.
     * @param a Anio.
     */
    void setAnio(int a);

    /**
     * @brief Operador menor que.
     * @param otra Otra fecha.
     * @return true si es menor.
     */
    bool operator<(const Fecha& otra) const;

    /**
     * @brief Operador mayor que.
     * @param otra Otra fecha.
     * @return true si es mayor.
     */
    bool operator>(const Fecha& otra) const;

    /**
     * @brief Operador de igualdad.
     * @param otra Otra fecha.
     * @return true si son iguales.
     */
    bool operator==(const Fecha& otra) const;

    /**
     * @brief Calcula la edad a partir de la fecha.
     * @return Edad.
     */
    int calcularEdad() const;

    /**
     * @brief Indica si la fecha es valida.
     * @return true si es valida.
     */
    bool esFechaValida() const;

    /**
     * @brief Indica si la fecha es pasada.
     * @return true si es pasada.
     */
    bool esFechaPasada() const;

    /**
     * @brief Indica si la fecha es futura.
     * @return true si es futura.
     */
    bool esFechaFutura() const;

    /**
     * @brief Indica si la fecha es mayor que otra en cierta cantidad de anos.
     * @param otra Otra fecha.
     * @param anos Cantidad de anos.
     * @return true si es mayor.
     */
    bool esMayorQue(const Fecha& otra, int anos) const;

    /**
     * @brief Devuelve el nombre del mes.
     * @return Nombre del mes.
     */
    std::string getMesStr() const;

    /**
     * @brief Operador de salida para imprimir la fecha.
     */
    friend std::ostream& operator<<(std::ostream& os, const Fecha& fecha);

    /**
     * @brief Operador de entrada para leer la fecha.
     */
    friend std::istream& operator>>(std::istream& is, Fecha& fecha);
};

#endif // FECHA_H
