#include "Headers/Clientes.h"
#include "Headers/Funcionarios.h"
#include "Headers/Quarto.h"
#include "Headers/Estadia.h"

void exibirMenuPrincipal() {
    cout << "\n";
    cout << "╔════════════════════════════════════════════╗\n";
    cout << "║   HOTEL DESCANSO GARANTIDO - SISTEMA      ║\n";
    cout << "╚════════════════════════════════════════════╝\n";
    cout << "\n[1] Cadastrar Cliente\n";
    cout << "[2] Cadastrar Funcionário\n";
    cout << "[3] Cadastrar Quarto\n";
    cout << "[4] Cadastrar Estadia\n";
    cout << "[5] Dar Baixa em Estadia\n";
    cout << "[6] Pesquisar Cliente\n";
    cout << "[7] Pesquisar Funcionário\n";
    cout << "[8] Listar Estadias de Cliente\n";
    cout << "[9] Calcular Pontos de Fidelidade\n";
    cout << "[10] Listar Todos os Clientes\n";
    cout << "[11] Listar Todos os Funcionários\n";
    cout << "[12] Listar Todos os Quartos\n";
    cout << "[13] Listar Quartos Disponíveis\n";
    cout << "[14] Listar Todas as Estadias\n";
    cout << "[0] Sair\n";
    cout << "\nEscolha uma opção: ";
}

void menuCadastrarCliente() {
    cout << "\n=== CADASTRAR CLIENTE ===\n";
    
    Cliente cliente;
    cliente.codigo = gerarCodigoCliente();
    
    cout << "Código gerado automaticamente: " << cliente.codigo << endl;
    
    limparBuffer();
    cout << "Nome: ";
    getline(cin, cliente.nome);
    
    cout << "Endereço: ";
    getline(cin, cliente.endereco);
    
    cout << "Telefone: ";
    getline(cin, cliente.telefone);
    
    cadastrarCliente(cliente);
    pausar();
}

void menuCadastrarFuncionario() {
    cout << "\n=== CADASTRAR FUNCIONÁRIO ===\n";
    
    Funcionario funcionario;
    funcionario.codigo = gerarCodigoFuncionario();
    
    cout << "Código gerado automaticamente: " << funcionario.codigo << endl;
    
    limparBuffer();
    cout << "Nome: ";
    getline(cin, funcionario.nome);
    
    cout << "Telefone: ";
    getline(cin, funcionario.telefone);
    
    cout << "Cargo (recepcionista/auxiliar de limpeza/garçom/gerente): ";
    getline(cin, funcionario.cargo);
    
    cout << "Salário: R$ ";
    cin >> funcionario.salario;
    
    cadastrarFuncionario(funcionario);
    pausar();
}

void menuCadastrarQuarto() {
    cout << "\n=== CADASTRAR QUARTO ===\n";
    
    Quarto quarto;
    
    cout << "Número do quarto: ";
    cin >> quarto.numero;
    
    cout << "Quantidade de hóspedes: ";
    cin >> quarto.quantidadeHospedes;
    
    cout << "Valor da diária: R$ ";
    cin >> quarto.valorDiaria;
    
    cadastrarQuarto(quarto);
    pausar();
}

void menuCadastrarEstadia() {
    cout << "\n=== CADASTRAR ESTADIA ===\n";
    
    int codigoCliente, qtdHospedes;
    
    cout << "Código do cliente: ";
    cin >> codigoCliente;
    
    cout << "Quantidade de hóspedes: ";
    cin >> qtdHospedes;
    
    Data entrada = lerData("Data de entrada (Check-in às 14h):");
    entrada.hora = 14;
    
    Data saida = lerData("Data de saída (Check-out às 12h):");
    saida.hora = 12;
    
    cadastrarEstadia(codigoCliente, qtdHospedes, entrada, saida);
    pausar();
}

void menuDarBaixaEstadia() {
    cout << "\n=== DAR BAIXA EM ESTADIA ===\n";
    
    int codigoEstadia;
    cout << "Código da estadia: ";
    cin >> codigoEstadia;
    
    darBaixaEstadia(codigoEstadia);
    pausar();
}

void menuPesquisarCliente() {
    cout << "\n=== PESQUISAR CLIENTE ===\n";
    cout << "[1] Por código\n";
    cout << "[2] Por nome\n";
    cout << "Escolha: ";
    
    int opcao;
    cin >> opcao;
    
    if (opcao == 1) {
        int codigo;
        cout << "Código do cliente: ";
        cin >> codigo;
        
        Cliente* cliente = buscarClientePorCodigo(codigo);
        if (cliente != nullptr) {
            cliente->exibir();
            delete cliente;
        } else {
            cout << "Cliente não encontrado!" << endl;
        }
    } else if (opcao == 2) {
        limparBuffer();
        string nome;
        cout << "Nome do cliente (ou parte dele): ";
        getline(cin, nome);
        
        vector<Cliente> clientes = buscarClientesPorNome(nome);
        if (clientes.empty()) {
            cout << "Nenhum cliente encontrado!" << endl;
        } else {
            for (const auto& cliente : clientes) {
                cliente.exibir();
            }
        }
    }
    
    pausar();
}

void menuPesquisarFuncionario() {
    cout << "\n=== PESQUISAR FUNCIONÁRIO ===\n";
    cout << "[1] Por código\n";
    cout << "[2] Por nome\n";
    cout << "Escolha: ";
    
    int opcao;
    cin >> opcao;
    
    if (opcao == 1) {
        int codigo;
        cout << "Código do funcionário: ";
        cin >> codigo;
        
        Funcionario* funcionario = buscarFuncionarioPorCodigo(codigo);
        if (funcionario != nullptr) {
            funcionario->exibir();
            delete funcionario;
        } else {
            cout << "Funcionário não encontrado!" << endl;
        }
    } else if (opcao == 2) {
        limparBuffer();
        string nome;
        cout << "Nome do funcionário (ou parte dele): ";
        getline(cin, nome);
        
        vector<Funcionario> funcionarios = buscarFuncionariosPorNome(nome);
        if (funcionarios.empty()) {
            cout << "Nenhum funcionário encontrado!" << endl;
        } else {
            for (const auto& func : funcionarios) {
                func.exibir();
            }
        }
    }
    
    pausar();
}

void menuListarEstadiasCliente() {
    cout << "\n=== ESTADIAS DE CLIENTE ===\n";
    cout << "[1] Por código\n";
    cout << "[2] Por nome\n";
    cout << "Escolha: ";
    
    int opcao;
    cin >> opcao;
    
    vector<Estadia> estadias;
    
    if (opcao == 1) {
        int codigo;
        cout << "Código do cliente: ";
        cin >> codigo;
        
        estadias = listarEstadiasCliente(codigo);
    } else if (opcao == 2) {
        limparBuffer();
        string nome;
        cout << "Nome do cliente (ou parte dele): ";
        getline(cin, nome);
        
        estadias = listarEstadiasClientePorNome(nome);
    }
    
    if (estadias.empty()) {
        cout << "Nenhuma estadia encontrada para este cliente!" << endl;
    } else {
        cout << "\nEstadias encontradas:\n";
        for (const auto& estadia : estadias) {
            estadia.exibir();
        }
    }
    
    pausar();
}

void menuCalcularPontosFidelidade() {
    cout << "\n=== PONTOS DE FIDELIDADE ===\n";
    
    int codigo;
    cout << "Código do cliente: ";
    cin >> codigo;
    
    Cliente* cliente = buscarClientePorCodigo(codigo);
    if (cliente == nullptr) {
        cout << "Cliente não encontrado!" << endl;
    } else {
        int pontos = calcularPontosFidelidade(codigo);
        int totalDiarias = calcularTotalDiariasCliente(codigo);
        
        cout << "\nCliente: " << cliente->nome << endl;
        cout << "Total de diárias: " << totalDiarias << endl;
        cout << "Pontos de fidelidade: " << pontos << " pontos" << endl;
        cout << "(10 pontos por diária)\n";
        
        delete cliente;
    }
    
    pausar();
}

int main() {
    int opcao;
    
    do {
        exibirMenuPrincipal();
        cin >> opcao;
        
        switch (opcao) {
            case 1:
                menuCadastrarCliente();
                break;
            case 2:
                menuCadastrarFuncionario();
                break;
            case 3:
                menuCadastrarQuarto();
                break;
            case 4:
                menuCadastrarEstadia();
                break;
            case 5:
                menuDarBaixaEstadia();
                break;
            case 6:
                menuPesquisarCliente();
                break;
            case 7:
                menuPesquisarFuncionario();
                break;
            case 8:
                menuListarEstadiasCliente();
                break;
            case 9:
                menuCalcularPontosFidelidade();
                break;
            case 10:
                listarClientes();
                pausar();
                break;
            case 11:
                listarFuncionarios();
                pausar();
                break;
            case 12:
                listarQuartos();
                pausar();
                break;
            case 13:
                listarQuartosDisponiveis();
                pausar();
                break;
            case 14:
                listarTodasEstadias();
                pausar();
                break;
            case 0:
                cout << "\nObrigado por usar o sistema do Hotel Descanso Garantido!\n";
                break;
            default:
                cout << "\nOpção inválida! Tente novamente.\n";
                pausar();
        }
        
    } while (opcao != 0);
    
    return 0;
}