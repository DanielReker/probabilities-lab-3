#include "Controller/Controller.h"

int main() {
    Controller controller("config.json");
    controller.execute();
    return 0;
}