#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#undef byte

#include <iostream>
#include <iomanip>
#include <limits>
#include <ctime>
#include <conio.h>
#include <algorithm>
#include "GestorCitas.h"
#include "IngresoDatosDLL.h"


using namespace std;

// Prototipo para la función de validación de cédula ecuatoriana
bool validarCedulaEcuatoriana(const char* cedula);

GestorCitas::   GestorCitas(const string& archivo) : archivoCitas(archivo) {
    inicializarFeriados();
    cargarCitas();
}
// Algoritmo de Meeus/Jones/Butcher para calcular la fecha de Pascua
static Fecha calcularPascua(int anio) {
    int a = anio % 19;
    int b = anio / 100;
    int c = anio % 100;
    int d = b / 4;
    int e = b % 4;
    int f = (b + 8) / 25;
    int g = (b - f + 1) / 3;
    int h = (19 * a + b - d - g + 15) % 30;
    int i = c / 4;
    int k = c % 4;
    int l = (32 + 2 * e + 2 * i - h - k) % 7;
    int m = (a + 11 * h + 22 * l) / 451;
    int mes = (h + l - 7 * m + 114) / 31;
    int dia = ((h + l - 7 * m + 114) % 31) + 1;
    return Fecha(dia, mes, anio);
}

void GestorCitas::inicializarFeriados() {
    feriados.clear();
    // Feriados nacionales fijos (día, mes)
    feriados.push_back({1, 1});    // Año Nuevo
    feriados.push_back({1, 5});    // Día del Trabajo
    feriados.push_back({24, 5});   // Batalla de Pichincha
    feriados.push_back({10, 8});   // Primer Grito de Independencia
    feriados.push_back({9, 10});   // Independencia de Guayaquil
    feriados.push_back({2, 11});   // Día de los Difuntos
    feriados.push_back({3, 11});   // Independencia de Cuenca
    feriados.push_back({25, 12});  // Navidad
    // Feriados móviles: Carnaval, Viernes Santo, Domingo de Pascua
    int anioActual = Fecha::fechaActual().getAnio();
    for (int offset = -1; offset <= 1; ++offset) { // Para agendar con anticipación/futuro
        int anio = anioActual + offset;
        Fecha pascua = calcularPascua(anio);
        // Carnaval: lunes y martes antes de Pascua (47 y 48 días antes)
        for (int diasAntes : {48, 47}) {
            int d = pascua.getDia(), m = pascua.getMes(), a = pascua.getAnio();
            // Retroceder días
            for (int i = 0; i < diasAntes; ++i) {
                d--;
                if (d < 1) {
                    m--;
                    if (m < 1) { m = 12; a--; }
                    d = Fecha::diasEnMesStatic(m, a);
                }
            }
            feriados.push_back({d, m});
        }
        // Viernes Santo: 2 días antes de Pascua
        int d = pascua.getDia(), m = pascua.getMes(), a = pascua.getAnio();
        for (int i = 0; i < 2; ++i) {
            d--;
            if (d < 1) {
                m--;
                if (m < 1) { m = 12; a--; }
                d = Fecha::diasEnMesStatic(m, a);
            }
        }
        feriados.push_back({d, m});
        // Domingo de Pascua
        feriados.push_back({pascua.getDia(), pascua.getMes()});
    }
}

// Devuelve true si la fecha es feriado nacional
bool GestorCitas::esFeriado(const Fecha& fecha) const {
    int d = fecha.getDia();
    int m = fecha.getMes();
    for (const auto& f : feriados) {
        if (f.first == d && f.second == m) return true;
    }
    return false;
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
    cout << "5. Realizar backup" << endl;
    cout << "6. Restaurar desde backup" << endl;
    cout << "7. Salir" << endl;
    cout << "8. Ayuda" << endl;
}

void GestorCitas::mostrarMenuEspecialistas() const {
    cout << "\n=== SELECCIONE ESPECIALISTA ===" << endl;
    cout << "1. Cardiologia" << endl;
    cout << "2. Pediatria" << endl;
    cout << "3. Dermatologia" << endl;
    cout << "4. Ginecologia" << endl;
    cout << "5. Medico General" << endl;
}

void GestorCitas::mostrar() const {
    mostrarMenuPrincipal();
}

void GestorCitas::ejecutar() {
    int opcion = 0;
    do {
        mostrar();
        // Entrada segura solo números 1-8 usando getch
        opcion = 0;
        cout << "Seleccione una opcion (1-8): ";
        string input;
        while (true) {
            int ch = getch();
            if (ch == 13) { // Enter
                if (!input.empty()) break;
            } else if (ch == 8) { // Backspace
                if (!input.empty()) {
                    input.pop_back();
                    cout << "\b \b";
                }
            } else if (ch >= '1' && ch <= '8' && input.empty()) {
                input += (char)ch;
                cout << (char)ch;
            }
        }
        cout << endl;
        opcion = input.empty() ? 0 : (input[0] - '0');

        switch (opcion) {
            case 1: agendarCita(); break;
            case 2: mostrarCitas(); break;
            case 3: borrarCita(); break;
            case 4: buscarPorCedula(); break;
            case 5: realizarBackup(); break;
            case 6: restaurarDesdeBackup(); break;
            case 7: cout << "Saliendo del sistema..." << endl; break;
            case 8: mostrarAyuda(); break;
            default: cout << "Opcion no valida. Intente nuevamente." << endl;
        }
    } while (opcion != 7);
}
// Opcion de ayuda
void GestorCitas::mostrarAyuda() const {
    system("cls||clear");
    cout << "=== AYUDA DEL SISTEMA DE CITAS MEDICAS ===\n";
    cout << "\nEste sistema permite gestionar citas medicas de manera sencilla y segura.\n";
    cout << "\nFUNCIONALIDADES PRINCIPALES:\n";
    cout << "1. Agendar cita: Permite registrar una nueva cita medica para un paciente.\n";
    cout << "2. Mostrar citas: Muestra todas las citas programadas.\n";
    cout << "3. Borrar cita: Permite cancelar una cita existente ingresando los datos correspondientes.\n";
    cout << "4. Buscar por cedula: Busca y muestra las citas asociadas a una cedula de paciente.\n";
    cout << "5. Realizar backup: Crea una copia de seguridad de todas las citas en 'data/citas_backup.dat'.\n";
    cout << "6. Restaurar desde backup: Recupera todas las citas desde el archivo de backup si se pierde o dana el archivo principal.\n";
    cout << "7. Salir: Cierra el sistema de citas medicas.\n";
    cout << "8. Ayuda: Muestra esta pantalla de ayuda.\n";
    cout << "\nDETALLES IMPORTANTES:\n";
    cout << "- El sistema valida que las citas no se agenden en fines de semana ni feriados nacionales.\n";
    cout << "- Solo se permiten fechas validas y futuras para agendar citas.\n";
    cout << "- El backup protege tus datos ante perdidas o errores.\n";
    cout << "- Puedes restaurar tus citas desde el backup si borras o pierdes el archivo principal.\n";
    cout << "\nPara regresar al menu principal, presione Enter..." << endl;
    cin.get();
    system("cls||clear");
}

Especialidad GestorCitas::seleccionarEspecialista() {
    int opcion = 0;
    do {
        system("cls||clear");
        mostrarMenuEspecialistas();
        cout << "Opcion (1-5): ";
        std::string input;
        while (true) {
            int ch = getch();
            if (ch == 13) { // Enter
                if (!input.empty()) break;
            } else if (ch == 8) { // Backspace
                if (!input.empty()) {
                    input.pop_back();
                    cout << "\b \b";
                }
            } else if (ch >= '1' && ch <= '5' && input.empty()) {
                input += (char)ch;
                cout << (char)ch;
            }
        }
        cout << endl;
        opcion = input.empty() ? 0 : (input[0] - '0');
        switch(opcion) {
            case 1: return Especialidad::CARDIOLOGIA;
            case 2: return Especialidad::PEDIATRIA;
            case 3: return Especialidad::DERMATOLOGIA;
            case 4: return Especialidad::GINECOLOGIA;
            case 5: return Especialidad::MEDICO_GENERAL;
            default:
                cout << "Opcion no valida. Intente nuevamente." << endl;
        }
    } while(true);
}

bool GestorCitas::esFinDeSemana(const Fecha& fecha) const {
    struct tm tm = {0, 0, 0, fecha.getDia(), fecha.getMes()-1, fecha.getAnio()-1900};
    mktime(&tm);
    return (tm.tm_wday == 0 || tm.tm_wday == 6);
}

bool GestorCitas::esDiaValido(const Fecha& fecha, Especialidad esp) const {
    if (esFinDeSemana(fecha)) {
    cout << "No se agenda citas los fines de semana." << endl;
        return false;
    }
    if (esFeriado(fecha)) {
    cout << "No se agenda citas en dias feriados nacionales." << endl;
        return false;
    }
    if (!validarFechaCita(fecha)) {
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
        cout << "\nControles: WA - Cambiar Mes SD - Cambiar Anio, Enter - Seleccionar, C - Cancelar" << endl;

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
            case 'c':
                system("cls||clear");
                return Fecha();
            case 'C':
                system("cls||clear");
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
        cout << "\nControles: Flechas - Navegar, Enter - Seleccionar, C - Cancelar" << endl;

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
            case 'c':
                system("cls||clear");
                return Hora();
            case 'C':
                system("cls||clear");
                return Hora();
        }
    }
}

void GestorCitas::agendarCita() {
    Especialidad esp = seleccionarEspecialista();
    Fecha fechaCita = seleccionarFecha(esp);
    if (!fechaCita.esFechaValida()) return;
    Hora horaCita = seleccionarHora(fechaCita, esp);
    if (!horaCita.esHoraValida()) return;
    system("cls||clear");
    cout << "=== INGRESE DATOS DEL PACIENTE ===" << endl;

    HINSTANCE hDll = LoadLibrary(TEXT("IngresoDatosDLL.dll"));
    if (!hDll) {
        cout << "No se pudo cargar el DLL de ingreso de datos." << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        getch();
        return;
    }
    typedef void (__stdcall *IngresarDatoFunc)(const char*, char*, int);
    IngresarDatoFunc ingresarDato = (IngresarDatoFunc)GetProcAddress(hDll, "ingresarDato");
    if (!ingresarDato) {
        cout << "No se encontro la funcion ingresarDato en el DLL." << endl;
        FreeLibrary(hDll);
        cout << "Presione cualquier tecla para continuar..." << endl;
        getch();
        return;
    }

    char nombre[100], cedula[11], motivo[200], fechaNacStr[20];

    // Nombre: solo letras y espacios (validación con getch)
    auto ingresarNombreSoloLetras = [] (char* buffer, int maxLen) {
        int pos = 0;
        cout << "\nNombre del paciente: ";
        while (true) {
            int ch = getch();
            if (ch == 13) { // Enter
                if (pos > 0) break;
            } else if (ch == 8) { // Backspace
                if (pos > 0) {
                    pos--;
                    cout << "\b \b";
                }
            } else if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z') || ch == ' ' || ch == 160 || ch == 130 || ch == 181 || ch == 144 || ch == 214 || ch == 224) {
                if (pos < maxLen - 1) {
                    buffer[pos++] = (char)ch;
                    cout << (char)ch;
                }
            }
        }
        buffer[pos] = '\0';
        cout << endl;
    };
    ingresarNombreSoloLetras(nombre, 100);

    // Cedula: solo 10 dígitos, solo números usando getch
    auto ingresarCedulaSoloNumeros = [] (char* buffer, int maxLen) {
        int pos = 0;
        cout << "Cedula (10 digitos): ";
        while (true) {
            int ch = getch();
            if (ch == 13) { // Enter
                if (pos == 10) break;
            } else if (ch == 8) { // Backspace
                if (pos > 0) {
                    pos--;
                    cout << "\b \b";
                }
            } else if (ch >= '0' && ch <= '9' && pos < 10) {
                buffer[pos++] = (char)ch;
                cout << (char)ch;
            }
        }
        buffer[pos] = '\0';
        cout << endl;
    };
    do {
        ingresarCedulaSoloNumeros(cedula, 11);
        if (!validarCedulaEcuatoriana(cedula)) {
            cout << "Cedula no valida. Intente de nuevo." << endl;
        }
    } while (strlen(cedula) != 10 || !validarCedulaEcuatoriana(cedula));

    // Permitir solo una cita por cedula, por dia
    bool yaTieneCitaEseDia = false;
    citas.forEach([&](const Cita& cita) {
        if (cita.getPaciente().getCedula() == cedula && cita.getFecha() == fechaCita) {
            yaTieneCitaEseDia = true;
        }
    });
    if (yaTieneCitaEseDia) {
        cout << "Error: Ya existe una cita para esta cedula en ese dia." << endl;
        cout << "Presione cualquier tecla para continuar..." << endl;
        getch();
        FreeLibrary(hDll);
        return;
    }

    // Fecha de nacimiento: solo números y '/'
    auto ingresarFechaNacimientoSeguro = [] (char* buffer, int maxLen) {
        int pos = 0;
        cout << "Fecha de nacimiento (DD/MM/AAAA): ";
        while (true) {
            int ch = getch();
            if (ch == 13) { // Enter
                if (pos > 0) break;
            } else if (ch == 8) { // Backspace
                if (pos > 0) {
                    pos--;
                    cout << "\b \b";
                }
            } else if ((ch >= '0' && ch <= '9') || ch == '/') {
                if (pos < maxLen - 1) {
                    buffer[pos++] = (char)ch;
                    cout << (char)ch;
                }
            }
        }
        buffer[pos] = '\0';
        cout << endl;
    };

    Fecha fechaNacimiento;
    bool fechaValida = false;
    do {
        ingresarFechaNacimientoSeguro(fechaNacStr, 20);
        int d = 0, m = 0, a = 0;
        char sep1 = '/', sep2 = '/';
        std::istringstream iss(fechaNacStr);
        iss >> d >> sep1 >> m >> sep2 >> a;
        if (iss.fail() || sep1 != '/' || sep2 != '/' || d < 1 || m < 1 || a < 1900) {
            cout << "Entrada no valida. Intente de nuevo.\n";
            continue;
        }
        if (m < 1 || m > 12) {
            cout << "Mes no valido. Debe estar entre 1 y 12.\n";
            continue;
        }
        int maxDias = Fecha::diasEnMesStatic(m, a);
        if (d < 1 || d > maxDias) {
            cout << "Dia no valido para el mes seleccionado.\n";
            continue;
        }
        try {
            fechaNacimiento = Fecha(d, m, a);
        } catch (const std::exception& e) {
            cout << "Fecha no valida: " << e.what() << endl;
            continue;
        }
        fechaValida = validarFechaNacimiento(fechaNacimiento);
        if (!fechaValida) {
            cout << "Fecha no valida. Debe ser una fecha pasada (maximo 100 anos atras).\n";
        }
    } while (!fechaValida);

    ingresarDato("Motivo de la cita: ", motivo, 200);
    Cita nuevaCita(Paciente(nombre, cedula, fechaNacimiento), fechaCita, horaCita, esp, motivo);
    citas.insertar(nuevaCita);
    FreeLibrary(hDll);
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
    system("cls||clear");
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
    char cedula[11] = {0};
    HINSTANCE hDll = LoadLibrary(TEXT("IngresoDatosDLL.dll"));
    typedef void (__stdcall *IngresarDatoFunc)(const char*, char*, int);
    IngresarDatoFunc ingresarDato = (IngresarDatoFunc)GetProcAddress(hDll, "ingresarDato");

    // Cedula: solo 10 dígitos, solo números usando getch
    auto ingresarCedulaSoloNumeros = [] (char* buffer, int maxLen) {
        int pos = 0;
        cout << "Ingrese la cedula a buscar (10 digitos): ";
        while (true) {
            int ch = getch();
            if (ch == 13) { // Enter
                if (pos == 10) break;
            } else if (ch == 8) { // Backspace
                if (pos > 0) {
                    pos--;
                    cout << "\b \b";
                }
            } else if (ch >= '0' && ch <= '9' && pos < 10) {
                buffer[pos++] = (char)ch;
                cout << (char)ch;
            }
        }
        buffer[pos] = '\0';
        cout << endl;
    };
    do {
        ingresarCedulaSoloNumeros(cedula, 11);
        if (!validarCedulaEcuatoriana(cedula)) {
            cout << "Cedula no valida. Intente de nuevo." << endl;
        }
    } while (strlen(cedula) != 10 || !validarCedulaEcuatoriana(cedula));

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
    FreeLibrary(hDll);
}

void GestorCitas::borrarCita() {
    system("cls||clear");
    mostrarCitas();
    if (citas.estaVacia()) {
        cout << "No hay citas para cancelar." << endl;
        return;
    }

    char cedula[11] = {0}, buffer[8] = {0};
    int dia, mes, anio, hora, minuto;

    HINSTANCE hDll = LoadLibrary(TEXT("IngresoDatosDLL.dll"));
    typedef void (__stdcall *IngresarDatoFunc)(const char*, char*, int);
    IngresarDatoFunc ingresarDato = (IngresarDatoFunc)GetProcAddress(hDll, "ingresarDato");

    cout << "\n=== CANCELAR CITA ===" << endl;

    // Cedula: solo 10 dígitos, solo números usando getch
    auto ingresarCedulaSoloNumeros = [] (char* buffer, int maxLen) {
        int pos = 0;
        cout << "Ingrese la cedula del paciente (10 digitos): ";
        while (true) {
            int ch = getch();
            if (ch == 13) { // Enter
                if (pos == 10) break;
            } else if (ch == 8) { // Backspace
                if (pos > 0) {
                    pos--;
                    cout << "\b \b";
                }
            } else if (ch >= '0' && ch <= '9' && pos < 10) {
                buffer[pos++] = (char)ch;
                cout << (char)ch;
            }
        }
        buffer[pos] = '\0';
        cout << endl;
    };
    do {
        ingresarCedulaSoloNumeros(cedula, 11);
        if (!validarCedulaEcuatoriana(cedula)) {
            cout << "Cedula no valida. Intente de nuevo." << endl;
        }
    } while (strlen(cedula) != 10 || !validarCedulaEcuatoriana(cedula));

    // Día, Mes, Año, Hora, Minuto: solo números válidos y longitud adecuada
    auto ingresarNumeroRango = [](const char* mensaje, int min, int max, int maxLen) -> int {
        char buffer[5] = {0};
        int pos = 0;
        int valor = 0;
        while (true) {
            cout << mensaje;
            pos = 0;
            while (true) {
                int ch = getch();
                if (ch == 13) { // Enter
                    if (pos > 0) break;
                } else if (ch == 8) { // Backspace
                    if (pos > 0) {
                        pos--;
                        cout << "\b \b";
                    }
                } else if (ch >= '0' && ch <= '9' && pos < maxLen) {
                    buffer[pos++] = (char)ch;
                    cout << (char)ch;
                }
            }
            buffer[pos] = '\0';
            cout << endl;
            valor = atoi(buffer);
            if (valor >= min && valor <= max) break;
            cout << "Valor fuera de rango. Intente de nuevo.\n";
        }
        return valor;
    };

    dia = ingresarNumeroRango("Ingrese el dia de la cita (1-31): ", 1, 31, 2);
    mes = ingresarNumeroRango("Ingrese el mes de la cita (1-12): ", 1, 12, 2);
    anio = ingresarNumeroRango("Ingrese el ano de la cita (2000-2100): ", 2000, 2100, 4);
    hora = ingresarNumeroRango("Ingrese la hora de la cita (0-23): ", 0, 23, 2);
    minuto = ingresarNumeroRango("Ingrese los minutos de la cita (0-59): ", 0, 59, 2);

    Fecha fecha(dia, mes, anio);
    Hora horaCita(hora, minuto, 0);

    Cita citaABorrar(Paciente("", cedula, Fecha()), fecha, horaCita, Especialidad::CARDIOLOGIA, "");

    if (citas.eliminar(citaABorrar)) {
        cout << "Cita cancelada exitosamente." << endl;
        guardarCitas();
    } else {
        cout << "No se encontro la cita especificada." << endl;
    }
    FreeLibrary(hDll);
}

void GestorCitas::guardarCitas() const {
    try {
        citas.guardarEnArchivo(archivoCitas);
    } catch (const std::exception& e) {
        cerr << "Error al guardar citas: " << e.what() << endl;
    }
}

void GestorCitas::cargarCitas() {
    citas.cargarDesdeArchivo(archivoCitas);
    cout << "Citas cargadas: " << citas.getTamanio() <<endl;
}

void GestorCitas::agregarOpcion(const string& opcion) {
    // Implementacion si se necesita extender
}

void GestorCitas::realizarBackup() const {
    std::ifstream archivoOriginal(archivoCitas, std::ios::binary);
    if (!archivoOriginal) {
        std::cerr << "No se pudo abrir el archivo original para el backup." << std::endl;
        return;
    }

    std::ofstream archivoBackup("data/citas_backup.dat", std::ios::binary);
    if (!archivoBackup) {
        std::cerr << "No se pudo crear el archivo de backup." << std::endl;
        return;
    }

    archivoBackup << archivoOriginal.rdbuf();
    std::cout << "Backup realizado exitosamente." << std::endl;
}

void GestorCitas::restaurarDesdeBackup() {
    std::ifstream archivoBackup("data/citas_backup.dat", std::ios::binary);
    if (!archivoBackup) {
        std::cerr << "No se encontro un archivo de backup para restaurar." << std::endl;
        return;
    }

    std::ofstream archivoOriginal(archivoCitas, std::ios::binary);
    if (!archivoOriginal) {
        std::cerr << "No se pudo abrir el archivo original para la restauracion." << std::endl;
        return;
    }

    archivoOriginal << archivoBackup.rdbuf();
    std::cout << "Restauracion desde backup realizada exitosamente." << std::endl;
}

bool validarCedulaEcuatoriana(const char* cedula) {
    if (strlen(cedula) != 10) return false;
    int provincia = (cedula[0] - '0') * 10 + (cedula[1] - '0');
    if (provincia < 1 || provincia > 24) return false;
    if (cedula[2] < '0' || cedula[2] > '5') return false;

    int suma = 0;
    for (int i = 0; i < 9; i++) {
        int dig = cedula[i] - '0';
        if (i % 2 == 0) { // posiciones impares
            dig *= 2;
            if (dig > 9) dig -= 9;
        }
        suma += dig;
    }
    int verificador = (10 - (suma % 10)) % 10;
    return verificador == (cedula[9] - '0');
}