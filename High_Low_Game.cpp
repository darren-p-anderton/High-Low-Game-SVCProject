//Program by Darren Anderton
//This program is a game where the user must guess a randomly generated number between 1 and 100

#include<iostream>
#include<fstream>
#include<cstdlib>
#include<time.h>
#include<iomanip>
#include<string>
#include<sstream>

using namespace std;

void GetHonorRoll(ifstream& in_HonorRoll);							//Open Honor Roll File
void WriteHonorRoll(string HonorName_par[10], int HonorScore_par[10], int ScoreCount_par);		//Write Honor Roll Data to File
int GetAnswer();					//Generate random answer, Returns Answer
int GuessingGame(int& CorrectNumber);		//Core Game function where user guesses the answer, Returns Score
bool IsHonorEligible(int MinHonorScore_par, int GameScore_par);		//Returns if the user can enter into Honor Roll

int main()
{
	ifstream in_HonorRoll;
	ofstream out_HonorRoll;
	bool EndPath=0, HonorEligible = 0, Invalid = 1;
	int CorrectNumber, MinHonorScore, MaxHonorScore, ScoreCount = 0, GameScore, HonorScore[10] = {}, i = 0, j = 0;
	string HonorData[20] = {}, HonorName[10] = {} ;
	char Ans;


	GetHonorRoll(in_HonorRoll);
	for (i = 0; i < 20; i++) {					//This reads all lines of data into an array HonorData
		getline(in_HonorRoll, HonorData[i]);
	}
	in_HonorRoll.close();

	for (i = 1; i < 20; i = i + 2) {
		if (HonorData[i] != "") ScoreCount++;		// Determine how many scores are in honor roll
	}

	for (i = 0; i < ScoreCount; i++) {					//This sorts the HonorData into Names and Scores respectively
		HonorName[i] = HonorData[j++];
		stringstream GetScore(HonorData[j++]);			//Stringstream will convert Score string to Score integer
		GetScore >> HonorScore[i];
	}

	cout << "Welcome to the number guessing game.\nYou win if you guess the correct number between 1 and 100\n";

	while (EndPath == 0) {

		if (ScoreCount == 10) {
			MinHonorScore = 1;
			for (i = 0; i < ScoreCount; i++) {
				if (HonorScore[i] > MinHonorScore) MinHonorScore = HonorScore[i];		//Sets the MinHonorScore as the highest number of guesses present on the honor roll
			}
		}
		else MinHonorScore = 100;		// If there are less than 10 high scores, the user must do better than 100 guesses to be added

		CorrectNumber = GetAnswer();	
		GameScore = GuessingGame(CorrectNumber);
		HonorEligible = IsHonorEligible(MinHonorScore, GameScore);

		if (HonorEligible) {
			while (Invalid)
			{
				Invalid = 0;
				cout << "\nWould you be added to the Honor Roll? Please enter Y or N: ";
				cin >> Ans;
				cout << "\n";
				Invalid = (cin.fail() || (!((Ans == 'Y') || (Ans == 'N') || (Ans == 'y') || (Ans == 'n'))));
				if (Invalid) {
					cin.clear();
					cin.ignore();
					cout << "Invalid entry\n";
				}
				else if ((Ans == 'N') || (Ans == 'n')) HonorEligible = 0;
			}
			Invalid = 1;
		}

		cin.clear();
		cin.ignore();

		if (HonorEligible) {	
			if (ScoreCount < 10) j = ScoreCount++;		//This loop determines the index of which HonorRoll score to overwrite
			else {
				MaxHonorScore = 0;
				for (i = 0; i < ScoreCount; i++) {
					if (HonorScore[i] >= MaxHonorScore) j = i, MaxHonorScore = HonorScore[i];
				}
			}
			HonorScore[j] = GameScore;
			cout << "Please enter your name: ";
			getline(cin, HonorName[j]);
		}

		WriteHonorRoll(HonorName, HonorScore, ScoreCount);	//Save current HonorRoll each loop

		cout << "Roll of Honor:\n";
		for (i = 0; i < ScoreCount; i++) {
			cout << HonorName[i] << " with " << HonorScore[i] << " guesses\n";
		}
		cout << endl;
		while (Invalid)						//Determines if user will play again
		{
			Invalid = 0;
			cout << "\nWould you like to play again? Please enter Y or N: ";
			cin >> Ans;
			cout << "\n";
			Invalid = (cin.fail() || (!((Ans == 'Y') || (Ans == 'N') || (Ans == 'y') || (Ans == 'n'))));
			if (Invalid) {
				cin.clear();
				cin.ignore();
				cout << "Invalid entry\n";
			}
			else if ((Ans == 'N') || (Ans == 'n')) EndPath = 1;		//Triggers EndPath if user answers N.
		}
		Invalid = 1;
	}


	return 0;
}

void GetHonorRoll(ifstream& in_HonorRoll) {
	in_HonorRoll.open("C:\\temp\\HonorRoll.txt");
	if (in_HonorRoll.fail())
	{
		cout << "Input file opening failed. \n";
		exit(1);
	}
}

int GetAnswer() {
	//initialize random seed
	srand(time(NULL));
	//generate answer
	return rand() % 100 + 1;
}

int GuessingGame(int& CorrectNumber) {
	int count = 1, difference, answer, prevdiff;
	bool Invalid = 1;

	while (Invalid)
	{
		Invalid = 0;
		cout << "Guess the number: ";
		cin >> answer;
		Invalid = (cin.fail());
			if (Invalid) {
				cin.clear();
				cin.ignore();
				cout << "Invalid entry\n";
			}
	}
	Invalid = 1;

	difference = abs(answer - CorrectNumber);
	if (difference != 0) {
		count++;
		prevdiff = difference;				//Compare current difference to previous guess difference
		while (Invalid)
		{
			Invalid = 0;
			cout << "The answer is not correct, guess again: ";
			cin >> answer;
			Invalid = (cin.fail());
			if (Invalid) {
				cin.clear();
				cin.ignore();
				cout << "Invalid entry\n";
			}
		}
		Invalid = 1;
		difference = abs(answer - CorrectNumber);
		if (difference !=0) {

			for (count = 2; difference != 0; count++) {			//Rather than directly tell the player if the answer is higher or lower, the loop will tell them if they 
				while (Invalid)									// are warmer/colder than their last guess
				{
					if (difference < prevdiff) {					
						cout << "The answer is not correct, but you're warmer... guess again: ";
					}
					else if (difference > prevdiff) {
						cout << "The answer is not correct, and you're colder... guess again: ";
					}
					else {
						cout << "The answer is not correct, and it's the same... guess again: ";
					}

					cin >> answer;
					Invalid = (cin.fail());
					if (Invalid) {
						cin.clear();
						cin.ignore();
						cout << "Invalid entry\n";
					}
				}
				Invalid = 1;
				prevdiff = difference;
				difference = abs(answer - CorrectNumber);
			}
		}
	}
	cout << "Congratulations! You guessed the answer! You're score is " << count << " guesses\n";
	return count;
}

bool IsHonorEligible(int MinHonorScore_par, int GameScore_par) {
	return GameScore_par <= MinHonorScore_par;
}
void WriteHonorRoll(string HonorName_par[10], int HonorScore_par[10], int ScoreCount_par) {

	ofstream out_HonorRoll;
	int i = 0, j = 0;

	out_HonorRoll.open("C:\\temp\\HonorRoll.txt");
	if (out_HonorRoll.fail())
	{
		cout << "Output file opening failed. \n";
		exit(1);
	}

	for (i = 0; i < ScoreCount_par; i++) {
		out_HonorRoll << HonorName_par[i] << endl;
		out_HonorRoll << HonorScore_par[i] << endl;
	}
	out_HonorRoll.close();

}
