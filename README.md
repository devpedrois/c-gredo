# C-GREDO

Projeto academico desenvolvido em C11 para executar um jogo de adivinhacao de numeros no terminal. O jogador tenta descobrir um numero aleatorio entre `1` e `100`, recebe feedback a cada tentativa e tem suas partidas registradas em um arquivo CSV.

## Escopo do Projeto

Este repositorio foi mantido apenas com as entregas solicitadas:

| Data | Entrega |
| --- | --- |
| 21/04 | Estrutura basica do projeto e geracao de numero aleatorio com RNG |
| 28/04 | Loop principal do jogo e persistencia do historico de partidas |

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

- Geracao aleatoria de um numero entre `1` e `100`.
- Validacao de entradas digitadas pelo jogador.
- Limite de `10` tentativas por partida.
- Feedback imediato: `muito baixo`, `muito alto` ou `acertou`.
- Menu simples no terminal.
- Salvamento automatico das partidas em `historico_partidas.csv`.
- Leitura do historico salvo ao iniciar o programa.

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

No Windows, usando GCC:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c game_logic.c persistence.c -o adivinhacao.exe
```

Em Linux/macOS:

```bash
gcc -std=c11 -Wall -Wextra -pedantic main.c game_logic.c persistence.c -o adivinhacao
```

## Como Executar

No Windows:

```bash
adivinhacao.exe
```

Em Linux/macOS:

```bash
./adivinhacao
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
