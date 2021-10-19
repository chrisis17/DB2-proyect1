#ifndef PROYECTOBD2_EXTENDIBLE_HASHING_H
#define PROYECTOBD2_EXTENDIBLE_HASHING_H

#include <utility>

#include "bucket_hashing.h"

class extendible_hashing{
private:
    string datafile;
    string indexfile;
    unordered_map<string, long> has_index;
    unordered_map<string, long> index_size;
    hash<string> hash;
    long freeListBucket = -1;
    string hash_function(string key);
    void initIndex();
    void readIndex();
    bool isFileEmpty(string fileName);
    void writeIndex();
    void insertInFreeList(fstream &f);
    void divideBucket(fstream &f, bucket_hashing &bucket, long pos);
    bool exists(string key);
public:
    extendible_hashing(const string& _fileName, const string& _indexName);
    ~extendible_hashing();
    void insert(record_hashing record);
    record_hashing* search(string key);
    vector<record_hashing> searchRange(string k1, string k2);
    bool erase(string key);

    void insertAll(vector<record_hashing> vector1);
};

string extendible_hashing::hash_function(string key) {
    auto code = hash(key);
    string bin = bitset<M>(code).to_string();
    return bin;
}

void extendible_hashing::initIndex(){

    std::ofstream ofs;
    ofs.open(datafile, std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    fstream f;
    f.open(indexfile, ios::in | ios::out | ios::binary);
    if (f.is_open()){
        fstream fa;
        fa.open(datafile, ios::in | ios::out | ios::binary);
        if (fa.is_open()){
            freeListBucket = -1;
            fa.write((char *)&freeListBucket, sizeof(freeListBucket));
            bucket_hashing b0, b1;
            long p0 = fa.tellp();
            fa.write((char *)&b0, sizeof(b0));
            long p1 = fa.tellp();
            fa.write((char *)&b1, sizeof(b1));
            string bin;
            long size = 0;
            for(long i = 0; i < (1 << M); i++){
                bin = bitset<M>(i).to_string();
                f.write(bin.c_str(), M);
                if(bin[M - 1] == '0')
                    f.write((char *)&p0, sizeof(p0));
                else
                    f.write((char *)&p1, sizeof(p1));
                f.write((char *)&size, sizeof(size));
            }
            f.close();
            fa.close();
        }else cerr << "Error initIndex\n";
    }else cerr << "Error initIndex\n";
}

void extendible_hashing::readIndex(){
    fstream f;
    f.open(indexfile, ios::in | ios::out | ios::binary);
    if (f.is_open()){
        string bin;
        long pos;
        long size;
        for(auto i = 0; i < (1 << M); i++){
            char* buffer = new char[M + 1];
            f.read(buffer, M);
            buffer[M] = '\0';
            bin = buffer;
            delete [] buffer;
            if(bin.empty())  return;
            f.read((char*)&pos, sizeof(pos));
            f.read((char*)&size, sizeof(size));
            has_index[bin] = pos;
            index_size[bin] = size;
        }
        f.close();
    } else cerr << "Error readIndex\n";
}

bool extendible_hashing::isFileEmpty(string fileName){
    fstream f;
    f.open(fileName, ios::in | ios::out | ios::binary);
    if (f.is_open()) {
        f.seekg(0, ios::beg);
        auto start = f.tellg();
        f.seekg(0, ios::end);
        auto end = f.tellg();
        return end == start;
        f.close();
    }
    return false;
}

void extendible_hashing::writeIndex(){
    fstream f;
    f.open(indexfile, ios::in | ios::out | ios::binary);
    if (f.is_open()){
        for(long i = 0; i < (1 << M); i++){
            string bin = bitset<M>(i).to_string();
            f.write(bin.c_str(), M);
            f.write((char*) &has_index[bin], sizeof(has_index[bin]));
            f.write((char*) &index_size[bin], sizeof(index_size[bin]));
        }
        f.close();
    }else cerr << "Error writeIndex\n";
}

void extendible_hashing::insertInFreeList(fstream &f){
    if(freeListBucket == -1){
        f.seekp(0, ios::end);
    }else{
        bucket_hashing to_deleted;
        f.seekg(freeListBucket, ios::beg);
        f.read((char *)&to_deleted, sizeof(to_deleted));
        f.seekp(freeListBucket, ios::beg);
        freeListBucket = to_deleted.getNext();
    }
}

void extendible_hashing::divideBucket(fstream &f, bucket_hashing &bucket, long pos){
    bucket_hashing other = bucket.divBucket();

    f.seekp(pos);
    f.write((char *)&bucket, sizeof(bucket));
    insertInFreeList(f);

    long tmp = f.tellp();
    f.seekp(tmp);
    f.write((char *)&other, sizeof(other));

    long local_height = bucket.getLH();
    for(auto &p : has_index)
        if(p.first[M - local_height] == '1' && p.second == pos)
            p.second = tmp;
}

bool extendible_hashing::exists(string key){
    string bin = hash_function(std::move(key));
    if(index_size[bin] == 0)
        return false;

    fstream f;
    f.open(datafile, ios::in | ios::out | ios::binary);
    if (f.is_open()){
        bucket_hashing b;
        long p = has_index[bin];
        while(p != -1){
            f.seekg(p, ios::beg);
            f.read((char *)&b, sizeof(b));
            if(b.existsRecord(key))
                return true;
            p = b.getNext();
        }
        return false;
        f.close();
    } else cout << "Error al abrir data en exists\n";
    return false;
}

extendible_hashing::extendible_hashing(const string& _fileName, const string& _indexName) {
    this->datafile = _fileName + ".dat";
    this->indexfile = _indexName + ".dat";
    if(isFileEmpty(indexfile))
        initIndex();
    readIndex();

    fstream f;
    f.open(datafile, ios::in | ios::out | ios::binary);
    if (f.is_open()){
        f.seekg(0, ios::beg);
        f.write((char *)&freeListBucket, sizeof(freeListBucket));
        f.close();
    } else cerr << "Error extendible_hashing\n";
}

extendible_hashing::~extendible_hashing() {
    writeIndex();
}

void extendible_hashing::insert(record_hashing record) {
    if(exists(record.getKey())){
        cerr << record.getKey() << " Registro existente\n";
        return;
    }
    string bin = hash_function(record.getKey());
    fstream f;
    f.open(datafile, ios::in | ios::out | ios::binary);
    if (f.is_open()){
        bucket_hashing bucket;
        long tmp = has_index[bin];
        f.seekg(tmp, ios::beg);
        f.read((char *)&bucket, sizeof(bucket));

        if(bucket.isFull()){
            if(bucket.isMaxLH()){
                long tmp2 = tmp;
                while(true){
                    tmp2 = tmp;
                    tmp = bucket.getNext();
                    if(tmp == -1) break;
                    f.seekg(tmp, ios::beg);
                    f.read((char *)&bucket, sizeof(bucket));
                }
                if(bucket.isFull()){
                    bucket_hashing new_bucket;
                    new_bucket.insertRecord(record);
                    insertInFreeList(f);
                    long tmp3 = f.tellp();
                    bucket.setNext(tmp3);
                    f.write((char *)&new_bucket, sizeof(new_bucket));
                    f.seekp(tmp2, ios::beg);
                    f.write((char *)&bucket, sizeof(bucket));
                }else{
                    bucket.insertRecord(record);
                    f.seekp(tmp2, ios::beg);
                    f.write((char *)&bucket, sizeof(bucket));
                }
            }else{
                bool page = false;
                while(true){
                    divideBucket(f, bucket, tmp);
                    tmp = has_index[bin];
                    f.seekg(tmp, ios::beg);
                    f.read((char *)&bucket, sizeof(bucket));
                    if(bucket.isFull() && bucket.isMaxLH()){
                        page = true;
                        break;
                    }
                    if(!bucket.isFull())
                        break;
                }
                if(page){
                    bucket_hashing other;
                    other.insertRecord(record);
                    insertInFreeList(f);
                    long new_pos = f.tellp();
                    bucket.setNext(new_pos);
                    f.write((char *)&other, sizeof(other));
                    f.seekp(tmp, ios::beg);
                    f.write((char *)&bucket, sizeof(bucket));
                }else{
                    bucket.insertRecord(record);
                    f.seekp(tmp, ios::beg);
                    f.write((char *)&bucket, sizeof(bucket));
                }
            }
        }else{
            bucket.insertRecord(record);
            f.seekp(tmp, ios::beg);
            f.write((char *)&bucket, sizeof(bucket));
        }
        index_size[bin]++;
        f.close();
    }else cerr << "Error insert\n";
}

record_hashing *extendible_hashing::search(string key) {
    string bin = hash_function(key);
    if(index_size[bin] == 0)
        return nullptr;
    fstream f;
    f.open(datafile, ios::in | ios::out | ios::binary);
    if (f.is_open()){
        bucket_hashing bucket;
        long tmp = has_index[bin];
        while(tmp != -1){
            f.seekg(tmp, ios::beg);
            f.read((char *)&bucket, sizeof(bucket));
            record_hashing* result = bucket.searchRecord(key);
            if(result){
                f.close();
                return result;
            }
            tmp = bucket.getNext();
        }
        return nullptr;
        f.close();
    } else cerr << "Error searchRange\n";
    return nullptr;
}

vector<record_hashing> extendible_hashing::searchRange(string k1, string k2) {
    vector<record_hashing> result;
    unordered_set<long> reference;
    fstream f;
    f.open(datafile, ios::in | ios::out | ios::binary);
    if (f.is_open()){
        for(long i = 0; i < (1 << M); i++){
            string bin = bitset<M>(i).to_string();
            long pos = has_index[bin];
            if(reference.count(pos))
                continue;
            reference.insert(pos);
            while(pos != -1){
                bucket_hashing bucket;
                f.seekg(pos, ios::beg);
                f.read((char *)&bucket, sizeof(bucket));
                bucket.searchRange(k1, k2, result);
                pos = bucket.getNext();
            }
        }
    } else cerr << "Error searchRange range\n";
    return result;
}

bool extendible_hashing::erase(string key) {
    if(!exists(key))
        return false;
    return false;
}

void extendible_hashing::insertAll(vector<record_hashing> vector1) {
    for(auto &v : vector1){
        insert(v);
    }
}

#endif //PROYECTOBD2_EXTENDIBLE_HASHING_H
