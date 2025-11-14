// Includes de Bibliotecas Padrão
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip> // Para setprecision
#include <stdexcept> // Para std::runtime_error

// Includes do Projeto
#include "Headers/Clientes.h"
#include "Headers/Funcionarios.h"
#include "Headers/Quarto.h"
#include "Headers/Estadia.h"
#include "Headers/Utils.h" // Para limparBuffer

// Namespace
using namespace std;

// --- Funções de Teste ---

/**
 * @brief Função auxiliar para checar uma condição e reportar falha.
 * Se 'condicao' for falsa, o teste falha.
 */
bool checar(bool condicao, const string& mensagemSucesso, const string& mensagemFalha) {
    if (condicao) {
        cout << "  [PASS] " << mensagemSucesso << endl;
        return true;
    } else {
        cout << "  [FAIL] " << mensagemFalha << endl;
        throw runtime_error(mensagemFalha); // Para o teste imediatamente
    }
}

/**
 * @brief Teste 1: Tenta cadastrar um quarto com um número que já existe.
 * O sistema deve rejeitar o segundo cadastro.
 */
void teste_duplicidade_quarto() {
    cout << "\n1. Teste de Duplicidade (Quarto):\n";
    Quarto q1(101, 2, 150.0);
    bool primeiroCadastro = cadastrarQuarto(q1);
    checar(primeiroCadastro, "Cadastro inicial do Quarto 101.", "Falha no cadastro inicial.");
    
    Quarto q2(101, 3, 200.0); // Mesmo número
    bool segundoCadastro = cadastrarQuarto(q2);
    checar(!segundoCadastro, "Sistema rejeitou corretamente o Quarto 101 duplicado.", "Sistema ACEITOU um quarto duplicado.");
}

/**
 * @brief Teste 2: Tenta cadastrar uma estadia para um cliente que não existe.
 * O sistema deve rejeitar o cadastro.
 */
void teste_integridade_cliente() {
    cout << "\n2. Teste de Integridade (Estadia sem Cliente):\n";
    Data ent(1, 12, 2025, 14);
    Data sai(5, 12, 2025, 12);
    int clienteInexistente = 999;
    
    bool cadastro = cadastrarEstadia(clienteInexistente, 1, ent, sai);
    checar(!cadastro, "Sistema rejeitou corretamente estadia para cliente 999 (inexistente).", "Sistema ACEITOU estadia para cliente inexistente.");
}

/**
 * @brief Teste 3: Tenta cadastrar uma estadia com data de saída anterior à entrada.
 * O sistema deve rejeitar o cadastro.
 */
void teste_datas_invalidas() {
    cout << "\n3. Teste de Datas Inválidas (Saída < Entrada):\n";
    // Pré-condição: Cliente 1 e Quarto 101 devem existir (do teste_duplicidade)
    // Precisamos criar o cliente 1 para este teste
    Cliente c1(1, "Cliente Teste 1", "Rua A", "98888-1111");
    cadastrarCliente(c1);

    Data ent(10, 12, 2025, 14);
    Data sai(5, 12, 2025, 12); // Saída ANTES da entrada
    
    bool cadastro = cadastrarEstadia(1, 1, ent, sai); // Cliente 1 existe
    checar(!cadastro, "Sistema rejeitou corretamente estadia com datas invertidas.", "Sistema ACEITOU estadia com datas invertidas.");
}

/**
 * @brief Teste 4: Tenta cadastrar duas estadias no mesmo quarto e mesmo período.
 * O sistema deve aceitar a primeira e rejeitar a segunda.
 */
void teste_sobreposicao_estadia() {
    cout << "\n4. Teste de Sobreposicao de Estadia (Crash Test):\n";
    // Pré-condição: Cliente 1 e 2, Quarto 201
    Cliente c2(2, "Cliente Teste 2", "Rua B", "98888-2222");
    cadastrarCliente(c2);
    Quarto q201(201, 2, 200.0);
    cadastrarQuarto(q201);
    
    Data ent(15, 12, 2025, 14);
    Data sai(20, 12, 2025, 12);
    
    // Cadastro 1 (Deve funcionar)
    bool cadastro1 = cadastrarEstadia(1, 2, ent, sai);
    checar(cadastro1, "Sistema cadastrou Estadia 1 (Cliente 1 no Quarto 201).", "Falha ao cadastrar Estadia 1.");
    
    // Cadastro 2 (Deve falhar)
    // Mesmo quarto (201), mesmo período, cliente diferente (2)
    bool cadastro2 = cadastrarEstadia(2, 2, ent, sai);
    checar(!cadastro2, "Sistema rejeitou corretamente Estadia 2 (sobreposicao).", "Sistema ACEITOU uma estadia sobreposta.");
}

// --- MAIN ---

int main() {
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║     TESTE DE RESISTENCIA DO SISTEMA        ║\n";
    cout << "║     Hotel Descanso Garantido               ║\n";
    cout << "╚════════════════════════════════════════════╝\n";

    char opcao;
    cout << "\nATENCAO: Isso ira apagar e recriar os arquivos .dat\n";
    cout << "Deseja continuar? (s/n): ";
    cin >> opcao;
    
    if (opcao != 's' && opcao != 'S') {
        cout << "Operacao cancelada.\n";
        return 0;
    }

    // Limpa os arquivos de dados para um teste limpo
    remove(ARQUIVO_CLIENTES.c_str());
    remove(ARQUIVO_FUNCIONARIOS.c_str());
    remove(ARQUIVO_QUARTOS.c_str());
    remove(ARQUIVO_ESTADIAS.c_str());

    cout << "\nIniciando testes...\n";

    try {
        // Executa a sequência de testes
        teste_duplicidade_quarto();
        teste_integridade_cliente();
        teste_datas_invalidas();
        teste_sobreposicao_estadia();

        // Se chegou aqui, tudo passou
        cout << "\n------------------------------------------------\n";
        cout << "✅  SUCESSO! Todos os testes de resistencia passaram.\n";
        cout << "   O sistema lidou com dados invalidos sem travar.\n";
        cout << "------------------------------------------------\n";
        
    } catch (const exception& e) {
        // Se a função checar() lançou um erro, o teste falhou.
        cout << "\n------------------------------------------------\n";
        cout << "❌  FALHA! O teste de resistencia foi interrompido:\n";
        cout << "   " << e.what() << endl;
        cout << "------------------------------------------------\n";
    }

    // Limpa os arquivos de teste
    remove(ARQUIVO_CLIENTES.c_str());
    remove(ARQUIVO_FUNCIONARIOS.c_str());
    remove(ARQUIVO_QUARTOS.c_str());
    remove(ARQUIVO_ESTADIAS.c_str());

    return 0;
}