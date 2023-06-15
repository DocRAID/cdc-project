#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <Poco/Dynamic/Struct.h>

#include <set>
#include <iostream>

#include<vector>

using namespace std;
using namespace Poco::JSON;


void testSimple();
void testStringProperty();

int main() {
    testSimple();
    testStringProperty();

    return 0;
}

void testSimple() {
    vector<int> v;

    string lsJson;
    Parser loParser;
    lsJson = "{ \"name\" : \"Franky\", \"children\" : [ \"Jonas\", \"Ellen\" ] }";
    Poco::Dynamic::Var result = loParser.parse(lsJson);
    Object::Ptr pObject = result.extract<Object::Ptr>();
    string name = pObject->getValue<std::string>("name");
    Array::Ptr pChildren = pObject->getArray("children");

    Poco::Dynamic::Var test = pObject->get("name");
    assert (test.isString());

    printf("test json : %s\n", lsJson.c_str());
    printf("test name : %s\n", name.c_str());
}


void testStringProperty()
{
    std::string json = "{ \"test\" : \"value\" }";
    Parser parser;
    Poco::Dynamic::Var result;

    try
    {
        result = parser.parse(json);
    }
    catch(JSONException& jsone)
    {
        std::cout << jsone.message() << std::endl;
        assert (false);
    }

    assert (result.type() == typeid(Object::Ptr));

    Object object = *result.extract<Object::Ptr>();
    Poco::Dynamic::Var test = object.get("test");
    assert (test.isString());
    std::string value = test.convert<std::string>();
    assert (value.compare("value") == 0);

    object.set("test2", 'a');
    std::ostringstream ostr;
    object.stringify(ostr);
    assert (ostr.str() == "{\"test\":\"value\",\"test2\":\"a\"}");

    Poco::DynamicStruct ds = object;
    assert (!ds["test"].isEmpty());
    assert (ds["test"].isString());
    assert (!ds["test"].isInteger());
    assert (ds["test"] == "value");
    value = ds["test"].toString();
    assert (value == "value");

    const Poco::DynamicStruct& rds = object;
    assert (!rds["test"].isEmpty());
    assert (rds["test"].isString());
    assert (!rds["test"].isInteger());
    assert (rds["test"] == "value");
    value = rds["test"].toString();
    assert (value == "value");
}

