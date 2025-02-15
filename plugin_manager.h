#pragma once
#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <string>
// #include <ostream>
#include <fstream>
#include <filesystem>
#include <fmt/format.h>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include "utils/get_os_name.h"

using namespace std;
using json = nlohmann::json;
namespace fs = std::filesystem;

class PluginManager
{
public:
    void installPlugin(string source)
    {
        string plugin_json_url = fmt::format("https://raw.githubusercontent.com/{}/refs/heads/main/plugin.json", source);
        cpr::Response response = cpr::Get(cpr::Url{plugin_json_url});
        json jsonResponse = json::parse(response.text);
        string plugin_file_url = jsonResponse["url"][this->os_name];
        fmt::print("Installing plugin from {}\n", plugin_file_url);
        cpr::Response plugin_response = cpr::Get(cpr::Url{plugin_file_url});
        string file_path = "plugins/" + jsonResponse["name"].get<string>() + extension;
        ofstream plugin_file(file_path, ios::binary);
        plugin_file << plugin_response.text;
        plugin_file.close();
        fmt::print("Plugin installed successfully\n");
    };

    void uninstallPlugin(string plugin_name)
    {
        string file_path = "plugins/" + plugin_name + extension;
        remove(file_path.c_str());
        fmt::print("Plugin uninstalled successfully\n");
    }

    void disablePlugin(string plugin_name)
    {
        if (!fs::exists("plugins/disabled/"))
        {
            fs::create_directory("plugins/disabled/");
        }
        string file_path = "plugins/" + plugin_name + extension;
        if (!fs::exists(file_path))
        {
            fmt::print("Plugin {} not found\n", plugin_name);
            return;
        }
        fs::rename(file_path, "plugins/disabled/" + plugin_name + extension);
        fmt::print("Plugin disabled successfully\n");
    }

    void enablePlugin(string plugin_name)
    {
        if (!fs::exists("plugins/disabled/" + plugin_name + extension))
        {
            fmt::print("Plugin {} not found in disabled plugins\n", plugin_name);
            return;
        }
        string file_path = "plugins/disabled/" + plugin_name + extension;
        fs::rename(file_path, "plugins/" + plugin_name + extension);
        fmt::print("Plugin enabled successfully\n");
    }

private:
    string os_name = getOsName();
    string extension = get_extension();
};

#endif // PLUGIN_MANAGER_H
