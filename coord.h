#ifndef COORD_H
#define COORD_H

class Square;

class Coord
{
public:
    Coord() {}
    Coord(short x, short y) : _x{ x }, _y{ y } {}

    void setX(short x) { _x = x; }
    void setY(short y) { _y = y; }
    void setCoords(short x, short y) { _x = x; _y = y; }

    inline short x() const { return _x; }
    inline short y() const { return _y; }

    bool operator!=(const Coord &other) const {return _x != other._x || _y != other._y; }

private:
    short _x, _y;
};

#endif // COORD_H
