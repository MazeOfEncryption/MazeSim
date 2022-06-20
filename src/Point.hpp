#ifndef MAZESIM_POINT
#define MAZESIM_POINT
namespace MazeSim {
    class Point : public std::pair<int, int> {
    public:
        Point() : std::pair<int, int>(0, 0) {};
        Point(int x, int y) : std::pair<int, int>(x, y) {};
        Point(std::pair<int, int> point) : std::pair<int, int>(point) {};
    };
    std::ostream &operator<<(std::ostream &os, const Point &point) {
        os << "(" << point.first << ", " << point.second << ")";
        return os;
    }
}
#endif