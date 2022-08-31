#ifndef BUTTON_H
#define BUTTON_H

// System Libraries
#include <string>
#include <chrono>
#include <functional>
// Custom Libraries
#include <gpiod.hpp>

using std::string;

/**
 * A Button Class for Lines with a PULL-UP Power-on Pulls
 */
class Button
{
private:
    /// delta time, a button cannot be pressed after being pressed
    const static std::chrono::milliseconds press_delta;

    /// stores the last time before the button state changes
    std::chrono::time_point<std::chrono::steady_clock> last_pressed;

    /// the line/ pin object for controlling the line
    gpiod::line line;

    /// the last value of the rising/ falling edge
    bool edge;

    /// the last state of the button
    bool last_state;

    /**
     * checks if the button is actually pressed
     * @return true if the button is pressed
     */
    bool get_value()
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

    /**
     * checks if the line is returning a falling edge
     * @return true if the line is a falling edge
     */
    bool is_falling_edge()
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

    /**
     * checks if the line is returning a falling edge
     * @return true if the line is a falling edge
     */
    bool is_rising_edge()
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

public:
    Button() = delete;

    Button(const gpiod::chip &chip, int line_number) : last_pressed(std::chrono::steady_clock::now()), edge(true), last_state(true)
    {
        line = chip.get_line(line_number);
        line.request({line.name(), gpiod::line_request::DIRECTION_INPUT, 0}, 1);
    }

    /**
     * Polls if the Button has been pressed.
     * If the button has been pressed, check if the press is within bounce filter range.
     * Discard button press, if the press was within filter delta time, otherwise execute the callback
     * @param callback function, which gets called when the button has been legitimately pressed
     */
    void poll(const std::function<void()>& callback)
    {
        // if button press detected
        if (is_rising_edge())
        {
            // run callback function
            callback();
        }
    }
};

const std::chrono::milliseconds Button::press_delta = std::chrono::milliseconds(50);

#endif //BUTTON_H
