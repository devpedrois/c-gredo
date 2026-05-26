#include "strategy.h"
#include "game_logic.h"

#include <stdio.h>

static void addStrategyLine(StrategyReport *strategyReport, const char *line) {
    if (strategyReport == NULL || line == NULL || strategyReport->count >= strategyMaxLines) {
        return;
    }

    snprintf(strategyReport->lines[strategyReport->count], strategyLineLength, "%s", line);
    strategyReport->count++;
}

StrategyReport buildStrategyReport(const GameHistory *gameHistory, const AnalyticsReport *analyticsReport) {
    StrategyReport strategyReport = {{{0}}, 0};

    if (gameHistory == NULL || analyticsReport == NULL || gameHistory->count == 0) {
        addStrategyLine(&strategyReport, "Jogue algumas partidas para gerar sugestoes com base no historico.");
        return strategyReport;
    }

    if (analyticsReport->taxaViesBaixo > 60.0) {
        char buffer[strategyLineLength];
        snprintf(buffer, sizeof(buffer), "Vies Direcional: Voce tende a chutar muito baixo (%.1f%% dos erros). Tente ser mais ousado nos chutes.", analyticsReport->taxaViesBaixo);
        addStrategyLine(&strategyReport, buffer);
    } else if (analyticsReport->taxaViesAlto > 60.0) {
        char buffer[strategyLineLength];
        snprintf(buffer, sizeof(buffer), "Vies Direcional: Voce tende a chutar muito alto (%.1f%% dos erros). Comece com valores mais moderados.", analyticsReport->taxaViesAlto);
        addStrategyLine(&strategyReport, buffer);
    }

    if (analyticsReport->partidasMonotonicas > 0) {
        char buffer[strategyLineLength];
        snprintf(buffer, sizeof(buffer), "Alerta de Busca: %d partida(s) com chutes monotonicos. Tente alternar palpites altos e baixos para cercar o alvo.", analyticsReport->partidasMonotonicas);
        addStrategyLine(&strategyReport, buffer);
    }

    if (analyticsReport->partidasRepetidas > 0) {
        char buffer[strategyLineLength];
        snprintf(buffer, sizeof(buffer), "Alerta de Ineficiencia: %d partida(s) com palpites repetidos! Evite desperdiçar tentativas.", analyticsReport->partidasRepetidas);
        addStrategyLine(&strategyReport, buffer);
    }

    if (analyticsReport->winRate < 50.0) {
        addStrategyLine(&strategyReport, "Estrategia Geral: Use busca binaria (chute 50 e divida o intervalo pela metade a cada feedback).");
    } else {
        addStrategyLine(&strategyReport, "Estrategia Geral: Taxa de vitorias positiva! Continue refinando os intervalos com precisao.");
    }

    if (analyticsReport->averageAttempts > 7.0) {
        char buffer[strategyLineLength];
        snprintf(buffer, sizeof(buffer), "Eficiencia: Media de tentativas alta (%.2f). Evite palpites muito proximos sem necessidade.", analyticsReport->averageAttempts);
        addStrategyLine(&strategyReport, buffer);
    } else {
        addStrategyLine(&strategyReport, "Eficiencia: Media de tentativas controlada. Otimo nivel de aproximacao do alvo!");
    }

    if (analyticsReport->worstAttempts >= maxAttempts) {
        addStrategyLine(&strategyReport, "Seguranca: Algumas partidas chegaram ao limite de 10 tentativas; tente ser mais sistematico.");
    }

    if (analyticsReport->bestAttempts <= 3) {
        addStrategyLine(&strategyReport, "Referencia: Ha partidas excelentes vencidas com poucos chutes; use-as como padrao de decisao.");
    }

    return strategyReport;
}

void printStrategyReport(const GameHistory *gameHistory, const AnalyticsReport *analyticsReport) {
    StrategyReport strategyReport = buildStrategyReport(gameHistory, analyticsReport);
    size_t index;

    printf("\nSugestoes estrategicas:\n");
    for (index = 0; index < strategyReport.count; index++) {
        printf("- %s\n", strategyReport.lines[index]);
    }
}
