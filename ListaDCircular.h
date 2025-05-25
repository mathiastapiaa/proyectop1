/**
 * @file ListaDCircular.h
 * @brief Definicion de la clase plantilla ListaDCircular.
 */

#ifndef LISTA_DCIRCULAR_H
#define LISTA_DCIRCULAR_H

#include <functional>
#include <iostream>
#include <fstream>
#include <stdexcept>

/**
 * @class ListaDCircular
 * @brief Lista doblemente circular generica.
 * @tparam T Tipo de dato almacenado.
 */

template <typename T>
/**
 * @class ListaDCircular
 * @brief Lista doblemente circular generica.
 * @tparam T Tipo de dato almacenado.
 */
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

    /**
     * @brief Inserta un elemento en la lista.
     * @param dato Elemento a insertar.
     */
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

    /**
     * @brief Elimina un elemento de la lista.
     * @param dato Elemento a eliminar.
     * @return true si se elimino.
     */
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

    /**
     * @brief Elimina un nodo especifico de la lista.
     * @param nodoAEliminar Nodo a eliminar.
     * @return true si se elimino.
     */
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

    /**
     * @brief Busca un elemento en la lista.
     * @param dato Elemento a buscar.
     * @return true si se encontro.
     */
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

    /**
     * @brief Aplica una funcion a cada elemento de la lista.
     * @param func Funcion a aplicar.
     */
    void forEach(std::function<void(const T&)> func) const {
        if (cabeza == nullptr) return;
        
        Nodo* actual = cabeza;
        
        do {
            func(actual->dato);
            actual = actual->siguiente;
        } while (actual != cabeza);
    }

    /**
     * @brief Aplica una funcion a cada elemento de la lista de forma recursiva.
     * @param func Funcion a aplicar.
     */
    void forEachRecursivo(std::function<void(const T&)> func) const {
        if (cabeza != nullptr) {
            forEachRecursivo(cabeza, func, cabeza, true);
        }
    }

    /**
     * @brief Elimina todos los elementos de la lista.
     */
    void limpiar() {
        while (!estaVacia()) {
            eliminarNodo(cabeza);
        }
    }

    /**
     * @brief Obtiene el tamanio de la lista.
     * @return Tamanio.
     */
    size_t getTamanio() const { return tamanio; }
    /**
     * @brief Indica si la lista esta vacia.
     * @return true si esta vacia.
     */
    bool estaVacia() const { return tamanio == 0; }

    // Métodos de serialización/deserialización para tipos con métodos propios
    template<typename U = T>
    typename std::enable_if<std::is_member_function_pointer<decltype(&U::serializar)>::value>::type
    /**
     * @brief Guarda la lista en un archivo binario.
     * @param nombreArchivo Nombre del archivo.
     */
    guardarEnArchivo(const std::string& nombreArchivo) const {
        std::ofstream archivo(nombreArchivo, std::ios::binary);
        if (!archivo) {
            throw std::runtime_error("No se pudo abrir el archivo para escritura");
        }
        size_t cantidad = tamanio;
        archivo.write(reinterpret_cast<const char*>(&cantidad), sizeof(cantidad));
        forEach([&archivo](const T& dato) {
            dato.serializar(archivo);
        });
    }

    template<typename U = T>
    typename std::enable_if<std::is_member_function_pointer<decltype(&U::deserializar)>::value>::type
    /**
     * @brief Carga la lista desde un archivo binario.
     * @param nombreArchivo Nombre del archivo.
     */
    cargarDesdeArchivo(const std::string& nombreArchivo) {
        std::ifstream archivo(nombreArchivo, std::ios::binary);
        if (!archivo) return;
        limpiar();
        size_t cantidad = 0;
        archivo.read(reinterpret_cast<char*>(&cantidad), sizeof(cantidad));
        for (size_t i = 0; i < cantidad; ++i) {
            T dato;
            dato.deserializar(archivo);
            insertar(dato);
        }
    }
};

#endif // LISTA_DCIRCULAR_H