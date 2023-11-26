#include<iostream>
#include<fstream>
#include<vector>

using namespace std;

typedef struct Bucket {
    long long value, key;
}Bucket;

void insert_value(vector<Bucket>& bucket, long long key) {

    long long hash_key = key % 16;
    long long  save_key = hash_key;

    if (bucket[hash_key].value == -1) {
        bucket[hash_key].value = key;
    }
    // check same value in bucket
    else if (bucket[hash_key].value == key) {
        return;
    }
    else {
        for (long long i = 16; i < 32; ++i) {
            // check same value in overflow bucket
            if (bucket[i].value == key) {
                return;
            }
            // save insert value's address for connect previous value
            if (bucket[i].value % 16 == hash_key && bucket[i].key == -1) {
                save_key = i;
            }
        }
        for (int i = 16; i < 32; ++i) {
            if (bucket[i].value == -1) {
                bucket[i].value = key;
                bucket[save_key].key = i;
                return;
            }
        }
    }
}

void delete_value(vector<Bucket>& bucket, long long key) {

    long long hash_key = key % 16;
    long long cur = hash_key, next = bucket[hash_key].key;

    // find delete value
    while (bucket[cur].value != key && next != -1) {
        cur = next, next = bucket[next].key;
    }

    // connect remove value's prev value and next value
    if (next != -1) {
        bucket[cur].value = bucket[next].value;
        bucket[cur].key = bucket[next].key;
        bucket[next].key = -1;
        bucket[next].value = -1;
    }

    return;
}

// print sequentially bucket and overflow bucket 
void print_value(vector<Bucket>& bucket) {

    ofstream fout("result.txt");

    int cur = 0;
    for (int i = 0; i < 16; ++i) {
        cur = i;
        fout << cur << ":";
        // using key for print value
        while (bucket[cur].key != -1) {
            fout << bucket[cur].value << ",";
            cur = bucket[cur].key;
        }
        // print remaining bucket[cur].value
        fout << bucket[cur].value << ",\n";
    }

    fout.close();
}

int main() {

    ios_base::sync_with_stdio(false);
    cin.tie(NULL), cout.tie(NULL);

    // variables initialize
    vector<Bucket> bucket(32, { -1,-1 });
    long long read_key;
    char cmd;

    ifstream fin("hash_cmd.txt");

    while (fin >> cmd >> read_key) {
        if (cmd == 'i') {
            insert_value(bucket, read_key);
        }
        else if (cmd == 'r') {
            delete_value(bucket, read_key);
        }
    }

    fin.close();

    print_value(bucket);

    return 0;
}