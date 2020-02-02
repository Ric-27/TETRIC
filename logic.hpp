#include <vector>
#include <string>
#include <algorithm>
#include "tetromino.hpp"

using namespace std;

const int rows_to_level_up = 6;
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
    vector<Pixel> curr_overlay;
    vector<Pixel> prev_overlay;
    bool locked;

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

    int getRows2LU();

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
    void Overlay();

    void CheckGameOver();
    void CleanUp();

    ~Logic();
};