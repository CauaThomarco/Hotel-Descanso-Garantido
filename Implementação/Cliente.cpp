#include "Headers/Clientes.h"

const string ARQUIVO_CLIENTES = "clientes.dat";

// Construtores
Cliente::Cliente() : codigo(0), nome(""), endereco(""), telefone(""), ativo(true) {}

Cliente::Cliente(int cod, const string& n, const string& end, const string& tel) 
    : codigo(cod), nome(n), endereco(end), telefone(tel), ativo(true) {}

// Exibir cliente
void Cliente::exibir() const {
    cout << "\n========== CLIENTE ==========\n";
    cout << "Código: " << codigo << endl;
    cout << "Nome: " << nome << endl;
    cout << "Endereço: " << endereco << endl;
    cout << "Telefone: " << telefone << endl;
    cout << "Status: " << (ativo ? "Ativo" : "Inativo") << endl;
    cout << "============================\n";
}

// Serialização
void Cliente::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write((char*)&codigo, sizeof(int));
    
    size_t tamanho = nome.size();
    arquivo.write((char*)&tamanho, sizeof(size_t));
    arquivo.write(nome.c_str(), tamanho);
    
    tamanho = endereco.size();
    arquivo.write((char*)&tamanho, sizeof(size_t));
    arquivo.write(endereco.c_str(), tamanho);
    
    tamanho = telefone.size();
    arquivo.write((char*)&tamanho, sizeof(size_t));
    arquivo.write(telefone.c_str(), tamanho);
    
    arquivo.write((char*)&ativo, sizeof(bool));
}

void Cliente::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read((char*)&codigo, sizeof(int));
    
    size_t tamanho;
    arquivo.read((char*)&tamanho, sizeof(size_t));
    nome.resize(tamanho);
    arquivo.read(&nome[0], tamanho);
    
    arquivo.read((char*)&tamanho, sizeof(size_t));
    endereco.resize(tamanho);
    arquivo.read(&endereco[0], tamanho);
    
    arquivo.read((char*)&tamanho, sizeof(size_t));
    telefone.resize(tamanho);
    arquivo.read(&telefone[0], tamanho);
    
    arquivo.read((char*)&ativo, sizeof(bool));
}

// Gerar código único
int gerarCodigoCliente() {
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    int maiorCodigo = 0;
    
    if (arquivo.is_open()) {
        Cliente cliente;
        while (!arquivo.eof()) {
            try {
                cliente.carregarDeArquivo(arquivo);
                if (cliente.codigo > maiorCodigo) {
                    maiorCodigo = cliente.codigo;
                }
            } catch (...) {
                break;
            }
        }
        arquivo.close();
    }
    
    return maiorCodigo + 1;
}

// Cadastrar cliente
bool cadastrarCliente(const Cliente& cliente) {
    // Verifica se já existe
    if (clienteExiste(cliente.codigo)) {
        cout << "Erro: Já existe um cliente com o código " << cliente.codigo << endl;
        return false;
    }
    
    ofstream arquivo(ARQUIVO_CLIENTES, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de clientes!" << endl;
        return false;
    }
    
    cliente.salvarEmArquivo(arquivo);
    arquivo.close();
    
    cout << "Cliente cadastrado com sucesso! Código: " << cliente.codigo << endl;
    return true;
}

// Buscar cliente por código
Cliente* buscarClientePorCodigo(int codigo) {
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    if (!arquivo.is_open()) {
        return nullptr;
    }
    
    Cliente* cliente = new Cliente();
    while (!arquivo.eof()) {
        try {
            cliente->carregarDeArquivo(arquivo);
            if (cliente->codigo == codigo && cliente->ativo) {
                arquivo.close();
                return cliente;
            }
        } catch (...) {
            break;
        }
    }
    
    arquivo.close();
    delete cliente;
    return nullptr;
}

// Buscar clientes por nome
vector<Cliente> buscarClientesPorNome(const string& nome) {
    vector<Cliente> clientes;
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    
    if (!arquivo.is_open()) {
        return clientes;
    }
    
    Cliente cliente;
    while (!arquivo.eof()) {
        try {
            cliente.carregarDeArquivo(arquivo);
            if (cliente.ativo && cliente.nome.find(nome) != string::npos) {
                clientes.push_back(cliente);
            }
        } catch (...) {
            break;
        }
    }
    
    arquivo.close();
    return clientes;
}

// Listar todos os clientes
void listarClientes() {
    ifstream arquivo(ARQUIVO_CLIENTES, ios::binary);
    
    if (!arquivo.is_open()) {
        cout << "Nenhum cliente cadastrado ainda." << endl;
        return;
    }
    
    Cliente cliente;
    bool encontrou = false;
    
    cout << "\n========== LISTA DE CLIENTES ==========\n";
    while (!arquivo.eof()) {
        try {
            cliente.carregarDeArquivo(arquivo);
            if (cliente.ativo) {
                cliente.exibir();
                encontrou = true;
            }
        } catch (...) {
            break;
        }
    }
    
    if (!encontrou) {
        cout << "Nenhum cliente cadastrado." << endl;
    }
    
    arquivo.close();
}

// Verificar se cliente existe
bool clienteExiste(int codigo) {
    Cliente* cliente = buscarClientePorCodigo(codigo);
    if (cliente != nullptr) {
        delete cliente;
        return true;
    }
    return false;
}

// Calcular pontos de fidelidade (será implementado após Estadia)
int calcularPontosFidelidade(int codigoCliente) {
    // Esta função precisa acessar as estadias do cliente
    // Será implementada depois que Estadia estiver pronta
    return 0;
}