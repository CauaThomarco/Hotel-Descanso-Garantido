#ifndef UTILS_H
#define UTILS_H

#include "Estadia.h"
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <iomanip>

using namespace std;

void limparBuffer();
void pausar();
int calcularDiarias(const Data& entrada, const Data& saida);
Data lerData(const string& mensagem);
bool periodosSesobrepoe(const Data& e1, const Data& s1, const Data& e2, const Data& s2);

bool periodosSeSobrepoe(const Data& s1, const Data& e1, const Data& s2, const Data& e2);

#endif