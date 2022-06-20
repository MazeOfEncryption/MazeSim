#include "Dep.hpp"
#include <set>
#include <iostream>
#include <random>

using Point = std::pair<int, int>;

Point toRectangular (Point in) {
    return {in.second % 2 == 0 ? in.first * 2 : in.first * 2 + 1, in.second};
}

int main () {
    std::set<Point> board;

    // board.insert({ 2, -1});
    // board.insert({ 1,  2});
    // board.insert({-4,  3});
    board.insert({ -2, -2});
    board.insert({ -2, -1});
    board.insert({ -2,  0});
    board.insert({ -2,  1});
    board.insert({ -2,  2});
    board.insert({ -1, -2});
    board.insert({ -1, -1});
    board.insert({ -1,  0});
    board.insert({ -1,  1});
    board.insert({ -1,  2});
    board.insert({  0, -2});
    board.insert({  0, -1});
    board.insert({  0,  0});
    board.insert({  0,  1});
    board.insert({  0,  2});
    board.insert({  1, -2});
    board.insert({  1, -1});
    board.insert({  1,  0});
    board.insert({  1,  1});
    board.insert({  1,  2});
    board.insert({  2, -2});
    board.insert({  2, -1});
    board.insert({  2,  0});
    board.insert({  2,  1});
    board.insert({  2,  2});

    // std::mt19937 gen(0);
    // std::uniform_int_distribution<int> dis(-4, 4);

    // for (int i = 0; i < 10; i++) {
    //     board.insert({dis(gen), dis(gen)});
    // }

    for (Point point : board) {
        Point rect = toRectangular(point);
        std::cout << "(" << ((point.first<0)?(""):(" ")) << point.first << ", ";
        std::cout << ((point.second<0)?(""):(" ")) << point.second << ")";
        std::cout << " -> ";
        std::cout << "(" << ((rect.first<0)?(""):(" ")) << rect.first << ", ";
        std::cout << ((rect.second<0)?(""):(" ")) << rect.second << ")";
        std::cout << std::endl;
    }

    return 0;
}