#include "../Headers/Estadia.h"

// --- INCLUDES NECESSÁRIOS (Faltando) ---
#include <iostream>     // Para cout, endl
#include <fstream>      // Para ifstream, ofstream
#include <iomanip>    // Para setprecision
#include <string>       // Para string
#include <vector>       // Para vector
#include <cstring>      // Para memset (se necessário)

// --- INCLUDES DOS MÓDULOS (Faltando) ---
#include "../Headers/Clientes.h"
#include "../Headers/Quarto.h"
#include "../Headers/Utils.h"

using namespace std;

// --- CONSTANTE GLOBAL (Definição) ---
const string ARQUIVO_ESTADIAS = "estadias.dat";

// --- IMPLEMENTAÇÃO DOS CONSTRUTORES ---
Estadia::Estadia() 
    : codigoEstadia(0), quantidadeDiarias(0), codigoCliente(0), numeroQuarto(0), status(ATIVA), ativa(true) {
    // Construtor padrão
}

// Assinatura corrigida para corresponder ao .h
Estadia::Estadia(int codEstadia, const Data& entrada, const Data& saida, int codCliente, int numQuarto)
    : codigoEstadia(codEstadia), dataEntrada(entrada), dataSaida(saida), 
      codigoCliente(codCliente), numeroQuarto(numQuarto), status(ATIVA), ativa(true) 
{
    // Calcula as diárias (usa a função de Utils.cpp)
    quantidadeDiarias = calcularDiarias(entrada, saida);
}

// --- IMPLEMENTAÇÃO DOS MÉTODOS ---
void Estadia::exibir() const {
    if (!ativa) return;
    cout << "\n========== ESTADIA ==========\n";
    cout << "Codigo: " << codigoEstadia << endl; // Corrigido
    cout << "Cliente (Codigo): " << codigoCliente << endl;
    cout << "Quarto (Numero): " << numeroQuarto << endl;
    dataEntrada.exibir();
    dataSaida.exibir();
    cout << "Diarias: " << quantidadeDiarias << endl;
    cout << fixed << setprecision(2);
    cout << "Valor total: R$ " << calcularValorTotal() << endl;
    cout << "Status: " << (status == ATIVA ? "ATIVA" : "FINALIZADA") << endl;
    cout << "=============================\n";
}

// Implementação Faltando
double Estadia::calcularValorTotal() const {
    // Busca o quarto para saber o valor da diária
    Quarto q = buscarQuartoPorNumero(numeroQuarto);
    if (q.numero != -1) {
        return q.valorDiaria * this->quantidadeDiarias;
    }
    return 0.0; // Se o quarto não for encontrado (erro)
}

void Estadia::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write(reinterpret_cast<const char*>(this), sizeof(Estadia));
}

void Estadia::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read(reinterpret_cast<char*>(this), sizeof(Estadia));
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DO MÓDULO ---

int gerarCodigoEstadia() {
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary | ios::ate);
    if (!arquivo.is_open() || arquivo.tellg() == 0) {
        arquivo.close();
        return 1;
    }

    arquivo.seekg(-static_cast<long>(sizeof(Estadia)), ios::end);
    Estadia ultimaEstadia;
    ultimaEstadia.carregarDeArquivo(arquivo);
    arquivo.close();
    return ultimaEstadia.codigoEstadia + 1; // Corrigido
}

// Função auxiliar (usada por Quarto.cpp e Estadia.cpp)
bool verificarDisponibilidadeQuarto(int numeroQuarto, const Data& entrada, const Data& saida) {
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivo.is_open()) {
        return true; // Arquivo não existe, então está disponível
    }

    Estadia estadia;
    while (arquivo.peek() != EOF) {
        estadia.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        // Se a estadia é no mesmo quarto, está ativa E os períodos conflitam
        if (estadia.numeroQuarto == numeroQuarto && estadia.ativa && estadia.status == ATIVA) {
            if (periodosSeSobrepoe(entrada, saida, estadia.dataEntrada, estadia.dataSaida)) {
                arquivo.close();
                return false; // Conflito! Quarto indisponível.
            }
        }
    }
    arquivo.close();
    return true; // Nenhum conflito encontrado
}


bool cadastrarEstadia(int codigoCliente, int qtdHospedes, const Data& entrada, const Data& saida) {
    
    // 1. Validar Datas
    if (!entrada.validar() || !saida.validar()) {
        cout << "Erro: Datas invalidas!" << endl;
        return false;
    }
    if (saida < entrada || saida == entrada) {
        cout << "Erro: Data de saida deve ser posterior a data de entrada!" << endl;
        return false;
    }
    
    // 2. Validar Cliente
    if (!clienteExiste(codigoCliente)) {
        cout << "Erro: Cliente nao encontrado!" << endl;
        return false;
    }
    
    // 3. Buscar Quarto (usa a função de Quarto.cpp)
    Quarto quarto = buscarQuartoDisponivel(qtdHospedes, entrada, saida);
    
    if (quarto.numero == -1) { // -1 é o sinal de "não encontrado"
        cout << "Erro: Nenhum quarto disponivel para " << qtdHospedes
             << " hospedes nesse periodo." << endl;
        return false;
    }
    
    // 4. Se tudo OK, criar e salvar a estadia
    int codigoEstadia = gerarCodigoEstadia();
    
    // Assinatura corrigida
    Estadia estadia(codigoEstadia, entrada, saida, codigoCliente, quarto.numero);
    
    ofstream arquivo(ARQUIVO_ESTADIAS, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de estadias!" << endl;
        return false;
    }
    
    estadia.salvarEmArquivo(arquivo);
    arquivo.close();

    // 5. Alterar status do quarto para OCUPADO
    alterarStatusQuarto(quarto.numero, OCUPADO);

    cout << "\n=== ESTADIA CADASTRADA COM SUCESSO! ===" << endl;
    cout << "Quarto " << quarto.numero << " alocado." << endl;
    cout << fixed << setprecision(2);
    cout << "Valor total: R$ " << estadia.calcularValorTotal() << endl;

    return true;
}

// Assinatura corrigida para VOID (como no main.cpp)
void darBaixaEstadia(int codigoEstadia) {
    ifstream arquivoLeitura(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivoLeitura.is_open()) {
        cout << "Erro ao abrir arquivo de estadias!" << endl;
        return;
    }

    // Lê todas as estadias para a memória
    vector<Estadia> estadias;
    Estadia estadia;
    bool encontrada = false;
    double valorTotal = 0.0;
    int numeroQuarto = -1;

    while (arquivoLeitura.peek() != EOF) {
        estadia.carregarDeArquivo(arquivoLeitura);
        if (arquivoLeitura.eof()) break;

        // Encontra a estadia, marca como finalizada e armazena os dados
        if (estadia.codigoEstadia == codigoEstadia && estadia.ativa && estadia.status == ATIVA) {
            estadia.status = FINALIZADA;
            valorTotal = estadia.calcularValorTotal();
            numeroQuarto = estadia.numeroQuarto;
            encontrada = true;
        }
        estadias.push_back(estadia);
    }
    arquivoLeitura.close();

    if (!encontrada) {
        cout << "Erro: Estadia nao encontrada ou ja finalizada!" << endl;
        return;
    }

    // Reescreve o arquivo com a estadia modificada
    ofstream arquivoEscrita(ARQUIVO_ESTADIAS, ios::binary | ios::trunc);
    for (const auto& e : estadias) {
        e.salvarEmArquivo(arquivoEscrita);
    }
    arquivoEscrita.close();

    // Altera o status do quarto para DESOCUPADO
    alterarStatusQuarto(numeroQuarto, DESOCUPADO);

    cout << "\n=== BAIXA REALIZADA COM SUCESSO! ===" << endl;
    cout << fixed << setprecision(2);
    cout << "Valor total a ser pago: R$ " << valorTotal << endl;
}


vector<Estadia> listarEstadiaCliente(int codigoCliente) {
    vector<Estadia> estadiasEncontradas;
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivo.is_open()) {
        return estadiasEncontradas;
    }

    Estadia estadia;
    while (arquivo.peek() != EOF) {
        estadia.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (estadia.ativa && estadia.codigoCliente == codigoCliente) {
            estadiasEncontradas.push_back(estadia);
        }
    }
    arquivo.close();
    return estadiasEncontradas;
}

vector<Estadia> listarEstadiaClientePorNome(const string& nomeCliente) {
    vector<Estadia> estadiasEncontradas;
    
    // 1. Acha todos os clientes com aquele nome
    vector<Cliente> clientes = buscarClientesPorNome(nomeCliente);
    if (clientes.empty()) {
        return estadiasEncontradas; // Nenhum cliente, nenhuma estadia
    }

    // 2. Para cada cliente, busca suas estadias
    for (const auto& cliente : clientes) {
        vector<Estadia> estadiasDoCliente = listarEstadiaCliente(cliente.codigo);
        // Adiciona as estadias encontradas ao vetor principal
        estadiasEncontradas.insert(estadiasEncontradas.end(), 
                                  estadiasDoCliente.begin(), 
                                  estadiasDoCliente.end());
    }
    
    return estadiasEncontradas;
}

void listarTodasEstadias() {
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivo.is_open()) {
        cout << "Nenhuma estadia cadastrada ainda." << endl;
        return;
    }

    Estadia estadia;
    bool encontrou = false;
    cout << "\n========== LISTA DE ESTADIAS ==========\n";

    while (arquivo.peek() != EOF) {
        estadia.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (estadia.ativa) {
            estadia.exibir();
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhuma estadia cadastrada." << endl;
    }
    arquivo.close();
}

int calcularTotalDiariasCliente(int codigoCliente) {
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivo.is_open()) {
        return 0;
    }

    Estadia estadia;
    int totalDiarias = 0;
    while (arquivo.peek() != EOF) {
        estadia.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        // Soma diárias de estadias ATIVAS e FINALIZADAS
        if (estadia.ativa && estadia.codigoCliente == codigoCliente) {
            totalDiarias += estadia.quantidadeDiarias;
        }
    }
    arquivo.close();
    return totalDiarias;
}

int calcularPontosFidelidade(int codigoCliente) {
    // A lógica é a mesma do PDF
    return calcularTotalDiariasCliente(codigoCliente) * 10;
}