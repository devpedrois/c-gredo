#include "gui.h"

#include "game_logic.h"

#include <raylib.h>

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
    windowWidth = 900,
    windowHeight = 520,
    inputBufferLength = 32
};

static void resetGuiGame(GameSession *gameSession, char *inputBuffer, size_t inputBufferSize, char *message, size_t messageSize) {
    inicializarSessaoJogo(gameSession, GetTime());

    if (inputBuffer != NULL && inputBufferSize > 0) {
        inputBuffer[0] = '\0';
    }

    if (message != NULL && messageSize > 0) {
        snprintf(message, messageSize, "Digite um numero entre %d e %d.", targetMinValue, targetMaxValue);
    }
}

static void appendInputDigit(char *inputBuffer, size_t inputBufferSize, int character) {
    size_t inputLength;

    if (inputBuffer == NULL || inputBufferSize == 0 || !isdigit(character)) {
        return;
    }

    inputLength = strlen(inputBuffer);
    if (inputLength + 1 < inputBufferSize) {
        inputBuffer[inputLength] = (char) character;
        inputBuffer[inputLength + 1] = '\0';
    }
}

static void removeLastInputDigit(char *inputBuffer) {
    size_t inputLength;

    if (inputBuffer == NULL) {
        return;
    }

    inputLength = strlen(inputBuffer);
    if (inputLength > 0) {
        inputBuffer[inputLength - 1] = '\0';
    }
}

static void saveFinishedGuiGame(GameHistory *gameHistory, const GameSession *gameSession, char *message, size_t messageSize) {
    GameResult gameResult = criarResultadoDaSessao(gameHistory, gameSession, "Jogador Raylib");

    if (salvarResultadoNoArquivo(&gameResult, rankingFilePath)) {
        adicionarResultadoAoHistorico(gameHistory, gameResult);
        snprintf(message, messageSize, "Partida finalizada e salva. Pressione N para jogar novamente.");
    } else {
        snprintf(message, messageSize, "Partida finalizada, mas nao foi possivel salvar.");
    }
}

int runRaylibApp(GameHistory *gameHistory) {
    GameSession gameSession;
    char inputBuffer[inputBufferLength] = "";
    char message[160];
    int savedCurrentGame = 0;

    InitWindow(windowWidth, windowHeight, "Jogo de Adivinhacao - Raylib");
    SetTargetFPS(60);
    resetGuiGame(&gameSession, inputBuffer, sizeof(inputBuffer), message, sizeof(message));

    while (!WindowShouldClose()) {
        int character = GetCharPressed();

        while (character > 0) {
            appendInputDigit(inputBuffer, sizeof(inputBuffer), character);
            character = GetCharPressed();
        }

        if (IsKeyPressed(KEY_BACKSPACE)) {
            removeLastInputDigit(inputBuffer);
        }

        if (IsKeyPressed(KEY_ENTER) && !gameSession.isFinished) {
            if (!palpiteValido(inputBuffer)) {
                snprintf(message, sizeof(message), "Entrada invalida. Use numeros entre %d e %d.", targetMinValue, targetMaxValue);
            } else {
                enviarPalpite(&gameSession, inputBuffer, GetTime());
                HintResult lastHint = gameSession.hints[gameSession.attemptCount - 1];
                snprintf(message, sizeof(message), "%s", obterTextoFeedback(&lastHint));
                inputBuffer[0] = '\0';
            }
        }

        if (gameSession.isFinished && !savedCurrentGame) {
            saveFinishedGuiGame(gameHistory, &gameSession, message, sizeof(message));
            savedCurrentGame = 1;
        }

        if (gameSession.isFinished && IsKeyPressed(KEY_N)) {
            resetGuiGame(&gameSession, inputBuffer, sizeof(inputBuffer), message, sizeof(message));
            savedCurrentGame = 0;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Jogo de Adivinhacao", 40, 40, 36, DARKBLUE);
        DrawText(TextFormat("Tentativas: %d/%d", gameSession.attemptCount, maxAttempts), 40, 100, 24, DARKGRAY);
        DrawText(TextFormat("Palpite: %s", inputBuffer[0] == '\0' ? "_" : inputBuffer), 40, 150, 28, BLACK);
        DrawText(message, 40, 210, 22, gameSession.didWin ? DARKGREEN : MAROON);
        DrawText(TextFormat("Historico carregado: %zu partida(s)", gameHistory == NULL ? 0 : gameHistory->count), 40, 270, 20, GRAY);

        if (gameSession.isFinished) {
            DrawText(TextFormat("Alvo: %d", gameSession.targetNumber), 40, 330, 24, BLACK);
            DrawText("Pressione N para uma nova partida.", 40, 370, 20, DARKGRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return EXIT_SUCCESS;
}
