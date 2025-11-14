# Makefile para Hotel Descanso Garantido
# Compatível com Windows e Linux

# Detectar o sistema operacional
ifeq ($(OS),Windows_NT)
    SISTEMA = Windows
    RM = del /Q /F
    RMDIR = rmdir /S /Q
    SEP = \\
    EXE = .exe
    NULL = nul
    MKDIR = mkdir
else
    SISTEMA = Linux
    RM = rm -f
    RMDIR = rm -rf
    SEP = /
    EXE = 
    NULL = /dev/null
    MKDIR = mkdir -p
endif

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++11 -Wall -Wextra -I. -g

# Diretório de implementações
SRC_DIR = Implementacao

# Executáveis
TARGET = hotel$(EXE)
GENERATOR_TARGET = gerador_teste$(EXE)
RESISTENCIA_TARGET = teste_resistencia$(EXE)

# Objetos dos Módulos (compartilhados)
MODULE_OBJS = $(SRC_DIR)$(SEP)Utils.o \
              $(SRC_DIR)$(SEP)Cliente.o \
              $(SRC_DIR)$(SEP)Funcionario.o \
              $(SRC_DIR)$(SEP)Quarto.o \
              $(SRC_DIR)$(SEP)Estadia.o

# Objetos do programa principal
MAIN_OBJS = main.o $(MODULE_OBJS)

# Objetos do gerador de testes
GENERATOR_OBJS = gerador_dados_teste.o $(MODULE_OBJS)

# Objetos do teste de resistência
RESISTENCIA_OBJS = teste_resistencia.o $(MODULE_OBJS)

# Regra principal
all: $(TARGET)

# Compilar executável principal
$(TARGET): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(MAIN_OBJS)
	@echo "Compilacao principal concluida! Execute com: make run"

# Compilar gerador de testes
gerador: $(GENERATOR_TARGET)

$(GENERATOR_TARGET): $(GENERATOR_OBJS)
	$(CXX) $(CXXFLAGS) -o $(GENERATOR_TARGET) $(GENERATOR_OBJS)
	@echo "Compilacao do gerador concluida! Execute com: make run-test"

# Compilar teste de resistência
resistencia: $(RESISTENCIA_TARGET)

$(RESISTENCIA_TARGET): $(RESISTENCIA_OBJS)
	$(CXX) $(CXXFLAGS) -o $(RESISTENCIA_TARGET) $(RESISTENCIA_OBJS)
	@echo "Compilacao do teste de resistencia concluida! Execute com: make run-resistencia"

# Regras de compilação
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(SRC_DIR)$(SEP)%.o: $(SRC_DIR)$(SEP)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos - Versão multiplataforma
clean:
ifeq ($(SISTEMA),Windows)
	@echo Limpando arquivos no Windows...
	-@$(RM) *.o 2> $(NULL) || true
	-@$(RM) *.exe 2> $(NULL) || true
	-@$(RM) *.bin 2> $(NULL) || true
	-@$(RM) $(SRC_DIR)$(SEP)*.o 2> $(NULL) || true
else
	@echo Limpando arquivos no Linux...
	-@$(RM) *.o
	-@$(RM) *.exe
	-@$(RM) hotel
	-@$(RM) gerador_teste
	-@$(RM) teste_resistencia
	-@$(RM) *.bin
	-@$(RM) $(SRC_DIR)/*.o
endif
	@echo Arquivos limpos!

# Limpar apenas dados
clean-data:
ifeq ($(SISTEMA),Windows)
	-@$(RM) *.bin 2> $(NULL) || true
else
	-@$(RM) *.bin
endif
	@echo Arquivos de dados removidos!

# Executar programa principal
run: $(TARGET)
ifeq ($(SISTEMA),Windows)
	@.$(SEP)$(TARGET)
else
	@./$(TARGET)
endif

# Executar gerador
run-test: $(GENERATOR_TARGET)
ifeq ($(SISTEMA),Windows)
	@.$(SEP)$(GENERATOR_TARGET)
else
	@./$(GENERATOR_TARGET)
endif

# Executar teste de resistência
run-resistencia: $(RESISTENCIA_TARGET)
ifeq ($(SISTEMA),Windows)
	@.$(SEP)$(RESISTENCIA_TARGET)
else
	@./$(RESISTENCIA_TARGET)
endif

# Recompilar tudo
rebuild: clean all

# Ajuda
help:
	@echo "=========================================="
	@echo "   MAKEFILE - Hotel Descanso Garantido"
	@echo "=========================================="
	@echo "Sistema detectado: $(SISTEMA)"
	@echo ""
	@echo "Comandos disponiveis:"
	@echo "  make              - Compila o sistema principal"
	@echo "  make run          - Compila e executa o sistema"
	@echo "  make gerador      - Compila o gerador de testes"
	@echo "  make run-test     - Compila e executa o gerador"
	@echo "  make resistencia  - Compila o teste de resistencia"
	@echo "  make run-resistencia - Executa o teste de resistencia"
	@echo "  make clean        - Remove arquivos compilados e dados"
	@echo "  make clean-data   - Remove apenas arquivos de dados"
	@echo "  make rebuild      - Limpa e recompila tudo"
	@echo "  make help         - Mostra esta ajuda"

.PHONY: all clean clean-data run run-test run-resistencia rebuild gerador resistencia help