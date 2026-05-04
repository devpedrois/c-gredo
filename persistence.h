#ifndef PERSISTENCE_H
#define PERSISTENCE_H

#include "game_logic.h"

#include <stdbool.h>
#include <stddef.h>

enum {
    playerNameMaxLength = 32,
    playerNameLength = playerNameMaxLength + 1,
    historyTimestampLength = 20,
    historyLineLength = 512,
    initialHistoryCapacity = 16
};

extern const char rankingFilePath[];

typedef struct {
    int id;
    char playerName[playerNameLength];
    int didWin;
    int targetNumber;
    int attemptsUsed;
    int durationSeconds;
    int lowBiasCount;
    int highBiasCount;
    char guessSequence[guessSequenceLength];
    char timestamp[historyTimestampLength];
} GameResult;

typedef struct {
    GameResult *results;
    size_t count;
    size_t capacity;
} GameHistory;

void inicializarHistorico(GameHistory *gameHistory);

void liberarHistorico(GameHistory *gameHistory);

bool adicionarResultadoAoHistorico(GameHistory *gameHistory, GameResult gameResult);

bool carregarHistorico(GameHistory *gameHistory, const char *filePath);

bool salvarResultadoNoArquivo(const GameResult *gameResult, const char *filePath);

bool nomeJaRegistrado(const char *name);

GameResult criarResultadoDaSessao(const GameHistory *gameHistory, const GameSession *gameSession, const char *playerName);

#endif
