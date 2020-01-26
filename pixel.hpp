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
Pixel::Pixel()
{
}
Pixel::Pixel(int argX, int argY)
{
    x = argX;
    y = argY;
}
int Pixel::getX()
{
    return x;
}
int Pixel::getY()
{
    return y;
}
void Pixel::setX(int argX)
{
    x = argX;
}
void Pixel::setY(int argY)
{
    y = argY;
}
void Pixel::modX(int argModX)
{
    x += argModX;
}
void Pixel::modY(int argModY)
{
    y += argModY;
}
void Pixel::Rotate()
{
    int temp = x;
    x = -y;
    y = temp;
}
Pixel::~Pixel()
{
}
