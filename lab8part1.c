//
// Author: Rithvika Kathroju
//

#include "lab8part1.h"

#include <stdbool.h>
#include <stdio.h>

void initializeBoard(char board[][26], int n) {
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      board[row][col] = 'U';
    }
  }
  board[n / 2 - 1][n / 2 - 1] = 'W';
  board[n / 2 - 1][n / 2] = 'B';
  board[n / 2][n / 2 - 1] = 'B';
  board[n / 2][n / 2] = 'W';
}

void printBoard(char board[][26], int n) {
  printf("  ");
  for (int i = 0; i < n; i++) {
    printf("%c", 'a' + i);
  }
  printf("\n");
  for (int row = 0; row < n; row++) {
    printf("%c ", 'a' + row);
    for (int col = 0; col < n; col++) {
      printf("%c", board[row][col]);
    }
    printf("\n");
  }
}

bool availableMoves(char board[][26], int n, char colour) {
  /*
  NW N NE
  W     E
  SW S SE
  top to bottom (rows), left to right (cols)
  */
  int deltaRow[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int deltaCol[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        for (int dir = 0; dir < 8; dir++) {
          if (checkLegalInDirection(board, n, row, col, colour, deltaRow[dir],
                                    deltaCol[dir])) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool positionInBounds(int n, int row, int col) {
  return (row >= 0 && row < n && col >= 0 && col < n);
}

//
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol) {
  // find opponent colour
  char opponentColour;
  if (colour == 'W') {
    opponentColour = 'B';
  } else {
    opponentColour = 'W';
  }

  // check first position
  row = row + deltaRow;
  col = col + deltaCol;
  if (!positionInBounds(n, row, col) || board[row][col] != opponentColour) {
    return false;
  }

  // check second and onwards positions
  row = row + deltaRow;
  col = col + deltaCol;
  while (positionInBounds(n, row, col) && board[row][col] != 'U') {
    if (board[row][col] == colour) {
      return true;
    }
    row = row + deltaRow;
    col = col + deltaCol;
  }

  // exited while loop meaning it never found the end of "trap"
  return false;
}

//
void flipTiles(char board[][26], int n, int row, int col, char colour) {
  int i = 1;
  int deltaRow[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int deltaCol[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  for (int dir = 0; dir < 8; dir++) {
    if (checkLegalInDirection(board, n, row, col, colour, deltaRow[dir],
                              deltaCol[dir])) {
      while (board[row + (deltaRow[dir] * i)][col + (deltaCol[dir] * i)] !=
             colour) {
        board[row + (deltaRow[dir] * i)][col + (deltaCol[dir] * i)] = colour;
        i++;
      }
      i = 1;
    }
  }
}

//
bool checkIfValidInput(char board[][26], int n, int row, int col, char colour) {
  int deltaRow[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int deltaCol[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  if (positionInBounds(n, row, col) && board[row][col] == 'U') {
    for (int dir = 0; dir < 8; dir++) {
      if (checkLegalInDirection(board, n, row, col, colour, deltaRow[dir],
                                deltaCol[dir])) {
        board[row][col] = colour;
        return true;
      }
    }
    return false;
  } else {
    return false;
  }
}

// humanMove
void humanMove(char board[][26], int n, char humanColour, bool* humanLegal,
               bool* invalid) {
  bool hasAvailableMove = availableMoves(board, n, humanColour);
  bool validInput;
  char row, col;

  if (hasAvailableMove) {
    *humanLegal = true;
    printf("Enter move for colour %c (RowCol): ", humanColour);
    scanf(" %c%c", &row, &col);

    validInput = checkIfValidInput(board, n, row - 'a', col - 'a', humanColour);
    if (validInput == true) {
      flipTiles(board, n, row - 'a', col - 'a', humanColour);
      printBoard(board, n);
    } else {
      *invalid = true;
    }
  } else {
    *humanLegal = false;
  }
  return;
}

// compMoveScore()
int compMoveScore(char board[][26], int n, char compColour, int row, int col) {
  int currentScore = 0;
  char tempBoard[26][26];

  // copy board
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < n; c++) {
      tempBoard[r][c] = board[r][c];
    }
  }

  // flip tiles
  flipTiles(tempBoard, n, row, col, compColour);

  // count total comp score
  for (int r = 0; r < n; r++) {
    for (int c = 0; c < n; c++) {
      if (tempBoard[r][c] == compColour) {
        currentScore++;
      }
    }
  }
  return currentScore;
}

// compMove
void compMove(char board[][26], int n, char compColour, bool* compLegal) {
  bool hasAvailableMove = availableMoves(board, n, compColour);
  int currentScore = 0, maxScore = 0;
  int bestRow, bestCol;

  if (hasAvailableMove) {
    for (int row = 0; row < n; row++) {
      for (int col = 0; col < n; col++) {
        if (board[row][col] == 'U') {
          currentScore = compMoveScore(board, n, compColour, row, col);
        }

        if (currentScore > maxScore) {
          maxScore = currentScore;
          bestRow = row;
          bestCol = col;
        }
      }
    }
    board[bestRow][bestCol] = compColour;
    printf("Computer places %c at %c%c.\n", compColour, bestRow + 'a',
           bestCol + 'a');
    flipTiles(board, n, bestRow, bestCol, compColour);
    printBoard(board, n);
  } else {
    *compLegal = false;
  }
  return;
}

// playGame

// determine winner/print winner message
void determineWinner(char board[][26], int n, char humanColour,
                     char compColour) {
  int humanScore = 0, compScore = 0;

  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == humanColour) {
        humanScore++;
      } else {
        compScore++;
      }
    }
  }

  if (humanScore > compScore) {
    printf("%c player wins.\n", humanColour);
  } else if (compScore > humanScore) {
    printf("%c player wins.\n", compColour);
  } else {
    printf("Draw!\n");
  }
}

int main(void) {
  int n;
  char board[26][26];
  char compColour, humanColour;
  bool humanTurn;
  bool humanLegal = true, compLegal = true;
  bool invalid = false;

  printf("Enter the board dimension: ");
  scanf("%d", &n);

  printf("Computer plays (B/W): ");
  scanf(" %c", &compColour);

  // 1. initialize AND print board with initaial starting positions
  initializeBoard(board, n);
  printBoard(board, n);

  // set player colours
  if (compColour == 'B') {
    humanTurn = false;
    humanColour = 'W';
  } else {
    humanTurn = true;
    humanColour = 'B';
  }

  // while loop to keep playing until both moves are invalid
  while (humanLegal || compLegal) {
    // if human is black they go first, otherwise comp will go - bool value will
    // change and game will continue
    if (humanTurn == true) {
      humanMove(board, n, humanColour, &humanLegal, &invalid);
      humanTurn = false;

      // moves available but human makes invalid move
      if (humanLegal == true && invalid == true) {
        printf("Invalid move.\n");
        break;
      }
    } else {
      compMove(board, n, compColour, &compLegal);
      humanTurn = true;
    }

    // check if players have available moves
    if (!humanLegal && !compLegal) {
      // no legal moves for either player, end game
      break;
    }
    // if it was human turn and there are no legal moves available
    else if (!humanTurn && !humanLegal) {
      printf("%c player has no valid move.\n", humanColour);
    }
    // if it was computer turn and there are no legal moves available
    else if (humanTurn && !compLegal) {
      printf("%c player has no valid move.\n", compColour);
    }

    // check if board is full
    if (!availableMoves(board, n, humanColour) &&
        !availableMoves(board, n, compColour)) {
      break;
    }
  }

  determineWinner(board, n, humanColour, compColour);
  return 0;
}
