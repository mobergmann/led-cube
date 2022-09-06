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


    std::chrono::time_point<std::chrono::steady_clock> start_time;

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
    bool get_value();

    /**
     * checks if the line is returning a falling edge
     * @return true if the line is a falling edge
     */
    bool is_falling_edge();

    /**
     * checks if the line is returning a falling edge
     * @return true if the line is a falling edge
     */
    bool is_rising_edge();

public:
    Button() = delete;

    Button(const gpiod::chip &chip, int line_number);

    /**
     *
     * @param press_callback
     * @param long_press_time
     * @param long_press_callback
     */
    void poll(const std::function<void()> &press_callback,
              const std::chrono::milliseconds long_press_time, const std::function<void()> &long_press_callback);

    /**
      * Polls if the Button has been pressed.
      * If the button has been pressed, check if the press is within bounce filter range.
      * Discard button press, if the press was within filter delta time, otherwise execute the callback
      * @param press_callback gets called when the button has been legitimately pressed
      * @param long_press_callback gets called when the button has been legitimately pressed for at least n milliseconds
      */
    void poll(const std::function<void()> &press_callback);

    /**
     *
     * @param long_press_time
     * @param long_press_callback
     */
    void poll(const std::chrono::milliseconds long_press_time, const std::function<void()> &long_press_callback);
};


#endif //BUTTON_H
