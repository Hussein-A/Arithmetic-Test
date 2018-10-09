//#include "../../std_lib_facilities.h"
// Program to test mental arithmetic using given operations with user range and time limit with score.
using namespace std;
#include<iostream>
#include<string>
#include<random>
#include<algorithm>
#include<vector>
#include<time.h>


int score{ 0 };//will be modified by each "operation"_op depending on right or wrong answer


enum class operation {
	plus = 1, minus, product, division
};

operation int_to_op(int x)
{
	if (x<int(operation::plus) || int(operation::division) < x) cerr <<"bad operation \n";
	return operation(x);
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

	//constructors
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



class invalid_num {}; //invalid number class to throw for error.







int get_num() {//takes number given by user. Used for range, time limit, etc.
	
	int num=-1; //set to -1 for while loop check below in case cin>> num fails
	cin >> num;
	
	while (!cin|| num<0) {//cin must be good() and num has a value that is reasonable ie. >0
							//keep asking until both are satisfied
		cin >> num;
		if (cin.bad()) throw invalid_num{};
		else if (cin.fail()) {
			cout << "Improper input entered, try again. \n";
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer
			num = -1; //reset to failure case
		}
		else if (num <= 0) cout << "Invalid number entered, must be greater than 0. Try again. \n";
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
	return user_range;
}


inline int get_rand(const range& range) {//provides a random number from the given range, called often.

	return (rand() % (range.up() + 1 - range.low())) + range.low();

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



 

bool get_yesno() {//prompts user if they want given operation
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
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer
				return false;
				break;
			default:
					cout << "Sorry incorrect alphabetic character entered, please try again. \n";
					cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer
					break;
			}
		}

		else {
			cout << "Sorry non-alphabetical character entered, please try again. \n";
			cin.ignore(numeric_limits<streamsize>::max(), '\n'); //clear buffer
		}

	}


}


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
	else {

	}

	cout << "Multiplication? 'y' for yes, 'n' for no. \n";   
	if (get_yesno()) {
		vector<range> range_multi{range(2,12), range(2,100)};
		cout << "Default range for multiplication? Left: (2,12) Right: (2,100). \n";
		if (get_yesno()) {

		}
		else range_multi = get_range();
		user_op.push_back(op_range(operation::product, range_multi[0], range_multi[1]));
	}
	else {

	}

	cout << "Division? 'y' for yes, 'n' for no. \n";   
	if (get_yesno()) {
		vector<range> range_div{ range(2,100), range(2,12) };
		cout << "Default range for division? Left: (2,100) Right: (2,12). \n";
		if (get_yesno()) {

		}
		else range_div = get_range();

		//must check that the upperbound of leftnum is at integrally divisible by the upperbound of the right num and is nonzero
		if (int quotient = range_div[0].up() / range_div[1].up() == 0) {
			cout << "Error, the ceiling of the left hand number must be divisible (and nonzero) by the ceiling of the righthand num. \n"
				<< "Setting division range to default. \n";
			range_div= {range(2,100), range(2,12) };
		}


		user_op.push_back(op_range(operation::division, range_div[0], range_div[1]));
	}
	else {

	}

	return user_op;
}



void plus_op(const range& lrange, const range& rrange) {

	int left_num = get_rand(lrange); //generating a number in the given range
	int right_num = get_rand(rrange);

	while (true) {//keep asking for input until gets question right, check_time prevents going past time limit despite incorrect attempts
		cout << left_num << '+' << right_num << "? \n";
		if (get_soln(left_num, right_num, operation::plus)) {
			cout << "Correct! \n";
			++score;
			break;
		}

		else {
			cout << "Incorrect. Try again. \n";
		}
	}
		
}

void minus_op(const range& lrange, const range& rrange) {
	int left_num = get_rand(lrange); //generating a number in the given range
	int right_num = get_rand(range(rrange.low(), left_num)); //send a new range such that right_num < left_num 

	while (true) {
		cout << left_num << '-' << right_num << "? \n";
		if (get_soln(left_num, right_num, operation::minus)) {
			cout << "Correct! \n";
			++score;
			break;
		}

		else {
			cout << "Incorrect. Try again. \n";
		}
	}
		

}


void multi_op(const range& lrange, const range& rrange) {
	int left_num = get_rand(lrange); //no need for restriction on left and right num
	int right_num = get_rand(rrange);

	while (true) {
		cout << left_num << '*' << right_num << "? \n";
		if (get_soln(left_num, right_num, operation::product)) {
			cout << "Correct! \n";
			++score;
			break;
		}

		else {
			cout << "Incorrect. Try again. \n";
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
			cout << "Correct! \n";
			++score;
			break;
		}

		else {
			cout << "Incorrect. Try again. \n";
		}
	}

	
	
}



int main() {

	try{
		
		time_t start_time{ time(NULL) };//Will be checked against multiple times, ex. after question is answered

		
		srand((int)start_time); //Generating random seed for rand() based on current time
		
		vector<op_range> opernums = get_ops();
		if (opernums.size() == 0) {
			cout << "No opertions entered. Closing program. \n";
			return 0;
		}

		cout << "Time limit? (In seconds) \n";
		int time_limit = get_num();
	

		//main loop to randomly pick what operations to give
		time_t current_time;
		while (difftime(time(&current_time),start_time)<time_limit) {
			operation op = opernums[(rand() % opernums.size())].op();  // Number to choose which ops ex. if all 4 operations then number between 1-4 then pick that position in the vector of opranges

			switch (op){
			case operation::plus:
				plus_op(opernums[0].lrange(), opernums[0].rrange());
				break;

			case operation::minus:
				minus_op(opernums[0].lrange(), opernums[0].rrange());
				break;

			case operation::product:
				multi_op(opernums[0].lrange(), opernums[0].rrange());
				break;

			case operation::division:
				div_op(opernums[0].lrange(), opernums[0].rrange());
				break;




			}

		}
	}
	
	

	catch (invalid_num) {
		cout << "ERROR! Number not entered. \n";
		return 1;
	}

	cout << "Score is: " << score;

	

}

