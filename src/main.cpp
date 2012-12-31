#include "CityApplication.hpp"

#include <iostream>

using namespace std;

int main(int argc, char **argv) {

    try {
        CityApplication app;
        app.setTitle("Test");
        app.loop();
        return EXIT_SUCCESS;
    } catch (exception &e) {
        cerr << e.what() << endl;
    } catch (...) {
        cerr << "Something very bad happened..." << endl;
    }
    return EXIT_FAILURE;
}

