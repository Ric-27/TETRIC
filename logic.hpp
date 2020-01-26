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
    void rotateTetromino();
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
    savedPos = activePiece.getPixels();
    vector<Pixel> newPos = savedPos;
    for (int i = 0; i < newPos.size(); i++)
    {
        newPos[i].modX(modX);
        newPos[i].modY(modY);
    }
    bool invalidSideMove = false;
    bool inactive = false;
    for (int i = 0; i < newPos.size(); i++)
    {
        if (newPos[i].getX() >= logicX || newPos[i].getX() < 0) // going out from the sides
        {
            invalidSideMove = true;
        }
        if (newPos[i].getY() >= logicY) // going out from the bottom
        {
            inactive = true;
        }
    }
    if (invalidSideMove == true || inactive == true)
    {
        activePiece.setPixels(savedPos);
        if (inactive == true)
        {
            newTetromino();
        }        
    } else {
        activePiece.setPixels(newPos);
    }        
    updateMatrix();
}

void Logic::rotateTetromino()
{
    int biggestYRef = -1;
    int biggestXRef = -1;
    int smallestXRef = logicX + 1;
    
    int biggestY = -1;
    int biggestX = -1;
    int smallestX = logicX + 1;
    
    savedPos = activePiece.getPixels();
    Pixel refPixel = savedPos[0];
    vector<Pixel> newPos = savedPos;
    
    for (int i = 0; i < newPos.size(); i++) //correction
    {
        biggestYRef = (newPos[i].getY() > biggestYRef) ? newPos[i].getY() : biggestYRef;
        biggestXRef = (newPos[i].getX() > biggestXRef  ) ? newPos[i].getX() : biggestXRef;
        smallestXRef = (newPos[i].getX() < smallestXRef) ? newPos[i].getX() : smallestXRef;
    }

    for (int i = 0; i < newPos.size(); i++)
    {
        newPos[i] = newPos[i] - refPixel;
        newPos[i].Rotate();
        newPos[i] = newPos[i] + refPixel;
    }

    for (int i = 0; i < newPos.size(); i++) //correction
    {
        biggestY = (newPos[i].getY() > biggestY) ? newPos[i].getY() : biggestY;
        biggestX = (newPos[i].getX() > biggestX) ? newPos[i].getX() : biggestX;
        smallestX = (newPos[i].getX() < smallestX) ? newPos[i].getX() : smallestX;
    }
    if (biggestY != biggestYRef)
    {
        int modY = biggestYRef - biggestY;
        for (int i = 0; i < newPos.size(); i++)
        {
            //newPos[i].modY(modY);
        } 
    }
    activePiece.setPixels(newPos);
    updateMatrix();
}
Logic::~Logic()
{
}
