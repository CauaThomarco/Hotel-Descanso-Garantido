#ifndef ESTADIA_H
#define ESTADIA_H

#include <string>
#include <vector>
#include <fstream> 

using namespace std;

extern const string ARQUIVO_ESTADIAS;

struct Data {
    int dia;
    int mes;
    int ano;
    int hora;

    Data();
    Data(int d, int m, int a, int h);

    bool validar() const;
    void exibir() const;
    int compararCom(const Data& outra) const;
    bool operator<(const Data& outra) const;
    bool operator>(const Data& outra) const;
    bool operator==(const Data& outra) const;
    bool operator<=(const Data& outra) const;
    bool operator>=(const Data& outra) const;
};

enum StatusEstadia {
    ATIVA,
    FINALIZADA
};

class Estadia {
public:
    int codigoEstadia;
    Data dataEntrada;
    Data dataSaida;
    int quantidadeDiarias;
    int codigoCliente;
    int numeroQuarto;
    StatusEstadia status;
    bool ativa;

    Estadia();
    Estadia(int codEstadia, const Data& entrada, const Data& saida, int codCliente, int numQuarto); 

    void exibir() const;
    double calcularValorTotal() const;
    void salvarEmArquivo(ofstream& arquivo) const;
    void carregarDeArquivo(ifstream& arquivo);
};


int gerarCodigoEstadia();
bool cadastrarEstadia(int codigoCliente, int qtdHospedes, const Data& entrada, const Data& saida);

void darBaixaEstadia(int codigoEstadia);

bool verificarDisponibilidadeQuarto(int numeroQuarto, const Data& entrada, const Data& saida);
vector<Estadia> listarEstadiaCliente(int codigoCliente);
vector<Estadia> listarEstadiaClientePorNome(const string& nomeCliente);
void listarTodasEstadias();

int calcularTotalDiariasCliente(int codigoCliente);
int calcularPontosFidelidade(int codigoCliente);

#endif