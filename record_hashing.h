#ifndef PROYECTOBD2_RECORD_HASHING_H
#define PROYECTOBD2_RECORD_HASHING_H

#include <iostream>
#include <fstream>
#include <cstdio>
#include <map>
#include <utility>
#include <vector>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <cstring>
#include <algorithm>
#include <cstdio>
#include <functional>
#include <bitset>
#include <sys/stat.h>

using namespace std;

class record_hashing{
private:
    //Name,Date,Rate,Votes,Genre,,,,,,,,,,
    //The Many Saints of Newark,2021,6.4,27145,Crime; Drama,,,,,,,,,
    string name;
    int date{};
    float rate{};
    int votes{};
    string genere;
public:
    record_hashing() = default;
    record_hashing(string _name,int _date,float _rate,int _votes,string _genere):
        name{std::move(_name)},
        date{_date },
        rate {_rate },
        votes {_votes },
        genere{std::move(_genere)}{}

    string getKey();
    void setData(string name,int date,float rate,int votes,string genere);
    record_hashing& operator =(const record_hashing& record);
};

string record_hashing::getKey() { return name; }

void record_hashing::setData(string _name, int _date, float _rate, int _votes, string _genere) {
    this->name = std::move(_name);
    this->date = _date;
    this->rate = _rate;
    this->votes = _votes;
    this->genere = std::move(_genere);
}

record_hashing &record_hashing::operator=(const record_hashing &record) {
    this->name = record.name;
    this->date = record.date;
    this->rate = record.rate;
    this->votes = record.votes;
    this->genere = record.genere;
    return *this;
}

#endif //PROYECTOBD2_RECORD_HASHING_H
