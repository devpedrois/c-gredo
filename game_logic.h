#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <stdbool.h>
#include <stddef.h>

enum {
    targetMinValue = 1,
    targetMaxValue = 100,
    passwordLength = 3,
    maxAttempts = 10,
    guessSequenceLength = 192
};

typedef enum {
    FEEDBACK_INVALIDO = 0,
    FEEDBACK_MUITO_BAIXO = -1,
    FEEDBACK_ACERTOU = 1,
    FEEDBACK_MUITO_ALTO = 2
} GuessFeedback;

typedef struct {
    int correctPositions;
    int misplacedDigits;
    GuessFeedback feedback;
} HintResult;

typedef struct {
    int targetNumber;
    int numericGuesses[maxAttempts];
    int lowBiasCount;
    int highBiasCount;
    char guessSequence[guessSequenceLength];
    char guesses[maxAttempts][passwordLength + 1];
    HintResult hints[maxAttempts];
    int attemptCount;
    bool didWin;
    bool isFinished;
    double startTime;
    double endTime;
} GameSession;

void inicializarSessaoJogo(GameSession *gameSession, double startTime);
int gerarNumeroAlvo(void);
HintResult avaliarPalpiteNumerico(int targetNumber, int guessValue);
bool palpiteValido(const char *guess);
bool enviarPalpite(GameSession *gameSession, const char *guess, double currentTime);
const char *obterTextoFeedback(const HintResult *hintResult);

#endif
