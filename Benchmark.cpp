//
// Created by ACER on 7.5.2026 г..
//

#include "Benchmark.h"

#include <algorithm>
#include <fstream>
#include <GLFW/glfw3.h>

Benchmark::Benchmark()
{
    is_recording = false;
}

void Benchmark::start(int frames, const std::string& mode)
{
    target_frames = frames;
    frame_count = 0;
    fps_history.clear();
    ms_history.clear();
    collision_checks_history.clear();
    is_recording = true;

    this->mode = mode;

}

void Benchmark::update(float delta_time, int cube_count, int collision_checks)
{
    if (!is_recording) return;

    fps_history.push_back(1.0f / delta_time);
    ms_history.push_back(delta_time * 1000.0f);
    collision_checks_history.push_back(collision_checks);
    frame_count++;


    if (frame_count >= target_frames)
    {
        is_recording = false;
        double currentTime = glfwGetTime();
        save("benchmarks/fps_output_" + std::to_string(currentTime) + ".csv", cube_count);
    }
}

void Benchmark::save(const std::string& filename, int cube_count) const
{
    std::ofstream file(filename);
    file.imbue(std::locale("nl_NL.utf8"));
    if (!file.is_open())
    {
        printf("Failed to open %s\n", filename.c_str());
        return;
    }

    file << "# Mode = " << mode << ", Cubes: " << cube_count << "\n";
    file << "FPS;ms;Collision Checks\n";

    for (int i = 0; i < fps_history.size(); i++)
    {
        file << fps_history[i] << ";"
         << ms_history[i] << ";"
         << collision_checks_history[i] << "\n";
    }

    file.close();
    printf("Saved FPS history to %s\n", filename.c_str());
}
