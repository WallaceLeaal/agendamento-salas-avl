# Agendamento de Salas com Arvore AVL

Atividade da disciplina Estrutura de Dados II (DSM - FATEC).

## Objetivo
Criar um sistema em C++ que permita reservar uma sala, detectar conflitos de horarios e consultar a agenda do dia usando uma arvore AVL.

## Arquivos
- `agendamento_avl.cpp`: programa principal em C++
- `01_prompt_utilizado.txt`: prompt empregado como apoio
- `04_analise_critica.txt`: avaliacao da implementacao

Os relatorios em PDF e evidencias podem ser adicionados separadamente.

## Compilar e executar
```bash
g++ -std=c++17 -Wall -Wextra -o agendamento_avl agendamento_avl.cpp
./agendamento_avl
```
No Windows, execute `agendamento_avl.exe` apos compilar.

## Como usar
1. Escolha 1 para agendar sala, informando nome, inicio e fim em HH:MM.
2. Escolha 2 para visualizar a agenda em ordem cronologica.
3. Escolha 3 para sair.

Reservas iniciais: Daily Standup (10:00-11:00) e Planejamento Trimestral (15:00-17:00).

## Observacao
O sistema utiliza intervalos [inicio, fim): terminar as 11:00 e iniciar outra reuniao as 11:00 nao constitui conflito.
