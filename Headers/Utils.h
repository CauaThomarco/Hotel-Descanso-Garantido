#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

// Estrutura para Data
class Data {
public:
    int dia;
    int mes;
    int ano;
    int hora;

    Data();
    Data(int d, int m, int a, int h = 14);
    
    bool validar() const;
    void exibir() const;
    int compararCom(const Data& outra) const;
    
    // Sobrecarga de operadores
    bool operator<(const Data& outra) const;
    bool operator>(const Data& outra) const;
    bool operator==(const Data& outra) const;
    bool operator<=(const Data& outra) const;
    bool operator>=(const Data& outra) const;
};

// Funções utilitárias
void limparBuffer();
void pausar();
int calcularDiarias(const Data& entrada, const Data& saida);
Data lerData(const string& mensagem);
bool periodosSesobrepoe(const Data& e1, const Data& s1, const Data& e2, const Data& s2);

#endif