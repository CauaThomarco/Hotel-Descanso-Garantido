# CLAUDE.md - AI Assistant Guide for Hotel-Descanso-Garantido

**Last Updated:** 2025-11-17
**Project:** Hotel Descanso Garantido (Guaranteed Rest Hotel)
**Language:** C++11
**Primary Language (UI/Code):** Portuguese (PT-BR)

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Codebase Structure](#codebase-structure)
3. [Technology Stack](#technology-stack)
4. [Development Workflow](#development-workflow)
5. [Build System](#build-system)
6. [Data Models & Schemas](#data-models--schemas)
7. [Key Algorithms](#key-algorithms)
8. [Testing Strategy](#testing-strategy)
9. [Coding Conventions](#coding-conventions)
10. [Common Tasks](#common-tasks)
11. [Important Notes for AI Assistants](#important-notes-for-ai-assistants)

---

## Project Overview

### Purpose
A comprehensive hotel management system designed to digitize and optimize hotel operations including client management, employee management, room management, and reservation/stay management with an integrated loyalty program.

### Key Features
- **Client Management**: Registration, search (by code/name), listing
- **Employee Management**: Full employment data tracking with position/salary management
- **Room Management**: Multi-category rooms (Simple/Double/Suite) with dynamic pricing
- **Reservation System**: Full lifecycle management with conflict detection
- **Loyalty Program**: Automatic point calculation (10 points per night)
- **Data Validation**: Comprehensive checks for dates, overlaps, and data integrity
- **Binary Persistence**: Direct struct serialization to .bin files

### Statistics
- **Total Lines of Code:** ~1,195 lines
- **Header Files:** 5 files (210 lines)
- **Implementation Files:** 5 files (985 lines)
- **Executables:** 3 programs (hotel, gerador_teste, teste_resistencia)

---

## Codebase Structure

```
Hotel-Descanso-Garantido/
├── Headers/                          # Class definitions
│   ├── Clientes.h                   # Client class + functions
│   ├── Funcionarios.h               # Employee class + functions
│   ├── Quarto.h                     # Room class + enums + functions
│   ├── Estadia.h                    # Stay class + Date struct + enums
│   └── Utils.h                      # Utility functions for dates/input
│
├── Implementacao/                    # Implementation files
│   ├── Cliente.cpp                  # Client implementation (181 lines)
│   ├── Funcionario.cpp              # Employee implementation (176 lines)
│   ├── Quarto.cpp                   # Room implementation (218 lines)
│   ├── Estadia.cpp                  # Stay implementation (270 lines)
│   └── Utils.cpp                    # Utility functions (140 lines)
│
├── main.cpp                         # Main application (360 lines, 14 menu options)
├── teste_resistencia.cpp            # Stress/resistance tests (221 lines)
├── gerador_dados_teste.cpp          # Test data generator (293 lines)
│
├── Makefile                         # Cross-platform build configuration
├── README.md                        # Project description (Portuguese)
├── LICENSE                          # MIT License
├── .gitignore                       # Git ignore patterns
└── CLAUDE.md                        # This file
```

### Data Files (Generated at Runtime)
- `clientes.bin` - Client records
- `funcionarios.bin` - Employee records
- `quartos.bin` - Room records
- `estadias.bin` - Stay/reservation records

---

## Technology Stack

### Core Technologies
- **Language:** C++11
- **Compiler:** g++ (GCC)
- **Build System:** GNU Make (cross-platform: Linux/Windows)
- **Data Persistence:** Binary file format (.bin) with direct struct serialization

### Standard Libraries Used
- `<iostream>` - I/O operations
- `<fstream>` - Binary file I/O
- `<string>` - String manipulation
- `<vector>` - Dynamic arrays for collections
- `<cstring>` - C-style string operations
- `<iomanip>` - Output formatting
- `<ctime>` - Time operations
- `<limits>` - Numeric limits
- `<windows.h>` - Windows UTF-8 console support (platform-specific)

### Development Tools
- Git version control
- Cross-platform Makefile
- Custom test framework

---

## Development Workflow

### Initial Setup
```bash
# Clone and enter repository
cd Hotel-Descanso-Garantido

# Generate sample test data (recommended for first-time setup)
make run-test

# Run the main application
make run
```

### Standard Workflow
```bash
# 1. Make changes to source files
# 2. Compile
make

# 3. Run application
make run

# 4. Test changes
make run-resistencia

# 5. Clean if needed
make clean-data  # Reset data only
make clean       # Full clean
```

### Git Branch Strategy
- Main branch: `main` (not specified in status)
- Feature branches: Use `claude/` prefix for AI-assisted development
- Current branch: `claude/claude-md-mi3p9x5duyfma3w1-016NgETfNauXN7zWmXhWgs4g`

---

## Build System

### Makefile Commands

| Command | Purpose | Output |
|---------|---------|--------|
| `make` or `make all` | Compile main application | `hotel` or `hotel.exe` |
| `make run` | Compile and execute main app | Interactive menu |
| `make gerador` | Compile test data generator | `gerador_teste` |
| `make run-test` | Generate sample data | Populates .bin files |
| `make resistencia` | Compile resistance tests | `teste_resistencia` |
| `make run-resistencia` | Run stress/validation tests | Test results |
| `make clean` | Remove all build artifacts + data | Clean slate |
| `make clean-data` | Remove only .bin data files | Reset data |
| `make rebuild` | Clean + recompile everything | Fresh build |
| `make help` | Display available commands | Help text |

### Compilation Flags
```makefile
CXXFLAGS = -std=c++11 -Wall -Wextra -I. -g
```
- `-std=c++11`: C++11 standard
- `-Wall -Wextra`: All warnings enabled
- `-I.`: Include current directory
- `-g`: Debug symbols included

### Cross-Platform Support
The Makefile automatically detects Windows vs Linux and adjusts:
- File separators (`\` vs `/`)
- Delete commands (`del` vs `rm`)
- Executable extensions (`.exe` vs none)
- NULL device (`nul` vs `/dev/null`)

---

## Data Models & Schemas

### 1. Cliente (Client) - Headers/Clientes.h

```cpp
class Cliente {
    int codigo;                    // Unique identifier (auto-generated)
    char nome[100];               // Name (max 99 chars)
    char endereco[200];           // Address (max 199 chars)
    char telefone[20];            // Phone (max 19 chars)
    bool ativo;                   // Active/inactive flag (soft delete)
}
```

**File:** `clientes.bin` (binary)

**Key Functions:**
- `gerarCodigoCliente()` - Auto-generate sequential ID
- `cadastrarCliente()` - Register new client (validates uniqueness)
- `buscarClientePorCodigo(int)` - Find by ID
- `buscarClientesPorNome(string)` - Search by partial name match
- `listarClientes()` - Display all active clients
- `clienteExiste(int)` - Validate client existence

---

### 2. Funcionario (Employee) - Headers/Funcionarios.h

```cpp
class Funcionario {
    int codigo;                    // Unique identifier (auto-generated)
    char nome[100];               // Name
    char telefone[20];            // Phone
    char cargo[50];               // Position/role
    double salario;               // Salary (R$)
    bool ativo;                   // Active flag
}
```

**File:** `funcionarios.bin`

**Common Positions (cargo):**
- Recepcionista (Receptionist)
- Gerente (Manager)
- Camareira (Cleaning Staff)
- Garçom (Waiter)

---

### 3. Quarto (Room) - Headers/Quarto.h

```cpp
enum StatusQuarto {
    DESOCUPADO,                   // Vacant (0)
    OCUPADO                       // Occupied (1)
}

class Quarto {
    int numero;                   // Room number (unique, e.g., 101, 201, 301)
    int maxHospedes;              // Maximum guest capacity
    double valorDiaria;           // Daily rate in R$ (Brazilian Real)
    StatusQuarto status;          // Current occupancy status
    bool ativo;                   // Active flag
}
```

**File:** `quartos.bin`

**Room Categories:**
| Type | Numbers | Capacity | Rate/Night |
|------|---------|----------|------------|
| Simple | 101-105 | 2 guests | R$ 150,00 |
| Double | 201-203 | 4 guests | R$ 250,00 |
| Suite | 301-302 | 6 guests | R$ 400,00 |

**Key Functions:**
- `buscarQuartoDisponivel(Data, Data, int)` - Find available room matching dates and capacity
- `verificarDisponibilidadeQuarto(int, Data, Data)` - Check if specific room is available
- `alterarStatusQuarto(int, StatusQuarto)` - Update room status
- `listarQuartosDisponiveis()` - List all vacant rooms

---

### 4. Data (Date) - Headers/Estadia.h

```cpp
struct Data {
    int dia;                      // Day (1-31)
    int mes;                      // Month (1-12)
    int ano;                      // Year (1900-2100)
    int hora;                     // Hour (12 or 14 only)
}
```

**Validation Rules:**
- Check-in time: **14:00** (2 PM)
- Check-out time: **12:00** (12 PM)
- Leap year support: Automatic detection
- Date range validation: 1900-2100

**Member Functions:**
- `validar()` - Validate date and time
- `exibir()` - Display formatted date
- `compararCom(Data)` - Compare dates
- Operators: `<`, `>`, `==`, `<=`, `>=`

---

### 5. Estadia (Stay/Reservation) - Headers/Estadia.h

```cpp
enum StatusEstadia {
    ATIVA,                        // Active stay (0)
    FINALIZADA                    // Finished stay (1)
}

class Estadia {
    int codigoEstadia;            // Unique stay ID (auto-generated)
    Data dataEntrada;             // Check-in date and time
    Data dataSaida;               // Check-out date and time
    int quantidadeDiarias;        // Number of nights
    int codigoCliente;            // Foreign key to Cliente
    int numeroQuarto;             // Foreign key to Quarto
    StatusEstadia status;         // Current status
    bool ativa;                   // Active flag
}
```

**File:** `estadias.bin`

**Business Rules:**
- Minimum stay: 1 night
- Check-in: 14:00 only
- Check-out: 12:00 only
- No overlapping reservations per room
- Client must exist before creating stay
- Room capacity must accommodate guest count

**Key Functions:**
- `cadastrarEstadia()` - Create new reservation (complex validation)
- `darBaixaEstadia(int)` - Finalize stay and calculate total
- `calcularValorTotal()` - Compute total cost
- `calcularTotalDiariasCliente(int)` - Sum all nights for client
- `calcularPontosFidelidade(int)` - Calculate loyalty points (nights × 10)
- `listarEstadiaCliente(int)` - Retrieve client's stay history

---

## Key Algorithms

### 1. Overlap Detection - `periodosSeSobrepoe()` (Utils.cpp)

**Purpose:** Prevent double-booking by detecting date range overlaps

**Algorithm:**
```cpp
bool periodosSeSobrepoe(Data inicio1, Data fim1, Data inicio2, Data fim2) {
    // Returns true if periods overlap
    // Used in: verificarDisponibilidadeQuarto()
}
```

**Logic:**
- Two periods overlap if: `inicio1 <= fim2 AND fim1 >= inicio2`
- Critical for reservation validation
- Called before every new stay registration

---

### 2. Date Calculation - `calcularDiarias()` (Utils.cpp)

**Purpose:** Calculate number of nights between check-in and check-out

**Considerations:**
- Accounts for leap years
- Handles month boundaries
- Validates date order (check-out must be after check-in)

---

### 3. Room Assignment - `buscarQuartoDisponivel()` (Quarto.cpp)

**Purpose:** Find available room matching requirements

**Parameters:**
- Check-in date
- Check-out date
- Number of guests

**Process:**
1. Filter rooms by capacity (maxHospedes >= guest count)
2. Check room status (DESOCUPADO)
3. Verify no overlapping stays using `verificarDisponibilidadeQuarto()`
4. Return first available room or -1 if none found

---

### 4. Auto-ID Generation

**Pattern Used Across All Entities:**
```cpp
int gerarCodigoCliente() {
    // 1. Load all existing records
    // 2. Find maximum ID
    // 3. Return max + 1 (or 1 if no records)
}
```

**Applied to:**
- `gerarCodigoCliente()` - Client codes
- `gerarCodigoFuncionario()` - Employee codes
- `gerarCodigoEstadia()` - Stay codes

**Note:** Room numbers are manually assigned (not auto-generated)

---

## Testing Strategy

### Test Data Generator - `gerador_dados_teste.cpp`

**Purpose:** Create realistic sample data for development/testing

**Generated Data:**
- **10 Clients** - Realistic Brazilian names, addresses, phones
- **5 Employees** - Various positions (receptionist, manager, cleaning staff, waiter)
- **10 Rooms** - All three categories (Simple, Double, Suite)
- **Multiple Stays** - Historical, active, and future reservations

**Usage:**
```bash
make run-test
```

**Output:**
- Statistics display with Unicode symbols (✓, ✅, 📊, 👥, 🏨, 📅)
- Room occupancy breakdown
- Stay status breakdown

---

### Resistance/Stress Tests - `teste_resistencia.cpp`

**Purpose:** Validate data integrity and business logic

**Test Framework:**
- Custom `checar()` function for assertions
- Pass/Fail reporting with `[PASS]`/`[FAIL]` markers
- Exception-based failure handling
- Final summary report

**5 Test Cases:**

1. **Duplicity Prevention (Quarto)**
   - Ensures duplicate room numbers are rejected
   - Tests: `cadastrarQuarto()` validation

2. **Data Integrity (Estadia without Cliente)**
   - Validates client existence before creating stay
   - Tests: Foreign key validation

3. **Invalid Dates Detection**
   - Checks rejection of invalid date ranges
   - Tests: `Data::validar()` and business logic

4. **Estadia Overlap Detection**
   - Ensures rooms aren't double-booked
   - Tests: `periodosSeSobrepoe()` algorithm

5. **Valid Period Acceptance**
   - Confirms available periods are accepted
   - Tests: Normal flow without conflicts

**Usage:**
```bash
make run-resistencia
```

**Warning:** Destroys and recreates all .bin files - manual confirmation required

---

## Coding Conventions

### Naming Conventions

| Element | Convention | Examples |
|---------|-----------|----------|
| Classes | PascalCase | `Cliente`, `Funcionario`, `Quarto`, `Estadia` |
| Functions | camelCase | `gerarCodigoCliente()`, `buscarClientePorCodigo()` |
| Member Variables | camelCase | `codigoCliente`, `dataEntrada`, `statusQuarto` |
| Constants | SCREAMING_SNAKE_CASE | `ARQUIVO_CLIENTES`, `ARQUIVO_ESTADIAS` |
| Enums | PascalCase | `StatusQuarto`, `StatusEstadia` |
| Enum Values | SCREAMING_SNAKE_CASE | `DESOCUPADO`, `OCUPADO`, `ATIVA`, `FINALIZADA` |

---

### Language Usage

**Code & UI:** Portuguese (PT-BR)
- Variable names: Portuguese (`codigoCliente`, `dataEntrada`)
- Function names: Portuguese (`cadastrarCliente`, `buscarQuarto`)
- User messages: Portuguese ("Cliente cadastrado com sucesso!")
- Comments: Portuguese

**Rationale:** Educational project in Brazilian Portuguese-speaking environment

---

### File Organization

**Header/Implementation Separation:**
- Headers in `Headers/` directory
- Implementations in `Implementacao/` directory
- One main class per header file
- Corresponding .cpp file for each .h file

**Include Pattern:**
```cpp
// In implementation files:
#include "Headers/Clientes.h"
#include "Headers/Utils.h"
```

---

### Memory Management

**String Storage:**
- Fixed-size `char` arrays (not `std::string`) in data classes
- Reason: Binary file compatibility and direct serialization
- Example: `char nome[100]`, `char endereco[200]`

**Collections:**
- `std::vector<>` for dynamic collections
- Example: `vector<Cliente> clientes`

**No Pointers:**
- Safe C++ style without manual memory management
- Stack-based objects
- RAII principles

---

### Error Handling

**Pattern:**
- Boolean return values for success/failure
- Descriptive error messages to `cout`
- Input validation before processing
- Graceful handling of missing files

**Example:**
```cpp
bool cadastrarCliente(...) {
    if (/* validation fails */) {
        cout << "Erro: descrição do erro" << endl;
        return false;
    }
    // Process...
    return true;
}
```

---

### Output Formatting

**Standards:**
- Monetary values: 2 decimal places (`setprecision(2)`)
- Aligned console output with separator lines
- Date formatting: DD/MM/YYYY HH:00
- UTF-8 support for Portuguese characters (á, ç, ã, etc.)

**Windows-Specific:**
```cpp
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // Enable UTF-8 in console
#endif
```

---

## Common Tasks

### Adding a New Client
```cpp
// Via menu option 1 in main.cpp
menuCadastrarCliente();
// OR programmatically:
Cliente c(nome, endereco, telefone);
if (cadastrarCliente(c)) {
    cout << "Success!" << endl;
}
```

---

### Creating a Reservation
```cpp
// Via menu option 4 in main.cpp
menuCadastrarEstadia();
// Internally:
// 1. Validate client exists (clienteExiste())
// 2. Read dates (lerData())
// 3. Find available room (buscarQuartoDisponivel())
// 4. Check overlaps (verificarDisponibilidadeQuarto())
// 5. Create Estadia and save
```

---

### Searching for Clients
```cpp
// By code:
Cliente* c = buscarClientePorCodigo(codigo);
if (c != nullptr) { c->exibir(); }

// By name (partial match):
vector<Cliente> clientes = buscarClientesPorNome("Silva");
for (auto& c : clientes) { c.exibir(); }
```

---

### Checking Room Availability
```cpp
Data checkin, checkout;
int numeroQuarto = 101;

if (verificarDisponibilidadeQuarto(numeroQuarto, checkin, checkout)) {
    cout << "Quarto disponível!" << endl;
} else {
    cout << "Quarto ocupado no período." << endl;
}
```

---

### Calculating Loyalty Points
```cpp
int codigoCliente = 1;
int pontos = calcularPontosFidelidade(codigoCliente);
// Formula: total nights × 10 points
```

---

## Important Notes for AI Assistants

### 1. **Language Awareness**
- All code, comments, and user-facing text are in **Portuguese (PT-BR)**
- When adding features, maintain Portuguese naming and messages
- Example: Use `cadastrar` (register), not `register`

---

### 2. **Data Persistence**
- Uses **binary file format**, not text/CSV/JSON
- Serialization via `reinterpret_cast<char*>(&object)`
- Fixed-size char arrays required (not `std::string`) in persisted classes
- Always test file I/O after changes to class structures

---

### 3. **Critical Algorithms**
- **Never modify** `periodosSeSobrepoe()` without thorough testing
- Date overlap detection is core business logic
- Room double-booking prevention depends on this function

---

### 4. **Testing Requirements**
- **Always run** `make run-resistencia` after modifying:
  - Date handling
  - Overlap detection
  - Validation logic
  - Data structures
- Regenerate test data with `make run-test` if .bin files corrupted

---

### 5. **Build System**
- Makefile is **cross-platform** (Windows/Linux)
- Don't add platform-specific commands without guards
- Test on both platforms if modifying Makefile

---

### 6. **Time Constraints**
- Only **12:00** (checkout) and **14:00** (checkin) are valid
- Don't allow arbitrary hours
- This is a business rule, not a technical limitation

---

### 7. **ID Generation**
- Auto-generated IDs are **sequential** based on existing records
- If deleting records (ativo = false), IDs are NOT reused
- Room numbers are **manually assigned** (100s, 200s, 300s series)

---

### 8. **Soft Deletes**
- All entities use `bool ativo` flag
- Never physically delete records
- Use `ativo = false` for "deletion"
- Search/list functions should filter by `ativo == true`

---

### 9. **Main Application Structure**
- `main.cpp` contains **14 menu options** (0-14)
- Each menu option calls a `menu*()` function
- Keep menu-driven flow consistent
- Always return to main menu after operations

---

### 10. **Git Workflow**
- Feature branches use `claude/` prefix
- Always develop on specified branch
- Commit messages should be clear (recent: Portuguese)
- Use `git push -u origin <branch-name>` for pushes

---

### 11. **Common Pitfalls to Avoid**

❌ **Don't:**
- Use `std::string` in persisted classes (Cliente, Funcionario, Quarto, Estadia)
- Allow overlapping reservations
- Skip validation checks
- Hardcode file paths
- Use hours other than 12 or 14
- Physically delete records (use `ativo = false`)

✅ **Do:**
- Use fixed-size char arrays for strings in data classes
- Always check `periodosSeSobrepoe()` before booking
- Validate dates with `Data::validar()`
- Use constants for file names
- Enforce check-in at 14:00, check-out at 12:00
- Implement soft deletes

---

### 12. **When Making Changes**

**Before Modifying:**
1. Read relevant header + implementation files
2. Understand dependencies (which functions call this?)
3. Check if test data exists (`make run-test`)
4. Review related tests in `teste_resistencia.cpp`

**After Modifying:**
1. Compile: `make clean && make`
2. Test manually: `make run`
3. Run resistance tests: `make run-resistencia`
4. Verify no data corruption
5. Test edge cases

**For New Features:**
1. Follow existing naming conventions (Portuguese)
2. Add validation logic
3. Update this CLAUDE.md if architecture changes
4. Consider adding test cases to `teste_resistencia.cpp`
5. Update README.md if user-facing

---

### 13. **File Structure Best Practices**

**Adding New Classes:**
```
1. Create header: Headers/NovaClasse.h
2. Create implementation: Implementacao/NovaClasse.cpp
3. Update Makefile MODULE_OBJS if shared
4. Include in relevant main programs
5. Document in CLAUDE.md
```

---

### 14. **Debugging Tips**

**Common Issues:**
- **Compilation errors:** Check include paths (`-I.`)
- **Segmentation faults:** Likely char array overflow
- **File not found:** Run from project root directory
- **UTF-8 display issues:** Ensure `SetConsoleOutputCP(CP_UTF8)` on Windows
- **Date validation fails:** Check hour is 12 or 14
- **Overlap detection fails:** Review `periodosSeSobrepoe()` logic

---

## Quick Reference

### Essential Files
- `main.cpp` - Main application entry point (360 lines)
- `Headers/Estadia.h` - Most complex business logic
- `Implementacao/Estadia.cpp` - Overlap detection, booking
- `Utils.cpp` - Date utilities and helper functions
- `Makefile` - Build configuration

### Essential Functions
- `periodosSeSobrepoe()` - Overlap detection (CRITICAL)
- `cadastrarEstadia()` - Create reservation (COMPLEX)
- `verificarDisponibilidadeQuarto()` - Check availability
- `buscarQuartoDisponivel()` - Find available room
- `calcularDiarias()` - Calculate nights
- `lerData()` - Date input with validation

### Essential Commands
```bash
make run-test           # Generate test data
make run                # Run application
make run-resistencia    # Run tests
make clean-data         # Reset data
make clean && make      # Full rebuild
```

---

## Version History

| Date | Version | Changes |
|------|---------|---------|
| 2025-11-17 | 1.0 | Initial CLAUDE.md creation based on comprehensive codebase analysis |

---

## License

This project is licensed under the MIT License.
Copyright © 2025 Cauã Thomarco

---

## Contact & Resources

- **Repository:** Hotel-Descanso-Garantido
- **Primary Developer:** Cauã Thomarco
- **Documentation:** This file (CLAUDE.md), README.md, Makefile help
- **Support:** Review code comments and test files for examples

---

*This document is maintained for AI assistants working with this codebase. Keep it updated when making architectural changes.*
