#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include <L2.h>
#include <parser.h>
#include <liveness.h>
#include <interference.h>
#include <spill.h>
#include <register_allocation.h>
#include <stack_management.h>

using namespace std;

void print_help (char *progName){
  std::cerr << "Usage: " << progName << " [-v] [-g 0|1] [-O 0|1|2] [-s] [-l] [-i] SOURCE" << std::endl;
  return ;
}

int main(
  int argc, 
  char **argv
  ){
  auto enable_code_generator = true;
  auto spill_only = false;
  auto interference_only = false;
  auto liveness_only = false;
  auto verbose = false;
  int32_t optLevel = 3;

  /* 
   * Check the compiler arguments.
   */
  //Utils::verbose = false;
  if( argc < 2 ) {
    print_help(argv[0]);
    return 1;
  }
  int32_t opt;
  while ((opt = getopt(argc, argv, "vg:O:sli")) != -1) {
    switch (opt){

      case 'l':
        liveness_only = true;
        break ;

      case 'i':
        interference_only = true;
        break ;

      case 's':
        spill_only = true;
        break ;

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
  L2::Program p;
  if (spill_only){

    /* 
     * Parse an L2 function and the spill arguments.
     */
    if (verbose) std::cout << "parsing for spill only" <<std::endl;
    p = L2::parse_spill_file(argv[optind]);
    if (verbose) std::cout << "done parsing" <<std::endl;
 
  } else if (liveness_only){

    /*
     * Parse an L2 function.
     */
    if (verbose) std::cout << "parsing for liveness only" <<std::endl;
    p = L2::parse_function_file(argv[optind]);
    if (verbose) std::cout << "done parsing" <<std::endl;

  } else if (interference_only){

    /*
     * Parse an L2 function.
     */
    if (verbose) std::cout << "parsing for interference" <<std::endl;
    p = L2::parse_function_file(argv[optind]);
    if (verbose) std::cout << "done parsing" <<std::endl;
  } else {

    /* 
     * Parse the L2 program.
     */
    p = L2::parse_file(argv[optind]);
  }



  /*
   * Special cases.
   */
  if (spill_only){
    if (verbose) std::cout << "generating liveness" <<std::endl;
    p.functions[0]->generate_liveness();
    if (verbose) std::cout << "performing interference analysis" <<std::endl;
		auto ig = new L2::InterferenceGraph(p.functions[0]);
    if (verbose) std::cout << "processing spill" <<std::endl;
		auto sp = new L2::Spiller();
    sp->spill(p.functions[0], p.spill_var, p.spill_prefix);
    if (verbose) std::cout << "generating output" <<std::endl;
    L2::generate_spill_output(p.functions[0]);
    if (verbose) std::cout << "done" <<std::endl;
    return 0;
  }

  /*
   * Liveness test.
   */
  if (liveness_only){
    if (verbose) std::cout << "generating liveness" <<std::endl;
    p.functions[0]->generate_liveness();
    if (verbose) std::cout << "generating output for " << argv[optind] <<std::endl;
    L2::generate_liveness_output(p.functions[0]);
    if (verbose) std::cout << "done" <<std::endl;

    return 0;
  }

  /*
   * Interference graph test.
   */
  if (interference_only){
    if (verbose) std::cout << "generating liveness" <<std::endl;
    p.functions[0]->generate_liveness();
    if (verbose) std::cout << "performing interference analysis" <<std::endl;
		auto ig = new L2::InterferenceGraph(p.functions[0]);
    if (verbose) std::cout << "generating output for " << argv[optind] <<std::endl;
    L2::generate_interference_output(ig);
    if (verbose) std::cout << "done" <<std::endl;

    return 0;
  }

	// register allocation and stack-arg translation
	if (verbose) std::cout << "beginning reg alloc and stack arg translation" << std::endl;
	for (auto f : p.functions) {
		if (verbose) std::cout << "allocating" << std::endl;
		L2::RegisterAllocator* ra = new L2::RegisterAllocator(f);
		if (verbose) std::cout << "allocator setup complete" << std::endl;
		f = ra->allocate_registers();
		if (verbose) std::cout << "translating" << std::endl;
		L2::convert_stack_args(f);
	}

  /*
   * Generate the target code.
   */
	if (verbose) std::cout << "beginning code gen" << std::endl;
  if (enable_code_generator){
    std::ofstream output_file;
		output_file.open("prog.L1");
		if (verbose) std::cout << "file open" << std::endl;

		// file setup
		output_file << "(" << p.entryPointLabel << "\n";

		// function output
		for (auto f : p.functions) {
			output_file << "\t(" << f->name << "\n";
			output_file << "\t\t" << std::to_string(f->arguments) << " " << std::to_string(f->locals) << "\n";
			for (auto i : f->instructions) {
				output_file << "\t\t" << i->toString() << "\n";
			}
			output_file << "\t)\n";
		}

		// end of file
		output_file << ")\n";
		output_file.close();
		if (verbose) std::cout << "end code gen" << std::endl;
  }

  return 0;
}
 