// GestorCitas.h
#ifndef GESTOR_CITAS_H
#define GESTOR_CITAS_H

#include "ListaDCircular.h"
#include "Cita.h"
#include "IMenu.h"
#include <string>
#include <vector>
#include <conio.h> // Para getch()

class GestorCitas : public IMenu {
private:
    ListaDCircular<Cita> citas;
    std::string archivoCitas;
    
    // Menús
    void mostrarMenuPrincipal() const;
    void mostrarMenuEspecialistas() const;
    
    // Nuevas funciones de selección interactiva
    Especialidad seleccionarEspecialista();
    Fecha seleccionarFecha(Especialidad esp);
    Hora seleccionarHora(const Fecha& fecha, Especialidad esp);
    void mostrarCalendario(int anio, int mes, int diaSeleccionado = 0) const;
    void mostrarHorarioDisponible(const Fecha& fecha, Especialidad esp, int horaSeleccionada = -1) const;
    
    // Validaciones
    bool esFinDeSemana(const Fecha& fecha) const;
    bool esDiaValido(const Fecha& fecha, Especialidad esp) const;
    bool validarDisponibilidad(const Fecha& fecha, const Hora& hora, Especialidad esp) const;
    bool validarFechaNacimiento(const Fecha& fecha) const;
    bool validarFechaCita(const Fecha& fecha) const;
    bool existeCitaConCedula(const std::string& cedula) const;
    
    // Funciones principales
    void agendarCita();
    void mostrarCitas() const;
    void borrarCita();
    void buscarPorCedula() const;
    
public:
    GestorCitas(const std::string& archivo = "data/citas.dat");
    ~GestorCitas();
    
    void mostrar() const override;
    void ejecutar() override;
    void agregarOpcion(const std::string& opcion) override;
    
    void guardarCitas() const;
    void cargarCitas();
    
    const ListaDCircular<Cita>& getCitas() const { return citas; }
};

#endif