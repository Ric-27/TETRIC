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
Pixel::~Pixel()
{
}
