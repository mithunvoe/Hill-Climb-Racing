#include "highscore.hpp"
vector<pair<int, string>> scoreVector;

void Score::reCalculate()
{

    sort(scoreVector.begin(), scoreVector.end(), greater<pair<int, string>>());

    fstream scorefile;
    scorefile.open("highscore.txt", ios::out );

    for (auto &it : scoreVector)
    {
        scorefile << it.first << ' ' << it.second << endl;
    }
    scorefile.close();

}
void Score::addScore(int score, const string &name)
{

    scoreVector.push_back({score, name});
    reCalculate();
}
void Score::inputScore()
{
    int score;
    string name;
    ifstream scorefile;
    scorefile.open("highscore.txt");
    while (scorefile >> score >> name)
    {
        scoreVector.push_back({score, name});
    }
    scorefile.close();
}

vector<pair<int, string>> Score::topFive(){
    vector<pair<int, string>> v;
    for (int i = 0; i < min((int)scoreVector.size(), 5); i++)
    {
        v.push_back(scoreVector[i]);
    }
    return v;
}