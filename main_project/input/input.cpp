#include <string>
#include <iostream>
#include <circuit/circuit.hpp>
#include <component/resistor.hpp>
#include <component/capacitor.hpp>
#include <component/inductor.hpp>
#include <component/voltageSource.hpp>
#include <component/currentSource.hpp>
#include <component/diode.hpp>
#include <component/bjt.hpp>

#include "input.hpp"

void readSpice(Circuit& c, istream& file){
    string title;
    getline(file, title);
    c.setTitle(title);

    string lineString{};
    int maxNode = 0;

    while(getline(file, lineString)){
        stringstream line;
        string compTypeC{};
        string name{};
        string arg{};
        vector<string> args{};

        line << lineString;
        line >> name;
        compTypeC = name[0];

        while(line >> arg){
            args.push_back(arg);
        }

        // for now this script will assume knowledge of components to get largest node values
        // will only work for components with two inputs, will fix this later
        int n1 = stoi(args[0]);
        int n2 = stoi(args[1]);
        
        if(n1 > maxNode) maxNode = n1;
        if(n2 > maxNode) maxNode = n2;

        // expected inputs are in comments, anything after the -> is optional
        if(compTypeC == "R" || compTypeC == "r"){
            // + - resistance
			c.addComponent<Resistor>(name, args);
		}else if(compTypeC == "V" || compTypeC == "v"){
            // + - voltage
			c.addComponent<VoltageSource>(name, args);
		}else if(compTypeC == "I" || compTypeC == "i"){
            // + - current
			c.addComponent<CurrentSource>(name, args);
		}else if(compTypeC == "L" || compTypeC == "l"){
            // + - inductance
			c.addComponent<Inductor>(name, args);
		}else if(compTypeC =="C" || compTypeC == "c"){
            // + - capacitance
			c.addComponent<Capacitor>(name,args);
		}else if(compTypeC =="D" || compTypeC == "d"){
            // + - -> IS N VT
			c.addComponent<Diode>(name,args);
		}else if(compTypeC =="Q" || compTypeC == "q"){
            // + - -> VB IS VAF
			c.addComponent<BJT>(name,args);
		}
		
    }
    c.setHighestNodeNumber(maxNode);
}

void setupBasic(Circuit& c, float timeStep){
    c.setTimeStep(timeStep);
}
