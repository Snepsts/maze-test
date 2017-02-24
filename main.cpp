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
	m.gen_walls();
	m.create_entrance();
	cout << "After entrance creation: \n";
	m.print();
	cout << "The minimum steps it will take to complete this maze is: ";
	cout << m.min_steps() << '\n';

	high_resolution_clock::time_point finish = high_resolution_clock::now();
	auto total_duration = duration_cast<microseconds>(finish - start).count();
	cout << "The time it took to run all of that was " << total_duration << " microseconds.\n";

	return 0;
}
