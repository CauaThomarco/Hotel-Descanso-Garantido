#include "../Headers/Quarto.h"
#include "../Headers/Utils.h"
#include "../Headers/Estadia.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <iomanip>

using namespace std;

// --- ARQUIVO DE DADOS ---
const string ARQUIVO_QUARTOS = "quartos.bin";

Quarto::Quarto() 
    : numero(0), maxHospedes(0), valorDiaria(0.0), status(DESOCUPADO), ativo(true) {
}

Quarto::Quarto(int num, int maxHosp, double valor)
    : numero(num), maxHospedes(maxHosp), valorDiaria(valor), status(DESOCUPADO), ativo(true) {
}

// --- IMPLEMENTAÇÃO DOS MÉTODOS ---
void Quarto::exibir() const {
    if (!ativo) return;
    cout << "----------------------------------" << endl;
    cout << "Numero: " << numero << endl;
    cout << "Max. Hospedes: " << maxHospedes << endl;
    cout << "Valor Diaria: R$ " << fixed << setprecision(2) << valorDiaria << endl;
    cout << "Status: " << obterStatusString() << endl;
    cout << "----------------------------------" << endl;
}

string Quarto::obterStatusString() const {
    return (status == DESOCUPADO) ? "Desocupado" : "Ocupado";
}

void Quarto::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write(reinterpret_cast<const char*>(this), sizeof(Quarto));
}

void Quarto::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read(reinterpret_cast<char*>(this), sizeof(Quarto));
}

// --- IMPLEMENTAÇÃO DAS FUNÇÕES DO MÓDULO ---

bool cadastrarQuarto(const Quarto& quarto) {
    if (quartoExiste(quarto.numero)) {
        cout << "Erro: Ja existe um quarto com o numero " << quarto.numero << endl;
        return false;
    }

    ofstream arquivo(ARQUIVO_QUARTOS, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de quartos!" << endl;
        return false;
    }

    quarto.salvarEmArquivo(arquivo);
    arquivo.close();

    cout << "Quarto cadastrado com sucesso! Numero: " << quarto.numero << endl;
    return true;
}

Quarto buscarQuartoPorNumero(int numero) {
    ifstream arquivo(ARQUIVO_QUARTOS, ios::binary);
    Quarto quarto;

    if (!arquivo.is_open()) {
        quarto.numero = -1;
        return quarto;
    }

    while (arquivo.peek() != EOF) {
        quarto.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (quarto.numero == numero && quarto.ativo) {
            arquivo.close();
            return quarto;
        }
    }

    arquivo.close();
    quarto.numero = -1;
    return quarto;
}

void listarQuartos() {
    ifstream arquivo(ARQUIVO_QUARTOS, ios::binary);
    if (!arquivo.is_open()) {
        cout << "Nenhum quarto cadastrado ainda." << endl;
        return;
    }

    Quarto quarto;
    bool encontrou = false;
    cout << "\n========== LISTA DE QUARTOS ==========\n";

    while (arquivo.peek() != EOF) {
        quarto.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (quarto.ativo) {
            quarto.exibir();
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum quarto cadastrado." << endl;
    }
    arquivo.close();
}

void listarQuartosDisponiveis() {
    ifstream arquivo(ARQUIVO_QUARTOS, ios::binary);
    if (!arquivo.is_open()) {
        cout << "Nenhum quarto cadastrado ainda." << endl;
        return;
    }

    Quarto quarto;
    bool encontrou = false;
    cout << "\n========== QUARTOS DISPONIVEIS ==========\n";

    while (arquivo.peek() != EOF) {
        quarto.carregarDeArquivo(arquivo);
        if (arquivo.eof()) break;

        if (quarto.ativo && quarto.status == DESOCUPADO) {
            quarto.exibir();
            encontrou = true;
        }
    }

    if (!encontrou) {
        cout << "Nenhum quarto disponivel no momento." << endl;
    }
    arquivo.close();
}

bool quartoExiste(int numero) {
    Quarto quarto = buscarQuartoPorNumero(numero);
    return (quarto.numero != -1);
}

bool alterarStatusQuarto(int numero, StatusQuarto novoStatus) {
    ifstream arquivoLeitura(ARQUIVO_QUARTOS, ios::binary);
    if (!arquivoLeitura.is_open()) {
        cout << "Erro: Arquivo de quartos nao encontrado." << endl;
        return false;
    }

    vector<Quarto> quartos;
    Quarto quarto;
    bool quartoEncontrado = false;

    while (arquivoLeitura.peek() != EOF) {
        quarto.carregarDeArquivo(arquivoLeitura);
        if (arquivoLeitura.eof()) break;

        if (quarto.numero == numero && quarto.ativo) {
            quarto.status = novoStatus;
            quartoEncontrado = true;
        }
        quartos.push_back(quarto);
    }
    arquivoLeitura.close();

    if (!quartoEncontrado) {
        cout << "Erro: Quarto " << numero << " nao foi encontrado." << endl;
        return false;
    }

    ofstream arquivoEscrita(ARQUIVO_QUARTOS, ios::binary | ios::trunc);
    if (!arquivoEscrita.is_open()) {
        cout << "Erro fatal: Nao foi possivel reescrever o arquivo de quartos." << endl;
        return false;
    }

    for (const auto& q : quartos) {
        q.salvarEmArquivo(arquivoEscrita);
    }
    arquivoEscrita.close();
    
    return true;
}

Quarto buscarQuartoDisponivel(int qtdHospedes, const Data& entrada, const Data& saida) {
    ifstream arqQuartos(ARQUIVO_QUARTOS, ios::binary);
    Quarto quarto;
    quarto.numero = -1;

    if (!arqQuartos.is_open()) {
        cout << "Arquivo de quartos não existe." << endl;
        return quarto;
    }

    while (arqQuartos.peek() != EOF) {
        quarto.carregarDeArquivo(arqQuartos);
        if (arqQuartos.eof()) break;

        if (quarto.ativo && quarto.maxHospedes >= qtdHospedes) {
            
            if (verificarDisponibilidadeQuarto(quarto.numero, entrada, saida)) {
                arqQuartos.close();
                return quarto;
            }
        }
    }

    arqQuartos.close();
    quarto.numero = -1;
    return quarto;
}