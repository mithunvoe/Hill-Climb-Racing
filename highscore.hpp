#ifndef High_Score
#define High_Score
#include "Game.hpp"
class Score
{
private:
public:
    int score = 0;
    string name;
    static void reCalculate();
    static void addScore(int n,const string& s);
    static void inputScore();
    static vector<pair<int, string>> topFive();
};
#endif