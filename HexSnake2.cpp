#include <chrono>
#include <thread>
#include <iostream>
#include <conio.h>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

#define WIDTH 5
#define HEIGHT 10

typedef pair <int, int> coordinate;

class Apple{
public:
	coordinate position;

	bool eaten(int x, int y) {
		if ((x == position.first) && (y == position.second)){
			return true;
		}
		return false;
	}

	coordinate new_position(){
		//int lowest = 1, highest_x = WIDTH, highest_y = HEIGHT;
		//int range_x = (highest_x - lowest) + 1;
		//int range_y = (highest_y - lowest) + 1;
		//int x = lowest + int(range_x * rand()/RAND_MAX + 1.0);
		//int y = lowest + int(range_y * rand()/RAND_MAX + 1.0);
		srand((unsigned)time(0));
		int x = (rand()%WIDTH+1);
		int y = (rand()%(HEIGHT-1)+1);
		position = make_pair(x, y);
		return position; // -1 to account for shape of hexes, +1 to account to make rand inclusive
	}

	void print(){
		cout << position.first <<  ' , ' << position.second << endl;
	}
};

class Snake{
public:
	vector<coordinate> snake;
	void add_coordinate(int x, int y){
		snake.push_back( make_pair(x,y));
	}

	bool contains(int x, int y){ //http://stackoverflow.com/a/571405/625919
		if (find(snake.begin(), snake.end(), make_pair(x,y))!=snake.end()){
			return true;
		}
		return false;
	}

	bool contains(coordinate pair){
		if (find(snake.begin(), snake.end(), pair)!=snake.end()){
			return true;
		}
		return false;
	}

	void print_coordinates(){
		for (auto & element : snake) { // http://stackoverflow.com/a/12702604/625919
			cout << element.first << " , " << element.second << endl;
		}
	}

	int size(){
		return snake.size();
	}

	int snakehead_x() {
		snake[0].first;
	}

	bool move_zig(int direction_zig, Apple apple1){
		int xhead, yhead;

		if          (direction_zig == 0) { // calculating the head coordinates
			xhead = snake[0].first;           // top                        // top = 0
			yhead = snake[0].second - 2; }                                                                     // t r = 1
		else if     (direction_zig == 3) {                                                             // b r = 2
			xhead = snake[0].first;           // bot                        // bot = 3
			yhead = snake[0].second + 2; }                                                                     // b l = 4
		else if (   (snake[0].second%2) == 0){ // EVEN (RIGHT SIDE)          // t l = 5
			if      (direction_zig == 1) { // top right
				xhead = snake[0].first + 1;
				yhead = snake[0].second - 1; }
			else if (direction_zig == 2) { // bot right
				xhead = snake[0].first + 1;
				yhead = snake[0].second + 1; }
			else if (direction_zig == 4) { // bot left
				xhead = snake[0].first;
				yhead = snake[0].second + 1; }
			else if (direction_zig == 5) { // top left
				xhead = snake[0].first;
				yhead = snake[0].second - 1; }
		}
		else {                             // ODD (LEFT SIDE)
			if      (direction_zig == 1) { // top right
				xhead = snake[0].first;
				yhead = snake[0].second - 1; }
			else if (direction_zig == 2) { // bot right
				xhead = snake[0].first;
				yhead = snake[0].second + 1; }
			else if (direction_zig == 4) { // bot left
				xhead = snake[0].first - 1;
				yhead = snake[0].second + 1; }
			else if (direction_zig == 5) { // top left
				xhead = snake[0].first - 1;
				yhead = snake[0].second - 1; }
		}

		int snaketail = snake.size()-1;

		if (apple1.eaten(xhead, yhead)){
			add_coordinate(snake[snaketail].first,snake[snaketail].second);
		}

		for(int i = snaketail; i > 0; i--){ // moving the elements of each array through the memory.
			snake[i].first = snake[i-1].first;
			snake[i].second = snake[i-1].second;
		}
		snake[0].first = xhead;               // assigning the head
		snake[0].second = yhead;

		//dead check

		if ((xhead <= 0) || (yhead <= 0) || (xhead > WIDTH) || (yhead >= HEIGHT)) // boundary check
			return true;
		else {
			for (size_t i = 1; i < snake.size(); i++) {  // self intersection check
				if ((snake[0].first == snake[i].first)
					&&
					(snake[0].second == snake[i].second)){
						return true;
				}
			}
		}
		return false;
	}
};

void draw_hex(Apple apple1, Snake player1);

int main() {
	Snake player1;
	Apple apple1;
	bool dead = false;
	int direction_zig = 2;
	int previous_zig = 2;

	player1.add_coordinate(2,4);
	player1.add_coordinate(2,3);
	player1.add_coordinate(1,2);
	player1.add_coordinate(1,1);

	do { // generates new apple coordinates and ensures they don't intersect with snake
		apple1.new_position();
		srand((unsigned)time(0));
		int x = (rand()%WIDTH+1); //xapple = random_number(1,XMAX);
		if (HEIGHT%2 == 0)
			int y = (rand()%(HEIGHT-1)+1); // yapple = random_number(1,YMAX-1);
		else
			int y = (rand()%(HEIGHT)+1); // yapple = random_number(1,YMAX);
	} while (player1.contains(apple1.position));

	// primary game loop
	while (!dead){
		draw_hex(apple1, player1);
		this_thread::sleep_for(chrono::milliseconds(1000));
		if (_kbhit()){
			switch (_getche()){
			case '8':
				direction_zig = 0; break;
			case '9':
				direction_zig = 1; break; // TODO: Perhaps prevent player from going backwards on themselves?
			case '6':
				direction_zig = 2; break;
			case '5':
				direction_zig = 3; break;
			case '4':
				direction_zig = 4; break;
			case '7':
				direction_zig = 5; break;
			}

			if      (previous_zig == 0 && direction_zig == 3)
				direction_zig = 0;
			else if (previous_zig == 1 && direction_zig == 4)
				direction_zig = 1;
			else if (previous_zig == 2 && direction_zig == 5)
				direction_zig = 2;
			else if (previous_zig == 3 && direction_zig == 6)
				direction_zig = 3;
			else if (previous_zig == 4 && direction_zig == 1)
				direction_zig = 4;
			else if (previous_zig == 5 && direction_zig == 2)
				direction_zig = 5;
			previous_zig = direction_zig;
		}
		system("cls");                    // TODO: is there a better way to clear the screen? Flickers quite a bit

		// apple eaten
		if (apple1.eaten(player1.snake[0].first,player1.snake[0].second)){

			do { // generates new apple coordinates and ensures they don't intersect with snake
				apple1.new_position();
				srand((unsigned)time(0));
				int x = (rand()%WIDTH+1); //1 to width
				if (HEIGHT%2 == 0)
					int y = (rand()%(HEIGHT-1)+1); // 1 to height-1
				else
					int y = (rand()%(HEIGHT)+1); // 1 to height
			} while (player1.contains(apple1.position));
		}

		dead = player1.move_zig(direction_zig, apple1);
	}
	cout << "YOU DIED" << endl << "with a score of " << player1.size() - 4 << ".";
	_getch();
	return 0;
}

void draw_hex(Apple apple1, Snake player1) {
	int x, y;
	bool top, bot;

	for (x = 1; x <= WIDTH; x++){ // draws the very top of the grid
		printf(" __   ");
		if (x == WIDTH)
			printf("\n");
	}
	for (y = 1; y <= HEIGHT; y++){
		for (x = 1; x <= WIDTH; x++){ // top of the hexes
			// checks if snake is in hex

			top = player1.contains(x,y);
			if (y%2 == 0)
				bot = player1.contains(x+1,y);
			else
				bot = player1.contains(x, y-1);

			// draws
			if      (((y) == (apple1.position.second)) && ((x) == apple1.position.first) && (bot == true))
				printf("/()\\##");
			else if (((y) == (apple1.position.second)) && ((x) == apple1.position.first))
				printf("/()\\__");
			else if ((top == 1) && (bot == 1))
				printf("/##\\##");
			else if (top == 1)
				printf("/##\\__");
			else if (bot == 1)
				printf("/  \\##");
			else
				printf("/  \\__");
			if (x == WIDTH)
				if (y == 1)
					printf(" %d\n",y);
				else
					printf("/%d\n",y);
		}
		y++; // moves us to lower half. Well, we're really drawing both halves in each section, but it just doesn't look that way
		for (x = 1; x <= WIDTH; x++){ // draws the bottom of the hexes
			// checks if snake is in hex
			top = player1.contains(x,y);
			bot = player1.contains(x,y-1);

			// draws
			if      (((y) == (apple1.position.second)) && ((x) == apple1.position.first) && (bot == 1))
				printf("\\##/()");
			else if (((y) == (apple1.position.second)) && ((x) == apple1.position.first))
				printf("\\__/()");
			else if ((top == 1) && (bot == 1))
				printf("\\##/##");
			else if (top == 1)
				printf("\\__/##");
			else if (bot == 1)
				printf("\\##/  ");
			else
				printf("\\__/  ");
			if (x == WIDTH)
				if ((y == HEIGHT) || (y == (HEIGHT+1)))
					printf("\n");
				else
					printf("\\%d\n",y);
		}
	}
	for (x = 1; x <= WIDTH; x++){
		printf(" %d    ",x); // maybe take the axis out in the final version, but I like 'em here for now
		if (x == WIDTH)
			printf("\n\nScore: %d\n", player1.size() - 4);
	}
}


/* circularly tireddddd
http://stackoverflow.com/questions/2164942/how-can-i-store-a-pair-of-numbers-in-c
*/