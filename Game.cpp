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
const unsigned MAX_SPEED  = 90;

const unsigned WIN_NUNBER = 10;

const unsigned CELL_SIZE  = 25;
const unsigned WIDTH      = 10;
const unsigned HEIGHT     = 20;

const unsigned X0          = 50;
const unsigned Y0          = 50;

const int X_BEGIN = 3;

std::vector<Figure> Figures::figures;

Figure::Figure(int size) : size(size)
{
	field.resize(size);
	for (int i = 0; i < size; ++i)
	{
		field[i].assign(size, 0);
	}
}

void Figure::rotateRight()
{
	for (int r = 0; r < size; r++) 
	{
		for (int c = r; c < size; c++) 
		{
			std::swap(field[r][c], field[c][r]);
		}
	}
	//reverse elements on row order
	for (int r = 0; r < size; r++) 
	{
		for (int c = 0; c < size / 2; c++) 
		{
			std::swap(field[r][c], field[r][size - c - 1]);
		}
	}
}

void Figure::rotateLeft()
{
	for (int r = 0; r < size; r++)
	{
		for (int c = r; c < size; c++)
		{
			std::swap(field[r][c], field[c][r]);
		}
	}
	//reverse elements on column order
	for (int c = 0; c < size; c++)
	{
		for (int r = 0; r < size / 2; r++)
		{
			std::swap(field[r][c], field[size - r - 1][c]);
		}
	}
}

void Figures::createFigures()
{
	// figure "o"
	Figure o(2);
	o.setIJ(0, 0, 1); o.setIJ(1, 0, 1); o.setIJ(0, 1, 1); o.setIJ(1, 1, 1);

	// figure "t"
	Figure t(3);
	t.setIJ(1, 1, 1); t.setIJ(0, 0, 1); t.setIJ(0, 1, 1); t.setIJ(0, 2, 1);

	// figure "l"
	Figure l(3);
	l.setIJ(1, 0, 1); l.setIJ(0, 0, 1); l.setIJ(0, 1, 1); l.setIJ(0, 2, 1);

	// figure "j"
	Figure j(3);
	j.setIJ(1, 2, 1); j.setIJ(0, 0, 1); j.setIJ(0, 1, 1); j.setIJ(0, 2, 1);

	// figure "s"
	Figure s(3);
	s.setIJ(1, 1, 1);	s.setIJ(1, 2, 1);	s.setIJ(0, 0, 1);	s.setIJ(0, 1, 1);

	// figure "z"
	Figure z(3);
	z.setIJ(1, 0, 1);	z.setIJ(1, 1, 1);	z.setIJ(0, 1, 1);	z.setIJ(0, 2, 1);

	// figure "i"
	Figure i(4);
	i.setIJ(0, 0, 1);	i.setIJ(0, 1, 1);	i.setIJ(0, 2, 1);	i.setIJ(0, 3, 1);

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
	return figures[std::rand() % 7];
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
, x0(X0)
, y0(Y0)
, _x_figure(X_BEGIN)
{	
	Figures::createFigures();
	_figure = Figures::getRandomFigure();
	_y_figure = HEIGHT;

	_field.resize(HEIGHT);

	for (int i = 0; i < HEIGHT; ++i)
	{
		_field[i].assign(WIDTH, 0);
	}
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
        switch (specialKey)
        {
			case GLUT_KEY_UP:   
				_figure.rotateLeft();
				//if (!canMove(Direction::LEFT) || !canMove(Direction::RIGHT) || !canMove(Direction::DOWN))
				//	_figure.rotateLeft(); 
				break;
			case GLUT_KEY_DOWN  : _speed = MAX_SPEED; break;
			case GLUT_KEY_LEFT  : moveFigure(Direction::LEFT); break;
			case GLUT_KEY_RIGHT : moveFigure(Direction::RIGHT); break;
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

void Game::drawFigure()
{
	for (int i = 0; i < _figure.getSize(); ++i)
	{
		for (int j = 0; j < _figure.getSize(); ++j)
		{
			if (_figure.getIJ(i, j) == 1)
			{
				int x = (j + _x_figure) * CELL_SIZE + x0;
				int y = (i + _y_figure) * CELL_SIZE + y0;
				if (y < 0)
					continue;
				GLDrawRect(x, y, x + CELL_SIZE, y + CELL_SIZE, 0x00ff00ff);
			}
		}
	}
}

void Game::drawGameField()
{
	for (int i = 0; i < HEIGHT; ++i)
	{
		for (int j = 0; j < WIDTH; ++j)
		{
			if (_field[i][j] == 1)
			{
				int x = j * CELL_SIZE + x0;
				int y = i * CELL_SIZE + y0; 
				GLDrawRect(x, y, x + CELL_SIZE, y + CELL_SIZE, 0x0000aaff);
			}
		}
	}
}

void Game::updateGameField()
{
	for (int i = 0; i < _figure.getSize(); ++i)
	{
		for (int j = 0; j < _figure.getSize(); ++j)
		{
			if (_figure.getIJ(i, j) == 1)
				_field[i + _y_figure][j + _x_figure] = 1;
		}
	}
	removeFullRowsIfExists();
}

auto isFullRow = [](const std::vector<int>& row)
{
	bool fullRaw = true;
	for (auto elem : row)
	{
		if (elem == 0)
			fullRaw = false;
	}
	return fullRaw;
};

void Game::removeFullRowsIfExists()
{
	_field.erase(std::remove_if(_field.begin(), _field.end(), isFullRow), _field.end());

	int deleted_rows_number = HEIGHT - _field.size();

	_field.insert(_field.end(), deleted_rows_number, std::vector<int>(WIDTH, 0));
	_score += deleted_rows_number;
}

void Game::DoDraw()
{
    GLDrawGrid(x0, y0, WIDTH, HEIGHT, CELL_SIZE);
    GLDrawBorder(x0, y0, WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE);
	if (!_lose && !_win)
	{
		moveFigure(Direction::DOWN);
	}
	/// draw game field
	drawGameField();
	drawFigure();

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

void Game::moveFigureToTop()
{
	_x_figure = X_BEGIN;
	_y_figure = HEIGHT;
}

void Game::Update()
{
    if (!_paused)
    {
		if (_y_figure == HEIGHT && !canMove(Direction::DOWN))
        {
            _lose = true;
        }

		if (!canMove(Direction::DOWN))
		{
			updateGameField();
			_figure = Figures::getRandomFigure();
			moveFigureToTop();
		}

		if(_score == WIN_NUNBER)
		{
			_win = true;
			_SpeedUp();
		}
    }
}

void Game::_Restart()
{
    _lose   = false;
    _paused = false;
    _score  = 0;
	_win = false;

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

bool Game::isOutOfBoundary(int i, int j)
{
	return true;
}

bool Game::canMove(Direction direction)
{
	int fig_size = _figure.getSize(); 

	switch (direction)
	{
	case LEFT:
		for (int i = 0; i < fig_size; ++i)
		{
			for (int j = 0; j < fig_size; ++j)
			{
				if (_figure.getIJ(j, i) * (_x_figure + i - 1) < 0)
					return false;					
			}
			for (int j = 0; j < fig_size; ++j)
			{
				if ((j + _x_figure < 0) || (j + _x_figure > WIDTH - 1) || (i + _y_figure > HEIGHT - 1))
					continue;

				if (_field[i + _y_figure][j + _x_figure] * _figure.getIJ(i, j) == 1) 
					return false;
			}
		}
		break;

	case RIGHT:
		for (int i = 0; i < fig_size; ++i)
		{
			for (int j = 0; j < fig_size; ++j)
			{
				if (_figure.getIJ(j, i) * (_x_figure + i + 1) > WIDTH -1 )
					return false;
			}
			for (int j = 0; j < fig_size; ++j)
			{
				if ((j + _x_figure < 0) || (j + _x_figure > WIDTH - 1) || (i + _y_figure > HEIGHT - 1))
					continue;

				if (_field[i + _y_figure][j + _x_figure] * _figure.getIJ(i, j) == 1)
					return false;
			}
		}
		break;

	case DOWN:
		for (int i = 0; i < fig_size; ++i)
		{
			for (int j = 0; j < fig_size; ++j)
			{
				if (_figure.getIJ(i, j) * (_y_figure + i - 1) < 0)
					return false;
			}

			for (int j = 0; j < fig_size; ++j)
			{
				if ((i + _y_figure + 1 > HEIGHT - 1) || (j + _x_figure < 0) || (j + _x_figure + 1 > WIDTH - 1) || (i + _y_figure - 1 < 0))
					continue;

				if (_field[i + _y_figure - 1][j + _x_figure] * _figure.getIJ(i, j) == 1)
					return false;
			}
		}
		break;
	}
	return true;
}

void Game::moveFigure(Direction direction)
{
	if ( direction == Direction::DOWN && canMove(Direction::DOWN))
		--_y_figure;

	if (direction == Direction::RIGHT && canMove(Direction::RIGHT))
		++_x_figure;

	if (direction == Direction::LEFT && canMove(Direction::LEFT))
		--_x_figure;
}



