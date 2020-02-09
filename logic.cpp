#include "logic.hpp"
#include <iostream>

Logic::Logic(int argX, int argY){
    coorZero.setX(argX/2 - 1);
    coorZero.setY(amount_of_pixels - 1);
    colorMatrix.resize(argX * argY);
    logicMatrix.resize(argX * argY);

    nextMatrix.resize(6*4);

    logicX = argX;
    logicY = argY;
    
    score = 0;
    level = 0;
    rows = 0;
    localRows = 0;
    slowness = initial_speed;

    special_counter = 0;

    hardDrop = false;

    status = 0;

    Tetromino newPiece(coorZero,0);
    nextPiece = newPiece;
    newTetromino();
}
vector<string> Logic::getMatrix() const{
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
    special_counter++;
    activePiece = nextPiece;
    savedPos = activePiece.getPixels();
    
    if (special_counter > count_spawn_special)
    {
        special_counter = 0;
        Tetromino newPiece(coorZero,(-2));
        nextPiece = newPiece;
    } else {
        Tetromino newPiece(coorZero,nextPiece.GetId());
        nextPiece = newPiece;
    }    
    fillNextMatrix();
    updateMatrix();
}
void Logic::fillNextMatrix(){
    vector<Pixel> positions = nextPiece.getPixels();
    Pixel ref = positions[0];
    Pixel newRef;
    newRef.setX(2);
    newRef.setY(2);
    for (unsigned i = 0; i < nextMatrix.size(); i++)
    {
        nextMatrix[i] = "";
    }
    for (unsigned i = 0; i < positions.size(); i++)
    {
        positions[i].setX(positions[i].getX() - ref.getX() + newRef.getX());
        positions[i].setY(positions[i].getY() - ref.getY() + newRef.getY());
        nextMatrix[positions[i].getX() + 6 * positions[i].getY()] = nextPiece.getColor();
    }
}
void Logic::updateMatrix(){
    Overlay();
    vector<Pixel> positions = activePiece.getPixels();
    if(!locked)
    {
        for (unsigned i = 0; i < prev_overlay.size(); i++)
        {
            colorMatrix[prev_overlay[i].getX() + logicX * prev_overlay[i].getY()] = "";
        }    
    }    
    for (unsigned i = 0; i < curr_overlay.size(); i++)
    {
        colorMatrix[curr_overlay[i].getX() + logicX * curr_overlay[i].getY()] = overlay_color;
    }
    for (unsigned i = 0; i < savedPos.size(); i++)
    {
        colorMatrix[savedPos[i].getX() + logicX * savedPos[i].getY()] = "";
    }
    for (unsigned i = 0; i < positions.size(); i++)
    {
        colorMatrix[positions[i].getX() + logicX * positions[i].getY()] = activePiece.getColor();
    }    
}
void Logic::moveTetromino(char keycode){
    int modX = 0;
    int modY = 0;
    bool invalidMove = false;
    locked = false;

    switch (keycode)
    {
        case 'd':
            modY = 1;
            break;
        case 'l':
            modX = -1;
            break;
        case 'r':
            modX = 1;
            break;
    }

    savedPos = activePiece.getPixels();
    vector<Pixel> newPos = savedPos;

    for (unsigned i = 0; i < newPos.size(); i++)
    {
        newPos[i].modX(modX);
        newPos[i].modY(modY);
    }

    for (unsigned i = 0; i < newPos.size(); i++)
    {
        if (newPos[i].getX() >= logicX || newPos[i].getX() < 0 || newPos[i].getY() >= logicY || logicMatrix[newPos[i].getX() + logicX * newPos[i].getY()] == true) //preventing side moves
        {
            invalidMove = true;
            if(keycode == 'd')
            {
                locked = true;
                
            }
        }
    }

    if (invalidMove == true || locked == true)
    {
        activePiece.setPixels(savedPos);
        if (locked == true)
        {
            hardDrop = false;
            for (unsigned i = 0; i < savedPos.size(); i++)
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
    locked = false;
    int biggestYRef = -1;
    int biggestXRef = -1;
    
    int biggestY = -1;
    int biggestX = -1;
    
    savedPos = activePiece.getPixels();
    Pixel refPixel = savedPos[0];
    vector<Pixel> newPos = savedPos;
    
    bool invalidMove;

    bool topOut = false;

    for (unsigned i = 0; i < newPos.size(); i++) //correction
    {
        biggestYRef = (newPos[i].getY() > biggestYRef) ? newPos[i].getY() : biggestYRef;
        biggestXRef = (newPos[i].getX() > biggestXRef  ) ? newPos[i].getX() : biggestXRef;
    }

    for (unsigned i = 0; i < newPos.size(); i++)
    {
        newPos[i] = newPos[i] - refPixel;
        newPos[i].Rotate();
        newPos[i] = newPos[i] + refPixel;
    }

    for (unsigned i = 0; i < newPos.size(); i++) //correction
    {
        biggestY = (newPos[i].getY() > biggestY) ? newPos[i].getY() : biggestY;
        biggestX = (newPos[i].getX() > biggestX) ? newPos[i].getX() : biggestX;
    }

    int modY = biggestYRef - biggestY;
    int modX = biggestXRef - biggestX;
    for (unsigned i = 0; i < newPos.size(); i++)
    {   
        newPos[i].modX(modX);
        newPos[i].modY(modY);
    }

    int correction = 1;
    
    do {
        invalidMove = false;
        for (unsigned i = 0; i < newPos.size(); i++)
        {   
            if (newPos[i].getX() < 0)
            {
                invalidMove = true;
                break;
            }
        }
        if(invalidMove)
        {
            for (unsigned i = 0; i < newPos.size(); i++)
            {   
                newPos[i].modX(correction);
            }
        }
    }
    while(invalidMove);

    invalidMove = false;
    for (unsigned i = 0; i < newPos.size(); i++)
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
    for (unsigned i = 0; i < savedPos.size(); i++)
    {
        if ((find(rowsToCheck.begin(),rowsToCheck.end(),savedPos[i].getY())) == rowsToCheck.end())
        {
            rowsToCheck.insert(rowsToCheck.end(),savedPos[i].getY());  
        }       
    }
    for (unsigned j = 0; j < rowsToCheck.size(); j++)
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
        for (unsigned j = 0; j < rowsToDelete.size(); j++)
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
    while (localRows >= rows_to_level)
    {
        level++;
        CalcSlowness();
        localRows -= rows_to_level;
    }    
}
void Logic::CalcSlowness(){
    slowness = (slowness > min_speed) ? slowness - step_speed : min_speed;
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
        if (logicMatrix[i + logicX * (logicY - height - 1)] == 1)
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

    Tetromino newPiece(coorZero,0);
    nextPiece = newPiece;
    newTetromino();    
}
void Logic::Overlay(){
    //cout << "overlay" << endl;
    prev_overlay = curr_overlay;
    curr_overlay = activePiece.getPixels();
    bool active = true;
    while (active)
    {
        for (unsigned i = 0; i < curr_overlay.size(); i++)
        {
            curr_overlay[i].modY(1);
        }
        for (unsigned i = 0; i < curr_overlay.size(); i++)
        {
            if (curr_overlay[i].getY() >= logicY || logicMatrix[curr_overlay[i].getX() + logicX * curr_overlay[i].getY()] == true)
            {
                active = false;
                break;
            }
        }    
    }
    for (unsigned i = 0; i < curr_overlay.size(); i++)
    {
        curr_overlay[i].modY(-1);
    }
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
