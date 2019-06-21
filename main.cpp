#include <iostream>
#include "tinyxml2.h"
#include <fstream>
#include "serialization/serializable.h"

using namespace std;

using namespace tinyxml2;

// inhert serializable interface
class shape: public serializable {
public:
    double area; // double type field
    std::string type; // string type field

    bool write_object() override { // implement write_object method
        if(not write("area", area)) return false; // invoke one of the overload methods write(...) to register data field: area
        if(not write("type", type)) return false; // invoke one of the overload methods write(...) to register data field: type
        return true;
    }

    bool read_object() override { // implement read_object method
        if(not read("area", area)) return false; // invoke one of the overload methods write(...) to recover data field: area
        if(not read("type", type)) return false; // invoke one of the overload methods write(...) to recover data field: area
        return true;
    }

    virtual void reset() { // reset this object's data field
        area = 0;
        type = "";
    }

    virtual std::string to_string() { // print data to string format
        return "[area: " + std::to_string(area) + "\ntype: " + type + "]\n";
    }

};

class triangle: public shape {
public:
    double edges[3];
    double angles[3];

    bool write_object() override {
        if(not shape::write_object()) return false; // invoke base class shape's method write_object
        if(not write("edges", edges, 3)) return false;
        if(not write("angles", angles, 3)) return false;
        return true;
    }

    bool read_object() override {
        if(not shape::read_object()) return false;
        double * pointer;
        int length;
        if(not read("edges", pointer, length)) return false;
        for(int i = 0; i < length; i++) {
            edges[i] = pointer[i];
        }

        if(not read("angles", pointer, length)) return false;
        for(int i = 0; i < length; i++) {
            angles[i] = pointer[i];
        }
        return true;
    }

    void reset() {
        shape::reset();
        edges[0] = edges[1] = edges[2] = 0;
        angles[0] = angles[1] = angles[2] = 0;

    }

    std::string to_string() {
        return "[" + shape::to_string() + "edges: {" + std::to_string(edges[0]) + ", " + std::to_string(edges[1]) + ", " + std::to_string(edges[2]) + "}\n" + "angles: {" + std::to_string(angles[0]) + ", " + std::to_string(angles[1]) + ", " + std::to_string(angles[2]) + "}]\n";
        ;
    }
};

class combine: public shape {
public:
    triangle triangle_a;
    triangle triangle_b;

    bool write_object() override {
        if (not shape::write_object()) return false;
        if (not write("triangle_a", "triangle", "1.0", triangle_a)) return false;
        if (not write("triangle_b", "triangle", "1.0", triangle_b)) return false;
        return true;
    }

    bool read_object() override {
        if(not shape::read_object()) return false;
        if(not read("triangle_a", "triangle", "1.0", triangle_a)) return false;
        if(not read("triangle_b", "triangle", "1.0", triangle_b)) return false;
        return true;
    }

    void reset() {
        triangle_a.reset();
        triangle_b.reset();
    }

    std::string to_string() {
        return "[" + shape::to_string() + triangle_a.to_string() + triangle_b.to_string() + "]\n";
    }
};

void serialize_test() {
    cout << "\nShape Object S-->" << endl;
    shape s;
    s.area = 1.2;
    s.type = "Shape Base";
    s.serialization("s", "shape", "1.0");
    cout << s.to_string() + "Serialize: " << endl;
    s.get_document()->Print();
    s.reset();
    s.deserialization("s", "shape", "1.0");
    cout << "DeSerialize: \n" + s.to_string() << endl;

    cout << "\nTriangle Object T-->" << endl;
    triangle t, c_t;
    t.area = 1.732;
    t.type = "Triangle";
    t.edges[0] = 2;
    t.edges[1] = 2;
    t.edges[2] = 2;
    t.angles[0] = 1.047;
    t.angles[1] = 1.047;
    t.angles[2] = 1.047;
    t.serialization("t", "triangle", "1.0");
    cout << t.to_string() + "Serialize: " << endl;
    t.get_document()->Print();
    c_t = t;
    t.reset();
    t.deserialization("t", "triangle", "1.0");
    cout << "DeSerialize: \n" + t.to_string() << endl;

    cout << "\nTriangle Object H-->" << endl;
    triangle h, c_h;
    h.area = 0.5;
    h.type = "Triangle";
    h.edges[0] = 1;
    h.edges[1] = 1;
    h.edges[2] = 1.414;
    h.angles[0] = 0.785;
    h.angles[1] = 0.785;
    h.angles[2] = 1.570;
    h.serialization("h", "triangle", "1.0");
    cout << h.to_string() + "Serialize: " << endl;
    h.get_document()->Print();
    c_h = h;
    h.reset();
    h.deserialization("h", "triangle", "1.0");
    cout << "DeSerialize: \n" + h.to_string() << endl;

    cout << "\nCombine Object C-->" << endl;
    combine c;
    c.area = 2.232;
    c.type = "Combine";
    c.triangle_a = c_t;
    c.triangle_b = c_h;
    c.serialization("c", "combine", "1.0");
    cout << c.to_string() + "Serialize: " << endl;
    c.get_document()->Print();
    c.reset();
    c.deserialization("c", "combine", "1.0");
    cout << "DeSerialize: \n" + c.to_string() << endl;
}

int main() {
    serialize_test();
    return 0;
}
