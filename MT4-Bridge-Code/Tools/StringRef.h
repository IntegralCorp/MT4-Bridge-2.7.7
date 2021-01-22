#pragma once
#include <cstring>
#include <functional>
#include <ostream>
#include <string>

// MSVC emits references to this into the translation units which reference it.
#ifndef _MSC_VER
const size_t StringRef::npos;
#endif

namespace FIX
  {
  /// Provides efficient way of accessing text-based FIX field values.
  ///
  /// By default, OnixS::FIX::Message and OnixS::FIX::GroupInstance classes
  /// convert all values assigned to the fields into their text presentations
  /// and store textual data. This approach optimizes message reuse when same
  /// instance of message is used to update a few fields and is sent over FIX
  /// session multiple times. Also, when message comes from the network and is
  /// deserialized from its raw (tag=value) presentation, all field values in
  /// fact represent segments of text. To have access to textual values, StringRef
  /// class was added together with new access members (FieldSet::getStringRef()).
  /// If value is accessed using FieldSet::getStringRef() member, instance of
  /// StringRef class is returned. That instance holds reference to a read-only
  /// segment of text. An important aspect is that no memory is allocated and
  /// copied while accessing field values that way.
  class StringRef
    {
    public:
      /// Immutable iterator over chars.
      typedef const char *Iterator;
      typedef const char *ConstIterator;

      static const size_t npos = ~size_t(0);
      typedef size_t size_type;

    private:
      /// Items being referenced.
      const char *chars_;
      /// Number of chars being referenced.
      size_t size_;

      // Workaround PR5482: nearly all gcc 4.x miscompile StringRef and std::min()
      // Changing the arg of min to be an integer, instead of a reference to an
      // integer works around this bug.
      //static size_t min(size_t a,size_t b) { return a<b?a:b; }
      //static size_t max(size_t a,size_t b) { return a>b?a:b; }

    public:
      /// Initializes blank instance.
      StringRef()
        : chars_(NULL),
        size_(0)
        {
        }

      /// Full initialization.
      StringRef(
        const char *chars,
        size_t size)
        : chars_(chars),
        size_(size)
        {
        }

      /// Initializes as clone of other instance.
      /// Data referenced by clone is not copied.
      /// Instead both instances will refer to same
      /// text segment.
      StringRef(
        const StringRef &other)
        : chars_(other.chars_),
        size_(other.size_)
        {
        }

      /// Initializes instance from string content.
      explicit StringRef(
        const std::string &stdStr)
        : chars_(stdStr.c_str()),
        size_(stdStr.size())
        {
        }

      /// Initializes instance from zero-terminated string.
      StringRef(const char *cStr)
        : chars_(cStr),
        size_(cStr?strlen(cStr):0)
        {
        }

      /// Indicates whether array of zero length.
      bool empty() const
        {
        return (0==size_);
        }

      /// Read-only content.
      const char *data() const
        {
        return chars_;
        }

      /// STL-like begin().
      ConstIterator begin() const
        {
        return chars_;
        }

      /// STL-like end().
      ConstIterator end() const
        {
        return chars_+size_;
        }

      /// Number of chars.
      size_t size() const
        {
        return size_;
        }

      /// Resets reference to nothing.
      void reset()
        {
        reset(NULL,0);
        }

      /// Updates data being referenced.
      void
        reset(
          const char *chars,
          size_t size)
        {
        chars_=chars;
        size_=size;
        }

      const char &
        operator [](size_t index) const
        {
        return chars_[index];
        }

      const char &
        at(size_t index) const
        {
        if(index<size_)
          return chars_[index];

        throw std::invalid_argument("index");
        }

      // Appends copy of text to the std::string.
      void
        toString(std::string &str) const
        {
        str.append(chars_,size_);
        }

      // Return copy of text as std::string.
      std::string
        toString() const
        {
        return std::string(chars_,size_);
        }

      /// Compares with another instance.
      bool
        operator == (
          const StringRef &other) const
        {
        return (
          size_==other.size_
          &&
          0==memcmp(
          chars_,other.chars_,size_));
        }

      /// Compares with another instance.
      bool
        operator != (
          const StringRef &other) const
        {
        return (
          size_!=other.size_
          ||
          0!=memcmp(
          chars_,other.chars_,size_));
        }

      /// Reinitializes from another instance.
      StringRef &
        operator = (
          const StringRef &other)
        {
        chars_=other.chars_;
        size_=other.size_;

        return *this;
        }

      /// Swaps content with other instance.
      void
        swap(StringRef &other)
        {
        std::swap(chars_,other.chars_);
        std::swap(size_,other.size_);
        }

      /// find - Search for the first character \arg C in the string.
      ///
      /// \return - The index of the first occurrence of \arg C, or npos if not
      /// found.
      size_t find(char C,size_t From = 0) const
        {
        for(size_t i = min(From,size_),e = size_; i!=e; ++i)
          if(chars_[i]==C)
            return i;
        return npos;
        }

      /// find - Search for the first string \arg Str in the string.
      ///
      /// \return - The index of the first occurence of \arg Str, or npos if not
      /// found.
      size_t StringRef::find(StringRef Str,size_t From) const
        {
        size_t N = Str.size();
        if(N>size_)
          return npos;
        for(size_t e = size_-N+1,i = min(From,e); i!=e; ++i)
          if(substr(i,N).equals(Str))
            return i;
        return npos;
        }

      /// equals - Check for string equality, this is more efficient than
       /// compare() when the relative ordering of inequal strings isn't needed.
      bool equals(StringRef RHS) const
        {
        return (size_==RHS.size_&&
                memcmp(chars_,RHS.chars_,RHS.size_)==0);
        }

      /// compare - Compare two strings; the result is -1, 0, or 1 if this string
      /// is lexicographically less than, equal to, or greater than the \arg RHS.
      int compare(StringRef RHS) const
        {
        // Check the prefix for a mismatch.
        if(int Res = memcmp(chars_,RHS.chars_,min(size_,RHS.size_)))
          return Res<0?-1:1;

        // Otherwise the prefixes match, so we only need to check the lengths.
        if(size_==RHS.size_)
          return 0;
        return size_<RHS.size_?-1:1;
        }

      /// @}
      /// @name Substring Operations
      /// @{

      /// substr - Return a reference to the substring from [Start, Start + N).
      ///
      /// \param Start - The index of the starting character in the substring; if
      /// the index is npos or greater than the length of the string then the
      /// empty substring will be returned.
      ///
      /// \param N - The number of characters to included in the substring. If N
      /// exceeds the number of characters remaining in the string, the string
      /// suffix (starting with \arg Start) will be returned.
      StringRef substr(size_t Start,size_t N = npos) const
        {
        Start = min(Start,size_);
        return StringRef(chars_+Start,min(N,size_-Start));
        }

      friend std::ostream &operator<< (std::ostream &out,const StringRef &ref);
    };

  inline
    bool
    operator == (
      const StringRef &ref,
      const std::string &str)
    {
    return ref==StringRef(str);
    }

  inline
    bool
    operator != (
      const StringRef &ref,
      const std::string &str)
    {
    return ref!=StringRef(str);
    }

  inline
    bool
    operator == (
      const std::string &str,
      const StringRef &ref)
    {
    return ref==StringRef(str);
    }

  inline
    bool
    operator != (
      const std::string &str,
      const StringRef &ref)
    {
    return ref!=StringRef(str);
    }

  //

  inline
    bool
    operator == (
      const StringRef &ref,
      const char *str)
    {
    return ref==StringRef(str);
    }

  inline
    bool
    operator != (
      const StringRef &ref,
      const char *str)
    {
    return ref!=StringRef(str);
    }

  inline
    bool
    operator == (
      const char *str,
      const StringRef &ref)
    {
    return ref==StringRef(str);
    }

  inline
    bool
    operator != (
      const char *str,
      const StringRef &ref)
    {
    return ref!=StringRef(str);
    }

  inline std::ostream &operator<< (std::ostream &out,const StringRef &ref)
    {
    if(!ref.empty())
      out.write(ref.chars_,ref.size_);

    return out;
    }
  }

namespace std
  {
  /// Allows using of StringRef in collections like std::map.
  template<>
  struct less<FIX::StringRef>
    {
    bool
      operator()(
        const FIX::StringRef &left,
        const FIX::StringRef &right) const
      {
      ptrdiff_t
        compareResult=memcmp(
          left.data(),right.data(),
          left.size()<right.size()
          ?left.size():right.size());

      if(0==compareResult)
        {
        compareResult=
          static_cast<ptrdiff_t>(
            left.size()-right.size());
        }

      return (0>compareResult);
      }
    };
  }