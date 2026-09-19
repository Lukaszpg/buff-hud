#include "loose_layout.hpp"

#include <array>
#include <charconv>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <system_error>
#include <utility>

namespace BuffPanel::Systems::BuffHud::LooseLayout {
namespace {

[[nodiscard]] bool ValidModName(const char* name) noexcept {
    if (name == nullptr || !*name) return false;
    std::size_t length{};
    for (const unsigned char* p = reinterpret_cast<const unsigned char*>(name); *p; ++p) {
        if (++length > 128 || *p < 0x20 || *p > 0x7e) return false;
        switch (*p) {
        case '/': case '\\': case ':': case '<': case '>': case '|':
        case '?': case '*': case '"': return false;
        default: break;
        }
    }
    if ((length == 1 && name[0] == '.')
        || (length == 2 && name[0] == '.' && name[1] == '.')) return false;
    return name[length - 1] != '.' && name[length - 1] != ' ';
}

[[nodiscard]] std::filesystem::path ModMpq(const char* name) {
    std::wstring widened;
    for (const char* p = name; *p; ++p) widened.push_back(static_cast<wchar_t>(*p));
    return std::filesystem::path(widened + L".mpq");
}

[[nodiscard]] bool ReadFile(const std::filesystem::path& path, std::string& out, std::string& error) {
    std::error_code ec;
    const auto size = std::filesystem::file_size(path, ec);
    if (ec) { error = "cannot determine BuffHudhd.json size"; return false; }
    if (size == 0 || size > MaximumBytes) {
        error = "BuffHudhd.json must be between 1 byte and 2 MiB";
        return false;
    }
    std::ifstream input(path, std::ios::binary);
    if (!input) { error = "cannot open BuffHudhd.json"; return false; }
    out.resize(static_cast<std::size_t>(size));
    if (!input.read(out.data(), static_cast<std::streamsize>(out.size()))) {
        error = "cannot read complete BuffHudhd.json";
        return false;
    }
    if (input.peek() != std::char_traits<char>::eof()) {
        error = "BuffHudhd.json changed during the read";
        return false;
    }
    return true;
}

class JsonParser final {
public:
    explicit JsonParser(std::string_view input) : input_(input) {}

    [[nodiscard]] bool Parse() {
        if (input_.empty() || input_.size() > MaximumBytes) return false;
        SkipSpace();
        if (!ParseValue(0)) return false;
        SkipSpace();
        return pos_ == input_.size();
    }

    [[nodiscard]] bool HasRequiredWidgetNames() const noexcept {
        if (!rootPanel_ || !rootName_ || !buffGrid_) return false;
        for (bool seen : slots_) if (!seen) return false;
        return true;
    }

    [[nodiscard]] bool GridRectValue(GridRect& rect) const noexcept {
        if (gridRectCount_ != 1) return false;
        rect = gridRect_;
        return true;
    }

private:
    [[nodiscard]] bool ParseValue(unsigned depth) {
        if (depth > 128 || pos_ >= input_.size()) return false;
        switch (input_[pos_]) {
        case '{': return ParseObject(depth + 1);
        case '[': return ParseArray(depth + 1);
        case '"': return ParseString(nullptr);
        case 't': return Literal("true");
        case 'f': return Literal("false");
        case 'n': return Literal("null");
        default: return ParseNumber();
        }
    }

    [[nodiscard]] bool ParseString(std::string_view* text) {
        if (!Take('"')) return false;
        const auto start = pos_;
        bool escaped{};
        while (pos_ < input_.size()) {
            const unsigned char c = static_cast<unsigned char>(input_[pos_++]);
            if (c == '"') {
                if (text != nullptr) *text = escaped
                    ? std::string_view{} : input_.substr(start, pos_ - start - 1);
                return true;
            }
            if (c < 0x20) return false;
            if (c != '\\') continue;
            escaped = true;
            if (pos_ == input_.size()) return false;
            const char escape = input_[pos_++];
            if (escape == 'u') {
                for (int i = 0; i < 4; ++i) {
                    if (pos_ == input_.size()
                        || !std::isxdigit(static_cast<unsigned char>(input_[pos_++]))) return false;
                }
            } else if (escape != '"' && escape != '\\' && escape != '/'
                && escape != 'b' && escape != 'f' && escape != 'n'
                && escape != 'r' && escape != 't') {
                return false;
            }
        }
        return false;
    }

    // Parse the actual fields.rect object belonging to a named BuffGrid widget.
    // This deliberately does NOT grep for the first "rect" near the text
    // "BuffGrid": that could silently report another widget's coordinates.
    [[nodiscard]] bool ParseRect(unsigned depth, GridRect& rect, bool& complete) {
        if (depth > 128 || !Take('{')) return false;
        SkipSpace();
        std::array<bool, 4> seen{};
        bool integerValues = true;
        if (!Peek('}')) {
            do {
                SkipSpace();
                std::string_view key;
                if (!ParseString(&key)) return false;
                SkipSpace();
                if (!Take(':')) return false;
                SkipSpace();
                const auto start = pos_;
                if (!ParseValue(depth)) return false;
                const auto token = input_.substr(start, pos_ - start);
                int index = -1;
                if (key == "x") index = 0;
                else if (key == "y") index = 1;
                else if (key == "width") index = 2;
                else if (key == "height") index = 3;
                if (index >= 0) {
                    std::int32_t value{};
                    const auto parsed = std::from_chars(
                        token.data(), token.data() + token.size(), value);
                    if (parsed.ec != std::errc{}
                        || parsed.ptr != token.data() + token.size()
                        || seen[static_cast<std::size_t>(index)]) {
                        integerValues = false;
                    } else {
                        seen[static_cast<std::size_t>(index)] = true;
                        switch (index) {
                        case 0: rect.x = value; break;
                        case 1: rect.y = value; break;
                        case 2: rect.width = value; break;
                        case 3: rect.height = value; break;
                        default: break;
                        }
                    }
                }
                SkipSpace();
                if (Peek('}')) break;
                if (!Take(',')) return false;
            } while (pos_ < input_.size());
        }
        if (!Take('}')) return false;
        complete = integerValues;
        for (const bool present : seen) complete = complete && present;
        return true;
    }

    [[nodiscard]] bool ParseFields(unsigned depth, GridRect& rect, bool& hasRect) {
        if (depth > 128 || !Take('{')) return false;
        SkipSpace();
        if (Take('}')) return true;
        do {
            SkipSpace();
            std::string_view key;
            if (!ParseString(&key)) return false;
            SkipSpace();
            if (!Take(':')) return false;
            SkipSpace();
            if (key == "rect" && Peek('{')) {
                GridRect candidate{};
                bool complete{};
                if (!ParseRect(depth + 1, candidate, complete)) return false;
                if (complete) {
                    rect = candidate;
                    hasRect = true;
                }
            } else if (!ParseValue(depth)) {
                return false;
            }
            SkipSpace();
            if (Take('}')) return true;
            if (!Take(',')) return false;
            SkipSpace();
        } while (pos_ < input_.size());
        return false;
    }

    [[nodiscard]] bool ParseObject(unsigned depth) {
        if (!Take('{')) return false;
        SkipSpace();
        if (Take('}')) return true;
        std::string_view objectName{};
        GridRect objectRect{};
        bool objectHasRect{};
        do {
            SkipSpace();
            std::string_view key;
            if (!ParseString(&key)) return false;
            SkipSpace();
            if (!Take(':')) return false;
            SkipSpace();
            // Track the direct name and fields.rect members of THIS object,
            // independent of their order. Child widgets cannot overwrite it.
            if ((key == "name" || key == "type") && Peek('"')) {
                std::string_view name;
                if (!ParseString(&name)) return false;
                if (key == "type" && name == "Panel") rootPanel_ = true;
                if (key == "name") {
                    objectName = name;
                    if (name == "buff-panel/BuffHud") rootName_ = true;
                    if (name == "BuffGrid") buffGrid_ = true;
                    for (std::size_t i = 0; i < slots_.size(); ++i) {
                        if (name.size() == 10 && name.substr(0, 8) == "BuffSlot"
                            && name[8] == static_cast<char>('0' + i / 10)
                            && name[9] == static_cast<char>('0' + i % 10)) {
                            slots_[i] = true;
                            break;
                        }
                    }
                }
            } else if (key == "fields" && Peek('{')) {
                if (!ParseFields(depth + 1, objectRect, objectHasRect)) return false;
            } else if (!ParseValue(depth)) {
                return false;
            }
            SkipSpace();
            if (Take('}')) {
                if (objectName == "BuffGrid" && objectHasRect) {
                    ++gridRectCount_;
                    gridRect_ = objectRect;
                }
                return true;
            }
            if (!Take(',')) return false;
            SkipSpace();
        } while (pos_ < input_.size());
        return false;
    }

    [[nodiscard]] bool ParseArray(unsigned depth) {
        if (!Take('[')) return false;
        SkipSpace();
        if (Take(']')) return true;
        do {
            if (!ParseValue(depth)) return false;
            SkipSpace();
            if (Take(']')) return true;
            if (!Take(',')) return false;
            SkipSpace();
        } while (pos_ < input_.size());
        return false;
    }

    [[nodiscard]] bool ParseNumber() {
        const auto start = pos_;
        (void)Take('-');
        if (Take('0')) {
            if (pos_ < input_.size() && input_[pos_] >= '0' && input_[pos_] <= '9') return false;
        } else {
            if (pos_ >= input_.size() || input_[pos_] < '1' || input_[pos_] > '9') return false;
            while (pos_ < input_.size() && input_[pos_] >= '0' && input_[pos_] <= '9') ++pos_;
        }
        if (Take('.')) {
            const auto digits = pos_;
            while (pos_ < input_.size() && input_[pos_] >= '0' && input_[pos_] <= '9') ++pos_;
            if (digits == pos_) return false;
        }
        if (Take('e') || Take('E')) {
            if (!Take('+')) (void)Take('-');
            const auto digits = pos_;
            while (pos_ < input_.size() && input_[pos_] >= '0' && input_[pos_] <= '9') ++pos_;
            if (digits == pos_) return false;
        }
        return pos_ > start;
    }

    [[nodiscard]] bool Literal(std::string_view literal) {
        if (input_.substr(pos_, literal.size()) != literal) return false;
        pos_ += literal.size();
        return true;
    }

    void SkipSpace() noexcept {
        while (pos_ < input_.size() && (input_[pos_] == ' '
            || input_[pos_] == '\r' || input_[pos_] == '\n' || input_[pos_] == '\t')) ++pos_;
    }
    [[nodiscard]] bool Peek(char c) const noexcept {
        return pos_ < input_.size() && input_[pos_] == c;
    }
    [[nodiscard]] bool Take(char c) noexcept {
        if (!Peek(c)) return false;
        ++pos_;
        return true;
    }

    std::string_view input_{};
    std::size_t pos_{};
    bool rootPanel_{};
    bool rootName_{};
    bool buffGrid_{};
    std::array<bool, 21> slots_{};
    GridRect gridRect_{};
    unsigned gridRectCount_{};
};

} // namespace

bool ReadBuffGridRect(std::string_view bytes, GridRect& rect) noexcept {
    try {
        JsonParser json(bytes);
        return json.Parse() && json.GridRectValue(rect);
    } catch (...) {
        return false;
    }
}

bool Validate(std::string_view bytes, std::string& error) noexcept {
    try {
        JsonParser json(bytes);
        if (!json.Parse()) {
            error = "BuffHudhd.json is not valid JSON";
            return false;
        }
        if (!json.HasRequiredWidgetNames()) {
            error = "BuffHudhd.json must contain BuffHud panel, BuffGrid and BuffSlot00..BuffSlot20";
            return false;
        }
        return true;
    } catch (...) {
        error = "exception validating BuffHudhd.json";
        return false;
    }
}

Selection Select(
    const wchar_t* modDirectory,
    const char* activeMod,
    std::string_view embedded) noexcept {
    Selection selected;
    try {
        if (modDirectory != nullptr && *modDirectory && ValidModName(activeMod)) {
            const std::filesystem::path root(modDirectory);
            const std::filesystem::path mpq = ModMpq(activeMod);
            const std::array candidates{
                root / mpq / std::filesystem::path(VirtualPath),
                root / std::filesystem::path(VirtualPath),
                root / std::filesystem::path(activeMod) / mpq
                    / std::filesystem::path(VirtualPath),
            };
            for (const auto& path : candidates) {
                std::error_code ec;
                const bool exists = std::filesystem::exists(path, ec);
                if (ec) {
                    selected.source = Source::InvalidOverride;
                    selected.path = path;
                    selected.error = "cannot inspect active-mod BuffHudhd.json path";
                    return selected;
                }
                if (!exists) continue;
                selected.path = path;
                selected.source = Source::InvalidOverride;
                if (!std::filesystem::is_regular_file(path, ec) || ec) {
                    selected.error = "BuffHudhd.json is not a regular file";
                    return selected;
                }
                if (!ReadFile(path, selected.bytes, selected.error)) return selected;
                if (!Validate(selected.bytes, selected.error)) return selected;
                selected.source = Source::ActiveMod;
                return selected;
            }
        }
        selected.bytes.assign(embedded);
        if (!Validate(selected.bytes, selected.error)) {
            selected.source = Source::InvalidOverride;
            selected.error = "invalid built-in BuffHudhd.json: " + selected.error;
        }
    } catch (...) {
        selected.source = Source::InvalidOverride;
        selected.error = "exception selecting BuffHudhd.json";
    }
    return selected;
}

} // namespace BuffPanel::Systems::BuffHud::LooseLayout
