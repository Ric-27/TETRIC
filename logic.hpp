#include <vector>
#include <string>
#include <algorithm>
#include "tetromino.hpp"

using namespace std;
class Logic
{
private:
    vector<string> colorMatrix;
    vector<string> nextMatrix;
    vector<bool> logicMatrix;

    Tetromino activePiece;
    Tetromino nextPiece;

    Pixel coorZero;
    vector<Pixel> savedPos;

    int score;
    int level;
    int rows;
    int localRows;
    int slowness;

    int status; //0 start, 1 playing, 2 pause, 3 gameover

    bool hardDrop;

    int logicX;
    int logicY;
public:
    Logic(int argRows, int argColumns);
    vector<string> getMatrix();
    vector<string> getNext();
    int getScore();
    int getLevel();
    int getRows();

    void setStatus(int);
    int getStatus();

    int GetSlowness();
    void newTetromino();
    void updateMatrix();
    void fillNextMatrix();
    void moveTetromino(char);
    void rotateTetromino();
    void deleteRow();

    void CalcLevel();
    void CalcScore(int);
    void CalcSlowness();

    void ActivateHardDrop();

    void CheckGameOver();
    void CleanUp();

    ~Logic();
};