#include "GL.h"
#include "Text.h"
#include "Game.h"
#include <iostream>
#include <stdlib.h>    
#include <time.h>   
#include <algorithm>

const unsigned char ESC   = 27;
const unsigned char SPACE = 32;

const unsigned MIN_SPEED  = 1;
const unsigned MAX_SPEED  = 9;

const unsigned WIN_NUNBER = 6;

const unsigned CELL_SIZE  = 25;
const unsigned WIDTH      = 10;
const unsigned HEIGHT     = 20;

const unsigned X0          = 50;
const unsigned Y0          = 50;



Figure::Figure(int size)
{
	field.reserve(size);
	for (int i = 0; i < size; ++i)
	{
		field[i].assign(size, 0);
	}
}

void Figure::rotateRight()
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			std::swap(field[i][j], field[size - j - 1][i]);
		}
	}
}

void Figure::rotateLeft()
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < i; ++j)
		{
			std::swap(field[i][j], field[j][size - i - 1]);
		}
	}
}





Figures::Figures()
{
	// figure "o"
	Figure o(2);
	o.setIJ(0, 0, 1); o.setIJ(1, 0, 1); o.setIJ(0, 1, 1); o.setIJ(1, 1, 1);

	// figure "t"
	Figure t(3);
	t.setIJ(1, 1, 1); t.setIJ(2, 0, 1); t.setIJ(2, 1, 1); t.setIJ(2, 2, 1);

	// figure "l"
	Figure l(3);
	l.setIJ(1, 0, 1); l.setIJ(2, 0, 1); l.setIJ(2, 1, 1); l.setIJ(2, 2, 1);

	// figure "j"
	Figure j(3);
	j.setIJ(1, 2, 1); j.setIJ(2, 0, 1); j.setIJ(2, 1, 1); j.setIJ(2, 2, 1);

	// figure "s"
	Figure s(3);
	s.setIJ(1, 1, 1);	s.setIJ(1, 2, 1);	s.setIJ(2, 0, 1);	s.setIJ(2, 1, 1);

	// figure "z"
	Figure z(3);
	z.setIJ(1, 0, 1);	z.setIJ(1, 1, 1);	z.setIJ(2, 1, 1);	z.setIJ(2, 2, 1);

	// figure "i"
	Figure i(4);
	i.setIJ(3, 0, 1);	i.setIJ(3, 1, 1);	i.setIJ(3, 2, 1);	i.setIJ(3, 3, 1);

	figures.push_back(o);
	figures.push_back(t);
	figures.push_back(l);
	figures.push_back(j);
	figures.push_back(s);
	figures.push_back(z);
	figures.push_back(i);

	srand(time(NULL));
}

Figure Figures::getRandomFigure()
{
	return figures[std::rand() % 8];
}



void Game::print(int value)
{
    printf("VALUE\n");
}

void Game::print(int& value)
{
    printf("REFERENCE\n");
}



Game::Game() 
: _score(0)
, _speed(MIN_SPEED)
, _lose(false)
, _paused(false)
, _win(false)
{	

	_figure = Figures::getRandomFigure();
    //static_assert(offsetof(Game,cell_x) > offsetof(Game,x0), "cell_x < x0");
    //static_assert(offsetof(Game,cell_y) > offsetof(Game,y0), "cell_y < y0");
}

#pragma warning (disable : 4100) //to suppress unused warning int x, int y needed by GLUT API
void Game::KeyPressed(int specialKey, unsigned char key, int x, int y)
{
    switch (specialKey)
    {
        case GLUT_KEY_UP    :    
        case GLUT_KEY_DOWN  :    
        case GLUT_KEY_LEFT  :    
        case GLUT_KEY_RIGHT : _InGameKey(specialKey, 0);  break;
    }

    switch (key)
    { 
        case SPACE          : _InGameKey(0, key);         break;
        case 'p'            :           
        case 'P'            : _Pause();                   break;
        case 'r'            :   
        case 'R'            : _Restart();                 break;
        case ESC            : _Stop();                    break;
    }
}
#pragma warning (default : 4100)

void Game::_InGameKey(int specialKey, unsigned char key)
{
    if (!_paused && !_lose)
    {
		unsigned oldDir;
        switch (specialKey)
        {
			case GLUT_KEY_UP    : _figure.rotateRight(); break;
			case GLUT_KEY_DOWN  : _speed == MAX_SPEED; break;
            case GLUT_KEY_LEFT  : _figure.rotateLeft(); break;
			case GLUT_KEY_RIGHT : _figure.rotateRight(); break;
        }

        switch (key)
        {
            case SPACE          : break;
        }
    }
}

void Game::Draw()
{
    GLClearBuffer();
    DoDraw();
    GLSwapBuffers();
}

void Game::DoDraw()
{
    GLDrawGrid(x0, y0, WIDTH, HEIGHT, CELL_SIZE);
    GLDrawBorder(x0, y0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE);
	if(!_lose && !_win)
	{
		moveSnake();
	}
	std::deque<Point>::iterator it = snakeCoords.begin();

	GLDrawRect(target.x, target.y, target.x + CELL_SIZE, target.y + CELL_SIZE, 0x00ff0000);

	
	while (it != snakeCoords.end())
	{
		std::cout << it->x << " " << it->y << std::endl;
		GLDrawRect(it->x, it->y, it-> x+ CELL_SIZE, it->y + CELL_SIZE, 0x00ff00ff);
		it++;
	}

    _DrawStatus(x0 + (WIDTH + 1) * CELL_SIZE, y0);
}

void Game::_DrawStatus(unsigned x0, unsigned y0)
{
    GLDrawText(x0, y0 + 40, GLUT_BITMAP_HELVETICA_12, FormatText("Score: %i", _score).c_str());
    GLDrawText(x0, y0 + 60, GLUT_BITMAP_HELVETICA_12, FormatText("Speed: %i", _speed).c_str());

    if (_lose)
    {
        GLDrawText(x0, y0 + 240, GLUT_BITMAP_HELVETICA_18, "    Game over."     );
        GLDrawText(x0, y0 + 220, GLUT_BITMAP_HELVETICA_18, "Press R to restart.");
    }
    else if (_paused)
    {
        GLDrawText(x0, y0 + 240, GLUT_BITMAP_HELVETICA_18, "   Game paused."     );
        GLDrawText(x0, y0 + 220, GLUT_BITMAP_HELVETICA_18, "Press P to continue.");
    }
	else if (_win)
    {
        GLDrawText(x0, y0 + 240, GLUT_BITMAP_HELVETICA_18, "    You win!! "     );
        GLDrawText(x0, y0 + 220, GLUT_BITMAP_HELVETICA_18, "Press R to restart.");
    }
}

void Game::Update()
{
    if (!_paused)
    {
        if (cell_x < x0 || cell_x >= x0 + WIDTH * CELL_SIZE ||
            cell_y < y0 || cell_y >= y0 + HEIGHT * CELL_SIZE)
        {
            _lose = true;
        }

		std::deque<Point>::iterator j = snakeCoords.begin();
		for(std::deque<Point>::iterator i = snakeCoords.begin() + 1; i != snakeCoords.end() - 1; ++i)
		{
			if( i->x == j->x && i->y == j->y)
				_lose = true;
		}

		if(_eatenTargets == WIN_NUNBER)
		{
			_win = true;
			_SpeedUp();
			//_Pause();
			//_Restart();
		}
    }

}

void Game::_Restart()
{
    _lose   = false;
    _paused = false;
    _score  = 0;
	_win = false;
    cell_x = x0;
    cell_y = y0;
	snakeCoords.clear();
	snakeCoords.push_back(Point(X0 + 2*CELL_SIZE,Y0));
	snakeCoords.push_back(Point(X0 + CELL_SIZE,Y0));
	snakeCoords.push_back(Point(X0,Y0));
	setTarget();
	direction = 1;
}

void Game::_Pause()
{
    _paused = !_paused;
}

void Game::_Stop()
{
    exit(0);
}

unsigned Game::UpdateInterval()
{
    return (MAX_SPEED - _speed + 1)*100;
}

void Game::_SpeedUp()
{
    _speed = std::min(_speed + 1, MAX_SPEED);
}

void Game::_SpeedDown()
{
    _speed = std::max(_speed - 1, MIN_SPEED);
}

void Game::addPointToSnake(Point p)
{
	 snakeCoords.push_front(p);
}

void Game::deletePointFromSnake()
{
	snakeCoords.pop_back();
}


void Game::moveSnake()
{	
	switch (direction)
    {
			case 0 : cell_y += CELL_SIZE; break;
            case 2 : cell_y -= CELL_SIZE; break;
            case 3 : cell_x -= CELL_SIZE; break;
            case 1 : cell_x += CELL_SIZE; break;
    }

	Update();
	addPointToSnake(Point(cell_x, cell_y));
	if(cell_x == target.x && cell_y == target.y)
	{
		_score += 10*_speed;
		++_eatenTargets;
		setTarget();
	}
	else
	{
		deletePointFromSnake();
	}
}