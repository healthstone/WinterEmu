#include <array>
#include <memory>
#include <string>
#include <vector>

struct bignum_st;

class BigNumber {
public:
    BigNumber();

    BigNumber(BigNumber const &bn);

    BigNumber(uint32_t v) : BigNumber() { SetDword(v); }

    BigNumber(int32_t v) : BigNumber() { SetDword(v); }

    BigNumber(std::string const &v) : BigNumber() { SetHexStr(v); }

    template<size_t Size>
    BigNumber(std::array<uint8_t, Size> const &v, bool littleEndian = true) : BigNumber() {
        SetBinary(v.data(), Size, littleEndian);
    }

    ~BigNumber();

    void SetDword(int32_t);

    void SetDword(uint32_t);

    void SetQword(uint64_t);

    void SetBinary(uint8_t const *bytes, int32_t len, bool littleEndian = true);

    template<typename Container>
    auto SetBinary(Container const &c,
                   bool littleEndian = true) -> std::enable_if_t<!std::is_pointer_v<std::decay_t<Container>>> {
        SetBinary(std::data(c), std::size(c), littleEndian);
    }

    bool SetHexStr(char const *str);

    bool SetHexStr(std::string const &str) { return SetHexStr(str.c_str()); }

    void SetRand(int32_t numbits);

    BigNumber &operator=(BigNumber const &bn);

    BigNumber &operator+=(BigNumber const &bn);

    BigNumber operator+(BigNumber const &bn) const {
        BigNumber t(*this);
        return t += bn;
    }

    BigNumber &operator-=(BigNumber const &bn);

    BigNumber operator-(BigNumber const &bn) const {
        BigNumber t(*this);
        return t -= bn;
    }

    BigNumber &operator*=(BigNumber const &bn);

    BigNumber operator*(BigNumber const &bn) const {
        BigNumber t(*this);
        return t *= bn;
    }

    BigNumber &operator/=(BigNumber const &bn);

    BigNumber operator/(BigNumber const &bn) const {
        BigNumber t(*this);
        return t /= bn;
    }

    BigNumber &operator%=(BigNumber const &bn);

    BigNumber operator%(BigNumber const &bn) const {
        BigNumber t(*this);
        return t %= bn;
    }

    BigNumber &operator<<=(int n);

    BigNumber operator<<(int n) const {
        BigNumber t(*this);
        return t <<= n;
    }

    int32_t CompareTo(BigNumber const &bn) const;

    bool operator==(BigNumber const &bn) const { return (CompareTo(bn) == 0); }

    std::strong_ordering operator<=>(BigNumber const &other) const {
        int32_t cmp = CompareTo(other);
        if (cmp < 0)
            return std::strong_ordering::less;
        if (cmp > 0)
            return std::strong_ordering::greater;
        return std::strong_ordering::equal;
    }

    bool IsZero() const;

    bool IsNegative() const;

    BigNumber ModExp(BigNumber const &bn1, BigNumber const &bn2) const;

    BigNumber Exp(BigNumber const &) const;

    int32_t GetNumBytes() const;

    struct bignum_st *BN() { return _bn; }

    struct bignum_st const *BN() const { return _bn; }

    uint32_t AsDword() const;

    void GetBytes(uint8_t *buf, size_t bufsize, bool littleEndian = true) const;

    std::vector<uint8_t> ToByteVector(int32_t minSize = 0, bool littleEndian = true) const;

    template<std::size_t Size>
    std::array<uint8_t, Size> ToByteArray(bool littleEndian = true) const {
        std::array<uint8_t, Size> buf;
        GetBytes(buf.data(), Size, littleEndian);
        return buf;
    }

    std::string AsHexStr() const;

    std::string AsDecStr() const;

private:
    struct bignum_st *_bn;

};