#include"Test.h"
#include "ErrorHandling.h"

void Test::startTest(const TestUI& menu)
{
	//for question selection
	std::random_device rd;
	std::mt19937 rng(rd());

	setStartTime();

	std::uniform_int_distribution<int> pickQuestionIndex(0, getQuestionVec().size() - 1);
	while (!isPastTimeLimit())
	{
		ArithmeticQuestion& currQuestion = *(getQuestionVec()[pickQuestionIndex(rng)]);
		std::cout << currQuestion.getRandQuestion();
		if (menu.getNum() != currQuestion.getSoln()) {
			addToScore(currQuestion.incorrectAnsPoints);
			incorrectAnsMsg(currQuestion.incorrectAnsPoints);
		}
		else {
			addToScore(currQuestion.correctAnsPoints);
			correctAnsMsg(currQuestion.correctAnsPoints);
		}
	}
}

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

//User prompts and input. TestUI Implementations
int TestUI::getNum() const {//takes number given by user. Used for range, time limit, etc.
	int num;
	std::cin >> num;

	while (!std::cin) {//std::cin must be good() and num has a value that is reasonable ie. >0
							//keep asking until both are satisfied
		if (std::cin.bad()) error("Input to num is bad(). \n");
		else if (std::cin.fail()) {
			std::cout << "Non-numerical input entered, try again. \n";
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); //clear buffer
			num = -1;
		}
		else  error("Unknown error occurred at get_num().");
		std::cin >> num;
	}

	return num;
}

std::uniform_int_distribution<int> TestUI::getRange() const {
	//Function to get range for the numbers for operations.

	std::cout << "Operand range? (Two integers). \n";
	int min = this->getNum();
	int max = this->getNum();
	if (max < min) std::swap(max, min);
	std::uniform_int_distribution<int>  dist(min, max);
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	return dist;
}

bool TestUI::getYesNo() const {//prompts user for a yes or no
	char ans;
	while (true) {//keep asking for input until correct format is entered
		std::cin.get(ans);
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		if (isalpha(ans))
		{
			switch (tolower(ans))
			{
			case 'y':
				return true;
				break;
			case 'n':
				return false;
				break;
			default:
				std::cout << "Sorry incorrect alphabetic character entered, please try again." << std::endl;
				break;
			}
		}
		else {
			std::cout << "Sorry non-alphabetical character entered, please try again." << std::endl;
		}
	}
}

void TestUI::getUserOps(Test& t) const {

	std::cout << "Addition? 'y' for yes, 'n' for no. \n";
	if (this->getYesNo()) {
		std::unique_ptr<ArithmeticQuestion> addUptr = std::make_unique<Addition>();
		std::cout << "Default range for '+'? Min: " << addUptr->getRange().a() << " Max: " << addUptr->getRange().b() << "\n";
		if (!this->getYesNo()) {
			auto newRange = this->getRange();
			addUptr->setRange(newRange.a(), newRange.b());
		}
		t.addQuestion(addUptr);
	}

	std::cout << "Subtraction? 'y' for yes, 'n' for no. \n";
	if (this->getYesNo()) {
		std::unique_ptr<ArithmeticQuestion> subUptr = std::make_unique<Subtraction>();
		std::cout << "Default range for '-'? Min: " << subUptr->getRange().a() << " Max: " << subUptr->getRange().b() << "\n";
		if (!this->getYesNo()) {
			auto newRange = this->getRange();
			subUptr->setRange(newRange.a(), newRange.b());
		}
		t.addQuestion(subUptr);
	}

	std::cout << "Multiplication? 'y' for yes, 'n' for no. \n";
	if (this->getYesNo()) {
		std::unique_ptr<ArithmeticQuestion> multiUptr = std::make_unique<Multiplication>();
		std::cout << "Default range for '*'? Min: " << multiUptr->getRange().a() << " Max: " << multiUptr->getRange().b() << "\n";
		if (!this->getYesNo()) {
			auto newRange = this->getRange();
			multiUptr->setRange(newRange.a(), newRange.b());
		}
		t.addQuestion(multiUptr);
	}

	std::cout << "Division? 'y' for yes, 'n' for no. \n";
	if (this->getYesNo()) {
		std::unique_ptr<ArithmeticQuestion> divUptr = std::make_unique<Division>();
		std::cout << "Default range for '/'? Min: " << divUptr->getRange().a() << " Max: " << divUptr->getRange().b() << "\n";
		if (!this->getYesNo()) {
			auto newRange = this->getRange();
			divUptr->setRange(newRange.a(), newRange.b());
		}
		t.addQuestion(divUptr);
	}

	std::cout << "Time limit (seconds)? \n";
	t.setTimeLimitSeconds(this->getNum());
}

int find_latest(const std::vector<std::string>& data) {//find where the last entry for "score:" is. This will let me read the last entry for settings instead of the first.
	std::string temp = "";
	std::istringstream iss;
	for (int i = data.size() - 1; i >= 0; --i) {
		iss.str(data[i]);
		iss.clear();                 // clear fail and eof bits
		iss.seekg(0, std::ios::beg);
		iss >> temp; //only bother reading the first word, just looking for the position of the occurence of "score:" to know where the latest settings is in file
		if (temp == "score:") return i;
	}
	return -1;
}

bool TestUI::getFileSettings(Test& t) const {
	std::cout << "File name? \n";
	std::string iofile;
	getline(std::cin, iofile);
	t.setSaveFileName(iofile);

	std::ifstream ist{ iofile };
	if (!ist) error("Cannot open file for reading! \n");
	std::vector<std::string> data;
	std::string temp;

	while (!ist.eof()) {//partitioning the results file into each nonempty line from the beginning.
		getline(ist, temp);
		data.push_back(temp);
	}
	temp = "";

	for (int i = find_latest(data); i < data.size(); ++i) {
		temp += data[i] + " ";
	}

	int timeLimit{ 1 };
	std::vector<std::unique_ptr<ArithmeticQuestion>> questionBank;
	std::istringstream iss{ temp };
	iss >> questionBank;
	iss.str(data[data.size() - 3]);//jump to the line that is of the form "time_limit: xx" where xx is some integer. Due to formatting this is the third last line.
	iss >> temp >> timeLimit;

	std::cout << "The following settings were found: \n";
	for (std::unique_ptr<ArithmeticQuestion>& q : questionBank) {
		std::cout << q->getOp() << " Range: " << q->getRange().a() << '-' << q->getRange().b() << '\n';
	}
	std::cout << "Time limit: " << timeLimit << "\n"
		<< "Do you want to proceed with these settings? \n";
	if (!this->getYesNo()) {
		return false;
	}
	else {
		t.setQuestionBank(questionBank);
	}

	std::ofstream ost{ iofile, std::ios_base::app };
	if (!ost) error("Cannot open file for writing! \n");

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
