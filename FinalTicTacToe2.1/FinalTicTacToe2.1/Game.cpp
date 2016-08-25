/// TIC TAC TOE  2

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include<sstream>
#include<fstream>

using namespace std;
void ShowBoard(vector< vector<char> > gameBoard);
int  WinCheckGame(vector < vector <char>> gameBoard, int winSequence, int turn, int column, int row);


void loadGame(bool &isGameSaved, vector<char> player, int &turn, int &gameBoardSize, int &seqnumber, int &playernumber, vector < vector <char>>&gameBoard, int &usedSpace, vector<int>&inputSequence, vector<int>&intervalVect) {
	ifstream  file;
	file.open("GameData.data", ios::in | ios::binary);
	if (file.is_open())
	{
		file.read((char*)&isGameSaved, sizeof(isGameSaved));
		file.read((char*)&gameBoardSize, sizeof(gameBoardSize));
		file.read((char*)&seqnumber, sizeof(seqnumber));
		file.read((char*)&playernumber, sizeof(playernumber));
		file.read((char*)&usedSpace, sizeof(usedSpace));

		inputSequence.resize((gameBoardSize * gameBoardSize), -1);
		intervalVect.resize((gameBoardSize * gameBoardSize), -1);
		file.read(reinterpret_cast<char*>(&inputSequence[0]), inputSequence.size() * sizeof(inputSequence[0]));
		file.read(reinterpret_cast<char*>(&intervalVect[0]), intervalVect.size() * sizeof(intervalVect[0]));

		char temp;
		for (int x = 0; x < gameBoardSize; x++) {
			gameBoard.push_back(vector<char>());
			for (int y = 0; y < gameBoardSize; y++) {
				file.read((char*)&temp, sizeof(char));
				gameBoard[x].push_back(temp);
			}
		}
		file.close();
	}

	vector< vector<char> > tempBoard(gameBoardSize, vector<char>(gameBoardSize, -1));
	for (int i = 0; i < usedSpace; i++)
	{
		int row = inputSequence[i] / gameBoardSize;
		int col = inputSequence[i] % gameBoardSize;
		tempBoard[row][col] = gameBoard[row][col];
		clock_t begin_time = clock();
		cout << "\nPlayer " << player[turn] << " turn...." << endl;
		while (clock() - begin_time < intervalVect[i])
		{
		}
		ShowBoard(tempBoard);
		if (turn >= playernumber - 1)
			turn = 0;
		else
			turn++;
	}
}

void saveGame(bool isGameSaved, int gameBoardSize, int seqnumber, int playernumber, vector < vector <char>>gameBoard, int usedSpace, vector<int>inputSequence, vector<int>intervalVect) {

	ofstream file;
	file.open("GameData.data", ios::out | ios::trunc | ios::binary);

	file.write((char*)&isGameSaved, sizeof(isGameSaved));
	file.write((char*)&gameBoardSize, sizeof(gameBoardSize));
	file.write((char*)&seqnumber, sizeof(seqnumber));
	file.write((char*)&playernumber, sizeof(playernumber));
	file.write((char*)&usedSpace, sizeof(usedSpace));

	file.write((char*)&inputSequence[0], inputSequence.size() * sizeof(int));
	file.write((char*)&intervalVect[0], intervalVect.size() * sizeof(int));

	for (size_t b = 0; b < size_t(gameBoardSize); b++) {
		file.write((char*)&gameBoard[b][0], gameBoard[b].size()*sizeof(char));
	}
	file.close();
}



bool askUserForValue(int &row, int &column, char player, int gameBoardSize, bool isGameSaved, int seqnumber, int playernumber, vector < vector <char>>gameBoard, int usedSpace, vector<int>inputSequence, vector<int>intervalVect)
{
	bool isSave = false;
	bool isValidInput = true;
	while (1) {
		isValidInput = true;
		cout << "\nEnter row and column seperated by space for Player " << player << " or 'y' to save and exit " << " : ";

		string input;
		cin >> input;

		if (input == "y" || input == "Y")
		{
			cin.clear();
			saveGame(isGameSaved, gameBoardSize, seqnumber, playernumber, gameBoard, usedSpace, inputSequence, intervalVect);
			isSave = true;
			break;
		}
		else
		{
			for (int i = 0; i<static_cast<int>(input.length()); i++)
			{
				if (input.at(i)<'0' || input.at(i)>'9')
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					isValidInput = false;
					break;
				}
			}
			if (isValidInput == false)
			{
				cout << "Row is invalid. Please enter both values again." << endl;
				continue;
			}
			else
			{
				row = atoi(input.c_str());
				cin >> column;
				if (cin.fail())
				{
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					cout << "Column is invalid. Please enter both values again." << endl;
					continue;
				}
				else
				{
					if (!((row >= 1 && row <= gameBoardSize) && (column >= 1 && column <= gameBoardSize)))
					{
						cin.clear();
						cin.ignore(numeric_limits<streamsize>::max(), '\n');
						cout << "Invalid inputs. Please enter between 1 to " << gameBoardSize << ". Please enter both values again." << endl;
						continue;
					}
					if (!cin.fail())
						break;
				}
			}
		}
	}
	return isSave;
}


void ShowBoard(vector< vector<char> > gameBoard)
{
	cout << endl;
	for (int i = 0; i < int(gameBoard.size()); i++)
	{
		cout << setw(3) << i + 1 << "  ";
	}
	cout << endl;
	for (int i = 0; i < int(gameBoard.size()); i++)
	{
		cout << setw(2);
		cout << i + 1;
		for (int j = 0; j < int(gameBoard.size()); j++)
		{
			if (j != 0)
				cout << setw(2) << " |" << " ";
			cout << " " << gameBoard[i][j];

		}
		cout << endl;
		if (i < int(gameBoard.size()) - 1)
		{
			cout << setw(8);
			for (int j = 0; j < int(gameBoard.size()) - 1; j++)
			{
				cout << "--+--";
			}
		}
		cout << endl;
	}
}
int WinCheckGame(vector< vector <char>> gameBoard, int winSequence, int turn, int column, int row)
{
	int k, temprow = row, tempcolumn = column;
	vector <int>Win(26);
	vector<char> player = { 'X', 'O', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'Y', 'Z' };

	for (k = 0; k < int(gameBoard.size()); k++)
	{
		if (player[turn] == gameBoard[row][k])
		{
			Win[turn] = Win[turn] + 1;
			if (Win[turn] >= winSequence)
				return turn;
		}
		else
		{
			Win[turn] = 0;
		}
	}
	Win[turn] = 0;
	for (k = 0; k < int(gameBoard.size()); k++)
	{
		if (player[turn] == gameBoard[k][column])
		{
			Win[turn]++;
			if (Win[turn] >= winSequence)
				return turn;
		}
		else
		{
			Win[turn] = 0;
		}
	}
	Win[turn] = 0;
	while (temprow >0 && tempcolumn > 0)
	{
		temprow--;
		tempcolumn--;

	}
	while (temprow < int(gameBoard.size()) && tempcolumn < int(gameBoard[row].size()))
	{
		if (player[turn] == gameBoard[temprow][tempcolumn])
		{
			Win[turn]++;
			if (Win[turn] >= winSequence)
				return turn;
		}
		else
		{
			Win[turn] = 0;
		}
		temprow++;
		tempcolumn++;
	}
	Win[turn] = 0;

	temprow = row;
	tempcolumn = column;
	while (temprow < int(gameBoard.size()) - 1 && tempcolumn >0)
	{
		temprow++;
		tempcolumn--;
	}


	while (temprow >= 0 && tempcolumn < int(gameBoard[0].size()))
	{

		if (player[turn] == gameBoard[temprow][tempcolumn])
		{
			Win[turn]++;
			if (Win[turn] >= winSequence)
				return turn;
		}
		else
		{
			Win[turn] = 0;
		}
		temprow--;
		tempcolumn++;
	}
	Win[turn] = 0;

	return -1;
}

void playgame(int &row, int &column, vector<char> player, int &turn, int gameBoardSize, bool isGameSaved, int seqnumber, int playernumber, vector < vector <char>>&gameBoard, int &usedSpace, vector<int>&inputSequence, vector<int>&intervalVect) {

	bool isSaveMode = false;
	int result;
	while (true)
	{
		clock_t begin_time = clock();
	step4:
		isSaveMode = askUserForValue(row, column, player[turn], gameBoardSize, isGameSaved, seqnumber, playernumber, gameBoard, usedSpace, inputSequence, intervalVect);
		if (isSaveMode == false) {
			if (gameBoard[row - 1][column - 1] != ' ') {
				cout << "selected position is already used. Please select another position. " << endl;
				goto step4;
			}
			else {
				gameBoard[row - 1][column - 1] = player[turn];
				inputSequence[usedSpace] = gameBoardSize * (row - 1) + (column - 1);;
				intervalVect[usedSpace] = clock() - begin_time;
				usedSpace++;
			}

			ShowBoard(gameBoard);
			result = WinCheckGame(gameBoard, seqnumber, turn, (column - 1), (row - 1));

			if (result != -1) {
				cout << "The winner is player " << player[turn] << "." << endl;
				cout << "The Game is over!" << endl;
				cout << endl << endl;
				ofstream file;
				file.open("GameData.data", ios::out | ios::trunc | ios::binary);
				file.close();
				break;
			}
			if (usedSpace == gameBoardSize*gameBoardSize) {
				cout << "Game is withdraw!" << endl;
				cout << "Game is over!" << endl;
				cout << endl << endl;
				ofstream file;
				file.open("GameData.data", ios::out | ios::trunc | ios::binary);
				file.close();
				break;
			}

			if (turn >= playernumber - 1)
				turn = 0;
			else
				turn++;
		}
		else {
			cout << "\nGame is stored." << endl;
			break;
		}
	}
}


void newGame(int &row, int &column, vector<char> player, int &turn, int &gameBoardSize, bool &isGameSaved, int &seqnumber, int &playernumber, vector < vector <char>>&gameBoard, int &usedSpace, vector<int>&inputSequence, vector<int>&intervalVect)
{

step1:
	cout << "\nHow many players are playing ?";
	cin >> playernumber;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Player Number is invalid. Please enter value again." << endl;
		goto step1;
	}
	else if (!(playernumber <= 26 && playernumber > 1))
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Player Number is invalid. Please enter value again." << endl;
		goto step1;
	}
step2:
	cout << "\nWhat board size u want?";
	cin >> gameBoardSize;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Board size is invalid. Please enter value again." << endl;
		goto step2;
	}
	else if (gameBoardSize < 1)
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Board size is invalid. Please enter value again." << endl;
		goto step2;
	}


step3:
	cout << "\nWhat the win sequence count should be(remember to choose 3 or more) ?";
	cin >> seqnumber;
	if (cin.fail())
	{
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Sequence count is invalid. Please enter value again." << endl;
		goto step3;
	}
	if (seqnumber <= 2 || seqnumber > gameBoardSize)
	{
		cout << "choose the sequence number larger than 3 and equal or less than " << gameBoardSize << endl;
		goto step3;
	}

	if ((gameBoardSize*gameBoardSize) < (playernumber * seqnumber))
	{
		cout << "To play this game correctly, choose a higher number." << endl;
		goto step2;
	}

	for (int i = 0; i < gameBoardSize; i++)	{
		gameBoard.push_back(vector<char>());
		for (int j = 0; j < gameBoardSize; j++) {
			gameBoard[i].push_back(' ');
		}
	}

	isGameSaved = true;

	intervalVect.resize((gameBoardSize*gameBoardSize), -1);
	inputSequence.resize((gameBoardSize*gameBoardSize), -1);
	ShowBoard(gameBoard);
}

int main() {
	int playernumber = 0;
	int seqnumber = 0;
	int row, column = -1;
	int turn = 0;
	int gameBoardSize = 0;
	int usedSpace = 0;
	bool isGameSaved = false;
	vector<char> player = { 'X', 'O', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'Y', 'Z' };
	vector< vector<char>> gameBoard;
	vector<int> intervalVect;
	vector<int> inputSequence;
	bool isSaveMode = false;
	string input = "";

	cout << "Welcome to Tic-Tac-Toe2 game." << endl;

	ifstream  file;
	file.open("GameData.data", ios::in | ios::binary);
	if (file.is_open() == true) {
		file.read((char*)&isGameSaved, sizeof(isGameSaved));
		file.close();
		if (isGameSaved == true) {
		GameSelection:
			cout << "\nDo you want to play saved game?(y/n) : ";
			cin >> input;

			if (input == "y" || input == "Y") {
				cout << "\nSaved game loading...." << endl;
				loadGame(isGameSaved, player, turn, gameBoardSize, seqnumber, playernumber, gameBoard, usedSpace, inputSequence, intervalVect);
			}
			else if (input == "n" || input == "N") {
				cout << "\nPlay a new game...." << endl;
				newGame(row, column, player, turn, gameBoardSize, isGameSaved, seqnumber, playernumber, gameBoard, usedSpace, inputSequence, intervalVect);
			}
			else {
				cout << "Invalid input. Please enter your choice (y/n) again" << endl;
				goto GameSelection;
			}
		}
		else {
			cout << "\nNo saved game. Please start new game" << endl;
			newGame(row, column, player, turn, gameBoardSize, isGameSaved, seqnumber, playernumber, gameBoard, usedSpace, inputSequence, intervalVect);
		}
	}
	else {
		cout << "\nNo saved game. Please start new game" << endl;
		newGame(row, column, player, turn, gameBoardSize, isGameSaved, seqnumber, playernumber, gameBoard, usedSpace, inputSequence, intervalVect);
	}
	playgame(row, column, player, turn, gameBoardSize, isGameSaved, seqnumber, playernumber, gameBoard, usedSpace, inputSequence, intervalVect);
	
}





