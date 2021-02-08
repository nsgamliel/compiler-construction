#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <iostream>

#include <code_generator.h>
#include <parser.h>

using namespace std;

void print_help (char *progName){
  std::cerr << "Usage: " << progName << " [-v] [-g 0|1] [-O 0|1|2] SOURCE" << std::endl;
  return ;
}

int main(
  int argc, 
  char **argv
  ){
  auto enable_code_generator = true;
  int32_t optLevel = 0;
  bool verbose;

  /* 
   * Check the compiler arguments.
   */
  if( argc < 2 ) {
    print_help(argv[0]);
    return 1;
  }
  int32_t opt;
  while ((opt = getopt(argc, argv, "vg:O:")) != -1) {
    switch (opt){
      case 'O':
        optLevel = strtoul(optarg, NULL, 0);
        break ;

      case 'g':
        enable_code_generator = (strtoul(optarg, NULL, 0) == 0) ? false : true ;
        break ;

      case 'v':
        verbose = true;
        break ;

      default:
        print_help(argv[0]);
        return 1;
    }
  }

  /*
   * Parse the input file.
   */
  if (verbose) std::cout << "==== parsing ====" << std::endl;
  auto p = L1::parse_file(argv[optind]);
  if (verbose) std::cout << "==== done parsing ====" << std::endl;

  /*
   * Generate x86_64 assembly.
   */
  if (verbose) std::cout << "==== generating x86_64 code ====" << std::endl;
  if (enable_code_generator){
    L1::x64::generate_code(p);
  }
  if (verbose) std::cout << "==== done generating x86_64 code ====" << std::endl;

  return 0;
}
