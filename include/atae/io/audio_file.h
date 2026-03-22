#pragma once
#include <vector>
#include <string>
#include "atae/types/audio_buffer.h"

class AudioFile {
public:
    static AudioBuffer load(const std::string& path);
    static bool save(const std::string& path, const AudioBuffer& buffer);

private:
    static AudioBuffer loadFlac(const std::string& fileName);
    static AudioBuffer loadWav(const std::string& fileName);
};