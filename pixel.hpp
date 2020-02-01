class Pixel
{
private:
    int x;
    int y;
public:
    Pixel();
    Pixel(int argX, int argY);
    int getX();
    int getY();
    void setX(int argX);
    void setY(int argY);
    void modX(int argModX);
    void modY(int argModY);
    void Rotate();
    Pixel operator - (const Pixel& operand)
    {
        Pixel result = *this;
        result.x -= operand.x;
        result.y -= operand.y;
        return result;
    }
    Pixel operator + (const Pixel& operand)
    {
        Pixel result = *this;
        result.x += operand.x;
        result.y += operand.y;
        return result;
    }
    ~Pixel();
};