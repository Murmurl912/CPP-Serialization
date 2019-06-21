//
// Created by murmu on 2019/5/14.
//

#include "serializable.h"

serializable::serializable() {
    document = nullptr;
    root = nullptr;
}

serializable::~serializable() {
    // problems code
}

bool serializable::serialization(const std::string &name, const std::string &type, const std::string &version) {
    delete document; // free memory
    document = nullptr;
    document = new XMLDocument(); // create the document
    if(document == nullptr) { // return false if new failed
        return false;
    }

    // add doc dec
    XMLDeclaration * xml = document->NewDeclaration(R"(xml version = "1.0" encoding = "utf-8")");
    if(xml == nullptr) {
        return false;
    }

    document->InsertFirstChild(xml);
    // root
    root = document->NewElement(name.c_str());
    root->SetAttribute("type", type.c_str());
    root->SetAttribute("version", version.c_str());
    document->InsertEndChild(root);
    return write_object();
}

bool serializable::deserialization(const std::string &name, const std::string &type, const std::string &version) {
    if(document == nullptr) { // return false if new failed
        return false;
    }

    root = document->FirstChildElement();
    if(root == nullptr) { // return false if no child element node was found
        return false;
    }

    if(root->Value() != name) { // return false if root element name is not the name of this instance
        return false;
    }

    const XMLAttribute * class_attr = root->FindAttribute("type");
    if(class_attr == nullptr) { // return false if no attribute named class
        return false;
    }

    if(class_attr->Value() != type) { // return false if value of class is not giving class_name
        return false;
    }

    const XMLAttribute * version_attr = root->FindAttribute("version");
    if(version_attr == nullptr) { // return false if no attribute named version
        return false;
    }

    if(version_attr->Value() != version) { // return false if value of version is not giving version
        return false;
    }

    return read_object();
}

void serializable::set_document(XMLDocument *doc) {
    this->document = doc;
}

const XMLDocument *serializable::get_document() {
    return document;
}

bool serializable::write(const string &name, const signed int &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const int *&value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document failed to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const std::string &name, const int *array, int length) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, array, length);
    if(element == nullptr) { // document failed to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const string &type, const string &version, serializable &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, type, version, value);
    if(element == nullptr) { // document failed to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::read(const std::string &name, signed int &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const string &name, int *&result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, int *&result, int &length) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result, length);
}

bool serializable::read(const std::string &name, const string &type, const string &version, serializable &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, type, version, result);
}

bool serializable::write(XMLElement *(*write_action)(), const string &name) {
    if(root == nullptr or document == nullptr) {
        return false;
    }
    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = write_action();

    if(element == nullptr) {
        return false;
    }

    if(element->GetText() != name) {
        return false;
    }

    XMLNode * node = element->DeepClone(document);
    if(node == nullptr) {
        return false;
    }

    root->InsertEndChild(node);
    return true;
}

template<typename T>
bool serializable::read(bool (*read_action)(T &), T &result) {
    return read_action(result);
}

bool serializable::set_document(const string &filename) {
    document = new XMLDocument();
    if(document->LoadFile(filename.c_str()) == XML_ERROR_FILE_READ_ERROR) {
        delete document;
        return false;
    }
    return true;
}

XMLElement *serializable::create_element(const string &name, const signed int &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "int");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const int *&value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "int*");
    XMLElement * node = create_element("value", *value);
    if(node == nullptr) {
        return nullptr;
    }
    element->InsertFirstChild(node);
    return element;
}

XMLElement *serializable::create_element(const string &name, const int *array, int length) {
    if(document == nullptr) {
        return nullptr;
    }
    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "int[]");

    if(length < 0) {
        return nullptr;
    }

    XMLElement * node = create_element("length", length);
    if(node == nullptr) {
        return nullptr;
    }
    element->InsertFirstChild(node);

    for(int i = 0; i < length; i++) {
        string index = "index_" + to_string(i);
        node = create_element("index_" + to_string(i), array[i]);
        if(node == nullptr) {
            return nullptr;
        }
        element->InsertEndChild(node);
    }
    return element;
}

XMLElement *
serializable::create_element(const string &name, const string &type, const string &version, serializable &value) {
    if(document == nullptr) {
        return nullptr;
    }
    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", type.c_str());

    if(not value.serialization(name, type, version)) {
        return nullptr;
    }

    if(value.document == nullptr) {
        return nullptr;
    }

    XMLNode * node = value.document->FirstChildElement(name.c_str());
    if(node == nullptr) {
        return nullptr;
    }
    // clone value.document to this document as a sub tree
    node = node->DeepClone(document);
    if(node == nullptr) {
        return nullptr;
    }

    element->InsertFirstChild(node);
    return element;
}

bool serializable::recover_element(XMLElement *element, const string &name, signed int &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("int")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atoi(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, int *&result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("int*")) {
        return false;
    }

    XMLElement * node = element->FirstChildElement("value");
    if(node != nullptr) {

    }
    return recover_element(node, "value", result);
}

bool serializable::recover_element(XMLElement *element, const string &name, int *&array, int &length) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }
    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("int[]")) {
        return false;
    }

    XMLElement * node = element->FirstChildElement("length");
    if(node == nullptr) {
        return false;
    }

    if(not recover_element(node, "length", length)) {
        return false;
    }

    if(length < 0) {
        return false;
    }

    vector<int> v;

    for(int i = 0; i < length; i++) {
        node = element->FirstChildElement(string("index_" + to_string(i)).c_str());
        if(node == nullptr) {
            return false;
        }
        int tmp;
        if(not recover_element(node, string("index_" + to_string(i)), tmp)) {
            return false;
        }
        v.push_back(tmp);
    }

    array = new int[v.size()];
    length = v.size();
    for(int i = 0; i < length; i++) {
        array[i] = v[i];
    }
    return true;
}

bool serializable::recover_element(XMLElement *element, const string &name, const string &type, const string &version,
                                   serializable &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }
    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string(type)) {
        return false;
    }

    XMLElement * node = element->FirstChildElement(name.c_str());
    if(node == nullptr) {
        return false;
    }

    XMLDocument * sub = new XMLDocument;
    XMLNode * clone = node->DeepClone(sub);
    sub->InsertFirstChild(clone);
    result.set_document(sub);
    return result.deserialization(name, type, version);
}

const XMLElement *serializable::get_root_element() {
    return root;
}

bool serializable::write(const string &name, const signed short &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const signed char &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const signed long &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const signed long long &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const unsigned int &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const unsigned short &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const unsigned char &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::read(const std::string &name, std::string &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::write(const string &name, const unsigned long &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const unsigned long long &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const float &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const double &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const string &name, const long double &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const std::string &name, const double *array, int length) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, array, length);
    if(element == nullptr) { // document failed to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::write(const std::string &name, const std::string &value) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    if(root->FirstChildElement(name.c_str()) != nullptr) {
        return false;
    }

    XMLElement * element = create_element(name, value);
    if(element == nullptr) { // document faild to create new element node
        return false;
    }

    root->InsertEndChild(element);
    return true;
}

bool serializable::read(const std::string &name, signed short &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, signed char &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, signed long &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, signed long long &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, unsigned int &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, unsigned short &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, unsigned char &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, unsigned long &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, unsigned long long &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, float &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, double &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, long double &result) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result);
}

bool serializable::read(const std::string &name, double *&result, int &length) {
    if(document == nullptr or root == nullptr) {
        return false;
    }

    XMLElement * element = root->FirstChildElement(name.c_str());
    if(element == nullptr) {
        return false;
    }

    return recover_element(element, name, result, length);
}

XMLElement *serializable::create_element(const string &name, const signed short &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "signed short");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const signed char &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "signed char");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const signed long &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "signed long");
    element->SetText((int64_t)value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const signed long long &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "signed long long");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const unsigned int &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "unsigned int");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const unsigned short &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "unsigned short");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const unsigned char &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "unsigned char");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const unsigned long &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "unsigned long");
    element->SetText((unsigned int)value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const unsigned long long &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "unsigned long long");
    element->SetText((unsigned int)value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const float &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "float");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const double &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "double");
    element->SetText(value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const long double &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "long double");
    element->SetText((double)value);
    return element;
}

XMLElement *serializable::create_element(const string &name, const double *array, int length) {
    if(document == nullptr) {
        return nullptr;
    }
    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "double[]");

    if(length < 0) {
        return nullptr;
    }

    XMLElement * node = create_element("length", length);
    if(node == nullptr) {
        return nullptr;
    }
    element->InsertFirstChild(node);

    for(int i = 0; i < length; i++) {
        string index = "index_" + to_string(i);
        node = create_element("index_" + to_string(i), array[i]);
        if(node == nullptr) {
            return nullptr;
        }
        element->InsertEndChild(node);
    }
    return element;
}

XMLElement *serializable::create_element(const string &name, const string &value) {
    if(document == nullptr) {
        return nullptr;
    }

    XMLElement * element = document->NewElement(name.c_str());
    if(element == nullptr) {
        return nullptr;
    }
    element->SetAttribute("type", "string");
    element->SetText(value.c_str());
    return element;
}

bool serializable::recover_element(XMLElement *element, const string &name, signed short &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("signed short")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atoi(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, signed char &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("signed char")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atoi(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, signed long &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("signed long")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atol(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, signed long long &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("signed long long")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atoll(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, unsigned int &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("unsigned int")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atoi(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, unsigned short &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("unsigned short")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atoi(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, unsigned char &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("unsigned char")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atoi(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, unsigned long &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("signed long")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atol(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, unsigned long long &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("unsigned long long")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atoll(text);
    return to_string(result) == string(text);
}

bool serializable::recover_element(XMLElement *element, const string &name, float &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("float")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atof(text);
    return true;
}

bool serializable::recover_element(XMLElement *element, const string &name, double &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("double")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atof(text);
    return true;
}

bool serializable::recover_element(XMLElement *element, const string &name, long double &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("long double")) {
        return false;
    }

    const char * text = element->GetText();
    if(text == nullptr) {
        return false;
    }

    result = atof(text);
    return true;
}

bool serializable::recover_element(XMLElement *element, const string &name, double *&array, int &length) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }
    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("double[]")) {
        return false;
    }

    XMLElement * node = element->FirstChildElement("length");
    if(node == nullptr) {
        return false;
    }

    if(not recover_element(node, "length", length)) {
        return false;
    }

    if(length < 0) {
        return false;
    }

    vector<double> v;

    for(int i = 0; i < length; i++) {
        node = element->FirstChildElement(string("index_" + to_string(i)).c_str());
        if(node == nullptr) {
            return false;
        }
        double tmp;
        if(not recover_element(node, string("index_" + to_string(i)), tmp)) {
            return false;
        }
        v.push_back(tmp);
    }

    array = new double[v.size()];
    length = v.size();
    for(int i = 0; i < length; i++) {
        array[i] = v[i];
    }
    return true;
}

bool serializable::recover_element(XMLElement *element, const string &name, std::string &result) {
    if(element == nullptr) {
        return false;
    }

    if(element->Name() != name) {
        return false;
    }

    const XMLAttribute * type_attr = element->FindAttribute("type");
    if(type_attr == nullptr) {
        return false;
    }

    if(type_attr->Value() != string("string")) {
        return false;
    } else {
        result = element->GetText();
        return true;
    }

}
