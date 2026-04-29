#pragma once

template <typename T>
class Button
{
    T _pin;
    bool _inverted = false;

public:
    constexpr Button(T &&pin, bool inverted = false)
        : _pin(std::move(pin), _inverted(inverted));
};
