#include "logic.hpp"
#include <iostream>

Logic::Logic(int argX, int argY){
    coorZero.setX(argX/2 - 1);
    coorZero.setY(3);
    colorMatrix.resize(argX * argY);
    logicMatrix.resize(argX * argY);

    nextMatrix.resize(6*4);

    logicX = argX;
    logicY = argY;
    
    score = 0;
    level = 0;
    rows = 0;
    localRows = 0;
    slowness = 98;

    hardDrop = false;

    status = 0;

    Tetromino newPiece(coorZero);
    nextPiece = newPiece;
    newTetromino();
    updateMatrix();
}
vector<string> Logic::getMatrix(){
    return colorMatrix;
}
vector<string> Logic::getNext(){
    return nextMatrix;
}
int Logic::getScore(){
    return score;
}
int Logic::getLevel(){
    return level;
}
int Logic::getRows(){
    return rows;
}
int Logic::GetSlowness(){
    return slowness;
}
void Logic::setStatus(int arg){
    status = arg;
}
int Logic::getStatus(){
    return status;
}
void Logic::newTetromino(){
    activePiece = nextPiece;
    savedPos = activePiece.getPixels();
    Tetromino newPiece(coorZero);
    nextPiece = newPiece;
    fillNextMatrix();
}
void Logic::fillNextMatrix(){
    vector<Pixel> positions = nextPiece.getPixels();
    Pixel ref = positions[0];
    Pixel newRef;
    newRef.setX(2);
    newRef.setY(2);
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
void Logic::updateMatrix(){
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
void Logic::moveTetromino(char keycode){
    int modX;
    int modY;
    bool invalidMove = false;
    bool inactive = false;

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

    for (int i = 0; i < newPos.size(); i++)
    {
        if (newPos[i].getX() >= logicX || newPos[i].getX() < 0 || newPos[i].getY() >= logicY || logicMatrix[newPos[i].getX() + logicX * newPos[i].getY()] == true) //preventing side moves
        {
            invalidMove = true;
            if(keycode == 'd')
            {
                inactive = true;
            }
        }
    }

    if (invalidMove == true || inactive == true)
    {
        activePiece.setPixels(savedPos);
        if (inactive == true)
        {   
            hardDrop = false;
            for (int i = 0; i < savedPos.size(); i++)
            {
                logicMatrix[savedPos[i].getX() + logicX * savedPos[i].getY()] = true;
            }
            updateMatrix();
            CheckGameOver();
            deleteRow();
            newTetromino();
        }        
    } else {
        activePiece.setPixels(newPos);
    }
    updateMatrix();     
}
void Logic::rotateTetromino(){
    int biggestYRef = -1;
    int biggestXRef = -1;
    
    int biggestY = -1;
    int biggestX = -1;
    
    savedPos = activePiece.getPixels();
    Pixel refPixel = savedPos[0];
    vector<Pixel> newPos = savedPos;
    
    bool invalidMove;

    bool topOut = false;

    for (int i = 0; i < newPos.size(); i++) //correction
    {
        biggestYRef = (newPos[i].getY() > biggestYRef) ? newPos[i].getY() : biggestYRef;
        biggestXRef = (newPos[i].getX() > biggestXRef  ) ? newPos[i].getX() : biggestXRef;
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
    }

    int modY = biggestYRef - biggestY;
    int modX = biggestXRef - biggestX;
    for (int i = 0; i < newPos.size(); i++)
    {   
        newPos[i].modX(modX);
        newPos[i].modY(modY);
    }

    int correction = 1;
    
    do {
        invalidMove = false;
        for (int i = 0; i < newPos.size(); i++)
        {   
            if (newPos[i].getX() < 0)
            {
                invalidMove = true;
                break;
            }
        }
        if(invalidMove)
        {
            for (int i = 0; i < newPos.size(); i++)
            {   
                newPos[i].modX(correction);
            }
        }
    }
    while(invalidMove);

    invalidMove = false;
    for (int i = 0; i < newPos.size(); i++)
    {
        if (logicMatrix[newPos[i].getX() + logicX * newPos[i].getY()] == true) //deleting another piece
        {
            invalidMove = true;
            break;
        }
    }
    if (invalidMove || topOut)
    {
        activePiece.setPixels(savedPos);
    } else {
        activePiece.setPixels(newPos);
    }    
    updateMatrix();
}
void Logic::deleteRow(){
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
            rowsToDelete.insert(rowsToDelete.end(),rowsToCheck[j]);
        }
    }
    if (rowsToDelete.size() > 0)
    {
        rows += rowsToDelete.size();
        localRows += rowsToDelete.size();
        CalcScore(rowsToDelete.size());
        CalcLevel();
        sort(rowsToDelete.begin(),rowsToDelete.end());
        for (int j = 0; j < rowsToDelete.size(); j++)
        {
            for (int k = rowsToDelete[j]; k > 0; k--)
            {
                for (int i = 0; i < logicX; i++)
                {
                    colorMatrix[i + logicX * k] = colorMatrix[i + logicX * (k - 1)];
                    logicMatrix[i + logicX * k] = logicMatrix[i + logicX * (k - 1)];           
                }            
            }       
        }    
    }  
}
void Logic::CalcLevel(){
    while (localRows >= 8)
    {
        level++;
        CalcSlowness();
        localRows -= 8;
    }    
}
void Logic::CalcSlowness(){
    slowness = (slowness > 7) ? slowness - 14 : 7;
}
void Logic::CalcScore(int argRows){
   switch (argRows)
   {
   case 1:
       score += 40*(level+1);
       break;
    case 2:
       score += 100*(level+1);
       break;
    case 3:
       score += 300*(level+1);
       break;
    case 4:
       score += 1200*(level+1);
       break;
   }
}
void Logic::CheckGameOver(){
    for (int i = 0; i < logicX; i++)
    {
        if (logicMatrix[i + logicX * 3] == 1)
        {
            status = 3;
            break;
        }        
    }    
}
void Logic::CleanUp(){
    for (int i = 0; i < logicX * logicY; i++)
    {
        colorMatrix[i] = "";
        logicMatrix[i] = false;
    }
    
    score = 0;
    level = 0;
    rows = 0;
    localRows = 0;
    slowness = 98;

    status = 1;

    Tetromino newPiece(coorZero);
    nextPiece = newPiece;
    newTetromino();    
}
void Logic::ActivateHardDrop(){
    hardDrop = true;
    while (hardDrop)
    {
        moveTetromino('d');
    }    
}
Logic::~Logic(){
}
