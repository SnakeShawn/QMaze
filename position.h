#ifndef POSITION
#define POSITION

class Position{
public:
    int x;
    int y;
    Position(int a=0, int b=0):x(a),y(b){}

    bool operator == (Position P)
    {
        return (x == P.x)&&(y == P.y);
    }

    bool isInRange(int rs, int re, int cs, int ce){
        return x>=rs && x <=re && y>=cs && y<=ce;
    }

    Position getMiddle(Position p){return Position((x+p.x)/2,(y+p.y)/2);}
    Position up(int n){return Position(x-n,y);}
    Position down(int n){return Position(x+n, y);}
    Position left(int n){return Position(x, y-n);}
    Position right(int n){return Position(x,y+n);}
};

#endif // POSITION

