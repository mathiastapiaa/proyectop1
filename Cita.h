#ifndef CITA_H
#define CITA_H

#include "Paciente.h"
#include "Fecha.h"
#include "Hora.h"
#include <string>

using namespace std;

enum class Especialidad { CARDIOLOGIA, PEDIATRIA, DERMATOLOGIA, GINECOLOGIA, TRAUMATOLOGIA };

class Cita {
private:
    Paciente paciente;
    Fecha fecha;
    Hora hora;
    Especialidad especialidad;
    string motivo;
    
public:
    Cita(const Paciente& pac = Paciente(), const Fecha& fec = Fecha(), 
         const Hora& hor = Hora(), Especialidad esp = Especialidad::CARDIOLOGIA, 
         const string& mot = "");
    
    Paciente getPaciente() const;
    Fecha getFecha() const;
    Hora getHora() const;
    Especialidad getEspecialidad() const;
    string getMotivo() const;
    string getEspecialidadStr() const;
    
    void setPaciente(const Paciente& pac);
    void setFecha(const Fecha& fec);
    void setHora(const Hora& hor);
    void setEspecialidad(Especialidad esp);
    void setMotivo(const string& mot);
    
    bool operator==(const Cita& otra) const;
    
    void mostrarCita() const;
};

#endif // CITA_H