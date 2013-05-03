//
// Main.cpp
// --------
// A main function that declares and invokes application classes.
//
// Copyright (c) 2013, skies457.
//

#include "Application.h"

template<class T>
void invoke() {
	T app;
	cout << app.name() << " initialized." << endl;
	app.run();
}

int main()
{
	invoke<Application>();
	return 0;
}