#ifndef CLIENTES_H
#define CLIENTES_H

#include <string>
#include <vector>
#include <fstream> 

using namespace std;

extern const string ARQUIVO_CLIENTES;

class Cliente {
public:
    int codigo;
    char nome[100];
    char endereco[200];
    char telefone[20];
    bool ativo;

    Cliente();
    Cliente(int cod, const string& n, const string& end, const string& tel);

    void exibir() const;
    void salvarEmArquivo(ofstream& arquivo) const;
    void carregarDeArquivo(ifstream& arquivo);
};


int gerarCodigoCliente();
bool cadastrarCliente(const Cliente& cliente);

Cliente buscarClientePorCodigo(int codigo);

vector<Cliente> buscarClientesPorNome(const string& nome);
void listarClientes();
bool clienteExiste(int codigo);

#endif