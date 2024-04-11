#include "natives.hpp"

const int ENV_VAR_VALUE_SIZE = 512;

bool EnvExists(const std::string& filename) {
    std::ifstream file(filename.c_str());
    return file.good();
}

cell Natives::GetEnv(AMX* amx, cell* params) {
    char* key = new char[ENV_VAR_VALUE_SIZE];
    int length = params[3];
    int result = 0;

    amx_GetCString(amx, params[1], key);

    std::string envFileName = EnvExists(".env.dev") ? ".env.dev" : ".env";

    std::ifstream envFile(envFileName);
    if (envFile.is_open()) {
        std::string line;
        while (std::getline(envFile, line)) {
            size_t delimiterPos = line.find('=');
            if (delimiterPos != std::string::npos) {
                std::string envKey = line.substr(0, delimiterPos);
                std::string envValue = line.substr(delimiterPos + 1);
                if (envKey == key) {
                    result = envValue.length();
                    if (result < length) {
                        amx_SetCString(amx, params[2], envValue.c_str(), length);
                    }
                    else {
                        logprintf("The buffer is too small");
                    }
                    break;
                }
            }
        }
        envFile.close();
    }
    else {
        logprintf(".env file couldn't be found on the specified path");
    }

    return result;
}