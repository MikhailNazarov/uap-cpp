#pragma once

#include <string>

namespace uap_cpp {

struct Generic {
  Generic() : family("Other") {}
  std::string family;
};

struct Device : Generic {
  std::string model;
  std::string brand;
};

struct Agent : Generic {
  std::string major;
  std::string minor;
  std::string patch;
  std::string patch_minor;

  std::string toString() const { return family + " " + toVersionString(); }

  std::string toVersionString() const {
    return (major.empty() ? "0" : major) + "." + (minor.empty() ? "0" : minor) +
           "." + (patch.empty() ? "0" : patch);
  }
};

struct UserAgent {
  Device device;

  Agent os;
  Agent browser;

  std::string toFullString() const {
    return browser.toString() + "/" + os.toString();
  }

  bool isSpider() const { return device.family == "Spider"; }
};

enum class DeviceType { kUnknown = 0, kDesktop, kMobile, kTablet };

class UserAgentParser {
 public:
  explicit UserAgentParser(std::string_view regexes_file_path);

  UserAgent parse(std::string_view) const noexcept;

  Device parse_device(std::string_view) const noexcept;
  Agent parse_os(std::string_view) const noexcept;
  Agent parse_browser(std::string_view) const noexcept;

  static DeviceType device_type(std::string_view) noexcept;

  ~UserAgentParser();

 private:
  const std::string regexes_file_path_;
  const void* ua_store_;
};

}  // namespace uap_cpp
