# Introduction

A terminal version of the legendary sudoku game.

# Author(s)

- Authors: *Caio Luiz Silvério Diógenes* e *Eduardo Nestor Ferreira Marinho*
- Emails: *<canalclsd@gmail.com>* e *<eduardonestormarinho228@gmail.com>*

# Additional features

The player can have multiple games ongoing simultaneously without losing any of the 
functionalities, so, no loss for the player, except in the [hardest ever sudoku
game](https://abcnews.go.com/blogs/headlines/2012/06/can-you-solve-the-hardest-ever-sudoku),
that the user can play after the special interface(if he is brave enough). There is also a
winning report for every game won by the player.

# Grading

Item     | Valor máximo   | Valor esperado
-------- | :-----: | :-----:
Trata corretamente os argumentos de linha de comando | 5 pts | 5 pts
Lê e armazena os vários puzzles contidos em um arquivo de entrada |5 pts| 5 pts
Exibe corretamente a tela principal com o menu de ações |5 pts| 5 pts
Permite a escolha dos vários puzzles lidos do arquivo  |5 pts| 5 pts
Executa corretamente a ação de posicionamento de dígitos no tabuleiro (comando posicionar) identificação jogadas inválidas |11 pts| 11 pts
Executa corretamente a remoção de dígitos posicionados pelo jogador (comando remover) |10 pts| 10 pts
Executa corretamente o comando de desfazer ação |12 pts| 12 pts
Executa corretamente o comando de verificação do tabuleiro |12 pts| 12 pts
Salva corretamente, em arquivo externo, uma partida em andamento | 7 pts | 7 pts
Carrega corretamente, de um arquivo externo, uma partida em andamento previamente salva e consegue retormar a partida | 8 pts | 8 pts
Exibe as regras do jogo quando solicitado |5 pts| 5 pts
Indica corretamente se uma partida finalizada foi vencida ou perdida |5 pts| 5 pts
Apresenta uma interface com usuário igual ou similar a que foi especificada nesse documento |5 pts| 5 pts
Apresenta uma validação robusta dos comandos textuais de ação fornecidos pelo usuário |5 pts| 5 pts
Exibe corretamente a lista de dígitos disponíveis, conforme o progresso do jogo |5 pts| 5 pts
Apresenta as indicações com `setas` da coordenada da célula que foi alvo da última ação |5 pts| 5 pts
Apresenta um fluxo correto entre telas, com opção de retomar uma partida suspensa e solicitação de confirmação de finalização de partida, caso exista uma partida em andamento |5 pts| 5 pts

# Compiling

This project is design to be compiled with Cmake.
To compile this project be sure to be in the main directory,
called "trabalho-04-sudoku-eduardo-e-caio" and then run the
followings commands (assuming '$' is the terminal prompt):

$ Cmake -S source -B build

$ cd build

$ make

This commands triggers the compiling process.

# Running

From the "build" directory run (assuming '$' is the terminal prompt):

$ ./sudoku
