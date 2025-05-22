#include "Persona.h"
#include <iostream>

using namespace std;

Persona::Persona(const string& nom, const string& ced, const Fecha& fn)
    : nombre(nom), cedula(ced), fechaNacimiento(fn) {}

string Persona::getNombre() const { return nombre; }
string Persona::getCedula() const { return cedula; }
Fecha Persona::getFechaNacimiento() const { return fechaNacimiento; }

int Persona::getEdad() const {
    return fechaNacimiento.calcularEdad();
}

void Persona::setNombre(const string& nom) { nombre = nom; }
void Persona::setCedula(const string& ced) { cedula = ced; }
void Persona::setFechaNacimiento(const Fecha& fn) { fechaNacimiento = fn; }

void Persona::mostrarInformacion() const {
    cout << "Nombre: " << nombre << "\n";
    cout << "Cedula: " << cedula << "\n";
    cout << "Edad: " << getEdad() << " anos\n";
}