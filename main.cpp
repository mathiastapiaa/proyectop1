#include "GestorCitas.h"
#include <iostream>
#include <memory>

using namespace std;

int main() {
    try {
        auto gestor = make_unique<GestorCitas>("data/citas.dat");
        gestor->ejecutar();
    } catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }
    return 0;
}