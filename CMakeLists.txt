cmake_minimum_required(VERSION 3.23)
project(atae)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(atae
	src/delay/delay_line.cpp
	src/delay/feedback_delay.cpp
	src/dynamics/compressor.cpp
	src/dynamics/limiter.cpp
	src/io/audio_file.cpp
	src/noise/white_noise.cpp
	src/noise/pink_noise.cpp
	src/oscillators/saw_wave.cpp
	src/oscillators/sine_wave.cpp
	src/oscillators/square_wave.cpp
	src/oscillators/oscillator.cpp
	src/mixer/mixer.cpp
	src/filters/low_pass/one_pole_lpf.cpp
	src/filters/biquad/biquad_filter.cpp
	src/filters/comb/comb_filter.cpp
	src/envelope/adsr.cpp
	src/envelope/envelope_follower.cpp
	
	tools/csv_writer.cpp
)

target_include_directories(atae PUBLIC include tools tools)
add_subdirectory(tests)