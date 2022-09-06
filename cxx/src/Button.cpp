#include "../include/Button.h"


const std::chrono::milliseconds Button::press_delta = std::chrono::milliseconds(50);

bool Button::get_value()
{
    bool state = line.get_value();

    if (state == last_state)
    {
        last_pressed = std::chrono::steady_clock::now();
    }
    else
    {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_pressed);
        if (elapsed_time >= press_delta)
        {
            last_state = state;
        }
    }

    return last_state;
}

bool Button::is_falling_edge()
{
    if (get_value() != edge) // edge
    {
        if (not edge) // rising edge
        {

            edge = true;
            return false;
        }
        else // falling edge
        {
            edge = false;
            return true;
        }
    }
    else // no edge
    {
        return false;
    }
}

bool Button::is_rising_edge()
{
    if (get_value() != edge)
    {
        if (not edge) // rising edge
        {
            edge = true;
            return true;
        }
        else // falling edge
        {
            edge = false;
            return false;
        }
    }
    else // no edge
    {
        return false;
    }
}

Button::Button(const gpiod::chip &chip, int line_number)
    : last_pressed(std::chrono::steady_clock::now()), edge(true), last_state(true)
{
    line = chip.get_line(line_number);
    line.request({line.name(), gpiod::line_request::DIRECTION_INPUT, 0}, 1);
}

void Button::poll(const std::function<void()> &press_callback,
                  const std::chrono::milliseconds long_press_time, const std::function<void()> &long_press_callback)
{
    // if button press detected
    if (is_rising_edge())
    {
        start_time = std::chrono::steady_clock::now();
    }
    else if (is_falling_edge())
    {
        // call callback function
        //  long press cb, when long press,
        //  otherwise short press cb
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time);
        if (elapsed_time >= long_press_time)
        {
            long_press_callback();
        }
        else
        {
            press_callback();
        }
    }
}

void Button::poll(const std::function<void()> &press_callback)
{
    poll(press_callback, std::chrono::milliseconds(0), {});
}

void Button::poll(const std::chrono::milliseconds long_press_time, const std::function<void()> &long_press_callback)
{
    poll({}, long_press_time, long_press_callback);
}
