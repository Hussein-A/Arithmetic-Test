#include"Test.h"
#include "ErrorHandling.h"

bool Test::isPastTimeLimit() const {
	return std::chrono::duration_cast<std::chrono::seconds> (std::chrono::high_resolution_clock::now() - this->startTime).count() > this->getTimeLimitSeconds().count();
}

//File saving settings
bool Test::saveSettingsAndScore() {
	if (this->getSaveFileName().empty()) return false;

	std::ofstream ost{ this->getSaveFileName(), std::ios_base::app };
	if (!ost) error("Cannot open file for writing! File Tampered after entry! \n");
	ost << *this; //see << operator overload above for the format of record keeping
	return true;
}

std::ostream& operator<<(std::ostream& ost, const Test& test) {

	char buffer[32];
	struct tm timeinfo;
	time_t currTime{ time(NULL) };
	localtime_s(&timeinfo, &currTime);
	asctime_s(buffer, 32, &timeinfo);
	ost << "score: " << test.getScore() << '\n' << "date: " << buffer;
	for (const std::unique_ptr<ArithmeticQuestion>& q : test.getQuestionVec()) {
		ost << q->getOp() << ": " << "( " << q->getRange().a() << " , " << q->getRange().b() << " )" << '\n';
	}
	ost << "time_limit: " << test.getTimeLimitSeconds().count() << "\n\n";
	return ost;

}

std::istringstream& operator>> (std::istringstream& iss, std::vector<std::unique_ptr<ArithmeticQuestion>>& questionBank) {//read settings from file and use settings if requested

	while (!iss.eof()) {
		std::string temp;
		iss >> temp;

		int min{ 0 };
		int max{ 0 };
		char ch;

		if (temp == "Addition:") {//read the left and right ranges.
			iss >> ch >> min >> ch >> max;
			questionBank.push_back(std::make_unique<Addition>(min, max));
		}
		else if (temp == "Subtraction:") {
			iss >> ch >> min >> ch >> max;
			questionBank.push_back(std::make_unique<Subtraction>(min, max));
		}
		else if (temp == "Multiplication:") {
			iss >> ch >> min >> ch >> max;
			questionBank.push_back(std::make_unique<Multiplication>(min, max));
		}
		else if (temp == "Division:") {
			iss >> ch >> min >> ch >> max;
			questionBank.push_back(std::make_unique<Division>(min, max));
		}
		else if (temp == "time_limit:") break; //will take care of time limit below
	}
	return iss;
}
