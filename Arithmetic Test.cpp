// Program to test mental arithmetic using given operations with user range and time limit with score.
#include<iostream>
#include <fstream>
#include<sstream>
#include<string>
#include<random>
#include<algorithm>
#include<vector>
#include<time.h>

using namespace std;

//Error handling. Note the following was borrowed from Strouroup's Programming: Principles and Practice using C++
struct Exit : runtime_error {
	Exit() : runtime_error("Exit") {}
};

// error() simply disguises throws:
inline void error(const string& s)
{
	throw runtime_error(s);
}

inline void error(const string& s, const string& s2)
{
	error(s + s2);
}

inline void error(const string& s, int i)
{
	ostringstream os;
	os << s << ": " << i;
	error(os.str());
}
//End of Error Handling


enum class operation {
	plus = 1, minus, product, division
};

operation int_to_op(int x)//simple error check for integer to associated operation
{
	if (x<int(operation::plus) || int(operation::division) < x) error ("No such operation for given int. \n");
	return operation(x);
}

string op_to_string(const operation& op) {//used when writing to file what op was used
	switch (op) {
	case operation::plus:
		return "plus";
		break;
	case operation::minus:
		return "minus";
		break;
	case operation::product:
		return "product";
		break;
	case operation::division:
		return "division";
		break;
	default:
		return "error no such operation";
		break;
	}

	return "Error unkown operation received. \n";
}

operation string_to_op(const string& s) {//used when writing to file what op was used
	if (s == "plus") {
		return operation::plus;
	}
	else if (s == "minus") {
		return operation::minus;
	}
	else if (s == "product") {
		return operation::product;
	}
	else if (s == "division") {
		return operation::division;
	}
	else {
		error("error no such operation");
	}
}


class range {//storing range lower bound and upper bound
public:
	range() {} //default constructor.
	range(int x, int y) {//constructor that sorts the input into proper range (lower, upper).
		upper = (x >= y) ? x : y;
		lower = (x >= y) ? y : x;
	}

	//reading functions.
	int low() const { return lower; }
	int up() const { return upper; }


private:
	int lower{2};//default range: [2,100]
	int upper{100};
};

class op_range {//stores operation and associated range for left and right num

public:
	op_range() {}

	op_range(operation x, range l, range r ) {
		operation = x;
		leftrange = l;
		rightrange = r;
	}

	operation op() const { return operation; }
	range lrange() const { return leftrange; }
	range rrange() const { return rightrange; }

private:
	operation operation{operation::plus};
	range leftrange{range()}; //left and right ranges for left and right operands
	range rightrange{range()};//default '+', on range (int) [0,1] for both ranges
};


struct result {//record, keeping the score and current time

	int score{ 0 };
	time_t ltime{ time(NULL) };
};

//Global variables
result lresult; //will be modified by each "operation"_op depending on right or wrong answer

//Writing to results file/Record keeping
ostream& operator<<(ostream& ost, result r) {
	//overloading to write to file
	//data will be stored as follows: ((score), (time)) where score is an integer and time is of the form (Www Mmm dd hh:mm:ss yyyy) followed by a '\n' and terminated with a null character

	char buffer[32];			//since whenever using << on a result it will always be writing to file,
								// will just take the time at the moment of writing to file with the operator
	struct tm timeinfo;
	localtime_s(&timeinfo, &r.ltime);
	asctime_s(buffer, 32, &timeinfo);
	ost << "score: " << r.score << '\n' << "date: " << buffer;
	return ost;

}

ostream& operator<<(ostream& ost, vector<op_range> r) {//adding settings to file, operations used and associated range for each

	for (int i = 0; i < (int)r.size(); ++i) {
		ost << op_to_string(r[i].op()) << ": " << "( " << r[i].lrange().low() <<  " , " << r[i].lrange().up() << " )"
			<< ", " << "( " << r[i].rrange().low() << " , " << r[i].rrange().up() << " )" << '\n';
	}
	return ost;
}

istringstream& operator>> (istringstream& iss, vector<op_range>& r) {//read settings from file and use settings if requested

	while (!iss.eof()) {
		string temp;
		iss >> temp;

		int lrange_low{ 0 };
		int lrange_up{ 0 };
		int rrange_low{ 0 };
		int rrange_up{ 0 };
		char ch;

		if (temp == "score:") {} //do nothing with score, keep going
		else if (temp == "Date:") {}
		else if (temp == "plus:") {//read the left and right ranges.
			iss >> ch >> lrange_low >> ch >> lrange_up;
			while (iss >> ch && ch != '('); //eat characters until you get to the proper position to read an int
			iss >> rrange_low >> ch >> rrange_up >> ch;
			r.push_back(op_range(operation::plus, range{ lrange_low, lrange_up }, range{ rrange_low, rrange_up }));
		}
		else if (temp == "minus:") {
			iss >> ch >> lrange_low >> ch >> lrange_up;
			while (iss >> ch && ch != '('); //eat characters until you get to the proper position to read an int
			iss >> rrange_low >> ch >> rrange_up >> ch;
			r.push_back(op_range(operation::minus, range{ lrange_low, lrange_up }, range{ rrange_low, rrange_up }));
		}
		else if (temp == "product:") {
			iss >> ch >> lrange_low >> ch >> lrange_up;
			while (iss >> ch && ch != '('); //eat characters until you get to the proper position to read an int
			iss >> rrange_low >> ch >> rrange_up >> ch;
			r.push_back(op_range(operation::product, range{ lrange_low, lrange_up }, range{ rrange_low, rrange_up }));
		}
		else if (temp == "division:") {
			iss >> ch >> lrange_low >> ch >> lrange_up;
			while (iss >> ch && ch != '('); //eat characters until you get to the proper position to read an int
			iss >> rrange_low >> ch >> rrange_up >> ch;
			r.push_back(op_range(operation::division, range{ lrange_low, lrange_up }, range{ rrange_low, rrange_up }));
		}
		else if (temp == "time_limit:") break; //will take care of time limit below
	}
	return iss;
}


//Numerical
inline int get_rand(const range& range) {//provides a random number from the given range, called often.
	return (rand() % (range.up() + 1 - range.low())) + range.low();
}

inline int flip_sign(int& x) {
	range negative_chance = range(1, 5); // 1/5 chance of flipping a given number to its negative
	if (get_rand(negative_chance) == 1) return x *= -1; // chance of flipping one of the numbers to negative. See definition of negative chance
	else return x;
}

int find_latest(vector<string>& data) {//find where the last entry for "score:" is. This will let me read the last entry for settings instead of the first.
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


//User prompts and input
int get_num() {//takes number given by user. Used for range, time limit, etc.
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

vector<range> get_range() {
	//Function to get range for the numbers for operations.
	//Returns a vector of two ranges. First being the range of the left hand number, second being the right.

	cout << "Left hand numerical range? (Two integers). \n";
	int min = get_num(); // Does not actually have to be ordered, will be sorted by constructor.
	int max = get_num();
	range lrange(min, max); //left range


	cout << "Right hand numerical range? (Two integers). \n";
	min = get_num();
	max = get_num();
	range rrange(min, max); //right range

	vector<range> user_range(2);
	user_range[0] = lrange; user_range[1] = rrange;
	cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer
	return user_range;
}


bool get_soln(const int& left,const int& right,const operation& op) {//takes the two operands and gets solution from user, returns true or false depending on correctness.
	
	int ans = get_num();
	switch (op) {//check user ans against answer given the chosen operation and operands
		case operation::plus:
			if (ans == left + right) return true;
			else return false;
			break;
		case operation::minus:
			if (ans == left - right) return true;
			else return false;
			break;
		case operation::product:
			if (ans == left * right) return true;
			else return false;
			break;
		case operation::division:
			if (ans == left / right) return true; //note assumes that left is divisible by right. Handled else where by operation
			else return false;
			break; 
		default:
			//do nothing all possible operations captured
			break;	
	}

}


bool get_yesno() {//prompts user for a yes or no
	char ans;
	while (true) {//keep asking for input until correct format is entered
		cin.get(ans);
		if (isalpha(ans)) 
		{
			switch (tolower(ans)) 
			{
			case 'y':
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer
				return true;
				break;
			case 'n':
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
				return false;
				break;
			default:
					cout << "Sorry incorrect alphabetic character entered, please try again. \n";
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
					break;
			}
		}

		else {
			cout << "Sorry non-alphabetical character entered, please try again. \n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}
}

//User get ops, associated range or default
vector<op_range> get_ops() {

	vector<op_range> user_op; //Will place chosen operations by user in this vector

	cout << "Addition? 'y' for yes, 'n' for no. \n";  
	if (get_yesno()) {
		vector<range> range_plus(2); //vector to hold the range of addition
		cout << "Default range for '+'? Left: (2,100) Right: (2,100). \n";
		if(get_yesno()) {
			//default already set by range constructor, do nothing 
		}
		else range_plus = get_range(); 
		user_op.push_back(op_range(operation::plus, range_plus[0], range_plus[1]));
	}
	else {
		//do nothing
	}

	cout << "Subtraction? 'y' for yes, 'n' for no. \n";   
	if (get_yesno()) {
		vector<range> range_sub(2);
		cout << "Default range for '-'? Left: (2,100) Right: (2,100). \n";
		if (get_yesno()) {

		}
		else range_sub = get_range();
		user_op.push_back(op_range(operation::minus, range_sub[0], range_sub[1]));
	}
	else {}

	cout << "Multiplication? 'y' for yes, 'n' for no. \n";   
	if (get_yesno()) {
		
		vector<range> range_multi{ range (2, 12), range (2, 100) };
		cout << "Default range for multiplication? Left: (2,12) Right: (2,100). \n";
		if (get_yesno()) {

		}
		else range_multi = get_range();

		user_op.push_back(op_range(operation::product, range_multi[0], range_multi[1]));

	}
	else {}

	cout << "Division? 'y' for yes, 'n' for no. \n";   
	if (get_yesno()) {
		vector<range> range_div{ range(2,100), range(2,12) };
		cout << "Default range for division? Left: (2,100) Right: (2,12). \n";
		if (get_yesno()) {
			
		}
		else range_div = get_range();

		//must check that the upperbound of leftnum is at divisible as an integer by the upperbound of the right num and is nonzero
		if (int quotient = range_div[0].up() / range_div[1].up() == 0) {
			cout << "Error, the ceiling of the left hand number must be divisible (and nonzero) by the ceiling of the righthand num. \n"
				<< "Setting division range to default. \n";
			range_div= {range(2,100), range(2,12) };
		}


		user_op.push_back(op_range(operation::division, range_div[0], range_div[1]));
	}
	else {}
	return user_op;
}

vector<op_range> get_settings(string& iofile, int& time_limit) {
	//search for results file
	cout << "Do you have a previous results file? y or n \n";
	vector<op_range> opernums;

	char ans = 'n'; //will be used to check if the answer to above is yes no for later
	if (get_yesno()) {
		ans = 'y';
		cout << "File name? \n";
		getline(cin, iofile);

		ifstream ist{ iofile };//do a quick check that the given files can be opened and written to then close. This prevents doing the test, having a score but being unable to write it in the end.
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

		istringstream iss{ temp };
		iss >> opernums;
		iss.str(data[data.size() - 3]);//jump to the line that is of the form "time_limit: xx" where xx is some integer. Due to formatting this is the third last line.
		iss >> temp >> time_limit;

		cout << "The following settings were found: \n";
		for (op_range x : opernums) {
			cout << op_to_string(x.op()) << " Left range: " << x.lrange().low() << '-' << x.lrange().up() << " Right range: " << x.rrange().low() << '-' << x.rrange().up() << '\n';
		}
		cout << "Time limit: " << time_limit << "\n"
			<< "Do you want to proceed with these settings? \n";
		if (get_yesno()) {}
		else {
			opernums = get_ops();
			cout << "Time limit? (In seconds) \n";
			time_limit = get_num();
		}

		ofstream ost{ iofile, ios_base::app };
		if (!ost) error("Cannot open file for writing! \n");
	}

	else {
		opernums = get_ops();
		cout << "Time limit? (In seconds) \n";
		time_limit = get_num();
	}

	return opernums;
}
//User prompts and input


//Operations
void plus_op(const range& lrange, const range& rrange) {

	int left_num = get_rand(lrange); //generating a number in the given range
	int right_num = get_rand(rrange);

	if (get_rand(range(1, 2)) == 1) left_num = flip_sign(left_num);//flip a coin to decide which number to have a chance at being negative
	else right_num = flip_sign(right_num);

	while (true) {//keep asking for input until gets question right, check_time prevents going past time limit despite incorrect attempts
		cout << left_num << '+' << right_num << "? \n";
		if (get_soln(left_num, right_num, operation::plus)) {
			cout << "Correct! (+1) \n";
			lresult.score++;
			break;
		}

		else {
			cout << "Incorrect (-2). Try again. \n";
			lresult.score-=2;
		}
	}
		
}

void minus_op(const range& lrange, const range& rrange) {
	int left_num = get_rand(lrange); //generating a number in the given range
	int right_num = get_rand(rrange); //Note resulting difference may be negative, this is a deliberate choice.

	if (get_rand(range(1, 2)) == 1) left_num = flip_sign(left_num);//flip a coin to decide which number to have a chance at being negative
	else right_num = flip_sign(right_num);

	while (true) {
		cout << left_num << '-' << right_num << "? \n";
		if (get_soln(left_num, right_num, operation::minus)) {
			cout << "Correct! (+1) \n";
			lresult.score++;
			break;
		}

		else {
			cout << "Incorrect (-2). Try again. \n";
			lresult.score-=2;
		}
	}
}


void multi_op(const range& lrange, const range& rrange) {
	int left_num = get_rand(lrange); //no need for restriction on left and right num
	int right_num = get_rand(rrange);


	while (true) {
		cout << left_num << '*' << right_num << "? \n";
		if (get_soln(left_num, right_num, operation::product)) {
			cout << "Correct! (+1) \n";
			lresult.score++;
			break;
		}

		else {
			cout << "Incorrect (-2). Try again. \n";
			lresult.score-=2;
		}
	}
}

void div_op(const range& lrange, const range& rrange) {

	int right_num = get_rand(rrange); //need left num to be a multiple of right num, handled here
	int quotient = lrange.up() / right_num; //check how many times right_num goes into the upper bound of left number

	int left_num = right_num * get_rand(range(1, quotient)); //left_num will be below ceiling of left hand number
	//guaranteed that the quotient is at least 1 by get_ops. Forced default range if not

	while (true) {
		cout << left_num << '/' << right_num << "? \n";
		if (get_soln(left_num, right_num, operation::division)) {
			cout << "Correct! (+1) \n";
			lresult.score++;
			break;
		}

		else {
			cout << "Incorrect (-2). Try again. \n";
			lresult.score-=2;
		}
	}
}
//Operations


int main() {
	try{
		string iofile;
		int time_limit;

		vector<op_range> opernums = get_settings(iofile, time_limit);
		if (opernums.size() == 0) {
			cout << "No opertions entered. Closing program. \n";
			return 0;
		}

		while (true) {
		

			time_t start_time{ time(NULL) };//Will be checked against multiple times, ex. after question is answered
			srand((int)start_time); //Generating random seed for rand() based on current time
			lresult.score = 0; //reset score after each iteration

			//main loop to randomly pick what operations to give
			time_t current_time;
			operation op;
			int position;
			while (difftime(time(&current_time), start_time) < time_limit) {
				position = (rand() % opernums.size()); // Number to choose which ops ex. if all 4 operations then number between 1-4 then pick that position in the vector of opranges
				op = opernums[position].op();

				switch (op) {
				case operation::plus:
					plus_op(opernums[position].lrange(), opernums[position].rrange());
					break;

				case operation::minus:
					minus_op(opernums[position].lrange(), opernums[position].rrange());
					break;

				case operation::product:
					multi_op(opernums[position].lrange(), opernums[position].rrange());
					break;

				case operation::division:
					div_op(opernums[position].lrange(), opernums[position].rrange());
					break;
				}
			}

			cout << "Score is: " << lresult.score << "\n";
			lresult.ltime = time(NULL);
			if (iofile != "") {//not the empty string means we must've written down someplace to write results to when get_settings was called
				ofstream ost{ iofile, ios_base::app };
				if (!ost) error("Cannot open file for writing! File Tampered after entry! \n");
				ost << lresult << opernums << "time_limit: " << time_limit << "\n\n"; //see << operator overload above for the format of record keeping
			}

			cout << "Try again? (y) or (n) \n";
			if (get_yesno()) {}
			else { break; }
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

