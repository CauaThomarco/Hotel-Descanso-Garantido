#include "../Headers/Clientes.h"
#include "../Headers/Estadia.h" // <-- INCLUDE NECESSÁRIO (Faltando)
#include "../Headers/Utils.h"

// --- INCLUDES NECESSÁRIOS (Faltando) ---
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring> // Para strcmp

using namespace std;

// --- ARQUIVO DE DADOS ---
const string ARQUIVO_CLIENTES = "clientes.bin";

// --- IMPLEMENTAÇÃO DOS CONSTRUTORES ---
Cliente::Cliente() : codigo(0), ativo(true) {
    // Inicializa os arrays de char
    memset(nome, 0, sizeof(nome));
    memset(endereco, 0, sizeof(endereco));
    memset(telefone, 0, sizeof(telefone));
}

Cliente::Cliente(int cod, const string& n, const string& end, const string& tel)
    : codigo(cod), ativo(true) {
    // Copia os dados das strings para os arrays de char
    strncpy(nome, n.c_str(), sizeof(nome) - 1);
    strncpy(endereco, end.c_str(), sizeof(endereco) - 1);
    strncpy(telefone, tel.c_str(), sizeof(telefone) - 1);
    
    // Garante terminação nula
    nome[sizeof(nome) - 1] = '\0';
    endereco[sizeof(endereco) - 1] = '\0';
    telefone[sizeof(telefone) - 1] = '\0';
}

// --- IMPLEMENTAÇÃO DOS MÉTODOS ---
void Cliente::exibir() const {
    if (!ativo) return; // Não exibe se estiver inativo
    cout << "----------------------------------" << endl;
    cout << "Codigo: " << codigo << endl;
    cout << "Nome: " << nome << endl;
    cout << "Endereco: " << endereco << endl;
    cout << "Telefone: " << telefone << endl;
    cout << "----------------------------------" << endl;
}

void Cliente::salvarEmArquivo(ofstream& arquivo) const {
    // Escreve a struct inteira no arquivo binário
    arquivo.write(reinterpret_cast<const char*>(this), sizeof(Cliente));
}

void Cliente::carregarDeArquivo(ifstream& arquivo) {
    // Lê a struct inteira do arquivo binário
    arquivo.read(reinterpret_cast<char*>(this), sizeof(Cliente));
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DO MÓDULO ---

int gerarCodigoCliente() {
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary | ios::ate); // Abre no final
    if (!arquivo.is_open()) {
        return 1; // Se o arquivo não existe, o primeiro código é 1
    }

    if (arquivo.tellg() == 0) {
        arquivo.close();
        return 1; // Arquivo vazio, o primeiro código é 1
    }

    // Pula para o início do ÚLTIMO registro
    arquivo.seekg(-static_cast<long>(sizeof(Cliente)), ios::end); 
    
    Cliente ultimoCliente;
    ultimoCliente.carregarDeArquivo(arquivo);
    arquivo.close();
    
    return ultimoCliente.codigo + 1; // Próximo código
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

// --- CORREÇÃO DE LÓGICA E ASSINATURA ---
// Retorna um OBJETO Cliente, não um ponteiro.
// Se não encontrar, retorna um Cliente com código -1.
Cliente buscarClientePorCodigo(int codigo) {
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    Cliente cliente;

    if (!arquivo.is_open()) {
        cliente.codigo = -1; // Sinaliza "não encontrado"
        return cliente;
    }

    while (arquivo.peek() != EOF) {
        cliente.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (cliente.codigo == codigo && cliente.ativo) {
            arquivo.close();
            return cliente; // Encontrou
        }
    }

    arquivo.close();
    cliente.codigo = -1; // Sinaliza "não encontrado"
    return cliente;
}

vector<Cliente> buscarClientesPorNome(const string& nome) {
    vector<Cliente> clientesEncontrados;
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    Cliente cliente;

    if (!arquivo.is_open()) {
        return clientesEncontrados; // Retorna lista vazia
    }

    // Converte o nome de busca (string) para minúsculo e C-string
    string nomeBuscaLower = nome;
    for (char &c : nomeBuscaLower) c = tolower(c);
    const char* busca = nomeBuscaLower.c_str();


    while (arquivo.peek() != EOF) {
        cliente.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (cliente.ativo) {
            // Converte o nome do cliente (char[]) para minúsculo
            string nomeClienteLower;
            for(int i = 0; cliente.nome[i] != '\0'; ++i) {
                nomeClienteLower += tolower(cliente.nome[i]);
            }

            // Compara usando strstr (procura substring)
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

// --- CORREÇÃO DE LÓGICA ---
// Agora usa a nova 'buscarClientePorCodigo' que retorna um objeto
bool clienteExiste(int codigo) {
    Cliente cliente = buscarClientePorCodigo(codigo);
    // Se o código for diferente de -1, o cliente existe
    return (cliente.codigo != -1);
}
