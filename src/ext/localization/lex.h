#ifndef _INCLUDE_LEX_H_
#define _INCLUDE_LEX_H_

enum class LexTokenType
{
    None,
    Value,
    // Condition, 
    Comment,
    BraceOpen,
    BraceClose,
    End

    /// TODO: Implement conditionals.
};

struct LexToken
{
    LexTokenType type;
    std::wstring value;

    LexToken(LexTokenType type = LexTokenType::None, std::wstring value = L"")
        : type(type), value(std::move(value))
    {}
};

#endif // _INCLUDE_LEX_H_
