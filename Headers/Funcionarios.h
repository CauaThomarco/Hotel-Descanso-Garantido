#ifndef FUNCIONARIOS_H
#define FUNCIONARIOS_H

#include <string>
#include <vector>
#include <fstream> 

using namespace std;

extern const string ARQUIVO_FUNCIONARIOS;

class Funcionario {
public:
    int codigo;
    char nome[100];
    char telefone[20];
    char cargo[50];
    double salario;
    bool ativo;

    Funcionario();
    Funcionario(int cod, const string& n, const string& tel, const string& c, double sal);

    void exibir() const;
    void salvarEmArquivo(ofstream& arquivo) const;
    void carregarDeArquivo(ifstream& arquivo);
};


int gerarCodigoFuncionario();
bool cadastrarFuncionario(const Funcionario& funcionario);

Funcionario buscarFuncionarioPorCodigo(int codigo);

vector<Funcionario> buscarFuncionariosPorNome(const string& nome);
void listarFuncionarios();
bool funcionarioExiste(int codigo);

#endif