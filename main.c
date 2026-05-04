#include "game_logic.h"
#include "persistence.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static bool lerLinhaTerminal(char *buffer, size_t bufferSize)
{
    if (buffer == NULL || bufferSize == 0 || fgets(buffer, bufferSize, stdin) == NULL)
    {
        return false;
    }

    buffer[strcspn(buffer, "\r\n")] = '\0';
    return true;
}

static bool lerInteiroTerminal(const char *prompt, int *value)
{
    char line[64];
    char *endPointer;
    long parsedValue;

    if (prompt != NULL)
    {
        printf("%s", prompt);
    }

    if (!lerLinhaTerminal(line, sizeof(line)))
    {
        return false;
    }

    parsedValue = strtol(line, &endPointer, 10);
    if (line[0] == '\0' || *endPointer != '\0')
    {
        return false;
    }

    *value = (int)parsedValue;
    return true;
}

static void lerNomeJogadorTerminal(char *playerName, size_t playerNameSize)
{
    if (playerName == NULL || playerNameSize == 0)
    {
        return;
    }

    do
    {
        printf("Nome do jogador: ");
        if (!lerLinhaTerminal(playerName, playerNameSize) || playerName[0] == '\0')
        {
            snprintf(playerName, playerNameSize, "Jogador");
        }

        if (nomeJaRegistrado(playerName))
        {
            printf("Nome ja registrado. Escolha outro nome para nao sobrepor dados.\n");
            playerName[0] = '\0';
        }
    } while (playerName[0] == '\0');
}

static void executarPartidaTerminal(GameHistory *gameHistory, const char *playerName)
{
    GameSession gameSession;
    GameResult gameResult;

    inicializarSessaoJogo(&gameSession, (double)time(NULL));
    printf("\nAdivinhe um numero entre %d e %d.\n", targetMinValue, targetMaxValue);

    while (!gameSession.isFinished)
    {
        char guessText[16];

        printf("Tentativa %d/%d: ", gameSession.attemptCount + 1, maxAttempts);
        if (!lerLinhaTerminal(guessText, sizeof(guessText)) || !palpiteValido(guessText))
        {
            printf("Entrada invalida. Digite um numero entre %d e %d.\n", targetMinValue, targetMaxValue);
            continue;
        }

        enviarPalpite(&gameSession, guessText, (double)time(NULL));
        printf("%s\n", obterTextoFeedback(&gameSession.hints[gameSession.attemptCount - 1]));
    }

    printf("Alvo: %d | Tentativas: %d | Sequencia: %s\n", gameSession.targetNumber, gameSession.attemptCount, gameSession.guessSequence);
    gameResult = criarResultadoDaSessao(gameHistory, &gameSession, playerName);
    if (salvarResultadoNoArquivo(&gameResult, rankingFilePath))
    {
        adicionarResultadoAoHistorico(gameHistory, gameResult);
    }
}

int main(void)
{
    GameHistory gameHistory;
    char playerName[playerNameLength];
    int option = 0;

    srand((unsigned int)time(NULL));
    inicializarHistorico(&gameHistory);
    carregarHistorico(&gameHistory, rankingFilePath);
    lerNomeJogadorTerminal(playerName, sizeof(playerName));

    while (option != 3)
    {
        printf("\n1. Jogar\n2. Analisar\n3. Sair\n");
        if (!lerInteiroTerminal("Escolha: ", &option))
        {
            printf("Opcao invalida.\n");
            continue;
        }

        if (option == 1)
        {
            executarPartidaTerminal(&gameHistory, playerName);
        }
        else if (option == 2)
        {
            printf("\nHistorico carregado: %d partida(s).\n", (int)gameHistory.count);
        }
        else if (option != 3)
        {
            printf("Opcao invalida.\n");
        }
    }

    liberarHistorico(&gameHistory);
    return EXIT_SUCCESS;
}
