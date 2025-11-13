#include "../Headers/Quarto.h"

const string ARQUIVO_QUARTOS = "quartos.dat";

// Construtores
Quarto::Quarto() : numero(0), quantidadeHospedes(0), valorDiaria(0.0), status(DESOCUPADO), ativo(true) {}

Quarto::Quarto(int num, int qtdHosp, float valor)
    : numero(num), quantidadeHospedes(qtdHosp), valorDiaria(valor), status(DESOCUPADO), ativo(true) {}

// Exibir quarto
void Quarto::exibir() const {
    cout << "\n========== QUARTO ==========\n";
    cout << "Número: " << numero << endl;
    cout << "Capacidade: " << quantidadeHospedes << " hóspede(s)" << endl;
    cout << fixed << setprecision(2);
    cout << "Valor da diária: R$ " << valorDiaria << endl;
    cout << "Status: " << obterStatusString() << endl;
    cout << "===========================\n";
}

string Quarto::obterStatusString() const {
    return (status == OCUPADO) ? "OCUPADO" : "DESOCUPADO";
}

// Serialização
void Quarto::salvarEmArquivo(ofstream& arquivo) const {
    arquivo.write((char*)&numero, sizeof(int));
    arquivo.write((char*)&quantidadeHospedes, sizeof(int));
    arquivo.write((char*)&valorDiaria, sizeof(float));
    arquivo.write((char*)&status, sizeof(StatusQuarto));
    arquivo.write((char*)&ativo, sizeof(bool));
}

void Quarto::carregarDeArquivo(ifstream& arquivo) {
    arquivo.read((char*)&numero, sizeof(int));
    arquivo.read((char*)&quantidadeHospedes, sizeof(int));
    arquivo.read((char*)&valorDiaria, sizeof(float));
    arquivo.read((char*)&status, sizeof(StatusQuarto));
    arquivo.read((char*)&ativo, sizeof(bool));
}

// Cadastrar quarto
bool cadastrarQuarto(const Quarto& quarto) {
    // Verifica se já existe
    if (quartoExiste(quarto.numero)) {
        cout << "Erro: Já existe um quarto com o número " << quarto.numero << endl;
        return false;
    }
    
    ofstream arquivo(ARQUIVO_QUARTOS, ios::binary | ios::app);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir arquivo de quartos!" << endl;
        return false;
    }
    
    quarto.salvarEmArquivo(arquivo);
    arquivo.close();
    
    cout << "Quarto cadastrado com sucesso! Número: " << quarto.numero << endl;
    return true;
}

// Buscar quarto por número
Quarto* buscarQuartoPorNumero(int numero) {
    ifstream arquivo(ARQUIVO_QUARTOS, ios::binary);
    if (!arquivo.is_open()) {
        return nullptr;
    }
    
    Quarto* quarto = new Quarto();
    while (!arquivo.eof()) {
        try {
            quarto->carregarDeArquivo(arquivo);
            if (quarto->numero == numero && quarto->ativo) {
                arquivo.close();
                return quarto;
            }
        } catch (...) {
            break;
        }
    }
    
    arquivo.close();
    delete quarto;
    return nullptr;
}

// Listar todos os quartos
void listarQuartos() {
    ifstream arquivo(ARQUIVO_QUARTOS, ios::binary);
    
    if (!arquivo.is_open()) {
        cout << "Nenhum quarto cadastrado ainda." << endl;
        return;
    }
    
    Quarto quarto;
    bool encontrou = false;
    
    cout << "\n========== LISTA DE QUARTOS ==========\n";
    while (!arquivo.eof()) {
        try {
            quarto.carregarDeArquivo(arquivo);
            if (quarto.ativo) {
                quarto.exibir();
                encontrou = true;
            }
        } catch (...) {
            break;
        }
    }
    
    if (!encontrou) {
        cout << "Nenhum quarto cadastrado." << endl;
    }
    
    arquivo.close();
}

// Listar quartos disponíveis
void listarQuartosDisponiveis() {
    ifstream arquivo(ARQUIVO_QUARTOS, ios::binary);
    
    if (!arquivo.is_open()) {
        cout << "Nenhum quarto cadastrado ainda." << endl;
        return;
    }
    
    Quarto quarto;
    bool encontrou = false;
    
    cout << "\n========== QUARTOS DISPONÍVEIS ==========\n";
    while (!arquivo.eof()) {
        try {
            quarto.carregarDeArquivo(arquivo);
            if (quarto.ativo && quarto.status == DESOCUPADO) {
                quarto.exibir();
                encontrou = true;
            }
        } catch (...) {
            break;
        }
    }
    
    if (!encontrou) {
        cout << "Nenhum quarto disponível no momento." << endl;
    }
    
    arquivo.close();
}

// Verificar se quarto existe
bool quartoExiste(int numero) {
    Quarto* quarto = buscarQuartoPorNumero(numero);
    if (quarto != nullptr) {
        delete quarto;
        return true;
    }
    return false;
}

// Alterar status do quarto
bool alterarStatusQuarto(int numero, StatusQuarto novoStatus) {
    ifstream arquivoLeitura(ARQUIVO_QUARTOS, ios::binary);
    if (!arquivoLeitura.is_open()) {
        return false;
    }
    
    vector<Quarto> quartos;
    Quarto quarto;
    bool encontrou = false;
    
    // Ler todos os quartos
    while (!arquivoLeitura.eof()) {
        try {
            quarto.carregarDeArquivo(arquivoLeitura);
            if (quarto.numero == numero && quarto.ativo) {
                quarto.status = novoStatus;
                encontrou = true;
            }
            quartos.push_back(quarto);
        } catch (...) {
            break;
        }
    }
    arquivoLeitura.close();
    
    if (!encontrou) {
        return false;
    }
    
    // Reescrever arquivo
    ofstream arquivoEscrita(ARQUIVO_QUARTOS, ios::binary | ios::trunc);
    if (!arquivoEscrita.is_open()) {
        return false;
    }
    
    for (const auto& q : quartos) {
        q.salvarEmArquivo(arquivoEscrita);
    }
    arquivoEscrita.close();
    
    return true;
}

// Buscar quarto disponível (será usado pela função de cadastrar estadia)
Quarto* buscarQuartoDisponivel(int qtdHospedes, const Data& entrada, const Data& saida) {
    // Esta função será implementada após Estadia estar pronta
    // pois precisa verificar conflitos de datas
    return nullptr;
}