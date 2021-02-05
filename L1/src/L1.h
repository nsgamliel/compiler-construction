#pragma once

#include <vector>
#include <string>

namespace L1 {

	// TODO: items

	// TODO: instructions

  class Function{
		public:
	    std::string name;
  	  int64_t arguments;
   		int64_t locals;
    	std::vector<Instruction *> instructions;
  };

  class Program{
    std::string entryPointLabel;
    std::vector<Function *> functions;
  };

}
