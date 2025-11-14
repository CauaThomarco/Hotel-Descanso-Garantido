#ifndef ESTADIA_H
#define ESTADIA_H

// --- INCLUDES NECESSÁRIOS (Faltando) ---
#include <string>
#include <vector>
#include <fstream> // Para salvar/carregar

using namespace std;

// --- CONSTANTE GLOBAL (Declaração) ---
extern const string ARQUIVO_ESTADIAS;

// --- DEFINIÇÃO DA STRUCT DATA (Corrigida) ---
// (Esta struct é usada por Utils.cpp)
struct Data {
    // Membros
    int dia;
    int mes;
    int ano;
    int hora;

    // Construtores
    Data();
    Data(int d, int m, int a, int h);

    // Métodos (implementados em Utils.cpp)
    bool validar() const;
    void exibir() const;
    int compararCom(const Data& outra) const;

    // Operadores (implementados em Utils.cpp)
    bool operator<(const Data& outra) const;
    bool operator>(const Data& outra) const;
    bool operator==(const Data& outra) const;
    bool operator<=(const Data& outra) const;
    bool operator>=(const Data& outra) const;
};

// --- ENUM PARA STATUS (Faltando) ---
// O .cpp usa ATIVA e FINALIZADA
enum StatusEstadia {
    ATIVA,
    FINALIZADA
};

// --- DEFINIÇÃO DA CLASSE ESTADIA (Corrigida) ---
class Estadia {
public:
    // Membros de dados (Corrigidos)
    int codigoEstadia; // <-- Nome corrigido (em vez de 'codigo')
    Data dataEntrada;
    Data dataSaida;
    int quantidadeDiarias;
    int codigoCliente;
    int numeroQuarto;
    StatusEstadia status; // <-- Membro Faltando
    bool ativa;           // <-- Membro Faltando

    // Construtores (Declarações Faltando)
    Estadia();
    // Assinatura corrigida para corresponder ao .cpp
    Estadia(int codEstadia, const Data& entrada, const Data& saida, int codCliente, int numQuarto); 

    // Métodos (Declarações Faltando)
    void exibir() const;
    double calcularValorTotal() const; // <-- Declaração Faltando (usei double)
    void salvarEmArquivo(ofstream& arquivo) const;
    void carregarDeArquivo(ifstream& arquivo);
};

// --- DECLARAÇÕES DE FUNÇÕES DO MÓDULO ---

int gerarCodigoEstadia();
bool cadastrarEstadia(int codigoCliente, int qtdHospedes, const Data& entrada, const Data& saida);

// --- CORREÇÃO DE ASSINATURA (main.cpp espera void) ---
void darBaixaEstadia(int codigoEstadia);

bool verificarDisponibilidadeQuarto(int numeroQuarto, const Data& entrada, const Data& saida);
vector<Estadia> listarEstadiaCliente(int codigoCliente);
vector<Estadia> listarEstadiaClientePorNome(const string& nomeCliente);
void listarTodasEstadias();

int calcularTotalDiariasCliente(int codigoCliente);
int calcularPontosFidelidade(int codigoCliente);

#endif // ESTADIA_H