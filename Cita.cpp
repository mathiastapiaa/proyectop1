/**
 * @file Cita.cpp
 * @brief Implementacion de la clase Cita.
 */
#include "Cita.h"
#include <iostream>

using namespace std;

Cita::Cita(const Paciente& pac, const Fecha& fec, const Hora& hor, Especialidad esp, const string& mot)
    : paciente(pac), fecha(fec), hora(hor), especialidad(esp), motivo(mot) {}

void Cita::serializar(std::ostream& os) const {
    paciente.serializar(os);
    fecha.serializar(os);
    hora.serializar(os);
    int esp = static_cast<int>(especialidad);
    os.write(reinterpret_cast<const char*>(&esp), sizeof(esp));
    size_t len = motivo.size();
    os.write(reinterpret_cast<const char*>(&len), sizeof(len));
    os.write(motivo.c_str(), len);
}

void Cita::deserializar(std::istream& is) {
    paciente.deserializar(is);
    fecha.deserializar(is);
    hora.deserializar(is);
    int esp;
    is.read(reinterpret_cast<char*>(&esp), sizeof(esp));
    especialidad = static_cast<Especialidad>(esp);
    size_t len;
    is.read(reinterpret_cast<char*>(&len), sizeof(len));
    char* buffer = new char[len+1];
    is.read(buffer, len); buffer[len] = '\0';
    motivo = buffer;
    delete[] buffer;
}

Paciente Cita::getPaciente() const { return paciente; }
Fecha Cita::getFecha() const { return fecha; }
Hora Cita::getHora() const { return hora; }
Especialidad Cita::getEspecialidad() const { return especialidad; }
string Cita::getMotivo() const { return motivo; }

string Cita::getEspecialidadStr() const {
    switch(especialidad) {
        case Especialidad::CARDIOLOGIA: return "Cardiologia";
        case Especialidad::PEDIATRIA: return "Pediatria";
        case Especialidad::DERMATOLOGIA: return "Dermatologia";
        case Especialidad::GINECOLOGIA: return "Ginecologia";
        case Especialidad::MEDICO_GENERAL: return "Medico General";
        default: return "Desconocida";
    }
}

void Cita::setPaciente(const Paciente& pac) { paciente = pac; }
void Cita::setFecha(const Fecha& fec) { fecha = fec; }
void Cita::setHora(const Hora& hor) { hora = hor; }
void Cita::setEspecialidad(Especialidad esp) { especialidad = esp; }
void Cita::setMotivo(const string& mot) { motivo = mot; }

bool Cita::operator==(const Cita& otra) const {
    return paciente.getCedula() == otra.paciente.getCedula() &&
           fecha == otra.fecha &&
           hora == otra.hora &&
           especialidad == otra.especialidad;
}

void Cita::mostrarCita() const {
    cout << "Paciente: " << paciente.getNombre() << "\n";
    cout << "Cedula: " << paciente.getCedula() << "\n";
    cout << "Fecha: " << fecha << "\n";
    cout << "Hora: " << hora << "\n";
    cout << "Especialidad: " << getEspecialidadStr() << "\n";
    cout << "Motivo: " << motivo << "\n";
}   