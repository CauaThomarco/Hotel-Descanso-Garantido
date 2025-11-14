#ifndef FUNCIONARIOS_H
#define FUNCIONARIOS_H

// --- INCLUDES NECESSÁRIOS (Faltando) ---
#include <string>
#include <vector>
#include <fstream> // Para salvar/carregar

using namespace std;

extern const string ARQUIVO_FUNCIONARIOS;

// --- DEFINIÇÃO DA CLASSE (Corrigida) ---
class Funcionario {
public:
    // Membros de dados
    int codigo;
    char nome[100];
    char telefone[20];
    char cargo[50];
    double salario; // PDF usa 'salário' (double é mais seguro que float)
    bool ativo; // <-- Membro Faltando

    // Construtores (Declarações Faltando)
    Funcionario();
    Funcionario(int cod, const string& n, const string& tel, const string& c, double sal);

    // Métodos (Declarações Faltando)
    void exibir() const;
    void salvarEmArquivo(ofstream& arquivo) const;
    void carregarDeArquivo(ifstream& arquivo);
};

// --- DECLARAÇÕES DE FUNÇÕES DO MÓDULO ---

int gerarCodigoFuncionario();
bool cadastrarFuncionario(const Funcionario& funcionario);

// --- CORREÇÃO DE ASSINATURA ---
// O main.cpp e o .h devem concordar em retornar um OBJETO
Funcionario buscarFuncionarioPorCodigo(int codigo);

vector<Funcionario> buscarFuncionariosPorNome(const string& nome);
void listarFuncionarios();
bool funcionarioExiste(int codigo);

#endif // FUNCIONARIOS_H