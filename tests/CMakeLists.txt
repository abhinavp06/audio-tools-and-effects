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

add_executable(adsr_test envelope/adsr_test.cpp)
target_link_libraries(adsr_test PRIVATE atae)
target_compile_definitions(adsr_test PRIVATE 
    OUTPUT_DIR="${ATAE_OUTPUT_DIR}")