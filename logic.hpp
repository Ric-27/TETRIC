#include <vector>
#include <string>
#include "tetromino.hpp"

using namespace std;
class Logic
{
private:
    vector<string> colorMatrix;

    vector<string> nextMatrix;
    bool pieceDied;
    vector<bool> logicMatrix;
    Tetromino activePiece;
    Tetromino nextPiece;
    Pixel coorZero;
    vector<Pixel> savedPos;
    int score;
    int level;
    int logicX;
    int logicY;
public:
    Logic(int argRows, int argColumns);
    vector<string> getMatrix();
    vector<string> getNext();
    bool getPieceDied();
    int getScore();
    int getLevel();
    void newTetromino();
    void updateMatrix();
    void fillNextMatrix();
    void moveTetromino(char);
    void rotateTetromino();
    void deleteRow();
    ~Logic();
};

Logic::Logic(int argX, int argY)
{
    coorZero.setX(argX/2 - 1);
    coorZero.setY(0);
    colorMatrix.resize(argX * argY);
    logicMatrix.resize(argX * argY);

    nextMatrix.resize(6*4);

    score = 0;
    level = 0;
    logicX = argX;
    logicY = argY;

    pieceDied = true;

    Tetromino newPiece(coorZero);
    nextPiece = newPiece;
    newTetromino();
}
vector<string> Logic::getMatrix()
{
    return colorMatrix;
}
vector<string> Logic::getNext()
{
    return nextMatrix;
}
int Logic::getScore()
{
    return score;
}
int Logic::getLevel()
{
    return level;
}
bool Logic::getPieceDied()
{
    return pieceDied;
}
void Logic::newTetromino()
{
    activePiece = nextPiece;
    savedPos = activePiece.getPixels();
    Tetromino newPiece(coorZero);
    nextPiece = newPiece;
    fillNextMatrix(); 
}
void Logic::fillNextMatrix()
{
    vector<Pixel> positions = nextPiece.getPixels();
    Pixel ref = positions[0];
    Pixel newRef;
    newRef.setX(2);
    newRef.setY(1);
    for (int i = 0; i < nextMatrix.size(); i++)
    {
        nextMatrix[i] = "";
    }
    for (int i = 0; i < positions.size(); i++)
    {
        positions[i].setX(positions[i].getX() - ref.getX() + newRef.getX());
        positions[i].setY(positions[i].getY() - ref.getY() + newRef.getY());
        nextMatrix[positions[i].getX() + 6 * positions[i].getY()] = nextPiece.getColor();
    }
}
void Logic::updateMatrix()
{
    vector<Pixel> positions = activePiece.getPixels();
    for (int i = 0; i < positions.size(); i++)
    {
        colorMatrix[savedPos[i].getX() + logicX * savedPos[i].getY()] = "";
    }
    for (int i = 0; i < positions.size(); i++)
    {
        colorMatrix[positions[i].getX() + logicX * positions[i].getY()] = activePiece.getColor();
    }
}
void Logic::moveTetromino(char keycode)
{
    pieceDied = false;
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
        if (newPos[i].getY() >= logicY || logicMatrix[newPos[i].getX() + logicX * newPos[i].getY()] == true) // going out from the bottom
        {
            inactive = true;
        }
    }
    if (invalidSideMove == true || inactive == true)
    {
        activePiece.setPixels(savedPos);
        if (inactive == true)
        {
            for (int i = 0; i < savedPos.size(); i++)
            {
                logicMatrix[savedPos[i].getX() + logicX * savedPos[i].getY()] = true;
            }
            updateMatrix();
            deleteRow();
            newTetromino();
            pieceDied = true;
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
void Logic::deleteRow()
{
    vector<int> rowsToCheck, rowsToDelete;
    bool canBeDeleted;
    int i;
    for (int i = 0; i < savedPos.size(); i++)
    {
        if ((find(rowsToCheck.begin(),rowsToCheck.end(),savedPos[i].getY())) == rowsToCheck.end())
        {
            rowsToCheck.insert(rowsToCheck.end(),savedPos[i].getY());  
        }       
    }
    for (int j = 0; j < rowsToCheck.size(); j++)
    {
        //cout << rowsToCheck[j] << endl;
        int jj = rowsToCheck[j];
        canBeDeleted = true;
        for (int i = 0; i < logicX; i++)
        {
            if (logicMatrix[i + logicX * jj] == 0)
            {
                canBeDeleted = false;
                break;
            }            
        }
        if (canBeDeleted)
        {
            //cout << "holis" << endl;
            rowsToDelete.insert(rowsToDelete.end(),rowsToCheck[j]);
        }
    }
    if (rowsToDelete.size() > 0)
    {
        sort(rowsToDelete.begin(),rowsToDelete.end());
        for (int j = 0; j < rowsToDelete.size(); j++)
        {
            //cout << "j=" << j << endl;
            for (int k = rowsToDelete[j]; k > 0; k--)
            {
                //cout << "k=" << k << endl;
                for (int i = 0; i < logicX; i++)
                {
                    //cout << "i=" << i << endl;
                    colorMatrix[i + logicX * k] = colorMatrix[i + logicX * (k - 1)];
                    logicMatrix[i + logicX * k] = logicMatrix[i + logicX * (k - 1)];           
                }            
            }       
        }    
    }  
}
Logic::~Logic()
{
}
