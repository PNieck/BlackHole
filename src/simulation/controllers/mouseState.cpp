#include <simulation/controllers/mouseState.hpp>


MouseState::MouseState():
oldMousePos(0), actMousePos(0)
{
    for (bool& i : buttonClicked) {
        i = false;
    }
}


void MouseState::Moved(const float x, const float y)
{
    oldMousePos = actMousePos;

    actMousePos.x = x;
    actMousePos.y = y;
}
