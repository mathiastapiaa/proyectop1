/**
 * @file Paciente.h
 * @brief Definicion de la clase Paciente.
 */

#ifndef PACIENTE_H
#define PACIENTE_H

#include "Persona.h"
using namespace std;

/**
 * @class Paciente
 * @brief Representa un paciente, hereda de Persona.
 */

class Paciente : public Persona {
private:
    string historialMedico;

public:
    /**
     * @brief Constructor de la clase Paciente.
     * @param nom Nombre del paciente.
     * @param ced Cedula del paciente.
     * @param fn Fecha de nacimiento.
     * @param hist Historial medico.
     */
    Paciente(const string& nom = "", const string& ced = "", 
             const Fecha& fn = Fecha(), const string& hist = "");

    /**
     * @brief Obtiene el historial medico del paciente.
     * @return Historial medico.
     */
    string getHistorialMedico() const;

    /**
     * @brief Establece el historial medico del paciente.
     * @param hist Historial medico.
     */
    void setHistorialMedico(const string& hist);

    /**
     * @brief Muestra la informacion del paciente por consola.
     */
    void mostrarInformacion() const override;

    /**
     * @brief Serializa el paciente en un flujo de salida.
     * @param os Flujo de salida.
     */
    void serializar(std::ostream& os) const override;

    /**
     * @brief Deserializa el paciente desde un flujo de entrada.
     * @param is Flujo de entrada.
     */
    void deserializar(std::istream& is) override;
};

#endif // PACIENTE_H    