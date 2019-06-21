//
// Created by murmu on 2019/5/14.
//

#ifndef SERIALIZATION_SERIALIZABLE_H
#define SERIALIZATION_SERIALIZABLE_H

#include <iostream>
#include "../tinyxml2.h"
#include <string>
#include <vector>


using namespace std;
using namespace tinyxml2;


class serializable {
public:
    /**
     * default constructor set document and root to nullptr
     */
    serializable();

    /**
     * default deconstructor delete document
     */
    ~serializable();

    /**
     * set the document field of this object
     * so that deserialization can be performed
     * @param doc the document that store serialization result
     */
    void set_document(XMLDocument * doc);

    /**
     * load xml document from file
     * @param filename the name of xml document that stores serialization data
     * @return false if file cannot be found
     */
    bool set_document(const string & filename);

    /**
     * get the xml document of this object
     * @return
     */
    const XMLDocument * get_document();

    /**
     * get the root element of this object
     * @return
     */
    const XMLElement * get_root_element();

    /**
     * function that must be implement in subclass
     * which help to tell the serialization function to perform serialization
     * by invoking write(....) method to register fields in order to serialize
     * @return false if write failed
     */
    virtual bool write_object() = 0;

    /**
     * function that must be implement in subclass
     * which help to tell the deserialization function to retrieve data for xml document
     * by invoke read(...) method to read data from xml node
     * the read(...)'s parameter result should be the field that has been serialized
     * it should restore this object semantically equivalent to the serialized object
     * @return false if read failed
     */
    virtual bool read_object() = 0;

    /**
     * by invoke this function, old document will be erased
     * it perform object serialization the serialization result is stored in xml document
     * which can be visited via get_document() method
     * @param name name of this object instance name
     * @param type class or type name of this object
     * @param version version of this serialization
     * @return true if serialization operate successfully
     */
    bool serialization(const string & name, const string & type, const string & version);

    /**
     * by invoke this function, it restore the serialized object stored in xml document
     * this object will be semantically identical to the object that performed this serialization
     * @param name name of serialized object
     * @param type class or type name of serialized object
     * @param version version of serialized object
     * @return true if serialization operate successfully
     */
    bool deserialization(const string & name, const string & type, const string & version);

    /**
     * write an int type field
     * @param name name of this field
     * @param value value of this field
     * @return true if field name is unique and no error occurred in allocate memory
     */
    bool write(const string & name, const signed int & value);

    bool write(const string & name, const signed short & value);

    bool write(const string & name, const signed char & value);

    bool write(const string & name, const signed long & value);

    bool write(const string & name, const signed long long & value);

    bool write(const string & name, const unsigned int & value);

    bool write(const string & name, const unsigned short & value);

    bool write(const string & name, const unsigned char & value);

    bool write(const string & name, const unsigned long & value);

    bool write(const string & name, const unsigned long long & value);

    bool write(const string & name, const float & value);

    bool write(const string & name, const double & value);

    bool write(const string & name, const long double & value);

    /**
     * write an int* type field
     * @param name name of this field
     * @param value value of this field
     * @return true if field name is unique and no error occurred in allocate memory
     */
    bool write(const string &name, const signed int * & value);


    /**
     * write an int[length] field
     */
    bool write(const std::string & name, const signed int * array, int length);

    bool write(const std::string &name, const double *array, int length);

    /**
     *
     */
    bool write(const std::string & name, const std::string & value);

    /**
     * write a serializable field
     */
    bool write(const string & name, const string & type, const string & version, serializable & value);

    /**
     * read an int type field return it's value via result
     * @param name name of this field
     * @param result value of this value
     * @return false if no element named name in documemnt root element's chiledren
     * and data type matched
     */
    bool read(const std::string & name, signed int & result);

    bool read(const std::string & name, signed short & result);

    bool read(const std::string & name, signed char & result);

    bool read(const std::string & name, signed long & result);

    bool read(const std::string & name, signed long long & result);

    bool read(const std::string & name, unsigned int & result);

    bool read(const std::string & name, unsigned short & result);

    bool read(const std::string & name, unsigned char & result);

    bool read(const std::string & name, unsigned long & result);

    bool read(const std::string & name, unsigned long long & result);

    bool read(const std::string & name, float & result);

    bool read(const std::string & name, double & result);

    bool read(const std::string & name, long double & result);

    /**
     * read an int* type field return it's value via result
     * @param name name of this field
     * @param result value of this value
     * @return false if no element named name in documemnt root element's chiledren
     * and data type matched
     */
    bool read(const string &name, int * & result);

    /**
     * read a int[length] field
     * @param name
     * @param result
     * @param length
     * @return
     */
    bool read(const std::string & name, int * & result, int & length);

    bool read(const std::string &name, double *&result, int &length);

    bool read(const std::string & name, std::string & result);

    /**
     *
     * @param name
     * @param type
     * @param version
     * @param result
     * @return
     */
    bool read(const std::string & name, const string & type, const string & version, serializable & result);

    /**
     *
     * @param write_action user self define serialization action
     * @param name
     * @return
     */
    bool write(XMLElement * (*write_action)(), const string & name);

    /**
     *
     * @tparam T store unit type
     * @param read_action
     * @param result return the deserialization result
     * @return
     */
    template <typename T>
    bool read(bool (*read_action)(T & result), T & result);

private:

    XMLDocument * document;

    XMLElement * root;

    XMLElement * create_element(const string & name, const signed int & value);

    XMLElement * create_element(const string & name, const signed short & value);

    XMLElement * create_element(const string & name, const signed char & value);

    XMLElement * create_element(const string & name, const signed long & value);

    XMLElement * create_element(const string & name, const signed long long & value);

    XMLElement * create_element(const string & name, const unsigned int & value);

    XMLElement * create_element(const string & name, const unsigned short & value);

    XMLElement * create_element(const string & name, const unsigned char & value);

    XMLElement * create_element(const string & name, const unsigned long & value);

    XMLElement * create_element(const string & name, const unsigned long long & value);

    XMLElement * create_element(const string & name, const float & value);

    XMLElement * create_element(const string & name, const double & value);

    XMLElement * create_element(const string & name, const long double & value);

    XMLElement * create_element(const string & name, const signed int * & value);

    XMLElement * create_element(const string & name, const signed int * array, int length);

    XMLElement * create_element(const string &name, const double *array, int length);

    XMLElement * create_element(const string & name, const string & value);

    XMLElement * create_element(const string & name, const string & type, const string & version, serializable & value);

    bool recover_element(XMLElement * element, const string & name, signed int & result);

    bool recover_element(XMLElement * element, const string & name, signed short & result);

    bool recover_element(XMLElement * element, const string & name, signed char & result);

    bool recover_element(XMLElement * element, const string & name, signed long & result);

    bool recover_element(XMLElement * element, const string & name, signed long long & result);

    bool recover_element(XMLElement * element, const string & name, unsigned int & result);

    bool recover_element(XMLElement * element, const string & name, unsigned short & result);

    bool recover_element(XMLElement * element, const string & name, unsigned char & result);

    bool recover_element(XMLElement * element, const string & name, unsigned long & result);

    bool recover_element(XMLElement * element, const string & name, unsigned long long & result);

    bool recover_element(XMLElement * element, const string & name, float & result);

    bool recover_element(XMLElement * element, const string & name, double & result);

    bool recover_element(XMLElement * element, const string & name, long double & result);

    bool recover_element(XMLElement * element, const string & name, int * &result);

    bool recover_element(XMLElement * element, const string & name, int * & array, int & length);

    bool recover_element(XMLElement *element, const string &name, double *&array, int &length);

    bool recover_element(XMLElement * element, const string & name, std::string & result);

    bool recover_element(XMLElement * element, const string & name, const string & type, const string & version, serializable & result);

};


#endif //SERIALIZATION_SERIALIZABLE_H
