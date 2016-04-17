#pragma once

#include <memory>
#include <vector>

class Figure 
{
public:
	Figure(int size);
	void setIJ(int i, int j, int val)
	{
		if (i > size && j > size)
			return;
		field[i][j] = val;
	}

	int getIJ(int i, int j) const
	{
		return field[i][j];
	}

	void rotateRight();
	void rotateLeft();

private:
	int size;
	std::vector<std::vector<int>> field;
};

class Figures
{
public:
	static Figure getRandomFigure();
	Figures();
private:
	static std::vector<Figure> figures;
};

struct Point
{
	Point():x(0), y(0){}
	Point(int x, int y):x(x), y(y){}
	int x;
	int y;
};

class Game
{
public:
    Game();

public:
    void Draw(); // NVI
    void KeyPressed(int specialKey, unsigned char key, int x, int y);
    void Update();
    unsigned UpdateInterval();

private:
	void addPointToSnake(Point);
	void deletePointFromSnake();
	void setTarget();
	void moveSnake();

private:
    virtual void DoDraw();

private:
    void _Pause();
    void _Restart();
    void _Stop();

private:
    void _SpeedUp();
    void _SpeedDown();

private:
    void _DrawStatus(unsigned x0, unsigned y0);

private:
    void _InGameKey(int specialKey, unsigned char key);

private:
    void print(int& value);
    void print(int value);

private:
//game state
    unsigned      _score;
    unsigned      _speed;
    bool          _lose;
    bool          _paused;
	bool          _win;
    unsigned      x0;
    unsigned      y0;
    unsigned      cell_x, cell_y;
	unsigned      _eatenTargets;
	unsigned direction;
	std::deque<Point> snakeCoords;
	Point target;
};

typedef std::unique_ptr<Game> GamePtr;