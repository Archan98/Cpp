#include <iostream>
#include <fstream>
#include <string>
#include <map>
using namespace std;

class Player {
public:
    string username;
    string password;
    int wins;
    int losses;
    int draws;

    Player() : username(""), password(""), wins(0), losses(0), draws(0) {}
};

// ANSI Colors
const string RESET = "\033[0m";
const string GREEN = "\033[32m";
const string RED = "\033[31m";
const string YELLOW = "\033[33m";

// Load player data from file
map<string, Player> loadPlayers() {
    map<string, Player> players;
    ifstream file("players.txt");
    if (file) {
        Player p;
        while (file >> p.username >> p.password >> p.wins >> p.losses >> p.draws) {
            players[p.username] = p;
        }
    }
    return players;
}

// Save player data to file
void savePlayers(map<string, Player>& players) {
    ofstream file("players.txt");
    for (map<string, Player>::iterator it = players.begin(); it != players.end(); ++it) {
        Player p = it->second;
        file << p.username << " " << p.password << " "
             << p.wins << " " << p.losses << " " << p.draws << "\n";
    }
}

// Print the Tic Tac Toe board
void printBoard(char board[3][3]) {
    cout << "\n";
    for (int i = 0; i < 3; i++) {
        cout << " ";
        for (int j = 0; j < 3; j++) {
            cout << board[i][j];
            if (j < 2) cout << " | ";
        }
        cout << "\n";
        if (i < 2) cout << "---|---|---\n";
    }
    cout << "\n";
}

// Check for winner
char checkWinner(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] && board[i][1] == board[i][2])
            return board[i][0];
        if (board[0][i] == board[1][i] && board[1][i] == board[2][i])
            return board[0][i];
    }
    if (board[0][0] == board[1][1] && board[1][1] == board[2][2])
        return board[0][0];
    if (board[0][2] == board[1][1] && board[1][1] == board[2][0])
        return board[0][2];
    return ' ';
}

// Main game loop
void playGame(Player &p1, Player &p2) {
    char board[3][3];
    int moveCount = 0;
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            board[i][j] = '1' + i * 3 + j;

    char current = 'X';
    Player* currentPlayer = &p1;
    Player* otherPlayer = &p2;

    while (true) {
        printBoard(board);
        int choice;
        cout << currentPlayer->username << " (" << current << "), choose position (1-9): ";
        cin >> choice;

        if (choice < 1 || choice > 9) {
            cout << "Invalid move! Try again.\n";
            continue;
        }

        int row = (choice - 1) / 3;
        int col = (choice - 1) % 3;

        if (board[row][col] == 'X' || board[row][col] == 'O') {
            cout << "Position already taken! Try again.\n";
            continue;
        }

        board[row][col] = current;
        moveCount++;

        char winner = checkWinner(board);
        if (winner == 'X' || winner == 'O') {
            printBoard(board);
            cout << GREEN << currentPlayer->username << " wins! ;)" << RESET << "\n";
            currentPlayer->wins++;
            otherPlayer->losses++;
            break;
        }

        if (moveCount == 9) {
            printBoard(board);
            cout << YELLOW << "It's a draw!" << RESET << "\n";
            p1.draws++;
            p2.draws++;
            break;
        }

        // Switch turns
        current = (current == 'X') ? 'O' : 'X';
        Player* temp = currentPlayer;
        currentPlayer = otherPlayer;
        otherPlayer = temp;
    }
}

// Show a progress bar
void showProgressBar(string label, int value, int total, string color) {
    cout << label << " ";
    int barWidth = 20; // size of the bar
    int filled = (total == 0) ? 0 : (value * barWidth / total);

    cout << "[";
    for (int i = 0; i < barWidth; i++) {
        if (i < filled) cout << color << "#" << RESET;
        else cout << "-";
    }
    cout << "] " << (total == 0 ? 0 : (value * 100 / total)) << "%\n";
}

// Display stats of a player with progress bars
void viewStats(Player &p) {
    int totalGames = p.wins + p.losses + p.draws;
    cout << "\n--- Stats for " << p.username << " ---\n";
    cout << "Games Played: " << totalGames << "\n";
    cout << "Wins   : " << p.wins << "\n";
    cout << "Losses : " << p.losses << "\n";
    cout << "Draws  : " << p.draws << "\n\n";

    showProgressBar("Wins  ", p.wins, totalGames, GREEN);
    showProgressBar("Losses", p.losses, totalGames, RED);
    showProgressBar("Draws ", p.draws, totalGames, YELLOW);

    cout << "----------------------\n\n";
}

int main() {
    map<string, Player> players = loadPlayers();

    cout << "==== WELCOME TO TIC TAC TOE GAME BY TEAM INFINITE LOOPERS ====\n";

    // Login system
    Player p1, p2;
    for (int i = 1; i <= 2; i++) {
        string user, pass;
        while (true) {
            cout << "Player " << i << " username: ";
            cin >> user;

            // prevent same username for Player1 and Player2
            if (i == 2 && user == p1.username) {
                cout << RED << "This username is already taken by Player 1. Choose another.\n" << RESET;
                continue;
            }

            cout << "Password: ";
            cin >> pass;

            if (players.count(user) && players[user].password == pass) {
                cout << GREEN << "Welcome back, " << user << "!\n" << RESET;
                (i == 1 ? p1 : p2) = players[user];
            } else if (players.count(user)) {
                cout << RED << "Wrong password! Try again.\n" << RESET;
                continue; // retry login
            } else {
                cout << YELLOW << "New player registered: " << user << RESET << "\n";
                Player newP;
                newP.username = user;
                newP.password = pass;
                players[user] = newP;
                (i == 1 ? p1 : p2) = newP;
            }
            break;
        }
    }

    // Menu loop
    int choice;
    do {
        cout << "\n===== Main Menu =====\n";
        cout << "1. Play Game\n";
        cout << "2. View Player 1 Stats\n";
        cout << "3. View Player 2 Stats\n";
        cout << "4. Logout\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                playGame(p1, p2);
                break;
            case 2:
                viewStats(p1);
                break;
            case 3:
                viewStats(p2);
                break;
            case 4:
                cout << "\nLogging out...\n";
                break;
            default:
                cout << "Invalid choice, try again.\n";
        }

        // Save progress after each action
        players[p1.username] = p1;
        players[p2.username] = p2;
        savePlayers(players);

    } while (choice != 4);

    cout << "\nThanks for playing!\n";
    return 0;
}
