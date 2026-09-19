#include "systems/buff_tracker/loose_buff_hud.hpp"

#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <string_view>

namespace B = BuffPanel::Systems::BuffTracker::LooseBuffHud;
namespace fs = std::filesystem;

constexpr std::string_view Default =
    "name\tstate_id\tdisplay_type\tvalue_stat\tmax_stat\tskill_id\tvalue_shift\tenabled\n"
    "fade\t159\ttimer\t0\t0\t0\t0\t1\n";

void Write(const fs::path& target, std::string_view content) {
    fs::create_directories(target.parent_path());
    std::ofstream out(target, std::ios::binary);
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
    assert(out.good());
}

int main() {
    auto base = fs::temp_directory_path() / "buff-panel-loose-test";
    fs::remove_all(base);
    fs::create_directories(base / "test-mod");
    const auto mod = base / "test-mod";
    const std::wstring root = mod.wstring();
    const auto path = mod / "test-mod.mpq" / std::string(B::VirtualPath);

    auto result = B::Select(root.c_str(), "test-mod", Default);
    assert(result.source == B::Source::Embedded && result.bytes == Default);

    const std::string custom =
        "name\tstate_id\tdisplay_type\tvalue_stat\tmax_stat\tskill_id\tvalue_shift\tenabled\r\n"
        "venom\t31\ttimer\t0\t0\t0\t0\t1\r\n";
    Write(path, custom);
    result = B::Select(root.c_str(), "test-mod", Default);
    assert(result.source == B::Source::ActiveMod);
    assert(result.bytes.find("venom\t31") != std::string::npos);
    assert(result.bytes.find('\r') == std::string::npos);
    assert(result.path == path);
    assert(result.bytes.back() == '\n');

    Write(path, "wrong\theader\n");
    result = B::Select(root.c_str(), "test-mod", Default);
    assert(result.source == B::Source::InvalidOverride);
    assert(!result.error.empty());

    Write(path, std::string(Default).substr(0, Default.size() - 1));
    result = B::Select(root.c_str(), "test-mod", Default);
    assert(result.source == B::Source::InvalidOverride);

    Write(path, std::string(Default) + "fade2\t159\ttimer\t0\t0\t0\t0\t1\n");
    result = B::Select(root.c_str(), "test-mod", Default);
    assert(result.source == B::Source::InvalidOverride);

    // Some SDK contexts point modDirectory directly at the .mpq data folder.
    fs::remove(path);
    const auto directPath = mod / std::string(B::VirtualPath);
    Write(directPath, custom);
    result = B::Select(root.c_str(), "test-mod", Default);
    assert(result.source == B::Source::ActiveMod && result.path == directPath);
    fs::remove(directPath);

    Write(path, std::string("\xef\xbb\xbf") + std::string(Default));
    result = B::Select(root.c_str(), "test-mod", Default);
    assert(result.source == B::Source::InvalidOverride);

    Write(path, Default);
    result = B::Select(root.c_str(), "../evil", Default);
    assert(result.source == B::Source::Embedded);

    fs::remove_all(base);
    std::cout << "PASS: embedded fallback, active-mod override, CRLF normalization, invalid header, "
                 "missing LF, duplicate enabled state, direct mod root, BOM rejection, unsafe mod name\n";
}
