#include <iostream>
#include <string>
#include <limits>
#include "asymmetric.h"

using namespace std;

int main() {
	ASYMMETRIC asymmetric;
	int option = 1;
	cout << "Public-Key CryptoSystem" << endl << endl;
	while (option != 3) {
		string task = "";
		ll user_input = NULL;
		cout << "MAIN MENU" << endl;
		cout << "Options:" << endl;
		cout << "1 - Create New Key Set" << endl;
		cout << "2 - Use Existing Key Set" << endl;
		cout << "3 - Quit" << endl;
		do {
			while (cout << "Enter option #: " && !(cin >> option)) {
				cin.clear(); //clear bad input flag
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
				cout << "Invalid input; please re-enter.\n";
			}
			cout << endl;
		} while (option < 1 || option > 3);

		if (option == 1) {
			cout << "CREATE NEW KEY SET" << endl;
			while (cout << "Enter a Random #: " && !(cin >> user_input)) {
				cin.clear(); //clear bad input flag
				cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
				cout << "Invalid input; please re-enter.\n";
			}
			cout << endl;
		}
		if (option == 1 || option == 2) {
			if (asymmetric.keyMngr(user_input)) {
				while (option != 3) {
					cout << "Options:" << endl;
					cout << "1 - Encrypt" << endl;
					cout << "2 - Decrypt" << endl;
					cout << "3 - Return to Main Menu" << endl;
					do {
						//https://stackoverflow.com/questions/10349857/how-to-handle-wrong-data-type-input
						while (cout << "Enter option #: " && !(cin >> option)) {
							cin.clear(); //clear bad input flag
							cin.ignore(numeric_limits<streamsize>::max(), '\n'); //discard input
							cout << "Invalid input; please re-enter.\n";
						}
						cout << endl;
					} while (option < 1 || option > 3);
					if (option == 1)
						task = "encrypt";
					else if (option == 2)
						task = "decrypt";
					else
						task = "";
					if (!task.empty())
						if (asymmetric.readText(task))
							if (asymmetric.cipher(task))
								if (asymmetric.writeText(task)) cout << "Process Complete" << endl << endl;
								else cout << "Error: Could not write to file" << endl << endl;
							else cout << "Error: Could not execute cipher code" << endl << endl;
						else cout << "Error: Could not read file" << endl << endl;
				}
			}
			option = 1;
		}
	}
	return 0;
}