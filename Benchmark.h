//
// Created by ACER on 7.5.2026 г..
//

#ifndef RAWENGINE_BENCHMARK_H
#define RAWENGINE_BENCHMARK_H

#include <string>
#include <vector>

class Benchmark
{
private:
    int target_frames;
    int frame_count;
    std::vector<float> fps_history;
    std::vector<float> ms_history;
    bool is_recording;

public:
    Benchmark();

    void start(int frames);
    void update(float delta_time, int cube_count);
    void save(const std::string& filename, int cube_count) const;

    bool recording() const { return is_recording; }


};


#endif //RAWENGINE_BENCHMARK_H