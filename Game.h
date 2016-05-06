#pragma once

#include <memory>
#include <vector>

typedef std::vector<std::vector<int>> int_matrix;

enum Direction{ RIGHT, LEFT, DOWN };

class Figure 
{
public:
	Figure(){}
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
	int getSize() const { return size; }

private:
	int size;
	std::vector<std::vector<int>> field;
};

class Figures
{
public:
	static Figure getRandomFigure();
	static void createFigures();
	Figures(){}
private:
	static std::vector<Figure> figures;
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
	void moveFigure(Direction direction);
	void drawFigure();
	void drawGameField();
	void updateGameField();
	void removeFullRowsIfExists();
	void moveFigureToTop();
	bool canMove(Direction direction);
	bool isOutOfBoundary(int i, int j);

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
	Figure        _figure;
	int           _x_figure;
	int           _y_figure;
	int_matrix    _field;
};

typedef std::unique_ptr<Game> GamePtr;