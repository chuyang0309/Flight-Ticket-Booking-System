#include <iostream>
#include <iomanip>
using namespace std;

/*
int main() {
	
	int a = 6, b = 4, c = 5, d = 9;

	a = ++b - --d / c--;
	cout << "a= " << a << ", b = " << b << ", c = " << c << ", d = " << d << endl;

	// 1. a=4, b=5, c=4, d=8
	
	// d = d + 5 - (5%3) = 8 + 5 - (2) = 11
	// 2. a=5, b=5, c=3, d=11
	
	// c = c - (5*4) = 3 - 20 = -17
	// 3. a=6, b=4, c=-17, d=11
	// 3. after cout: a=6, b=5, c=-18, d=11
	


	system("pause");
	return 0;
}
*/



//Exercise 2
/*
int main() {
	bool a = 5 + 6 == 3 + 7;
	cout << a << endl;

	bool b = 2 * 6 - 4 >= 9 - 1;
	cout << b << endl;

	bool c = 'U' >= 't';
	cout << "U is " << int('U') << ", t is " << int('t') << endl;
	cout << c << endl;

	cout << "A is " << int('A') << ", a is " << int('a') << endl;
	cout << ('A' <= 'a') << endl;

	cout << "# is " << int('#') << ", + is " << int('+') << endl;
	cout << ('#' <= '+') << endl;

	cout << (6.28 / 3 < 3 - 1.2) << endl;

	system("pause");
	return 0;
}
*/



//Exercise 3
/*
int main() {

	if ('+' < '*') //43 < 42 (false)
		cout << "+*";
	cout << "%%" << endl;

	if (10 <= 2 * 5) // 10<=10 (true)
		cout << "10 ";
	cout << "2*5 ";
	cout << endl;

	if ('a' < 'A') // a =97, b=98 and so on....  (97<65, false)
		cout << 'a';
	cout << 'A';
	cout << endl;

	if (6 == 2 * 4 - 2) // true
		cout << 3 * 4 / 6 - 8 << endl;
	cout << "**" << endl;

	if ('R' < '$' && '&' < '#') // 82<36 && 38<=35,  false && false, false
		cout << "$#";
	cout << "R&";
	cout << endl;

	if ('4' > '3' || 2 < -10) //52>51 (true) || 2<-10 (false)
		cout << "1 2 3 4" << endl;
	cout << "$$" << endl;

	system("pause");
	return 0;
}
*/



//Exercise 4
/*
int main() {
	int score;
	cout << "Score: ";
	cin >> score;

	if (score >= 60 && score <= 100)
		cout << "You pass." << endl;
	else if (score < 0 || score >100)
		cout << "Invalid score." << endl;
	else
		cout << "You fail." << endl;

	system("pause");
	return 0;
}
*/



//Exercise 5
/*
int main() {
	int myNum =33;
	int yourNum = 30;

	if (yourNum % myNum == 3)
	{
		yourNum = 3;
		myNum = 1;
	} 
	else if (yourNum % myNum == 2)
	{
		yourNum = 2;
		myNum = 2;
	}
	else
	{
		yourNum = 1;
		myNum = 3;
	}
	cout << myNum << " " << yourNum << endl;

	system("pause");
	return 0;
}
*/



//Exercise 6
/*
int main() {
	int beta;
	cout << "Input beta value: ";
	cin >> beta;

	switch (beta % 7) {
	case 0:
	case 1:
		beta = beta * beta;
		break;
	case 2:
		beta++;
		break;
	case 3:
		beta = static_cast<int>(sqrt(beta * 1.0));
		break;
	case 4:
		beta = beta + 4;
	case 6:
		beta = beta--;
		break;
	default: 
		beta = -10;
	}
	cout << beta << endl;

	system("pause");
	return 0;
}
*/



//Exercise 7
/*
int main() {
	int weight, distance;
	double rate, cost;
	cout << "Enter weight: ";
	cin >> weight;
	cout << "Enter distance: ";
	cin >> distance;

	if (1 <= weight && weight <= 5) {
		if (1 <= distance && distance <= 99)
			rate = 1.2;
		else if (distance >= 100)
			rate = 2.5;
		else {
			cout << "Wrong distance!" << endl;
			rate = 0;
		}
	}
	else if (weight <= 30) {
		if (1 <= distance && distance <= 99)
			rate = 2.2;
		else if (distance >= 100)
			rate = 2.5;
		else {
			cout << "Wrong distance!" << endl;
			rate = 0;
		}
	}
	else {
		if (distance>=1)
			rate = 4.5;
		else {
			cout << "Wrong distance!" << endl;
			rate = 0;
		}
	}

	if (1<=distance)
		cout << fixed << setprecision(2);
		cout << weight << "KG*" << rate << "=" << cost << endl;
		

	system("pause");
	return 0;
}
*/



//Exercise 8
int main() {
	double num1, num2, num3, largestNum;
	cout << "Enter 3 numbers: ";
	cin >> num1, num2, num3;

	if (num1 >= num2 && num1 >= num3)
		largestNum = num1;
	else if (num2 >= num1 && num2 >= num3)
		largestNum = num2;


	system("pause");
	return 0;
}