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
	//set the borders
	for (uint8_t i = 0; i < SIZE; i++) {
		//set to walls
		grid[0][i].atr = Wall;
		grid[SIZE-1][i].atr = Wall;
		grid[i][0].atr = Wall;
		grid[i][SIZE-1].atr = Wall;
		//set to visited for min_steps
		grid[0][i].gen = Visited;
		grid[SIZE-1][i].gen = Visited;
		grid[i][0].gen = Visited;
		grid[i][SIZE-1].gen = Visited;
	}

	for (uint8_t x = 0; x < SIZE; x++) {
		for (uint8_t y = 0; y < SIZE; y++) {
			grid[x][y].x = x;
			grid[x][y].y = y;
		}
	}
}

void maze::gen_main()
{
	std::stack<block> s;
	uniform_int_distribution<uint8_t> startpt(1, SIZE-3);
	uint8_t x;
	uint8_t y;

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
	if (x % 2 == 0) //if x is even
		x++;
	if (y % 2 == 0) //if y is even
		y++;

	grid[x][y].atr = Open;
	s.push(grid[x][y]);

	while (!s.empty()) {
		x = s.top().x;
		y = s.top().y;

		std::string dir = get_directions(x, y, true); //get all avail. directions

		switch(gen_next(dir)) {
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

/* get_directions
This function may look a little confusing. Here's the basics:
 - If isGen is true, we do a DFS check (check the next two blocks) to see if the
   blocks in question have been assigned an attribute yet.
 - If isGen is false, we do a basic check (only the next block) to see if the
   block is open or the exit.

Regardless of isGen's state, if the evaluation is true, we add the char
representing a direction to the string that we're returning. d is down, l is
left, and yeah you get the point. This string gets returned and is dealt with
accordingly.
*/
std::string maze::get_directions(const uint8_t& x, const uint8_t& y, bool isGen) const
{
	std::string dir;
	dir.clear(); //ensure the string is empty before starting

	//check if we can check right
	if (x < SIZE-1) { //if x is at least 1 less than SIZE, check right
		if (isGen) { //if the function was called from gen_main
			if (grid[x+1][y].atr == Unassigned && grid[x+2][y].atr == Unassigned)
				dir += "r";
		} else { //other purposes
			if (grid[x+1][y].atr == Open || grid[x+1][y].atr == Exit)
				dir += "r";
		}
	}
	//check if we can check up
	if (y < SIZE-1) { //if y is at least 1 less than the SIZE, check up
		if (isGen) { //if the function was called from gen_main
			if (grid[x][y+1].atr == Unassigned && grid[x][y+2].atr == Unassigned)
				dir += "u";
		} else { //other purposes
			if (grid[x][y+1].atr == Open || grid[x][y+1].atr == Exit)
				dir += "u";
		}
	}
	//check if we can check left
	if (x > 0) { //if x is at least 1, check left
		if (isGen) { //if the function was called from gen_main
			if (grid[x-1][y].atr == Unassigned && grid[x-2][y].atr == Unassigned)
				dir += "l";
		} else { //other purposes
			if (grid[x-1][y].atr == Open || grid[x-1][y].atr == Exit)
				dir += "l";
		}
	}
	//check if we can check up
	if (y > 0) { //if y is at least 1, check up
		if (isGen) { //if the function was called from gen_main
			if (grid[x][y-1].atr == Unassigned && grid[x][y-2].atr == Unassigned)
				dir += "d";
		} else { //other purposes
			if (grid[x][y-1].atr == Open || grid[x][y-1].atr == Exit)
				dir += "d";
		}
	}

	return dir; //return string
}

char maze::gen_next(const std::string& dir)
{
	if (dir.length() <= 0)
		return ' ';

	uniform_int_distribution<uint8_t> letter(0, dir.length()-1);

	return dir[letter(randmaze)];
}

void maze::gen_walls()
{
	for (uint8_t x = 0; x < SIZE; x++) {
		for (uint8_t y = 0; y < SIZE; y++) {
			if (grid[x][y].atr == Unassigned)
				grid[x][y].atr = Wall;
		}
	}
}

void maze::gen_start()
{
	uint8_t swtch;
	/* Walls:
		0 = left
		1 = up
		2 = right
		3 = down */
	uniform_int_distribution<uint8_t> wall(0, 3);

	swtch = wall(randmaze); //we store the value bc we'll use it later

	gen_switch_case(swtch, true); //set the entrance
	gen_exit(swtch);
}

void maze::gen_exit(const uint8_t& ent)
{
	uint8_t swtch;
	uniform_int_distribution<uint8_t> wall(0, 3);

	do {
		swtch = wall(randmaze);
	} while (swtch == ent); //this ensures the start and end will be on diff sides

	gen_switch_case(swtch, false); //set the exit
	gen_dead_end(); //set our dead ends
	gen_finish();
}

void maze::gen_switch_case(const uint8_t& swtch, const bool& isEnter)
{
	uint8_t x, y;
	uniform_int_distribution<uint8_t> block(0, SIZE-1);

	/* This switch statement looks to see what side of the wall was determined
	and looks to ensure the block next to the entrance/exit is open. Until it is
	open, it keeps trying a new random value until it is in a valid spot. */
	switch(swtch) {
		case 0: //left
			x = 0;
			y = 0;
			while (grid[x+1][y].atr != Open)
				y = block(randmaze);
			break;
		case 1: //up
			x = 0;
			y = SIZE-1;
			while (grid[x][y-1].atr != Open)
				x = block(randmaze);
			break;
		case 2: //right
			x = SIZE-1;
			y = 0;
			while (grid[x-1][y].atr != Open)
				y = block(randmaze);
			break;
		case 3: //down
			x = 0;
			y = 0;
			while (grid[x][y+1].atr != Open)
				x = block(randmaze);
			break;
		default:
			cout << "gen_switch_case failed.\n";
			exit(1); //stop
			break;
	}

	if (isEnter) { //this is called in the case of gen_start
		grid[x][y].atr = Enter;
		cout << "Enter is: x: " << (int)grid[x][y].x << " y: " << (int)grid[x][y].y << '\n';
		Start = grid[x][y];
	} else { //is exit
		grid[x][y].atr = Exit;
		cout << "Exit is: x: " << (int)grid[x][y].x << " y: " << (int)grid[x][y].y << '\n';
		grid[x][y].gen = Unvisited; //for min_steps
		End = grid[x][y];
	}
}

void maze::gen_dead_end()
{
	for (uint8_t y = 1; y < SIZE-1; y++) { //y axis staying inside of the boundary
		for (uint8_t x = 1; x < SIZE-1; x++) { //x axis inside of boundary
			if (grid[x][y].atr == Open) {
				uint8_t counter = 0; //count the walls surrounding our block

				if (grid[x+1][y].atr == Wall) //check right
					counter++;
				if (grid[x-1][y].atr == Wall) //check left
					counter++;
				if (grid[x][y+1].atr == Wall) //check up
					counter++;
				if (grid[x][y-1].atr == Wall) //check down
					counter++;

				if (counter == 3) //if three of four are walls, it's a dead end
					grid[x][y].is_deadend = true; //set the dead end
			} //end if != Wall
		} //end x
	} //end y
}

uint8_t maze::min_steps()
{
	uint8_t count = 0;
	uint8_t x = Start.x;
	uint8_t y = Start.y;
	bool is_finished = false;
	std::stack<block> check, move, temp;

	/* The following "if" checks are to keep us from checking parts of the array
	that are out of bounds during the first block (start). I'm not sure if it
	would throw errors or break the compiler but it's a good idea regardless to
	stay _inside_ the bounds of our arrays :) */
	if (x == 0)
		x++;
	else if (x == SIZE-1)
		x--;
	else if (y == 0)
		y++;
	else if (y == SIZE-1)
		y--;

	grid[x][y].gen = Visited;
	move.push(grid[x][y]);
	count++; //technically we took a step here

	while (!is_finished) { //Search loop
		count++; //took one 'step'

		while (!move.empty()) { //while there are still coords
			x = move.top().x; //set x to new coord x
			y = move.top().y; //set y to new coord y

			if (check_spot(x+1, y)) { //if (x+1, y) is open and not visted,
				temp.push(grid[x+1][y]); //push it to our stacks
				check.push(grid[x+1][y]);
			}
			if (check_spot(x-1, y)) { //if (x-1, y) is open and not visted...
				temp.push(grid[x-1][y]);
				check.push(grid[x-1][y]);
			}
			if (check_spot(x, y+1)) { //if (x, y+1) is open and not visted...
				temp.push(grid[x][y+1]);
				check.push(grid[x][y+1]);
			}
			if (check_spot(x, y-1)) { //if (x, y-1) is open and not visted...
				temp.push(grid[x][y-1]);
				check.push(grid[x][y-1]);
			}

			move.pop(); //remove current coord
		}

		while (!check.empty()) { //check all coords
			if (check.top() == End) { //if one of our next coords is the end
				is_finished = true; //then we're done
				break; //exit loop bc we're done
			}
			check.pop();
		}

		move = temp; //next coords are now current coords
		temp = check; //instead of writing another while loop, = empty stack
	}

	return count;
}

bool maze::check_spot(const uint8_t& x, const uint8_t& y)
{
	bool retvar = true;

	if (grid[x][y].atr != Wall && grid[x][y].gen != Visited)
		grid[x][y].gen = Visited;
	else //if it's a Wall just return false
		retvar = false;

	return retvar;
}

void maze::print() const
{
	for (uint8_t h = 0; h < SIZE; h++) { //y >= 0 does not work with uint8_t, so change approach
		uint8_t y = SIZE-1 - h; //y axis, inverted so (0,0) is bot left,
		for (uint8_t x = 0; x < SIZE; x++) { //x axis
			if (!grid[x][y].is_seen) { //if it hasn't been seen yet
				cout << "m"; //for mystery ooOOOoOOoOOOOOo spooky
			} else { //otherwise it's been seen so we can draw it
				switch(grid[x][y].atr) {
					case Enter:
						if (grid[x][y].has_player) //check for a player
							cout << "P"; //Player
						else
							cout << "B"; //Start/Beginning
						break;
					case Exit:
						if (grid[x][y].has_player) //check for a player
							cout << "P"; //Player
						else
							cout << "E"; //Exit/End
						break;
					case Open:
						if (grid[x][y].has_player) //check for a player
							cout << "P"; //Player
						else if (grid[x][y].is_deadend) //check for a dead end
							cout << "d"; //Dead End
						else
							cout << " "; //Open
						break;
					case Wall:
						cout << "x"; //Wall
						break;
					case Unassigned:
						cout << "n"; //Invalid (Unassigned)
						break;
					default:
						std::cerr << "ERROR\n"; //In case of error
						break;
				} //end switch
			} //end else
			cout << " "; //Space to make the formatting look nicer
		} //end x
		cout << '\n'; //newline for each new line
	} //end y
}

std::string maze::to_string() const
{
	std::string r = ""; //string to be returned

	for (uint8_t y = SIZE-1; y >= 0; y--) { //y axis, inverted so (0,0) is bot left
		for (uint8_t x = 0; x < SIZE; x++) { //x axis
			if (!grid[x][y].is_seen) { //if it hasn't been seen yet
				r += "m"; //for mystery ooOOOoOOoOOOOOo spooky
			} else { //otherwise it's been seen so we can draw it
				switch(grid[x][y].atr) {
					case Enter:
						if (grid[x][y].has_player) //check for a player
							r += "P"; //Player
						else
							r += "B"; //Start/Beginning
						break;
					case Exit:
						if (grid[x][y].has_player) //check for a player
							r += "P"; //Player
						else
							r += "E"; //Exit/End
						break;
					case Open:
						if (grid[x][y].has_player) //check for a player
							r += "P"; //Player
						else if (grid[x][y].is_deadend) //check for a dead end
							r += "d"; //Dead End
						else
							r += " "; //Open
						break;
					case Wall:
						r += "x"; //Wall
						break;
					case Unassigned:
						r += "n"; //Invalid (Unassigned)
						break;
					default:
						r += "ERROR\n"; //In case of error
						break;
				} //end switch
			} //end else
			r += " "; //Space to make the formatting look nicer
		} //end x
		r += '\n'; //newline for each new line
	} //end y

	return r;
}

uint8_t maze::move(const uint8_t& x, const uint8_t& y) //it would return a bool but we need a finished case too
{ //this is where a Russian ternary computer would come in handy :P
	if (grid[x][y].atr == Exit) {
		grid[cx][cy].has_player = false; //leave this block
		grid[x][y].has_player = true; //enter this block

		cx = x; cy = y; //set our x and y coords accordingly

		return 2; //return finished with maze
	} else if (grid[x][y].atr != Wall) { //check whether or not it's a wall
		grid[cx][cy].has_player = false; //leave this block
		grid[x][y].has_player = true; //enter this block

		grid[x+1][y].is_seen = true; grid[x-1][y].is_seen = true; //set all blocks
		grid[x][y+1].is_seen = true; grid[x][y-1].is_seen = true; //around this
		grid[x+1][y+1].is_seen = true; grid[x+1][y-1].is_seen = true; //block to
		grid[x-1][y+1].is_seen = true; grid[x-1][y-1].is_seen = true; //be seen

		cx = x; cy = y; //set our x and y coords accordingly

		return 1; //return successful move
	} else {
		return 0; //return unsuccessful move
	}
}

void maze::gen_finish()
{
	//work from the "start" block
	uint8_t x = Start.x;
	uint8_t y = Start.y;

	cx = x; cy = y; //set the "character"'s (x, y)
	grid[x][y].has_player = true; //and set the player
	grid[x][y].is_seen = true; //set the enter point to seen

	if (x == 0) { //if it starts on the left
		grid[x][y+1].is_seen = true; //block above
		grid[x][y-1].is_seen = true; //block below
		grid[x+1][y].is_seen = true; //block to the right
		grid[x+1][y+1].is_seen = true; //right + up
		grid[x+1][y-1].is_seen = true; //right + down
	} else if (x == SIZE-1) { //if it starts on the right
		grid[x][y+1].is_seen = true; //block above
		grid[x][y-1].is_seen = true; //block below
		grid[x-1][y].is_seen = true; //block to the left
		grid[x-1][y+1].is_seen = true; //left + up
		grid[x-1][y-1].is_seen = true; //left + down
	} else if (y == 0) { //if it starts on the bottom
		grid[x+1][y].is_seen = true; //block to the right
		grid[x-1][y].is_seen = true; //block to the left
		grid[x][y+1].is_seen = true; //block above
		grid[x+1][y+1].is_seen = true; //up + right
		grid[x-1][y+1].is_seen = true; //up + left
	} else if (y == SIZE-1) { //if it starts on the top
		grid[x+1][y].is_seen = true; //block to the right
		grid[x-1][y].is_seen = true; //block to the left
		grid[x][y-1].is_seen = true; //block below
		grid[x+1][y-1].is_seen = true; //down + right
		grid[x-1][y-1].is_seen = true; //down + left
	} else { //error
		cout << "ERROR in gen_finish, cannot determine maze start position.\n";
	}
}
