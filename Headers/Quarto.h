#ifndef QUARTO_H
#define QUARTO_H

// --- INCLUDES NECESSÁRIOS (Faltando) ---
#include <string>
#include <vector>
#include <fstream> // Para salvar/carregar
#include "Estadia.h" // Para a struct Data

using namespace std;

extern const string ARQUIVO_QUARTOS;

// Enum para o status (como definido no seu .cpp)
enum StatusQuarto {
    DESOCUPADO,
    OCUPADO
};

// --- DEFINIÇÃO DA CLASSE (Corrigida) ---
class Quarto {
public:
    // Membros de dados
    int numero;
    int maxHospedes; // Nome corrigido (estava 'quantidadeHospedes' no .cpp)
    double valorDiaria; // PDF usa 'valor da diária'
    StatusQuarto status;
    bool ativo; // <-- Membro Faltando

    // Construtores (Declarações Faltando)
    Quarto();
    Quarto(int num, int maxHosp, double valor); // Usando double

    // Métodos (Declarações Faltando)
    void exibir() const;
    string obterStatusString() const; // <-- Declaração Faltando
    void salvarEmArquivo(ofstream& arquivo) const;
    void carregarDeArquivo(ifstream& arquivo);
};

// --- DECLARAÇÕES DE FUNÇÕES DO MÓDULO ---

bool cadastrarQuarto(const Quarto& quarto);

// --- CORREÇÃO DE ASSINATURA ---
// O main.cpp e o .h devem concordar em retornar um OBJETO
Quarto buscarQuartoPorNumero(int numero);

void listarQuartos();
void listarQuartosDisponiveis();
bool quartoExiste(int numero);

// --- CORREÇÃO DE ASSINATURA (float para double) ---
bool alterarStatusQuarto(int numero, StatusQuarto novoStatus);
Quarto buscarQuartoDisponivel(int qtdHospedes, const Data& entrada, const Data& saida);


#endif // QUARTO_H