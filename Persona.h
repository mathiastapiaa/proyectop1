#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include "Fecha.h"

using namespace std;

class Persona {
protected:
    string nombre;
    string cedula;
    Fecha fechaNacimiento;

public:
    Persona(const string& nom = "", const string& ced = "", 
            const Fecha& fn = Fecha());
    virtual ~Persona() = default;
    
    string getNombre() const;
    string getCedula() const;
    Fecha getFechaNacimiento() const;
    int getEdad() const;
    
    void setNombre(const string& nom);
    void setCedula(const string& ced);
    void setFechaNacimiento(const Fecha& fn);
    
    virtual void mostrarInformacion() const;
};

#endif // PERSONA_H