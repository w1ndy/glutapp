#include "ObjectLoader.h"

#include <iostream>
using namespace std;

int main(int argc, char *argv[])
{
    if(argc != 2) {
        cout << "Usage: " << argv[0] << " [obj file name]" << endl;
        cout << "Example: " << argv[0] << " path/to/project/examples/ObjectLoader/body.obj" << endl;
        return -1;
    }

    ObjectLoader loader(argv[1]);
    return loader.run();
}
