#pragma once

#include <cstdint>

struct ButtonBase
{
    enum class State : uint8_t
    {
        Idle,              ///< Waiting for button press
        DebouncePress,     ///< Debouncing initial press
        Pressed,           ///< Button confirmed pressed
        DebounceRelease,   ///< Debouncing button release
        WaitForDoubleClick ///< Within double-click window after first click
    };
};

template <typename T>
class Button : ButtonBase
{
    T _pin;
    bool _inverted = false;
    State _state;

public:
    // TODO: static assert T is InputPin<...>
    // NOTE: Don't touch move semantics
    constexpr Button(T &&pin, bool inverted = false)
        : _pin(std::move(pin),
               _inverted(inverted)),
          _state(State::Idle) {};

    inline void init() noexcept
    {
        _pin.init();
    }

    [[nodiscard]] bool is_pressed() const noexcept
    {
        int level = _pin.get_level();
        return _inverted ? (level != 0) : (level == 0);
    }

    // TODO: implement click, doubleClick, longPress, debounce
};
