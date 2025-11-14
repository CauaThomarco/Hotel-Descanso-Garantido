# Makefile para Hotel Descanso Garantido
# (Adaptado para Windows e para compilar os 3 executáveis)

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++11 -Wall -Wextra -I. -g

# --- ARQUIVOS ---

# Diretório de implementações
SRC_DIR = Implementacao

# Executável principal
TARGET = hotel.exe

# Executável do gerador de testes
GENERATOR_TARGET = gerador_teste.exe

# --- NOVO ---
# Executável do teste de resistência
RESISTENCIA_TARGET = teste_resistencia.exe

# Objetos dos Módulos (compartilhados por todos os executáveis)
MODULE_OBJS = $(SRC_DIR)/Utils.o \
              $(SRC_DIR)/Cliente.o \
              $(SRC_DIR)/Funcionario.o \
              $(SRC_DIR)/Quarto.o \
              $(SRC_DIR)/Estadia.o

# Objetos do programa principal
MAIN_OBJS = main.o $(MODULE_OBJS)

# Objetos do gerador de testes
GENERATOR_OBJS = gerador_dados_teste.o $(MODULE_OBJS)

# --- NOVO ---
# Objetos do teste de resistência
RESISTENCIA_OBJS = teste_resistencia.o $(MODULE_OBJS)


# --- REGRAS DE COMPILAÇÃO ---

# Regra principal (padrão): Compila apenas o hotel
all: $(TARGET)

# Regra para compilar o executável principal
$(TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_OBJS)
	@echo "Compilacao principal concluida! Execute com: make run"

# Regra para compilar o gerador de testes
gerador: $(GENERATOR_TARGET)

$(GENERATOR_TARGET): $(GENERATOR_OBJS)
	$(CXX) $(CXXFLAGS) -o $(GENERATOR_TARGET) $(GENERATOR_OBJS)
	@echo "Compilacao do gerador concluida! Execute com: make run-test"

# --- NOVO ---
# Regra para compilar o teste de resistência
resistencia: $(RESISTENCIA_TARGET)

$(RESISTENCIA_TARGET): $(RESISTENCIA_OBJS)
	$(CXX) $(CXXFLAGS) -o $(RESISTENCIA_TARGET) $(RESISTENCIA_OBJS)
	@echo "Compilacao do teste de resistencia concluida! Execute com: make run-resistencia"


# Regras genéricas para compilar arquivos .cpp em .o
# Compila .cpp na raiz (main.cpp, gerador_dados_teste.cpp, teste_resistencia.cpp)
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compila .cpp no diretório Implementacao/
$(SRC_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- REGRAS DE EXECUÇÃO E LIMPEZA (Para Windows) ---

# Limpar arquivos compilados e dados
clean:
	# --- ATUALIZADO ---
	-del $(TARGET) $(GENERATOR_TARGET) $(RESISTENCIA_TARGET) .o *.dat $(SRC_DIR)\.o 2> nul
	@echo "Arquivos limpos!"

# Limpar apenas os arquivos de dados
clean-data:
	-del *.dat 2> nul
	@echo "Arquivos de dados removidos!"

# Executar o programa principal
run: $(TARGET)
	@.\\$(TARGET)

# Executar o gerador de testes
run-test: $(GENERATOR_TARGET)
	@.\\$(GENERATOR_TARGET)

# --- NOVO ---
# Executar o teste de resistência
run-resistencia: $(RESISTENCIA_TARGET)
	@.\\$(RESISTENCIA_TARGET)

# Recompilar tudo do zero
rebuild: clean all

# --- AJUDA ---

# Help
help:
	@echo "Comandos disponiveis:"
	@echo "  make               - Compila o sistema principal (hotel.exe)"
	@echo "  make run             - Compila e executa o sistema principal"
	@echo ""
	@echo "  make gerador         - Compila o gerador de testes (gerador_teste.exe)"
	@echo "  make run-test        - Compila e executa o gerador de testes"
	@echo ""
	@echo "  make resistencia     - Compila o teste de resistencia (teste_resistencia.exe)"
	@echo "  make run-resistencia - Compila e executa o teste de resistencia"
	@echo ""
	@echo "  make clean           - Remove todos os arquivos compilados (.o, .exe) e de dados (.dat)"
	@echo "  make clean-data      - Remove apenas os arquivos de dados (.dat)"
	@echo "  make rebuild         - Limpa e recompila o sistema principal"

# --- ATUALIZADO ---
.PHONY: all clean clean-data run run-test rebuild gerador resistencia run-resistencia help