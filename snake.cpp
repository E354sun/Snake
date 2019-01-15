/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

#include <iostream>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <utility>
#include <vector>
#include <sstream>
#include <stdlib.h>
#include <time.h>

/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;
 
/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;
const int width = 800;
const int height = 600;
const int UNIT = 10;
const int INI_LEN = 3;
const int INI_SIZE = 10;
const int INI_FRAME = 5;
const int INI_SCOREBOARD = 80;

bool PAUSE = false;
bool START = false;
bool FF = false;
int FPS = 30;
int Speed = 1;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	Pixmap   pixmap;  
	GC		 gc[8];
	int		width;		// size of window
	int		height;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


/*
 * An abstract class representing displayable things. 
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};

class Block {
public:
    Block (int x, int y, char direction) {
        Coord = std::make_pair(x, y);
        dir = direction;
    }
    
    int getX() {
        return Coord.first;
    }
    int getY() {
        return Coord.second;
    }
    void setX(int x) {
        int y = Coord.second;
        Coord = std::make_pair(x, y);
    }
    void setY(int y) {
        int x = Coord.first;
        Coord = std::make_pair(x, y);
    }
    char getDir() {
        return dir;
    }
    void setDir(char d) {
        dir = d;
    }
private:
    std::pair <int,int> Coord;
    char dir;
};

class Snake : public Displayable {
    	public:
    		virtual void paint(XInfo &xinfo) {
    			if (START) {
                int block_index = 0;

                while (block_index < length) {
                    int x = body.at(block_index)->getX();
                    int y = body.at(block_index)->getY();
                    XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x, y, blockSize, blockSize);

                    block_index ++;
                }
            }
            }
    /*
     N
 E       W 			:axis
     S
    */
	    	void move(XInfo &xinfo) {
		    for (int i = 0; i < length; i++) {
			char dir = body.at(i)->getDir();
			int x = body.at(i)->getX();
			int y = body.at(i)->getY();
			int action = speed * blockSize;

			if (dir == 'E') {
			    body.at(i) -> setX(x - action);
			}
			else if (dir == 'W') {
			    body.at(i) -> setX(x + action);
			}
			else if (dir == 'N') {
			    body.at(i)->setY(y - action);
			}
			else if (dir == 'S') {
			    body.at(i) -> setY(y + action);
			}
		    }
		    
		    int follower = length - 1;
		    while (follower > 0) {
			    char new_dir = body.at(follower-1)->getDir();
			    body.at(follower)->setDir(new_dir);
			    -- follower;
		    }
		}

            // ** ADD YOUR LOGIC **
            // Here, you will be performing collision detection between the snake, 
            // the fruit, and the obstacles depending on what the snake lands on.
	    	
		
		char getD() {
		    return body.at(0)->getDir();
		}

		int getX() {
			return body.at(0)->getX();
		}
		
		int getY() {
			return body.at(0)->getY();
		}

        /*
         * ** ADD YOUR LOGIC **
         * Use these placeholder methods as guidance for implementing the snake behaviour. 
         * You do not have to use these methods, feel free to implement your own.
         */ 

	bool didEatFruit (int x, int y) {
	    int self = 0;
	    while (self < length) {
		if ((x == body.at(self)->getX()) && (y == body.at(self)->getY())) {
		    return true;
		}
		++ self;
	    }
	    return false;
	}

        void EatFruit() {
	    int tailX = body.back()->getX();
	    int tailY = body.back()->getY();
	    char tailD = body.back()->getDir();
	    Block *newBlock = NULL;
	    switch (tailD) {
		case('E'): 
			newBlock = new Block(tailX+blockSize, tailY, tailD);
			break;
		case('W'): 
			newBlock = new Block(tailX-blockSize, tailY, tailD);
			break;
		case('N'): 
			newBlock = new Block(tailX, tailY+blockSize, tailD);
			break;
		case('S'): 
			newBlock = new Block(tailX, tailY-blockSize, tailD);
	    	break;
	    }

	    body.push_back(newBlock);
        ++ length;
        }

        bool didEatFF (int x, int y) {
	    int self = 0;
	    while (self < length) {
		if ((x == body.at(self)->getX()) && (y == body.at(self)->getY())) {
		    return true;
		}
		++ self;
	    }
	    return false;
	}

		void EatFF() {
			++ life;
		}

		void DeadOnce() {
			-- life;
		}

		int getLife() {
	    	return life;
	    }

	    int getScore() {
	    int score = length - INI_LEN;
	    return score;
		}

		bool paintFF() {
		if ((getScore()+1) % 3 == 0) {
			return true;
		}
		}


        bool didHitObstacle() {
	    	// Hit itself
	    	int self = 1;
            	while (self < length) {
			if(getX() == body[self]->getX() && getY() == body[self]->getY()) {
		    	return true;
			}
			++ self;
	    	}

	    	int margin_x = width-UNIT-INI_SCOREBOARD-INI_FRAME-20;
	    	int margin_y = height-UNIT-INI_SCOREBOARD-INI_FRAME-20;

	    	//Hit Obstacle
	    	if ((getX() > 30) && (getX() < 770) && (getY() < 30)) {
	    		// up-margin
				return true;
			} else if ((getX() > 30) && (getX() < 770) && (getY() > 560)) {
				// down-margin
				return true;
			} else if ((getY() > 30) && (getY() < 570) && (getX() < 30)) {
				// left-margin
				return true;
			} else if ((getY() > 30) && (getY() < 570) && (getX() > 760)) {
				// right-margin
				return true;
	    	} else if ((getX() > margin_x) && (getY() > margin_y)) {
	    		return true;
	    	} else {
			// going on well
	    		return false;
        	}
        }

        void turnLeft() {
	    int originalD = body.at(0)->getDir();
	    switch (originalD) {
		case('E'):
			didSwerve = true;
			body.at(0)->setDir('S');
			break;
		case('W'):
			didSwerve = true;
			body.at(0)->setDir('N');
			break;
		case('N'): 
			didSwerve = true;
			body.at(0)->setDir('E');
			break;
		case('S'):
			didSwerve = true;
			body.at(0)->setDir('W');
			break;
	    }
        }

        void turnRight() {
            int originalD = body.at(0)->getDir();
	    switch (originalD) {
		case('E'):
			didSwerve = true;
			body.at(0)->setDir('N');
			break;
		case('W'):
			didSwerve = true;
	                body.at(0)->setDir('S');
			break;
		case('N'): 
			didSwerve = true;
			body.at(0)->setDir('W');
			break;
		case('S'):
			didSwerve = true;
			body.at(0)->setDir('E');
			break;
	    }
        }

	Snake(int x, int y) {
	    blockSize = INI_SIZE;
	    length = INI_LEN;
	    life = 0;

	    Block *start = NULL;
	    for (int i = 0; i < length; i++) {
		// initial go E
		start = new Block(x - i*blockSize, y, 'W');
		body.push_back(start);
	    }

	    speed = Speed;
	    didSwerve = false;
	}
	
	~Snake() {
	    for (int i = 0; i < length; i++) {
		delete body.at(i);
	    }
	}
	
	void reborn (int x, int y) {
	    body.clear();

	    blockSize = INI_SIZE;
	    length = INI_LEN;
	    life = 0;

	    Block *start = NULL;
	    for (int i = 0; i < length; i++) {
		// initial go E
		start = new Block(x - i*blockSize, y, 'W');
		body.push_back(start);
	    }

	    speed = Speed;
	    didSwerve = false;
	}


	private:
		int length;
		int blockSize;
		int speed;
		int life;
		vector<Block*> body;
		bool didSwerve;
};

class Fruit : public Displayable {
	public:
	virtual void paint(XInfo &xinfo) {
			if (START) {
			// apple
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x, y+4, 1, 5);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+1, y+3, 1, 6);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+2, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+3, y+2, 1, 8);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+4, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+5, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+6, y+2, 1, 8);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+7, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+8, y+3, 1, 6);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+9, y+4, 1, 5);

            // apple leaf
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], x+4, y+1, 2, 3);

            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+1, y+1, 1, 1);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+2, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+3, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+4, y+1, 1, 2);    

            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+8, y+1, 1, 1);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+7, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+6, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+5, y+1, 1, 2); 

            //highlighter
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], x+2, y+5, 1, 3);
        }

    }
        
        Fruit() {
            // ** ADD YOUR LOGIC **
            // generate the x and y value for the fruit 
            x = 150;
            y = 150;
        }

	int getX() {
	    return x;
	}

	int getY() {
	    return y;
	}
	
	void reset() {				// need to modify
        srand(time(0));
        x = rand() % ((width - 6*UNIT)/UNIT) + 3;
		y = rand() % ((height - 6*UNIT)/UNIT) + 3;
		x *= UNIT;
		y *= UNIT;

        int margin_x = width-UNIT-INI_SCOREBOARD-INI_FRAME-20;
	    int margin_y = height-UNIT-INI_SCOREBOARD-INI_FRAME-20;

	    if ((x > margin_x) && (y > margin_y)) {
	    	reset();
	    }

	}

        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */
    private:
        int x;
        int y;
};

class FelixFelicis : public Displayable {
public:
	virtual void paint(XInfo &xinfo) {
		if (FF) {
			// apple
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x, y+4, 1, 5);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+1, y+3, 1, 6);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+2, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+3, y+2, 1, 8);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+4, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+5, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+6, y+2, 1, 8);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+7, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+8, y+3, 1, 6);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+9, y+4, 1, 5);

            // apple leaf
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], x+4, y+1, 2, 3);

            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+1, y+1, 1, 1);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+2, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+3, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+4, y+1, 1, 2);    

            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+8, y+1, 1, 1);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+7, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+6, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+5, y+1, 1, 2); 

            //highlighter
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], x+2, y+5, 1, 3);
        }
    }

	FelixFelicis() {
            x = 450;
            y = 450;
        }

	int getX() {
	    return x;
	}

	int getY() {
	    return y;
	}
	
	void reset() {		
		FF = false;
		
		srand(time(NULL));
        x = rand() % ((width - 6*UNIT)/UNIT) + 3;
		y = rand() % ((height - 6*UNIT)/UNIT) + 3;
		x *= UNIT;
		y *= UNIT;

        int margin_x = width-UNIT-INI_SCOREBOARD-INI_FRAME-20;
	    int margin_y = height-UNIT-INI_SCOREBOARD-INI_FRAME-20;

	    if ((x > margin_x) && (y > margin_y)) {
	    	reset();
	    }
	}

private:
	int x;
	int y;
};


list<Displayable *> dList;           // list of Displayables
Snake snake(100, 450);
Fruit fruit;
FelixFelicis felixfelicis;


class Obstacles : public Displayable {
public:
    virtual void paint(XInfo &xinfo) {
    	if (START) {
    	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 30, 15, 740, INI_FRAME);
        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 15, 30, INI_FRAME, 540);
        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 30, 580, 755, INI_FRAME);
        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 780, 30, INI_FRAME, 550);
        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 700, 500, INI_FRAME, INI_SCOREBOARD);
        XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], 700, 500, INI_SCOREBOARD, INI_FRAME);

        string scoreText = "SCORE:";
        XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 715, 535, scoreText.c_str(), 6);
        int score = snake.getScore();
		stringstream ss;
		ss << score;
		string Score = ss.str();
		XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 755, 535, Score.c_str(), Score.length());

		string lifeText = "LIFE:";
        XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 715, 555, lifeText.c_str(), 5);
        int life = snake.getLife();
		stringstream ss2;
		ss2 << life;
		string Life = ss2.str();
		XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 755, 555, Life.c_str(), Life.length());
	}
	}
};

class StartFlash : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
				if (!START) {
					// S
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], 70, 70, 50, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], 70, 120, 50, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], 70, 70, INI_FRAME, 50);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], 120, 120, INI_FRAME, 55);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], 70, 170, 50, INI_FRAME);

					// N
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], 150, 70, INI_FRAME, 105);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], 150, 70, 50, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], 200, 70, INI_FRAME, 105);

					// A
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], 230, 70, INI_FRAME, 105);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], 230, 70, 50, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], 230, 120, 50, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], 280, 70, INI_FRAME, 105);

					// K
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 310, 70, INI_FRAME, 105);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 310, 120, 2*INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 315, 125, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 320, 130, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 325, 135, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 330, 140, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 335, 145, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 340, 150, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 345, 155, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 350, 160, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 355, 165, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 360, 170, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 315, 115, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 320, 110, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 325, 105, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 330, 100, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 335, 95, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 340, 90, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 345, 85, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 350, 80, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 355, 75, INI_FRAME, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[7], 360, 70, INI_FRAME, INI_FRAME);
					// E
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[6], 390, 70, 50, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[6], 390, 120, 50, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[6], 390, 70, INI_FRAME, 50);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[6], 390, 120, INI_FRAME, 55);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[6], 390, 170, 50, INI_FRAME);

					// snake + fruit

				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 120, 270, 90, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 210, 270,  INI_FRAME, 30);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 210, 300, 20, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 230, 250,  INI_FRAME, 55);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 230, 250, 30, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 260, 250,  INI_FRAME, 65);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 260, 315, 30, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 290, 270, INI_FRAME, 50);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 290, 270, 150, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 440, 270,  INI_FRAME, 20);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 440, 290, 20, INI_FRAME);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 460, 270,  INI_FRAME, 25);
				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], 460, 270, 200, INI_FRAME);

				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], 680, 270, INI_FRAME, INI_FRAME);

					// instruction
				string controls = "S - Start/Restart               P - Pause               Q - Quit";
				XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 220, 375, controls.c_str(), controls.length());
				string guide = "Use the arrow keys to guide through the walls!";
				XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 250, 420, guide.c_str(), guide.length());
				string fruit = "Eat       to grow up!";
				XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 250, 480, fruit.c_str(), fruit.length());
				string ff = "Eat       to get extra life!";
				XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 250, 540, ff.c_str(), ff.length());	

					// id
				string ID1 = "Yiran Sun 20651784";
				XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 500, 100, ID1.c_str(), ID1.length());
				string ID2 = "Y354sun";
				XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[1], 520, 120, ID2.c_str(), ID2.length());

					// fruit 
				int x = 282;
				int y = 470;

				XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x, y+4, 1, 5);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+1, y+3, 1, 6);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+2, y+3, 1, 7);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+3, y+2, 1, 8);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+4, y+3, 1, 7);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+5, y+3, 1, 7);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+6, y+2, 1, 8);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+7, y+3, 1, 7);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+8, y+3, 1, 6);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[4], x+9, y+4, 1, 5);

            	// apple leaf
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], x+4, y+1, 2, 3);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+1, y+1, 1, 1);
           		XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+2, y, 1, 3);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+3, y, 1, 3);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+4, y+1, 1, 2);    

            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+8, y+1, 1, 1);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+7, y, 1, 3);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+6, y, 1, 3);
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+5, y+1, 1, 2); 

            	//highlighter
            	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], x+2, y+5, 1, 3);

            	x = 282;
            	y = 530;

            	// apple
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x, y+4, 1, 5);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+1, y+3, 1, 6);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+2, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+3, y+2, 1, 8);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+4, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+5, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+6, y+2, 1, 8);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+7, y+3, 1, 7);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+8, y+3, 1, 6);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[5], x+9, y+4, 1, 5);

            // apple leaf
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[3], x+4, y+1, 2, 3);

            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+1, y+1, 1, 1);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+2, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+3, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+4, y+1, 1, 2);    

            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+8, y+1, 1, 1);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+7, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+6, y, 1, 3);
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[2], x+5, y+1, 1, 2); 

            //highlighter
            XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], x+2, y+5, 1, 3);


				}
		}
};


Obstacles obstacles;
StartFlash startflash;


/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo) {
	if (argc == 3){
		if (atoi(argv[1]) >= 1 && atoi(argv[1]) <= 100){
			FPS = atoi(argv[1]);
		}	
		if (atoi(argv[2]) >= 1 && atoi(argv[2]) <= 10){
			Speed = atoi(argv[2]);
		}
	}


	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/	
	xInfo.display = XOpenDisplay( "" );
	if ( !xInfo.display )	{
		error( "Can't open display." );
	}
	
   /*
	* Find out some things about the display you're using.
	*/
	xInfo.screen = DefaultScreen( xInfo.display );

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.flags = PPosition | PSize;

	xInfo.window = XCreateSimpleWindow( 
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour
		
	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"animation",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/* 
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	i = 1;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                      1, LineSolid, CapButt, JoinRound);
	XSetGraphicsExposures(xInfo.display, xInfo.gc[i],0);


	i = 2;
	XColor xgreen;
  	char green[] = "#9ACD32";

	Colormap colormap_g = DefaultColormap(xInfo.display, 0);
	XParseColor(xInfo.display, colormap_g, green, &xgreen);
	XAllocColor(xInfo.display, colormap_g, &xgreen);

	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], xgreen.pixel);
	XSetGraphicsExposures(xInfo.display, xInfo.gc[i],0);

	i = 3;
	XColor xbrown;
  	char brown[] = "#8B4513";

	Colormap colormap_br = DefaultColormap(xInfo.display, 0);
	XParseColor(xInfo.display, colormap_br, brown, &xbrown);
	XAllocColor(xInfo.display, colormap_br, &xbrown);

	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], xbrown.pixel);
	XSetGraphicsExposures(xInfo.display, xInfo.gc[i],0);

	i = 4;
	XColor xred;
  	char red[] = "#FF4500";

	Colormap colormap_r = DefaultColormap(xInfo.display, 0);
	XParseColor(xInfo.display, colormap_r, red, &xred);
	XAllocColor(xInfo.display, colormap_r, &xred);

	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], xred.pixel);
	XSetGraphicsExposures(xInfo.display, xInfo.gc[i],0);

	i = 5;
	XColor xyellow;
  	char yellow[] = "#FFA500";

	Colormap colormap_y = DefaultColormap(xInfo.display, 0);
	XParseColor(xInfo.display, colormap_y, yellow, &xyellow);
	XAllocColor(xInfo.display, colormap_y, &xyellow);

	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], xyellow.pixel);
	XSetGraphicsExposures(xInfo.display, xInfo.gc[i],0);

	i = 6;
	XColor xpink;
  	char pink[] = "#E9967A";

	Colormap colormap_p = DefaultColormap(xInfo.display, 0);
	XParseColor(xInfo.display, colormap_p, pink, &xpink);
	XAllocColor(xInfo.display, colormap_p, &xpink);

	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], xpink.pixel);
	XSetGraphicsExposures(xInfo.display, xInfo.gc[i],0);

	i = 7;
	XColor xblue;
  	char blue[] = "#87CEFA";

	Colormap colormap_b = DefaultColormap(xInfo.display, 0);
	XParseColor(xInfo.display, colormap_b, blue, &xblue);
	XAllocColor(xInfo.display, colormap_b, &xblue);

	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], xblue.pixel);
	XSetGraphicsExposures(xInfo.display, xInfo.gc[i],0);



	// double buffering 
	int depth = DefaultDepth(xInfo.display, DefaultScreen(xInfo.display));

	xInfo.pixmap = XCreatePixmap(xInfo.display, xInfo.window, 
									hints.width, hints.height, depth);
	xInfo.width = hints.width;
	xInfo.height = hints.height;


	XSelectInput(xInfo.display, xInfo.window, KeyPressMask);  // for resize events

	XSetWindowBackgroundPixmap(xInfo.display, xInfo.window, None);

	/*
	 * Put the window on the screen.
	 */
	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	// clear window
	XClearWindow( xinfo.display, xinfo.window );
	
	// get height and width of window (might have changed since last repaint)

	//XWindowAttributes windowInfo;
	//XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
	//unsigned int height = windowInfo.height;
	//unsigned int width = windowInfo.width;

	// clear buffer
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 0, 0, xinfo.width, xinfo.height);
    
	// draw display list
	while( begin != end ) {
		Displayable *d = *begin;
		d->paint(xinfo);
		begin++;
	}

	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, xinfo.width, xinfo.height, 		 // region of pixmap to copy
		0, 0); 									// position to put top left corner of pixmap in window
	XFlush( xinfo.display );
}

void handleKeyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[BufferSize];
	
	/*
	 * Exit when 'q' is typed.
	 * This is a simplified approach that does NOT use localization.
	 */
	int i = XLookupString( 
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)

	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);
		if (text[0] == 'q') {
			error("Terminating normally.");
		} else if (text[0] == 'p') {
			PAUSE = (!PAUSE);
		} else if (text[0] == 's') {
			START = (!START);
		}
	}

	switch(key) {
        case XK_Left:
        	if (snake.getD() == 'N') {
        		snake.turnLeft();
	    	} else if (snake.getD() == 'S') {
				snake.turnRight();
	    	}
            cout << "Left" << endl;
            break;

        case XK_Right:
            if (snake.getD() == 'N') {
				snake.turnRight();
	    	} else if (snake.getD() == 'S') {
				snake.turnLeft();
	    	}
            cout << "Right" << endl;
            break;

        case XK_Up:
            if (snake.getD() == 'E') {
				snake.turnRight();
	    	} else if (snake.getD() == 'W') {
				snake.turnLeft();
	    	}
            cout << "Up" << endl;
            break;

        case XK_Down:
            if (snake.getD() == 'E') {
				snake.turnLeft();
	    	} else if (snake.getD() == 'W') {
		 		snake.turnRight();
	    	}
            cout << "Down" << endl;
            break;
    }
}



void handleAnimation(XInfo &xinfo, int inside) {
    /*
     * ADD YOUR OWN LOGIC
     * This method handles animation for different objects on the screen and readies the next frame before the screen is re-painted.
     */
	if (snake.didHitObstacle()) {
		snake.DeadOnce();

		if (snake.getLife() < 0) {
			START = false;
			snake.reborn(150, 450);
			fruit.reset();
		}
	}

	int x = fruit.getX();
	int y = fruit.getY();

	if (snake.didEatFruit(x, y)) {
		snake.EatFruit();
		fruit.reset();
	}

	FF = snake.paintFF();

	int fx = felixfelicis.getX();
	int fy = felixfelicis.getY();

	if (snake.didEatFF(fx, fy) && FF) {
		snake.EatFF();
		FF = false;
		felixfelicis.reset();
	}

	snake.move(xinfo);
}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

void eventLoop(XInfo &xinfo) {
	// Add stuff to paint to the display list
	dList.push_front(&startflash); 
	dList.push_front(&snake);
    dList.push_front(&fruit);
    dList.push_front(&obstacles);
    dList.push_front(&felixfelicis);


	XEvent event;
	unsigned long lastRepaint = 0;
	int inside = 0;

	while( true ) {
		/*
		 * This is NOT a performant event loop!  
		 * It needs help!
		 */
		
		if (XPending(xinfo.display) > 0) {
			XNextEvent( xinfo.display, &event );
			// cout << "event.type=" << event.type << "\n";
			switch( event.type ) {
				case KeyPress:
					handleKeyPress(xinfo, event);
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
			}
		} 

		unsigned long end = now();

		if (end - lastRepaint > 1000000/FPS) {
			if (START && (!PAUSE)) {
				handleAnimation(xinfo, inside);
			}
			repaint(xinfo);
			lastRepaint = now();
		}

		if (XPending(xinfo.display) == 0) {
			usleep(1000000 / FPS - (end - lastRepaint));
		}
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {
	XInfo xInfo;

	initX(argc, argv, xInfo);
	eventLoop(xInfo);
	XCloseDisplay(xInfo.display);
}
