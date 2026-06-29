#include<iostream>
#include<iomanip>
#include<string>
#include<fstream>
#include<cctype>
#include <windows.h>
using namespace std;

//Structs	
// (A) Name
struct nameType {
	string firstName;
	string lastName;
};

// (B) User registration info
struct regDetails {
	nameType name;
	string phNum;
	string gmail;
	string username;
	string password;
};

// (C) Booking details
struct Booking {
	nameType passenger_fullname;
	int depart_flight_from_to = 0;
	string departure_date;
	int departure_time = 0;
	int return_flight_from_to = 0;
	string return_date;
	int return_time = 0;
};

// (D) Check In details for the passenger
struct CheckInPassenger {
	nameType passengerName;
	string passportNumber = "";
	nameType contactName;
	string contactMobile = "";
};

// (E) Payment and Check In details
struct PaymentCheckIn {
	double totalAmount = 0.0;
	string paymentStatus = "unpaid";
	CheckInPassenger checkInList[10];
	string checkInStatus = "not checked";
};


#define SIZE 20
string currentUser = "lily";
Booking flight = { {"Lily Lee Li", "Ling"}, 7, "12/08/2025", 1, 8, "14/08/2025", 3 };
int noTickets = 3;

void menu();
void FlightSchedule();
void registration();
void readUser(regDetails user[], int& userCount);
int login(regDetails user[], int userCount);
void performBooking();
void readBooking(Booking[], int&);
void editBooking(Booking[], int);
void payment();
void readPaymentCheckIn(PaymentCheckIn&);
void checkIn(PaymentCheckIn&);
void printInvoice(Booking[], PaymentCheckIn);
void regDecoration();
void loginDecoration();
void coverDecoration();
void bookingDecoration();
void paymentDecoration();
void editBookingDecoration();
void checkInDecoration();
void mainMenuDecoration();
void setConsoleColor(int colorCode);


// Input Validation Functions
bool number_validation(string input, int min, int max) {
	if (input.empty()) //if user input nothing and press enter
		return false; //then it is invalid, return false to calling function
	for (int i = 0; i < input.length(); i++) //check the entered input by user 1 by 1 on whether it is a digit or not
	{
		if (!isdigit(input[i]))//if any of the input is not a digit
			return false;// then it is invalid, return false to calling function
	}
	int digit = stoi(input);//if the input is a digit and the input is not empty, then convert it (currently is string since use getline) into integer value and save into digit(temporary variable)
	return digit >= min && digit <= max;//if the input is within range stated in the calling function, then return true to calling function, else return false
}

bool date_validation(string date) {

	if (date.length() != 10 || date[2] != '/' || date[5] != '/') 	// Date must have 10 elements and index 2 and 5 must be a '/'
		return false;												//else, return false to calling function

	for (int i = 0; i < date.length(); i++) {  	// Check the date input 1 by 1
		if (i == 2 || i == 5)   // no need to check for index 2 and 5 since it should be '/', so continue 
			continue;
		if (!isdigit(date[i]))   //check if other index is digit or not
			return false;   //if not, then return false to calling function
	}

	int day = stoi(date.substr(0, 2));  // take index 0 and 1, then convert to integer 
	int month = stoi(date.substr(3, 2)); //  take index 3 and 4, then convert to integer 
	int year = stoi(date.substr(6, 4));  // take index 6,7,8,9 , then convert to integer 

	if (day < 1 || day > 31) // Check valid ranges for day month and year
		return false;
	if (month < 1 || month > 12)
		return false;
	if (year < 2025) //only accept year 2025 and onwards
		return false;

	return true;// if date is valid, return true to calling function
}

bool name_validation(string name) {//Function to check name entered is valid or not
	if (name.empty())//if user input nothing and press enter
		return false; // then it is invalid, return false to calling function
	bool with_letter = false; //to check if the entered name is with letter
	for (char n : name) {//check every input char by char 
		if (isalpha(n)) {
			with_letter = true; // if the char is a letter, then true
		}
		else if (n == ' ') { //if one of the input is not a letter but a space,then do nothing
		}
		else
			return false;//if one of the input is not a letter or a space,then return false to calling function (invalid name)
	}

	return with_letter; // return true to calling function if the input is only with space and letter (have at least one letter)
}

// Function to check if the input is a string of digits with an exact required length
// Example use for Card Number (16 digits), CVV (3 digits)
bool digit_string_validation(string input, int exact_length) {
	// Check if input length matches the required exact length
	if (input.length() != exact_length)
		return false;

	// Check if all characters are digits (0 9)
	if (input.find_first_not_of("0123456789") != string::npos)
		return false;

	// If both checks pass, the input is valid
	return true;
}

// Function to validate credit/debit card expiry date in MM/YYYY format
// Ensures proper format and that the date is not in the past
bool expiry_date_validation(string date) {
	// Check if the string has exactly 7 characters (e.g., "04/2026")
	if (date.length() != 7)
		return false;

	// Check that the 3rd character is '/'
	if (date[2] != '/')
		return false;

	// Check that the first two characters (month) are digits
	if (!isdigit(date[0]) || !isdigit(date[1]))
		return false;

	// Check that the last four characters (year) are digits
	for (int i = 3; i < 7; ++i) {
		if (!isdigit(date[i]))
			return false;
	}

	// Convert the substring to integer values for month and year
	int month = stoi(date.substr(0, 2));       // MM
	int year = stoi(date.substr(3, 4));        // YYYY (corrected from index 6 to 3)

	// Validate month range (must be 01 to 12)
	if (month < 1 || month > 12)
		return false;

	// Validate year is 2025 or later
	if (year < 2025)
		return false;

	// All checks passed; format and values are valid
	return true;
}

// Function to check if the input is a string of digits with at least a minimum length
// Example use for Bank Account Number (at least 8 digits)
bool min_digit_string_validation(string input, int min_length) {
	// Check if input length meets or exceeds the minimum length
	if (input.length() < min_length)
		return false;

	// Check if all characters are digits (0 9)
	if (input.find_first_not_of("0123456789") != string::npos)
		return false;

	// If both checks pass, the input is valid
	return true;
}

// Validate passport number (letters and digits only, no spaces)
bool passport_validation(const string& passport) {
	if (passport.empty()) return false;
	for (char c : passport) {
		if (!isalnum(c)) return false;
	}
	return true;
}

// Validate Malaysian phone numbers starting with 01 and 10/11 digits
bool phone_number_validation(const string& phone) {
	if (phone.length() != 10 && phone.length() != 11)
		return false;
	if (phone.substr(0, 2) != "01")
		return false;
	for (char c : phone) {
		if (!isdigit(c)) return false;
	}
	return true;
}


int main() {
	regDetails user[20];
	string choice;
	string contRL;
	int userCount;
	int loginStatus;

	Booking flight_booking[SIZE];
	int num_of_passengers = 0, cont = 1, noTickets, select;
	string input;
	bool valid_input = false;

	PaymentCheckIn paymentRecord;
	readPaymentCheckIn(paymentRecord);


	do {
		coverDecoration();


		//INPUT VALIDATION 
		do {
			cout << endl;
			cout << "                                                                           Choice: ";
			getline(cin, choice);

			if (!number_validation(choice, 1, 3)) {
				cout << "                                                                Invalid input. Please re-enter." << endl << endl;
			}
		} while (!number_validation(choice, 1, 3));

		if (choice == "1") {
			system("cls");
			registration();

			do {
				cout << "Do you want to continue login? (1-yes, 2-no): ";
				getline(cin, contRL);
				if (!number_validation(contRL, 1, 2)) {
					cout << "Invalid input. Please re-enter." << endl << endl;
				}
			} while (!number_validation(contRL, 1, 2));

			if (contRL == "2")
				return 0;
		}
		else if (choice == "2") {
			//Function call readUserDetails to read the users' details from "user.txt"
			system("cls");
			readUser(user, userCount);

			//Function call login to return a value for the if...else statement to handle possible conditions:
			loginStatus = login(user, userCount);

			switch (loginStatus)
			{
			case 1:
				cout << endl << "Login successful." << endl;
				break;
			case 2:
				cout << "Incorrect password. Try to login again." << endl;
				break;
			case 3:
				cout << "Username is incorrect or does not exist. Please register if you haven't created an account." << endl;
				break;
			}

			if (loginStatus == 2 || loginStatus == 3) {
				do {
					cout << "Do you want to continue register/login? (1-yes, 2-no): ";
					getline(cin, contRL);
					if (!number_validation(contRL, 1, 2)) {
						cout << "Invalid input. Please re-enter." << endl << endl;
					}
				} while (!number_validation(contRL, 1, 2));
				if (contRL == "2")
					return 0;  //EXIT PROGRAM
			}
			else {
				contRL = "0";  //not 1 (login/ register again) and not 2 either (exit program)
				system("pause");
			}


		}

		else return 0;    //3, option exit
		system("cls");
	} while (contRL == "1");  //1, login/register again


	FlightSchedule(); //Display flight schedule

	do {
		system("cls");
		cout << "Username: " << currentUser << endl;

		mainMenuDecoration();
		menu();  //Display main menu
		readBooking(flight_booking, num_of_passengers);  //Check if there's any booking is made before
		noTickets = num_of_passengers; //numbers of flight tickets to book is based on the number of passengers
		cout << endl;
		do {

			cout << "                                                                                Choice: ";
			getline(cin, input);
			valid_input = number_validation(input, 1, 6); // call function to check the entered choice is valid or not//1,6 is the valid range from 1-6
			if (!valid_input)
				cout << "                                                       Invalid input. Please enter a number between 1 and 6." << endl;//if invalid (false returned from function definition), display this invalid message
			cout << endl;
		} while (!valid_input); //and ask user to re-enter.

		select = stoi(input); //if valid(true returned from function definition), convert input(currently is string)into integer value and save in select(temporary variable)
		system("cls");


		if (select == 1) {//if user enter 1, book flight tickets
			performBooking();
			ofstream outFile(currentUser + "_paymentCheckIn.txt");
			ofstream outFile1(currentUser + "_invoice.txt");
			cont = 1;
		}
		else if (select == 2) {//if user enter 2, edit user's previous flight booking details
			if (num_of_passengers == 0) { // If no booking records found
				setConsoleColor(5);
				cout << "No booking available, please book your flight ticket(s)!" << endl << endl;
				setConsoleColor(7);
				system("pause");
				cont = 1; // Go back to main menu
			}
			else {
				editBooking(flight_booking, num_of_passengers);
				ofstream outFile(currentUser + "_paymentCheckIn.txt");
				ofstream outFile1(currentUser + "_invoice.txt");
				cont = 1;
			}
		}
		// If user chooses option 3: Payment
		else if (select == 3) {
			// Try to open the booking file
			ifstream bookingFile(currentUser + "_Booking.txt");
			// If booking file does NOT exist, show no booking message
			if (!bookingFile.is_open()) {
				setConsoleColor(5);
				cout << "No booking available, please book your flight ticket(s)!" << endl;
				setConsoleColor(7);
				system("pause");
				cont = 1;
			}

			// If booking file exists, proceed with payment
			else {
				payment(); // Call payment function
				readPaymentCheckIn(paymentRecord); // Update payment record
				cont = 1;
			}
		}
		// If user chooses option 4: Check-In
		else if (select == 4) {
			// Try to open the booking file
			ifstream bookingFile(currentUser + "_Booking.txt");
			// If booking file does NOT exist, show no booking message
			if (!bookingFile.is_open()) {
				setConsoleColor(5);
				cout << "No booking available, please book your flight ticket(s)!" << endl;
				setConsoleColor(7);
				system("pause");
				cont = 1;
			}

			// If booking file exists, proceed with check-in
			else {
				checkIn(paymentRecord); // Call check-in function
				cont = 1;
			}
		}

		// If user chooses option 5: Print Invoice
		else if (select == 5) {
			// Try to open the booking file
			ifstream bookingFile(currentUser + "_Booking.txt");
			// If booking file does NOT exist, show no booking message
			if (!bookingFile.is_open()) {
				setConsoleColor(5);
				cout << "No booking available, please book your flight ticket(s)!" << endl;
				setConsoleColor(7);
				system("pause");
				cont = 1;
			}

			// If booking file exists, proceed to print invoice
			else {
				Booking bookingList[10]; // Create an array to hold bookings
				int tempCount = 0; // Temporary passenger count
				readBooking(bookingList, tempCount); // Read booking data
				printInvoice(bookingList, paymentRecord); // Print invoice
				cont = 1;
			}
		}

		// If user chooses option 6: Exit program
		else if (select == 6) {
			cout << "Exiting... Thank you!" << endl;
			cont = 0;
		}

		// If user enters an invalid option (not 1-6)
		else {
			cout << "This option is not available." << endl;
			system("pause");
			cont = 1;
		}

	} while (cont == 1);

	system("pause");
	return 0;  //main()'s return 0
}

//FUNCTIONS

void menu() {
	setConsoleColor(14);
	cout << endl << endl;
	cout << "                      ===============================               ===============================               ===============================" << endl;
	cout << "                     |                               |             |                               |             |                               |" << endl;
	cout << "                     |    1. Book Flight Ticket(s)   |             |        2. Edit Booking        |             |     3. Perform transaction    |" << endl;
	cout << "                     |                               |             |                               |             |                               |" << endl;
	cout << "                      ===============================               ===============================               ===============================" << endl << endl;
	cout << "                      ===============================               ===============================               ===============================" << endl;
	cout << "                     |                               |             |                               |             |                               |" << endl;
	cout << "                     |       4. Check In Flight      |             |        5. Print Invoice       |             |            6. Quit            |" << endl;
	cout << "                     |                               |             |                               |             |                               |" << endl;
	cout << "                      ===============================               ===============================               ===============================" << endl << endl;
	cout << endl;
	setConsoleColor(1);
	cout << "                                                                     What can we assist with today? " << endl;
	setConsoleColor(7);

}
// Displays the list of available flights and time slots
void FlightSchedule() {
	setConsoleColor(15);
	cout << "====================================" << endl;
	cout << "||        Flight Available:       ||" << endl;
	cout << "------------------------------------" << endl;
	cout << "||   1. KL - Penang --> RM200     ||" << endl;
	cout << "||   2. Penang - KL --> RM200     ||" << endl;
	cout << "||   3. KL - Johor --> RM200      ||" << endl;
	cout << "||   4. Johor - KL --> RM200      ||" << endl;
	cout << "||   5. KL - Singapore --> RM250  ||" << endl;
	cout << "||   6. Singapore - KL --> RM250  ||" << endl;
	cout << "||   7. KL - Bangkok --> RM300    ||" << endl;
	cout << "||   8. Bangkok - KL --> RM300    ||" << endl;
	cout << "====================================" << endl;

	cout << endl;
	cout << "==============================================" << endl;
	cout << "||  Available Departure/Return Time Slots:  ||" << endl;
	cout << "----------------------------------------------" << endl;
	cout << "||   1. 8:00 A.M.                           ||" << endl;
	cout << "||   2. 13:00 P.M.                          ||" << endl;
	cout << "||   3. 18:00 P.M.                          ||" << endl;
	cout << "||   4. 23:00 P.M.                          ||" << endl;
	cout << "==============================================" << endl;
	setConsoleColor(7);

}

//Registration function
void registration() {
	regDetails user[20];
	int userCount;
	readUser(user, userCount);  //call readUser function to check if username is taken

	regDecoration();

	//Get user details
	cout << "Please key in your details for registration:" << endl;

	//GET FIRST NAME  
	string firstName;    //FIRST NAME
	bool check;         //checking status, no errors = true

	do {
		check = true;
		cout << "First name: ";
		getline(cin, firstName);         //get name

		if (firstName.length() < 2) {     //if first name too short (less than 2 characters)
			cout << "Please enter a valid first name (too short). "; //prevent unrealistic names
			check = false;
		}

		if (check == true) {  //if 1st round checking pass
			if (firstName[0] == ' ') {
				cout << "Your name shall not start with a space.";
				check = false;
			}
			else {
				for (int i = 0; i < firstName.length(); i++) {    //check for invalid characters. only alphabets and space accepted.
					if (!isalpha(firstName[i]) && firstName[i] != ' ') {
						cout << "Your name must include alphabetical characters only. ";
						check = false;
						break;      //exits the for loop if violation is found
					}
				}
			}
		}

		if (check == false) {
			cout << "Please re-enter." << endl << endl;
		}
	} while (check == false);

	//GET LAST NAME
	string lastName; //LAST NAME

	do {
		check = true;        //any error will change it into fail
		cout << "Last name: ";  //name requirements
		getline(cin, lastName);         //get name

		if (lastName.length() < 2) {     //if first name too short (less than 2 characters)
			cout << "Please enter a valid last name (too short). "; //prevent unrealistic names
			check = false;
		}

		if (check == true) {  //if 1st round checking pass
			if (lastName[0] == ' ') {
				cout << "Your name shall not start with a space. ";
				check = false;
			}
			else {
				for (int i = 0; i < lastName.length(); i++) {    //check for invalid characters. only alphabets and space accepted.
					if (!isalpha(lastName[i]) && lastName[i] != ' ') {
						cout << "Your name must include alphabetical characters only. ";
						check = false;
						break;      //exits the for loop if violation is found
					}
				}
			}
		}

		if (check == false) {
			cout << "Please re-enter." << endl << endl;
		}
	} while (check == false);

	//GET PHONE NUMBER
	string hpNum;
	do {
		check = true;
		cout << "Phone number: ";
		getline(cin, hpNum);

		for (int i = 0; i < hpNum.length(); i++) {
			if (!isdigit(hpNum[i])) {
				cout << "Your phone number shall only consist of numbers." << endl << endl;
				check = false;
				break;
			}
		}
		if (check == true) {
			if (hpNum.substr(0, 2) != "01" || hpNum.length() < 10 || hpNum.length() > 11) {
				check = false;
				cout << "Invalid phone number. Please follow the format 01XXXXXXXXX (10-11 numbers)." << endl << endl;
			}
		}
	} while (check == false);


	string gmail;
	do {
		bool atExist = false;
		bool domainExist = false;
		bool whitespaceExist = false;
		check = true;
		cout << "Email: ";
		getline(cin, gmail);

		if (gmail.length() < 6) {
			check = false;
		}

		if (check == true) {
			for (char c : gmail) {
				if (c == '@')
					atExist = true;
				if (c == ' ')
					whitespaceExist = true;
			}
			if (gmail.substr(gmail.length() - 4, 4) == ".com" || gmail.substr(gmail.length() - 3, 3) == ".my") {
				domainExist = true;
			}

			if (!((domainExist && atExist) && whitespaceExist == false)) // only accepts when the email address provided has a domain, @ and no whitespace
				check = false;
		}


		if (check == false)
			cout << "Please enter a valid email address." << endl << endl;
	} while (check == false);

	//GET USERNAME
	string username;

	do {
		check = true;
		cout << "Username (>=3 characters): ";
		getline(cin, username);

		if (username.length() < 3) {
			cout << "Please enter a longer username." << endl << endl;
			check = false;
		}

		if (check == true) {
			for (int i = 0; i < username.length(); i++) {
				if (!isalnum(username[i]) && !ispunct(username[i]) && username[i] == ' ') {
					check = false;
					break;
				}
			}

			if (check != true)
				cout << "Your username shall only consist of alphabets, numbers and symbols (except whitespaces)." << endl << endl;
			else {
				//Check if user already registered 
				for (int i = 0; i < userCount; i++) {
					if (username == user[i].username) {
						check = false;
						cout << "This username has already been taken. Please choose another username." << endl << endl;
						break;
					}
				}
			}
		}

	} while (check == false);



	//GET PASSWORD
	string password;
	string password2;
	bool upperExist = false;
	bool numberExist = false;
	bool symbolExist = false;

	do {
		check = true;
		cout << "Set up your password (1 uppercase, 1 number, 1 symbol, >=8 characters): ";
		getline(cin, password);

		if (password.length() < 8) {
			cout << "Please enter a longer password." << endl << endl;
			check = false;
		}

		if (check == true) {
			for (int i = 0; i < password.length(); i++) {          //check the password has uppercase, number, symbols and minimum 8 characters
				if (isupper(password[i]))
					upperExist = true;
				else if (isdigit(password[i]))
					numberExist = true;
				else if (ispunct(password[i]))
					symbolExist = true;
			}
			if (!(upperExist && numberExist && symbolExist))
				check = false;

			if (check == false)
				cout << "Your password needs to contain at least 1 uppercase alphabet, 1 number and 1 symbol." << endl << endl;
		}
	} while (check == false);

	do {
		check = true;
		cout << "Confirm your password: ";             //confirm password again
		getline(cin, password2);

		if (password != password2) {
			cout << "Passwords do not match.\n\n";
			check = false;
		}
	} while (check == false);

	//SAVE USER DETAILS
	ofstream writeFile;
	string confirmReg;  // registration confirmation, if yes, write user data to text file 
	do {
		cout << "Confirm to register? (y-yes, n-no): ";
		getline(cin, confirmReg);
		if (confirmReg.empty() || confirmReg != "Y" && confirmReg != "y" && confirmReg != "N" && confirmReg != "n") {
			cout << "Invalid input. Please re-enter." << endl << endl;
		}
	} while (confirmReg.empty() || confirmReg != "Y" && confirmReg != "y" && confirmReg != "N" && confirmReg != "n");


	if (confirmReg == "Y" || confirmReg == "y") {
		writeFile.open("user.txt", ios::app);
		writeFile << firstName << endl << lastName << endl << hpNum << endl << gmail << endl << username << endl << password << endl;
		cout << "Registration successful." << endl << endl;
		writeFile.close();
	}
	else {
		cout << "Registration cancelled." << endl << endl;
	}
}

//Read user function
void readUser(regDetails user[], int& userCount) {
	userCount = 0;
	string line;
	ifstream readFile;
	readFile.open("user.txt");
	if (readFile.is_open()) {
		while (getline(readFile, line) && userCount < 20) {  //20 according to struct array regDetails user[]'s size
			user[userCount].name.firstName = line;

			getline(readFile, line);
			user[userCount].name.lastName = line;

			getline(readFile, line);
			user[userCount].phNum = line;

			getline(readFile, line);
			user[userCount].gmail = line;

			getline(readFile, line);
			user[userCount].username = line;

			getline(readFile, line);
			user[userCount].password = line;

			userCount++;
		}
		readFile.close();
	}
}

//Login function
int login(regDetails user[], int userCount) {
	string username;
	string password;
	bool usernameExist = false;

	loginDecoration();

	cout << endl;
	cout << "Username: ";
	getline(cin, username);
	cout << endl << endl << "Password: ";
	getline(cin, password);


	for (int i = 0; i < userCount; i++) {
		if (username == user[i].username && password == user[i].password) {  //if the username and password matches
			currentUser = username;
			return 1;
		}
		else if (username == user[i].username && password != user[i].password) {  //if the username matches but not the password
			return 2;
		}
	}
	return 3; //Even if the password matches, it will not be shown for security purposes because other people could ve shared the same password. 
}

//Perform booking function
void performBooking()
{
	string book_again;
	do
	{
		bookingDecoration();
		cout << endl;
		Booking flight_booking[SIZE];
		int num_of_passengers;
		string booking_confirmation;
		int depart_flight_from_to, departure_time, return_flight_from_to, return_time;
		string departure_date, return_date;
		string input;


		FlightSchedule();
		cout << endl;
		while (true) {
			cout << endl;
			cout << "Please enter the total number of passengers (1-20): ";
			getline(cin, input);
			if (number_validation(input, 1, SIZE)) {//call function to check the entered number of passengers is valid or not//1,SIZE is the valid range from 1-20
				num_of_passengers = stoi(input);//if valid (true returned from function definition),convert input (currently is string)into integer value and save in num_of_passengers (temporary variable)
				break;//exit loop
			}
			cout << "Invalid input detected! Please enter numbers (1-20) only. " << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
		}

		while (true) {
			cout << endl;
			setConsoleColor(14);
			cout << "----------------------------------------------------" << endl;
			cout << "Please key in your preferred flight booking details:" << endl;
			cout << "----------------------------------------------------" << endl;
			setConsoleColor(7);
			cout << "1. Flight departure (from and to), please enter the flight slot number only (1-8): ";
			getline(cin, input);

			if (number_validation(input, 1, 8)) { //call function to check the entered input is valid or not// 1,8 is the valid range from 1-8 
				depart_flight_from_to = stoi(input);//if valid (true returned from function definition), convert it into integer value and save it into depart_flight_from_to (temporary variable)
				break;//then exit loop
			}
			cout << "Invalid input detected! Please enter the flight slot number between 1-8 only." << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
		}


		while (true) {
			cout << endl;
			cout << "2. Date of departure (dd/mm/yyyy): ";
			getline(cin, departure_date);
			if (date_validation(departure_date)) //call function to check if the entered departure date format is valid or not
				break;//if the entered departure date format is valid (true returned from function definition) then exit loop
			cout << "Invalid date format detected! Please enter date in dd/mm/yyyy format only." << endl;//if the entered departure date format is invalid (return false), display invalid message and ask user to re-enter.
		}

		while (true) {
			cout << endl;
			cout << "3. Time of departure, please enter the slot number only (1-4): ";
			getline(cin, input);

			if (number_validation(input, 1, 4)) {//call function to check if the entered time slot number is valid or not //1,4 is the valid range from 1-4
				departure_time = stoi(input);//if valid (true returned from function definition), convert it into integer value and save it into departure_time (temporary variable)
				break;//then exit loop
			}
			cout << "Invalid input detected! Please enter a number between 1-4 only." << endl; //if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
		}

		while (true) {
			cout << endl;
			cout << "4. Flight return (from and to), please enter the flight slot number only (1-8): ";
			getline(cin, input);

			if (number_validation(input, 1, 8)) {//call function to check the entered slot number is valid or not// 1,8 is the valid range from 1-8 
				return_flight_from_to = stoi(input);//if valid (true returned from function definition), convert it into integer value and save it into return_flight_from_to (temporary variable)
				break;
			}
			cout << "Invalid input detected! Please enter the flight slot number between 1-8 only" << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
		}

		while (true)
		{
			cout << endl;
			cout << "5. Date of return (dd/mm/yyyy): ";
			getline(cin, return_date);
			if (date_validation(return_date)) //call function to check if the entered return date format is valid or not
				break;//if the entered return date format is valid (true returned from function definition) then exit loop
			cout << "Invalid date format detected! Please enter date in dd/mm/yyyy format only." << endl;//if the entered return date format is invalid (return false), display invalid message and ask user to re-enter.

		}


		while (true) {
			cout << endl;
			cout << "6. Time of return, please enter the slot number only (1-4): ";
			getline(cin, input);

			if (number_validation(input, 1, 4)) {//call function to check if the entered time slot number is valid or not //1,4 is the valid range from 1-4
				return_time = stoi(input);//if valid (true returned from function definition), convert it into integer value and save it into return_time (temporary variable)
				break;//then exit loop
			}
			cout << "Invalid input detected! Please enter a number between 1-4 only." << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
		}

		for (int i = 0; i < num_of_passengers; i++)// Repeat to ask the user to enter the passenger(s) last and first name 1 by 1 based on the number of passengers entered
		{
			cout << endl;
			cout << "Passenger " << i + 1 << ":" << endl;
			while (true)
			{
				cout << "Enter passenger " << i + 1 << "'s first name: ";// Enter passenger's first name
				getline(cin, flight_booking[i].passenger_fullname.firstName);
				if (name_validation(flight_booking[i].passenger_fullname.firstName)) //Call function to check the entered first name is valid or not
					break;//if valid (true returned from function definition), then exit loop
				cout << "Invalid input detected! Please enter letters or alphabets only." << endl << endl;//if invalid (false returned from function definition), display invalid message and ask user to re-enter.
			}
			while (true)
			{
				cout << "Enter passenger " << i + 1 << "'s last name : ";//Enter passenger's last name
				getline(cin, flight_booking[i].passenger_fullname.lastName);
				if (name_validation(flight_booking[i].passenger_fullname.lastName)) //Call function to check the entered last name is valid or not
					break;//if valid (true returned from function definition), then exit loop
				cout << "Invalid input detected! Please enter letters or alphabets only." << endl << endl;//if invalid (false returned from function definition), display invalid message and ask user to re-enter.
			}
			//Save the entered booking details into respective members in struct
			flight_booking[i].depart_flight_from_to = depart_flight_from_to;
			flight_booking[i].departure_date = departure_date;
			flight_booking[i].departure_time = departure_time;
			flight_booking[i].return_flight_from_to = return_flight_from_to;
			flight_booking[i].return_date = return_date;
			flight_booking[i].return_time = return_time;
		}

		//Ask user to confirm booking
		while (true) {
			cout << endl;
			cout << "********************************************" << endl;
			cout << "Are you confirm your booking? (Enter Y/N): ";
			getline(cin, booking_confirmation);
			cout << "********************************************" << endl;
			if (booking_confirmation.empty() || booking_confirmation != "Y" && booking_confirmation != "y" && booking_confirmation != "N" && booking_confirmation != "n")
			{
				//If the entered input is not 'Y','y','N','n' or is empty, then ask user to reenter
				cout << "Invalid input. Please enter only Y or N." << endl;
			}
			else
				break; //if valid input (Y,y,n or N), exit loop

		}

		if ((booking_confirmation) == "Y" || (booking_confirmation) == "y") //If user confirm the booking (entered y or Y),then save the booking into XX_Booking.txt
		{
			ofstream outFile;
			outFile.open(currentUser + "_Booking.txt");


			for (int i = 0; i < num_of_passengers; i++)//Save the booking details 1 by 1 and repeat to save each passengers' details properly based on the number of passengers entered by users
			{
				outFile << flight_booking[i].passenger_fullname.firstName << endl;
				outFile << flight_booking[i].passenger_fullname.lastName << endl;
				outFile << flight_booking[i].depart_flight_from_to << endl;
				outFile << flight_booking[i].departure_date << endl;
				outFile << flight_booking[i].departure_time << endl;
				outFile << flight_booking[i].return_flight_from_to << endl;
				outFile << flight_booking[i].return_date << endl;
				outFile << flight_booking[i].return_time << endl;
			}
			outFile.close();
			cout << endl;
			setConsoleColor(10);
			cout << "===============================================================================================================================" << endl;
			cout << "||Congratulations! Your booking is successful! Thank you for choosing JSJK International Airline and have a nice journey. :D ||" << endl;
			cout << "===============================================================================================================================" << endl;
			setConsoleColor(7);
			cout << endl;
			while (true) {
				cout << "*********************************************************" << endl;
				cout << "Do you want to perform new flight ticket booking? (Y/N): ";// Ask user if s/he want to book more flight tickets
				getline(cin, book_again);
				cout << "*********************************************************" << endl;
				if (book_again.empty() || book_again != "Y" && book_again != "y" && book_again != "N" && book_again != "n") {
					cout << "Invalid input detected! Please enter only Y or N." << endl;//Display invalid input message if user do not enter Y,y,n or N and ask user to re-enter again.
				}
				else {
					system("cls");
					break; //exit loop on valid input
				}
				cout << endl;
			}
		}
		else
		{
			cout << endl;
			setConsoleColor(12);
			cout << "Opps! Your booking has been cancelled." << endl;//If user enter N or n for booking confirmation, then the previously entered passengers' details will not be saved in the XX_Booking.txt file
			setConsoleColor(7);
			cout << endl;
			while (true) {
				cout << "*****************************************************" << endl;
				cout << "Do you want to perform new flight ticket booking? (Y/N): ";
				getline(cin, book_again);
				cout << "*****************************************************" << endl;
				if (book_again.empty() || book_again != "Y" && book_again != "y" && book_again != "N" && book_again != "n") {
					cout << "Invalid input. Please enter only Y or N." << endl;//Display invalid input message if user do not enter Y,y,n or N and ask user to re-enter again.
				}
				else {
					system("cls");
					break; // exit loop on valid input
				}
				cout << endl;
			}
		}



	} while ((book_again) == "Y" || (book_again) == "y");
}// If user enter Y or y, which reflects that s/he wants to book more flight tickets, then repeat the whole performBooking function

//Read booking function
void readBooking(Booking flight_booking[], int& num_of_passengers)
{
	ifstream inFile(currentUser + "_Booking.txt"); ;  //read all the items (booking details) listed in XX_Booking.txt
	if (!inFile)//If file fail to open (No booking made before)
	{
		num_of_passengers = 0; // set number of passengers to 0
		return; // Return immediately if file not found
	}


	num_of_passengers = 0;
	while (getline(inFile, flight_booking[num_of_passengers].passenger_fullname.firstName)) //if the file exist, read the details and load into the respective parameters
	{
		getline(inFile, flight_booking[num_of_passengers].passenger_fullname.lastName);
		inFile >> flight_booking[num_of_passengers].depart_flight_from_to;
		inFile.ignore();
		getline(inFile, flight_booking[num_of_passengers].departure_date);
		inFile >> flight_booking[num_of_passengers].departure_time;
		inFile.ignore();
		inFile >> flight_booking[num_of_passengers].return_flight_from_to;
		inFile.ignore();
		getline(inFile, flight_booking[num_of_passengers].return_date);
		inFile >> flight_booking[num_of_passengers].return_time;
		inFile.ignore();
		num_of_passengers++;
	}

	inFile.close();
	cout << endl;

}

//Edit booking function
void editBooking(Booking flight_booking[], int num_of_passengers)// To edit passenger(s) booking details
{

	string update_more; // to check if user wants to edit more

	do
	{
		editBookingDecoration();

		string editconfirmation;
		int choice;
		string input;
		string quit_confirmation;



		FlightSchedule();

		// Display edit menu and get user choice
		cout << endl;

		cout << "==============================================" << endl;
		cout << "||            Edit Booking Menu:            ||" << endl;
		cout << "----------------------------------------------" << endl;
		cout << "|| 1. Change departure flight (from and to) ||" << endl;
		cout << "|| 2. Change date of departure              ||" << endl;
		cout << "|| 3. Change time of departure              ||" << endl;
		cout << "|| 4. Change return flight (from and to)    ||" << endl;
		cout << "|| 5. Change date of return                 ||" << endl;
		cout << "|| 6. Change time of return                 ||" << endl;
		cout << "|| 7. Change name for a passenger           ||" << endl;
		cout << "|| 8. Quit                                  ||" << endl;
		cout << "==============================================" << endl;

		while (true)
		{
			cout << endl;
			cout << "Enter your choice for edit booking menu: ";
			getline(cin, input);

			if (number_validation(input, 1, 8)) {//call function to check if the entered choice is valid or not //1,8 is the valid range from 1-8
				choice = stoi(input);//if valid (true returned from function definition), convert input (currently is string since use getline) into integer value and save it into choice (temporary variable)
				break;//then exit loop
			}
			cout << "Invalid menu choice! Please enter a number from 1 to 8 only." << endl; //if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
		}

		if (choice == 8) { //if user enter 8 for choice
			while (true) {
				cout << endl;
				cout << "***********************************************" << endl;
				cout << "Are you sure you want to quit editing? (Y/N): ";
				getline(cin, quit_confirmation);
				cout << "***********************************************" << endl;
				if (quit_confirmation == "Y" || quit_confirmation == "y") {//if user enter Y or y

					cout << "Exiting edit booking menu..." << endl << endl;//Go back to main menu
					system("pause");
					return;
				}
				else if (quit_confirmation == "N" || quit_confirmation == "n") {//If user enter n or N

					update_more = "Y"; //Ask user to enter choice again
					system("cls");
					break;
				}
				else {
					cout << "Invalid input. Please enter only Y or N." << endl;// if user not enter y,Y,N or n, then display this invalid message and ask user to re-enter.
				}
			}
			continue;
		}

		else if (choice >= 1 && choice <= 6) {//check if the user entered choice is between 1 to 6,if yes continue the below code execution


			int int_input;

			switch (choice) {
			case 1: // Change departure flight
				while (true) {
					cout << endl;
					cout << "Please choose your preferred new departure flight, kindly enter the flight slot number only (1-8): ";
					getline(cin, input);
					if (number_validation(input, 1, 8)) {//call function to check if the entered slot number is valid or not //1,8 is the valid range from 1-8
						int_input = stoi(input);//if valid (true returned from function definition), convert it into integer value and save it into int_input (temporary variable)
						for (int i = 0; i < num_of_passengers; i++)//then save the new departure flight number 1 by 1 for every passengers
							flight_booking[i].depart_flight_from_to = int_input;
						break;//then exit loop after done all saving for each passengers
					}
					cout << "Invalid input detected! Please enter a number from 1 to 8 only." << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
				}
				break;

			case 2: {// Change date of departure
				while (true) {
					cout << endl;
					cout << "Please choose your preferred new date of departure (dd/mm/yyyy): ";
					getline(cin, input);
					if (date_validation(input)) {//call function to check if the entered new departure date format is valid or not
						for (int i = 0; i < num_of_passengers; i++)//if it is valid (true returned from function definition)
							flight_booking[i].departure_date = input;//then save the new departure date 1 by 1 for every passengers
						break;//then exit loop after done all saving for each passengers
					}
					cout << "Invalid date format detected! Please enter date in dd/mm/yyyy format only." << endl;//if it is invalid (return false), display invalid message and ask user to re-enter.
				}

				break;
			}
			case 3: // Change time of departure
				while (true) {
					cout << endl;
					cout << "Please choose your preferred new time of departure, kindly enter the slot number (1-4) only: ";
					getline(cin, input);
					if (number_validation(input, 1, 4)) {//call function to check if the entered slot number is valid or not //1,4 is the valid range from 1-4
						int_input = stoi(input);//if valid (true returned from function definition), convert it into integer value and save it into int_input (temporary variable)
						for (int i = 0; i < num_of_passengers; i++)//then save the new time of departure 1 by 1 for every passengers
							flight_booking[i].departure_time = int_input;
						break;//then exit loop after done all saving for each passengers
					}
					cout << "Invalid input detected! Please enter a number from 1 to 4 only." << endl; //if it is invalid (return false), display invalid message and ask user to re-enter.
				}

				break;//exit while loop

			case 4: // Change return flight
				while (true) {
					cout << endl;
					cout << "Please choose your preferred new return flight, kindly enter the flight slot number only (1-8): ";
					getline(cin, input);
					if (number_validation(input, 1, 8)) {//call function to check if the entered slot number is valid or not //1,8 is the valid range from 1-8
						int_input = stoi(input);//if valid (true returned from function definition), convert it into integer value and save it into int_input (temporary variable)
						for (int i = 0; i < num_of_passengers; i++)//then save the new return flight number 1 by 1 for every passengers
							flight_booking[i].return_flight_from_to = int_input;
						break;//then exit loop after done all saving for each passengers
					}
					cout << "Invalid input detected. Please enter a number from 1 to 8 only." << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
				}

				break;

			case 5: {// Change date of return
				while (true) {
					cout << endl;
					cout << "Please choose your preferred new date of return (dd/mm/yyyy): ";
					getline(cin, input);
					if (date_validation(input)) {//call function to check if the entered new return date format is valid or not
						for (int i = 0; i < num_of_passengers; i++)//if it is valid (true returned from function definition)
							flight_booking[i].return_date = input;//then save the new return date 1 by 1 for every passengers
						break;//then exit loop after done all saving for each passengers
					}
					cout << "Invalid date format detected! Please enter date in dd/mm/yyyy format only." << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
				}

				break;
			}
			case 6: // Change time of return
				while (true) {
					cout << endl;
					cout << "Please choose your preferred new time of return ,kindly enter the slot number (1-4) only: ";
					getline(cin, input);
					if (number_validation(input, 1, 4)) {//call function to check if the entered slot number is valid or not //1,4 is the valid range from 1-4
						int_input = stoi(input);//if valid (true returned from function definition), convert it into integer value and save it into int_input (temporary variable)
						for (int i = 0; i < num_of_passengers; i++)//then save the new time of return 1 by 1 for every passengers
							flight_booking[i].return_time = int_input;
						break;//then exit loop after done all saving for each passengers
					}
					cout << "Invalid input detected! Please enter a number from 1 to 4 only." << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
				}

				break;
			}
		}
		else if (choice == 7) {//if choice entered by user is 7
			string existing_firstname, existing_lastname;
			bool name_found = false;//to check whether the entered first/last name is available in the XX_Booking.txt file

			do {
				while (true) { // Validate first name input
					cout << endl;
					cout << "Please enter the existing first name of the passenger to update: ";
					getline(cin, existing_firstname);
					if (name_validation(existing_firstname))//call function to check the first name entered is valid or not
						break;//if valid (true returned from function definition), then exit loop 
					cout << "Invalid input detected. Please enter letters/alphabets only." << endl;//if invalid (false returned from function definition), display this invalid message and ask user to re-enter.
				}


				while (true) { // Validate last name input
					cout << "Please enter the existing last name of the passenger to update: ";
					getline(cin, existing_lastname);
					if (name_validation(existing_lastname)) // call function to check the last name entered is valid or not
						break;
					cout << "Invalid input detected. Please enter letters/alphabets only." << endl;
				}

				for (int i = 0; i < num_of_passengers; i++) {
					if (flight_booking[i].passenger_fullname.firstName == existing_firstname && flight_booking[i].passenger_fullname.lastName == existing_lastname) {
						//if the entered existing first name and the entered existing last name is same as the one of the (if there are more than 1 passengers book 
						// the flight tickets) previously entered passenger's first and last name when booking flight ticket(s), continue the below code execution.

						string new_first_name, new_last_name;

						while (true) {
							cout << endl;
							cout << "Please enter new first name: ";//Ask user to enter new first name
							getline(cin, new_first_name);
							if (name_validation(new_first_name))//call function to check the new first name entered is valid or not
								break;
							cout << "Invalid input detected. Please enter letters/alphabets only." << endl;
						}

						while (true) {
							cout << "Please enter new last name: ";
							getline(cin, new_last_name);
							if (name_validation(new_last_name))//call function to check the new last name entered is valid or not
								break;
							cout << "Invalid input detected. Please enter letters/alphabets only." << endl;
						}

						flight_booking[i].passenger_fullname.firstName = new_first_name;//save the new first name
						flight_booking[i].passenger_fullname.lastName = new_last_name;//and new last name
						name_found = true;//passenger's name are available in XX_Booking.txt file
						break;
					}
				}


				if (!name_found) {// If the entered existing first and last name are not found in the XX_Booking.txt file,
					cout << endl;
					cout << "Passenger not found in data. Please re-enter." << endl;//then, display this message and ask user to re-enter the existing first and last name
				}
			} while (!name_found); //loop to go back to prompt user enter existing first and last name
		}
		// Confirm saving changes
		while (true) {
			cout << endl;
			cout << "*************************************" << endl;
			cout << "Confirm save the changes? (Y/N): ";
			getline(cin, editconfirmation);
			cout << "*************************************" << endl;
			if (editconfirmation == "Y" || editconfirmation == "y" || editconfirmation == "N" || editconfirmation == "n") //if user enter y,Y,n or N 
				break;//then exit loop

			cout << "Invalid input. Please enter only Y or N." << endl; //if user does not enter y,Y,n or N, then display this message and ask user to re-enter
		}

		if (editconfirmation == "Y" || editconfirmation == "y") { //if the user enter y or Y
			ofstream outFile(currentUser + "_Booking.txt"); //save the updated details into "current user"_Booking.txt 1 by 1
			for (int i = 0; i < num_of_passengers; i++) {
				outFile << flight_booking[i].passenger_fullname.firstName << endl;
				outFile << flight_booking[i].passenger_fullname.lastName << endl;
				outFile << flight_booking[i].depart_flight_from_to << endl;
				outFile << flight_booking[i].departure_date << endl;
				outFile << flight_booking[i].departure_time << endl;
				outFile << flight_booking[i].return_flight_from_to << endl;
				outFile << flight_booking[i].return_date << endl;
				outFile << flight_booking[i].return_time << endl;
			}
			outFile.close();
			cout << endl;
			setConsoleColor(10);
			cout << "Booking updated successfully.";
			setConsoleColor(7);
			cout << endl;
		}
		else {
			cout << endl;
			setConsoleColor(12);
			cout << "No changes were saved.";
			setConsoleColor(7);
			cout << endl;
		}

		while (true) {     // Ask if user wants to update more
			cout << endl;
			cout << "****************************************************" << endl;
			cout << "Do you want to update other flight details? (Y/N): ";
			getline(cin, update_more);
			cout << "****************************************************" << endl;
			cout << endl;
			if (update_more == "Y" || update_more == "y" || update_more == "N" || update_more == "n") { //if user enter y,Y,n or N
				system("cls");
				break;//then exit loop
			}
			cout << "Invalid input. Please enter only Y or N." << endl;//if user does not enter y,Y,n or N, then display this message and ask user to re-enter
		}

	} while (update_more == "Y" || update_more == "y"); //Repeat the whole editBooking function if the user enter y or Y 

}

//Payment function
void payment() {
	paymentDecoration();
	string centerPrefix = "                                                        ";

	Booking flightBooking[20]; // Can store up to 20 bookings
	int num_of_passengers = 0; // Declare num_of_passengers
	PaymentCheckIn paymentRecord;
	readPaymentCheckIn(paymentRecord); // Read existing payment record

	// Read booking details first
	ifstream inFile(currentUser + "_Booking.txt");
	if (!inFile) {
		cout << centerPrefix << "No booking available, please book your flight ticket(s)!" << endl;
		system("pause");
		return;
	}

	// Check if payment has already been made
	if (paymentRecord.paymentStatus == "paid") {
		// Flag to detect whether the current booking details have changed
		bool bookingChanged = false;

		// Open the previous payment file
		ifstream paidFile(currentUser + "_paymentCheckIn.txt");
		if (paidFile) {
			string line;
			int passengerIndex = 0;

			// Skip first two lines: totalAmount and "Paid" status
			getline(paidFile, line); // Skip totalAmount
			getline(paidFile, line); // Skip payment status

			// Loop through the booking data stored in payment file
			for (int i = 0; i < num_of_passengers; i++) {
				string fileName, fileGender, filePassport, fileNationality, fileDepart, fileReturn;

				// Read stored passenger check-in data (6 lines per passenger)
				getline(paidFile, fileName);
				getline(paidFile, fileGender);
				getline(paidFile, filePassport);
				getline(paidFile, fileNationality);
				getline(paidFile, fileDepart);
				getline(paidFile, fileReturn);

				// Build current full name from current booking
				string fullName = flightBooking[i].passenger_fullname.firstName + " " +
					flightBooking[i].passenger_fullname.lastName;

				// Convert current booking's route indices to string
				string bookingDepart = to_string(flightBooking[i].depart_flight_from_to);
				string bookingReturn = to_string(flightBooking[i].return_flight_from_to);

				// Compare the key booking details: name, depart route, return route
				if (fileName != fullName || fileDepart != bookingDepart || fileReturn != bookingReturn) {
					// If any mismatch is found, booking is considered changed
					bookingChanged = true;
					break;
				}
			}
			paidFile.close(); // Close the file after checking
		}

		// If no changes found, skip payment
		if (!bookingChanged) {
			cout << centerPrefix << "Payment already made." << endl;
			system("pause");
			return;
		}
		else {
			// Changes detected: require new payment
			cout << centerPrefix << "Booking details have changed since last payment. Re-payment is required." << endl;
		}
	}

	while (getline(inFile, flightBooking[num_of_passengers].passenger_fullname.firstName)) {
		getline(inFile, flightBooking[num_of_passengers].passenger_fullname.lastName);
		inFile >> flightBooking[num_of_passengers].depart_flight_from_to;
		inFile.ignore();
		getline(inFile, flightBooking[num_of_passengers].departure_date);
		inFile >> flightBooking[num_of_passengers].departure_time;
		inFile.ignore();
		inFile >> flightBooking[num_of_passengers].return_flight_from_to;
		inFile.ignore();
		getline(inFile, flightBooking[num_of_passengers].return_date);
		inFile >> flightBooking[num_of_passengers].return_time;
		inFile.ignore();
		num_of_passengers++;
	}
	inFile.close();

	// Now num_of_passengers = real noTickets
	double priceList[9] = { 0,200,200,200,200,250,250,300,300 };
	string routeList[9] = { "", "KL - Penang", "Penang - KL", "KL - Johor", "Johor - KL", "KL - Singapore", "Singapore - KL", "KL - Bangkok", "Bangkok - KL" };
	string timeSlot[5] = { "", "8:00 A.M.", "13:00 P.M.", "18:00 P.M.", "23:00 P.M." };

	double departPrice = priceList[flightBooking[0].depart_flight_from_to];
	double returnPrice = priceList[flightBooking[0].return_flight_from_to];
	double totalPerTicket = departPrice + returnPrice;
	double totalAmount = num_of_passengers * totalPerTicket;

	// Display payment details
	cout << centerPrefix << "----- P A Y M E N T   D E T A I L S -----" << endl;
	cout << centerPrefix << "Total Flight Ticket(s)  : " << num_of_passengers << endl;
	cout << centerPrefix << "Departure Flight        : " << flightBooking[0].departure_date << ", " << routeList[flightBooking[0].depart_flight_from_to] << ", " << timeSlot[flightBooking[0].departure_time] << endl;
	cout << centerPrefix << "Return Flight           : " << flightBooking[0].return_date << ", " << routeList[flightBooking[0].return_flight_from_to] << ", " << timeSlot[flightBooking[0].return_time] << endl;
	cout << centerPrefix << "Total Payment           : RM " << totalAmount << endl;

	char confirm;
	string confirmInput;

	// Ask user to confirm payment
	while (true) {
		cout << centerPrefix << "Continue for payment? (y-yes, n-no): ";
		getline(cin, confirmInput);

		// Validate input: only accept 'y', 'Y', 'n', 'N'
		if (confirmInput.length() == 1 &&
			(confirmInput[0] == 'y' || confirmInput[0] == 'Y' ||
				confirmInput[0] == 'n' || confirmInput[0] == 'N')) {
			confirm = confirmInput[0];
			break; // Valid input, break loop
		}
		else {
			cout << centerPrefix << "Invalid input. Please enter 'y' for yes or 'n' for no." << endl;
		}
	}

	// If user cancels payment
	if (confirm == 'n' || confirm == 'N') {
		cout << centerPrefix << "Payment cancelled." << endl;
		return; // Exit payment function
	}

	// Prompt user to select a payment method
	int method;
	string methodInput;

	while (true) {
		cout << centerPrefix << "-----------------------------------------------------------" << endl;
		cout << centerPrefix << ">> Select Payment Method" << endl;
		cout << centerPrefix << "-----------------------------------------------------------" << endl;
		cout << centerPrefix << "1. Credit/Debit Card" << endl;
		cout << centerPrefix << "2. Bank Transfer" << endl;
		cout << centerPrefix << "Choice: ";
		getline(cin, methodInput);

		// Validate payment method selection
		if (methodInput == "1" || methodInput == "2") {
			method = stoi(methodInput); // Convert choice to integer
			break;
		}
		else {
			cout << centerPrefix << "Invalid input. Please enter number 1 or 2 only." << endl;
		}
	}

	// If user selects Credit/Debit Card, collect card details
	if (method == 1) {
		string cardName, cardNum, expDate, cvv;

		// Validate Card Holder Name (only letters and spaces, must include at least one letter)
		do {
			cout << "\n" << centerPrefix << "Card Holder Name       : ";
			getline(cin, cardName);

			if (!name_validation(cardName)) // Call name validation function
				cout << centerPrefix << "Invalid name. Name must contain only letters and spaces, and must have at least one letter." << endl;
			else
				break;
		} while (true);

		// Validate Card Number (must be exactly 16 digits, numbers only)
		while (true) {
			cout << "\n" << centerPrefix << "Card Number (16 digits): ";
			getline(cin, cardNum);

			if (digit_string_validation(cardNum, 16)) // Use digit string validator
				break;

			cout << centerPrefix << "Invalid card number. Please enter exactly 16 digits (numbers only)." << endl;
		}

		// Validate Expiry Date (MM/YYYY format)
		while (true) {
			cout << "\n" << centerPrefix << "Expiry Date (MM/YYYY)  : ";
			getline(cin, expDate);

			if (expiry_date_validation(expDate)) // Validate using expiry date format
				break;

			cout << centerPrefix << "Invalid expiry date. Format must be MM/YYYY." << endl;
		}

		// Validate CVV (must be exactly 3 digits)
		while (true) {
			cout << "\n" << centerPrefix << "CVV (3 digits)         : ";
			getline(cin, cvv);

			if (digit_string_validation(cvv, 3)) // Use digit string validator
				break;

			cout << centerPrefix << "Invalid CVV. Please enter exactly 3 digits." << endl;
		}
	}

	// If user selects Bank Transfer, collect bank details
	else {
		string bankUsername, bankPassword, bankAcc;

		// Validate Bank Username (must not be empty and must not contain spaces)
		do {
			cout << "\n" << centerPrefix << "Bank Username      : ";
			getline(cin, bankUsername);

			// A valid username is non-empty and contains no whitespace
			bool valid = !bankUsername.empty() && bankUsername.find(' ') == string::npos;

			if (!valid)
				cout << centerPrefix << "Invalid username. Cannot be empty or contain spaces." << endl;
			else
				break;
		} while (true);

		// Validate Bank Password (must not be empty and must not contain spaces)
		do {
			cout << "\n" << centerPrefix << "Bank Password      : ";
			getline(cin, bankPassword);

			// A valid password is non-empty and contains no whitespace
			bool valid = !bankPassword.empty() && bankPassword.find(' ') == string::npos;

			if (!valid)
				cout << centerPrefix << "Invalid password. Cannot be empty or contain spaces." << endl;
			else
				break;
		} while (true);

		// Validate Bank Account Number using a digit-only function (at least 8 digits)
		while (true) {
			cout << "\n" << centerPrefix << "Bank Account Number: ";
			getline(cin, bankAcc);

			if (min_digit_string_validation(bankAcc, 8)) // Function ensures only digits and minimum length
				break;

			cout << centerPrefix << "Invalid account number. Must be at least 8 digits (numbers only, no spaces)." << endl;
		}
	}

	// Confirm the transaction
	char confirm2;

	while (true) {
		cout << "\n" << centerPrefix << "Confirm Transaction? (y-yes, n-no): ";
		getline(cin, confirmInput);

		// Validate confirmation input (y/Y or n/N)
		if (confirmInput.length() == 1 &&
			(confirmInput[0] == 'y' || confirmInput[0] == 'Y' ||
				confirmInput[0] == 'n' || confirmInput[0] == 'N')) {
			confirm2 = confirmInput[0];
			break; // Exit the loop if input is valid
		}
		else {
			cout << centerPrefix << "Invalid input. Please enter 'y' for yes or 'n' for no." << endl;
		}
	}

	// If user cancels the transaction, exit the function
	if (confirm2 == 'n' || confirm2 == 'N') {
		cout << "\n" << centerPrefix << "Transaction cancelled." << endl;
		return;
	}

	// Save payment data to file
	paymentRecord.paymentStatus = "paid"; // Mark payment as completed
	paymentRecord.checkInStatus = "not checked"; // Set initial check-in status
	paymentRecord.totalAmount = totalAmount; // Save the total amount too

	string fileName = currentUser + "_paymentCheckIn.txt"; // Payment record file name
	ofstream outFile(fileName); // Open the file for writing
	if (!outFile) {
		cerr << "Error: Unable to create payment file!" << endl;
		return;
	}

	cout << "\n" << centerPrefix << "Saving to file: " << fileName << endl;

	// Write payment details to the file
	outFile << paymentRecord.totalAmount << endl;
	outFile << "Paid" << endl;

	// Write empty check-in details for each ticket
	for (int i = 0; i < num_of_passengers; i++) {
		outFile << "NULL\nNULL\nNULL\nNULL\nNULL\nNULL" << endl;
	}
	outFile << "Not Check" << endl;  // Mark as not checked-in

	outFile.close();  // Close the file
	cout << "\n" << centerPrefix << "Payment details saved successfully." << endl;

	system("pause");
}

// Function to read the payment and check-in record from the file
void readPaymentCheckIn(PaymentCheckIn& record) {
	// Open the payment record file
	ifstream inFile(currentUser + "_paymentCheckIn.txt");
	if (!inFile) {
		record.paymentStatus = "not paid";
		record.checkInStatus = "not checked";
		record.totalAmount = 0.0;
		return; // Exit if payment file does not exist
	}

	// Read the total payment amount (first line)
	inFile >> record.totalAmount;
	inFile.ignore(); // Ignore the newline after the amount

	// Read the payment status (second line)
	getline(inFile, record.paymentStatus);

	// Convert paymentStatus to lowercase
	for (int i = 0; i < record.paymentStatus.length(); ++i) {
		if (record.paymentStatus[i] >= 'A' && record.paymentStatus[i] <= 'Z') {
			record.paymentStatus[i] += ('a' - 'A');
		}
	}

	// Now, open the booking file to count how many passengers
	int passengerCount = 0;
	ifstream bookingFile(currentUser + "_Booking.txt");
	if (bookingFile.is_open()) {
		string temp;
		while (getline(bookingFile, temp)) {
			passengerCount++; // Read first line (firstname)
			for (int i = 0; i < 7; i++) { // Each passenger has 8 lines
				getline(bookingFile, temp);
			}
		}
		bookingFile.close();
	}

	// In payment file, each passenger has 6 lines of NULL
	string temp;
	for (int i = 0; i < passengerCount * 6; ++i) {
		getline(inFile, temp); // Skip NULL lines
	}

	// Read the check-in status (after NULLs)
	getline(inFile, record.checkInStatus);

	inFile.close(); // Close the file
}

// Function to handle the check-in process
void checkIn(PaymentCheckIn& record) {
	checkInDecoration();
	string centerPrefix = "                                                        ";

	// Read existing payment record to check payment status
	PaymentCheckIn paymentRecord;
	readPaymentCheckIn(paymentRecord);

	// Read current user's booking details
	Booking flight_booking[20];
	int num_of_passengers = 0;
	readBooking(flight_booking, num_of_passengers);

	// Open the check-in file if it exists
	ifstream checkInFile(currentUser + "_CheckInDetails.txt");
	bool checkInAlreadyDone = false;   // Flag to indicate if check-in was already done
	int checkedInCount = 0;            // Number of passengers previously checked in
	bool bookingChanged = false;       // Flag to detect any change in booking info (name or passenger count)

	if (checkInFile) {
		string line;
		int bookingIndex = 0; // Index to track passengers in current booking

		// Read the check-in file line by line
		while (getline(checkInFile, line)) {
			// Count how many "Passenger" sections are present
			if (line.find("Passenger ") != string::npos) {
				checkedInCount++;
			}
			// Detect if check-in was marked as "Checked"
			else if (line.find("Status of Check-In: Checked") != string::npos) {
				checkInAlreadyDone = true;
			}
			// Check passenger names for mismatch
			else if (line.find("Full Name:") != string::npos && bookingIndex < num_of_passengers) {
				// Extract name from file
				size_t colonPos = line.find(":");
				string fullNameInFile = line.substr(colonPos + 1);
				fullNameInFile.erase(0, fullNameInFile.find_first_not_of(" ")); // Trim leading spaces

				// Construct current full name from booking
				string fullNameInBooking = flight_booking[bookingIndex].passenger_fullname.firstName + " " +
					flight_booking[bookingIndex].passenger_fullname.lastName;

				// If name from file doesn't match current booking, flag as changed
				if (fullNameInFile != fullNameInBooking) {
					bookingChanged = true;
				}
				bookingIndex++; // Move to next passenger
			}
		}
		checkInFile.close(); // Done reading check-in file

		// If check-in already done
		if (checkInAlreadyDone) {
			// Redo required if either passenger details changed or passenger count differs
			if (bookingChanged || checkedInCount != num_of_passengers) {
				cout << centerPrefix << "Booking details (names or number of passengers) have changed since last check-in. Re-check-in is required." << endl;
			}
			// Otherwise, inform user that check-in is already complete
			else {
				cout << centerPrefix << "You have already completed check-in for this booking!" << endl;
				system("pause");
				return;
			}
		}
	}

	// Verify if payment was made before allowing check-in
	if (paymentRecord.paymentStatus != "paid") {
		cout << centerPrefix << "Payment Status: Not Paid" << endl;
		cout << centerPrefix << "No payment for the flight ticket(s) yet, please proceed to make payment before check-in. Thank you!" << endl;
		system("pause");
		return;
	}

	cout << centerPrefix << "Payment Status: Paid" << endl;
	cout << centerPrefix << "~~~~~~~~~ Proceed to Check-In ~~~~~~~~~" << endl << endl;
	cout << centerPrefix << "Press Enter Key to proceed check-in ~~";

	cin.ignore(); // Clear any leftover newline character

	int contactCount = 0; // Track the number of different contacts

	// Loop through each passenger for check-in
	for (int i = 0; i < num_of_passengers; i++) {
		cout << "\n" << centerPrefix << "Passenger " << (i + 1) << endl;

		// First Name 
		while (true) {
			cout << "\n" << centerPrefix << "First Name: ";
			getline(cin, record.checkInList[i].passengerName.firstName);
			if (!name_validation(record.checkInList[i].passengerName.firstName)) {
				cout << centerPrefix << "Invalid input. First Name must contain only letters and spaces!" << endl;
				continue;
			}
			if (record.checkInList[i].passengerName.firstName != flight_booking[i].passenger_fullname.firstName) {
				cout << centerPrefix << "First Name does not match booking record! Please re-enter correctly." << endl;
				continue;
			}
			break;
		}

		//  Last Name 
		while (true) {
			cout << "\n" << centerPrefix << "Last Name: ";
			getline(cin, record.checkInList[i].passengerName.lastName);
			if (!name_validation(record.checkInList[i].passengerName.lastName)) {
				cout << centerPrefix << "Invalid input. Last Name must contain only letters and spaces!" << endl;
				continue;
			}
			if (record.checkInList[i].passengerName.lastName != flight_booking[i].passenger_fullname.lastName) {
				cout << centerPrefix << "Last Name does not match booking record! Please re-enter correctly." << endl;
				continue;
			}
			break;
		}

		// Passport Number
		while (true) {
			cout << "\n" << centerPrefix << "Passport Number: ";
			getline(cin, record.checkInList[i].passportNumber);
			if (!passport_validation(record.checkInList[i].passportNumber)) {
				cout << centerPrefix << "Invalid input. Passport Number must contain only letters and numbers without spaces!" << endl;
				continue;
			}
			break;
		}

		//Contact Person 
		if (i == 0) {
			// For first passenger, always ask new contact
			while (true) {
				cout << "\n" << centerPrefix << "Contact Person First Name: ";
				getline(cin, record.checkInList[i].contactName.firstName);
				if (name_validation(record.checkInList[i].contactName.firstName)) break;
				cout << centerPrefix << "Invalid input. Contact First Name must contain only letters and spaces!" << endl;
			}

			while (true) {
				cout << "\n" << centerPrefix << "Contact Person Last Name: ";
				getline(cin, record.checkInList[i].contactName.lastName);
				if (name_validation(record.checkInList[i].contactName.lastName)) break;
				cout << centerPrefix << "Invalid input. Contact Last Name must contain only letters and spaces!" << endl;
			}

			while (true) {
				cout << "\n" << centerPrefix << "Contact Person Phone Number: ";
				getline(cin, record.checkInList[i].contactMobile);
				if (phone_number_validation(record.checkInList[i].contactMobile)) break;
				cout << centerPrefix << "Invalid input. Phone Number must be in format '01XXXXXXXX' (10/11 digits, no spaces)." << endl;
			}
			contactCount++;
		}
		else {
			// For next passengers, ask if reuse previous contact
			string sameContactInput;
			char sameContact;
			while (true) {
				cout << "\n" << centerPrefix << "Same contact person as previous? (y-yes, n-no): ";
				getline(cin, sameContactInput);
				if (sameContactInput.length() == 1 &&
					(sameContactInput[0] == 'y' || sameContactInput[0] == 'Y' ||
						sameContactInput[0] == 'n' || sameContactInput[0] == 'N')) {
					sameContact = sameContactInput[0];
					break;
				}
				cout << centerPrefix << "Invalid input. Please enter 'y' or 'n' only." << endl;
			}

			if (sameContact == 'y' || sameContact == 'Y') {
				record.checkInList[i].contactName = record.checkInList[0].contactName;
				record.checkInList[i].contactMobile = record.checkInList[0].contactMobile;
			}
			else {
				while (true) {
					cout << "\n" << centerPrefix << "Contact Person First Name: ";
					getline(cin, record.checkInList[i].contactName.firstName);
					if (name_validation(record.checkInList[i].contactName.firstName)) break;
					cout << centerPrefix << "Invalid input. Contact First Name must contain only letters and spaces!" << endl;
				}

				while (true) {
					cout << "\n" << centerPrefix << "Contact Person Last Name: ";
					getline(cin, record.checkInList[i].contactName.lastName);
					if (name_validation(record.checkInList[i].contactName.lastName)) break;
					cout << centerPrefix << "Invalid input. Contact Last Name must contain only letters and spaces!" << endl;
				}

				while (true) {
					cout << "\n" << centerPrefix << "Contact Person Phone Number: ";
					getline(cin, record.checkInList[i].contactMobile);
					if (phone_number_validation(record.checkInList[i].contactMobile)) break;
					cout << centerPrefix << "Invalid input. Phone Number must be in format '01XXXXXXXX' (10/11 digits, no spaces)." << endl;
				}
				contactCount++;
			}
		}
	}

	// Confirm the whole check-in
	string confirmInput;
	char confirm;
	while (true) {
		cout << "\n" << centerPrefix << "Confirm check-in for " << num_of_passengers << " passengers? (y/n): ";
		getline(cin, confirmInput);

		if (confirmInput.length() == 1 &&
			(confirmInput[0] == 'y' || confirmInput[0] == 'Y' ||
				confirmInput[0] == 'n' || confirmInput[0] == 'N')) {
			confirm = confirmInput[0];
			break;
		}
		cout << centerPrefix << "Invalid input. Please enter 'y' for yes or 'n' for no." << endl;
	}

	if (confirm == 'n' || confirm == 'N') {
		cout << centerPrefix << "Check-in cancelled." << endl;
		return;
	}

	// Saving check-in into file
	record.checkInStatus = "checked"; // Update the status
	record.totalAmount = paymentRecord.totalAmount; // Copy payment total

	ofstream outFile(currentUser + "_CheckInDetails.txt");
	if (!outFile) {
		cout << centerPrefix << "Error creating check-in details file!" << endl;
		return;
	}

	// Write header
	outFile << centerPrefix << "=============== CHECK-IN DETAILS ===============" << endl;
	outFile << centerPrefix << "Total Payment: RM " << fixed << setprecision(2) << record.totalAmount << endl;
	outFile << centerPrefix << "Status of Payment: Paid" << endl;
	outFile << centerPrefix << "----------------------------------------------" << endl;

	// Write each passenger info
	for (int i = 0; i < num_of_passengers; i++) {
		outFile << centerPrefix << "Passenger " << (i + 1) << ":" << endl;
		outFile << centerPrefix << "  Full Name: " << record.checkInList[i].passengerName.firstName << " " << record.checkInList[i].passengerName.lastName << endl;
		outFile << centerPrefix << "  Passport Number: " << record.checkInList[i].passportNumber << endl;
		outFile << centerPrefix << "  Contact Person Name: " << record.checkInList[i].contactName.firstName << " " << record.checkInList[i].contactName.lastName << endl;
		outFile << centerPrefix << "  Contact Person Phone Number: " << record.checkInList[i].contactMobile << endl;
		outFile << centerPrefix << "----------------------------------------------" << endl;
	}

	// Write the check-in status
	outFile << centerPrefix << "Status of Check-In: Checked" << endl;
	outFile.close(); // Done writing

	cout << "\n" << centerPrefix << "Check-in complete. Data saved to " << currentUser << "_CheckInDetails.txt" << endl;
	system("pause"); // Pause screen
}
// Function to print the invoice after payment and check-in
void printInvoice(Booking bookingList[], PaymentCheckIn payCheckInfo) {
	// Route list for flight directions
	string routeList[9] = { "", "KL - Penang", "Penang - KL", "KL - Johor", "Johor - KL",
							"KL - Singapore", "Singapore - KL", "KL - Bangkok", "Bangkok - KL" };

	// Time slots for flight departures
	string timeSlot[5] = { "", "8:00 A.M.", "13:00 P.M.", "18:00 P.M.", "23:00 P.M." };

	Booking flight_booking[20];     // Array to store booking details
	PaymentCheckIn checkInInfo;     // Structure to store check-in details
	int num_of_passengers = 0;      // Counter for number of passengers
	string temp;                    // Temporary string for line reading

	// Read Booking File 
	ifstream bookingFile(currentUser + "_Booking.txt");
	if (!bookingFile.is_open()) {
		cout << "Error opening booking file!" << endl;
		system("pause");
		return;
	}

	// Read each passenger's booking info
	while (getline(bookingFile, flight_booking[num_of_passengers].passenger_fullname.firstName)) {
		getline(bookingFile, flight_booking[num_of_passengers].passenger_fullname.lastName);
		bookingFile >> flight_booking[num_of_passengers].depart_flight_from_to;
		bookingFile.ignore();
		getline(bookingFile, flight_booking[num_of_passengers].departure_date);
		bookingFile >> flight_booking[num_of_passengers].departure_time;
		bookingFile.ignore();
		bookingFile >> flight_booking[num_of_passengers].return_flight_from_to;
		bookingFile.ignore();
		getline(bookingFile, flight_booking[num_of_passengers].return_date);
		bookingFile >> flight_booking[num_of_passengers].return_time;
		bookingFile.ignore();
		num_of_passengers++; // Increment count of passengers
	}
	bookingFile.close();

	// Read Payment File 
	ifstream paymentFile(currentUser + "_paymentCheckIn.txt");
	if (!paymentFile.is_open()) {
		cout << "Error opening paymentCheckIn file!" << endl;
		system("pause");
		return;
	}
	paymentFile >> payCheckInfo.totalAmount;
	paymentFile.ignore();
	getline(paymentFile, payCheckInfo.paymentStatus);
	paymentFile.close();

	// ======= Read Check-In File =======
	ifstream checkInFile(currentUser + "_CheckInDetails.txt");
	if (!checkInFile.is_open()) {
		cout << "Error opening CheckInDetails file!" << endl;
		system("pause");
		return;
	}

	// Skip header lines
	getline(checkInFile, temp); // Title
	getline(checkInFile, temp); // Total Payment
	getline(checkInFile, temp); // Status of Payment
	getline(checkInFile, temp); // Separator line

	// Read check-in details for each passenger
	for (int i = 0; i < num_of_passengers; i++) {
		getline(checkInFile, temp); // "Passenger X:"

		// Read passenger name
		getline(checkInFile, temp);
		size_t pos = temp.find(':');
		if (pos != string::npos) {
			string name = temp.substr(pos + 2);
			size_t spacePos = name.find(' ');
			if (spacePos != string::npos) {
				checkInInfo.checkInList[i].passengerName.firstName = name.substr(0, spacePos);
				checkInInfo.checkInList[i].passengerName.lastName = name.substr(spacePos + 1);
			}
			else {
				checkInInfo.checkInList[i].passengerName.firstName = name;
				checkInInfo.checkInList[i].passengerName.lastName = "";
			}
		}

		// Read passport number
		getline(checkInFile, temp);
		pos = temp.find(':');
		if (pos != string::npos)
			checkInInfo.checkInList[i].passportNumber = temp.substr(pos + 2);

		// Read contact person's name
		getline(checkInFile, temp);
		pos = temp.find(':');
		if (pos != string::npos) {
			string contact = temp.substr(pos + 2);
			size_t spacePos = contact.find(' ');
			if (spacePos != string::npos) {
				checkInInfo.checkInList[i].contactName.firstName = contact.substr(0, spacePos);
				checkInInfo.checkInList[i].contactName.lastName = contact.substr(spacePos + 1);
			}
			else {
				checkInInfo.checkInList[i].contactName.firstName = contact;
				checkInInfo.checkInList[i].contactName.lastName = "";
			}
		}

		// Read contact person's phone number
		getline(checkInFile, temp);
		pos = temp.find(':');
		if (pos != string::npos)
			checkInInfo.checkInList[i].contactMobile = temp.substr(pos + 2);

		// Skip separator line
		getline(checkInFile, temp);
	}

	// Read overall check-in status
	getline(checkInFile, temp);
	size_t pos = temp.find(": ");
	if (pos != string::npos && pos + 2 < temp.length()) {
		checkInInfo.checkInStatus = temp.substr(pos + 2);
	}
	else {
		checkInInfo.checkInStatus = "unknown";
	}

	checkInFile.close();

	// Create Invoice File
	ofstream outFile(currentUser + "_invoice.txt");
	if (!outFile.is_open()) {
		cout << "Error creating invoice file!" << endl;
		system("pause");
		return;
	}

	// Write invoice header
	outFile << "=====================================" << endl;
	outFile << "        JSJK AIRLINES INVOICE        " << endl;
	outFile << "=====================================" << endl;
	outFile << "Username              : " << currentUser << endl;
	outFile << "Tickets Purchased     : " << num_of_passengers << endl;
	outFile << "Total Payment         : RM " << fixed << setprecision(2) << payCheckInfo.totalAmount << endl;
	outFile << "Payment Status        : " << payCheckInfo.paymentStatus << endl;
	outFile << "Check-in Status       : " << checkInInfo.checkInStatus << endl;
	outFile << "-------------------------------------" << endl;

	// Write flight schedule
	outFile << "----------------------------- Flight Schedule -----------------------------" << endl;
	outFile << "Departure              : " << routeList[flight_booking[0].depart_flight_from_to]
		<< " | Date: " << flight_booking[0].departure_date
		<< " | Time: " << timeSlot[flight_booking[0].departure_time] << endl;
	outFile << "Return                 : " << routeList[flight_booking[0].return_flight_from_to]
		<< " | Date: " << flight_booking[0].return_date
		<< " | Time: " << timeSlot[flight_booking[0].return_time] << endl;
	outFile << "---------------------------------------------------------------------------" << endl;

	// Write passenger details
	outFile << "--------- Passenger Details ---------" << endl;
	for (int i = 0; i < num_of_passengers; i++) {
		outFile << "Passenger " << (i + 1) << ":" << endl;
		outFile << "  Name                  : " << checkInInfo.checkInList[i].passengerName.firstName
			<< " " << checkInInfo.checkInList[i].passengerName.lastName << endl;
		outFile << "  Passport Number       : " << checkInInfo.checkInList[i].passportNumber << endl;
		outFile << "  Contact Person Name   : " << checkInInfo.checkInList[i].contactName.firstName
			<< " " << checkInInfo.checkInList[i].contactName.lastName << endl;
		outFile << "  Contact Person Phone  : " << checkInInfo.checkInList[i].contactMobile << endl;
		outFile << "-------------------------------------" << endl;
	}

	// Invoice footer
	outFile << "=====================================" << endl;
	outFile << "         END OF INVOICE              " << endl;
	outFile << "=====================================" << endl;

	outFile.close(); // Finish writing the invoice file

	cout << "Invoice printed successfully into '" << currentUser << "_invoice.txt'! Please check your folder!" << endl << endl;
	cout << "========= Below is the Sample Invoice ==========" << endl << endl;

	cout << "=====================================" << endl;
	cout << "        JSJK AIRLINES INVOICE        " << endl;
	cout << "=====================================" << endl;
	cout << "Username              : " << currentUser << endl;
	cout << "Tickets Purchased     : " << num_of_passengers << endl;
	cout << "Total Payment         : RM " << fixed << setprecision(2) << payCheckInfo.totalAmount << endl;
	cout << "Payment Status        : " << payCheckInfo.paymentStatus << endl;
	cout << "Check-in Status       : " << checkInInfo.checkInStatus << endl;
	cout << "-------------------------------------" << endl;

	cout << "----------------------------- Flight Schedule -----------------------------" << endl;
	cout << "Departure              : " << routeList[flight_booking[0].depart_flight_from_to]
		<< " | Date: " << flight_booking[0].departure_date
		<< " | Time: " << timeSlot[flight_booking[0].departure_time] << endl;
	cout << "Return                 : " << routeList[flight_booking[0].return_flight_from_to]
		<< " | Date: " << flight_booking[0].return_date
		<< " | Time: " << timeSlot[flight_booking[0].return_time] << endl;
	cout << "---------------------------------------------------------------------------" << endl;

	cout << "--------- Passenger Details ---------" << endl;
	for (int i = 0; i < num_of_passengers; i++) {
		cout << "Passenger " << (i + 1) << ":" << endl;
		cout << "  Name                  : " << checkInInfo.checkInList[i].passengerName.firstName
			<< " " << checkInInfo.checkInList[i].passengerName.lastName << endl;
		cout << "  Passport Number       : " << checkInInfo.checkInList[i].passportNumber << endl;
		cout << "  Contact Person Name   : " << checkInInfo.checkInList[i].contactName.firstName
			<< " " << checkInInfo.checkInList[i].contactName.lastName << endl;
		cout << "  Contact Person Phone  : " << checkInInfo.checkInList[i].contactMobile << endl;
		cout << "-------------------------------------" << endl;
	}

	cout << "=====================================" << endl;
	cout << "         END OF INVOICE              " << endl;
	cout << "=====================================" << endl;

	system("pause"); // Pause for user to view message
}

void coverDecoration() {
	setConsoleColor(3);
	cout << endl;
	cout << "                                                                                                       4eec" << endl;
	cout << "                                                                                                        888L" << endl;
	cout << "                                                                                                         888b" << endl;
	cout << "                                                                                                          ^8888." << endl; //2tabs
	cout << "                                                                                                           '8888c" << endl;
	cout << "                                                                                                             88888L" << endl;
	cout << "                               _______     ____ __    ___    _                            \"8b                 :8888b " << endl;
	cout << "                              / / ___/    / / //_/   /   |  (_)____                        *8b                 888888" << endl;
	cout << "                         __  / /\\__ \\__  / / ,<     / /| | / / ___/                       =8888888888888888888888888888888888888888888a" << endl;
	cout << "                        / /_/ /___/ / /_/ / /| |   / ___ |/ / /                           488\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\" 888888 \"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"\"" << endl;
	cout << "                        \\____//____/\\____/_/ |_|  /_/  |_/_/_/                           .88                   488888%" << endl;
	cout << "                                                                                         \"                   .88888\"" << endl;
	cout << "                                                                                                             8888P" << endl;
	cout << "                                                                                                            d888F" << endl;
	cout << "                                                                                                           J888\"" << endl;
	cout << "                                                                                                          4888\"" << endl;
	cout << "                                                                                                        ,88P" << endl;
	cout << endl << endl << endl;
	setConsoleColor(11);
	cout << "                                   -----------------------------------------------------------------------------------------" << endl;
	cout << "                                   || Wanna plan a seamless journey? Book JSJK International Airlines flight tickets now! ||" << endl;
	cout << "                                   -----------------------------------------------------------------------------------------" << endl;
	setConsoleColor(14);
	cout << endl;
	cout << "                      ============================               ============================               ============================" << endl;
	cout << "                     |                            |             |                            |             |                            |" << endl;
	cout << "                     |    1. Register New User    |             |          2. Login          |             |           3. Quit          |" << endl;
	cout << "                     |                            |             |                            |             |                            |" << endl;
	cout << "                      ============================               ============================               ============================" << endl << endl;
	setConsoleColor(7);
}

void regDecoration() {
	setConsoleColor(11);
	cout << "" << endl;
	cout << "                                      **************************************" << endl;
	cout << "                                      *   JSJK AIRLINE REGISTRATION PORTAL *" << endl;
	cout << "                                      **************************************" << endl;
	setConsoleColor(14);
	cout << "                                                                                               __" << endl;
	cout << "                                                                                              (__)" << endl;
	cout << "                                                                                              |  |\\" << endl;
	cout << "                            ____             _      __             __  _                      |  ||" << endl;
	cout << "                           / __ \\___  ____ _(_)____/ /__________ _/ /_(_)___  ____            |  ||" << endl;
	cout << "                          / /_/ / _ \\/ __ `/ / ___/ __/ ___/ __ `/ __/ / __ \\/ __ \\           |__||" << endl;
	cout << "                         / _, _/  __/ /_/ / (__  ) /_/ /  / /_/ / /_/ / /_/ / / / /           |  |" << endl; //2tabs
	cout << "                        /_/ |_|\\___/\\__, /_/____/\\__/_/   \\__,_/\\__/_/\\____/_/ /_/            |  |" << endl;
	cout << "                                   /____/                                                     |  |" << endl;
	cout << "                                                                                              |  |" << endl;
	cout << "                                                                                              |__|" << endl;
	cout << "                                                                                              \\||/" << endl;
	cout << "                                                                                               \\/" << endl << endl;
	setConsoleColor(7);

}

void loginDecoration() {
	setConsoleColor(11);
	cout << "" << endl;
	cout << "                                                         **************************************" << endl;
	cout << "                                                         *       JSJK AIRLINE LOGIN PORTAL    *" << endl;
	cout << "                                                         **************************************" << endl;
	setConsoleColor(14);
	cout << "                                                                __                _     " << endl;
	cout << "                                                               / /   ____  ____ _(_)___ " << endl;
	cout << "                                                              / /   / __ \\/ __ `/ / __ \\" << endl;
	cout << "                                                             / /___/ /_/ / /_/ / / / / /" << endl; //2tabs
	cout << "                                                            /_____/\\____/\\__, /_/_/ /_/ " << endl;
	cout << "                                                                        /____/          " << endl << endl << endl;
	setConsoleColor(2);
	cout << "                                                              Your account, your travel" << endl;
	setConsoleColor(7);


}

void bookingDecoration() {
	setConsoleColor(11);
	cout << "" << endl;
	cout << "                                               **************************************************" << endl;
	cout << "                                               *    JSJK AIRLINE FLIGHT TICKET BOOKING PORTAL   *" << endl;
	cout << "                                               **************************************************" << endl;
	cout << endl;
	setConsoleColor(14);
	cout << "                            ____              __   _                        __...--~~~~~-._   _.-~~~~~--...__" << endl;
	cout << "                           / __ )____  ____  / /__(_)___  ____ _          //               `V'               \\\\ " << endl;
	cout << "                          / __  / __ \\/ __ \\/ //_/ / __ \\/ __ `/         //                 |                 \\\\ " << endl;
	cout << "                         / /_/ / /_/ / /_/ / ,< / / / / / /_/ /         //__...--~~~~~~-._  |  _.-~~~~~~--...__\\\\" << endl;
	cout << "                        /_____/\\____/\\____/_/|_/_/_/ /_/\\__, /         //__.....----~~~~._\\ | /_.~~~~----.....__\\\\" << endl;
	cout << "                                                       /____/         ====================\\\\|//====================" << endl;
	cout << "                                                                                          `---`" << endl;
	cout << endl;
	setConsoleColor(11);
	cout << "                                              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	cout << "                                              Discover the world with JSJK International Airlines." << endl;
	cout << "                                              ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	setConsoleColor(13);
	cout << endl << endl;
	cout << "                                 If you are flying in a group of 10 or more, let us help you with the arrangements. " << endl;
	cout << "                        Simply let us know your travel details and we'll plan an ideal journey for everyone in your group." << endl;
	setConsoleColor(7);

}

void setConsoleColor(int colorCode) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void paymentDecoration() {
	system("cls");

	setConsoleColor(11);
	cout << R"(  
   .---------------------.
   |  **** JSJK AIR **** |                       **************************************************                           ____     
   |  ______             |                       *          JSJK AIRLINE PAYMENT PORTAL           *                          /\   \    
   | | xxxx |  ****      |                       **************************************************                         /  \___\   
   | |______|            |                                                                                                  \  /   /   
   |   1234 5678 9012    |                  +----------------------------------------------------------+                     \/___/    
   |   VALID 12/34       |                  |  Please follow the instructions to complete your payment.|                     | $$ |      
   |   xxxxxxxxxx        |                  +----------------------------------------------------------+                    /      \     
   '---------------------'                                                                                                 /  $$$$  \    
                                                                                                                          / $$$$$$$$ \   
                                                                                                                          \ $$$$$$$$ /   
                                                                                                                           \________/    
)";

	setConsoleColor(14);
	cout << R"(
                                                   __    ____                                   __ 
                                                 _/ /   / __ \____ ___  ______ ___  ___  ____  / /_
                                                / __/  / /_/ / __ `/ / / / __ `__ \/ _ \/ __ \/ __/
                                               (_  )  / ____/ /_/ / /_/ / / / / / /  __/ / / / /_  
                                              /  _/  /_/    \__,_/\__, /_/ /_/ /_/\___/_/ /_/\__/  
                                              /_/                /____/                            
)" << endl;

	setConsoleColor(13);
	cout << "                                             --------------------------------------------------------\n";
	cout << "                                             | SECURE FLIGHT TICKET PAYMENT | EASY. FAST. RELIABLE. |\n";
	cout << "                                             --------------------------------------------------------\n";

	setConsoleColor(11);
	cout << "                                             ========================================================\n";
	cout << "                                                   S E C U R E   F L I G H T   P A Y M E N T\n";
	cout << "                                             ========================================================\n";

	setConsoleColor(7);
}


void editBookingDecoration() {
	setConsoleColor(11);
	cout << "                                                 **************************************************" << endl;
	cout << "                                                 *         JSJK AIRLINE EDIT BOOKING PORTAL       *" << endl;
	cout << "                                                 **************************************************" << endl;
	cout << endl;
	setConsoleColor(14);
	cout << "                                                 ______    ___ __     __                __   _            " << endl;
	cout << "                                                / ____/___/ (_) /_   / /_  ____  ____  / /__(_)___  ____ _" << endl;
	cout << "                                               / __/ / __  / / __/  / __ \\/ __ \\/ __ \\/ //_/ / __ \\/ __ `/" << endl;
	cout << "                                              / /___/ /_/ / / /_   / /_/ / /_/ / /_/ / ,< / / / / / /_/ / " << endl;
	cout << "                                             /_____/\\__,_/_/\\__/  /_.___/\\____/\\____/_/|_/_/_/ /_/\\__, /  " << endl;
	cout << "                                                                                                 /____/   " << endl << endl << endl << endl;
	setConsoleColor(11);
	cout << "                                                               Modify your flight." << endl;
	cout << endl;
	setConsoleColor(3);
	cout << "               Let us accommodate your plans! If you require a change to your flight date and time, you can do so up to 4 hours before departure. " << endl;
	cout << endl;
	setConsoleColor(7);
}

void checkInDecoration() {
	system("cls");

	setConsoleColor(11);
	cout << R"(  

                                                    **************************************************                     
                                                    *          JSJK AIRLINE CHECK IN PORTAL          *                     
                                                    **************************************************                        

)";

	setConsoleColor(9);
	cout << R"(  
      ______________________________________
      |                                    |               ________              __      ____    
      |    JSJK AIRLINE FLIGHT TICKET      |              / ____/ /_  ___  _____/ /__   /  _/___ 
      |----------------------------------- |             / /   / __ \/ _ \/ ___/ //_/   / // __ \
      |   Departure: KL                    |            / /___/ / / /  __/ /__/ ,<    _/ // / / /
      |   Arrival: Singapore               |            \____/_/ /_/\___/\___/_/|_|  /___/_/ /_/  
      |   Date: xx xx xxxx                 |
      |   Flight: xxxxxxxx                 | 
      |                                    | 
      |____________________________________|
)";

	setConsoleColor(13);
	cout << "                                                          ~~~~~~~~ CHECK-IN SECTION ~~~~~~~~" << endl;
	cout << endl << endl;

	setConsoleColor(10);
	cout << "                                            ================================================================" << endl;
	cout << "                                            |                                                              |" << endl;
	cout << "                                            |  Welcome to JSJK AIRLINE - Check-In Portal! Please follow    |" << endl;
	cout << "                                            |  the instructions below to proceed with your check-in.       |" << endl;
	cout << "                                            |                                                              |" << endl;
	cout << "                                            ================================================================" << endl;
	cout << endl;

	setConsoleColor(11);
	cout << "\n                                            Before proceeding, ensure that you have completed your payment." << endl;
	cout << endl;
	cout << "                                           If payment is successful, you will be able to proceed with the check-in." << endl;
	cout << endl;

	cout << "\n";
	setConsoleColor(14);
	cout << "                                              >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl;
	cout << "                                              >>>>>>>>>>>>>>>>> Ready to proceed with Check-In? <<<<<<<<<<<<<< " << endl;
	cout << "                                              >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl;
	cout << endl;

	setConsoleColor(14);
	cout << "                                              ================================================================" << endl;
	cout << "                                                Please make sure your payment is completed before proceeding. " << endl;
	cout << "                                              ================================================================" << endl;

	setConsoleColor(7);
}


void mainMenuDecoration() {
	setConsoleColor(9);
	cout << "                                                         __  __       _       __  __                  " << endl;
	cout << "                                                        |  \\/  | __ _(_)_ __ |  \\/  | ___ _ __  _   _ " << endl;
	cout << "                                                        | |\\/| |/ _` | | '_ \\| |\\/| |/ _ \\ '_ \\| | | |" << endl;
	cout << "                                                        | |  | | (_| | | | | | |  | |  __/ | | | |_| |" << endl;
	cout << "                                                        |_|  |_|\\__,_|_|_| |_|_|  |_|\\___|_| |_|\\__,_|" << endl;
	cout << endl << endl;
	setConsoleColor(7);

}