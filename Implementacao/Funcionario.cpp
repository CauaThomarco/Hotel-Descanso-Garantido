#include "../Headers/Funcionarios.h"
#include "../Headers/Utils.h" // Para pausar, etc.

// --- INCLUDES NECESSÁRIOS (Faltando) ---
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring> // Para memset, strncpy, strcmp
#include <iomanip> // Para setprecision

using namespace std;

// --- ARQUIVO DE DADOS ---
const string ARQUIVO_FUNCIONARIOS = "funcionarios.dat";

// --- IMPLEMENTAÇÃO DOS CONSTRUTORES ---
Funcionario::Funcionario() : codigo(0), salario(0.0), ativo(true) {
    // Zera os arrays de char
    memset(nome, 0, sizeof(nome));
    memset(telefone, 0, sizeof(telefone));
    memset(cargo, 0, sizeof(cargo));
}

// Construtor usa string, mas armazena em char[]
Funcionario::Funcionario(int cod, const string& n, const string& tel, const string& c, double sal)
    : codigo(cod), salario(sal), ativo(true) {
    
    // Copia strings para arrays de char com segurança
    strncpy(nome, n.c_str(), sizeof(nome) - 1);
    strncpy(telefone, tel.c_str(), sizeof(telefone) - 1);
    strncpy(cargo, c.c_str(), sizeof(cargo) - 1);
    
    // Garante terminação nula
    nome[sizeof(nome) - 1] = '\0';
    telefone[sizeof(telefone) - 1] = '\0';
    cargo[sizeof(cargo) - 1] = '\0';
}

// --- IMPLEMENTAÇÃO DOS MÉTODOS ---
void Funcionario::exibir() const {
    if (!ativo) return; // Não exibe inativos
    cout << "----------------------------------" << endl;
    cout << "Codigo: " << codigo << endl;
    cout << "Nome: " << nome << endl;
    cout << "Telefone: " << telefone << endl;
    cout << "Cargo: " << cargo << endl;
    // Formata o salário para 2 casas decimais
    cout << "Salario: R$ " << fixed << setprecision(2) << salario << endl;
    cout << "----------------------------------" << endl;
}

void Funcionario::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write(reinterpret_cast<const char*>(this), sizeof(Funcionario));
}

void Funcionario::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read(reinterpret_cast<char*>(this), sizeof(Funcionario));
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DO MÓDULO ---

int gerarCodigoFuncionario() {
    ifstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary | ios::ate);
    if (!arquivo.is_open() || arquivo.tellg() == 0) {
        arquivo.close();
        return 1; // Se arquivo não existe ou está vazio
    }
    
    arquivo.seekg(-static_cast<long>(sizeof(Funcionario)), ios::end);
    Funcionario ultimoFunc;
    ultimoFunc.carregarDeArquivo(arquivo);
    arquivo.close();
    return ultimoFunc.codigo + 1;
}

bool cadastrarFuncionario(const Funcionario& funcionario) {
    if (funcionarioExiste(funcionario.codigo)) {
        cout << "Erro: Ja existe um funcionario com o codigo " << funcionario.codigo << endl;
        return false;
    }

    ofstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de funcionarios!" << endl;
        return false;
    }

    funcionario.salvarEmArquivo(arquivo);
    arquivo.close();

    cout << "Funcionario cadastrado com sucesso! Codigo: " << funcionario.codigo << endl;
    return true;
}

// --- CORREÇÃO DE LÓGICA E ASSINATURA ---
// Retorna um OBJETO Funcionario, não um ponteiro.
Funcionario buscarFuncionarioPorCodigo(int codigo) {
    ifstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary);
    Funcionario funcionario;

    if (!arquivo.is_open()) {
        funcionario.codigo = -1; // Sinaliza "não encontrado"
        return funcionario;
    }

    while (arquivo.peek() != EOF) {
        funcionario.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break; // Segurança extra

        if (funcionario.codigo == codigo && funcionario.ativo) {
            arquivo.close();
            return funcionario; // Encontrou
        }
    }

    arquivo.close();
    funcionario.codigo = -1; // Sinaliza "não encontrado"
    return funcionario;
}

vector<Funcionario> buscarFuncionariosPorNome(const string& nome) {
    vector<Funcionario> funcionariosEncontrados;
    ifstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary);
    Funcionario funcionario;

    if (!arquivo.is_open()) {
        return funcionariosEncontrados; // Retorna lista vazia
    }

    // Prepara o nome de busca (minúsculo, C-string)
    string nomeBuscaLower = nome;
    for (char &c : nomeBuscaLower) c = tolower(c);
    const char* busca = nomeBuscaLower.c_str();

    while (arquivo.peek() != EOF) {
        funcionario.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (funcionario.ativo) {
            // Prepara o nome do funcionário (minúsculo)
            string nomeFuncLower;
            for(int i = 0; funcionario.nome[i] != '\0'; ++i) {
                nomeFuncLower += tolower(funcionario.nome[i]);
            }

            // Compara usando strstr (procura se 'busca' é substring de 'nomeFuncLower')
            if (strstr(nomeFuncLower.c_str(), busca) != nullptr) {
                funcionariosEncontrados.push_back(funcionario);
            }
        }
    }

    arquivo.close();
    return funcionariosEncontrados;
}

void listarFuncionarios() {
    ifstream arquivo(ARQUIVO_FUNCIONARIOS, ios::binary);
    if (!arquivo.is_open()) {
        cout << "Nenhum funcionario cadastrado ainda." << endl;
        return;
    }

    Funcionario funcionario;
    bool encontrou = false;
    cout << "\n========== LISTA DE FUNCIONARIOS ==========\n";

    while (arquivo.peek() != EOF) {
        funcionario.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (funcionario.ativo) {
            funcionario.exibir();
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum funcionario cadastrado." << endl;
    }
    arquivo.close();
}

// --- CORREÇÃO DE LÓGICA ---
// Agora usa a nova 'buscarFuncionarioPorCodigo' que retorna um objeto
bool funcionarioExiste(int codigo) {
    Funcionario func = buscarFuncionarioPorCodigo(codigo);
    // Se o código for diferente de -1, o funcionário existe
    return (func.codigo != -1);
}