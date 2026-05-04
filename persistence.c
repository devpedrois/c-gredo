#include "persistence.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

const char rankingFilePath[] = "historico_partidas.csv";

static bool garantirCapacidadeHistorico(GameHistory *gameHistory) {
    GameResult *resizedResults;
    size_t newCapacity;

    if (gameHistory == NULL) {
        return false;
    }

    if (gameHistory->count < gameHistory->capacity) {
        return true;
    }

    newCapacity = gameHistory->capacity == 0 ? initialHistoryCapacity : gameHistory->capacity * 2;
    resizedResults = (GameResult *) realloc(gameHistory->results, newCapacity * sizeof(GameResult));
    if (resizedResults == NULL) {
        return false;
    }

    gameHistory->results = resizedResults;
    gameHistory->capacity = newCapacity;
    return true;
}

void inicializarHistorico(GameHistory *gameHistory) {
    if (gameHistory == NULL) {
        return;
    }

    gameHistory->results = NULL;
    gameHistory->count = 0;
    gameHistory->capacity = 0;
}

void liberarHistorico(GameHistory *gameHistory) {
    if (gameHistory == NULL) {
        return;
    }

    free(gameHistory->results);
    gameHistory->results = NULL;
    gameHistory->count = 0;
    gameHistory->capacity = 0;
}

bool adicionarResultadoAoHistorico(GameHistory *gameHistory, GameResult gameResult) {
    if (gameHistory == NULL || !garantirCapacidadeHistorico(gameHistory)) {
        return false;
    }

    gameHistory->results[gameHistory->count] = gameResult;
    gameHistory->count++;
    return true;
}

bool carregarHistorico(GameHistory *gameHistory, const char *filePath) {
    FILE *historyFile;
    char line[historyLineLength];

    if (gameHistory == NULL || filePath == NULL) {
        return false;
    }

    historyFile = fopen(filePath, "r");
    if (historyFile == NULL) {
        return errno == ENOENT;
    }

    while (fgets(line, sizeof(line), historyFile) != NULL) {
        GameResult loadedResult;
        char loadedPlayerName[playerNameLength];
        char loadedGuessSequence[guessSequenceLength];
        char loadedTimestamp[historyTimestampLength];
        int readFields;

        memset(&loadedResult, 0, sizeof(loadedResult));
        readFields = sscanf(
            line,
            "%d;%32[^;];%d;%d;%d;%d;%d;%d;%191[^;];%19[^\n]",
            &loadedResult.id,
            loadedPlayerName,
            &loadedResult.didWin,
            &loadedResult.targetNumber,
            &loadedResult.attemptsUsed,
            &loadedResult.durationSeconds,
            &loadedResult.lowBiasCount,
            &loadedResult.highBiasCount,
            loadedGuessSequence,
            loadedTimestamp
        );

        if (readFields == 10) {
            snprintf(loadedResult.playerName, sizeof(loadedResult.playerName), "%s", loadedPlayerName);
            snprintf(loadedResult.guessSequence, sizeof(loadedResult.guessSequence), "%s", loadedGuessSequence);
            snprintf(loadedResult.timestamp, sizeof(loadedResult.timestamp), "%s", loadedTimestamp);
            adicionarResultadoAoHistorico(gameHistory, loadedResult);
        }
    }

    return fclose(historyFile) != EOF;
}

bool salvarResultadoNoArquivo(const GameResult *gameResult, const char *filePath) {
    FILE *historyFile;
    int writeStatus;

    if (gameResult == NULL || filePath == NULL) {
        return false;
    }

    historyFile = fopen(filePath, "a");
    if (historyFile == NULL) {
        return false;
    }

    writeStatus = fprintf(
        historyFile,
        "%d;%s;%d;%d;%d;%d;%d;%d;%s;%s\n",
        gameResult->id,
        gameResult->playerName,
        gameResult->didWin,
        gameResult->targetNumber,
        gameResult->attemptsUsed,
        gameResult->durationSeconds,
        gameResult->lowBiasCount,
        gameResult->highBiasCount,
        gameResult->guessSequence,
        gameResult->timestamp
    );

    if (writeStatus < 0 || fclose(historyFile) == EOF) {
        return false;
    }

    return true;
}

bool nomeJaRegistrado(const char *name) {
    FILE *rankingFile;
    char line[historyLineLength];

    if (name == NULL || name[0] == '\0') {
        return false;
    }

    rankingFile = fopen(rankingFilePath, "r");
    if (rankingFile == NULL) {
        return false;
    }

    while (fgets(line, sizeof(line), rankingFile) != NULL) {
        char registeredName[playerNameLength];
        int registeredId;
        int readFields = sscanf(line, "%d;%32[^;]", &registeredId, registeredName);

        if (readFields == 2 && strcmp(registeredName, name) == 0) {
            fclose(rankingFile);
            return true;
        }
    }

    fclose(rankingFile);
    return false;
}

GameResult criarResultadoDaSessao(const GameHistory *gameHistory, const GameSession *gameSession, const char *playerName) {
    GameResult gameResult;
    time_t currentTime = time(NULL);
    struct tm *timeInfo = localtime(&currentTime);

    memset(&gameResult, 0, sizeof(gameResult));
    gameResult.id = (gameHistory != NULL && gameHistory->count > 0) ? gameHistory->results[gameHistory->count - 1].id + 1 : 1;
    snprintf(gameResult.playerName, sizeof(gameResult.playerName), "%s", playerName == NULL || playerName[0] == '\0' ? "Jogador" : playerName);

    if (gameSession != NULL) {
        gameResult.didWin = gameSession->didWin ? 1 : 0;
        gameResult.targetNumber = gameSession->targetNumber;
        gameResult.attemptsUsed = gameSession->attemptCount;
        gameResult.durationSeconds = gameSession->endTime >= gameSession->startTime ? (int) (gameSession->endTime - gameSession->startTime) : 0;
        gameResult.lowBiasCount = gameSession->lowBiasCount;
        gameResult.highBiasCount = gameSession->highBiasCount;
        snprintf(gameResult.guessSequence, sizeof(gameResult.guessSequence), "%s", gameSession->guessSequence);
    }

    if (timeInfo == NULL || strftime(gameResult.timestamp, sizeof(gameResult.timestamp), "%Y-%m-%d %H:%M:%S", timeInfo) == 0) {
        snprintf(gameResult.timestamp, sizeof(gameResult.timestamp), "1970-01-01 00:00:00");
    }

    return gameResult;
}
