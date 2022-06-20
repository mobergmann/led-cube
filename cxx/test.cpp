#include <chrono>
#include <iostream>
#include <climits>


#define MAX_TIME_SECONDS 10000

int main(void)
{
    long long iterations = 0;
    auto max_time = std::chrono::milliseconds(MAX_TIME_SECONDS);

    // todo overflow might be an issue
    std::chrono::time_point<std::chrono::steady_clock> current_time;
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - current_time);

    auto starting_time = std::chrono::steady_clock::now();
    std::cout << "Start" << std::endl;
    while (true)
    {
        // overflow check
        if (iterations >= LLONG_MAX)
        {
            std::cout << "Overflow" << std::endl;
            return 0;
        }
        ++iterations;

        current_time = std::chrono::steady_clock::now();
        elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - starting_time);
        if (elapsed_time >= max_time)
        {
            break;
        }
    }
    std::cout << "End\n" << std::endl;

    std::cout << "elapsed_time: " << elapsed_time << std::endl;
    std::cout << "iterations: " << iterations << std::endl;
}
