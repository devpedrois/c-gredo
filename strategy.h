#ifndef STRATEGY_H
#define STRATEGY_H

#include "analytics.h"
#include "persistence.h"

#include <stddef.h>

enum {
    strategyMaxLines = 8,
    strategyLineLength = 192
};

typedef struct {
    char lines[strategyMaxLines][strategyLineLength];
    size_t count;
} StrategyReport;

StrategyReport buildStrategyReport(const GameHistory *gameHistory, const AnalyticsReport *analyticsReport);
void printStrategyReport(const GameHistory *gameHistory, const AnalyticsReport *analyticsReport);

#endif
