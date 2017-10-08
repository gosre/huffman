#include <iostream>
#include <string.h>
#include "Utilities.h"
#include <stdlib.h>
using namespace std;

/**
 * Asks for input stream response, then validates the given characters until correct.
 * @param var - Pointer to the stored string
 * @param min - minimum input length
 * @param max - maximum input length
 * @param task - Question to be asked.
 * @param type - ValidationType to be cross-checked with input.
 */
void validateString(char* var, const int min, const int max, const string task, const ValidationType type) {
	char input[256];
	cout << task << endl;
	cin >> input;
	while (cin.fail() || !validateChars(input, type) || strlen(input) < min || strlen(input) > max) {
		cin.clear();
		cin.ignore(10000, '\n');
		clearScreen();
		cout << Color::getAnsiCode(RED) << "Invalid input. " << validationStrings[type] << " characters only! (" << min << "-" << max << (type == NUMERIC ? " digits" : " characters") << ")" << Color::toDefault() << endl;
		cout << task << endl;
		cin >> input;
	}
	strcpy(var, input);
}

/**
 * Clears the screen
 **/
void clearScreen() {
	cout << "\e[2J\e[;H";
}

/**
 *
 * Gets an Integer from a validation string
 * return int
 **/
int getInteger(const int min, const int max, const string task) {
	char input[256];
	int i = -1;
	while (i < min || i > max) {
		validateString(input, 1, 9, task, NUMERIC); //max 999 million
		//clearScreen();
		i = atoi(input);
	}
	return i;
}


/**
 * Compares the given string to the validation type specified. Uses ASCII char codes
 * @param str - given string
 * @param ValidationType type - type of valdiation to be compared against
 * @return boolean - true if string contains correct chars, false if not
 */
bool validateChars(const char str[], const ValidationType type) {
	for (int i = 0; i < strlen(str); i++) {
		char c = str[i];
		if (!(type == NUMERIC ? (c >= '0' && c <= '9') :
				(c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A') || (type == ALPHA_NUMERIC ? (c >= '0' && c <= '9') : false))) {
			return false;
		}
	}
	return true;
}

/**
 * Exits with an error message
 * @param String error - the error message to display
 **/
void error(const std::string error) {
    cout << Color::getAnsiCode(RED) << "Exiting Application, Error: " << error << Color::toDefault() << endl;
    exit(1);
}

