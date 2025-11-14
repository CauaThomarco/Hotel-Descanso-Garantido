#ifndef CLIENTES_H
#define CLIENTES_H

// --- INCLUDES NECESSÁRIOS (Faltando) ---
#include <string>
#include <vector>
#include <fstream> // Para salvar/carregar

// Usar std:: para não ter que digitar em todo lugar
using namespace std;

extern const string ARQUIVO_CLIENTES;
// --- DEFINIÇÃO DA ESTRUTURA (Corrigida) ---
// Mudamos de 'struct' para 'class' para organizar
// e adicionamos todas as funções que estão no .cpp
class Cliente {
public: // Todos os membros são públicos para facilitar
    // Membros de dados
    int codigo;
    char nome[100];
    char endereco[200];
    char telefone[20];
    bool ativo; // <-- Membro Faltando

    // Construtores (Declarações Faltando)
    Cliente();
    Cliente(int cod, const string& n, const string& end, const string& tel);

    // Métodos (Declarações Faltando)
    void exibir() const;
    void salvarEmArquivo(ofstream& arquivo) const;
    void carregarDeArquivo(ifstream& arquivo);
};

// --- DECLARAÇÕES DE FUNÇÕES DO MÓDULO ---

int gerarCodigoCliente();
bool cadastrarCliente(const Cliente& cliente);

// --- CORREÇÃO DE ASSINATURA ---
// O main.cpp e o .h esperam um OBJETO Cliente, não um ponteiro
Cliente buscarClientePorCodigo(int codigo);

vector<Cliente> buscarClientesPorNome(const string& nome);
void listarClientes();
bool clienteExiste(int codigo);

#endif // CLIENTES_H