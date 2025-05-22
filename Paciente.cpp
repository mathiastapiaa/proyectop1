#include "Paciente.h"
#include <iostream>

using namespace std;

Paciente::Paciente(const string& nom, const string& ced, const Fecha& fn, const string& hist)
    : Persona(nom, ced, fn), historialMedico(hist) {}

string Paciente::getHistorialMedico() const { return historialMedico; }
void Paciente::setHistorialMedico(const string& hist) { historialMedico = hist; }

void Paciente::mostrarInformacion() const {
    Persona::mostrarInformacion();
    cout << "Historial Medico: " << historialMedico << "\n";
}