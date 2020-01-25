#include <vector>
#include <string>
#include "tetromino.hpp"

using namespace std;
class Logic
{
private:
    vector<string> matrix;
    Tetromino activePiece;
    Pixel coorZero;
    vector<Pixel> savedPos;
    int score;
    int level;
    int logicX;
    int logicY;
public:
    Logic(int argRows, int argColumns);
    vector<string> getMatrix();
    int getScore();
    int getLevel();
    void newTetromino();
    void updateMatrix();
    void moveTetromino(char);
    ~Logic();
};

Logic::Logic(int argX, int argY)
{
    coorZero.setX(argX/2 - 1);
    coorZero.setY(0);
    matrix.resize(argX * argY);
    score = 0;
    level = 0;
    logicX = argX;
    logicY = argY;
}
vector<string> Logic::getMatrix()
{
    return matrix;
}
int Logic::getScore()
{
    return score;
}
int Logic::getLevel()
{
    return level;
}
void Logic::newTetromino()
{
    Tetromino newPiece(coorZero);
    activePiece = newPiece;
    savedPos = activePiece.getPixels();
    
}
void Logic::updateMatrix()
{
    vector<Pixel> positions = activePiece.getPixels();
    for (int i = 0; i < positions.size(); i++)
    {
        matrix[savedPos[i].getX() + logicX * savedPos[i].getY()] = "";
    }
    for (int i = 0; i < positions.size(); i++)
    {
        matrix[positions[i].getX() + logicX * positions[i].getY()] = activePiece.getColor();
    }
}
void Logic::moveTetromino(char keycode)
{
    int modX;
    int modY;
    switch (keycode)
        {
        case 'd':
            modX = 0;
            modY = 1;
            break;
        case 'l':
            modX = -1;
            modY = 0;
            break;
        case 'r':
            modX = 1;
            modY = 0;
            break;
    }
    vector<Pixel> newPos = activePiece.getPixels();
    savedPos = newPos;
    for (int i = 0; i < activePiece.getPixels().size(); i++)
    {
        newPos[i].modX(modX);
        newPos[i].modY(modY);
    }
    for (int i = 0; i < savedPos.size(); i++)
    {
        matrix[savedPos[i].getX() + logicX * savedPos[i].getY()] = "";
    }
    activePiece.setPixels(newPos);
    updateMatrix();
}
Logic::~Logic()
{
}
