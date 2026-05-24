#include "analytics.h"

#include <stdio.h>

static int isBetterLeaderboardEntry(const GameResult *candidate, const GameResult *current) {
    if (candidate->didWin != current->didWin) {
        return candidate->didWin > current->didWin;
    }

    if (candidate->attemptsUsed != current->attemptsUsed) {
        return candidate->attemptsUsed < current->attemptsUsed;
    }

    return candidate->durationSeconds < current->durationSeconds;
}

AnalyticsReport buildAnalyticsReport(const GameHistory *gameHistory) {
    AnalyticsReport report = {0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0};
    size_t index;
    int totalAttempts = 0;
    int totalLowBias = 0;
    int totalHighBias = 0;

    if (gameHistory == NULL || gameHistory->count == 0) {
        return report;
    }

    report.playedGames = (int) gameHistory->count;
    report.bestAttempts = gameHistory->results[0].attemptsUsed;
    report.worstAttempts = gameHistory->results[0].attemptsUsed;

    for (index = 0; index < gameHistory->count; index++) {
        const GameResult *result = &gameHistory->results[index];

        if (result->didWin) {
            report.wonGames++;
        }

        if (result->attemptsUsed < report.bestAttempts) {
            report.bestAttempts = result->attemptsUsed;
        }

        if (result->attemptsUsed > report.worstAttempts) {
            report.worstAttempts = result->attemptsUsed;
        }

        totalAttempts += result->attemptsUsed;
        totalLowBias += result->lowBiasCount;
        totalHighBias += result->highBiasCount;
    }

    report.lostGames = report.playedGames - report.wonGames;
    report.averageAttempts = (double) totalAttempts / (double) report.playedGames;
    report.winRate = ((double) report.wonGames / (double) report.playedGames) * 100.0;

    if (totalLowBias + totalHighBias > 0) {
        report.taxaViesBaixo = ((double) totalLowBias / (totalLowBias + totalHighBias)) * 100.0;
        report.taxaViesAlto = ((double) totalHighBias / (totalLowBias + totalHighBias)) * 100.0;
    } else {
        report.taxaViesBaixo = 0.0;
        report.taxaViesAlto = 0.0;
    }

    return report;
}

Leaderboard buildLeaderboard(const GameHistory *gameHistory) {
    Leaderboard leaderboard = {{{0}}, 0};
    size_t historyIndex;

    if (gameHistory == NULL) {
        return leaderboard;
    }

    for (historyIndex = 0; historyIndex < gameHistory->count; historyIndex++) {
        const GameResult *candidate = &gameHistory->results[historyIndex];
        size_t insertIndex = leaderboard.count;

        if (leaderboard.count < leaderboardMaxEntries) {
            leaderboard.entries[leaderboard.count] = *candidate;
            insertIndex = leaderboard.count;
            leaderboard.count++;
        } else if (isBetterLeaderboardEntry(candidate, &leaderboard.entries[leaderboard.count - 1])) {
            leaderboard.entries[leaderboard.count - 1] = *candidate;
            insertIndex = leaderboard.count - 1;
        } else {
            continue;
        }

        while (insertIndex > 0 && isBetterLeaderboardEntry(&leaderboard.entries[insertIndex], &leaderboard.entries[insertIndex - 1])) {
            GameResult temporary = leaderboard.entries[insertIndex - 1];
            leaderboard.entries[insertIndex - 1] = leaderboard.entries[insertIndex];
            leaderboard.entries[insertIndex] = temporary;
            insertIndex--;
        }
    }

    return leaderboard;
}

void printAnalyticsReport(const GameHistory *gameHistory) {
    AnalyticsReport report = buildAnalyticsReport(gameHistory);

    if (report.playedGames == 0) {
        printf("\nAinda nao existem partidas para analisar.\n");
        return;
    }

    printf("\nRelatorio de analytics:\n");
    printf("Partidas jogadas: %d\n", report.playedGames);
    printf("Vitorias: %d\n", report.wonGames);
    printf("Derrotas: %d\n", report.lostGames);
    printf("Taxa de vitoria: %.2f%%\n", report.winRate);
    printf("Media de tentativas: %.2f\n", report.averageAttempts);
    printf("Melhor desempenho: %d tentativa(s)\n", report.bestAttempts);
    printf("Pior desempenho: %d tentativa(s)\n", report.worstAttempts);
    printf("Vies para baixo (chutes muito baixos): %.2f%%\n", report.taxaViesBaixo);
    printf("Vies para cima (chutes muito altos): %.2f%%\n", report.taxaViesAlto);
}

void printLeaderboard(const GameHistory *gameHistory) {
    Leaderboard leaderboard = buildLeaderboard(gameHistory);
    size_t index;

    if (leaderboard.count == 0) {
        printf("\nRanking vazio.\n");
        return;
    }

    printf("\nTop %d ranking:\n", leaderboardMaxEntries);
    for (index = 0; index < leaderboard.count; index++) {
        const GameResult *entry = &leaderboard.entries[index];

        printf(
            "%zu. %s | %s | %d tentativa(s) | %ds | alvo %d\n",
            index + 1,
            entry->playerName,
            entry->didWin ? "vitoria" : "derrota",
            entry->attemptsUsed,
            entry->durationSeconds,
            entry->targetNumber
        );
    }
}