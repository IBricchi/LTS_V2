#include <string>
#include <vector>
#include <cmath>
#include <iostream>

#include "mosfet.hpp"

using namespace std;

Mosfet::Mosfet(string name, vector<string> args, vector<float> extraInfo)
    :Component{name}
{
    // figures out if 3 or 4 inputs are used for BJT and then set's model name appropriately
    if(args.size()==3){
        modelName = "";
    }else if(args.size()==4){
        if(args[3][0] > '9'){
            modelName = args[3];
        }else{
            modelName = "";
        }
    }else if(args.size()==5){
        modelName = args[4];
    }else{
        cerr << "Wrong number of argumnets passed to BJT" << endl;
        exit(1);
    }

    // sets minimum conductance
    minPNConductance = extraInfo[3];

    // Order: C, B, E
    nodes = processNodes({args[n::D], args[n::G], args[n::S]});

    setNodalVoltages({0,0,0});

	types.push_back(componentType::nonVoltageSource);
	types.push_back(componentType::nonLinear);
}

void Mosfet::addParam(int paramId, float paramValue){
    switch(static_cast<mosfetParamType>(paramId)){ //need this as strongly typed enums don't automatically convert to their underlying type
        case mosfetParamType::TYPE:
            NMOS = false;
            break;
        case mosfetParamType::K:
            K = paramValue;
            break;
        case mosfetParamType::VA:
            hasVA=true;
            VA = paramValue;
            break;
        case mosfetParamType::VTO:
            VT = paramValue;
            break;
    }
}

void Mosfet::setNodalVoltages(vector<float> v){
    nodalVoltages = v;
    VGS = (nodalVoltages[n::G] - nodalVoltages[n::S]) * (NMOS ? 1:-1);
    VDS = (nodalVoltages[n::D] - nodalVoltages[n::S]) * (NMOS ? 1:-1);

    if(VGS-VT<0){
        GM = 1e-10;
        GO = 1e-10;
        IS = 0;
        IDEQ = 0;
    }else if(NMOS ? (VGS-VT<VDS) : (0<VDS+VGS+VT)){
        GM = sqrt(2*K*IS);
        GO = (hasVA?IS/VA:0);
        IS = K * (VGS-VT)*(VGS-VT) * (hasVA ? (1 + VDS/VA):1);
        IDEQ = (IS - GM*VGS - GO*VDS) * (NMOS?1:-1);
    }else if(NMOS ? (VDS <= VGS-VT) : (VDS+VGS+VT<=0)){
        GM = K*VDS;
        GO = K*((VGS-VT)-VDS);
        IS = K * (2*(VGS-VT)*VDS-VDS*VDS);
        IDEQ = (IS - GM*VGS - GO*VDS) * (NMOS?1:-1);
    }else{
        cerr << "mosfet in a non supported state" << endl;
        exit(1);
    }

    if(GM < minPNConductance) GM = minPNConductance;
    if(GO < minPNConductance) GO = minPNConductance;
}

double Mosfet::ivAtNode(int nin){  
    // selects the right node type (Drain Gate and Source)
    int n = nin==nodes[n::D]?n::D:(nin==nodes[n::G]?n::G:n::S);

    double current;
    switch(n){
        case n::D:
            current = IDEQ;
            lastId = current;
            break;
        case n::G:
            current = 0;
            lastIg = current;
            break;
        case n::S:
            current = -IDEQ;
            lastIs = current;
            break;
    }
    // cout << "n: " << n << " current: " << current << endl << endl;
    return current;
}

double Mosfet::divAtNode(int nin, int dnin){
    // selects the right node type (Drain Gate and Source)
    int n = nin==nodes[n::D]?n::D:(nin==nodes[n::G]?n::G:n::S);
    int dn = dnin==nodes[n::D]?n::D:(dnin==nodes[n::G]?n::G:n::S);

    double conductance;
    switch(n){
        case n::D:
            switch(dn){
                case n::D:
                    conductance = GO;
                    break;
                case n::G:
                    conductance = GM;
                    break;
                case n::S:
                    conductance = -GO-GM;
                    break;
            }
            break;
        case n::G:
            switch(dn){
                case n::D:
                    conductance = 0;
                    break;
                case n::G:
                    conductance = 0;
                    break;
                case n::S:
                    conductance = 0;
                    break;
            }
            break;
        case n::S:
            switch(dn){
                case n::D:
                    conductance = -GO;
                    break;
                case n::G:
                    conductance = -GM;
                    break;
                case n::S:
                    conductance = GO+GM;
                    break;
            }
            break;
    }
    return conductance;
}

void Mosfet::setMinPNConductance(float con){
    minPNConductance = con;
}

string Mosfet::getModelName() const{
    return modelName;
}

string Mosfet::getCurrentHeadingName() const{
    return "id_" + name + ",ig_" + name + ",is_" + name;
}

string Mosfet::getTotalCurrentString(const VectorXd &x, int highestNodeNumber, float voltage, int order) {
    // total current = current through current source, through resistor, through dependent current source
    if(NMOS){
        return to_string(lastId + VDS*GO + GM*VGS) + "," + to_string(lastIg) + "," + to_string(lastIs - VDS*GO - GM*VGS);
    }else{
        return to_string(lastId - VDS*GO - GM*VGS) + "," + to_string(lastIg) + "," + to_string(lastIs + VDS*GO + GM*VGS);
    }
}