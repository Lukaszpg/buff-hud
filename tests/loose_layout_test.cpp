#include "systems/buff_hud/loose_layout.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

namespace B = BuffPanel::Systems::BuffHud::LooseLayout;
namespace fs = std::filesystem;

void Write(const fs::path& path, std::string_view bytes) {
    fs::create_directories(path.parent_path());
    std::ofstream out(path, std::ios::binary);
    out.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    assert(out.good());
}

int main(int argc, char** argv) {
    assert(argc == 2);
    std::ifstream input(argv[1], std::ios::binary);
    assert(input.good());
    const std::string defaultLayout(std::istreambuf_iterator<char>(input), {});
    std::string error;
    assert(B::Validate(defaultLayout, error));

    const auto base = fs::temp_directory_path() / "buff-panel-layout-test";
    fs::remove_all(base);
    const auto mod = base / "test-mod";
    fs::create_directories(mod);
    const auto root = mod.wstring();
    const auto path = mod / "test-mod.mpq" / std::string(B::VirtualPath);
    auto selection = B::Select(root.c_str(), "test-mod", defaultLayout);
    assert(selection.source == B::Source::Embedded && selection.bytes == defaultLayout);

    auto custom = defaultLayout;
    const auto x = custom.find("\"x\": 50");
    assert(x != std::string::npos);
    custom.replace(x, 7, "\"x\": 250");
    Write(path, custom);
    selection = B::Select(root.c_str(), "test-mod", defaultLayout);
    assert(selection.source == B::Source::ActiveMod);
    assert(selection.bytes == custom && selection.path == path);

    Write(path, std::string("\xef\xbb\xbf") + custom);
    selection = B::Select(root.c_str(), "test-mod", defaultLayout);
    assert(selection.source == B::Source::InvalidOverride);
    assert(!selection.error.empty());

    Write(path, "{\"type\":\"Panel\",\"name\":\"buff-panel/BuffHud\",\"children\": []}");
    selection = B::Select(root.c_str(), "test-mod", defaultLayout);
    assert(selection.source == B::Source::InvalidOverride);

    Write(path, defaultLayout.substr(0, defaultLayout.find_last_of('}')));
    selection = B::Select(root.c_str(), "test-mod", defaultLayout);
    assert(selection.source == B::Source::InvalidOverride);

    Write(path, defaultLayout);
    selection = B::Select(root.c_str(), "../wrong", defaultLayout);
    assert(selection.source == B::Source::Embedded);
    fs::remove(path);
    const auto directPath = mod / std::string(B::VirtualPath);
    Write(directPath, custom);
    selection = B::Select(root.c_str(), "test-mod", defaultLayout);
    assert(selection.source == B::Source::ActiveMod && selection.path == directPath);
    fs::remove_all(base);
    std::cout << "PASS: JSON syntax/layout shape, embedded fallback, override, altered position, BOM, truncation, "
                 "missing slots, unsafe mod name, alternative loader root\n";
}
