#include <chrono>
#include <iostream>


int main(void)
{
    double max_time = 10;

    auto starting_time = std::chrono::steady_clock::now();
    while (true)
    {
        auto current_time = std::chrono::steady_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - starting_time);
        auto max_frame_time = std::chrono::microseconds((int)(max_time * 10000));
        if (elapsed_time >= max_frame_time)
        {
            break;
        }
    }

    auto current_time = std::chrono::steady_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::seconds>(current_time - starting_time);
    std::cout << "elapsed_time: " << elapsed_time << std::endl;
}
