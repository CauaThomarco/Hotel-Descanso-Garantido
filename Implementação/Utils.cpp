#include "Headers/Utils.h"

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
    if (hora != 12 && hora != 14) return false;
    
    return true;
}

// Exibir data
void Data::exibir() const {
    cout << setfill('0') << setw(2) << dia << "/" 
         << setw(2) << mes << "/" << ano 
         << " às " << setw(2) << hora << ":00h";
}

// Comparar datas
int Data::compararCom(const Data& outra) const {
    if (ano != outra.ano) return ano - outra.ano;
    if (mes != outra.mes) return mes - outra.mes;
    if (dia != outra.dia) return dia - outra.dia;
    return hora - outra.hora;
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
    cin.ignore(10000, '\n');
}

void pausar() {
    cout << "\nPressione ENTER para continuar...";
    cin.ignore();
    cin.get();
}

int calcularDiarias(const Data& entrada, const Data& saida) {
    // Converte datas para dias desde uma época
    int diasEntrada = entrada.ano * 365 + entrada.mes * 30 + entrada.dia;
    int diasSaida = saida.ano * 365 + saida.mes * 30 + saida.dia;
    
    int diarias = diasSaida - diasEntrada;
    
    // Se check-out for depois das 12h, conta mais uma diária
    if (saida.hora > 12) {
        diarias++;
    }
    
    return diarias > 0 ? diarias : 1;
}

Data lerData(const string& mensagem) {
    Data data;
    cout << mensagem << endl;
    
    do {
        cout << "Dia (1-31): ";
        cin >> data.dia;
        
        cout << "Mês (1-12): ";
        cin >> data.mes;
        
        cout << "Ano: ";
        cin >> data.ano;
        
        cout << "Hora (12 ou 14): ";
        cin >> data.hora;
        
        if (!data.validar()) {
            cout << "\nData inválida! Tente novamente.\n" << endl;
        }
    } while (!data.validar());
    
    return data;
}

bool periodosSesobrepoe(const Data& e1, const Data& s1, const Data& e2, const Data& s2) {
    // Verifica se os períodos se sobrepõem
    // Período 1: [e1, s1]
    // Período 2: [e2, s2]
    
    // Não se sobrepõe se:
    // - Período 1 termina antes do período 2 começar
    // - Período 2 termina antes do período 1 começar
    
    if (s1 < e2 || s2 < e1) {
        return false;
    }
    
    return true;
}