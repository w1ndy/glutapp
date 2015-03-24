#ifndef __UTILS_H__
#define __UTILS_H__

#include <iostream>

template<class T>
void invoke() {
    T app;
    std::cout << app.name() << " initialized." << std::endl;
    app.run();
}

#endif // __UTILS_H__
