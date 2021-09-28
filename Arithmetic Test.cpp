// Program to test mental arithmetic using given operations with user range and time limit with score.
#include<chrono>
#include"Test.h"
#include"Questions.h"
#include"ErrorHandling.h"

using namespace std;

int find_latest(const vector<string>& data) {//find where the last entry for "score:" is. This will let me read the last entry for settings instead of the first.
	string temp = "";
	istringstream iss;
	for (int i = data.size() - 1; i >= 0; --i) {
		iss.str(data[i]);
		iss.clear();                 // clear fail and eof bits
		iss.seekg(0, std::ios::beg);
		iss >> temp; //only bother reading the first word, just looking for the position of the occurence of "score:" to know where the latest settings is in file
		if (temp == "score:") return i;
	}
	return -1;
}


//User prompts and input. TestUI Implementations
int TestUI::getNum() {//takes number given by user. Used for range, time limit, etc.
	int num;
	cin >> num;

	while (!cin) {//cin must be good() and num has a value that is reasonable ie. >0
							//keep asking until both are satisfied
		if (cin.bad()) error("Input to num is bad(). \n");
		else if (cin.fail()) {
			cout << "Non-numerical input entered, try again. \n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer
			num = -1; 
		}
		else  error("Unknown error occurred at get_num().");
		cin >> num;
	}

	return num;
}

std::uniform_int_distribution<int> TestUI::getRange() {
	//Function to get range for the numbers for operations.

	cout << "Operand range? (Two integers). \n";
	int min = this->getNum(); 
	int max = this->getNum();
	if (max < min) std::swap(max, min);
	uniform_int_distribution<int>  dist(min, max);
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	return dist;
}

bool TestUI::getYesNo() {//prompts user for a yes or no
	char ans;
	while (true) {//keep asking for input until correct format is entered
		cin.get(ans);
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
					cout << "Sorry incorrect alphabetic character entered, please try again." << endl;
					break;
			}
		}
		else {
			cout << "Sorry non-alphabetical character entered, please try again." << endl;
		}
	}
}

void TestUI::getUserOps(Test& t) {

	cout << "Addition? 'y' for yes, 'n' for no. \n";  
	if (this->getYesNo()) {
		std::unique_ptr<ArithmeticQuestion> addUptr = std::make_unique<Addition>();
		cout << "Default range for '+'? Min: " << addUptr->getRange().a() << " Max: " << addUptr->getRange().b() << "\n";
		if(!this->getYesNo()) {
			auto newRange = this->getRange();
			addUptr->setRange(newRange.a(), newRange.b());
		}
		t.addQuestion(addUptr);
	}

	cout << "Subtraction? 'y' for yes, 'n' for no. \n";   
	if (this->getYesNo()) {
		std::unique_ptr<ArithmeticQuestion> subUptr = std::make_unique<Subtraction>();
		cout << "Default range for '-'? Min: " << subUptr->getRange().a() << " Max: " << subUptr->getRange().b() << "\n";
		if (!this->getYesNo()) {
			auto newRange = this->getRange();
			subUptr->setRange(newRange.a(), newRange.b());
		}
		t.addQuestion(subUptr);
	}

	cout << "Multiplication? 'y' for yes, 'n' for no. \n";   
	if (this->getYesNo()) {
		std::unique_ptr<ArithmeticQuestion> multiUptr = std::make_unique<Multiplication>();
		cout << "Default range for '*'? Min: " << multiUptr->getRange().a() << " Max: " << multiUptr->getRange().b() << "\n";
		if (!this->getYesNo()) {
			auto newRange = this->getRange();
			multiUptr->setRange(newRange.a(), newRange.b());
		}
		t.addQuestion(multiUptr);
	}

	cout << "Division? 'y' for yes, 'n' for no. \n";   
	if (this->getYesNo()) {
		std::unique_ptr<ArithmeticQuestion> divUptr = std::make_unique<Division>();
		cout << "Default range for '/'? Min: " << divUptr->getRange().a() << " Max: " << divUptr->getRange().b() << "\n";
		if (!this->getYesNo()) {
			auto newRange = this->getRange();
			divUptr->setRange(newRange.a(), newRange.b());
		}
		t.addQuestion(divUptr);
	}

	cout << "Time limit (seconds)? \n";
	t.setTimeLimitSeconds(this->getNum());
}

bool TestUI::getFileSettings(Test& t) {
	cout << "File name? \n";
	std::string iofile;
	getline(cin, iofile);
	t.setSaveFileName(iofile);

	ifstream ist{ iofile };
	if (!ist) error("Cannot open file for reading! \n");
	vector<string> data;
	string temp;

	while (!ist.eof()) {//partitioning the results file into each nonempty line from the beginning.
		getline(ist, temp);
		data.push_back(temp);
	}
	temp = "";

	for (int i = find_latest(data); i < data.size(); ++i) {
		temp += data[i] + " ";
	}

	int timeLimit{ 1 };
	vector<std::unique_ptr<ArithmeticQuestion>> questionBank;
	istringstream iss{ temp };
	iss >> questionBank;
	iss.str(data[data.size() - 3]);//jump to the line that is of the form "time_limit: xx" where xx is some integer. Due to formatting this is the third last line.
	iss >> temp >> timeLimit;

	cout << "The following settings were found: \n";
	for (std::unique_ptr<ArithmeticQuestion>& q : questionBank) {
		cout << q->getOp() << " Range: " << q->getRange().a() << '-' << q->getRange().b() << '\n';
	}
	cout << "Time limit: " << timeLimit << "\n"
		<< "Do you want to proceed with these settings? \n";
	if (!this->getYesNo()) {
		return false;
	}
	else {
		t.setQuestionBank(questionBank);
	}

	ofstream ost{ iofile, ios_base::app };
	if (!ost) error("Cannot open file for writing! \n");

	return true;
}

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
			cout << "No opertions entered. Closing program. \n";
			return 0;
		}


		//for question selection
		random_device rd;
		mt19937 rng(rd());
		while (true) {
			//main loop to randomly pick what operations to give
			currTest.setStartTime();
			std::uniform_int_distribution<int> pickVectorIndex(0, currTest.getQuestionVec().size()-1);
			while (!currTest.isPastTimeLimit()) {
				ArithmeticQuestion& currQuestion = *(currTest.getQuestionVec()[pickVectorIndex(rng)]);
				cout << currQuestion.getRandQuestion();
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

			cout << "Score is: " << currTest.getScore() << "\n";
			if (!currTest.getSaveFileName().empty()) currTest.saveSettingsAndScore();

			cout << "Try again? (y) or (n) \n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //seems to be an issue where a newline is entered here. This is a temp fix for now.
			if (!menu.getYesNo()) break;
		}
		
	}
	
	catch (runtime_error& e) {
		cerr << "Runtime error: " << e.what() << "\n";
		return 1;
	}
	catch (...) {
		cerr << "Exception: something went wrong. \n";
		return 2;
	}
}