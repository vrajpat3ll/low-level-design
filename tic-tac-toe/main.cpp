#include <bits/stdc++.h>
using namespace std;

/// @brief basic player class, could be avoided.
/// But, kept in case we want to extend the functionality of a player, eg. player_avatar.
class Player {
    string name;
    char marker;

   public:
    const string getName() const { return name; }
    const char getMarker() const { return marker; }
    Player(string name, char marker) : name(name), marker(marker) {};
};

/// @brief simple board class, mostly a wrapper over a 2D array.
/// Using a faster implementation of placing a move and checking for a win.
class Board {
    vector<vector<char>> board;
    int size;

    unordered_map<int, unordered_map<char, int>> rowCounts, columnCounts;
    unordered_map<string, unordered_map<char, int>> diagonalCounts;

   public:
    Board(int size) {
        this->reset(size);
    }

    const int getSize() const { return this->size; }

    void reset(int size) {
        this->size = size;
        board = vector<vector<char>>(size, vector<char>(size, ' '));
        rowCounts.clear();
        columnCounts.clear();
        diagonalCounts.clear();
    }

    bool place(Player p, int x, int y) {
        assert(x >= 0 and x < this->size);
        assert(y >= 0 and y < this->size);
        // assert(this->board[x][y] == ' ');

        auto marker = p.getMarker();

        if (this->board[x][y] != ' ') {
            cout << "Error: Cannot place at " << x << ", " << y << endl;
            return false;
        }
        this->board[x][y] = marker;
        this->rowCounts[y][marker]++;
        if (this->rowCounts[y][marker] == this->size) return true;
        this->columnCounts[x][marker]++;
        if (this->columnCounts[x][marker] == this->size) return true;

        if (x == y) {
            this->diagonalCounts["forwards"][marker]++;
            if (this->diagonalCounts["forwards"][marker] == this->size) return true;
        }
        if (x + y == this->size - 1) {
            this->diagonalCounts["backwards"][marker]++;
            if (this->diagonalCounts["backwards"][marker] == this->size) return true;
        }

        return false;
    }

    void display() {
        int padding = this->size + 1;

        cout << "+";
        for (int i = 0; i < padding - 1; i++) {
            for (int i = 0; i < padding - 1; i++) {
                cout << "-";
            }
            cout << "+";
        }
        cout << endl;
        for (int i = 0; i < this->size; i++) {
            cout << "|";
            for (int j = 0; j < this->size; j++) {
                cout << setw(padding / 2) << this->board[i][j] << setw(padding / 2) << "|";
            }
            cout << endl;

            cout << "+";
            for (int i = 0; i < padding - 1; i++) {
                for (int i = 0; i < padding - 1; i++) {
                    cout << "-";
                }
                cout << "+";
            }
            cout << endl;
        }
    }
};

class Game {
    Player p1, p2;
    Board board;

   public:
    Game(Player p1, Player p2, Board board) : p1(p1), p2(p2), board(board) {};
    void play() {
        int currentTurn = 0;
        bool gameDone = false;
        int x, y;
        while (!gameDone) {
            if (currentTurn == this->board.getSize() * this->board.getSize()) {
                cout << "It's a DRAW!" << endl;
                return;
            }
            auto currentPlayer = ((currentTurn & 1) == 0 ? p1 : p2);
            cout << "Write x position of marker: ";
            cin >> x;
            cout << "Write y position of marker: ";
            cin >> y;
            if (this->board.place(currentPlayer, x, y)) {
                gameDone = true;
                cout << "Player " << currentPlayer.getName() << " wins!" << endl;
            } else
                currentTurn++;
            this->board.display();
        }
    }
};

int main() {
    auto Ram = Player("Ram", 'o');
    auto Shaam = Player("Shaam", 'x');
    auto board = Board(3);

    auto game = Game(Ram, Shaam, board);

    game.play();
    return 0;
}