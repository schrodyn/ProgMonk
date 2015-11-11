#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>

const int SIZE = 6;
const char comp_c = '@';
const char player_c = '0';

void display(char board[][SIZE]);
int valid_moves(char board[][SIZE], bool moves[][SIZE], char player);
void make_move(char board[][SIZE], int row, int col, char player);
void computer_move(char board[][SIZE], bool moves[][SIZE], char player);

int main(void){
	char board[SIZE][SIZE] = { 0 };
	bool moves[SIZE][SIZE] = { false };
	char again;
	int row = 0;
	int col = 0;
	int no_of_games = 0;
	int no_of_moves = 0;
	int invalid_moves = 0;
	int comp_score = 0;
	int user_score = 0;
	char y = 0;
	int x = 0;
	char again = 0;

	bool next_player = true;

	printf("\nREVERSI\n\n");
	printf("You can go first on the first game, then we will take turns.\n");
	printf("   You will be white - (%c)\n   I will be black - (%c).\n",
							player_c, comp_c);
	printf("Select a square for your move by typing a digit for the row\n"
				"and a letter for the column with no spaces between.\n");
	printf("\nGood luck! Press Enter to start.\n");
	scanf("%c", &again);

	
	do{
		next_player != next_player;
		no_of_moves = 4;
		
		for(row = 0; row < SIZE; row++)
			for(col = 0; col < SIZE; col++)
				board[row][col] = ' ';	
		
		int mid = SIZE/2;
		board[mid-1][mid-1] = board[mid][mid] = player_c;
		board[mid-1][mid] = board[mid][mid-1] = comp_c;
		do{
			display(board);
			if(next_player = !next_player){
				if(valid_moves(board, moves, player_c)){
					for(;;){
						printf("Please enter your move (row column): ");
						scanf("%d%c", &x, &y);
						y = tolower(y) - 'a';
						x--;
						if(x>=0 && y>=0 && x<SIZE && y<SIZE &&moves[x][y]){
						  make_move(board, x, y, player_c);
						  no_of_moves++;
						  break;
						}
						else
							printf("Not a valid move, try again.\n");
					}
				}
				else
				  if(++invalid_moves<2){
				    printf("\nYou have to pass, press return");
				    scanf("%c", &again);
				  }
				  else
				    printf("\nNeither of us can go, so the game is over.\n");
			}
			else
			{
				/* it's computer turn */
				if(valid_moves(board, moves, '@'))
				{
					invalid_moves = 0;
					computer_move(board, moves, '@');
					no_of_moves++;
				}
				else
				{
					if(++invalid_moves<2)
						printf("\nI have to pass, your go\n");
					else
						printf("\nNeither of us can go, so the game is over.\n");
				}
			}
		}while(no_of_moves < SIZE*SIZE && invalid_moves < 2);
		
		display(board);

		comp_score = user_score = 0;
		for(row = 0; row < SIZE; row++)
			for(col = 0; col < SIZE; col++)
			{
				comp_score += board[row][col] == comp_c;
				user_score += board[row][col] == player_c;
			}
		printf("The final score is:\n");
		printf("Computer %d\n  User %d\n\n", comp_score, user_score);

		printf("Do you want to play again (y/n): ");
		scanf("%c", &again);
	}while(tolower(again) == 'y');

	printf("\nGoodbye\n");
	return 0;
	return 0;
}

void display(char board[][SIZE]){
	char col_label = 'a';
	printf("\n");
	for(int col = 0; col<SIZE; col++)
		printf("   %c", col_label+col);
	printf("\n");

	for(int row = 0; row < SIZE; row++)
	{
		printf("  +");
		for(int col = 0; col<SIZE; col++)
			printf("---+");
		printf("\n%2d|", row+1);

		for(int col = 0; col<SIZE; col++)
			printf(" %c |", board[row][col]);
		printf("\n");
	}

	printf("  +");
	for(int col = 0; col<SIZE; col++)
		printf("---+");
	printf("\n");
}

int valid_moves(char board[][SIZE], bool moves[][SIZE], char player)
{
	int rowdelta = 0;
	int coldelta = 0;
	int x = 0;
	int y = 0;
	int no_of_moves = 0;

	char opponent = (player == player_c) ? comp_c : player_c;

	for(int row = 0; row < SIZE; row++)
		for(int col = 0; col < SIZE; col++)
			moves[row][col] = false;

	for(int row = 0; row < SIZE; row++)
		for(int col = 0; col < SIZE; col++)
		{
			if(board[row][col] != ' ')
				continue;
			
			for(rowdelta = -1; rowdelta <= 1; rowdelta++)
				for(coldelta = -1; coldelta <= 1; coldelta++)
				{
					if(row + rowdelta < 0 || row + rowdelta >= SIZE ||
					col + coldelta < 0 || col + coldelta >= SIZE ||
								(rowdelta==0 && coldelta==0))
					continue;
					
					if(board[row + rowdelta][col + coldelta] == opponent)
					{
						x = row + rowdelta;
						y = col + coldelta;
						
						for(;;){
							x += rowdelta;
							y += coldelta;
							if(x < 0 || x >= SIZE || y < 0 || y >= SIZE)
								break;
							if(board[x][y] == ' ')
								break;

							if(board[x][y] == player){
								moves[row][col] = true;
								no_of_moves++;
								break;
							}
						}
					}
					
				}
		}
		return no_of_moves;	
}

