#include "../Headers/Funcionarios.h"

const string ARQUIVO_FUNCIONARIOS = "funcionarios.dat";

// Construtores
Funcionario::Funcionario() : codigo(0), nome(""), telefone(""), cargo(""), salario(0.0), ativo(true) {}

Funcionario::Funcionario(int cod, const string& n, const string& tel, const string& c, float sal)
    : codigo(cod), nome(n), telefone(tel), cargo(c), salario(sal), ativo(true) {}

// Exibir funcionário
void Funcionario::exibir() const {
    cout << "\n========== FUNCIONÁRIO ==========\n";
    cout << "Código: " << codigo << endl;
    cout << "Nome: " << nome << endl;
    cout << "Telefone: " << telefone << endl;
    cout << "Cargo: " << cargo << endl;
    cout << fixed << setprecision(2);
    cout << "Salário: R$ " << salario << endl;
    cout << "Status: " << (ativo ? "Ativo" : "Inativo") << endl;
    cout << "=================================\n";
}

// Serialização
void Funcionario::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write((char*)&codigo, sizeof(int));
    
    size_t tamanho = nome.size();
    arquivo.write((char*)&tamanho, sizeof(size_t));
    arquivo.write(nome.c_str(), tamanho);
    
    tamanho = telefone.size();
    arquivo.write((char*)&tamanho, sizeof(size_t));
    arquivo.write(telefone.c_str(), tamanho);
    
    tamanho = cargo.size();
    arquivo.write((char*)&tamanho, sizeof(size_t));
    arquivo.write(cargo.c_str(), tamanho);
    
    arquivo.write((char*)&salario, sizeof(float));
    arquivo.write((char*)&ativo, sizeof(bool));
}

void Funcionario::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read((char*)&codigo, sizeof(int));
    
    size_t tamanho;
    arquivo.read((char*)&tamanho, sizeof(size_t));
    nome.resize(tamanho);
    arquivo.read(&nome[0], tamanho);
    
    arquivo.read((char*)&tamanho, sizeof(size_t));
    telefone.resize(tamanho);
    arquivo.read(&telefone[0], tamanho);
    
    arquivo.read((char*)&tamanho, sizeof(size_t));
    cargo.resize(tamanho);
    arquivo.read(&cargo[0], tamanho);
    
    arquivo.read((char*)&salario, sizeof(float));
    arquivo.read((char*)&ativo, sizeof(bool));
}

// Gerar código único
int gerarCodigoFuncionario() {
    ifstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary);
    int maiorCodigo = 0;
    
    if (arquivo.is_open()) {
        Funcionario funcionario;
        while (!arquivo.eof()) {
            try {
                funcionario.carregarDeArquivo(arquivo);
                if (funcionario.codigo > maiorCodigo) {
                    maiorCodigo = funcionario.codigo;
                }
            } catch (...) {
                break;
            }
        }
        arquivo.close();
    }
    
    return maiorCodigo + 1;
}

// Cadastrar funcionário
bool cadastrarFuncionario(const Funcionario& funcionario) {
    // Verifica se já existe
    if (funcionarioExiste(funcionario.codigo)) {
        cout << "Erro: Já existe um funcionário com o código " << funcionario.codigo << endl;
        return false;
    }
    
    ofstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de funcionários!" << endl;
        return false;
    }
    
    funcionario.salvarEmArquivo(arquivo);
    arquivo.close();
    
    cout << "Funcionário cadastrado com sucesso! Código: " << funcionario.codigo << endl;
    return true;
}

// Buscar funcionário por código
Funcionario* buscarFuncionarioPorCodigo(int codigo) {
    ifstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary);
    if (!arquivo.is_open()) {
        return nullptr;
    }
    
    Funcionario* funcionario = new Funcionario();
    while (!arquivo.eof()) {
        try {
            funcionario->carregarDeArquivo(arquivo);
            if (funcionario->codigo == codigo && funcionario->ativo) {
                arquivo.close();
                return funcionario;
            }
        } catch (...) {
            break;
        }
    }
    
    arquivo.close();
    delete funcionario;
    return nullptr;
}

// Buscar funcionários por nome
vector<Funcionario> buscarFuncionariosPorNome(const string& nome) {
    vector<Funcionario> funcionarios;
    ifstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary);
    
    if (!arquivo.is_open()) {
        return funcionarios;
    }
    
    Funcionario funcionario;
    while (!arquivo.eof()) {
        try {
            funcionario.carregarDeArquivo(arquivo);
            if (funcionario.ativo && funcionario.nome.find(nome) != string::npos) {
                funcionarios.push_back(funcionario);
            }
        } catch (...) {
            break;
        }
    }
    
    arquivo.close();
    return funcionarios;
}

// Listar todos os funcionários
void listarFuncionarios() {
    ifstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary);
    
    if (!arquivo.is_open()) {
        cout << "Nenhum funcionário cadastrado ainda." << endl;
        return;
    }
    
    Funcionario funcionario;
    bool encontrou = false;
    
    cout << "\n========== LISTA DE FUNCIONÁRIOS ==========\n";
    while (!arquivo.eof()) {
        try {
            funcionario.carregarDeArquivo(arquivo);
            if (funcionario.ativo) {
                funcionario.exibir();
                encontrou = true;
            }
        } catch (...) {
            break;
        }
    }
    
    if (!encontrou) {
        cout << "Nenhum funcionário cadastrado." << endl;
    }
    
    arquivo.close();
}

// Verificar se funcionário existe
bool funcionarioExiste(int codigo) {
    Funcionario* funcionario = buscarFuncionarioPorCodigo(codigo);
    if (funcionario != nullptr) {
        delete funcionario;
        return true;
    }
    return false;
}