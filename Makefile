# Makefile para Hotel Descanso Garantido

# Compilador
CXX = g++

# Flags de compilação
CXXFLAGS = -std=c++11 -Wall -I.

# Diretórios
SRC_DIR = Implementação
HEADER_DIR = Headers

# Arquivos fonte
SOURCES = main.cpp \
          $(SRC_DIR)/Utils.cpp \
          $(SRC_DIR)/Cliente.cpp \
          $(SRC_DIR)/Funcionario.cpp \
          $(SRC_DIR)/Quarto.cpp \
          $(SRC_DIR)/Estadia.cpp

# Arquivos objeto
OBJECTS = $(SOURCES:.cpp=.o)

# Nome do executável
TARGET = hotel

# Regra principal
all: $(TARGET)

# Compilar o executável
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Compilação concluída! Execute com: ./$(TARGET)"

# Compilar arquivos .cpp em .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpar arquivos compilados
clean:
	rm -f $(OBJECTS) $(TARGET)
	rm -f *.dat
	@echo "Arquivos limpos!"

# Limpar apenas os arquivos de dados
clean-data:
	rm -f *.dat
	@echo "Arquivos de dados removidos!"

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Recompilar tudo do zero
rebuild: clean all

# Compilar com informações de debug
debug: CXXFLAGS += -g -DDEBUG
debug: rebuild

# Verificar dependências
depend:
	@echo "Arquivos fonte encontrados:"
	@echo $(SOURCES)

# Help
help:
	@echo "Comandos disponíveis:"
	@echo "  make          - Compila o projeto"
	@echo "  make run      - Compila e executa"
	@echo "  make clean    - Remove arquivos compilados e dados"
	@echo "  make clean-data - Remove apenas arquivos de dados"
	@echo "  make rebuild  - Recompila tudo do zero"
	@echo "  make debug    - Compila com símbolos de debug"
	@echo "  make help     - Mostra esta mensagem"

.PHONY: all clean clean-data run rebuild debug depend help