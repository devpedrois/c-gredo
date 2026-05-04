#include "gui.h"
#include "game_logic.h"

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void inicializarContextoGui(GuiContext *guiContext) {
    if (guiContext == NULL) {
        return;
    }

    memset(guiContext, 0, sizeof(*guiContext));
    snprintf(guiContext->statusMessage, sizeof(guiContext->statusMessage), "Digite seu nome para entrar no jogo.");
}

void resetarGuiParaNovaPartida(GuiContext *guiContext) {
    if (guiContext == NULL) {
        return;
    }

    memset(guiContext->currentGuess, 0, sizeof(guiContext->currentGuess));
    memset(guiContext->filledPositions, 0, sizeof(guiContext->filledPositions));
    guiContext->currentGuessLength = 0;
    guiContext->cursorPosition = 0;
    guiContext->resultSaved = false;
    snprintf(guiContext->statusMessage, sizeof(guiContext->statusMessage), "Nova partida: adivinhe um numero entre %d e %d.", targetMinValue, targetMaxValue);
}

static void validarNomeParaMenu(GameState *gameState, GuiContext *guiContext) {
    if (gameState == NULL || guiContext == NULL) {
        return;
    }

    if (guiContext->playerNameLength == 0) {
        snprintf(guiContext->statusMessage, sizeof(guiContext->statusMessage), "Informe seu nome antes de continuar.");
        return;
    }

    if (nomeJaRegistrado(guiContext->playerName)) {
        snprintf(guiContext->statusMessage, sizeof(guiContext->statusMessage), "Nome ja registrado!");
        return;
    }

    snprintf(guiContext->statusMessage, sizeof(guiContext->statusMessage), "Bem-vindo, %s.", guiContext->playerName);
    *gameState = MENU;
}

void desenharTelaNome(GameState *gameState, GuiContext *guiContext) {
    validarNomeParaMenu(gameState, guiContext);
}

void desenharTelaMenu(GameState *gameState, GameSession *gameSession, GuiContext *guiContext, const GameHistory *gameHistory) {
    (void) gameHistory;

    if (gameState == NULL || gameSession == NULL || guiContext == NULL) {
        return;
    }

    inicializarSessaoJogo(gameSession, 0.0);
    resetarGuiParaNovaPartida(guiContext);
    *gameState = JOGANDO;
}

static void atualizarTamanhoPalpiteAtual(char currentGuess[], bool filledPositions[], int *currentGuessLength) {
    int index;

    if (currentGuess == NULL || filledPositions == NULL || currentGuessLength == NULL) {
        return;
    }

    currentGuess[passwordLength] = '\0';
    *currentGuessLength = (int) strlen(currentGuess);
    for (index = 0; index < passwordLength; index++) {
        filledPositions[index] = index < *currentGuessLength;
    }
}

static bool montarPalpiteGui(char currentGuess[], bool filledPositions[], int *currentGuessLength, char *guessBuffer, size_t bufferSize) {
    if (currentGuess == NULL || filledPositions == NULL || currentGuessLength == NULL || guessBuffer == NULL || bufferSize <= passwordLength) {
        return false;
    }

    atualizarTamanhoPalpiteAtual(currentGuess, filledPositions, currentGuessLength);
    if (*currentGuessLength <= 0) {
        return false;
    }

    snprintf(guessBuffer, bufferSize, "%s", currentGuess);
    return palpiteValido(guessBuffer);
}

static bool processarEnvioPalpiteGui(GameSession *gameSession, char currentGuess[], bool filledPositions[], int *currentGuessLength, char *statusMessage, size_t statusMessageSize) {
    char guessToSubmit[passwordLength + 1];
    HintResult lastHint;

    if (gameSession == NULL || statusMessage == NULL) {
        return false;
    }

    if (!montarPalpiteGui(currentGuess, filledPositions, currentGuessLength, guessToSubmit, sizeof(guessToSubmit))) {
        snprintf(statusMessage, statusMessageSize, "Digite um numero valido entre %d e %d.", targetMinValue, targetMaxValue);
        return false;
    }

    if (!enviarPalpite(gameSession, guessToSubmit, 0.0)) {
        snprintf(statusMessage, statusMessageSize, "Palpite invalido ou partida ja encerrada.");
        return false;
    }

    lastHint = gameSession->hints[gameSession->attemptCount - 1];
    snprintf(statusMessage, statusMessageSize, "Feedback: %s.", obterTextoFeedback(&lastHint));
    memset(currentGuess, 0, passwordLength + 1);
    memset(filledPositions, 0, (size_t) passwordLength * sizeof(bool));
    *currentGuessLength = 0;
    return true;
}
