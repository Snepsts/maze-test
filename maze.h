#ifndef __MAZE_H__
#define __MAZE_H__

#include <string>

const int SIZE = 21;

enum attribute {Enter = -1, Exit = 0, Open = 1, Wall = 2, Unassigned = 3};
enum generation {Unvisited = 0, Visited = 1};

/* maze blocks
 * attribute atr:
 *   -1 = enter (starting block)
 *   0 = exit
 *   1 = open
 *   2 = wall
 *   3 = unassigned (there should be 0 of these when the maze is fully generated)
 * generation gen: //only for maze generation purposes
 *   0 = unvisited
 *   1 = visited
 * isSeen:
 *   true = draw it on the map
 *   false = show it as not seen yet
 */
struct block
{
	attribute atr = Unassigned;
	bool isSeen = false;
	generation gen = Unvisited;
	int x, y; //these are for keeping track of the x/y for gen purposes
	bool operator==(const block& b) const //for check_spot()
	{ return (this->x == b.x && this->y == b.y && this->atr == b.atr); }
};

class maze
{
public:
	/* default constructor
	 * Define the walls of the maze and assign each block it's (x, y) value.
	 */
	maze();

	/* destructor
	 * Set to default bc I don't know what to do with it
	 */
	~maze() = default;

	/* function gen_main
	 * Upon calling this the maze generation is started.
	 *
	 * Calls gen_next each iteration of its loop.
	 * Calls gen_walls and gen_start upon completion.
	 */
	void gen_main();

	/* function min_steps
	 * Runs through the maze until the end is reached. Then returns the amount
	 * of "steps" it took. It runs through open blocks that have not been run
	 * through yet, and eventually it will find the end. Since it gurantees
	 * only touching each block once, the path it takes to reach the end will
	 * always be the shortest.
	 *
	 * PLANNED: Use min_steps to determine the placement of the end such that
	 * there is a range of minimum steps to complete the maze has to have.
	 * (i.e): if the maze must be 40-60 steps, the exit will continue to be
	 * placed until it is 40-60 steps away from the beginning.
	 */
	int min_steps();

	/* function print
	 * Called whenever needed.
	 *
	 * Prints out the maze to the command line.
	 */
	void print() const;

private:
	//Methods:
	/* function gen_next
	 * Called every iteration of gen_main's maze generation loop.
	 *
	 * Takes a string dir. This string dir has all the possible directions
	 * ( u(p), d(own), l(eft), r(ight) ) it chooses randomly from its choices
	 * (unless it's only one) and returns it.
	 */
	char gen_next(const std::string& dir);

	/* function gen_walls
	 * Only called at the end of gen_main's maze generation loop.
	 *
	 * Takes all "Unassigned" blocks and turns them into "Wall"s. That's it.
	 */
	void gen_walls();

	/* function gen_start
	 * Only called from gen_main after calling gen_walls.
	 *
	 * Creates the entrance of the maze, which is guaranteed to be connected to
	 * an "Open" block in the maze grid (as one would expect).
	 *
	 * Calls gen_switch_case.
	 * Calls gen_exit upon completion.
	 */
	void gen_start();

	/* function gen_exit
	 * Only called after gen_start.
	 *
	 * Creates the exit of the maze, which is guaranteed to be connected to an
	 * "Open" block in the maze grid (as one would expect). Also, the exit will
	 * never be on the same wall that "Enter" is located on.
	 *
	 * Calls gen_switch_case.
	 */
	int gen_exit(const int& ent);

	/* function gen_switch_case
	 * Called from gen_start and gen_exit
	 *
	 * Since gen_start and gen_exit use almost identical code for randomly
	 * selecting a spot, we move it to a separate function to save from having
	 * 30 nearly identical lines of code.
	 * Takes an int and a bool. The int determines what side the wall is on, and
	 * the bool determines whether gen_start called it or gen_exit called it.
	 */
	void gen_switch_case(const int& swtch, const bool& isEnter);

	/* function check_spot
	 * Called from min_steps multiple times each iteration of it's loop.
	 *
	 * Takes an x and a y for the coordinates.
	 * Sees whether or not the block (denoted by the x y coordinates) is a wall
	 * and if it's already been visited. If it is either of those, false is
	 * returned. The other case, if it's open __and__ has not been visited, the
	 * function returns true.
	 */
	bool check_spot(const int& x, const int& y);

	//Members:
	block grid[SIZE][SIZE];
	block Start, End; //capitalize bc end all lowers throws an error
};

#endif //__MAZE_H__
