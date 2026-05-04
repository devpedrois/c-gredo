#ifndef GUI_H
#define GUI_H

#include "game_logic.h"
#include "persistence.h"

#include <stdbool.h>

enum {
    screenWidth = 1280,
    screenHeight = 720,
    targetFps = 60,
    guiMessageLength = 192
};

typedef enum {
    NOME,
    MENU,
    JOGANDO,
    RANKING,
    SAIR
} GameState;

typedef struct {
    char playerName[playerNameLength];
    int playerNameLength;
    char currentGuess[passwordLength + 1];
    bool filledPositions[passwordLength];
    int currentGuessLength;
    int cursorPosition;
    float rankingScroll;
    float analysisScroll;
    char statusMessage[guiMessageLength];
    bool resultSaved;
} GuiContext;

void inicializarContextoGui(GuiContext *guiContext);
void resetarGuiParaNovaPartida(GuiContext *guiContext);
void desenharTelaNome(GameState *gameState, GuiContext *guiContext);
void desenharTelaMenu(GameState *gameState, GameSession *gameSession, GuiContext *guiContext, const GameHistory *gameHistory);

#endif
