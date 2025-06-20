#include "Persona.h"
#include <iostream>

using namespace std;

Persona::Persona(const string& nom, const string& ced, const Fecha& fn)
    : nombre(nom), cedula(ced), fechaNacimiento(fn) {}

void Persona::serializar(std::ostream& os) const {
    size_t len;
    len = nombre.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(nombre.c_str(), len);
    len = cedula.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(cedula.c_str(), len);
    fechaNacimiento.serializar(os);
}

void Persona::deserializar(std::istream& is) {
    size_t len;
    char* buffer;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    buffer = new char[len+1];
    is.read(buffer, len); buffer[len] = '\0';
    nombre = buffer;
    delete[] buffer;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    buffer = new char[len+1];
    is.read(buffer, len); buffer[len] = '\0';
    cedula = buffer;
    delete[] buffer;
    fechaNacimiento.deserializar(is);
}

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