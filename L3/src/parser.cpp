#include <sched.h>
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
#include <assert.h>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <L3.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;

namespace L3 {

  bool printActions = false;

  /* 
   * Data required to parse
   */ 
  std::vector<Item*> parsedItems;
	std::vector<Item*> paramList;

  /* 
   * Grammar rules from now on.
   */

  struct name:
    pegtl::seq<
      pegtl::plus< 
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >
        >
      >,
      pegtl::star<
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >,
          pegtl::digit
        >
      >
    > {};

  /* 
   * keywords and operation tokens
   */

  struct str_return  : TAOCPP_PEGTL_STRING( "return"       ) {};
  struct str_define  : TAOCPP_PEGTL_STRING( "define"       ) {};
  struct str_call    : TAOCPP_PEGTL_STRING( "call"         ) {};
  struct str_print   : TAOCPP_PEGTL_STRING( "print"        ) {};
  struct str_alloc   : TAOCPP_PEGTL_STRING( "allocate"     ) {};
  struct str_inp     : TAOCPP_PEGTL_STRING( "input"        ) {};
  struct str_tensor  : TAOCPP_PEGTL_STRING( "tensor-error" ) {};
	struct str_stack   : TAOCPP_PEGTL_STRING( "stack-arg"    ) {};
	struct str_br      : TAOCPP_PEGTL_STRING( "br"           ) {};
	struct str_load    : TAOCPP_PEGTL_STRING( "load"         ) {};
	struct str_store   : TAOCPP_PEGTL_STRING( "store"        ) {};
	struct str_plus    : TAOCPP_PEGTL_STRING( "+"            ) {};
	struct str_minus   : TAOCPP_PEGTL_STRING( "-"            ) {};
	struct str_times   : TAOCPP_PEGTL_STRING( "*"            ) {};
	struct str_and     : TAOCPP_PEGTL_STRING( "&"            ) {};
	struct str_lsh     : TAOCPP_PEGTL_STRING( "<<"           ) {};
	struct str_rsh     : TAOCPP_PEGTL_STRING( ">>"           ) {};
  struct str_eq      : TAOCPP_PEGTL_STRING( "="            ) {};
  struct str_le      : TAOCPP_PEGTL_STRING( "<="           ) {};
	struct str_ge      : TAOCPP_PEGTL_STRING( ">="           ) {};
  struct str_less    : TAOCPP_PEGTL_STRING( "<"            ) {};
	struct str_greater : TAOCPP_PEGTL_STRING( ">"            ) {};
  struct str_arrow   : TAOCPP_PEGTL_STRING( "<-"           ) {};

  struct comment: 
    pegtl::disable< 
      TAOCPP_PEGTL_STRING( "//" ), 
      pegtl::until< pegtl::eolf > 
    > {};

	/*
	 * general
	 */

  struct label:
    pegtl::seq<
      pegtl::one<':'>,
      name
    > {};

  struct number:
    pegtl::seq<
      pegtl::opt<
        pegtl::sor<
          pegtl::one< '-' >,
          pegtl::one< '+' >
        >
      >,
      pegtl::plus< 
        pegtl::digit
      >
    >{};

  struct seps: 
    pegtl::star< 
      pegtl::sor< 
        pegtl::ascii::space, 
        comment 
      > 
    > {};

	/*
	 * items
	 */

	struct variable:
    pegtl::seq<
      pegtl::one<'%'>,
      name
    > {};

	struct label_operand_rule:
		label {};

	struct number_operand_rule:
		number {};

	struct variable_operand_rule:
    variable {};

	struct var_num: // t
		pegtl::sor<
			variable_operand_rule,
			number_operand_rule
		> {};

	struct var_lbl: // u
		pegtl::sor<
			variable_operand_rule,
			label_operand_rule
		> {};

	struct var_num_lbl: // s
		pegtl::sor<
			variable_operand_rule,
			number_operand_rule,
			label_operand_rule
		> {};

	struct list_variable_operand_rule:
		variable {};

	struct list_number_operand_rule:
		number {};

	struct list_var_num:
		pegtl::sor<
			list_variable_operand_rule,
			list_number_operand_rule
		> {};
	
	struct vars_list:
		pegtl::seq<
			seps,
			list_variable_operand_rule,
			seps,
			pegtl::star<
				pegtl::one<','>,
				seps,
				list_variable_operand_rule,
				seps
			>
		> {};

	struct args_list:
		pegtl::seq<
			list_var_num,
			seps,
			pegtl::star<
				pegtl::one<','>,
				seps,
				list_var_num,
				seps
			>
		> {};

	/*
	 * instructions
	 */

  struct Instruction_return_rule:
    str_return {};

	struct Instruction_return_val_rule:
		pegtl::seq<
			str_return,
			seps,
			var_num
		> {};

	struct Instruction_mov_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num_lbl
		> {};

	struct Instruction_plus_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_plus,
			seps,
			var_num
		> {};

	struct Instruction_minus_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_minus,
			seps,
			var_num
		> {};

	struct Instruction_times_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_times,
			seps,
			var_num
		> {};

	struct Instruction_and_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_and,
			seps,
			var_num
		> {};

	struct Instruction_lsh_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_lsh,
			seps,
			var_num
		> {};

	struct Instruction_rsh_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_rsh,
			seps,
			var_num
		> {};

	struct Instruction_op_rule:
		pegtl::sor<
			pegtl::seq< pegtl::at<Instruction_plus_rule >, Instruction_plus_rule  >,
			pegtl::seq< pegtl::at<Instruction_minus_rule>, Instruction_minus_rule >,
			pegtl::seq< pegtl::at<Instruction_times_rule>, Instruction_times_rule >,
			pegtl::seq< pegtl::at<Instruction_and_rule  >, Instruction_and_rule   >,
			pegtl::seq< pegtl::at<Instruction_lsh_rule  >, Instruction_lsh_rule   >,
			pegtl::seq< pegtl::at<Instruction_rsh_rule  >, Instruction_rsh_rule   >
		> {};

	struct Instruction_eq_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_eq,
			seps,
			var_num
		> {};

	struct Instruction_le_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_le,
			seps,
			var_num
		> {};

	struct Instruction_ge_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_ge,
			seps,
			var_num
		> {};

	struct Instruction_less_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_less,
			seps,
			var_num
		> {};

	struct Instruction_greater_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num,
			seps,
			str_greater,
			seps,
			var_num
		> {};

	struct Instruction_cmp_rule:
		pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_eq_rule     >, Instruction_eq_rule      >,
      pegtl::seq< pegtl::at<Instruction_le_rule     >, Instruction_le_rule      >,
      pegtl::seq< pegtl::at<Instruction_ge_rule     >, Instruction_ge_rule      >,
      pegtl::seq< pegtl::at<Instruction_less_rule   >, Instruction_less_rule    >,
      pegtl::seq< pegtl::at<Instruction_greater_rule>, Instruction_greater_rule >
		> {};

	struct Instruction_load_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			str_load,
			seps,
			variable_operand_rule
		> {};

	struct Instruction_store_rule:
		pegtl::seq<
			str_store,
			seps,
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			var_num_lbl
		> {};

	struct Instruction_label_rule:
		label_operand_rule {};

	struct Instruction_branch_rule:
		pegtl::seq<
			str_br,
			seps,
			label_operand_rule
		> {};

	struct Instruction_cond_branch_rule:
		pegtl::seq<
			str_br,
			seps,
			variable_operand_rule,
			seps,
			label_operand_rule
		> {};

	struct Instruction_call_rule:
		pegtl::seq<
			str_call,
			seps,
			var_lbl,
			seps,
			pegtl::one<'('>,
			seps,
			args_list,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_print_rule:
		pegtl::seq<
			str_call,
			seps,
			str_print,
			seps,
			pegtl::one<'('>,
			seps,
			var_num,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_allocate_rule:
		pegtl::seq<
			str_call,
			seps,
			str_alloc,
			seps,
			pegtl::one<'('>,
			seps,
			var_num,
			seps,
			pegtl::one<','>,
			seps,
			var_num,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_input_rule:
		pegtl::seq<
			str_call,
			seps,
			str_inp,
			seps,
			pegtl::one<'('>,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_tensor_error_rule:
		pegtl::seq<
			str_call,
			seps,
			str_tensor,
			seps,
			pegtl::one<'('>,
			seps,
			number_operand_rule,
			seps,
			pegtl::one<')'>
		> {};

	struct Instruction_calls_rule:
		pegtl::sor<
	 		pegtl::seq< pegtl::at<Instruction_call_rule             >, Instruction_call_rule              >,
      pegtl::seq< pegtl::at<Instruction_call_print_rule       >, Instruction_call_print_rule        >,
      pegtl::seq< pegtl::at<Instruction_call_allocate_rule    >, Instruction_call_allocate_rule     >,
      pegtl::seq< pegtl::at<Instruction_call_input_rule       >, Instruction_call_input_rule        >,
      pegtl::seq< pegtl::at<Instruction_call_tensor_error_rule>, Instruction_call_tensor_error_rule >
		> {};

	struct Instruction_call_assign_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			str_call,
			seps,
			var_lbl,
			seps,
			pegtl::one<'('>,
			seps,
			args_list,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_allocate_assign_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			str_call,
			seps,
			str_alloc,
			seps,
			pegtl::one<'('>,
			seps,
			var_num,
			seps,
			pegtl::one<','>,
			seps,
			var_num,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_input_assign_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			str_call,
			seps,
			str_inp,
			seps,
			pegtl::one<'('>,
			seps,
			pegtl::one<')'>
		> {};

	struct Instruction_calls_assign_rule:
		pegtl::sor<
	 		pegtl::seq< pegtl::at<Instruction_call_assign_rule             >, Instruction_call_assign_rule              >,
      pegtl::seq< pegtl::at<Instruction_call_allocate_assign_rule    >, Instruction_call_allocate_assign_rule     >,
      pegtl::seq< pegtl::at<Instruction_call_input_assign_rule       >, Instruction_call_input_assign_rule        >
		> {};

  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_return_val_rule  >, Instruction_return_val_rule   >,
      pegtl::seq< pegtl::at<Instruction_return_rule      >, Instruction_return_rule       >,
      pegtl::seq< pegtl::at<Instruction_mov_rule         >, Instruction_mov_rule          >,
      pegtl::seq< pegtl::at<Instruction_op_rule          >, Instruction_op_rule           >,
      pegtl::seq< pegtl::at<Instruction_cmp_rule         >, Instruction_cmp_rule          >,
      pegtl::seq< pegtl::at<Instruction_load_rule        >, Instruction_load_rule         >,
      pegtl::seq< pegtl::at<Instruction_store_rule       >, Instruction_store_rule        >,
      pegtl::seq< pegtl::at<Instruction_label_rule       >, Instruction_label_rule        >,
      pegtl::seq< pegtl::at<Instruction_branch_rule      >, Instruction_branch_rule       >,
      pegtl::seq< pegtl::at<Instruction_cond_branch_rule >, Instruction_cond_branch_rule  >,
      pegtl::seq< pegtl::at<Instruction_calls_rule       >, Instruction_calls_rule        >,
      pegtl::seq< pegtl::at<Instruction_calls_assign_rule>, Instruction_calls_assign_rule >
    > {};

  struct Instructions_rule:
    pegtl::plus<
      pegtl::seq<
        seps,
        Instruction_rule,
        seps
      >
    > {};

	/*
	 * functions
	 */

  struct function_name:
    label {};

  struct Function_rule:
    pegtl::seq<
      str_define,
			seps,
			function_name,
			seps,
      pegtl::one< '(' >,
      seps,
			pegtl::opt<
				vars_list
			>,
      seps,
      pegtl::one< ')' >,
			seps,
			pegtl::one<'{'>,
			seps,
			Instructions_rule,
			seps,
			pegtl::one<'}'>
    > {};

  struct Functions_rule:
    pegtl::plus<
      seps,
      Function_rule,
      seps
    > {};

	/*
	 * setup
	 */

  struct grammar: 
    pegtl::must< 
      Functions_rule
    > {};

  /* 
   * actions
   */

  template< typename Rule >
  struct action : pegtl::nothing< Rule > {};

  template<> struct action < function_name > {
    template< typename Input >
  	static void apply( const Input & in, Program & p){
    	if (printActions) std::cout << "function name: " << in.string() << std::endl;
      auto newF = new Function();
      newF->name = in.string();
			newF->contexts.push_back(new Context());
      p.functions.push_back(newF);
    }
  };

  template<> struct action < vars_list > {
    template< typename Input >
  	static void apply( const Input & in, Program & p){
    	if (printActions) std::cout << "vars list" << std::endl;
      auto currF = p.functions.back();
			for (auto i : paramList) {
				currF->params.push_back(dynamic_cast<Variable*>(i)->getDup(currF->vars));
			}
			paramList.clear();
    }
  };

	template<> struct action < label_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "label operand (push)" << std::endl;
			auto new_item = new Label(in.string());
			parsedItems.push_back(new_item);
			if (in.string().length() > p.longestLabel.length()) {
				p.longestLabel = in.string();
			}
    }
  };

	template<> struct action < number_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "number operand (push)" << std::endl;
			auto new_item = new Number(std::stoll(in.string()));
			parsedItems.push_back(new_item);
    }
  };

	template<> struct action < variable_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "variable operand (push)" << std::endl;
			auto currF = p.functions.back();
			auto new_item = new Variable(in.string());
			auto check = new_item->getDup(currF->vars);
			if (check) {
				parsedItems.push_back(check);
			} else {
				parsedItems.push_back(new_item);
				currF->vars.push_back(new_item);
			}
    }
  };

	template<> struct action < list_variable_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << " list variable operand" << std::endl;
			auto currF = p.functions.back();
			auto new_item = new Variable(in.string());
			auto check = new_item->getDup(currF->vars);
			if (check) {
				paramList.push_back(check);
			} else {
				paramList.push_back(new_item);
				currF->vars.push_back(new_item);
			}
    }
  };

	template<> struct action < list_number_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << " list num operand" << std::endl;
			auto new_item = new Number(std::stoll(in.string()));
			paramList.push_back(new_item);
    }
  };

	template<> struct action < Instruction_return_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "ret rule" << std::endl;
			auto currF = p.functions.back();
			auto newInstr = new Instruction_return();
			currF->instructions.push_back(newInstr);
			// tree and context handling
			auto newTree = new InstructionNode();
			newTree->instr = newInstr;
			newTree->isLeaf = false;
			currF->contexts.back()->trees.push_back(newTree);
    }
  };

	template<> struct action < Instruction_return_val_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "ret val rule" << std::endl;
			auto currF = p.functions.back();
			auto rv = parsedItems.back();
			parsedItems.pop_back();
			auto newInstr = new Instruction_return_val(rv);
			currF->instructions.push_back(newInstr);
			// tree and context handling
			auto newTree = new InstructionNode();
			auto newLeaf = new InstructionNode();
			newLeaf->head = rv; // since ret reads this we want it as a leaf (can be merged with other trees this way)
			newTree->leaves.push_back(newLeaf);
			newTree->instr = newInstr;
			newTree->isLeaf = false;
			currF->contexts.back()->trees.push_back(newTree);
    }
  };

  template<> struct action < Instruction_mov_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "mov rule" << std::endl;
			auto currF = p.functions.back();
			auto src = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_mov(src, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf = new InstructionNode();
				newLeaf->head = src;
				newTree->leaves.push_back(newLeaf);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_plus_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "plus rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_plus(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_minus_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "minus rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_minus(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_times_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "times rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_times(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_and_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "and rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_and(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_lsh_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "lsh rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_lsh(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_rsh_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "rsh rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_rsh(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_eq_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "eq rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_eq(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_le_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "le rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_le(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_ge_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "ge rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_ge(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_less_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "less rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_less(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_greater_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "greater rule" << std::endl;
			auto currF = p.functions.back();
			auto right = parsedItems.back();
			parsedItems.pop_back();
			auto left = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_greater(left, right, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = left;
				newTree->leaves.push_back(newLeaf1);
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = right;
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_load_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "load rule" << std::endl;
			auto currF = p.functions.back();
			auto src = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (src && dst) {
				auto instr = new Instruction_load(src->getDup(currF->vars), dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf = new InstructionNode();
				newLeaf->head = src->getDup(currF->vars);
				newTree->leaves.push_back(newLeaf);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_store_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "store rule" << std::endl;
			auto currF = p.functions.back();
			auto src = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_store(src, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf = new InstructionNode();
				newLeaf->head = src;
				newTree->leaves.push_back(newLeaf);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_label_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "label rule" << std::endl;
			auto currF = p.functions.back();
			auto lbl = dynamic_cast<Label*> (parsedItems.back());
			parsedItems.pop_back();
			if (lbl) {
				auto instr = new Instruction_label(lbl);
				currF->instructions.push_back(instr);
				currF->contexts.push_back(new Context());
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_branch_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "branch rule" << std::endl;
			auto currF = p.functions.back();
			auto lbl = dynamic_cast<Label*> (parsedItems.back());
			parsedItems.pop_back();
			if (lbl) {
				auto instr = new Instruction_branch(lbl);
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr; // no head necessary
				newTree->isLeaf = false;
				currF->contexts.back()->trees.push_back(newTree);
				currF->contexts.push_back(new Context());
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_cond_branch_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "cond branch rule" << std::endl;
			auto currF = p.functions.back();
			auto lbl = dynamic_cast<Label*> (parsedItems.back());
			parsedItems.pop_back();
			auto cond = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (cond && lbl) {
				auto instr = new Instruction_cond_branch(cond->getDup(currF->vars), lbl);
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				auto newLeaf = new InstructionNode();
				newLeaf->head = cond;
				newTree->leaves.push_back(newLeaf);
				currF->contexts.back()->trees.push_back(newTree);
				currF->contexts.push_back(new Context());
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_call_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call rule" << std::endl;
			auto currF = p.functions.back();
			auto dst = parsedItems.back();
			parsedItems.pop_back();
			std::vector<Item*> args;
			for (auto i : paramList) {
				args.push_back(i);
			}
			paramList.clear();
			auto instr = new Instruction_call(dst, args);
			currF->instructions.push_back(instr);
			// tree and context handling
			auto newTree = new InstructionNode();
			newTree->instr = instr;
			newTree->isLeaf = false;
			auto newLeaf = new InstructionNode();
			newLeaf->head = dst; // dst gets a leaf since it could also be merged
			newTree->leaves.push_back(newLeaf);
			for (auto i : args) {
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = i;
				newTree->leaves.push_back(newLeaf1);
			}
			currF->contexts.back()->trees.push_back(newTree);
    }
  };

  template<> struct action < Instruction_call_print_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_print rule" << std::endl;
			auto currF = p.functions.back();
			auto arg = parsedItems.back();
			parsedItems.pop_back();
			auto instr = new Instruction_call_print(arg);
			currF->instructions.push_back(instr);
			// tree and context handling
			auto newTree = new InstructionNode();
			newTree->instr = instr;
			newTree->isLeaf = false;
			auto newLeaf = new InstructionNode();
			newLeaf->head = arg;
			newTree->leaves.push_back(newLeaf);
			currF->contexts.back()->trees.push_back(newTree);
    }
  };

  template<> struct action < Instruction_call_allocate_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_allocate rule" << std::endl;
			auto currF = p.functions.back();
			auto val = parsedItems.back();
			parsedItems.pop_back();
			auto num = parsedItems.back();
			parsedItems.pop_back();
			auto instr = new Instruction_call_allocate(num, val);
			currF->instructions.push_back(instr);
			// tree and context handling
			auto newTree = new InstructionNode();
			newTree->instr = instr;
			newTree->isLeaf = false;
			auto newLeaf1 = new InstructionNode();
			newLeaf1->head = val;
			auto newLeaf2 = new InstructionNode();
			newLeaf2->head = num;
			newTree->leaves.push_back(newLeaf1);
			newTree->leaves.push_back(newLeaf2);
			currF->contexts.back()->trees.push_back(newTree);
    }
  };

  template<> struct action < Instruction_call_input_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_input rule" << std::endl;
			auto currF = p.functions.back();
			auto instr = new Instruction_call_input();
			currF->instructions.push_back(instr);
			// tree and context handling
			auto newTree = new InstructionNode();
			newTree->instr = instr;
			newTree->isLeaf = false;
			currF->contexts.back()->trees.push_back(newTree);
    }
  };

  template<> struct action < Instruction_call_tensor_error_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_tensor_error rule" << std::endl;
			auto currF = p.functions.back();
			auto args = dynamic_cast<Number*> (parsedItems.back());
			parsedItems.pop_back();
			if (args) {
				auto instr = new Instruction_call_tensor_error(args);
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = args;
				newTree->leaves.push_back(newLeaf1);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_call_assign_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call assign rule" << std::endl;
			auto currF = p.functions.back();
			auto src = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				std::vector<Item*> args;
				for (auto i : paramList) {
					args.push_back(i);
				}
				paramList.clear();
				auto instr = new Instruction_call_assign(src, args, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf = new InstructionNode();
				newLeaf->head = src;
				newTree->leaves.push_back(newLeaf);
				for (auto i : args) {
					auto newLeaf1 = new InstructionNode();
					newLeaf1->head = i;
					newTree->leaves.push_back(newLeaf1);
				}
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_call_allocate_assign_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_allocate assign rule" << std::endl;
			auto currF = p.functions.back();
			auto val = parsedItems.back();
			parsedItems.pop_back();
			auto num = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_call_allocate_assign(num, val, dst->getDup(currF->vars));
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				auto newLeaf1 = new InstructionNode();
				newLeaf1->head = val;
				auto newLeaf2 = new InstructionNode();
				newLeaf2->head = num;
				newTree->leaves.push_back(newLeaf1);
				newTree->leaves.push_back(newLeaf2);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_call_input_assign_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_input assign rule" << std::endl;
			auto currF = p.functions.back();
			auto dst = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				auto instr = new Instruction_call_input_assign(dst);
				currF->instructions.push_back(instr);
				// tree and context handling
				auto newTree = new InstructionNode();
				newTree->instr = instr;
				newTree->isLeaf = false;
				newTree->head = dst->getDup(currF->vars);
				currF->contexts.back()->trees.push_back(newTree);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  L3::Program parseFile (char *fileName){
    pegtl::analyze< grammar >(); 
    file_input< > fileInput(fileName);
    L3::Program p;
    parse< grammar, action >(fileInput, p);

    return p;
	}

}
