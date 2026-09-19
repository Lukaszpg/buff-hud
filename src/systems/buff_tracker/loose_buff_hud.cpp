#include "loose_buff_hud.hpp"

#include <array>
#include <charconv>
#include <cstdint>
#include <fstream>
#include <limits>
#include <system_error>
#include <unordered_set>
#include <utility>
#include <vector>

namespace BuffPanel::Systems::BuffTracker::LooseBuffHud {
namespace {

inline constexpr std::string_view Header =
    "name\tstate_id\tdisplay_type\tvalue_stat\tmax_stat\tskill_id\tvalue_shift\tenabled";

// A single-file byte count prevents oversized or accidentally binary files
// from being registered in D2RLoader's custom TXT compiler.
[[nodiscard]] bool ReadFile(
    const std::filesystem::path& file,
    std::string& bytes,
    std::string& error) {
    std::error_code ec;
    const auto size = std::filesystem::file_size(file, ec);
    if (ec) {
        error = "cannot get external buff-hud.txt size";
        return false;
    }
    if (size > MaximumBytes) {
        error = "external buff-hud.txt exceeds 1 MiB";
        return false;
    }
    std::ifstream stream(file, std::ios::binary);
    if (!stream) {
        error = "cannot open external buff-hud.txt";
        return false;
    }
    bytes.resize(static_cast<std::size_t>(size));
    if (!bytes.empty() && !stream.read(bytes.data(), static_cast<std::streamsize>(bytes.size()))) {
        error = "cannot read complete external buff-hud.txt";
        return false;
    }
    // A file being modified between file_size() and read() is rejected rather
    // than unintentionally registering a truncated table.
    if (stream.peek() != std::char_traits<char>::eof()) {
        error = "external buff-hud.txt changed while being read";
        return false;
    }
    return true;
}

[[nodiscard]] bool ReadUnsigned(std::string_view value, std::uint32_t& output) noexcept {
    if (value.empty()) {
        output = 0; // D2RLoader's custom table format accepts empty numeric cells.
        return true;
    }
    const char* const begin = value.data();
    const char* const end = begin + value.size();
    std::uint32_t parsed{};
    const auto result = std::from_chars(begin, end, parsed);
    if (result.ec != std::errc{} || result.ptr != end) return false;
    output = parsed;
    return true;
}

[[nodiscard]] bool ValidModName(const char* value) noexcept {
    if (value == nullptr || value[0] == '\0') return false;
    std::size_t count{};
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(value); *p; ++p) {
        if (++count > 128 || *p < 0x20 || *p > 0x7e) return false;
        switch (*p) {
        case '/': case '\\': case ':': case '<': case '>': case '|':
        case '?': case '*': case '"':
            return false;
        default: break;
        }
    }
    if ((count == 1 && value[0] == '.')
        || (count == 2 && value[0] == '.' && value[1] == '.')) return false;
    return value[count - 1] != '.' && value[count - 1] != ' ';
}

[[nodiscard]] std::filesystem::path ModFolder(const char* activeMod) {
    // Verified ASCII in ValidModName, so widening bytes is lossless.
    std::wstring name;
    for (const char* it = activeMod; *it; ++it) name.push_back(static_cast<wchar_t>(*it));
    return std::filesystem::path(name + L".mpq");
}

} // namespace

bool ValidateAndNormalize(std::string& bytes, std::string& error) noexcept {
    try {
        // D2RLoader's CustomTableService requires strict ASCII TSV with an LF
        // after the final row. Accept Windows CRLF authoring by normalization.
        if (bytes.empty() || bytes.size() > MaximumBytes) {
            error = "table empty or exceeds 1 MiB";
            return false;
        }
        std::string normalized;
        normalized.reserve(bytes.size());
        for (std::size_t index = 0; index < bytes.size(); ++index) {
            const unsigned char character = static_cast<unsigned char>(bytes[index]);
            if (character == '\r') {
                if (index + 1 >= bytes.size() || bytes[index + 1] != '\n') {
                    error = "bare CR detected; use LF or CRLF line endings";
                    return false;
                }
                continue;
            }
            if (character != '\n' && character != '\t'
                && (character < 0x20 || character > 0x7e)) {
                error = "non-printable/non-ASCII byte detected; save as plain ASCII without BOM";
                return false;
            }
            normalized.push_back(static_cast<char>(character));
        }
        if (normalized.empty() || normalized.back() != '\n') {
            error = "table requires final line feed";
            return false;
        }
        const auto firstLineEnd = normalized.find('\n');
        if (firstLineEnd == std::string::npos
            || std::string_view(normalized).substr(0, firstLineEnd) != Header) {
            error = "header must exactly match the eight buff-hud.txt columns";
            return false;
        }

        std::unordered_set<std::uint32_t> enabledStates;
        std::size_t lineStart = firstLineEnd + 1;
        std::size_t rowNumber = 1;
        while (lineStart < normalized.size()) {
            const auto lineEnd = normalized.find('\n', lineStart);
            if (lineEnd == std::string::npos) {
                error = "missing final line feed";
                return false;
            }
            ++rowNumber;
            const std::string_view line(normalized.data() + lineStart, lineEnd - lineStart);
            lineStart = lineEnd + 1;
            if (line.empty()) continue;

            std::array<std::string_view, 8> cells{};
            std::size_t cellStart{};
            for (std::size_t col = 0; col < 7; ++col) {
                const auto separator = line.find('\t', cellStart);
                if (separator == std::string_view::npos) {
                    error = "row " + std::to_string(rowNumber) + ": fewer than eight columns";
                    return false;
                }
                cells[col] = line.substr(cellStart, separator - cellStart);
                cellStart = separator + 1;
            }
            cells[7] = line.substr(cellStart);
            if (cells[7].find('\t') != std::string_view::npos) {
                error = "row " + std::to_string(rowNumber) + ": more than eight columns";
                return false;
            }
            if (cells[0].empty() || cells[0].size() >= 48
                || cells[2].empty() || cells[2].size() >= 16) {
                error = "row " + std::to_string(rowNumber) + ": invalid name or display_type length";
                return false;
            }
            std::array<std::uint32_t, 6> numbers{};
            constexpr std::array<std::size_t, 6> NumericColumns{1, 3, 4, 5, 6, 7};
            for (std::size_t i = 0; i < numbers.size(); ++i) {
                if (!ReadUnsigned(cells[NumericColumns[i]], numbers[i])) {
                    error = "row " + std::to_string(rowNumber) + ": invalid unsigned decimal field";
                    return false;
                }
            }
            const auto stateId = numbers[0];
            const auto valueStat = numbers[1];
            const auto maxStat = numbers[2];
            const auto skillId = numbers[3];
            const auto shift = numbers[4];
            const auto enabled = numbers[5];
            if (stateId == 0 || stateId > 4095 || shift > 255 || enabled > 1) {
                error = "row " + std::to_string(rowNumber) + ": state/shift/enabled out of range";
                return false;
            }
            if (cells[2] != "timer" && cells[2] != "resource") {
                error = "row " + std::to_string(rowNumber) + ": display_type must be timer or resource";
                return false;
            }
            if (enabled != 0) {
                if (cells[2] == "timer" && (valueStat || maxStat || skillId || shift)) {
                    error = "row " + std::to_string(rowNumber) + ": timer columns must be zero";
                    return false;
                }
                if (cells[2] == "resource"
                    && (!valueStat || !maxStat || !skillId
                        || valueStat > 4095 || maxStat > 4095 || skillId > 4095
                        || shift > 30)) {
                    error = "row " + std::to_string(rowNumber) + ": invalid resource stat/skill/shift";
                    return false;
                }
                if (!enabledStates.insert(stateId).second) {
                    error = "row " + std::to_string(rowNumber) + ": duplicate enabled state_id";
                    return false;
                }
            }
        }
        bytes.swap(normalized);
        return true;
    } catch (...) {
        error = "exception while validating buff-hud.txt";
        return false;
    }
}

Selection Select(
    const wchar_t* modDirectory,
    const char* activeMod,
    std::string_view embedded) noexcept {
    Selection selected;
    try {
        if (modDirectory != nullptr && modDirectory[0] != L'\0'
            && ValidModName(activeMod)) {
            const std::filesystem::path modRoot(modDirectory);
            const std::filesystem::path modMpq = ModFolder(activeMod);
            // Normally modDirectory is <game>/mods/<mod>; also accept SDK
            // contexts that already point to <mod>.mpq or to <game>/mods.
            const std::array candidates{
                modRoot / modMpq / std::filesystem::path(VirtualPath),
                modRoot / std::filesystem::path(VirtualPath),
                modRoot / std::filesystem::path(activeMod) / modMpq
                    / std::filesystem::path(VirtualPath),
            };
            for (const auto& path : candidates) {
                std::error_code ec;
                const bool exists = std::filesystem::exists(path, ec);
                if (ec) {
                    selected.source = Source::InvalidOverride;
                    selected.path = path;
                    selected.error = "failed to inspect external buff-hud.txt path";
                    return selected;
                }
                if (!exists) continue;
                selected.path = path;
                selected.source = Source::InvalidOverride;
                if (!std::filesystem::is_regular_file(path, ec) || ec) {
                    selected.error = "external buff-hud.txt exists but is not a regular file";
                    return selected;
                }
                if (!ReadFile(path, selected.bytes, selected.error)) return selected;
                if (!ValidateAndNormalize(selected.bytes, selected.error)) return selected;
                selected.source = Source::ActiveMod;
                return selected;
            }
        }
        selected.bytes.assign(embedded);
        if (!ValidateAndNormalize(selected.bytes, selected.error)) {
            selected.source = Source::InvalidOverride;
            selected.error = "invalid embedded default: " + selected.error;
        }
    } catch (...) {
        selected.source = Source::InvalidOverride;
        selected.error = "exception resolving external buff-hud.txt";
    }
    return selected;
}

} // namespace BuffPanel::Systems::BuffTracker::LooseBuffHud
