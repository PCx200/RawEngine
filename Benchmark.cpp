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

void Benchmark::start(int frames)
{
    target_frames = frames;
    frame_count = 0;
    fps_history.clear();
    is_recording = true;
}

void Benchmark::update(float delta_time, int cube_count)
{
    if (!is_recording) return;

    fps_history.push_back(1.0f / delta_time);
    ms_history.push_back(delta_time * 1000.0f);
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
    if (!file.is_open())
    {
        printf("Failed to open %s\n", filename.c_str());
        return;
    }

    file << "FPS;ms;Cubes\n";

    for (int i = 0; i < fps_history.size(); i++)
    {
        auto ms = ms_history.at(i);
        auto fps = fps_history.at(i);

        file << fps << ";" << ms << ";" << cube_count << "\n";
    }

    file.close();
    printf("Saved FPS history to %s\n", filename.c_str());
}
