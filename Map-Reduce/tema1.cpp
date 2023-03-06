#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <set>
#include <climits>
#include <queue>
#include <unordered_set>
#include <algorithm>
using namespace std;
pthread_mutex_t mutex;

typedef struct mapReducer{
    int ID;
    int M;
    int R;
    pthread_barrier_t *barrier;
    pthread_mutex_t *mutex;
    queue<string> *input;
    vector<vector<unordered_set<int>>> *v_mapped;
    vector<unordered_set<int>> *powers;
    unordered_set<int> *reducer;

}MR;

void mapFunc(vector<vector<unordered_set<int>>> &v_mapped, int maper_num,int no_reducers, int num, vector<unordered_set<int>> powers) {
    
}


int raise_to_power(int x, unsigned n) {
    int pow = 1;
    for (unsigned i = 0; i < n; i ++)
        pow = pow * x;
    return pow;
}

vector<unordered_set<int>> perfect_power(int power) {
    auto v = new vector<unordered_set<int>>();

    for(auto i = 2; i <= power + 1; i++) {
        auto s = new unordered_set<int>();
        s->insert(1);
        for(int j = 2; raise_to_power(j, i) < INT_MAX; j++) {
            if (raise_to_power(j, i) < 0)
                break;
            s->insert(raise_to_power(j, i));
        }
        v->push_back(*s);
    }
    return *v;
}

queue<string> createQueue(string FileName) {
    ifstream in;
    in.open(FileName);
    if (!in) {
        cout << "nu";
    }
    int no_files = 0;
    in >> no_files;
    auto que = new queue<string>();

    for(auto i = 0; i < no_files; i++) {
        string in_file;
        in >> in_file;
        que->push(in_file);
    }
    in.close();

    return *que;
}

void* threadFunc(void* arg) {
    MR mapReduce = *(MR*)arg;
    if (mapReduce.ID < mapReduce.M) {
        while (!mapReduce.input->empty()) {
            pthread_mutex_lock(mapReduce.mutex);
            if (!mapReduce.input->empty()) {
                string fileToBeParsed = mapReduce.input->front();
                mapReduce.input->pop();
                pthread_mutex_unlock(mapReduce.mutex);
                ifstream file;
                file.open(fileToBeParsed);
                int numberOfElements;
                file >> numberOfElements;
                for (auto i = 0; i < numberOfElements; i ++) {
                    int number;
                    file >> number;
                    for (auto j = 0; j < mapReduce.R; j ++) {
                        if ((*mapReduce.powers)[j].find(number) != (*mapReduce.powers)[j].end()) {
                            if ((*mapReduce.v_mapped)[mapReduce.ID].empty()) {
                                (*mapReduce.v_mapped)[mapReduce.ID] = vector<unordered_set<int>>(mapReduce.R);
                            }
                            (*mapReduce.v_mapped)[mapReduce.ID][j].insert(number);
                        }
                    }
                }
                file.close();
            } else {
                pthread_mutex_unlock(mapReduce.mutex);
                break;
            }
        }
    }
    pthread_barrier_wait(mapReduce.barrier);
    if (mapReduce.ID >= mapReduce.M) {
        unordered_set<int> *set = new unordered_set<int>();
        for (auto i = 0; i < mapReduce.M; i ++) {
            if ((*mapReduce.v_mapped)[i].size() != 0) {
                for (auto it = (*mapReduce.v_mapped)[i][mapReduce.ID - mapReduce.M].begin(); it != (*mapReduce.v_mapped)[i][mapReduce.ID - mapReduce.M].end(); ++it) {
                    set->insert(*it);
                }
            }
        }
        string outputName = "out" + to_string(2 + mapReduce.ID - mapReduce.M) + ".txt";
        ofstream out;
        out.open(outputName);
        out << set->size();
        out.close();   
    }
    return NULL;    
}

int main(int argc, char** argv) {
    int M, R;
    string file;
    M = atoi(argv[1]);
    R = atoi(argv[2]);
    file = argv[3];
    pthread_t threads[M+R];
    int response;
    void* status;

    auto perfectPowers = new vector<unordered_set<int>>();
    *perfectPowers = perfect_power(R);
    auto mappers = new vector<vector<unordered_set<int>>>(M);
    auto filesQueue = new queue<string>();
    *filesQueue = createQueue(file);
    auto mutex = new pthread_mutex_t();
    auto barrier = new pthread_barrier_t();

    
    if (pthread_barrier_init(barrier,NULL, M + R) != 0) {
			exit(-1);
		}
    if(pthread_mutex_init(mutex, NULL) != 0) {
		exit(-1);
    
	}
    
    for (auto i = 0; i < M + R; i ++) {
        MR *mapReduce = (MR*)malloc(sizeof(MR));
        mapReduce -> ID = i;
        mapReduce -> M = M;
        mapReduce -> R = R;
        mapReduce -> barrier = barrier;
        mapReduce -> mutex = mutex;
        mapReduce -> input = filesQueue;
        mapReduce -> v_mapped = mappers;
        mapReduce -> powers = perfectPowers;
        
        
        response = pthread_create(&threads[i], NULL, threadFunc, mapReduce);
        if (response) {
            cout << "error\n";
            exit(-1);
        }
    }

    for (auto i = 0; i < M + R; i ++) {
        response = pthread_join(threads[i], &status);

        if (response) {
            cout << "error\n";
            exit(-1);
        }
    }

    pthread_mutex_destroy(mutex);
    pthread_barrier_destroy(barrier);
    return 0;
}