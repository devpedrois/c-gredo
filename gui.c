#include "gui.h"

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
