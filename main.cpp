#include <iostream>
#include <vector>
#define BLACK "\033[30m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define PURPUR "\033[35m"
#define LIGHT_BLUE "\033[36m"
#define WHITE "\033[37m"

#define BLACK_BG "\033[40m"
#define RED_BG "\033[41m"
#define GREEN_BG "\033[42m"
#define YELLOW_BG "\033[43m"
#define BLUE_BG "\033[44m"
#define PURPUR_BG "\033[45m"
#define LIGHT_BLUE_BG "\033[26m"
#define WHITE_BG "\033[47m"

using namespace std;

class Cell {
private:
	int number;
	bool hasBomb, _isOpen;

public:
	Cell(int number, bool hasBomb) : hasBomb(hasBomb), number(number), _isOpen(false) {

	}

	int getNumber() {
		return number;
	}

	bool isBomb() {
		return hasBomb;
	}

	bool isOpen() {
		
		return _isOpen;
	}

	void setNumber(int num) {
		number = num;
	}

	void setBomb(bool state) {
		hasBomb = state;
	}

	void open(int& openedCells) {
		if (!_isOpen) openedCells++;
		_isOpen = true;
	}
};

class Board {
private:
	vector<vector<Cell>> playground;
	const int bombs;
	const int rows, columns;
	bool isDead, firstMove;
	int openedCells;

public:
	Board(int rows, int columns, int bombs): rows(rows), columns(columns), bombs(bombs), isDead(false), playground(rows, vector<Cell>(columns, Cell(0, false))), openedCells(0), firstMove(true)
	{
		genPlayground();
	}

	void genPlayground() {
		for (int i = 0; i < bombs; i++)
		{
			int x = rand() % rows;
			int y = rand() % columns;

			while (playground[x][y].isBomb())
			{
				x = rand() % rows;
				y = rand() % columns;
			}
			playground[x][y].setBomb(true);
			playground[x][y].setNumber(9);
		}

		for (int x = 0; x < rows; x++)
		{
			for (int y = 0; y < columns; y++)
			{
				int num = 0;
				if (playground[x][y].isBomb()) continue;

				for (int i = -1; i <= 1; i++)
					for (int j = -1; j <= 1; j++)
						if (x + i >= 0 && x + i < rows && y + j >= 0 && y + j < columns && playground[x + i][y + j].isBomb())
							num++;

				playground[x][y].setNumber(num);
			}
		}
	}

	void openCells(int x, int y) {
		if (x < 0 || x >= rows || y < 0 || y >= columns) return; // Check boundary
		Cell& cell = playground[x][y];
		if (cell.getNumber() == 0 && !cell.isOpen()) {
			cell.open(openedCells);
			for (int i = -1; i <= 1; i++) {
				for (int j = -1; j <= 1; j++) {
					if (i == 0 && j == 0) continue;
					openCells(x + i, y + j);
				}
			}
		}
		else if (cell.getNumber() != 9 && !cell.isOpen()) {
			cell.open(openedCells);
		}
	}

	void openBombs() {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < columns; j++)
			{
				if (playground[i][j].isBomb())
					playground[i][j].open(openedCells);
			}
		}
	}

	int open(int x, int y) 
	{
		if (x < 0 || y < 0 || x >= rows || y >= columns)
			return -1;
		Cell& cell = playground[x][y];

		if (cell.isBomb()) 
		{
			isDead = true;
			openBombs();
			return -2;
		}
		else if (cell.isOpen())
		{
			return 1;
		}
		else
		{
			openCells(x, y);
		}
		if (openedCells == columns * rows - bombs && !isDead) {
			return 2;
		}
		return 0;
	}

	void draw(bool __isOpen, string text) 
	{
		system("cls");
		cout << "    0   1   2   3   4   5   6   7   8   9\n"
			 << " ----------------------------------------\n";
		for (int i = 0; i < rows; i++)
		{
			if ( i < 10) cout << " " << i << "| ";
			else cout << i << "| ";
			for (int j = 0; j < columns; j++)
			{
				if (__isOpen || playground[i][j].isOpen())
				{
					switch (playground[i][j].getNumber())
					{
					case 0:
						cout << WHITE_BG << WHITE << (char)254u << BLACK_BG << WHITE;
						break;
					case 1:
						cout << LIGHT_BLUE << 1 << WHITE;
						break;
					case 2:
						cout << GREEN << 2 << WHITE;
						break;
					case 3:
						cout << RED << 3 << WHITE;
						break;
					case 4:
						cout << BLUE << 4 << WHITE;
						break;
					case 5:
						cout << PURPUR << 5 << WHITE;
						break;
					case 6:
						cout << WHITE_BG << BLACK << 6 << BLACK_BG << WHITE;
						break;
					case 7:
						cout << WHITE_BG << BLACK << 7 << BLACK_BG << WHITE;
						break;
					case 8:
						cout << BLUE << BLACK << 8 << BLACK_BG << WHITE;
						break;
					case 9:
						cout << RED_BG << BLACK << "X" << BLACK_BG << WHITE;
						break;
					default:
						cout << playground[i][j].getNumber();
						break;
					}
				}
				else {
					cout << (char)254u;
				}
				cout << "   ";
			}
			cout << "\n  |\n";
		}
		cout << text;
	}
};

int main() { 
	srand(time(NULL));
	Board board(10, 10, 20);
	bool run = 1;
	board.draw(true, "");
	string text;
	while (run)
	{
		board.draw(false, text);
		text = "";
		int x;
		int y;
		std::cout << "\nEnter move (x y) : ";
		std::cin >> y;
		std::cin >> x;
		if (x == -1 && y == -1)
		{
			run = 0;
			break;
		}
		int res = board.open(x, y);
		if (res != 0)
		{
			switch (res)
			{
			case -1:
				text = "Incorrect input!";
				break;
			case -2:
				board.draw(false, RED + string("Game Over!") + WHITE);
				run = 0;
				break;
			case 1:
				text = "Cell now opened!";
				break;
			case 2:
				text = GREEN + string("You win!");
				board.draw(true, text);
				run = 0;
				break;
			}

		}
	}
}