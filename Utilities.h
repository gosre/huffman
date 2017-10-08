#ifndef UTILITIES_H
#define UTILITIES_H

#include <iostream>
#include <sstream>
#include <map>
#include <string>
#include <iterator>
#include <stdlib.h>

using namespace std;

/******************
 * Outstream Manipulation
 ***************/

enum Colors {DEFAULT = 0, BLACK = 30, RED = 31, GREEN = 32, CYAN = 36, BG_GREEN = 42, BG_WHITE = 47, BG_DEFAULT = 49}; //ansi

class Color {
    public:
        static std::string getAnsiCode(Colors color) {
            std::stringstream ss;
            ss << "\e[" << color << "m";
            return ss.str();
        }

        static std::string getAnsiCode(Colors foreground, Colors background) {
            std::stringstream ss;
            ss << "\e[" << foreground << ";" << background << "m";
            return ss.str();
        }

        static std::string toDefault() {
            return getAnsiCode(DEFAULT);
        }
};

/**
 * Clears the screen of characters ([2j]) and sets cursor position to 0,0 ([;H)
 * using ANSI escape sequences
 */
void clearScreen();

/***************
 * Input Validation
 ***************/
enum ValidationType {ALPHA, ALPHA_NUMERIC, NUMERIC};
const std::string validationStrings[] = {"Alpha", "Alpha-Numeric", "Numeric"};

bool validateChars(const char[], const ValidationType);
void validateString(char*, const int min, const int max, const std::string, const ValidationType);
int getInteger(const int min, const int max, const std::string);

void error(const std::string);

/**
 * Menu
 */
class Menu {
	private:
		map<int, string> menu;
		/* Prints the menu */
		void printMenu() {
			cout << "Menu:" << endl;
			map<int, string>::iterator iterator;
			for(iterator = menu.begin(); iterator != menu.end(); iterator++) {
				cout << iterator->first << ".) " << iterator->second << endl;
			}
		}

		/* Returns true if an option key exists */
		bool optionExists(int option) {
			return menu.find(option) != menu.end();
		}
	public:
		/* Adds option to the menu */
		void addOption(int option, string name) {
			menu[option] = name;
		}

		/* Requests input for a menu element */
		int requestInput() {
			char input[2];
			int selection = -1;
			while (!optionExists(selection)) {
				printMenu();
				validateString(input, 1, 1, "Choose an Option:", NUMERIC);
				selection = atoi(input);
				clearScreen();
				if (!optionExists(selection)) {
					cout << Color::getAnsiCode(RED) << "Error, incorrect choice. Please try again." << Color::toDefault() << endl;
				}
			}
			return selection;
		}
};


#endif
