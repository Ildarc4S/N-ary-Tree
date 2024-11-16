#include "ntree.hpp"
#include "xmlreader.hpp"

int main() {
    srand(static_cast<unsigned int>(time(NULL))); // for random
    XMLReader a;
    a.loadFile("../test_files/test.xml");

    NTree<int> root(10, 0, ""); // 10 - max child count 
                                //  0 - data this node
                                // "" - node index
    root.generateChildren(a.getDepth(), a.getProbability());
    root.print();
    root.getTwoRandomAndFindCommon(); 

    return 0;
}