#ifndef DIODE_HPP
#define DIODE_HPP

#include <vector>
#include <string>

#include "component.hpp"

/*
    This diode is for testing only.
    It shoud be removed once a real diode is added.
*/
class ExampleInputDiode: public Component{
private:
    float is; //saturation current
    float n; //emission coefficient
    // float bv; //reverse bias breakdown voltage
    // float ibv; //reverse bias breakdown current
public:
    ExampleInputDiode(string _name, vector<string> args, vector<float> extraInfo);
    
    ~ExampleInputDiode() = default;

    void addParam(int paramId, float paramValue);
    vector<int> getNodes() const override;
};

#endif