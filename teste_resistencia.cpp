// Includes de Bibliotecas Padrão
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip> 
#include <stdexcept>
#include <windows.h>
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
    cout << "   Preparando ambiente de teste...\n";
    
    // Pré-condição: Cliente 1 e 2, Quarto 201
    Cliente c2(2, "Cliente Teste 2", "Rua B", "98888-2222");
    cadastrarCliente(c2);
    cout << "   - Cliente 2 cadastrado\n";
    
    Quarto q201(201, 2, 200.0);
    cadastrarQuarto(q201);
    cout << "   - Quarto 201 cadastrado (2 hóspedes, R$ 200/dia)\n";
    
    Data ent(15, 12, 2025, 14);
    Data sai(20, 12, 2025, 12);
    cout << "   - Período testado: 15/12 a 20/12/2025\n\n";
    
    // Cadastro 1 (Deve funcionar)
    cout << "   Tentativa 1: Cliente 1, Quarto 201, 15-20/12...\n";
    bool cadastro1 = cadastrarEstadia(1, 2, ent, sai);
    checar(cadastro1, "Sistema cadastrou Estadia 1 (Cliente 1 no Quarto 201).", "Falha ao cadastrar Estadia 1.");
    
    // Cadastro 2 (Deve falhar - mesmo período)
    cout << "\n   Tentativa 2: Cliente 2, Quarto 201, MESMO período (15-20/12)...\n";
    bool cadastro2 = cadastrarEstadia(2, 2, ent, sai);
    checar(!cadastro2, "Sistema rejeitou corretamente Estadia 2 (sobreposição total).", "Sistema ACEITOU uma estadia sobreposta!");
    
    // Teste adicional: sobreposição parcial
    cout << "\n   Tentativa 3: Cliente 2, Quarto 201, período parcial (18-22/12)...\n";
    Data ent2(18, 12, 2025, 14);
    Data sai2(22, 12, 2025, 12);
    bool cadastro3 = cadastrarEstadia(2, 2, ent2, sai2);
    checar(!cadastro3, "Sistema rejeitou corretamente Estadia 3 (sobreposição parcial).", "Sistema ACEITOU uma estadia com sobreposição parcial!");
}

/**
 * @brief Teste 5: Verifica se o sistema aceita estadias em períodos não sobrepostos
 */
void teste_periodo_valido() {
    cout << "\n5. Teste de Período Válido (Não Sobreposto):\n";
    
    // Cliente 2 deve conseguir fazer uma reserva DEPOIS da estadia do Cliente 1
    Data ent(21, 12, 2025, 14); // Um dia após o checkout do Cliente 1
    Data sai(25, 12, 2025, 12);
    
    cout << "   Tentando cadastrar Cliente 2 no Quarto 201 de 21-25/12...\n";
    bool cadastro = cadastrarEstadia(2, 2, ent, sai);
    checar(cadastro, "Sistema aceitou corretamente estadia em período livre.", "Sistema rejeitou incorretamente uma estadia válida!");
}

// --- MAIN ---

int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);       
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

    int testesFalhos = 0;
    int testesPassados = 0;

    // Lista de testes
    vector<pair<string, void(*)()>> testes = {
        {"Duplicidade de Quarto", teste_duplicidade_quarto},
        {"Integridade de Cliente", teste_integridade_cliente},
        {"Datas Inválidas", teste_datas_invalidas},
        {"Sobreposição de Estadia", teste_sobreposicao_estadia},
        {"Período Válido", teste_periodo_valido}
    };

    for (const auto& teste : testes) {
        try {
            teste.second();
            testesPassados++;
        } catch (const exception& e) {
            testesFalhos++;
            cout << "   ⚠️  Teste '" << teste.first << "' falhou.\n";
        }
    }

    // Relatório final
    cout << "\n================================================\n";
    cout << "               RELATÓRIO FINAL                  \n";
    cout << "================================================\n";
    cout << "Testes executados: " << (testesPassados + testesFalhos) << endl;
    cout << "✅ Passaram: " << testesPassados << endl;
    cout << "❌ Falharam: " << testesFalhos << endl;
    
    if (testesFalhos == 0) {
        cout << "\n🎉 SUCESSO TOTAL! Todos os testes passaram.\n";
        cout << "   O sistema está resistente a dados inválidos.\n";
    } else {
        cout << "\n⚠️  ATENÇÃO: Alguns testes falharam.\n";
        cout << "   Revise o código para corrigir os problemas.\n";
    }
    cout << "================================================\n";

    // Limpa os arquivos de teste
    remove(ARQUIVO_CLIENTES.c_str());
    remove(ARQUIVO_FUNCIONARIOS.c_str());
    remove(ARQUIVO_QUARTOS.c_str());
    remove(ARQUIVO_ESTADIAS.c_str());

    return testesFalhos; // Retorna 0 se tudo passou, caso contrário o número de falhas
}