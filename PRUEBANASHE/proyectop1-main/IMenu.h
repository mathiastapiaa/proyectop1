#ifndef IMENU_H
#define IMENU_H

#include <string>
#include <vector>

using namespace std;

class IMenu {
public:
    virtual ~IMenu() = default;
    virtual void mostrar() const = 0;
    virtual void ejecutar() = 0;
    virtual void agregarOpcion(const string& opcion) = 0;
};

#endif // IMENU_H
