# C-GREDO

Projeto academico desenvolvido em C11 para executar um jogo de adivinhacao de numeros no terminal. O jogador tenta descobrir um numero aleatorio entre `1` e `100`, recebe feedback a cada tentativa e tem suas partidas registradas em um arquivo CSV.

## Escopo do Projeto

Este repositorio foi mantido apenas com as entregas solicitadas:

| Data | Entrega | Status | Descrição |
| --- | --- | :---: | --- |
| 21/04 | Milestone 1 | 🟩 *Concluído* | Estrutura básica do projeto e geração de número aleatório com RNG |
| 28/04 | Milestone 2 | 🟩 *Concluído* | Loop principal do jogo e persistência do histórico de partidas em CSV |
| 05/05 | **Capstone 1** | 🟩 *Concluído* | MVP funcional do jogo clássico de terminal com salvamento do histórico |
| 12/05 | Milestone 4 | 🟩 *Concluído* | Relatório analítico dinâmico de João e Top 5 Leaderboard integrado |
| 19/05 | Milestone 5 | 🟩 *Concluído* | **Entregável do Ricardo**: Algoritmos recursivos, desvio padrão e interface Raylib |
| 26/05 | **Capstone 2** | 🟩 *Concluído* | Integração do motor heurístico de estratégia e sugestões de Pedro |

## Equipe e Responsabilidades

| Pessoa do codigo | Participante | Funcao no projeto | Responsabilidade tecnica |
| --- | --- | --- | --- |
| Rafael Aimbere | Desenvolvedor de Fluxo Principal | `main.c`, menu, fluxo principal e integracao da execucao no terminal. |
| Gabriel Barbosa | Desenvolvedor de Logica de Jogo | `game_logic.c`, logica da partida, geracao do alvo, validacao de palpites e feedback ao jogador. |
| Eduardo Borges | Desenvolvedor de Persistencia | `persistence.c`, salvamento e carregamento do historico em CSV, estrutura dos registros e controle dos dados persistidos. |
| João Vinicius | Desenvolvedor de Analytics | `analytics.c`, metricas agregadas, estatisticas das partidas e estrutura de ranking. |
| Ricardo Severiano | Desenvolvedor de Algoritmos | Implementacao de funcoes recursivas, calculos auxiliares e apoio a metricas do projeto. |
| Pedro Iranildo | Desenvolvedor de Estrategia | Tech Lead, `strategy.c`, heuristicas, analise de padroes e sugestoes textuais baseadas no historico. |
| Gabriela Bayo | Product Owner, Documentacao e QA | README, documentacao tecnica, revisao final, criterios de entrega e polimento do projeto. |

## Funcionalidades

* **Geração RNG e Validação Segura**: Sorteio aleatório entre 1 e 100 com controle rígido de 10 tentativas por partida.
* **Persistência Híbrida em CSV**: Carregamento e gravação automáticos em `historico_partidas.csv` gravando desvios e estatísticas.
* **Métricas Estatísticas Avançadas**: Relatório com contagem de jogos, taxa de vitória (%), média de tentativas e desvio padrão.
* **Leaderboard Top 5**: Classificação em tempo real dos melhores jogadores ordenados por vitória, menor número de tentativas e menor tempo.
* **Paradigma Funcional (Recursão Pura)**: Cálculo estatístico de média, mínimo, máximo e soma de diferenças quadráticas (desvio padrão) sem o uso de laços `for` ou `while`.
* **Interface Gráfica de Usuário (Raylib GUI)**: Janela de jogo dinâmica rodando a 60 FPS com teclado físico capturado e renderização instantânea dos estados do jogo.

## Tecnologias

- Linguagem: C
- Padrao: C11
- Interface: Terminal
- Persistencia: Arquivo CSV
- Compilador recomendado: GCC

## Estrutura de Arquivos

```text
C-gredo/
|-- main.c              
|-- game_logic.c         
|-- game_logic.h
|-- persistence.c        
|-- persistence.h
|-- analytics.c          
|-- analytics.h
|-- recursive_utils.c    
|-- recursive_utils.h
|-- gui.c                
|-- gui.h
|-- README.md
`-- .gitignore
```

### Descricao dos Modulos

| Arquivo | Responsabilidade |
| --- | --- |
| `main.c` | Controla a entrada do programa, o menu, a leitura de dados do terminal e o fluxo principal do jogo. |
| `game_logic.c` / `game_logic.h` | Contem a regra do jogo: geracao do alvo, validacao de palpites, avaliacao das tentativas e feedback. |
| `persistence.c` / `persistence.h` | Gerencia o historico de partidas, incluindo carregamento, armazenamento em memoria e gravacao em CSV. |

## Como Compilar

Esta versão suporta dois modos de compilação e execução por meio de diretivas do pré-processador:

### 🖥️ Modo 1: Sem GUI (Interface clássica de Terminal)
Esta versão executa o menu completo interativo no terminal padrão, utilizando a biblioteca padrão C e a análise estatística com desvio padrão recursivo.

**Para Compilar:**
```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c game_logic.c persistence.c analytics.c recursive_utils.c -lm -o adivinhacao.exe
```

**Para Executar:**
```bash
.\adivinhacao.exe
```

---

### 🎮 Modo 2: Com GUI (Interface Gráfica via Raylib)
Esta versão inicia o jogo em uma janela gráfica dinâmica nativa a 60 FPS. Requer que a biblioteca **Raylib** esteja instalada no compilador GCC/MinGW do sistema.

**Para Compilar:**
```bash
gcc -std=c11 -Wall -Wextra -pedantic -DUSE_RAYLIB main.c game_logic.c persistence.c analytics.c recursive_utils.c gui.c -lraylib -lm -o adivinhacao_raylib.exe
```

**Para Executar:**
```bash
.\adivinhacao_raylib.exe
```

## Fluxo de Uso

1. O programa solicita o nome do jogador.
2. O menu principal exibe as opcoes:
   - `1. Jogar`
   - `2. Ver historico`
   - `3. Sair`
3. Ao iniciar uma partida, o sistema gera um numero secreto entre `1` e `100`.
4. O jogador informa palpites ate acertar ou atingir o limite de tentativas.
5. Ao final, o resultado e salvo automaticamente no historico.

## Persistencia

As partidas sao gravadas no arquivo:

```text
historico_partidas.csv
```

Formato dos registros:

```text
id;nome;vitoria;alvo;tentativas;duracao;sequencia;timestamp
```

Exemplo:

```text
1;Rafael;1;42;6;18;50,25,37,43,41,42;2026-05-05 20:30:00
```

### Campos do Historico

| Campo | Descricao |
| --- | --- |
| `id` | Identificador incremental da partida. |
| `nome` | Nome informado pelo jogador. |
| `vitoria` | `1` para vitoria e `0` para derrota. |
| `alvo` | Numero secreto gerado pelo jogo. |
| `tentativas` | Quantidade de palpites usados. |
| `duracao` | Duracao da partida em segundos. |
| `sequencia` | Lista de palpites realizados durante a partida. |
| `timestamp` | Data e hora em que o resultado foi salvo. |

## Regras do Jogo

- O numero secreto sempre fica entre `1` e `100`.
- Apenas numeros inteiros dentro da faixa sao aceitos.
- Entradas invalidas nao contam como tentativa.
- O jogador vence ao acertar o numero antes do limite.
- Se o limite de `10` tentativas for atingido, a partida termina como derrota.

## Observacoes

- O projeto atual nao inclui interface grafica, ranking, analytics ou estrategias adicionais.
- O arquivo `historico_partidas.csv` e gerado automaticamente durante o uso.
- Arquivos `.exe`, `.o` e historicos locais ficam fora do controle de versao por estarem listados no `.gitignore`.
