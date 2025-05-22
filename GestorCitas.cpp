#include "GestorCitas.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <conio.h>
#include <algorithm>

using namespace std;

GestorCitas::GestorCitas(const string& archivo) : archivoCitas(archivo) {
    cargarCitas();
}
    
GestorCitas::~GestorCitas() {
    guardarCitas();
}

void GestorCitas::mostrarMenuPrincipal() const {
    cout << "\n=== SISTEMA DE CITAS MEDICAS ===" << endl;
    cout << "1. Agendar cita" << endl;
    cout << "2. Mostrar citas" << endl;
    cout << "3. Borrar cita" << endl;
    cout << "4. Buscar por cedula" << endl;
    cout << "5. Salir" << endl;
}

void GestorCitas::mostrarMenuEspecialistas() const {
    cout << "\n=== SELECCIONE ESPECIALISTA ===" << endl;
    cout << "1. Cardiologia" << endl;
    cout << "2. Pediatria" << endl;
    cout << "3. Dermatologia" << endl;
    cout << "4. Ginecologia" << endl;
    cout << "5. Traumatologia" << endl;
}

void GestorCitas::mostrar() const {
    mostrarMenuPrincipal();
}

void GestorCitas::ejecutar() {
    int opcion;
    do {
        mostrar();
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Entrada no valida. Por favor ingrese un numero." << endl;
            continue;
        }
        
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        
        switch (opcion) {
            case 1: agendarCita(); break;
            case 2: mostrarCitas(); break;
            case 3: borrarCita(); break;
            case 4: buscarPorCedula(); break;
            case 5: cout << "Saliendo del sistema..." << endl; break;
            default: cout << "Opcion no valida. Intente nuevamente." << endl;
        }
    } while (opcion != 5);
}

Especialidad GestorCitas::seleccionarEspecialista() {
    int opcion;
    do {
        system("cls||clear");
        mostrarMenuEspecialistas();
        cout << "Opcion: ";
        cin >> opcion;
        cin.ignore();

        switch(opcion) {
            case 1: return Especialidad::CARDIOLOGIA;
            case 2: return Especialidad::PEDIATRIA;
            case 3: return Especialidad::DERMATOLOGIA;
            case 4: return Especialidad::GINECOLOGIA;
            case 5: return Especialidad::TRAUMATOLOGIA;
            default: 
                cout << "Opcion no valida. Intente nuevamente." << endl;
                cin.ignore();
        }
    } while(true);
}

bool GestorCitas::esFinDeSemana(const Fecha& fecha) const {
    struct tm tm = {0, 0, 0, fecha.getDia(), fecha.getMes()-1, fecha.getAnio()-1900};
    mktime(&tm);
    return (tm.tm_wday == 0 || tm.tm_wday == 6);
}

bool GestorCitas::esDiaValido(const Fecha& fecha, Especialidad esp) const {
    if(esFinDeSemana(fecha)) {
        cout << "No se agenda citas los fines de semana." << endl;
        return false;
    }
    if(!validarFechaCita(fecha)) {
        cout << "Fecha no valida." << endl;
        return false;
    }
    return true;
}

void GestorCitas::mostrarCalendario(int anio, int mes, int diaSeleccionado) const {
    cout << "\n  " << Fecha(1, mes, anio).getMesStr() << " " << anio << endl;
    cout << "  L  M  X  J  V  S  D" << endl;

    struct tm tm = {0, 0, 0, 1, mes - 1, anio - 1900};
    mktime(&tm);
    int diasEnMes = Fecha::diasEnMesStatic(mes, anio);  // Corregido aquí
    int diaSemana = tm.tm_wday == 0 ? 6 : tm.tm_wday - 1;

    for (int i = 0; i < diaSemana; i++) cout << "   ";

    for (int dia = 1; dia <= diasEnMes; dia++) {
        if (dia == diaSeleccionado) cout << "[" << setw(2) << dia << "]";
        else cout << " " << setw(2) << dia << " ";

        if ((dia + diaSemana) % 7 == 0) cout << endl;
    }
    cout << endl;
}

Fecha GestorCitas::seleccionarFecha(Especialidad esp) {
    Fecha hoy = Fecha::fechaActual();
    int anio = hoy.getAnio();
    int mes = hoy.getMes();
    int dia = 1; // Siempre iniciar en el primer día del mes
    bool mostrarMensaje = false;
    std::string mensajeError;

    while (true) {
        system("cls||clear");
        cout << "=== SELECCIONE FECHA ===" << endl;
        mostrarCalendario(anio, mes, dia);
        if (mostrarMensaje) {
            cout << "\n" << mensajeError << endl;
            cout << "Presione cualquier tecla para continuar..." << endl;
            getch();
            mostrarMensaje = false;
        }
        cout << "\nControles: WA - Cambiar Mes SD - Cambiar Anio, Enter - Seleccionar, ESC - Cancelar" << endl;

        int input = getch();
        switch (input) {
            case 'w': {
                if (mes == 1) { mes = 12; anio--; }
                else mes--;
                dia = min(dia, Fecha::diasEnMesStatic(mes, anio));
                break;
            }
            case 's': {
                if (mes == 12) { mes = 1; anio++; }
                else mes++;
                dia = min(dia, Fecha::diasEnMesStatic(mes, anio));
                break;
            }
            case 'a': {
                anio--;
                dia = min(dia, Fecha::diasEnMesStatic(mes, anio));
                break;
            }
            case 'd': {
                anio++;
                dia = min(dia, Fecha::diasEnMesStatic(mes, anio));
                break;
            }
            case 72: { // Flecha arriba
                dia = max(1, dia - 7);
                break;
            }
            case 80: { // Flecha abajo
                dia = min(Fecha::diasEnMesStatic(mes, anio), dia + 7);
                break;
            }
            case 75: { // Flecha izquierda
                dia = max(1, dia - 1);
                break;
            }
            case 77: { // Flecha derecha
                dia = min(Fecha::diasEnMesStatic(mes, anio), dia + 1);
                break;
            }
            case 13: { // Enter
                Fecha fechaSeleccionada(dia, mes, anio);
                if (esDiaValido(fechaSeleccionada, esp)) {
                    return fechaSeleccionada;
                } else {
                    mostrarMensaje = true;
                    mensajeError = "Fecha no valida o es fin de semana.";
                }
                break;
            }
            case 27: // ESC
                return Fecha();
        }

        int maxDias = Fecha::diasEnMesStatic(mes, anio);
        if (dia > maxDias) dia = maxDias;
    }
}


void GestorCitas::mostrarHorarioDisponible(const Fecha& fecha, Especialidad esp, int horaSeleccionada) const {
    cout << "\n=== HORARIOS DISPONIBLES ===" << endl;
    int pos = 0;
    for(int hora = 9; hora <= 17; hora++) {
        for(int minuto = 0; minuto < 60; minuto += 30) {
            Hora h(hora, minuto, 0);
            bool disponible = validarDisponibilidad(fecha, h, esp);
            if(pos == horaSeleccionada) {
                if(disponible)
                    cout << "[" << h << "] ";
                else
                    cout << "[ X ] ";
            } else if(disponible) {
                cout << " " << h << "  ";
            } else {
                cout << "  X   ";
            }
            pos++;
        }
        cout << endl;
    }
}

Hora GestorCitas::seleccionarHora(const Fecha& fecha, Especialidad esp) {
    int horaPos = 0;
    int totalHoras = 18; // 9:00-17:30, 18 slots
    bool mostrarMensaje = false;
    std::string mensajeError;

    while (true) {
        system("cls||clear");
        mostrarHorarioDisponible(fecha, esp, horaPos);
        if (mostrarMensaje) {
            cout << "\n" << mensajeError << endl;
            cout << "Presione cualquier tecla para continuar..." << endl;
            getch();
            mostrarMensaje = false;
        }
        cout << "\nControles: Flechas - Navegar, Enter - Seleccionar, ESC - Cancelar" << endl;

        int input = getch();
        switch (input) {
            case 72: { // Flecha arriba
                horaPos = max(0, horaPos - 4);
                break;
            }
            case 80: { // Flecha abajo
                horaPos = min(totalHoras - 1, horaPos + 4);
                break;
            }
            case 75: { // Flecha izquierda
                horaPos = max(0, horaPos - 1);
                break;
            }
            case 77: { // Flecha derecha
                horaPos = min(totalHoras - 1, horaPos + 1);
                break;
            }
            case 13: { // Enter
                int hora = 9 + (horaPos / 2);
                int minuto = (horaPos % 2) * 30;
                Hora hSeleccionada(hora, minuto, 0);
                if (validarDisponibilidad(fecha, hSeleccionada, esp)) {
                    return hSeleccionada;
                } else {
                    mostrarMensaje = true;
                    mensajeError = "Horario no disponible.";
                }
                break;
            }
            case 27: // ESC
                return Hora();
        }
    }
}

void GestorCitas::agendarCita() {
    Especialidad esp = seleccionarEspecialista();
    
    Fecha fechaCita = seleccionarFecha(esp);
    if(!fechaCita.esFechaValida()) {
        system("cls||clear");
        cout << "Operacion cancelada. Volviendo al menu principal..." << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        getch();
        return;
    }
    
    Hora horaCita = seleccionarHora(fechaCita, esp);
    if(!horaCita.esHoraValida()) {
        system("cls||clear");
        cout << "Operacion cancelada. Volviendo al menu principal..." << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        getch();
        return;
    }
    
    system("cls||clear");
    cout << "=== INGRESE DATOS DEL PACIENTE ===" << endl;
    
    string nombre, cedula;
    cout << "\nNombre del paciente: ";
    getline(cin, nombre);
    
    cout << "Cedula: ";
    getline(cin, cedula);
    
    if(existeCitaConCedula(cedula)) {
        cout << "Error: Ya existe una cita para esta cedula." << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        getch();
        return;
    }
    
    Fecha fechaNacimiento;
    bool fechaValida = false;
    do {
        int d, m, a;
        cout << "Fecha de nacimiento (DD MM AAAA): ";
        if (!(cin >> d >> m >> a)) {
            cout << "Entrada no valida. Intente de nuevo.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        cin.ignore();
        // Validar rangos de día y mes antes de crear la fecha
        if (m < 1 || m > 12) {
            cout << "Mes no valido. Debe estar entre 1 y 12.\n";
            cout << "Presione cualquier tecla para continuar..." << endl;
            getch();
            continue;
        
        }
        int maxDias = Fecha::diasEnMesStatic(m, a);
        if (d < 1 || d > maxDias) {
            cout << "Dia no valido para el mes seleccionado.\n";
            cout << "Presione cualquier tecla para continuar..." << endl;
            getch();
            continue;
        }
        fechaNacimiento = Fecha(d, m, a);
        fechaValida = validarFechaNacimiento(fechaNacimiento);
        if(!fechaValida) {
            cout << "Fecha no valida. Debe ser una fecha pasada (maximo 100 anos atras).\n";
            cout << "Presione cualquier tecla para continuar..." << endl;
            getch();
        }
    } while(!fechaValida);
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    string motivo;
    cout << "Motivo de la cita: ";
    getline(cin, motivo);
    
    Cita nuevaCita(Paciente(nombre, cedula, fechaNacimiento), 
            fechaCita, horaCita, esp, motivo);
    citas.insertar(nuevaCita);
    
    system("cls||clear");
    cout << "\nCita agendada exitosamente!\n";
    cout << "Presione cualquier tecla para continuar..." << endl;
    getch();
    guardarCitas();
}

bool GestorCitas::validarFechaNacimiento(const Fecha& fecha) const {
    Fecha hoy = Fecha::fechaActual();
    Fecha minimo(1, 1, hoy.getAnio() - 100);
    
    return fecha.esFechaValida() && 
        !fecha.esFechaFutura() && 
        fecha > minimo;
}

bool GestorCitas::validarFechaCita(const Fecha& fecha) const {
    Fecha hoy = Fecha::fechaActual();
    Fecha maximo(31, 12, hoy.getAnio() + 20);
    
    return fecha.esFechaValida() && 
        !fecha.esFechaPasada() && 
        fecha < maximo;
}

bool GestorCitas::existeCitaConCedula(const string& cedula) const {
    bool existe = false;
    citas.forEach([&](const Cita& cita) {
        if (cita.getPaciente().getCedula() == cedula) {
            existe = true;
        }
    });
    return existe;
}

bool GestorCitas::validarDisponibilidad(const Fecha& fecha, const Hora& hora, Especialidad esp) const {
    bool disponible = true;
    
    citas.forEach([&](const Cita& cita) {
        if (cita.getFecha() == fecha && 
            cita.getHora() == hora && 
            cita.getEspecialidad() == esp) {
            disponible = false;
        }
    });
    
    return disponible;
}

void GestorCitas::mostrarCitas() const {
    cout << "\n=== CITAS PROGRAMADAS ===" << endl;
    if (citas.estaVacia()) {
        cout << "No hay citas programadas." << endl;
        return;
    }
    
    
    citas.forEachRecursivo([](const Cita& cita) {
        cita.mostrarCita();
        cout << "-------------------------" << endl;
    });
}

void GestorCitas::buscarPorCedula() const {
    string cedula;
    cout << "Ingrese la cedula a buscar: ";
    cin >> cedula;
    cin.ignore();

    bool encontrada = false;
    citas.forEach([&](const Cita& cita) {
        if (cita.getPaciente().getCedula() == cedula) {
            cita.mostrarCita();
            cout << "-------------------------" << endl;
            encontrada = true;
        }
    });

    if (!encontrada) {
        cout << "No se encontraron citas para esta cedula." << endl;
    }
}

void GestorCitas::borrarCita() {
    mostrarCitas();
    if (citas.estaVacia()) {
        cout << "No hay citas para cancelar." << endl;
        return;
    }

    string cedula;
    int dia, mes, anio, hora, minuto;

    cout << "\n=== CANCELAR CITA ===" << endl;
    cout << "Ingrese la cedula del paciente: ";
    cin >> cedula;

    auto leerNumero = [](const string& mensaje, int& valor, int min, int max) {
        while (true) {
            cout << mensaje;
            if (cin >> valor && valor >= min && valor <= max) {
                break;
            } else {
                cout << "Error! Ingrese un valor entre " << min << " y " << max << ": ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
    };

    leerNumero("Ingrese el dia de la cita (1-31): ", dia, 1, 31);
    leerNumero("Ingrese el mes de la cita (1-12): ", mes, 1, 12);
    leerNumero("Ingrese el ano de la cita (2000-2100): ", anio, 2000, 2100);
    leerNumero("Ingrese la hora de la cita (0-23): ", hora, 0, 23);
    leerNumero("Ingrese los minutos de la cita (0-59): ", minuto, 0, 59);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Fecha fecha(dia, mes, anio);
    Hora horaCita(hora, minuto, 0);

    Cita citaABorrar(Paciente("", cedula, Fecha()), fecha, horaCita, Especialidad::CARDIOLOGIA, "");

    if (citas.eliminar(citaABorrar)) {
        cout << "Cita cancelada exitosamente." << endl;
        guardarCitas();
    } else {
        cout << "No se encontro la cita especificada." << endl;
    }
}

void GestorCitas::guardarCitas() const {
    ofstream archivo(archivoCitas, ios::binary);
    if (!archivo) {
        cerr << "Error al abrir el archivo para escritura." << endl;
        return;
    }
    
    citas.forEach([&archivo](const Cita& cita) {
        archivo.write(reinterpret_cast<const char*>(&cita), sizeof(Cita));
    });
}

void GestorCitas::cargarCitas() {
    ifstream archivo(archivoCitas, ios::binary);
    if (!archivo) return;
    
    Cita cita;
    while (archivo.read(reinterpret_cast<char*>(&cita), sizeof(Cita))) {
        citas.insertar(cita);
    }
}

void GestorCitas::agregarOpcion(const string& opcion) {
    // Implementacion si se necesita extender
}