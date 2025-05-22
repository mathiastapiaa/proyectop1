#ifndef PACIENTE_H
#define PACIENTE_H

#include "Persona.h"
using namespace std;

class Paciente : public Persona {
private:
    string historialMedico;

public:
    Paciente(const string& nom = "", const string& ced = "", 
             const Fecha& fn = Fecha(), const string& hist = "");
    
    string getHistorialMedico() const;
    void setHistorialMedico(const string& hist);
    
    void mostrarInformacion() const override;
};

#endif // PACIENTE_H    