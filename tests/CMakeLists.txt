set(ATAE_OUTPUT_DIR "${CMAKE_SOURCE_DIR}/output/")

add_executable(white_noise_test noise/white_noise_test.cpp)
target_link_libraries(white_noise_test PRIVATE atae)
target_compile_definitions(white_noise_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(pink_noise_test noise/pink_noise_test.cpp)
target_link_libraries(pink_noise_test PRIVATE atae)
target_compile_definitions(pink_noise_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(saw_wave_test oscillators/saw_wave_test.cpp)
target_link_libraries(saw_wave_test PRIVATE atae)
target_compile_definitions(saw_wave_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(sine_wave_test oscillators/sine_wave_test.cpp)
target_link_libraries(sine_wave_test PRIVATE atae)
target_compile_definitions(sine_wave_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(square_wave_test oscillators/square_wave_test.cpp)
target_link_libraries(square_wave_test PRIVATE atae)
target_compile_definitions(square_wave_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(mixer_test mixer/mixer_test.cpp)
target_link_libraries(mixer_test PRIVATE atae)
target_compile_definitions(mixer_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(one_pole_lpf_test filters/low_pass/one_pole_lpf_test.cpp)
target_link_libraries(one_pole_lpf_test PRIVATE atae)
target_compile_definitions(one_pole_lpf_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(biquad_filter_test filters/biquad/biquad_filter_test.cpp)
target_link_libraries(biquad_filter_test PRIVATE atae)
target_compile_definitions(biquad_filter_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(comb_filter_test filters/comb/comb_filter_test.cpp)
target_link_libraries(comb_filter_test PRIVATE atae)
target_compile_definitions(comb_filter_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(adsr_test envelope/adsr_test.cpp)
target_link_libraries(adsr_test PRIVATE atae)
target_compile_definitions(adsr_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(delay_line_test delay/delay_line_test.cpp)
target_link_libraries(delay_line_test PRIVATE atae)
target_compile_definitions(delay_line_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")

add_executable(feedback_delay_test delay/feedback_delay_test.cpp)
target_link_libraries(feedback_delay_test PRIVATE atae)
target_compile_definitions(feedback_delay_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")