#include "ilp_config.h"

// #include <cpr/cpr.h>
#include <fmt/core.h>
#include <string>
#include <filesystem>
// #include "utils/get_os_name.h"
// #include <fanqie_scraper.h>
#include "plugin_loader.h"
#include "plugin_manager.h"
#include <unordered_map>
#include <vector>

#include <CLI/CLI.hpp>

namespace fs = std::filesystem;
using namespace std;

template <>
struct fmt::formatter<unordered_map<string, string>>
{
    constexpr auto parse(format_parse_context &ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto format(const unordered_map<string, string> &map, FormatContext &ctx)
    {
        auto out = ctx.out();
        fmt::format_to(out, "{{");
        bool first = true;
        for (const auto &[key, value] : map)
        {
            if (!first)
            {
                fmt::format_to(out, ", ");
            }
            fmt::format_to(out, "\"{}\": \"{}\"", key, value);
            first = false;
        }
        fmt::format_to(out, "}}");
        return out;
    }
};

int main(int argc, char **argv)
{
    PluginLoader loader;
    string plugin_path = "plugins";
    vector<string> site_ids;
    loader.loadPlugins(plugin_path);
    const auto &plugins = loader.getPlugins();
    for (const auto &plugin : plugins)
    {
        if (plugin != nullptr)
        {
            site_ids.push_back(plugin->getSiteId());
        }
        else
        {
            // 处理空指针的情况
            fmt::println("Error: Null plugin pointer");
        }
    }

    CLI::App app;

    // get subcommand
    CLI::App *get = app.add_subcommand("get");
    CLI::App *chapter = get->add_subcommand("chapter", "download chapter");
    CLI::App *cover = get->add_subcommand("cover", "download cover");
    CLI::App *author = get->add_subcommand("author", "get author name of the novel");
    CLI::App *all = get->add_subcommand("all", "download all chapters and cover");

    string id;
    get->add_option("id", id, "ID of the novel")
        ->required();
    string site_id;
    get->add_option("-s, --site_id", site_id, "ID of the plugin")
        ->check(CLI::IsMember(site_ids))
        ->required();
    bool force_update;
    get->add_flag("-f, --force_update", force_update, "force update the novel");

    // plugin subcommand
    CLI::App *plugin = app.add_subcommand("plugin", "manage plugins");
    plugin->add_subcommand("list", "list all plugins");
    CLI::App *install = plugin->add_subcommand("install", "install a plugin");
    CLI::App *uninstall = plugin->add_subcommand("uninstall", "uninstall a plugin");
    CLI::App *enable = plugin->add_subcommand("enable", "enable a plugin");
    CLI::App *disable = plugin->add_subcommand("disable", "disable a plugin");
    plugin->require_subcommand(1, 1);

    string source;
    install->add_option("source", source, "install source of the plugin")
        ->required();

    string plugin_name;
    uninstall->add_option("name", plugin_name, "name of the plugin")
        ->required();

    enable->add_option("name", plugin_name, "name of the plugin")
        ->required();

    disable->add_option("name", plugin_name, "name of the plugin")
        ->required();

    CLI::App *version = app.add_subcommand("version", "show version");
    version->require_subcommand(0, 1);

    CLI11_PARSE(app, argc, argv);
    if (app.got_subcommand("get"))
    {
        if (find(site_ids.begin(), site_ids.end(), site_id) == site_ids.end())
        {
            fmt::print("Invalid site_id: {}\n", site_id);
            return 0;
        }
        if (get->got_subcommand("chapter"))
        {
            fmt::print("Downloading chapters of {} from {}\n", id, site_id);
            auto &plugin = loader.getPlugin(site_id);
            plugin->init(id, force_update);
            plugin->fetchAllChapter();
        }
        else if (get->got_subcommand("cover"))
        {
            fmt::print("Downloading cover of {} from {}\n", id, site_id);
            {
                auto &plugin = loader.getPlugin(site_id);
                plugin->init(id, force_update);
                plugin->getCover();
            }
        }
        else if (get->got_subcommand("author"))
        {
            fmt::print("Getting author name of {} from {}\n", id, site_id);
            {
                auto &plugin = loader.getPlugin(site_id);
                plugin->init(id, force_update);
                fmt::print("Author: {}\n", plugin->getAuthor());
            }
        }
        else if (get->got_subcommand("all"))
        {
            fmt::print("Downloading all chapters and cover of {} from {}\n", id, site_id);
            auto &plugin = loader.getPlugin(site_id);
            plugin->init(id, force_update);
            plugin->fetchAllChapter();
            plugin->getCover();
        }
    }
    if (app.got_subcommand("plugin"))
    {
        PluginManager manager;
        if (plugin->got_subcommand("list"))
        {
            fmt::print("Listing all plugins\n");
            for (const auto &plugin : plugins)
            {
                if (plugin != nullptr)
                {
                    fmt::print("Plugin Name: {}, Supported Site Name (ID): {} ({})\n", plugin->getPluginName(), plugin->getSiteName(), plugin->getSiteId());
                }
            }
        }
        else if (plugin->got_subcommand("install"))
        {
            manager.installPlugin(source);
        }
        else if (plugin->got_subcommand("uninstall"))
        {
            manager.uninstallPlugin(plugin_name);
        }
        else if (plugin->got_subcommand("enable"))
        {
            manager.enablePlugin(plugin_name);
        }
        else if (plugin->got_subcommand("disable"))
        {
            manager.disablePlugin(plugin_name);
        }
    }
    if (app.got_subcommand("version"))
    {
        fmt::println("ILP-C++ version {}", PROJECT_VERSION);
    }
    return 0;
}
