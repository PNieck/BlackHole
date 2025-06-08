#include <simulation/window.hpp>

int main() {
    Window window(600, 400, "Black Hole");

    window.RunMessageLoop();

    return 0;
}
