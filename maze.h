#ifndef __MAZE_H__
#define __MAZE_H__

#include <string>

const int SIZE = 21;

enum attribute {Enter = -1, Exit = 0, Open = 1, Wall = 2, Unassigned = 3};
enum generation {Unvisited = 0, Visited = 1};

/* maze blocks
attribute atr:
	-1 = enter (starting block)
	0 = exit
	1 = open
	2 = wall
	3 = unassigned (there should be 0 of these when the maze is fully generated)
generation gen: //only for maze generation purposes
	0 = unvisited
	1 = visited
isSeen:
	true = draw it on the map
	false = show it as not seen yet
*/
struct block{
	attribute atr = Unassigned;
	bool isSeen = false;
	generation gen = Unvisited;
	int x, y; //these are for keeping track of the x/y for gen purposes
	bool operator==(const block& b) const{ //for check_spot()
		return(this->x == b.x && this->y == b.y && this->atr == b.atr);
	}
};

class maze
{
public:
	maze();
	~maze() = default;
	void gen_main();
	char gen_next(const std::string& dir);
	void gen_walls();
	void create_entrance();
	int create_exit(const int& ent);
	void create_switch_case(const int& swtch, const bool& isEnter);
	int min_steps();
	bool check_spot(const int& x, const int& y);
	void print() const;
private:
	block grid[SIZE][SIZE];
	block Start, End; //capitalize bc end all lowers throws an error
};

#endif //__MAZE_H__
