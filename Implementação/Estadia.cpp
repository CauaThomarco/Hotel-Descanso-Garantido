#include "../Headers/Estadia.h"

const string ARQUIVO_ESTADIAS = "estadias.dat";

// Construtores
Estadia::Estadia() : codigo(0), quantidadeDiarias(0), codigoCliente(0), numeroQuarto(0), status(ATIVA), ativa(true) {}

Estadia::Estadia(int cod, const Data& entrada, const Data& saida, int codCliente, int numQuarto)
    : codigo(cod), dataEntrada(entrada), dataSaida(saida), codigoCliente(codCliente), 
      numeroQuarto(numQuarto), status(ATIVA), ativa(true) {
    quantidadeDiarias = calcularDiarias(entrada, saida);
}

// Exibir estadia
void Estadia::exibir() const {
    cout << "\n========== ESTADIA ==========\n";
    cout << "Código: " << codigo << endl;
    cout << "Cliente (cód): " << codigoCliente << endl;
    cout << "Quarto: " << numeroQuarto << endl;
    cout << "Data entrada: ";
    dataEntrada.exibir();
    cout << endl << "Data saída: ";
    dataSaida.exibir();
    cout << endl << "Quantidade de diárias: " << quantidadeDiarias << endl;
    cout << fixed << setprecision(2);
    cout << "Valor total: R$ " << calcularValorTotal() << endl;
    cout << "Status: " << (status == ATIVA ? "ATIVA" : "FINALIZADA") << endl;
    cout << "============================\n";
}

// Calcular valor total
float Estadia::calcularValorTotal() const {
    Quarto* quarto = buscarQuartoPorNumero(numeroQuarto);
    if (quarto == nullptr) {
        return 0.0;
    }
    
    float total = quarto->valorDiaria * quantidadeDiarias;
    delete quarto;
    return total;
}

// Serialização
void Estadia::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write((char*)&codigo, sizeof(int));
    arquivo.write((char*)&dataEntrada, sizeof(Data));
    arquivo.write((char*)&dataSaida, sizeof(Data));
    arquivo.write((char*)&quantidadeDiarias, sizeof(int));
    arquivo.write((char*)&codigoCliente, sizeof(int));
    arquivo.write((char*)&numeroQuarto, sizeof(int));
    arquivo.write((char*)&status, sizeof(StatusEstadia));
    arquivo.write((char*)&ativa, sizeof(bool));
}

void Estadia::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read((char*)&codigo, sizeof(int));
    arquivo.read((char*)&dataEntrada, sizeof(Data));
    arquivo.read((char*)&dataSaida, sizeof(Data));
    arquivo.read((char*)&quantidadeDiarias, sizeof(int));
    arquivo.read((char*)&codigoCliente, sizeof(int));
    arquivo.read((char*)&numeroQuarto, sizeof(int));
    arquivo.read((char*)&status, sizeof(StatusEstadia));
    arquivo.read((char*)&ativa, sizeof(bool));
}

// Gerar código único
int gerarCodigoEstadia() {
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    int maiorCodigo = 0;
    
    if (arquivo.is_open()) {
        Estadia estadia;
        while (!arquivo.eof()) {
            try {
                estadia.carregarDeArquivo(arquivo);
                if (estadia.codigo > maiorCodigo) {
                    maiorCodigo = estadia.codigo;
                }
            } catch (...) {
                break;
            }
        }
        arquivo.close();
    }
    
    return maiorCodigo + 1;
}

// Verificar disponibilidade de quarto
bool verificarDisponibilidadeQuarto(int numeroQuarto, const Data& entrada, const Data& saida) {
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivo.is_open()) {
        return true; // Se não há estadias, está disponível
    }
    
    Estadia estadia;
    while (!arquivo.eof()) {
        try {
            estadia.carregarDeArquivo(arquivo);
            if (estadia.numeroQuarto == numeroQuarto && estadia.ativa && estadia.status == ATIVA) {
                // Verifica se há sobreposição de datas
                if (periodosSesobrepoe(entrada, saida, estadia.dataEntrada, estadia.dataSaida)) {
                    arquivo.close();
                    return false; // Quarto ocupado neste período
                }
            }
        } catch (...) {
            break;
        }
    }
    
    arquivo.close();
    return true; // Quarto disponível
}

// Cadastrar estadia
bool cadastrarEstadia(int codigoCliente, int qtdHospedes, const Data& entrada, const Data& saida) {
    // Validar datas
    if (!entrada.validar() || !saida.validar()) {
        cout << "Erro: Datas inválidas!" << endl;
        return false;
    }
    
    if (entrada >= saida) {
        cout << "Erro: Data de saída deve ser posterior à data de entrada!" << endl;
        return false;
    }
    
    // Verificar se cliente existe
    if (!clienteExiste(codigoCliente)) {
        cout << "Erro: Cliente não encontrado!" << endl;
        return false;
    }
    
    // Buscar quarto disponível
    ifstream arquivo(ARQUIVO_QUARTOS, ios::binary);
    if (!arquivo.is_open()) {
        cout << "Erro: Nenhum quarto cadastrado!" << endl;
        return false;
    }
    
    Quarto quarto;
    int quartoEncontrado = -1;
    
    while (!arquivo.eof()) {
        try {
            quarto.carregarDeArquivo(arquivo);
            if (quarto.ativo && quarto.quantidadeHospedes >= qtdHospedes) {
                // Verificar se está disponível no período
                if (verificarDisponibilidadeQuarto(quarto.numero, entrada, saida)) {
                    quartoEncontrado = quarto.numero;
                    break;
                }
            }
        } catch (...) {
            break;
        }
    }
    arquivo.close();
    
    if (quartoEncontrado == -1) {
        cout << "Erro: Nenhum quarto disponível para " << qtdHospedes 
             << " hóspede(s) no período solicitado!" << endl;
        return false;
    }
    
    // Criar estadia
    int codigoEstadia = gerarCodigoEstadia();
    Estadia estadia(codigoEstadia, entrada, saida, codigoCliente, quartoEncontrado);
    
    // Salvar estadia
    ofstream arquivoEst(ARQUIVO_ESTADIAS, ios::binary | ios::app);
    if (!arquivoEst.is_open()) {
        cout << "Erro ao abrir arquivo de estadias!" << endl;
        return false;
    }
    
    estadia.salvarEmArquivo(arquivoEst);
    arquivoEst.close();
    
    // Alterar status do quarto para OCUPADO
    alterarStatusQuarto(quartoEncontrado, OCUPADO);
    
    cout << "\n=== ESTADIA CADASTRADA COM SUCESSO! ===" << endl;
    cout << "Código da estadia: " << codigoEstadia << endl;
    cout << "Quarto alocado: " << quartoEncontrado << endl;
    cout << "Quantidade de diárias: " << estadia.quantidadeDiarias << endl;
    cout << fixed << setprecision(2);
    cout << "Valor total: R$ " << estadia.calcularValorTotal() << endl;
    cout << "=======================================" << endl;
    
    return true;
}

// Dar baixa em estadia
bool darBaixaEstadia(int codigoEstadia) {
    ifstream arquivoLeitura(ARQUIVO_ESTADIAS, ios::binary);
    if (!arquivoLeitura.is_open()) {
        cout << "Erro ao abrir arquivo de estadias!" << endl;
        return false;
    }
    
    vector<Estadia> estadias;
    Estadia estadia;
    bool encontrou = false;
    int numeroQuarto = 0;
    float valorTotal = 0.0;
    
    // Ler todas as estadias
    while (!arquivoLeitura.eof()) {
        try {
            estadia.carregarDeArquivo(arquivoLeitura);
            if (estadia.codigo == codigoEstadia && estadia.ativa && estadia.status == ATIVA) {
                estadia.status = FINALIZADA;
                numeroQuarto = estadia.numeroQuarto;
                valorTotal = estadia.calcularValorTotal();
                encontrou = true;
            }
            estadias.push_back(estadia);
        } catch (...) {
            break;
        }
    }
    arquivoLeitura.close();
    
    if (!encontrou) {
        cout << "Erro: Estadia não encontrada ou já finalizada!" << endl;
        return false;
    }
    
    // Reescrever arquivo
    ofstream arquivoEscrita(ARQUIVO_ESTADIAS, ios::binary | ios::trunc);
    if (!arquivoEscrita.is_open()) {
        return false;
    }
    
    for (const auto& e : estadias) {
        e.salvarEmArquivo(arquivoEscrita);
    }
    arquivoEscrita.close();
    
    // Liberar quarto
    alterarStatusQuarto(numeroQuarto, DESOCUPADO);
    
    cout << "\n=== BAIXA REALIZADA COM SUCESSO! ===" << endl;
    cout << fixed << setprecision(2);
    cout << "Valor total a ser pago: R$ " << valorTotal << endl;
    cout << "Quarto " << numeroQuarto << " liberado." << endl;
    cout << "====================================" << endl;
    
    return true;
}

// Listar estadias de um cliente
vector<Estadia> listarEstadiasCliente(int codigoCliente) {
    vector<Estadia> estadias;
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    
    if (!arquivo.is_open()) {
        return estadias;
    }
    
    Estadia estadia;
    while (!arquivo.eof()) {
        try {
            estadia.carregarDeArquivo(arquivo);
            if (estadia.ativa && estadia.codigoCliente == codigoCliente) {
                estadias.push_back(estadia);
            }
        } catch (...) {
            break;
        }
    }
    
    arquivo.close();
    return estadias;
}

// Listar estadias por nome do cliente
vector<Estadia> listarEstadiasClientePorNome(const string& nomeCliente) {
    vector<Estadia> resultado;
    vector<Cliente> clientes = buscarClientesPorNome(nomeCliente);
    
    for (const auto& cliente : clientes) {
        vector<Estadia> estadias = listarEstadiasCliente(cliente.codigo);
        resultado.insert(resultado.end(), estadias.begin(), estadias.end());
    }
    
    return resultado;
}

// Listar todas as estadias
void listarTodasEstadias() {
    ifstream arquivo(ARQUIVO_ESTADIAS, ios::binary);
    
    if (!arquivo.is_open()) {
        cout << "Nenhuma estadia cadastrada ainda." << endl;
        return;
    }
    
    Estadia estadia;
    bool encontrou = false;
    
    cout << "\n========== LISTA DE ESTADIAS ==========\n";
    while (!arquivo.eof()) {
        try {
            estadia.carregarDeArquivo(arquivo);
            if (estadia.ativa) {
                estadia.exibir();
                encontrou = true;
            }
        } catch (...) {
            break;
        }
    }
    
    if (!encontrou) {
        cout << "Nenhuma estadia cadastrada." << endl;
    }
    
    arquivo.close();
}

// Calcular total de diárias de um cliente
int calcularTotalDiariasCliente(int codigoCliente) {
    vector<Estadia> estadias = listarEstadiasCliente(codigoCliente);
    int totalDiarias = 0;
    
    for (const auto& estadia : estadias) {
        totalDiarias += estadia.quantidadeDiarias;
    }
    
    return totalDiarias;
}