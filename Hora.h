/**
 * @file Hora.h
 * @brief Definicion de la clase Hora.
 */

#ifndef HORA_H
#define HORA_H

#include <iostream>
#include <stdexcept>

using namespace std;

/**
 * @class Hora
 * @brief Representa una hora.
 */
class Hora {
private:
    int hora, minuto, segundo;
    void validarHora() const;

public:
    /**
     * @brief Constructor de la clase Hora.
     * @param h Hora.
     * @param m Minuto.
     * @param s Segundo.
     */
    Hora(int h = -1, int m = -1, int s = -1);

    /**
     * @brief Constructor de copia.
     * @param otra Otra hora.
     */
    Hora(const Hora& otra);

    /**
     * @brief Serializa la hora en un flujo de salida.
     * @param os Flujo de salida.
     */
    void serializar(std::ostream& os) const;

    /**
     * @brief Deserializa la hora desde un flujo de entrada.
     * @param is Flujo de entrada.
     */
    void deserializar(std::istream& is);

    /**
     * @brief Obtiene la hora.
     * @return Hora.
     */
    int getHora() const;

    /**
     * @brief Obtiene el minuto.
     * @return Minuto.
     */
    int getMinuto() const;

    /**
     * @brief Obtiene el segundo.
     * @return Segundo.
     */
    int getSegundo() const;

    /**
     * @brief Establece la hora.
     * @param h Hora.
     */
    void setHora(int h);

    /**
     * @brief Establece el minuto.
     * @param m Minuto.
     */
    void setMinuto(int m);

    /**
     * @brief Establece el segundo.
     * @param s Segundo.
     */
    void setSegundo(int s);

    /**
     * @brief Operador menor que.
     * @param otra Otra hora.
     * @return true si es menor.
     */
    bool operator<(const Hora& otra) const;

    /**
     * @brief Operador mayor que.
     * @param otra Otra hora.
     * @return true si es mayor.
     */
    bool operator>(const Hora& otra) const;

    /**
     * @brief Operador de igualdad.
     * @param otra Otra hora.
     * @return true si son iguales.
     */
    bool operator==(const Hora& otra) const;

    /**
     * @brief Indica si la hora es valida.
     * @return true si es valida.
     */
    bool esHoraValida() const;

    /**
     * @brief Operador de salida para imprimir la hora.
     */
    friend ostream& operator<<(ostream& os, const Hora& hora);

    /**
     * @brief Operador de entrada para leer la hora.
     */
    friend istream& operator>>(istream& is, Hora& hora);
};

#endif // HORA_H