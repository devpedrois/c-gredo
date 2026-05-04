#include "game_logic.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int gerarNumeroAlvo(void) {
    return targetMinValue + (rand() % (targetMaxValue - targetMinValue + 1));
}

void inicializarSessaoJogo(GameSession *gameSession, double startTime) {
    if (gameSession == NULL) {
        return;
    }

    memset(gameSession, 0, sizeof(*gameSession));
    gameSession->targetNumber = gerarNumeroAlvo();
    gameSession->startTime = startTime;
    gameSession->endTime = startTime;
}

HintResult avaliarPalpiteNumerico(int targetNumber, int guessValue) {
    HintResult hintResult = {0, 0, FEEDBACK_INVALIDO};

    if (guessValue < targetNumber) {
        hintResult.feedback = FEEDBACK_MUITO_BAIXO;
    } else if (guessValue > targetNumber) {
        hintResult.feedback = FEEDBACK_MUITO_ALTO;
    } else {
        hintResult.correctPositions = 1;
        hintResult.feedback = FEEDBACK_ACERTOU;
    }

    return hintResult;
}

bool palpiteValido(const char *guess) {
    int index;
    int guessValue;
    size_t guessLength;

    if (guess == NULL) {
        return false;
    }

    guessLength = strlen(guess);
    if (guessLength == 0 || guessLength > passwordLength) {
        return false;
    }

    for (index = 0; guess[index] != '\0'; index++) {
        if (!isdigit((unsigned char) guess[index])) {
            return false;
        }
    }

    guessValue = atoi(guess);
    return guessValue >= targetMinValue && guessValue <= targetMaxValue;
}

bool enviarPalpite(GameSession *gameSession, const char *guess, double currentTime) {
    HintResult hintResult;
    int guessValue;

    if (gameSession == NULL || guess == NULL || gameSession->isFinished || gameSession->attemptCount >= maxAttempts) {
        return false;
    }

    if (!palpiteValido(guess)) {
        return false;
    }

    guessValue = atoi(guess);
    hintResult = avaliarPalpiteNumerico(gameSession->targetNumber, guessValue);
    if (hintResult.feedback == FEEDBACK_MUITO_BAIXO) {
        gameSession->lowBiasCount++;
    } else if (hintResult.feedback == FEEDBACK_MUITO_ALTO) {
        gameSession->highBiasCount++;
    }

    gameSession->numericGuesses[gameSession->attemptCount] = guessValue;
    snprintf(gameSession->guesses[gameSession->attemptCount], sizeof(gameSession->guesses[gameSession->attemptCount]), "%03d", guessValue);
    gameSession->hints[gameSession->attemptCount] = hintResult;
    gameSession->attemptCount++;

    if (hintResult.feedback == FEEDBACK_ACERTOU) {
        gameSession->didWin = true;
        gameSession->isFinished = true;
        gameSession->endTime = currentTime;
    } else if (gameSession->attemptCount >= maxAttempts) {
        gameSession->didWin = false;
        gameSession->isFinished = true;
        gameSession->endTime = currentTime;
    }

    return true;
}

const char *obterTextoFeedback(const HintResult *hintResult) {
    if (hintResult == NULL) {
        return "palpite invalido";
    }

    switch (hintResult->feedback) {
        case FEEDBACK_MUITO_BAIXO:
            return "muito baixo";
        case FEEDBACK_MUITO_ALTO:
            return "muito alto";
        case FEEDBACK_ACERTOU:
            return "acertou";
        case FEEDBACK_INVALIDO:
        default:
            return "palpite invalido";
    }
}
