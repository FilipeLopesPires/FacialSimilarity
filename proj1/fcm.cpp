#include <iostream>

#include "model.h"

int main(){
    Model m;
    m.fileParser();
    auto aux = m.getOccurTable();
    std::cout << m.getModelEntropy() << std::endl;
    return 0;

}