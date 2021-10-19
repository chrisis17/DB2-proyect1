#ifndef PROYECTOBD2_BUCKET_HASHING_H
#define PROYECTOBD2_BUCKET_HASHING_H

#include "record_hashing.h"

#define MAX_B 3  // factor de bloque
#define M 3      // profundidad global

class bucket_hashing{
    record_hashing records[MAX_B];
    long lh = 1; // local height
    long size = 0;
    long next = -1;
public:
    void setNext(long next);
    long getNext();
    long getSize();
    void setLH(long local_height);
    long getLH();
    bool isEmpty();
    bool isFull();
    bool isMaxLH();
    bool existsRecord(string key);
    record_hashing* searchRecord(string key);
    void insertRecord(record_hashing record);
    void deleteRecord(string key);
    bucket_hashing divBucket();
    void searchRange(string k1, string k2, vector<record_hashing> &v);
};

void bucket_hashing::setNext(long _next) {
    this->next = _next;
}

long bucket_hashing::getNext() {
    return next;
}

long bucket_hashing::getSize() {
    return size;
}

void bucket_hashing::setLH(long _lh) {
    this->lh = _lh;
}

long bucket_hashing::getLH() {
    return lh;
}

bool bucket_hashing::isEmpty() {
    return size == 0;
}

bool bucket_hashing::isFull()  {
    return size == MAX_B;
}

bool bucket_hashing::isMaxLH()  {
    return lh == M;
}

bool bucket_hashing::existsRecord(string key) {
    for(int i=0; i<size; i++)
        if(records[i].getKey() == key)
            return true;
    return false;
}

record_hashing *bucket_hashing::searchRecord(string key) {
    for(int i=0; i<size; i++){
        if(records[i].getKey() == key){
            auto* record = new record_hashing;
            *record = records[i];
            return record;
        }
    }
    return nullptr;
}

void bucket_hashing::insertRecord(record_hashing record){
    records[size++] = record;
}

void bucket_hashing::deleteRecord(string key){
    int p = 0;
    for(int i = 0; i<size; i++){
        p = i;
        if(records[i].getKey() == key )
            break;
    }
    for(int i = p; i < size - 1; i++)
        records[i] = records[i+1];
    size--;
}

bucket_hashing bucket_hashing::divBucket(){
    hash<string> hash;
    bucket_hashing result;
    string bin;
    vector<string> recordsDelete;

    this->lh++;
    result.setLH(this->lh);

    for(int i=0; i<size; i++){
        bin = bitset<MAX_B>(records[i].getKey()).to_string();
        string tmp{};
        for (int j = bin.size() - this->lh; j < bin.size(); j++)
            tmp += bin[j];
        bin = tmp;
        if(bin[0] == '1'){
            result.insertRecord(records[i]);
            recordsDelete.push_back(records[i].getKey());
        }
    }
    for(auto key : recordsDelete)
        deleteRecord(key);

    return result;
}

void bucket_hashing::searchRange(string k1, string k2, vector<record_hashing> &v){
    hash<string> hash;
    for(int i=0; i<size; i++)
        if(hash(k1) <= hash(records[i].getKey()) && hash(records[i].getKey()) <= hash(k2))
            v.push_back(records[i]);
}

#endif //PROYECTOBD2_BUCKET_HASHING_H
