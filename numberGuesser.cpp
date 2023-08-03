#include<iostream>
#include<cmath>
#include<cstdlib>
#include<ctime>
#include<cctype>
#include<string>


void printGameRules();

//get user input and validate if it's the correct input
int getUserGuess(int guess, int low, int high);

//if the user guessed it correctly return true else false and decrement guess if it's incorrect
bool checkGuess(int& guess, int secret, int input);

//game is over if out of guesses or user guessed it correctly
bool gameOver(int guess, int secret, bool correctGuess);

//inititalizes the random range and secret number and set the game round
void playGame();

//get user input and returns true if user wants to play game again(y || Y) or returns false(n || N)
bool getYesOrNo(std::string msg);

//if getReplayInput returns true, it calls playGame again to set another round
void numberGuesser();

//generates random numbers from[start,end]
int generateRadomNumber(const int start, const int end);

//returns true if the user wants to provide the range else false
bool userProvideRange(int& low, int& high);

//get range from the user and check if high is bigger than lower
void getRange(int& low, int& high);

int main()
{
	numberGuesser();
	return 0;

}

int generateRadomNumber(const int start, const int end)
{
	// end - start + 1 so that end is included as well
	return { (std::rand() % (end - start + 1)) + start };
}

void numberGuesser()
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));
	printGameRules();
	bool replay{ false };
	do
	{
		playGame();


	} while (getYesOrNo("Do you want to replay(y or n): "));


}

bool getYesOrNo(std::string msg)
{
	bool fail{ false };
	bool wrongChar{ false };
	char userInput{};
	constexpr int CHAR_IGNORE{ 256 };
	do
	{
		std::cout << msg;
		std::cin >> userInput;
		fail = std::cin.fail();

		if (fail)
		{
			std::cout << "Enter a valid character.\n";
			std::cin.clear();
			std::cin.ignore(CHAR_IGNORE, '\n');
		}
		else
		{
			userInput = std::tolower(userInput);
			wrongChar = !(userInput == 'y' || userInput == 'n');
			if (wrongChar)
			{
				std::cout << "Please enter y or n.\n";
				std::cin.ignore(CHAR_IGNORE, '\n');
			}
		}

	} while (fail || wrongChar);
	return userInput == 'y';
}

void playGame()
{
	int range_low{};
	int range_high{};
	bool userRange{ userProvideRange(range_low, range_high) };
	if (!userRange)
	{
		range_low = generateRadomNumber(0, 100);
		//range high should be atleast 20 numbers apart for game to be fun
		range_high = range_low + generateRadomNumber(20, 100);
	}
	const int secret{ generateRadomNumber(range_low, range_high) };
	//number of guesses should be log2(rangeHigh - rangeLow), the most optimized way to calculate a guess (binary search) - 1
	int guess{ static_cast<int> (std::floor(std::log2(range_high - range_low))) };
	int userGuess{};
	bool correctGuess{};
	do
	{
		userGuess = getUserGuess(guess, range_low, range_high);
		correctGuess = checkGuess(guess, secret, userGuess);

	} while (!gameOver(guess, secret, correctGuess));

}


bool gameOver(int guess, int secret, bool correctGuess)
{
	bool remainingGuess{ guess <= 0 };

	if (correctGuess || remainingGuess)
	{
		std::cout << "Game OVER\n"
			<< "The secret number is " << secret << "\n";
		if (correctGuess)
		{
			std::cout << "Congratulations, you guessed it correctly.\n";
		}
		return true;
	}
	else
		return false;

}

bool checkGuess(int& guess, int secret, int input)
{
	if (input == secret)
		return true;
	else if (input > secret)
	{
		std::cout << "Guess is too high.\n";
		--guess;
		return false;
	}
	else
	{
		std::cout << "Guess is too low\n";
		--guess;
		return false;
	}
}

int getUserGuess(int guess, int low, int high)
{
	bool lowFail{ false };
	bool highFail{ false };
	bool cinFail{ false };
	constexpr int CHAR_IGNORE{ 256 };
	int userInput{};
	do
	{
		std::cout << "Guesses remaining: " << guess << '\n'
			<< "Please enter an int from range [" << low << ',' << high << "]: ";
		std::cin >> userInput;
		cinFail = std::cin.fail();
		if (cinFail)
		{
			std::cout << "Please enter a correct number.\n";
			std::cin.clear();
			std::cin.ignore(CHAR_IGNORE, '\n');
		}
		else
		{
			lowFail = userInput < low;
			highFail = userInput > high;
			if (lowFail)
			{
				std::cout << "Please enter a number higher than or equal to " << low << ".\n";
			}
			else if (highFail)
			{
				std::cout << "Please enter a number lower than or equal to " << high << ".\n";
			}
		}

	} while (cinFail || lowFail || highFail);
	return userInput;
}

void getRange(int& low, int& high)
{
	bool lowerCinFail{ false };
	int lower{};
	constexpr int IGNORE_CHARS{ 256 };
	do
	{
		std::cout << "Please enter the lower limit: ";
		std::cin >> lower;
		lowerCinFail = std::cin.fail();
		if (lowerCinFail)
		{
			std::cin.clear();
			std::cin.ignore(IGNORE_CHARS, '\n');
			std::cout << "Please enter a valid int.\n";
		 }
	} while (lowerCinFail);
	low = lower;
	bool upperCinFail{ false };
	int upper{};
	bool upperLessLower{ false };
	do
	{
		std::cout << "Please enter the upper limit: ";
		std::cin >> upper;
		upperCinFail = std::cin.fail();
		if (upperCinFail)
		{
			std::cin.clear();
			std::cin.ignore(IGNORE_CHARS, '\n');
			std::cout << "Please enter a valid int.\n";

		}
		else
		{
			upperLessLower = upper < lower;
			if (upperLessLower)
			{
				std::cout << "Please enter the upper limit that's bigger than lower.\n";
			}
		}

	} while (upperCinFail || upperLessLower);
	high = upper;
}


bool userProvideRange(int& low, int& high)
{
	bool userRange{ getYesOrNo("Do you want to provide a range(y or n): ") };
	if (!userRange)
		return false;
	else
	{
		getRange(low, high);
		return true;
	}
}

void printGameRules()
{
	std::cout << "Welcome to number guesser. You have to guess a number in the range that computer randomly picks. You can provide the range or let computer randomly picks it for you. The number will be within the range[x,y] and inclusive i.e. it can be x or y as well. You will have limited guesses so pick carefully.\n\n";
}
