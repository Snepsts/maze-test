/* maze-test
Copyright (C) 2017 Michael Ranciglio

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#include <iostream>
#include <chrono>
#include "maze.h"

using namespace std::chrono;
using namespace std;

int main()
{
	high_resolution_clock::time_point start = high_resolution_clock::now();
	maze m;

	high_resolution_clock::time_point t1 = high_resolution_clock::now();
	m.gen_main();
	high_resolution_clock::time_point t2 = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(t2 - t1).count();

	cout << "Generating a " << SIZE << "x" << SIZE << " maze took " << duration << " microseconds." << '\n';
	m.print();
	cout << "The minimum steps it will take to complete this maze is: ";
	cout << m.min_steps() << '\n';

	high_resolution_clock::time_point finish = high_resolution_clock::now();
	auto total_duration = duration_cast<microseconds>(finish - start).count();
	cout << "The time it took to run all of that was " << total_duration << " microseconds.\n";

	return 0;
}
