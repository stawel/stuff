#include <iostream>
#include <vector>
#include <stdlib.h>
#include <algorithm>
#include <cassert>


#define MAX 10000
#define MAX_TREE 20000
#define MAX_SIZE 10000000

#define I(x) x; cout.flush()

using namespace std;

struct Tree {
    int id;
    int pid;
    int size;
    int rsize;
    int epoch;
    int flag;
};

Tree tree[MAX_TREE];
int ile = 0;
int epoch = 0;

vector<int> dir_;
vector<int> file_;

#define D(x) x

void print(int id) {
    D(Tree &t = tree[id];
    cout << "print id:"<< t.id << " pid:" << t.pid << " size:" << t.size << "\n";
);
}

int add(int id, int pid, int size) {
//    print(pid);
    assert(tree[id].id == 0);
    assert(tree[pid].id != 0);
    assert(tree[pid].size == 0);
    tree[id] = {id,pid,size,size,epoch++, 0};
    return size;
}

void remove_all_(int id) {
    D(cout << "remove_all: " << id << "\n");
    file_.erase(std::remove(file_.begin(),file_.end(), id),file_.end());
    dir_.erase(std::remove(dir_.begin(),dir_.end(), id),dir_.end());
}

int ch_remove(int id) {
    if(tree[id].id == 0) return 0;
    if(tree[id].epoch == epoch) return 0;
    int pid = tree[id].pid;
    tree[id].epoch = epoch;
    int rsize = ch_remove(pid);
    if(tree[pid].id == 0) {
        rsize += tree[id].rsize;
        tree[id] = {0,0,0,0,0,0};
        remove_all_(id);
    }
    return rsize;
}

int remove(int id) {
//    print(id);
    assert(tree[id].id != 0);
    int rsize = tree[id].rsize;
    remove_all_(id);
    tree[id] = {0,0,0,0,0,0};
    for(auto &t: tree) {
        rsize += ch_remove(t.id);
    }
//    print(id);
    epoch++;
    return rsize;
}


int ch_count(int id) {
    if(tree[id].id == 0) return 0;
    if(tree[id].epoch == epoch) return 0;
    
    int pid = tree[id].pid;
    tree[id].epoch = epoch;
    int rsize = ch_count(pid);
    if(tree[pid].flag == epoch) {
        rsize += tree[id].rsize;
        tree[id].flag = epoch;
    }
    return rsize;
}
int my_count(int id) {
    int rsize = tree[id].rsize;
    tree[id].epoch = epoch;
    tree[id].flag = epoch;

    for(auto &t: tree) {
        rsize += ch_count(t.id);
    }
    epoch++;
    return rsize;
}

int move(int id, int pid) {
    assert(tree[id].id != 0);
    assert(tree[pid].size == 0);
    int rsize = my_count(id);
    tree[id].pid = pid;
    return rsize;
}


int ch_infect2(int id, int size) {
    if(tree[id].id == 0) return 0;
    if(tree[id].epoch == epoch) return 0;

    int pid = tree[id].pid;
    tree[id].epoch = epoch;
    int rsize = ch_infect2(pid, size);
    if(tree[pid].flag == epoch) {
        tree[id].rsize+=size;
        rsize += tree[id].rsize;
        tree[id].flag = epoch;
    }
    return rsize;
}

int infect2(int id, int size) {
    tree[id].epoch = epoch;
    tree[id].flag = epoch;
    tree[id].rsize += size;
    int rsize = tree[id].rsize;
    for(auto &t: tree) {
        rsize += ch_infect2(t.id, size);
    }
    epoch++;
    return rsize;
}

int infect(int id) {
    assert(tree[id].id != 0);
    int files = 0;
    int rsize = 0;
    for(auto &t: tree) {
        if(t.id != 0 && t.size>0) {
            rsize+=t.rsize;
            files++;
        }
    }
    int inf_size = 0;
    if(files>0) inf_size = rsize/files;
    return infect2(id, inf_size);
}


int ch_recover(int id) {
    if(tree[id].id == 0) return 0;
    if(tree[id].epoch == epoch) return 0;

    int pid = tree[id].pid;
    tree[id].epoch = epoch;
    int rsize = ch_recover(pid);
    if(tree[pid].flag == epoch) {
        tree[id].rsize=tree[id].size;
        rsize += tree[id].rsize;
        tree[id].flag = epoch;
    }
    return rsize;
}

int recover(int id) {
    assert(tree[id].id != 0);

    tree[id].epoch = epoch;
    tree[id].flag = epoch;
    tree[id].rsize = tree[id].size;
    int rsize = tree[id].rsize;
    for(auto &t: tree) {
        rsize += ch_recover(t.id);
    }
    epoch++;

    return rsize;
}


int rand_dir_id_() {
    return dir_[rand()%dir_.size()];
}

int rand_id_() {
    int i = rand()%(dir_.size()+file_.size());
    int id;
    if(i >= dir_.size()) {
        id = file_[i-dir_.size()];
    } else {
        id = dir_[i];
    }
    return id;
}

void add_() {
    int file = rand()%2;
    int size = 0;
    if(file) size = (rand()%(MAX_SIZE-1)) +1;
    int pid = rand_dir_id_();
    int id;
    I(cout << "add: ");
    do {
        id = (rand()%(MAX_TREE-1)) + 1;
    } while(tree[id].id);
    if(file) file_.push_back(id);
    else     dir_.push_back(id);
    I(cout << id << " " << pid << " " << size << "\n");
    I(cout << "return: " << add(id,pid,size) <<"\n");
}


void remove_() {
    int id;

    if(dir_.size()+file_.size()<=1) {
        I(cout << "remove_error\n");
        return;
    }

    I(cout << "remove: ");
    do {
        id = rand_id_();
    } while(id == 1);

    I(cout << id << "\n");
    I(cout << "return: " << remove(id) << "\n");
}


bool isScion_(int scion, int id) {
    if(id == scion) return true;
    if(id == 1) return true;
    if(scion == 1) return false;
    cout << "id    "; print(id);
    cout << "scion "; print(scion);
    return isScion_(tree[scion].pid, id);
}

void move_() {
    int id;
    int pid;

    if(dir_.size()<=1) {
        I(cout << "move_error\n");
        return;
    }
    I(cout << "move: ");
    do {
        id = rand_id_();
        pid = rand_dir_id_();
    } while(isScion_(pid, id));

    I(cout << id  << " " << pid << "\n");
    I(cout << "return: " << move(id, pid) << "\n");
}

void infect_() {
    int id = rand_id_();
    I(cout << "invect: " << id << "\n");
    I(cout << "return: " << infect(id) << "\n");
}

void recover_() {
    int id = rand_id_();
    I(cout << "recover: " << id << "\n");
    I(cout << "return: " << recover(id) << "\n");
}


int main() {

    for(int t=0;t<50;t++) {
    dir_.clear();
    file_.clear();
    for(auto &x: tree) x = {0,0,0,0,0,0};

    tree[1] = {1,1,0,0,0,0};
    print(1);
    dir_.push_back(1);

    for(int i = 0;i < MAX;i++) {
        int op = rand()%5;
        switch(op) {
            case 0: add_(); break;
            case 1: remove_(); break;
            case 2: move_(); break;
            case 3: infect_(); break;
            case 4: recover_(); break;
        }
    }
    }
}
