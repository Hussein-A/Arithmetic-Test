// Program to test mental arithmetic using given operations with user range and time limit with score.
#include<chrono>
#include"Test.h"
#include"Questions.h"
#include"ErrorHandling.h"
#include<thread>

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

		std::thread test{ &Test::startTest, &currTest, menu };
		std::this_thread::sleep_for(currTest.getTimeLimitSeconds());
		std::cout << "Score is: " << currTest.getScore() << "\n";
		if (!currTest.getSaveFileName().empty()) currTest.saveSettingsAndScore();

		std::terminate();
		
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