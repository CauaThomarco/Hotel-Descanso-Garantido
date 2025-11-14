// Includes de Bibliotecas Padrão (Faltando)
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
// Includes do Projeto
#include "Headers/Clientes.h"
#include "Headers/Funcionarios.h"
#include "Headers/Quarto.h"
#include "Headers/Estadia.h"
#include "Headers/Utils.h" // Necessário para limparBuffer

// Namespace (Faltando)
using namespace std;

/**
 * Script para gerar dados de teste automaticamente
 * Útil para popular o sistema rapidamente durante desenvolvimento e testes
 */

void gerarClientes() {
    cout << "\n=== Gerando Clientes de Teste ===\n";
    
    string nomes[] = {
        "João Silva", "Maria Santos", "Pedro Oliveira",
        "Ana Costa", "Carlos Ferreira", "Juliana Lima",
        "Roberto Alves", "Fernanda Rocha", "Lucas Mendes", "Beatriz Souza"
    };
    
    string enderecos[] = {
        "Rua das Flores, 123", "Av. Principal, 456", "Rua da Praia, 789",
        "Av. Central, 101", "Rua do Comércio, 202", "Av. das Palmeiras, 303",
        "Rua do Porto, 404", "Av. Beira Mar, 505", "Rua das Acácias, 606",
        "Av. dos Coqueiros, 707"
    };
    
    string telefones[] = {
        "(31) 98765-4321", "(31) 99876-5432", "(31) 97654-3210",
        "(31) 96543-2109", "(31) 95432-1098", "(31) 94321-0987",
        "(31) 93210-9876", "(31) 92109-8765", "(31) 91098-7654",
        "(31) 90987-6543"
    };
    
    for (int i = 0; i < 10; i++) {
        Cliente cliente(i + 1, nomes[i], enderecos[i], telefones[i]);
        if (cadastrarCliente(cliente)) {
            cout << "✓ Cliente " << (i + 1) << " cadastrado\n";
        }
    }
}

void gerarFuncionarios() {
    cout << "\n=== Gerando Funcionários de Teste ===\n";
    
    string nomes[] = {
        "Mariana Oliveira", "José Santos", "Paula Silva",
        "Ricardo Costa", "Amanda Lima"
    };
    
    string telefones[] = {
        "(31) 91234-5678", "(31) 92345-6789", "(31) 93456-7890",
        "(31) 94567-8901", "(31) 95678-9012"
    };
    
    string cargos[] = {
        "recepcionista", "gerente", "auxiliar de limpeza",
        "garçom", "recepcionista"
    };
    
    // CORREÇÃO: Usar double para salarios, como na classe
    double salarios[] = {
        2500.00, 5000.00, 1800.00, 2000.00, 2500.00
    };
    
    for (int i = 0; i < 5; i++) {
        Funcionario func(i + 1, nomes[i], telefones[i], cargos[i], salarios[i]);
        if (cadastrarFuncionario(func)) {
            cout << "✓ Funcionário " << (i + 1) << " cadastrado\n";
        }
    }
}

void gerarQuartos() {
    cout << "\n=== Gerando Quartos de Teste ===\n";
    
    // Quartos simples (1-2 pessoas)
    for (int i = 101; i <= 105; i++) {
        Quarto quarto(i, 2, 150.00);
        if (cadastrarQuarto(quarto)) {
            cout << "✓ Quarto " << i << " cadastrado (2 hóspedes, R$ 150/dia)\n";
        }
    }
    
    // Quartos duplos (3-4 pessoas)
    for (int i = 201; i <= 203; i++) {
        Quarto quarto(i, 4, 250.00);
        if (cadastrarQuarto(quarto)) {
            cout << "✓ Quarto " << i << " cadastrado (4 hóspedes, R$ 250/dia)\n";
        }
    }
    
    // Suítes (5-6 pessoas)
    for (int i = 301; i <= 302; i++) {
        Quarto quarto(i, 6, 400.00);
        if (cadastrarQuarto(quarto)) {
            cout << "✓ Quarto " << i << " cadastrado (6 hóspedes, R$ 400/dia)\n";
        }
    }
}

void gerarEstadias() {
    cout << "\n=== Gerando Estadias de Teste ===\n";
    
    // Estadias passadas (já finalizadas)
    cout << "\nEstadias finalizadas:\n";
    
    // Cliente 1 - Duas estadias antigas
    Data ent1(10, 10, 2025, 14);
    Data sai1(13, 10, 2025, 12);
    if (cadastrarEstadia(1, 2, ent1, sai1)) {
        cout << "✓ Estadia histórica 1 criada\n";
        // Finalizar imediatamente
        darBaixaEstadia(gerarCodigoEstadia() - 1);
    }
    
    Data ent2(20, 10, 2025, 14);
    Data sai2(25, 10, 2025, 12);
    if (cadastrarEstadia(1, 2, ent2, sai2)) {
        cout << "✓ Estadia histórica 2 criada\n";
        darBaixaEstadia(gerarCodigoEstadia() - 1);
    }
    
    // Cliente 2 - Uma estadia antiga
    Data ent3(15, 10, 2025, 14);
    Data sai3(18, 10, 2025, 12);
    if (cadastrarEstadia(2, 4, ent3, sai3)) {
        cout << "✓ Estadia histórica 3 criada\n";
        darBaixaEstadia(gerarCodigoEstadia() - 1);
    }
    
    // Estadias atuais (ativas)
    cout << "\nEstadias ativas:\n";
    
    // Cliente 3 - Hospedado agora
    Data ent4(13, 11, 2025, 14);
    Data sai4(18, 11, 2025, 12);
    if (cadastrarEstadia(3, 2, ent4, sai4)) {
        cout << "✓ Estadia ativa 1 criada\n";
    }
    
    // Cliente 4 - Hospedado agora
    Data ent5(12, 11, 2025, 14);
    Data sai5(20, 11, 2025, 12);
    if (cadastrarEstadia(4, 3, ent5, sai5)) {
        cout << "✓ Estadia ativa 2 criada\n";
    }
    
    // Cliente 5 - Hospedado agora
    Data ent6(14, 11, 2025, 14);
    Data sai6(16, 11, 2025, 12);
    if (cadastrarEstadia(5, 5, ent6, sai6)) {
        cout << "✓ Estadia ativa 3 criada\n";
    }
    
    // Estadias futuras
    cout << "\nReservas futuras:\n";
    
    // Cliente 6 - Reserva futura
    Data ent7(20, 11, 2025, 14);
    Data sai7(25, 11, 2025, 12);
    if (cadastrarEstadia(6, 2, ent7, sai7)) {
        cout << "✓ Reserva futura 1 criada\n";
    }
    
    // Cliente 7 - Reserva futura
    Data ent8(22, 11, 2025, 14);
    Data sai8(28, 11, 2025, 12);
    if (cadastrarEstadia(7, 4, ent8, sai8)) {
        cout << "✓ Reserva futura 2 criada\n";
    }
}

void exibirEstatisticas() {
    cout << "\n";
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║     ESTATÍSTICAS DOS DADOS DE TESTE        ║\n";
    cout << "╚════════════════════════════════════════════╝\n\n";
    
    // Contar clientes
    ifstream arquivoClientes(ARQUIVO_CLIENTES, ios::binary);
    int totalClientes = 0;
    if (arquivoClientes.is_open()) {
        Cliente c;
        while (arquivoClientes.peek() != EOF) {
            c.carregarDeArquivo(arquivoClientes);
            if (arquivoClientes.eof()) break;
            if (c.ativo) totalClientes++;
        }
        arquivoClientes.close();
    }
    
    // Contar funcionários
    ifstream arquivoFuncionarios(ARQUIVO_FUNCIONARIOS, ios::binary);
    int totalFuncionarios = 0;
    if (arquivoFuncionarios.is_open()) {
        Funcionario f;
        while (arquivoFuncionarios.peek() != EOF) {
            f.carregarDeArquivo(arquivoFuncionarios);
            if (arquivoFuncionarios.eof()) break;
            if (f.ativo) totalFuncionarios++;
        }
        arquivoFuncionarios.close();
    }
    
    // Contar quartos
    ifstream arquivoQuartos(ARQUIVO_QUARTOS, ios::binary);
    int totalQuartos = 0, quartosOcupados = 0;
    if (arquivoQuartos.is_open()) {
        Quarto q;
        while (arquivoQuartos.peek() != EOF) {
            q.carregarDeArquivo(arquivoQuartos);
            if (arquivoQuartos.eof()) break;
            if (q.ativo) {
                totalQuartos++;
                if (q.status == OCUPADO) quartosOcupados++;
            }
        }
        arquivoQuartos.close();
    }
    
    // Contar estadias
    ifstream arquivoEstadias(ARQUIVO_ESTADIAS, ios::binary);
    int totalEstadias = 0, estadiasAtivas = 0;
    if (arquivoEstadias.is_open()) {
        Estadia e;
        while (arquivoEstadias.peek() != EOF) {
            e.carregarDeArquivo(arquivoEstadias);
            if (arquivoEstadias.eof()) break;
            if (e.ativa) {
                totalEstadias++;
                if (e.status == ATIVA) estadiasAtivas++;
            }
        }
        arquivoEstadias.close();
    }
    
    cout << "📊 Clientes cadastrados: " << totalClientes << endl;
    cout << "👥 Funcionários cadastrados: " << totalFuncionarios << endl;
    cout << "🏨 Quartos cadastrados: " << totalQuartos << endl;
    cout << "   ├─ Ocupados: " << quartosOcupados << endl;
    cout << "   └─ Disponíveis: " << (totalQuartos - quartosOcupados) << endl;
    cout << "📅 Estadias registradas: " << totalEstadias << endl;
    cout << "   ├─ Ativas: " << estadiasAtivas << endl;
    cout << "   └─ Finalizadas: " << (totalEstadias - estadiasAtivas) << endl;
    
    cout << "\n✅ Sistema populado com sucesso!\n";
    cout << "Agora você pode executar ./hotel para testar as funcionalidades.\n\n";
}


int main() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);       
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║     GERADOR DE DADOS DE TESTE              ║\n";
    cout << "║     Hotel Descanso Garantido               ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
    
    char opcao;
    cout << "\nATENÇÃO: Isso irá apagar e recriar os arquivos .dat\n";
    cout << "Deseja continuar? (s/n): ";
    cin >> opcao;
    
    if (opcao != 's' && opcao != 'S') {
        cout << "Operação cancelada.\n";
        return 0;
    }

    // Apaga os arquivos antigos
    remove(ARQUIVO_CLIENTES.c_str());
    remove(ARQUIVO_FUNCIONARIOS.c_str());
    remove(ARQUIVO_QUARTOS.c_str());
    remove(ARQUIVO_ESTADIAS.c_str());
    
    gerarClientes();
    gerarFuncionarios();
    gerarQuartos();
    gerarEstadias();
    exibirEstatisticas();
    
    return 0;
}