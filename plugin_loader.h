// PluginLoader.h
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "IPlugin.h"
#include "utils/get_os_name.h"
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

class PluginLoader
{
public:
    // 加载插件
    void loadPlugin(const std::string &pluginPath)
    {
#ifdef _WIN32
        HMODULE handle = LoadLibrary(pluginPath.c_str());
#else
        void *handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
#endif
        if (!handle)
        {
            throw std::runtime_error("Failed to load plugin: " + pluginPath);
        }

        auto createPluginFunc = reinterpret_cast<IPlugin *(*)()>(
#ifdef _WIN32
            GetProcAddress(handle, "createPlugin")
#else
            dlsym(handle, "createPlugin")
#endif
        );

        if (!createPluginFunc)
        {
#ifdef _WIN32
            FreeLibrary(handle);
#else
            dlclose(handle);
#endif
            throw std::runtime_error("Failed to find createPlugin function in: " + pluginPath);
        }

        handles.push_back(handle);
        auto plugin = std::unique_ptr<IPlugin>(createPluginFunc());
        plugins.push_back(std::move(plugin));
    }

    // 卸载指定插件
    void unloadPlugin(const std::string &pluginName)
    {
        auto it = std::find_if(plugins.begin(), plugins.end(), [&](const std::unique_ptr<IPlugin> &plugin)
                               { return plugin->getPluginName() == pluginName; });

        if (it != plugins.end())
        {
            // 保存句柄的索引
            size_t index = std::distance(plugins.begin(), it);

            // 调用插件的 unload 方法
            (*it)->unload();

            // 释放插件库句柄
#ifdef _WIN32
            std::vector<HMODULE> handles;
#else
            std::vector<void *> handles;
#endif

            // 从容器中移除插件和句柄
            plugins.erase(it);
            handles.erase(handles.begin() + index);
        }
        else
        {
            throw std::runtime_error("Plugin not found: " + pluginName);
        }
    }

    // 获取所有插件
    const std::vector<std::unique_ptr<IPlugin>> &getPlugins() const
    {
        return plugins;
    }

    void loadPlugins(const std::string &pluginsDir)
    {
        std::string os_name = getOsName();
        for (const auto &entry : fs::directory_iterator(pluginsDir))
        {
            if (os_name == "Linux")
            {
                if (entry.path().extension() == ".so")
                {
                    loadPlugin(entry.path().string());
                }
            }
            else if (os_name == "Windows")
            {
                if (entry.path().extension() == ".dll")
                {
                    loadPlugin(entry.path().string());
                }
            }
            else
            {
                std::cout << "Unsupported OS: " << os_name << std::endl; // 替换fmt::print为std::cout
            }
        }
    }

    const std::unique_ptr<IPlugin> &getPlugin(const std::string &siteId) const
    {
        auto it = std::find_if(plugins.begin(), plugins.end(), [&](const std::unique_ptr<IPlugin> &plugin)
                               { return plugin->getSiteId() == siteId; });

        if (it != plugins.end())
        {
            return *it;
        }
        else
        {
            throw std::runtime_error("Plugin not found: " + siteId);
        }
    }

    // 析构时释放所有插件
    ~PluginLoader()
    {
        for (size_t i = 0; i < plugins.size(); ++i)
        {
            // 调用插件的 unload 方法
            plugins[i]->unload();
        }
    }

private:
    std::vector<void *> handles;                   // 存储插件库句柄
    std::vector<std::unique_ptr<IPlugin>> plugins; // 存储插件实例
};

#endif // PLUGINLOADER_H