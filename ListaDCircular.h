#ifndef LISTA_DCIRCULAR_H
#define LISTA_DCIRCULAR_H

#include <functional>
#include <iostream>
#include <fstream>
#include <stdexcept>

template <typename T>
class ListaDCircular {
private:
    struct Nodo {
        T dato;
        Nodo* siguiente;
        Nodo* anterior;
        
        Nodo(const T& dato) : dato(dato), siguiente(nullptr), anterior(nullptr) {}
    };
    
    Nodo* cabeza;
    size_t tamanio;

    void forEachRecursivo(Nodo* actual, std::function<void(const T&)> func, Nodo* inicio, bool primeraVez) const {
        if (!primeraVez && actual == inicio) return;
        func(actual->dato);
        forEachRecursivo(actual->siguiente, func, inicio, false);
    }

public:
    ListaDCircular() : cabeza(nullptr), tamanio(0) {}
    
    ~ListaDCircular() {
        limpiar();
    }

    void insertar(const T& dato) {
        Nodo* nuevo = new Nodo(dato);
        
        if (cabeza == nullptr) {
            nuevo->siguiente = nuevo;
            nuevo->anterior = nuevo;
            cabeza = nuevo;
        } else {
            Nodo* ultimo = cabeza->anterior;
            
            nuevo->siguiente = cabeza;
            nuevo->anterior = ultimo;
            
            ultimo->siguiente = nuevo;
            cabeza->anterior = nuevo;
        }
        tamanio++;
    }

    bool eliminar(const T& dato) {
        if (cabeza == nullptr) return false;

        Nodo* actual = cabeza;
        bool encontrado = false;

        do {
            if (actual->dato == dato) {
                encontrado = true;
                break;
            }
            actual = actual->siguiente;
        } while (actual != cabeza);

        if (!encontrado) return false;

        return eliminarNodo(actual);
    }

    bool eliminarNodo(Nodo* nodoAEliminar) {
        if (nodoAEliminar == nullptr) return false;

        if (tamanio == 1) {
            cabeza = nullptr;
        } else {
            nodoAEliminar->anterior->siguiente = nodoAEliminar->siguiente;
            nodoAEliminar->siguiente->anterior = nodoAEliminar->anterior;

            if (nodoAEliminar == cabeza) {
                cabeza = nodoAEliminar->siguiente;
            }
        }

        delete nodoAEliminar;
        tamanio--;
        return true;
    }

    bool buscar(const T& dato) const {
        if (cabeza == nullptr) return false;
        
        Nodo* actual = cabeza;
        
        do {
            if (actual->dato == dato) {
                return true;
            }
            actual = actual->siguiente;
        } while (actual != cabeza);
        
        return false;
    }

    void forEach(std::function<void(const T&)> func) const {
        if (cabeza == nullptr) return;
        
        Nodo* actual = cabeza;
        
        do {
            func(actual->dato);
            actual = actual->siguiente;
        } while (actual != cabeza);
    }

    void forEachRecursivo(std::function<void(const T&)> func) const {
        if (cabeza != nullptr) {
            forEachRecursivo(cabeza, func, cabeza, true);
        }
    }

    void limpiar() {
        while (!estaVacia()) {
            eliminarNodo(cabeza);
        }
    }

    size_t getTamanio() const { return tamanio; }
    bool estaVacia() const { return tamanio == 0; }

    void guardarEnArchivo(const std::string& nombreArchivo) const {
        std::ofstream archivo(nombreArchivo, std::ios::binary);
        if (!archivo) {
            throw std::runtime_error("No se pudo abrir el archivo para escritura");
        }
        
        forEach([&archivo](const T& dato) {
            archivo.write(reinterpret_cast<const char*>(&dato), sizeof(T));
        });
    }

    void cargarDesdeArchivo(const std::string& nombreArchivo) {
        std::ifstream archivo(nombreArchivo, std::ios::binary);
        if (!archivo) return;
        
        limpiar();
        
        T dato;
        while (archivo.read(reinterpret_cast<char*>(&dato), sizeof(T))) {
            insertar(dato);
        }
    }
};

#endif // LISTA_DCIRCULAR_H