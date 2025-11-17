#include "../Headers/Clientes.h"
#include "../Headers/Estadia.h"
#include "../Headers/Utils.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>

using namespace std;

// --- ARQUIVO DE DADOS ---
const string ARQUIVO_CLIENTES = "clientes.bin";

// --- IMPLEMENTAÇÃO DOS CONSTRUTORES ---
Cliente::Cliente() : codigo(0), ativo(true) {
    memset(nome, 0, sizeof(nome));
    memset(endereco, 0, sizeof(endereco));
    memset(telefone, 0, sizeof(telefone));
}

Cliente::Cliente(int cod, const string& n, const string& end, const string& tel)
    : codigo(cod), ativo(true) {
    strncpy(nome, n.c_str(), sizeof(nome) - 1);
    strncpy(endereco, end.c_str(), sizeof(endereco) - 1);
    strncpy(telefone, tel.c_str(), sizeof(telefone) - 1);
    
    nome[sizeof(nome) - 1] = '\0';
    endereco[sizeof(endereco) - 1] = '\0';
    telefone[sizeof(telefone) - 1] = '\0';
}

// --- IMPLEMENTAÇÃO DOS MÉTODOS ---
void Cliente::exibir() const {
    if (!ativo) return;
    cout << "----------------------------------" << endl;
    cout << "Codigo: " << codigo << endl;
    cout << "Nome: " << nome << endl;
    cout << "Endereco: " << endereco << endl;
    cout << "Telefone: " << telefone << endl;
    cout << "----------------------------------" << endl;
}

void Cliente::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write(reinterpret_cast<const char*>(this), sizeof(Cliente));
}

void Cliente::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read(reinterpret_cast<char*>(this), sizeof(Cliente));
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DO MÓDULO ---

int gerarCodigoCliente() {
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary | ios::ate);
    if (!arquivo.is_open()) {
        return 1;
    }

    if (arquivo.tellg() == 0) {
        arquivo.close();
        return 1;
    }

    arquivo.seekg(-static_cast<long>(sizeof(Cliente)), ios::end); 
    
    Cliente ultimoCliente;
    ultimoCliente.carregarDeArquivo(arquivo);
    arquivo.close();
    
    return ultimoCliente.codigo + 1;
}

bool cadastrarCliente(const Cliente& cliente) {
    if (clienteExiste(cliente.codigo)) {
        cout << "Erro: Ja existe um cliente com o codigo " << cliente.codigo << endl;
        return false;
    }

    ofstream arquivo(ARQUIVO_CLIENTES, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de clientes!" << endl;
        return false;
    }

    cliente.salvarEmArquivo(arquivo);
    arquivo.close();

    cout << "Cliente cadastrado com sucesso! Codigo: " << cliente.codigo << endl;
    return true;
}

Cliente buscarClientePorCodigo(int codigo) {
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    Cliente cliente;

    if (!arquivo.is_open()) {
        cliente.codigo = -1;
        return cliente;
    }

    while (arquivo.peek() != EOF) {
        cliente.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (cliente.codigo == codigo && cliente.ativo) {
            arquivo.close();
            return cliente;
        }
    }

    arquivo.close();
    cliente.codigo = -1;
    return cliente;
}

vector<Cliente> buscarClientesPorNome(const string& nome) {
    vector<Cliente> clientesEncontrados;
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    Cliente cliente;

    if (!arquivo.is_open()) {
        return clientesEncontrados;
    }

    string nomeBuscaLower = nome;
    for (char &c : nomeBuscaLower) c = tolower(c);
    const char* busca = nomeBuscaLower.c_str();


    while (arquivo.peek() != EOF) {
        cliente.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (cliente.ativo) {
            string nomeClienteLower;
            for(int i = 0; cliente.nome[i] != '\0'; ++i) {
                nomeClienteLower += tolower(cliente.nome[i]);
            }

            if (strstr(nomeClienteLower.c_str(), busca) != nullptr) {
                clientesEncontrados.push_back(cliente);
            }
        }
    }

    arquivo.close();
    return clientesEncontrados;
}

void listarClientes() {
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    if (!arquivo.is_open()) {
        cout << "Nenhum cliente cadastrado ainda." << endl;
        return;
    }

    Cliente cliente;
    bool encontrou = false;
    cout << "\n========== LISTA DE CLIENTES ==========\n";

    while (arquivo.peek() != EOF) {
        cliente.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (cliente.ativo) {
            cliente.exibir();
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum cliente cadastrado." << endl;
    }
    arquivo.close();
}

bool clienteExiste(int codigo) {
    Cliente cliente = buscarClientePorCodigo(codigo);
    return (cliente.codigo != -1);
}
