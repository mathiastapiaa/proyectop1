#include "Paciente.h"
#include <iostream>

using namespace std;

Paciente::Paciente(const string& nom, const string& ced, const Fecha& fn, const string& hist)
    : Persona(nom, ced, fn), historialMedico(hist) {}

void Paciente::serializar(std::ostream& os) const {
    Persona::serializar(os);
    size_t len = historialMedico.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(historialMedico.c_str(), len);
}

void Paciente::deserializar(std::istream& is) {
    Persona::deserializar(is);
    size_t len;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    char* buffer = new char[len+1];
    is.read(buffer, len); buffer[len] = '\0';
    historialMedico = buffer;
    delete[] buffer;
}

string Paciente::getHistorialMedico() const { return historialMedico; }
void Paciente::setHistorialMedico(const string& hist) { historialMedico = hist; }

void Paciente::mostrarInformacion() const {
    Persona::mostrarInformacion();
    cout << "Historial Medico: " << historialMedico << "\n";
}