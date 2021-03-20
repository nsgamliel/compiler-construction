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

#include <LA.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;

namespace LA {

  bool printActions = false;

  /* 
   * data obtained while parsing
   */ 
  std::vector<Item*> parsedItems;
	std::vector<Item*> paramList;
	std::vector<Variable*> declaredVars;
	std::string dType;
	std::vector<Variable*> tups;
	std::vector<Variable*> codeVars;
	std::vector<Variable*> ints;
	std::map<Variable*, Label*> codePtrs;

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

	struct str_int     : TAOCPP_PEGTL_STRING( "int64"        ) {};
	struct str_tuple   : TAOCPP_PEGTL_STRING( "tuple"        ) {};
	struct str_code    : TAOCPP_PEGTL_STRING( "code"         ) {};
  struct str_new     : TAOCPP_PEGTL_STRING( "new"          ) {};
	struct str_arr     : TAOCPP_PEGTL_STRING( "Array"        ) {};
	struct str_tup     : TAOCPP_PEGTL_STRING( "Tuple"        ) {};
	struct str_len     : TAOCPP_PEGTL_STRING( "length"       ) {};
	struct str_void    : TAOCPP_PEGTL_STRING( "void"         ) {};
	struct str_return  : TAOCPP_PEGTL_STRING( "return"       ) {};
  struct str_print   : TAOCPP_PEGTL_STRING( "print"        ) {};
  struct str_inp     : TAOCPP_PEGTL_STRING( "input"        ) {};
  struct str_tensor  : TAOCPP_PEGTL_STRING( "tensor-error" ) {};
	struct str_br      : TAOCPP_PEGTL_STRING( "br"           ) {};
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

	struct int_array:
		pegtl::seq<
			str_int,
			pegtl::plus<
				pegtl::one<'['>,
				pegtl::one<']'>
			>
		> {};
	
	struct data_type:
		pegtl::sor<
			pegtl::seq< pegtl::at<int_array>, int_array >,
			pegtl::seq< pegtl::at<str_int  >, str_int   >,
			pegtl::seq< pegtl::at<str_tuple>, str_tuple >,
			pegtl::seq< pegtl::at<str_code >, str_code  >,
			pegtl::seq< pegtl::at<str_void >, str_void  >
		> {};

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
    name {};

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

	struct func_list_variable_operand_rule:
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
			data_type,
			seps,
			func_list_variable_operand_rule,
			seps,
			pegtl::star<
				pegtl::one<','>,
				seps,
				data_type,
				seps,
				func_list_variable_operand_rule,
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

	struct Instruction_define_var_rule:
		pegtl::seq<
			data_type,
			seps,
			variable_operand_rule
		> {};

  struct Instruction_return_rule:
    str_return {};

	struct Instruction_return_val_rule:
		pegtl::seq<
			str_return,
			pegtl::one<' '>,
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

	struct Instruction_from_array_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			variable_operand_rule,
			seps,
			pegtl::plus<
				pegtl::one<'['>,
				seps,
				list_var_num,
				seps,
				pegtl::one<']'>,
				seps
			>
		> {};

	struct Instruction_to_array_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			pegtl::plus<
				pegtl::one<'['>,
				seps,
				list_var_num,
				seps,
				pegtl::one<']'>,
				seps
			>,
			seps,
			str_arrow,
			seps,
			var_num_lbl
		> {};

	struct Instruction_length_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			str_len,
			seps,
			variable_operand_rule,
			seps,
			var_num
		> {};

	struct Instruction_label_rule:
		pegtl::seq<
			label_operand_rule
		> {};

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
			var_num,
			seps,
			label_operand_rule,
			seps,
			label_operand_rule
		> {};

	struct Instruction_call_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			pegtl::one<'('>,
			seps,
			pegtl::opt<
				args_list
			>,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_print_rule:
		pegtl::seq<
			str_print,
			seps,
			pegtl::one<'('>,
			seps,
			var_num,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_input_rule:
		pegtl::seq<
			str_inp,
			seps,
			pegtl::one<'('>,
			seps,
			pegtl::one<')'>
		> {};

	struct Instruction_calls_rule:
		pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_call_print_rule       >, Instruction_call_print_rule        >,
      pegtl::seq< pegtl::at<Instruction_call_input_rule       >, Instruction_call_input_rule        >,
	 		pegtl::seq< pegtl::at<Instruction_call_rule             >, Instruction_call_rule              >
		> {};

	struct Instruction_call_assign_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			variable_operand_rule,
			seps,
			pegtl::one<'('>,
			seps,
			pegtl::opt<
				args_list
			>,
			seps,
			pegtl::one<')'>
		> {};
	
	struct Instruction_call_print_assign_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			str_print,
			seps,
			pegtl::one<'('>,
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
			str_inp,
			seps,
			pegtl::one<'('>,
			seps,
			pegtl::one<')'>
		> {};

	struct Instruction_calls_assign_rule:
		pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_call_input_assign_rule       >, Instruction_call_input_assign_rule        >,
      pegtl::seq< pegtl::at<Instruction_call_print_assign_rule       >, Instruction_call_print_assign_rule        >,
	 		pegtl::seq< pegtl::at<Instruction_call_assign_rule             >, Instruction_call_assign_rule              >
		> {};

	struct Instruction_array_init_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			str_new,
			seps,
			str_arr,
			seps,
			pegtl::one<'('>,
			seps,
			args_list,
			seps,
			pegtl::one<')'>
		> {};

	struct Instruction_tuple_init_rule:
		pegtl::seq<
			variable_operand_rule,
			seps,
			str_arrow,
			seps,
			str_new,
			seps,
			str_tup,
			seps,
			pegtl::one<'('>,
			seps,
			var_num,
			seps,
			pegtl::one<')'>
		> {};

	struct Instruction_inits_rule:
		pegtl::sor<
	 		pegtl::seq< pegtl::at<Instruction_array_init_rule>, Instruction_array_init_rule >,
      pegtl::seq< pegtl::at<Instruction_tuple_init_rule>, Instruction_tuple_init_rule >
		> {};

  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_define_var_rule  >, Instruction_define_var_rule   >,
      pegtl::seq< pegtl::at<Instruction_inits_rule       >, Instruction_inits_rule        >,
      pegtl::seq< pegtl::at<Instruction_calls_rule       >, Instruction_calls_rule        >,
      pegtl::seq< pegtl::at<Instruction_calls_assign_rule>, Instruction_calls_assign_rule >,
      pegtl::seq< pegtl::at<Instruction_op_rule          >, Instruction_op_rule           >,
      pegtl::seq< pegtl::at<Instruction_cmp_rule         >, Instruction_cmp_rule          >,
      pegtl::seq< pegtl::at<Instruction_from_array_rule  >, Instruction_from_array_rule   >,
      pegtl::seq< pegtl::at<Instruction_to_array_rule    >, Instruction_to_array_rule     >,
      pegtl::seq< pegtl::at<Instruction_length_rule      >, Instruction_length_rule       >,
      pegtl::seq< pegtl::at<Instruction_mov_rule         >, Instruction_mov_rule          >,
      pegtl::seq< pegtl::at<Instruction_label_rule       >, Instruction_label_rule        >,
      pegtl::seq< pegtl::at<Instruction_branch_rule      >, Instruction_branch_rule       >,
      pegtl::seq< pegtl::at<Instruction_cond_branch_rule >, Instruction_cond_branch_rule  >,
      pegtl::seq< pegtl::at<Instruction_return_val_rule  >, Instruction_return_val_rule   >,
      pegtl::seq< pegtl::at<Instruction_return_rule      >, Instruction_return_rule       >
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
    name {};

  struct Function_rule:
    pegtl::seq<
			data_type,
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
			newF->retType = dType;
      p.functions.push_back(newF);
			declaredVars.clear();
			tups.clear();
			//codeVars.clear();
			ints.clear();
    }
  };

	template<> struct action < data_type > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "data type found: " << in.string() << std::endl;
			dType = in.string();
    }
  };

  template<> struct action < vars_list > {
    template< typename Input >
  	static void apply( const Input & in, Program & p){
			auto currF = p.functions.back();
    	if (printActions) std::cout << "vars list" << std::endl;
			for (auto i : paramList) {
				currF->params.push_back(dynamic_cast<Variable*>(i));
			}
			paramList.clear();
    }
  };

	template<> struct action < label_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "label operand (push): " << in.string() << std::endl;
			auto currF = p.functions.back();
			auto new_item = new Label(in.string());
			parsedItems.push_back(new_item);
			if (currF->longestLabel.size() < in.string().size() - 1) {
				currF->longestLabel = in.string().substr(1);
			}
    }
  };

	template<> struct action < number_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "number operand (push): " << in.string() << std::endl;
			auto new_item = new Number(std::stoll(in.string()));
			parsedItems.push_back(new_item);
    }
  };

	template<> struct action < variable_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "variable operand (push): " << in.string() << std::endl;
			auto new_item = new Variable(in.string());
			parsedItems.push_back(new_item);
			auto currF = p.functions.back();
			if (new_item->getDup(ints)) {
				new_item->type = "int64";
			}
			if (currF->longestVar.size() < in.string().size()) {
				currF->longestVar = in.string();
			}
    }
  };

	template<> struct action < list_variable_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "list variable operand" << std::endl;
			auto new_item = new Variable(in.string());
			new_item->type = dType;
			paramList.push_back(new_item);
			auto currF = p.functions.back();
			if (currF->longestVar.size() < in.string().size()) {
				currF->longestVar = in.string();
			}
			//if (dType.compare("tuple") == 0) tups.push_back(new_item);
    }
  };

	template<> struct action < func_list_variable_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "func list variable operand" << std::endl;
			auto new_item = new Variable(in.string());
			new_item->type = dType;
			paramList.push_back(new_item);
			declaredVars.push_back(new_item);
			auto currF = p.functions.back();
			if (currF->longestVar.size() < in.string().size()) {
				currF->longestVar = in.string();
			}
			if (dType.compare("tuple") == 0) tups.push_back(new_item);
			if (dType.compare("code") == 0) codeVars.push_back(new_item);
    }
  };

	template<> struct action < list_number_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "list num operand" << std::endl;
			auto new_item = new Number(std::stoll(in.string()));
			paramList.push_back(new_item);
    }
  };

	template<> struct action < Instruction_return_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "ret rule" << std::endl;
			auto currF = p.functions.back();
			currF->instrs.push_back(new Instruction_return());
    }
  };

	template<> struct action < Instruction_return_val_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "ret val rule" << std::endl;
			auto currF = p.functions.back();
			auto rv = parsedItems.back();
			parsedItems.pop_back();
			if (dynamic_cast<Number*>(rv)) {
				auto newInstr = new Instruction_return_val(new Number(2*dynamic_cast<Number*>(rv)->value + 1));
				currF->instrs.push_back(newInstr);
			} else {
				auto newInstr = new Instruction_return_val(rv);
				currF->instrs.push_back(newInstr);
			}
    }
  };

	template<> struct action < Instruction_define_var_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "defn var rule" << std::endl;
			auto currF = p.functions.back();
			auto var = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			if (var) {
				declaredVars.push_back(var);
				auto newInstr = new Instruction_define_var(dType, var);
				currF->instrs.push_back(newInstr);
				if (dType.compare("tuple") == 0) {
					tups.push_back(var);
				} else if (dType.compare("int64") == 0) {
					ints.push_back(var);
				} else if (dType.compare("code") == 0) {
					codeVars.push_back(var);
				}
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}			
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
				if (dst->getDup(codeVars)) {
					auto newS = dynamic_cast<Variable*>(src);
					if (newS) {
						auto newInstr1 = new Instruction_mov(new Label(newS->name), dst);
						codePtrs[dst] = new Label(newS->name);
						currF->instrs.push_back(newInstr1);
					} else {
						std::cerr << "parse error: improper ops" << std::endl;
					}
				} else {
					auto newInstr = new Instruction_mov(src, dst);
					currF->instrs.push_back(newInstr);
				}
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
				auto newInstr = new Instruction_plus(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_minus(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_times(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_and(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_lsh(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_rsh(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_eq(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_le(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_ge(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_less(left, right, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_greater(left, right, dst);
				currF->instrs.push_back(newInstr);
			} else {
				std::cerr << "parse error: improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_from_array_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "from array rule" << std::endl;
			auto currF = p.functions.back();
			auto src = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			std::vector<Item*> args;
			for (auto i : paramList) {
				args.push_back(i);
			}
			paramList.clear();
			if (src && dst) {
				bool isTup = false;
				if (src->getDup(tups)) {
					isTup = true;
				}
				auto newInstr = new Instruction_from_array((2*in.position().line + 1), isTup, src, args, dst);
				currF->instrs.push_back(newInstr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_to_array_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "to array rule" << std::endl;
			auto currF = p.functions.back();
			auto src = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			std::vector<Item*> args;
			for (auto i : paramList) {
				args.push_back(i);
			}
			paramList.clear();
			if (dst) {
				bool isTup = false;
				if (dst->getDup(tups)) {
					isTup = true;
				}
				auto newInstr = new Instruction_to_array((2*in.position().line + 1), isTup, src, args, dst);
				currF->instrs.push_back(newInstr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_length_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "length rule" << std::endl;
			auto currF = p.functions.back();
			auto dim = parsedItems.back();
			parsedItems.pop_back();
			auto src = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*> (parsedItems.back());
			parsedItems.pop_back();
			if (src && dst) {
				auto newInstr = new Instruction_length(src, dim, dst);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_label(lbl);
				currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_branch(lbl);
				currF->instrs.push_back(newInstr);
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
			auto lblF = dynamic_cast<Label*> (parsedItems.back());
			parsedItems.pop_back();
			auto lblT = dynamic_cast<Label*> (parsedItems.back());
			parsedItems.pop_back();
			auto cond = parsedItems.back();
			parsedItems.pop_back();
			if (lblT && lblF) {
				auto newInstr = new Instruction_cond_branch(cond, lblT, lblF);
				currF->instrs.push_back(newInstr);
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
			auto dst = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			std::vector<Item*> args;
			for (auto i : paramList) {
				if (dynamic_cast<Number*>(i)) {
					args.push_back(new Number(2*(dynamic_cast<Number*>(i)->value) + 1));
				} else {
					args.push_back(i);
				}
			}
			paramList.clear();
			if (dst) {
				if (!(dst->getDup(declaredVars))) { // call to local fcn
					//if (dst->getDup(codeVars)) {
						
					//}
					auto dstLbl = new Label(dst->name);
					auto newInstr1 = new Instruction_call(dstLbl, args);
					currF->instrs.push_back(newInstr1);
				} else {
					auto newInstr = new Instruction_call(dst, args);
					currF->instrs.push_back(newInstr);
				}
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_call_print_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_print rule" << std::endl;
			auto currF = p.functions.back();
			auto arg = parsedItems.back();
			parsedItems.pop_back();
			if (dynamic_cast<Number*>(arg)) {
				auto newArg = new Number(2*(dynamic_cast<Number*>(arg)->value)+1);
				auto newInstr = new Instruction_call_print(newArg);
				currF->instrs.push_back(newInstr);
			} else {
				auto newInstr1 = new Instruction_call_print(arg);
				currF->instrs.push_back(newInstr1);
			}
    }
  };

  template<> struct action < Instruction_call_input_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_input rule" << std::endl;
			auto currF = p.functions.back();
			auto newInstr = new Instruction_call_input();
			currF->instrs.push_back(newInstr);
    }
  };

  template<> struct action < Instruction_call_assign_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call assign rule" << std::endl;
			auto currF = p.functions.back();
			auto src = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			std::vector<Item*> args;
			for (auto i : paramList) {
				if (dynamic_cast<Number*>(i)) {
					args.push_back(new Number(2*(dynamic_cast<Number*>(i)->value) + 1));
				} else {
					args.push_back(i);
				}
			}
			paramList.clear();
			if (dst && src) {
				if (!(src->getDup(declaredVars))) {
					auto srcLbl = new Label(src->name);
					auto newInstr1 = new Instruction_call_assign(srcLbl, args, dst);
					currF->instrs.push_back(newInstr1);
				} else {
					auto newInstr = new Instruction_call_assign(src, args, dst);
					currF->instrs.push_back(newInstr);
				}
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_call_print_assign_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_print assign rule" << std::endl;
			auto currF = p.functions.back();
			auto arg = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			auto newInstr = new Instruction_call_print_assign(arg, dst);
			currF->instrs.push_back(newInstr);
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
				auto newInstr = new Instruction_call_input_assign(dst);
				currF->instrs.push_back(newInstr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < Instruction_array_init_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "array init rule" << std::endl;
			auto currF = p.functions.back();
			auto dst = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			std::vector<Item*> args;
			for (auto i : paramList) {
				if (dynamic_cast<Number*>(i)) {
					args.push_back(new Number(2*(dynamic_cast<Number*>(i))->value + 1));
				} else {
					args.push_back(i);
				}
			}
			paramList.clear();
			if (dst) {
				auto newInstr = new Instruction_array_init(args, dst);
				currF->instrs.push_back(newInstr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
			
    }
  };

	template<> struct action < Instruction_tuple_init_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "tuple init rule" << std::endl;
			auto currF = p.functions.back();
			auto arg = parsedItems.back();
			parsedItems.pop_back();
			auto dst = dynamic_cast<Variable*>(parsedItems.back());
			parsedItems.pop_back();
			if (dst) {
				if (dynamic_cast<Number*>(arg)) {
					auto newInstr1 = new Instruction_tuple_init(new Number(2*(dynamic_cast<Number*>(arg))->value + 1), dst);
					currF->instrs.push_back(newInstr1);
				} else {
					auto newInstr = new Instruction_tuple_init(arg, dst);
					currF->instrs.push_back(newInstr);
				}
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  LA::Program parseFile (char *fileName){
    pegtl::analyze< grammar >(); 
    file_input< > fileInput(fileName);
    LA::Program p;
    parse< grammar, action >(fileInput, p);

    return p;
	}

}
