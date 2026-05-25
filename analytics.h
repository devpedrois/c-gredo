#ifndef ANALYTICS_H
#define ANALYTICS_H

#include "persistence.h"
#include <stddef.h>

enum {
    leaderboardMaxEntries = 5
};

typedef struct {
    int playedGames;
    int wonGames;
    int lostGames;
    int bestAttempts;
    int worstAttempts;
    double averageAttempts;
    double standardDeviation;
    double winRate;
    double taxaViesBaixo;      
    double taxaViesAlto;       
    int partidasMonotonicas;  
    int partidasRepetidas;    
} AnalyticsReport;

typedef struct {
    GameResult entries[leaderboardMaxEntries];
    size_t count;
} Leaderboard;

AnalyticsReport buildAnalyticsReport(const GameHistory *gameHistory);
Leaderboard buildLeaderboard(const GameHistory *gameHistory);
void printAnalyticsReport(const GameHistory *gameHistory);
void printLeaderboard(const GameHistory *gameHistory);

#endif
