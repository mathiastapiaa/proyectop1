/**
 * @file Cita.h
 * @brief Definicion de la clase Cita y enumeracion Especialidad.
 */

/**
 * @enum Especialidad
 * @brief Enum para las especialidades medicas.
 */
/**
 * @class Cita
 * @brief Representa una cita medica.
 */
#ifndef CITA_H
#define CITA_H

#include "Paciente.h"
#include "Fecha.h"
#include "Hora.h"
#include <string>

using namespace std;

/**
 * @enum Especialidad
 * @brief Enum para las especialidades medicas.
 * @note Se reemplazo Traumatologia por Medico General.
 */
enum class Especialidad { CARDIOLOGIA, PEDIATRIA, DERMATOLOGIA, GINECOLOGIA, MEDICO_GENERAL };

class Cita {
private:
    Paciente paciente;
    Fecha fecha;
    Hora hora;
    Especialidad especialidad;
    string motivo;
    
public:
    /**
     * @brief Constructor de la clase Cita.
     * @param pac Paciente de la cita.
     * @param fec Fecha de la cita.
     * @param hor Hora de la cita.
     * @param esp Especialidad de la cita.
     * @param mot Motivo de la cita.
     */
    Cita(const Paciente& pac = Paciente(), const Fecha& fec = Fecha(), 
         const Hora& hor = Hora(), Especialidad esp = Especialidad::CARDIOLOGIA, 
         const string& mot = "");

    /**
     * @brief Serializa la cita en un flujo de salida.
     * @param os Flujo de salida.
     */
    void serializar(std::ostream& os) const;

    /**
     * @brief Deserializa la cita desde un flujo de entrada.
     * @param is Flujo de entrada.
     */
    void deserializar(std::istream& is);

    /**
     * @brief Obtiene el paciente de la cita.
     * @return Paciente.
     */
    Paciente getPaciente() const;

    /**
     * @brief Obtiene la fecha de la cita.
     * @return Fecha.
     */
    Fecha getFecha() const;

    /**
     * @brief Obtiene la hora de la cita.
     * @return Hora.
     */
    Hora getHora() const;

    /**
     * @brief Obtiene la especialidad de la cita.
     * @return Especialidad.
     */
    Especialidad getEspecialidad() const;

    /**
     * @brief Obtiene el motivo de la cita.
     * @return Motivo.
     */
    string getMotivo() const;

    /**
     * @brief Obtiene la especialidad como string.
     * @return Especialidad en texto.
     */
    string getEspecialidadStr() const;

    /**
     * @brief Establece el paciente de la cita.
     * @param pac Paciente.
     */
    void setPaciente(const Paciente& pac);

    /**
     * @brief Establece la fecha de la cita.
     * @param fec Fecha.
     */
    void setFecha(const Fecha& fec);

    /**
     * @brief Establece la hora de la cita.
     * @param hor Hora.
     */
    void setHora(const Hora& hor);

    /**
     * @brief Establece la especialidad de la cita.
     * @param esp Especialidad.
     */
    void setEspecialidad(Especialidad esp);

    /**
     * @brief Establece el motivo de la cita.
     * @param mot Motivo.
     */
    void setMotivo(const string& mot);

    /**
     * @brief Compara si dos citas son iguales.
     * @param otra Otra cita.
     * @return true si son iguales.
     */
    bool operator==(const Cita& otra) const;

    /**
     * @brief Muestra la informacion de la cita por consola.
     */
    void mostrarCita() const;
};

#endif // CITA_H