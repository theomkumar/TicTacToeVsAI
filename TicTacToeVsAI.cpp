/*
# TIC TAC TOE VS. A.I --> V1.5

-> minimax implemented with alpha-beta pruning
->difficulty level (level 1 == random moves, level 2 == mini max depth 2, level 3 == alpha-beta pruning)

*/
#include<iostream>
#include<ctime>

// ANSI escape codes for colors
#define RED "\033[31m"
#define BLUE "\033[34m"
#define RESET "\033[0m"
#define INFINITY 1e9

void printBoard(char board[3][3]) {
    //top 
    printf("\n\t\t      |     |     \n");

    for (int i = 0; i < 3; i++) {
        printf("\t\t  "); 
        for (int j = 0; j < 3; j++) {
            // Print colored X or O, otherwise print number
            if (board[i][j] == 'X')
                printf(RED " %c " RESET, board[i][j]);  // Red X
            else if (board[i][j] == 'O')
                printf(BLUE " %c " RESET, board[i][j]); // Blue O
            else
                printf(" %c ", board[i][j]); 

            if (j < 2) printf(" | ");
        }
        printf("\n");
        
        //divider
        if (i < 2){
            printf("\t\t _____|_____|____\n");
            printf("\t\t      |     |     \n");
        } 
    }
    //bottom
    printf("\t\t      |     |     \n");
}

void printMenu() {
    //menu
    printf("\n\t**************************************\n");
    printf("\t| T I C -- T A C -- T O E -- G A M E | \n");
    printf("\t**************************************\n");
    printf("\t-> Press 1 to play against human\n");
    printf("\t-> Press 2 to play against computer\n");
    printf("\t-> Press 3 to exit\n\n");
    printf("\tInput: ");
}

void printDifficultyMenu() {
    //menu
    printf("\n\t**************************************\n");
    printf("\t| C H O O S E   D I F F I C U L T Y ! | \n");
    printf("\t**************************************\n");
    printf("\t-> Press 1 for EASY\n");
    printf("\t-> Press 2 for MEDIUM\n");
    printf("\t-> Press 3 for HARD\n\n");

    printf("\tInput: ");
}

void printWinner(char& playerSymbol, bool& isAgainstComputer) {
    if (isAgainstComputer && playerSymbol == 'O') {
        printf("\n\t**************************************\n");
        printf("\t| C O M P U T E R  H A S  W O N ! !  | \n");
        printf("\t**************************************\n");
    }
    else {
        printf("\n\t**************************************\n");
        printf("\t| P L A Y E R  (%c)  H A S  W O N ! ! | \n", playerSymbol);
        printf("\t**************************************\n");
    }
}

void printDrawMessage() {
    printf("\n\t***************************\n");
    printf("\t| I T ' S  A  D R A W !!! | \n");
    printf("\t***************************\n");
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

bool isWinningState(char board[3][3]) {

    for (int i = 0; i < 3; i++){
        //check row
        if (board[i][0] == board[i][1] && (board[i][1] == board[i][2]))
            return true;
        //check col
        if (board[0][i] == board[1][i] && (board[1][i] == board[2][i]))
            return true;
    }
    //check diagonals
    return  (board[0][0] == board[1][1] && board[1][1] == board[2][2]) ||
            (board[0][2] == board[1][1] && board[1][1] == board[2][0]);
}

int evaluateBestMove(char board[3][3], int totalTurn, int isValidChoice[10], int &bestChoice, int &difficultyLevel, int depth, int alpha, int beta) {

    if (isWinningState(board)) {
        return (totalTurn % 2) ? 1 : -1;
    } else if (totalTurn == 9 || (difficultyLevel == 2 && depth > 2)) {
        return 0;
    }

    int score, currBestChoice;

    if (totalTurn % 2) {  // Computer's turn (minimize loss)
        for (int currChoice = 1; currChoice <= 9; currChoice++) {
            if (isValidChoice[currChoice] == 0) continue;
            
            int row = (currChoice - 1) / 3;
            int col = (currChoice - 1) % 3;
            
            char prevVal = board[row][col];

            //mark it as played
            board[row][col] = 'O';
            isValidChoice[currChoice] = 0;

            int currScore = evaluateBestMove(board, totalTurn + 1, isValidChoice,bestChoice, difficultyLevel, depth + 1, alpha, beta);
            
            if (currScore < beta) {
                beta = currScore;
                currBestChoice = currChoice;//updating best choice for the computer 
            }

            // Backtrack
            board[row][col] = prevVal; 
            isValidChoice[currChoice] = 1;
            
            //alpha-beta pruning 
            if (difficultyLevel == 3) {
                if (beta <= alpha) break;
            }
        }
        score = beta;
    } else {  // Human's turn (maximize win)
        for (int currChoice = 1; currChoice <= 9; currChoice++) {
            if (isValidChoice[currChoice] == 0) continue;
            
            int row = (currChoice - 1) / 3;
            int col = (currChoice - 1) % 3;
            char prevVal = board[row][col];
            board[row][col] = 'X';

            isValidChoice[currChoice] = 0;

            int currScore = evaluateBestMove(board, totalTurn + 1, isValidChoice,bestChoice, difficultyLevel, depth + 1, alpha, beta);

            if (currScore > alpha) {
                alpha = currScore;
                currBestChoice = currChoice; //updating best choice for human
            }

             // Backtrack
            board[row][col] = prevVal;             
            isValidChoice[currChoice] = 1;

            //alpha-beta pruning 
            if (difficultyLevel == 3) {
                if (beta <= alpha) break;
            }
        }
        score = alpha;
    }
    bestChoice = currBestChoice;
    return score;
}

int getRandomMove (int isValidChoice[10]){
    int availableMoves[9];
    int size = 0;

    for (int i = 1; i <= 9; i++){
        if (isValidChoice[i]){
            availableMoves[size++] = i;
        }
    }
    int randomIdx = rand() % size;
    return availableMoves[randomIdx];
}

void takeInput(char board[3][3], int& choice, char playerSymbol, int isValidChoice[10],int totalTurn,  bool isAgainstComputer, int &difficultyLevel) {

    //computer input
    if (playerSymbol == 'O' && isAgainstComputer == true) {
        if (difficultyLevel == 1) {//easy -> select any random move
            choice = getRandomMove(isValidChoice);
        }
        else  {
            int depth = 0;
            int alpha = -INFINITY, beta = INFINITY;
            evaluateBestMove(board, totalTurn, isValidChoice, choice, difficultyLevel, depth, alpha, beta);
        }
        clearScreen();
        printf("\n\tComputer(O) chose box number %d ! \n", choice);
    }
    //human INPUT 
    else {
        printf("\n\tPlayer(%c) Turn, Choose your box(1-9)! \n\n", playerSymbol);
        printBoard(board);

        printf("\n\tinput: ");

        std::cin >> choice;

        //Human error handling
        while (choice < 1 || choice > 9 || isValidChoice[choice] == 0) {
            //clearing cin buffer and preventing cin stream fail
            std::cin.clear();
            std::cin.ignore(256, '\n');

            clearScreen();

            printf("\t\t****INVALID INPUT****  \n\tPlease Enter the unused box between (1-9): \n");

            printBoard(board);
            printf("\tINPUT: ");
            std::cin >> choice;
        }

        clearScreen();
        //print choice only for human
        if (!isAgainstComputer)
            printf("\n\tPlayer(%c) chose box number %d ! \n", playerSymbol, choice);
    }
    //marking curr choice as invalid for further use
    isValidChoice[choice] = 0;
    int row = (choice - 1) / 3;
    int col = (choice - 1) % 3;

    board[row][col] = playerSymbol;

    printf("_ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _ _\n");
}
int getDifficultyLevel () {
    printDifficultyMenu();
    bool invalidInput = true;;
    int level;
    std::cin >> level;

    //human input error handling
    while(level < 1 || level > 3){
        std::cin.clear();
        std::cin.ignore(256,'\n');
        
        clearScreen();
        printf("\n\t!!INVALID INPUT!!, Please choose correct option(1-3)!\n");
        printDifficultyMenu();

        std::cin >> level;
    }
    clearScreen();
    return level;
}
void play(bool isAgainstComputer) {
    clearScreen();

    //Initialises new board each time a play is initiated
    int isValidChoice[10] = { 0,1,1,1,1,1,1,1,1,1};
    char board[3][3] = { {'1','2','3'}, {'4','5','6'}, {'7','8','9'} };
    
    //choose difficulty Level if playing against a computer
    int difficultyLevel = 0;

    if (isAgainstComputer)
        difficultyLevel = getDifficultyLevel();
    

    int totalTurn = 0;
    int choice;

    while (totalTurn < 9) {
        //INPUT : player(X) starts first 
        char playerSymbol = (totalTurn % 2 == 0) ? 'X' : 'O';

        takeInput(board, choice, playerSymbol, isValidChoice, totalTurn, isAgainstComputer, difficultyLevel);

        //WIN CONDITION:
        if (isWinningState(board)) {
            clearScreen();
            printWinner(playerSymbol, isAgainstComputer);
            printBoard(board);
            break;
        }
        totalTurn++;
    }

    //DRAW CONDITION:
    if (totalTurn == 9) {
        clearScreen();
        printDrawMessage();
        printBoard(board);
    }

    //return to menu
    printf("\nENTER ANY KEY TO CONTINUE TO MAIN MENU........ ");
    char temp;
    std::cin >> temp;
    clearScreen();
}

void TicTacToe() {
    bool playing = true;

    bool againstHuman = false;
    bool againstComputer = true;

    while (playing) {
        printMenu();

        int options;
        std::cin >> options;

        switch (options) {
            case 1:     play(againstHuman);
                        break;
            case 2:     play(againstComputer);
                        break;
            case 3:     playing = false;
                        break;
            default:    clearScreen();
                        printf("\n\t!!INVALID INPUT!!, Please choose correct option(1-3)!\n");
                        //to handle cin stream fail after invalid input type
                        std::cin.clear();
                        std::cin.ignore(256, '\n');
                        break;
        }
    }
}

int main() {
    //seeding the random number generator
    srand(time(nullptr));

    clearScreen();
    TicTacToe();
    return 0;
}