// Program to test mental arithmetic using given operations with user range and time limit with score.
#include<chrono>
#include"Test.h"
#include"Questions.h"
#include"ErrorHandling.h"


int main() {
	try{
		TestUI menu;
		Test currTest;
		std::cout << "Do you have a settings file? (y) or (n)\n";
		if (menu.getYesNo()) {
			if (!menu.getFileSettings(currTest)) menu.getUserOps(currTest); //user refused saved settings for custom settings
		}
		else
			menu.getUserOps(currTest);

		if (currTest.getQuestionVec().size() == 0) {
			std::cout << "No opertions entered. Closing program. \n";
			return 0;
		}


		//for question selection
		std::random_device rd;
		std::mt19937 rng(rd());
		while (true) {
			//main loop to randomly pick what operations to give
			currTest.setStartTime();
			std::uniform_int_distribution<int> pickVectorIndex(0, currTest.getQuestionVec().size()-1);
			while (!currTest.isPastTimeLimit()) {
				ArithmeticQuestion& currQuestion = *(currTest.getQuestionVec()[pickVectorIndex(rng)]);
				std::cout << currQuestion.getRandQuestion();
				//cout << currQuestion.getSoln() << "\n" << currQuestion.getRange().a() << "\n";
				if (menu.getNum() != currQuestion.getSoln()) {
					currTest.addToScore(currQuestion.incorrectAnsPoints);
					currTest.incorrectAnsMsg( currQuestion.incorrectAnsPoints );
				}
				else {
					currTest.addToScore(currQuestion.correctAnsPoints);
					currTest.correctAnsMsg(currQuestion.correctAnsPoints);
				}

			}

			std::cout << "Score is: " << currTest.getScore() << "\n";
			if (!currTest.getSaveFileName().empty()) currTest.saveSettingsAndScore();

			std::cout << "Try again? (y) or (n) \n";
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //seems to be an issue where a newline is entered here. This is a temp fix for now.
			if (!menu.getYesNo()) break;
		}
		
	}
	
	catch (std::runtime_error& e) {
		std::cerr << "Runtime error: " << e.what() << "\n";
		return 1;
	}
	catch (...) {
		std::cerr << "Exception: something went wrong. \n";
		return 2;
	}
}