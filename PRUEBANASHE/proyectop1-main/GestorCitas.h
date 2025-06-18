/**
 * @file GestorCitas.h
 * @brief Definicion de la clase GestorCitas.
 */

// GestorCitas.h
#ifndef GESTOR_CITAS_H
#define GESTOR_CITAS_H

/**
 * @class GestorCitas
 * @brief Gestiona el sistema de citas medicas.
 */
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

    // Feriados nacionales de Ecuador (día, mes)
    std::vector<std::pair<int, int>> feriados;
    void inicializarFeriados();
    void inicializarFeriados(int anio); // Nueva sobrecarga para año específico
    bool esFeriado(const Fecha& fecha) const;

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
    /**
     * @brief Constructor de la clase GestorCitas.
     * @param archivo Ruta del archivo de citas.
     */
    GestorCitas(const std::string& archivo = "data/citas.dat");

    /**
     * @brief Destructor de la clase GestorCitas.
     */
    ~GestorCitas();

    /**
     * @brief Muestra el menu principal.
     */
    void mostrar() const override;

    /**
     * @brief Ejecuta el ciclo principal del sistema.
     */
    void ejecutar() override;

    /**
     * @brief Agrega una opcion al menu (extensible).
     * @param opcion Opcion a agregar.
     */
    void agregarOpcion(const std::string& opcion) override;

    /**
     * @brief Guarda las citas en archivo.
     */
    void guardarCitas() const;

    /**
     * @brief Carga las citas desde archivo.
     */
    void cargarCitas();

    /**
     * @brief Realiza un backup de las citas.
     */
    void realizarBackup() const;

    /**
     * @brief Restaura las citas desde el backup.
     */
    void restaurarDesdeBackup();

    /**
     * @brief Muestra la ayuda del sistema.
     */
    void mostrarAyuda() const;

    /**
     * @brief Obtiene la lista de citas.
     * @return Referencia a la lista de citas.
     */
    const ListaDCircular<Cita>& getCitas() const { return citas; }

    // Devuelve los feriados del año actual (vector de pares dia, mes)
    std::vector<std::pair<int, int>> getFeriados() const { return feriados; }
    // Devuelve los feriados para un año específico (recalcula)
    std::vector<std::pair<int, int>> getFeriados(int anio) {
        inicializarFeriados(anio);
        return feriados;
    }
};

#endif