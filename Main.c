#include <stdio.h>
#include <limits.h>



#define player1 'X' // global variables to assign X and O to player names
#define AI 'O'

void makeBoard(char board[3][3]){ // makes the board
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
        board[i][j] = ' ';
        }
    }
}

void displayBoard(char board[3][3]){ // prints the board with grid numbers to help users
  printf("\n    1   2   3\n\n");
  for (int i = 0; i < 3; i++){
    printf("%d   ", (i + 1));
    for (int j = 0; j < 3; j++){
      printf("%c", board[i][j]);
      if (j != 2){
        printf(" | ");
      }
    }
    if (i != 2){
    printf("\n   ---+---+---\n");
    }
  }
  printf("\n\n");
}

int valid(char board[3][3], int row, int col){ // checks if the move is valid
  if (board[row][col] != ' '){
    return 0;
  }
  else return row < 0 || row > 2 || col < 0 || col > 2 ? 0 : 1;
}

int checkGame(char board[3][3], int pturn, char player, int isMinimaxing){ // checks if a player has won

  for(int i = 0; i < 3; i++){
    if (board[i][0] == player && board[i][1] == player && board[i][2] == player || board[0][i] == player && board[1][i] == player && board[2][i] == player){
      return 1;
    }
  }
  if (board[0][0] == player && board[1][1] == player && board[2][2] == player || board[2][0] == player && board[1][1] == player && board [0][2] == player){
    return 1;
  }

  if (isMinimaxing) { // only used within Minimax
      player = player1;

      for (int i = 0; i < 3; i++) {
          if (board[i][0] == player && board[i][1] == player && board[i][2] == player ||
              board[0][i] == player && board[1][i] == player && board[2][i] == player) {
              return -1;
          }
      }
      if (board[0][0] == player && board[1][1] == player && board[2][2] == player ||
          board[2][0] == player && board[1][1] == player && board[0][2] == player) {
          return -1;
      }
  }
  if (pturn >= 9){
    return 0;
  }

  return 2;
}


int miniMax(char board[3][3], int pturn, char player, int alpha, int beta){ // miniMax algorithm to evaluate hypothetical moves

  int gameState = checkGame(board, pturn, AI, 1);
  if (gameState == 0){
    pturn++;
    return 0;
  }
  else if (gameState == 1){
    pturn++;
    return 100 - pturn;
  }
  else if (gameState == -1){
    return -100 + pturn;
  }

  else{
    pturn++;
  }
  if(player == AI){ // maximizing tree
    int bestScore = INT_MIN;
    for (int i = 0; i < 3; i++){
      for (int j = 0; j < 3; j++) {
          if (board[i][j] == ' ') {
              board[i][j] = AI;
              int score = miniMax(board, pturn, player1, alpha, beta);
              board[i][j] = ' ';
              bestScore = (bestScore < score) ? score : bestScore;
              alpha = (bestScore > alpha) ? bestScore : alpha;
              if (alpha >= beta) {
                  goto tag1;
              }
          }
      }
    }
    tag1:
      return bestScore;
  }
  else{ // minimizing tree
    int bestScore = INT_MAX;
    for (int i = 0; i < 3; i++){
      for (int j = 0; j < 3; j++){
        if (board[i][j] == ' '){
          board[i][j] = player1;
          int score = miniMax(board, pturn, AI, alpha, beta);
          board[i][j] = ' ';
          bestScore = (bestScore > score) ? score : bestScore;
          beta = (bestScore < beta) ? bestScore : beta;
          if (alpha >= beta){
              goto tag2;
          }
        }
      }
    }
    tag2:
        return bestScore;
  }
}

void compMove(char board[3][3], int pturn){ // iterates through hypothetical moves and finds the best one using miniMax

  int bestScore = INT_MIN;
  int bestI = -1;
  int bestJ = -1;
  int alpha = INT_MIN;
  int beta = INT_MAX;

  for (int i = 0; i < 3; i++){
    for (int j = 0; j < 3; j ++){
      if (board[i][j] == ' '){
        board[i][j] = AI;
        int score = miniMax(board, pturn + 1, player1, alpha, beta);
        board[i][j] = ' ';

        if (score >= bestScore){
          bestI = i;
          bestJ = j;
          bestScore = score;
        }
      }
    }
  }

  board[bestI][bestJ] = AI;
}

int main(void) {
  int row, col;
  char board[3][3];
  int pturns = 0;
  int cPlayer;
  int startTurn;


  makeBoard(board);
  printf("Would you like to play (1)st or (2)nd? ");
  scanf("%d", &startTurn);
  cPlayer = startTurn - 1;

  while(1){

    displayBoard(board);

    if (cPlayer == 0){
      printf("It's your turn\n");
      printf("Where would you like to move?\nInput (row, col): ");
      scanf("%d, %d", &row, &col);

      row--;
      col--;

      if(!valid(board, row, col)){
        while(getchar() != '\n'){}
        printf("\nThis is an invalid move! Please try again.\n");

        continue;
      }

      board[row][col] = player1;
    }
    else{
      compMove(board, pturns);
    }
    pturns++;

    switch (checkGame(board, pturns, ((cPlayer == 0) ? player1 : AI), 0)){ // checks if the game is over and returns ending output if the game is over
      case 0:
        displayBoard(board);
        printf("The game is over. It's a tie!");
        return 0;
      case 1:
        displayBoard(board);
        printf("The game is over. %c won!", ((cPlayer == 0) ? player1 : AI));
        return 0;
    }

    cPlayer = (cPlayer == 0) ? 1 : 0;
  }
}
