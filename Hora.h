#ifndef HORA_H
#define HORA_H

#include <iostream>
#include <stdexcept>

using namespace std;

class Hora {
private:
    int hora, minuto, segundo;
    void validarHora() const;

public:
    Hora(int h = -1, int m = -1, int s = -1);
    Hora(const Hora& otra);
    
    int getHora() const;
    int getMinuto() const;
    int getSegundo() const;
    
    void setHora(int h);
    void setMinuto(int m);
    void setSegundo(int s);
    
    bool operator<(const Hora& otra) const;
    bool operator>(const Hora& otra) const;
    bool operator==(const Hora& otra) const;
    
    bool esHoraValida() const;
    
    friend ostream& operator<<(ostream& os, const Hora& hora);
    friend istream& operator>>(istream& is, Hora& hora);
};

#endif // HORA_H