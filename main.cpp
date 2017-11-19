/* maze-test
Copyright (C) 2017 Michael Ranciglio

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include <iostream>
#include <chrono>
#include "maze.h"

using namespace std::chrono;
using namespace std;

void print_choices(const maze& m, const int& x, const int& y);

int main()
{
	//part 1, initializing and seeing how fast it is
	high_resolution_clock::time_point start = high_resolution_clock::now();
	maze m;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m.gen_main();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();

	cout << "Generating a " << SIZE << "x" << SIZE << " maze took " << duration << " microseconds." << '\n';
	//m.print();
	cout << "The minimum steps it will take to complete this maze is: ";
	cout << m.min_steps() << '\n';

	high_resolution_clock::time_point finish = high_resolution_clock::now();
	auto total_duration = duration_cast<microseconds>(finish - start).count();
	cout << "The time it took to run all of that was " << total_duration << " microseconds.\n";

	//part 2, testing the maze itself
	bool whilevar = true;
	bool completed = true;
	int choicevar, x, y;

	cout << "Welcome to the maze tester.\n";

	do {
		m.print();

		x = m.get_x();
		y = m.get_y();
		print_choices(m, x, y);

		cin >> choicevar;
		switch (choicevar) {
			case 0: //right
				if(m.move(x+1, y) == 2) //check if the maze is completed
					whilevar = false;
				break;
			case 1: //up
				if(m.move(x, y+1) == 2) //check if the maze is completed
					whilevar = false;
				break;
			case 2: //down
				if(m.move(x, y-1) == 2)
					whilevar = false;
				break;
			case 3: //left
				if(m.move(x-1, y) == 2) //check if the maze is completed
					whilevar = false;
				break;
			default: //otherwise, end
				whilevar = false;
				completed = false;
				break;
		}
	} while(whilevar);

	if(completed)
		cout << "Congrats! You have completed the maze!!!\n";

	return 0;
}

void print_choices(const maze& m, const int& x, const int& y)
{
	//since we're using namespace std we don't need the std:: on size_t & string
	string dir = m.get_directions(x, y); //get the avail. directions
	size_t f; //for checking if the string contains a char

	f = dir.find('r'); //check for right
	if(f != string::npos) //if it's not equal to "not found"
		cout << "0 - Move right.\n";

	f = dir.find('u'); //check for up
	if(f != string::npos) //if it's not equal to "not found"
		cout << "1 - Move up.\n";

	f = dir.find('d'); //check for down
	if(f != string::npos) //if it's not equal to "not found"
		cout << "2 - Move down.\n";

	f = dir.find('l'); //check for left
	if(f != string::npos) //if it's not equal to "not found"
		cout << "3 - Move left.\n";

	cout << "4 - Exit.\n";
}
