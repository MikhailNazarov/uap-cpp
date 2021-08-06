#include "../UaParser.h"
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
int main(int argc, char* argv[]) {
  if (argc != 4) {
    printf("Usage: %s <regexes.yaml> <input file> <times to repeat>\n", argv[0]);
    return -1;
  }

  std::vector<std::string> input;
  {
    std::ifstream infile(argv[2]);
    std::string line;
    while (std::getline(infile, line)) {
      input.push_back(line);
    }
  }

  uap_cpp::UserAgentParser p(argv[1]);

  int j = 0;
  auto t1 = std::chrono::system_clock::now();
  int n = atoi(argv[3]);
  for (int i = 0; i < n; i++) {
    for (const auto& user_agent_string : input) {
      p.parse(user_agent_string);
      j++;
    }
  }
  auto t2 = std::chrono::system_clock::now();
  auto mss = std::chrono::duration_cast<std::chrono::milliseconds>(t2-t1).count();
  std::cout<<1000.0f * (double)j/(double)mss;

  return 0;
}
