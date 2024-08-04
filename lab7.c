#include <stdbool.h>
#include <stdio.h>

void initializeBoard(char board[][26], int n);
void printBoard(char board[][26], int n);
void availableMoves(char board[][26], int n, char colour);

bool positionInBounds(int n, int row, int col);
bool checkLegalInDirection(char board[][26], int n, int row, int col,
                           char colour, int deltaRow, int deltaCol);
void checkIfValidInput(char board[][26], int n, int row, int col, char colour);
void flipTiles(char board[][26], int n, int row, int col, char colour);

int main(void) {
  int n;
  char board[26][26];

  printf("Enter the board dimension: ");
  scanf("%d", &n);

  // 1. initialize AND print board with initaial starting positions
  initializeBoard(board, n);
  printBoard(board, n);

  // 2. board configuration, 3 inputs (colour, row, col), keep taking input
  // until !!!
  char colour, row, col;
  printf("Enter board configuration: \n");
  do {
    scanf(" %c%c%c", &colour, &row, &col);
    if (colour != '!' && row != '!' && col != '!') {
      board[row - 'a'][col - 'a'] = colour;
    }
  } while (colour != '!' && row != '!' && col != '!');
  printBoard(board, n);

  // 3. print available moves for White and Black player
  availableMoves(board, n, 'W');
  availableMoves(board, n, 'B');

  // 4+5. Input move, validate it, flip tile accordingly, and print updated
  // board
  printf("Enter a move:\n");
  scanf(" %c%c%c", &colour, &row, &col);
  checkIfValidInput(board, n, row - 'a', col - 'a', colour);

  return 0;
}

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

void availableMoves(char board[][26], int n, char colour) {
  printf("Available moves for %c:\n", colour);

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
            printf("%c%c\n", row + 'a', col + 'a');
            dir = 7;
          }
        }
      }
    }
  }
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
void checkIfValidInput(char board[][26], int n, int row, int col, char colour) {
  int deltaRow[] = {-1, -1, -1, 0, 0, 1, 1, 1};
  int deltaCol[] = {-1, 0, 1, -1, 1, -1, 0, 1};

  if (positionInBounds(n, row, col) && board[row][col] == 'U') {
    for (int dir = 0; dir < 8; dir++) {
      if (checkLegalInDirection(board, n, row, col, colour, deltaRow[dir],
                                deltaCol[dir])) {
        board[row][col] = colour;

        printf("Valid move.\n");
        flipTiles(board, n, row, col, colour);
        printBoard(board, n);
        return;
      }
    }
    printf("Invalid move.\n");
    printBoard(board, n);
    return;
  } else {
    printf("Invalid move.\n");
    printBoard(board, n);
    return;
  }
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
        printf("\n%d\n", dir);
        board[row + (deltaRow[dir] * i)][col + (deltaCol[dir] * i)] = colour;
        i++;
      }
      i = 1;
    }
  }
}
