#include "Pattern.h"

#include "MakeUnique.h"

namespace uap_cpp {

Pattern::Pattern() : groupCount_(0) {}

Pattern::Pattern(std::string_view pattern, bool case_sensitive)
    : groupCount_(0) {
  assign(pattern, case_sensitive);
}

void Pattern::assign(std::string_view pattern, bool case_sensitive) {
  // Add parentheses around expression for capture group 0
  std::string pattern_with_zero_group;
  pattern_with_zero_group.resize(pattern.size()+2);
  pattern_with_zero_group[0]='(';
  pattern_with_zero_group[pattern_with_zero_group.size()-1]=')';
  memcpy(pattern_with_zero_group.data()+1, pattern.data(),pattern.size());


  re2::RE2::Options options;
  options.set_case_sensitive(case_sensitive);

  regex_ = uap_cpp::make_unique<re2::RE2>(pattern_with_zero_group, options);

  groupCount_ = regex_->NumberOfCapturingGroups();
  if (groupCount_ > Match::MAX_MATCHES) {
    groupCount_ = Match::MAX_MATCHES;
  }
}

bool Pattern::match(std::string_view s, Match& m) const {
  if (regex_ && re2::RE2::PartialMatchN(s.data(), *regex_, m.argPtrs_, groupCount_)) {
    m.count_ = groupCount_;
    return true;
  }
  m.count_ = 0;
  return false;
}

Match::Match() {
  for (size_t i = 0; i < MAX_MATCHES; i++) {
    args_[i] = &strings_[i];
    argPtrs_[i] = &args_[i];
  }
}

size_t Match::size() const {
  return count_;
}

namespace {
std::string empty_string;
}

const std::string& Match::get(size_t index) const {
  if (index > count_) {
    return empty_string;
  }
  return strings_[index];
}

}  // namespace uap_cpp
