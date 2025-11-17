#ifndef QUARTO_H
#define QUARTO_H

#include <string>
#include <vector>
#include <fstream>
#include "Estadia.h"

using namespace std;

extern const string ARQUIVO_QUARTOS;

enum StatusQuarto {
    DESOCUPADO,
    OCUPADO
};

class Quarto {
public:
    int numero;
    int maxHospedes;
    double valorDiaria;
    StatusQuarto status;
    bool ativo;

    Quarto();
    Quarto(int num, int maxHosp, double valor);

    void exibir() const;
    string obterStatusString() const;
    void salvarEmArquivo(ofstream& arquivo) const;
    void carregarDeArquivo(ifstream& arquivo);
};

bool cadastrarQuarto(const Quarto& quarto);

Quarto buscarQuartoPorNumero(int numero);

void listarQuartos();
void listarQuartosDisponiveis();
bool quartoExiste(int numero);

bool alterarStatusQuarto(int numero, StatusQuarto novoStatus);
Quarto buscarQuartoDisponivel(int qtdHospedes, const Data& entrada, const Data& saida);


#endif 