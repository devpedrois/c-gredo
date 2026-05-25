#include "recursive_utils.h"

#include <stdlib.h>
#include <string.h>

static int obterPalpitesVetor(const char *sequenceStr, int *guesses) {
    int count = 0;
    char temp[guessSequenceLength];
    strncpy(temp, sequenceStr, sizeof(temp));
    temp[sizeof(temp) - 1] = '\0';
    
    char *token = strtok(temp, ",");
    while (token != NULL && count < maxAttempts) {
        guesses[count++] = atoi(token);
        token = strtok(NULL, ",");
    }
    return count;
}

int contarVitoriasRecursivo(const GameResult *results, size_t count) {
    if (results == NULL || count == 0) {
        return 0;
    }
    return (results[0].didWin ? 1 : 0) + contarVitoriasRecursivo(results + 1, count - 1);
}

int somarTentativasRecursivo(const GameResult *results, size_t count) {
    if (results == NULL || count == 0) {
        return 0;
    }
    return results[0].attemptsUsed + somarTentativasRecursivo(results + 1, count - 1);
}

int minTentativasRecursivo(const GameResult *results, size_t count) {
    int restMinimum;

    if (results == NULL || count == 0) {
        return 0;
    }

    if (count == 1) {
        return results[0].attemptsUsed;
    }

    restMinimum = minTentativasRecursivo(results + 1, count - 1);
    return results[0].attemptsUsed < restMinimum ? results[0].attemptsUsed : restMinimum;
}

int maxTentativasRecursivo(const GameResult *results, size_t count) {
    int restMaximum;

    if (results == NULL || count == 0) {
        return 0;
    }

    if (count == 1) {
        return results[0].attemptsUsed;
    }

    restMaximum = maxTentativasRecursivo(results + 1, count - 1);
    return results[0].attemptsUsed > restMaximum ? results[0].attemptsUsed : restMaximum;
}

double somarDiferencasQuadradasRecursivo(const GameResult *results, size_t count, double average) {
    double difference;

    if (results == NULL || count == 0) {
        return 0.0;
    }

    difference = (double) results[0].attemptsUsed - average;
    return (difference * difference) + somarDiferencasQuadradasRecursivo(results + 1, count - 1, average);
}

int somarPalpitesBaixosRecursivo(const GameResult *results, size_t count) {
    if (results == NULL || count == 0) {
        return 0;
    }
    return results[0].lowBiasCount + somarPalpitesBaixosRecursivo(results + 1, count - 1);
}

int somarPalpitesAltosRecursivo(const GameResult *results, size_t count) {
    if (results == NULL || count == 0) {
        return 0;
    }
    return results[0].highBiasCount + somarPalpitesAltosRecursivo(results + 1, count - 1);
}

bool sequenciaCrescenteRecursivo(const int *array, size_t size) {
    if (size <= 1) return true;
    return (array[0] <= array[1]) && sequenciaCrescenteRecursivo(array + 1, size - 1);
}

bool sequenciaDecrescenteRecursivo(const int *array, size_t size) {
    if (size <= 1) return true;
    return (array[0] >= array[1]) && sequenciaDecrescenteRecursivo(array + 1, size - 1);
}

bool sequenciaMonotonaRecursivo(const int *array, size_t size) {
    if (size <= 1) return true;
    return sequenciaCrescenteRecursivo(array, size) || sequenciaDecrescenteRecursivo(array, size);
}

int contarPartidasMonotonasRecursivo(const GameResult *results, size_t count) {
    if (results == NULL || count == 0) return 0;
    int guesses[maxAttempts];
    int numGuesses = obterPalpitesVetor(results[0].guessSequence, guesses);
    bool mon = (numGuesses > 1) && sequenciaMonotonaRecursivo(guesses, numGuesses);
    return (mon ? 1 : 0) + contarPartidasMonotonasRecursivo(results + 1, count - 1);
}

bool contemValorRecursivo(const int *array, size_t size, int value) {
    if (size == 0) return false;
    if (array[0] == value) return true;
    return contemValorRecursivo(array + 1, size - 1, value);
}

bool temPalpitesRepetidosRecursivo(const int *array, size_t size) {
    if (size <= 1) return false;
    if (contemValorRecursivo(array + 1, size - 1, array[0])) return true;
    return temPalpitesRepetidosRecursivo(array + 1, size - 1);
}

int contarPartidasComRepeticaoRecursivo(const GameResult *results, size_t count) {
    if (results == NULL || count == 0) return 0;
    int guesses[maxAttempts];
    int numGuesses = obterPalpitesVetor(results[0].guessSequence, guesses);
    bool rep = (numGuesses > 1) && temPalpitesRepetidosRecursivo(guesses, numGuesses);
    return (rep ? 1 : 0) + contarPartidasComRepeticaoRecursivo(results + 1, count - 1);
}
