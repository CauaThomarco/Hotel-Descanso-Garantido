#include "../Headers/Utils.h"
#include <iostream>
#include <iomanip>
#include <limits>
#include <string>

using namespace std;


// Construtores da classe Data
Data::Data() : dia(1), mes(1), ano(2025), hora(14) {}

Data::Data(int d, int m, int a, int h) : dia(d), mes(m), ano(a), hora(h) {}

// Validação de data
bool Data::validar() const {
    if (ano < 1900 || ano > 2100) return false;
    if (mes < 1 || mes > 12) return false;
    
    int diasPorMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    // Ano bissexto
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        diasPorMes[2] = 29;
    }
    
    if (dia < 1 || dia > diasPorMes[mes]) return false;
    // PDF especifica entrada às 14h e saída às 12h
    if (hora != 12 && hora != 14) return false;
    
    return true;
}

// Exibir data
void Data::exibir() const {
    cout << "Data: " << setfill('0') << setw(2) << dia << "/" 
         << setw(2) << mes << "/" << ano 
         << " as " << setw(2) << hora << ":00h";
}

// Comparar datas
int Data::compararCom(const Data& outra) const {
    if (ano != outra.ano) return ano - outra.ano;
    if (mes != outra.mes) return mes - outra.mes;
    if (dia != outra.dia) return dia - outra.dia;
    return hora - outra.hora; // Compara a hora por último
}

// Sobrecarga de operadores
bool Data::operator<(const Data& outra) const {
    return compararCom(outra) < 0;
}

bool Data::operator>(const Data& outra) const {
    return compararCom(outra) > 0;
}

bool Data::operator==(const Data& outra) const {
    return compararCom(outra) == 0;
}

bool Data::operator<=(const Data& outra) const {
    return compararCom(outra) <= 0;
}

bool Data::operator>=(const Data& outra) const {
    return compararCom(outra) >= 0;
}

// Funções utilitárias
void limparBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausar() {
    cout << "\nPressione ENTER para continuar...";
    limparBuffer(); 
    cin.get();
}

int calcularDiarias(const Data& entrada, const Data& saida) {
    
    long diasEntrada = entrada.ano * 365 + entrada.dia;
    int diasPorMes[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((entrada.ano % 4 == 0 && entrada.ano % 100 != 0) || (entrada.ano % 400 == 0)) {
        diasPorMes[2] = 29;
    }
    for (int m = 1; m < entrada.mes; ++m) diasEntrada += diasPorMes[m];

    long diasSaida = saida.ano * 365 + saida.dia;
    if ((saida.ano % 4 == 0 && saida.ano % 100 != 0) || (saida.ano % 400 == 0)) {
        diasPorMes[2] = 29;
    }
    for (int m = 1; m < saida.mes; ++m) diasSaida += diasPorMes[m];
    
    int diarias = diasSaida - diasEntrada;

    
    return diarias > 0 ? diarias : 1; // Garante pelo menos 1 diária
}

Data lerData(const string& mensagem) {
    Data data;
    cout << mensagem << endl;
    
    do {
        cout << "Dia (1-31): ";
        cin >> data.dia;
        
        cout << "Mes (1-12): ";
        cin >> data.mes;
        
        cout << "Ano: ";
        cin >> data.ano;
        
        cout << "Hora (12 ou 14): ";
        cin >> data.hora;
        
        if (!data.validar()) {
            cout << "\nData invalida! Tente novamente.\n" << endl;
            limparBuffer();
        }
    } while (!data.validar());
    
    limparBuffer();
    return data;
}

/**
 * @brief Verifica se dois períodos de tempo se sobrepõem.
 * Esta é a função que estava causando o 'undefined reference'.
 * Período 1: [s1, e1] (Start1, End1)
 * Período 2: [s2, e2] (Start2, End2)
 */
bool periodosSeSobrepoe(const Data& s1, const Data& e1, const Data& s2, const Data& e2) {
    
    bool naoSeSobrepoe = (e1 < s2) || (e2 < s1);

    return !naoSeSobrepoe;
}