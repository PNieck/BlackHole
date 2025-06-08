#pragma once

#include <glm/vec2.hpp>


enum MouseButton {
    Right = 0,
    Middle = 1,
    Left = 2
};


class MouseState {
public:
    MouseState();

    void ButtonClicked(const MouseButton button)
        { buttonClicked[button] = true; }
    void ButtonReleased(const MouseButton button)
        { buttonClicked[button] = false; }

    [[nodiscard]]
    bool IsButtonClicked(const MouseButton button) const
        { return buttonClicked[button]; }

    void Moved(float x, float y);

    [[nodiscard]]
    glm::vec2 TranslationGet() const
        { return actMousePos - oldMousePos; }

    [[nodiscard]]
    glm::vec2 PositionGet() const
        { return actMousePos; }

private:
    static constexpr int BUTTON_CNT = 3;

    bool buttonClicked[BUTTON_CNT] = {};

    glm::vec2 oldMousePos;
    glm::vec2 actMousePos;
};
