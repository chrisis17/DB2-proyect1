#include <ctime>
#include <sstream>
#include "extendible_hashing.h"

void insertar_hash(extendible_hashing &eh){
    ifstream archivo("imdb.csv");
    string linea;
    clock_t t;
    char delimitator = ',';
    getline(archivo,linea);
    vector<record_hashing> records;
    while (getline(archivo,linea)){
        stringstream stream(linea);

        string name,date,rate,votes,genere;

        getline(stream,name,delimitator);
        getline(stream,date,delimitator);
        getline(stream,rate,delimitator);
        getline(stream,votes,delimitator);
        getline(stream,genere,delimitator);
        if(rate == "No Rate") rate = '0';
        if(votes == "No Votes") votes = '0';
        record_hashing temp(name,stoi(date),stof(rate),stoi(votes),genere);
        records.push_back(temp);
    }
    t = clock();
    eh.insertAll(records);
    t = clock() - t;
    double time_take = ((double)t)/CLOCKS_PER_SEC;
    printf("\nTiempo %f segundos al insertar los %zu registros",time_take,records.size());
}
int main() {
    std::ofstream data;
    data.open("data.dat", std::ofstream::out | std::ofstream::trunc);
    data.close();

    std::ofstream index;
    index.open("index.dat", std::ofstream::out | std::ofstream::trunc);
    index.close();

    extendible_hashing hashing("data","index");
    insertar_hash(hashing);
    return 0;
}
