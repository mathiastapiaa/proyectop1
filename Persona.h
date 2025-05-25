/**
 * @file Persona.h
 * @brief Definicion de la clase Persona.
 */

#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include "Fecha.h"

using namespace std;

/**
 * @class Persona
 * @brief Clase base para personas.
 */
class Persona {
protected:
    string nombre;
    string cedula;
    Fecha fechaNacimiento;

public:
    /**
     * @brief Constructor de la clase Persona.
     * @param nom Nombre de la persona.
     * @param ced Cedula de la persona.
     * @param fn Fecha de nacimiento.
     */
    Persona(const string& nom = "", const string& ced = "", 
            const Fecha& fn = Fecha());

    /**
     * @brief Destructor virtual.
     */
    virtual ~Persona() = default;

    /**
     * @brief Serializa la persona en un flujo de salida.
     * @param os Flujo de salida.
     */
    virtual void serializar(std::ostream& os) const;

    /**
     * @brief Deserializa la persona desde un flujo de entrada.
     * @param is Flujo de entrada.
     */
    virtual void deserializar(std::istream& is);

    /**
     * @brief Obtiene el nombre de la persona.
     * @return Nombre.
     */
    string getNombre() const;

    /**
     * @brief Obtiene la cedula de la persona.
     * @return Cedula.
     */
    string getCedula() const;

    /**
     * @brief Obtiene la fecha de nacimiento.
     * @return Fecha de nacimiento.
     */
    Fecha getFechaNacimiento() const;

    /**
     * @brief Calcula la edad de la persona.
     * @return Edad.
     */
    int getEdad() const;

    /**
     * @brief Establece el nombre de la persona.
     * @param nom Nombre.
     */
    void setNombre(const string& nom);

    /**
     * @brief Establece la cedula de la persona.
     * @param ced Cedula.
     */
    void setCedula(const string& ced);

    /**
     * @brief Establece la fecha de nacimiento.
     * @param fn Fecha de nacimiento.
     */
    void setFechaNacimiento(const Fecha& fn);

    /**
     * @brief Muestra la informacion de la persona por consola.
     */
    virtual void mostrarInformacion() const;
};

#endif // PERSONA_H