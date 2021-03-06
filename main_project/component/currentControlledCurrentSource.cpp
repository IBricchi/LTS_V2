#include <string>
#include <iostream>

#include "currentControlledCurrentSource.hpp"

CurrentControlledCurrentSource::CurrentControlledCurrentSource(string name, vector<string> args, vector<float> extraInfo)
    :CurrentControlledSource(name, args, extraInfo)
{
    types.push_back(componentType::currentSource);
}

double CurrentControlledCurrentSource::ivAtNode(int n){
    return 0;
}
double CurrentControlledCurrentSource::divAtNode(int n, int dn){
    return 0;
}

string CurrentControlledCurrentSource::getTotalCurrentString(const VectorXd &x, int highestNodeNumber, float voltage, int order) {
    return to_string(x(highestNodeNumber+nodes[2]) * gain); //nodes[2] = idx of voltage source that the controlling current flows through
}