#ifndef RECURSIVE_UTILS_H
#define RECURSIVE_UTILS_H

#include "persistence.h"
#include <stddef.h>

int contarVitoriasRecursivo(const GameResult *results, size_t count);
int somarTentativasRecursivo(const GameResult *results, size_t count);
int minTentativasRecursivo(const GameResult *results, size_t count);
int maxTentativasRecursivo(const GameResult *results, size_t count);
double somarDiferencasQuadradasRecursivo(const GameResult *results, size_t count, double average);

int somarPalpitesBaixosRecursivo(const GameResult *results, size_t count);
int somarPalpitesAltosRecursivo(const GameResult *results, size_t count);

bool sequenciaCrescenteRecursivo(const int *array, size_t size);
bool sequenciaDecrescenteRecursivo(const int *array, size_t size);
bool sequenciaMonotonaRecursivo(const int *array, size_t size);
int contarPartidasMonotonasRecursivo(const GameResult *results, size_t count);

bool contemValorRecursivo(const int *array, size_t size, int value);
bool temPalpitesRepetidosRecursivo(const int *array, size_t size);
int contarPartidasComRepeticaoRecursivo(const GameResult *results, size_t count);

#endif
