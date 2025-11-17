#include "../Headers/Estadia.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cstring>

#include "../Headers/Clientes.h"
#include "../Headers/Quarto.h"
#include "../Headers/Utils.h"

using namespace std;

const string ARQUIVO_ESTADIAS = "estadias.bin";

Estadia::Estadia() 
    : codigoEstadia(0), quantidadeDiarias(0), codigoCliente(0), numeroQuarto(0), status(ATIVA), ativa(true) {
}

Estadia::Estadia(int codEstadia, const Data& entrada, const Data& saida, int codCliente, int numQuarto)
    : codigoEstadia(codEstadia), dataEntrada(entrada), dataSaida(saida), 
      codigoCliente(codCliente), numeroQuarto(numQuarto), status(ATIVA), ativa(true) 
{
    quantidadeDiarias = calcularDiarias(entrada, saida);
}

void Estadia::exibir() const {
    if (!ativa) return;
    cout << "\n========== ESTADIA ==========\n";
    cout << "Codigo: " << codigoEstadia << endl;
    cout << "Cliente (Codigo): " << codigoCliente << endl;
    cout << "Quarto (Numero): " << numeroQuarto << endl;
    cout << "Data de entrada: ";
    dataEntrada.exibir();
    cout << "Data de saida: ";
    dataSaida.exibir();
    cout << "Diarias: " << quantidadeDiarias << endl;
    cout << fixed << setprecision(2);
    cout << "Valor total: R$ " << calcularValorTotal() << endl;
    cout << "Status: " << (status == ATIVA ? "ATIVA" : "FINALIZADA") << endl;
    cout << "=============================\n";
}

double Estadia::calcularValorTotal() const {
    Quarto q = buscarQuartoPorNumero(numeroQuarto);
    if (q.numero != -1) {
        return q.valorDiaria * this->quantidadeDiarias;
    }
    return 0.0;
}

void Estadia::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write(reinterpret_cast<const char*>(this), sizeof(Estadia));
}

void Estadia::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read(reinterpret_cast<char*>(this), sizeof(Estadia));
}

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
    return ultimaEstadia.codigoEstadia + 1;
}

bool verificarDisponibilidadeQuarto(int numeroQuarto, const Data& entrada, const Data& saida) {
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivo.is_open()) {
        return true; // Se não há arquivo, está disponível
    }

    Estadia estadia;
    while (arquivo.peek() != EOF) {
        estadia.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (estadia.numeroQuarto == numeroQuarto && estadia.ativa && estadia.status == ATIVA) {
            if (periodosSeSobrepoe(entrada, saida, estadia.dataEntrada, estadia.dataSaida)) {
                arquivo.close();
                return false;
            }
        }
    }
    arquivo.close();
    return true;
}

bool cadastrarEstadia(int codigoCliente, int qtdHospedes, const Data& entrada, const Data& saida) {
    
    if (!entrada.validar() || !saida.validar()) {
        cout << "Erro: Datas invalidas!" << endl;
        return false;
    }
    if (saida < entrada || saida == entrada) {
        cout << "Erro: Data de saida deve ser posterior a data de entrada!" << endl;
        return false;
    }
    
    if (!clienteExiste(codigoCliente)) {
        cout << "Erro: Cliente nao encontrado!" << endl;
        return false;
    }
    
    Quarto quarto = buscarQuartoDisponivel(qtdHospedes, entrada, saida);
    
    if (quarto.numero == -1) {
        cout << "Erro: Nenhum quarto disponivel para " << qtdHospedes
             << " hospedes nesse periodo." << endl;
        return false;
    }
    
    int codigoEstadia = gerarCodigoEstadia();
    Estadia estadia(codigoEstadia, entrada, saida, codigoCliente, quarto.numero);
    
    ofstream arquivo(ARQUIVO_ESTADIAS, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de estadias!" << endl;
        return false;
    }
    
    estadia.salvarEmArquivo(arquivo);
    arquivo.close();

    cout << "\n=== ESTADIA CADASTRADA COM SUCESSO! ===" << endl;
    cout << "Codigo da Estadia: " << codigoEstadia << endl;
    cout << "Quarto " << quarto.numero << " reservado." << endl;
    cout << fixed << setprecision(2);
    cout << "Valor total: R$ " << estadia.calcularValorTotal() << endl;

    return true;
}

void darBaixaEstadia(int codigoEstadia) {
    ifstream arquivoLeitura(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivoLeitura.is_open()) {
        cout << "Erro ao abrir arquivo de estadias!" << endl;
        return;
    }

    vector<Estadia> estadias;
    Estadia estadia;
    bool encontrada = false;
    double valorTotal = 0.0;

    while (arquivoLeitura.peek() != EOF) {
        estadia.carregarDeArquivo(arquivoLeitura);
        if (arquivoLeitura.eof()) break;

        if (estadia.codigoEstadia == codigoEstadia && estadia.ativa && estadia.status == ATIVA) {
            estadia.status = FINALIZADA;
            valorTotal = estadia.calcularValorTotal();
            encontrada = true;
        }
        estadias.push_back(estadia);
    }
    arquivoLeitura.close();

    if (!encontrada) {
        cout << "Erro: Estadia nao encontrada ou ja finalizada!" << endl;
        return;
    }

    ofstream arquivoEscrita(ARQUIVO_ESTADIAS, ios::binary | ios::trunc);
    for (const auto& e : estadias) {
        e.salvarEmArquivo(arquivoEscrita);
    }
    arquivoEscrita.close();

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
    
    vector<Cliente> clientes = buscarClientesPorNome(nomeCliente);
    if (clientes.empty()) {
        return estadiasEncontradas;
    }

    for (const auto& cliente : clientes) {
        vector<Estadia> estadiasDoCliente = listarEstadiaCliente(cliente.codigo);
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

        if (estadia.ativa && estadia.codigoCliente == codigoCliente) {
            totalDiarias += estadia.quantidadeDiarias;
        }
    }
    arquivo.close();
    return totalDiarias;
}

int calcularPontosFidelidade(int codigoCliente) {
    return calcularTotalDiariasCliente(codigoCliente) * 10;
}