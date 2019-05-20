#include <iostream>
#include "tinyxml2.h"
#include <fstream>
#include "serialization/serializable.h"

using namespace std;

using namespace tinyxml2;

class cat: public serializable {
public:
    int integer;
    int array[10];

    cat() {

    }

    cat(int n, const int a[]) {
        integer = n;
        for(int i = 0; i < 10; i++) {
            array[i] = a[i];
        }
    }

    bool write_object() override {
        if(not write("integer", integer)) return false;
        return write("array", array, 10);
    }

    bool read_object() override {
        if(not read("integer", integer)) return false;
        int * pointer;
        int length;
        if(not read("array", pointer, length)) return false;
        for(int i = 0; i < length; i++) {
            array[i] = pointer[i];
        }
        return true;
    }
};

class cats: public serializable {
public:
    int id = 123;
    cat leader;

    bool write_object() override {
        if(not write("id", id)) return false;
        return write("leader", "cat", "1.0", leader);
    }

    bool read_object() override {
        if(not read("id", id)) return false;
        return not read("leader", "cat", "1.0", leader);
    }

};

int main() {
    int a[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    cat mur = cat(1, a);

    cats cs;
    cs.leader = mur;
    cs.serialization("cs", "cats", "1.0");
    cs.get_document()->Print();
    return 0;
}