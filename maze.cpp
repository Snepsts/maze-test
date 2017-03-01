#include "maze.h" //class def for maze
#include <iostream> //cout for print()
#include <random> //uniform_int_distribution
#include <stack> //stack s for DFS gen and min_steps
#include "universal.h" //randmaze (random numbers)

using std::cout;
using std::uniform_int_distribution;

//default constructor
maze::maze()
{
	for(int i = 0; i < SIZE; i++)
	{
		grid[0][i].atr = Wall;
		grid[SIZE-1][i].atr = Wall;
		grid[i][0].atr = Wall;
		grid[i][SIZE-1].atr = Wall;
		grid[0][i].gen = Visited;
		grid[SIZE-1][i].gen = Visited;
		grid[i][0].gen = Visited;
		grid[i][SIZE-1].gen = Visited;
	}

	for(int x = 0; x < SIZE; x++)
	{
		for(int y = 0; y < SIZE; y++)
		{
			grid[x][y].x = x;
			grid[x][y].y = y;
		}
	}
}

void maze::gen_main()
{
	std::stack<block> s;
	uniform_int_distribution<int> startpt(1, SIZE-3);
	int x;
	int y;

	/* randmaze seems to consistently churn out the same number on it's initial
	call. Afterwards, however, on each subsequent call it appears to be fine.
	We solve this by calling it once before using it. */
	startpt(randmaze); //run the randmaze once before using it
	x = startpt(randmaze); //x = startpt(randmaze);
	y = startpt(randmaze);

	/* We do the following to confirm x (and y) begins on an odd number. If they
	do not, our implementation of DFS will not work correctly. We depend on each
	calculation moving two blocks at a time. If it starts on an even block, the
	"edge" will be two blocks thick. Making them odd ensures the wall stays one
	block thick. */
	if(x % 2 == 0) //if x is even
		x++;
	if(y % 2 == 0) //if y is even
		y++;

	grid[x][y].atr = Open;
	s.push(grid[x][y]);

	while(!s.empty())
	{
		std::string dir;
		dir.clear();

		x = s.top().x;
		y = s.top().y;

		//check if we can check right
		if(x < SIZE-2) //if x is at least 2 less than SIZE
		{ //check right
			if(grid[x+1][y].atr == Unassigned && grid[x+2][y].atr == Unassigned)
				dir += "r";
		}
		//check if we can check up
		if(y < SIZE-2) //if y is at least 2 less than the SIZE
		{ //check up
			if(grid[x][y+1].atr == Unassigned && grid[x][y+2].atr == Unassigned)
				dir += "u";
		}
		//check if we can check left
		if(x > 1) //if x is at least 2
		{ //check left
			if(grid[x-1][y].atr == Unassigned && grid[x-2][y].atr == Unassigned)
				dir += "l";
		}
		//check if we can check up
		if(y > 1) //if y is at least 2
		{ //check up
			if(grid[x][y-1].atr == Unassigned && grid[x][y-2].atr == Unassigned)
				dir += "d";
		}

		switch(gen_next(dir))
		{
			case 'r': //move right
				grid[x+1][y].atr = Open;
				grid[x+2][y].atr = Open;
				s.push(grid[x+2][y]);
				break;
			case 'u': //move up
				grid[x][y+1].atr = Open;
				grid[x][y+2].atr = Open;
				s.push(grid[x][y+2]);
				break;
			case 'l': //move left
				grid[x-1][y].atr = Open;
				grid[x-2][y].atr = Open;
				s.push(grid[x-2][y]);
				break;
			case 'd': //move down
				grid[x][y-1].atr = Open;
				grid[x][y-2].atr = Open;
				s.push(grid[x][y-2]);
				break;
			default:
				s.pop();
				break;
		}
	}
	gen_walls();
	gen_start();
}

char maze::gen_next(const std::string& dir)
{
	if(dir.length() <= 0)
		return ' ';

	uniform_int_distribution<int> letter(0, dir.length()-1);

	return dir[letter(randmaze)];
}

void maze::gen_walls()
{
	for(int x = 0; x < SIZE; x++)
	{
		for(int y = 0; y < SIZE; y++)
		{
			if(grid[x][y].atr == Unassigned)
				grid[x][y].atr = Wall;
		}
	}
}

void maze::gen_start()
{
	int swtch;
	/* Walls:
		0 = left
		1 = up
		2 = right
		3 = down
	*/
	uniform_int_distribution<int> wall(0, 3);

	swtch = wall(randmaze); //we store the value bc we'll use it later

	gen_switch_case(swtch, true);
	gen_exit(swtch);
}

int maze::gen_exit(const int& ent)
{
	int swtch;
	uniform_int_distribution<int> wall(0, 3);

	do
		swtch = wall(randmaze);
	while(swtch == ent);

	gen_switch_case(swtch, false);

	return 0;
}

void maze::gen_switch_case(const int& swtch, const bool& isEnter)
{
	int x, y;
	uniform_int_distribution<int> block(0, SIZE-1);

	switch(swtch)
	{
		case 0: //left
			x = 0;
			y = 0;
			while(grid[x+1][y].atr != Open)
				y = block(randmaze);
			break;
		case 1: //up
			x = 0;
			y = SIZE-1;
			while(grid[x][y-1].atr != Open)
				x = block(randmaze);
			break;
		case 2: //right
			x = SIZE-1;
			y = 0;
			while(grid[x-1][y].atr != Open)
				y = block(randmaze);
			break;
		case 3:
			x = 0;
			y = 0;
			while(grid[x][y+1].atr != Open)
				x = block(randmaze);
			break;
		default:
			cout << "gen_switch_case failed.\n";
			break;
	}

	if(isEnter)
	{ //this is called in the case of gen_start
		grid[x][y].atr = Enter;
		cout << "Enter is: x: " << grid[x][y].x << " y: " << grid[x][y].y << '\n';
		Start = grid[x][y];
	}
	else
	{ //is exit
		grid[x][y].atr = Exit;
		cout << "Exit is: x: " << grid[x][y].x << " y: " << grid[x][y].y << '\n';
		grid[x][y].gen = Unvisited; //for min_steps
		End = grid[x][y];
	}
}

int maze::min_steps()
{
	int count = 0;
	int x = Start.x;
	int y = Start.y;
	bool isFinished = false;
	std::stack<block> check, move, temp;

	/*The following "if" checks are to keep us from checking parts of the array
	that are out of bounds during the first block (start). I'm not sure if it
	would throw errors or break the compiler but it's a good idea regardless to
	stay _inside_ the bounds of our arrays :) */
	if(x == 0)
		x++;
	else if(x == SIZE-1)
		x--;
	else if (y == 0)
		y++;
	else if (y == SIZE-1)
		y--;

	grid[x][y].gen = Visited;
	move.push(grid[x][y]);
	count++; //technically we took a step here

	while(!isFinished){ //Search loop
		count++; //took one 'step'

		while(!move.empty()) //while there are still coords
		{
			x = move.top().x; //set x to new coord x
			y = move.top().y; //set y to new coord y

			if(check_spot(x+1, y)) //call check_spot for (x+1, y)
			{ //if (x+1, y) is open and not visted, push it to our stacks
				temp.push(grid[x+1][y]);
				check.push(grid[x+1][y]);
			}
			if(check_spot(x-1, y)) //call check_spot for (x-1, y)
			{ //if (x-1, y) is open and not visted, push it to our stacks
				temp.push(grid[x-1][y]);
				check.push(grid[x-1][y]);
			}
			if(check_spot(x, y+1)) //call check_spot for (x, y+1)
			{ //if (x, y+1) is open and not visted, push it to our stacks
				temp.push(grid[x][y+1]);
				check.push(grid[x][y+1]);
			}
			if(check_spot(x, y-1)) //call check_spot for (x, y-1)
			{ //if (x, y-1) is open and not visted, push it to our stacks
				temp.push(grid[x][y-1]);
				check.push(grid[x][y-1]);
			}

			move.pop(); //remove current coord
		}

		while(!check.empty()) //check all coords
		{
			if(check.top() == End) //if one of our next coords is
				isFinished = true;
			check.pop();
		}

		move = temp; //next coords are now current coords
		temp = check; //instead of writing another while loop, = empty stack
	}

	return count;
}

bool maze::check_spot(const int& x, const int& y)
{
	bool retvar = true;

	if(grid[x][y].atr != Wall && grid[x][y].gen != Visited)
		grid[x][y].gen = Visited;
	else //if it's a Wall just return false
		retvar = false;

	return retvar;
}

void maze::print() const
{
	for(int y = SIZE-1; y >= 0; y--) //y axis, inverted so (0,0) is bot left
	{
		for(int x = 0; x < SIZE; x++) //x axis
		{
			switch(grid[x][y].atr)
			{
				case -1:
					cout << "B"; //Start/Beginning
					break;
				case 0:
					cout << "E"; //Exit/End
					break;
				case 1:
					cout << " "; //Open
					break;
				case 2:
					cout << "x"; //Wall
					break;
				case 3:
					cout << "n"; //Invalid (Unassigned)
					break;
				default:
					std::cerr << "ERROR\n"; //In case of error
					break;
			}
			cout << " "; //Space to make the formatting look nicer
		} //end x
		cout << '\n'; //newline for each new line
	} //end y
}
