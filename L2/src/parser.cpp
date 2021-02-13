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
//#include <iostream>
#include <stdint.h>
#include <assert.h>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <L2.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
//using namespace std;

namespace L2 {

  bool printActions = false;

  /* 
   * Data required to parse
   */ 
  std::vector<Item*> parsed_items;

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
   * keywords
   */

  struct str_rdi    : TAOCPP_PEGTL_STRING( "rdi" ) {};
  struct str_rax    : TAOCPP_PEGTL_STRING( "rax" ) {};
  struct str_rbx    : TAOCPP_PEGTL_STRING( "rbx" ) {};
  struct str_rcx    : TAOCPP_PEGTL_STRING( "rcx" ) {};
  struct str_rdx    : TAOCPP_PEGTL_STRING( "rdx" ) {};
  struct str_rbp    : TAOCPP_PEGTL_STRING( "rbp" ) {};
  struct str_rsi    : TAOCPP_PEGTL_STRING( "rsi" ) {};
  struct str_rsp    : TAOCPP_PEGTL_STRING( "rsp" ) {};
  struct str_r8     : TAOCPP_PEGTL_STRING( "r8"  ) {};
  struct str_r9     : TAOCPP_PEGTL_STRING( "r9"  ) {};
  struct str_r10    : TAOCPP_PEGTL_STRING( "r10" ) {};
  struct str_r11    : TAOCPP_PEGTL_STRING( "r11" ) {};
  struct str_r12    : TAOCPP_PEGTL_STRING( "r12" ) {};
  struct str_r13    : TAOCPP_PEGTL_STRING( "r13" ) {};
  struct str_r14    : TAOCPP_PEGTL_STRING( "r14" ) {};
  struct str_r15    : TAOCPP_PEGTL_STRING( "r15" ) {};
  struct str_mem    : TAOCPP_PEGTL_STRING( "mem" ) {};
  struct str_pe     : TAOCPP_PEGTL_STRING( "+="  ) {};
  struct str_me     : TAOCPP_PEGTL_STRING( "-="  ) {};
  struct str_te     : TAOCPP_PEGTL_STRING( "*="  ) {};
  struct str_ae     : TAOCPP_PEGTL_STRING( "&="  ) {};
  struct str_pp     : TAOCPP_PEGTL_STRING( "++"  ) {};
  struct str_mm     : TAOCPP_PEGTL_STRING( "--"  ) {};
  struct str_eq     : TAOCPP_PEGTL_STRING( "="   ) {};
  struct str_at     : TAOCPP_PEGTL_STRING( "@"   ) {};
  struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};
  struct str_goto   : TAOCPP_PEGTL_STRING( "goto" ) {};
  struct str_cjump  : TAOCPP_PEGTL_STRING( "cjump" ) {};
  struct str_call   : TAOCPP_PEGTL_STRING( "call" ) {};
  struct str_print  : TAOCPP_PEGTL_STRING( "print" ) {};
  struct str_alloc  : TAOCPP_PEGTL_STRING( "allocate" ) {};
  struct str_inp    : TAOCPP_PEGTL_STRING( "input" ) {};
  struct str_tensor : TAOCPP_PEGTL_STRING( "tensor-error" ) {};
	struct str_stack  : TAOCPP_PEGTL_STRING( "stack-arg" ) {};
  struct str_arrow  : TAOCPP_PEGTL_STRING( "<-" ) {};
  struct str_le     : TAOCPP_PEGTL_STRING( "<="  ) {};
  struct str_less   : TAOCPP_PEGTL_STRING( "<"   ) {};
  struct str_lsh    : TAOCPP_PEGTL_STRING( "<<=" ) {};
  struct str_rsh    : TAOCPP_PEGTL_STRING( ">>=" ) {};

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

  struct register_rdi_rule:
    str_rdi {};

  struct register_rax_rule:
    str_rax {};

  struct register_rbx_rule:
    str_rbx {};

  struct register_rcx_rule:
    str_rcx {};

  struct register_rdx_rule:
    str_rdx {};

  struct register_rbp_rule:
    str_rbp {};

  struct register_rsi_rule:
    str_rsi {};

  struct register_rsp_rule:
    str_rsp {};

  struct register_r8_rule:
    str_r8 {};

  struct register_r9_rule:
    str_r9 {};

  struct register_r10_rule:
    str_r10 {};

  struct register_r11_rule:
    str_r11 {};

  struct register_r12_rule:
    str_r12 {};

  struct register_r13_rule:
  	str_r13 {};

  struct register_r14_rule:
    str_r14 {};

  struct register_r15_rule:
    str_r15 {};
   
  struct register_rule:
    pegtl::sor<
      register_rdi_rule,
      register_rax_rule,
      register_rbx_rule,
      register_rcx_rule,
      register_rdx_rule,
      register_rbp_rule,
      register_rsi_rule,
      register_rsp_rule,
      register_r8_rule,
      register_r9_rule,
      register_r10_rule,
      register_r11_rule,
      register_r12_rule,
      register_r13_rule,
      register_r14_rule,
      register_r15_rule
    > {};

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

	struct spill_var_rule:
		variable {};

	struct spill_prefix_rule:
		variable {};

	struct reg_var:
		pegtl::sor<
			register_rule,
			variable_operand_rule
		> {};

	struct memory_operand_rule:
		pegtl::seq<
			str_mem,
			seps,
			reg_var,
			seps,
			number_operand_rule
		> {};

	struct reg_var_lbl: // u
		pegtl::sor<
			register_rule,
			label_operand_rule,
			variable_operand_rule
		> {};

	struct reg_var_num: // t
		pegtl::sor<
			register_rule,
			variable_operand_rule,
			number_operand_rule
		> {};
	
	struct reg_var_num_lbl: // s
		pegtl::sor<
			register_rule,
			variable_operand_rule,
			number_operand_rule,
			label_operand_rule
		> {};

	struct reg_var_mem: // aop_pe/me only
		pegtl::sor<
			register_rule,
			variable_operand_rule,
			memory_operand_rule
		> {};
	
	struct reg_var_mem_num_lbl:
		pegtl::sor<
			register_rule,
			variable_operand_rule,
			memory_operand_rule,
			number_operand_rule,
			label_operand_rule
		> {};

	/*
	 * instructions
	 */

  struct Instruction_return_rule:
    pegtl::seq<
      str_return
    > {};

	struct Instruction_mov_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_arrow,
			seps,
			reg_var_num_lbl,
			seps
		> {};

	struct Instruction_label_rule:
		pegtl::seq<
			seps,
			label_operand_rule,
			seps
		> {};

	struct Instruction_aop_pe_rule:
		pegtl::seq<
			seps,
			reg_var_mem,
			seps,
			str_pe,
			seps,
			reg_var_mem_num_lbl,
			seps
		> {};

	struct Instruction_aop_me_rule:
		pegtl::seq<
			seps,
			reg_var_mem,
			seps,
			str_me,
			seps,
			reg_var_mem_num_lbl,
			seps
		> {};

	struct Instruction_aop_te_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_te,
			seps,
			reg_var_num,
			seps
		> {};

	struct Instruction_aop_ae_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_ae,
			seps,
			reg_var_num,
			seps
		> {};

	struct Instruction_aop_pp_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_pp,
			seps
		> {};

	struct Instruction_aop_mm_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_mm,
			seps
		> {};

	struct Instruction_sop_lsh_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_lsh,
			seps,
			reg_var_num,
			seps
		> {};

	struct Instruction_sop_rsh_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_rsh,
			seps,
			reg_var_num,
			seps
		> {};

	struct Instruction_dir_jmp_rule:
		pegtl::seq<
			seps,
			str_goto,
			seps,
			label_operand_rule,
			seps
		> {};

	struct Instruction_cmp_less_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_arrow,
			seps,
			reg_var_num,
			seps,
			str_less,
			seps,
			reg_var_num,
			seps
		> {};

	struct Instruction_cmp_le_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_arrow,
			seps,
			reg_var_num,
			seps,
			str_le,
			seps,
			reg_var_num,
			seps
		> {};

	struct Instruction_cmp_eq_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_arrow,
			seps,
			reg_var_num,
			seps,
			str_eq,
			seps,
			reg_var_num,
			seps
		> {};

	struct Instruction_cnd_jmp_less_rule:
		pegtl::seq<
			seps,
			str_cjump,
			seps,
			reg_var_num,
			seps,
			str_less,
			seps,
			reg_var_num,
			seps,
			label_operand_rule,
			seps
		> {};

	struct Instruction_cnd_jmp_le_rule:
		pegtl::seq<
			seps,
			str_cjump,
			seps,
			reg_var_num,
			seps,
			str_le,
			seps,
			reg_var_num,
			seps,
			label_operand_rule,
			seps
		> {};

	struct Instruction_cnd_jmp_eq_rule:
		pegtl::seq<
			seps,
			str_cjump,
			seps,
			reg_var_num,
			seps,
			str_eq,
			seps,
			reg_var_num,
			seps,
			label_operand_rule,
			seps
		> {};

	struct Instruction_at_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_at,
			seps,
			reg_var,
			seps,
			reg_var,
			seps,
			number_operand_rule,
			seps
		> {};

	struct Instruction_load_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_arrow,
			seps,
			memory_operand_rule,
			seps
		> {};

	struct Instruction_store_rule:
		pegtl::seq<
			seps,
			memory_operand_rule,
			seps,
			str_arrow,
			seps,
			reg_var_num_lbl,
			seps
		> {};

	struct Instruction_call_rule:
		pegtl::seq<
			seps,
			str_call,
			seps,
			reg_var_lbl,
			seps,
			number_operand_rule,
			seps
		> {};

	struct Instruction_call_print_rule:
		pegtl::seq<
			seps,
			str_call,
			seps,
			str_print,
			seps,
			pegtl::one<'1'>,
			seps
		> {};

	struct Instruction_call_input_rule:
		pegtl::seq<
			seps,
			str_call,
			seps,
			str_inp,
			seps,
			pegtl::one<'0'>,
			seps
		> {};

	struct Instruction_call_allocate_rule:
		pegtl::seq<
			seps,
			str_call,
			seps,
			str_alloc,
			seps,
			pegtl::one<'2'>,
			seps
		> {};

	struct Instruction_call_tensor_error_rule:
		pegtl::seq<
			seps,
			str_call,
			seps,
			str_tensor,
			seps,
			number_operand_rule,
			seps
		> {};

	struct Instruction_load_stack_arg_rule:
		pegtl::seq<
			seps,
			reg_var,
			seps,
			str_arrow,
			seps,
			str_stack,
			seps,
			number_operand_rule,
			seps
		> {};

  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_return_rule           >, Instruction_return_rule           >,
			pegtl::seq< pegtl::at<Instruction_cmp_less_rule         >, Instruction_cmp_less_rule         >,
			pegtl::seq< pegtl::at<Instruction_cmp_le_rule           >, Instruction_cmp_le_rule           >,
			pegtl::seq< pegtl::at<Instruction_cmp_eq_rule           >, Instruction_cmp_eq_rule           >,
			pegtl::seq< pegtl::at<Instruction_cnd_jmp_less_rule     >, Instruction_cnd_jmp_less_rule     >,
			pegtl::seq< pegtl::at<Instruction_cnd_jmp_le_rule       >, Instruction_cnd_jmp_le_rule       >,
			pegtl::seq< pegtl::at<Instruction_cnd_jmp_eq_rule       >, Instruction_cnd_jmp_eq_rule       >,
			pegtl::seq< pegtl::at<Instruction_load_rule             >, Instruction_load_rule             >,
			pegtl::seq< pegtl::at<Instruction_store_rule            >, Instruction_store_rule            >,
			pegtl::seq< pegtl::at<Instruction_mov_rule              >, Instruction_mov_rule              >,
			pegtl::seq< pegtl::at<Instruction_label_rule            >, Instruction_label_rule            >,
			pegtl::seq< pegtl::at<Instruction_aop_pe_rule           >, Instruction_aop_pe_rule           >,
			pegtl::seq< pegtl::at<Instruction_aop_me_rule           >, Instruction_aop_me_rule           >,
			pegtl::seq< pegtl::at<Instruction_aop_te_rule           >, Instruction_aop_te_rule           >,
			pegtl::seq< pegtl::at<Instruction_aop_ae_rule           >, Instruction_aop_ae_rule           >,
			pegtl::seq< pegtl::at<Instruction_aop_pp_rule           >, Instruction_aop_pp_rule           >,
			pegtl::seq< pegtl::at<Instruction_aop_mm_rule           >, Instruction_aop_mm_rule           >,
			pegtl::seq< pegtl::at<Instruction_sop_lsh_rule          >, Instruction_sop_lsh_rule          >,
			pegtl::seq< pegtl::at<Instruction_sop_rsh_rule          >, Instruction_sop_rsh_rule          >,
			pegtl::seq< pegtl::at<Instruction_dir_jmp_rule          >, Instruction_dir_jmp_rule          >,
			pegtl::seq< pegtl::at<Instruction_at_rule               >, Instruction_at_rule               >,
			pegtl::seq< pegtl::at<Instruction_call_rule             >, Instruction_call_rule             >,
			pegtl::seq< pegtl::at<Instruction_call_print_rule       >, Instruction_call_print_rule       >,
			pegtl::seq< pegtl::at<Instruction_call_input_rule       >, Instruction_call_input_rule       >,
			pegtl::seq< pegtl::at<Instruction_call_allocate_rule    >, Instruction_call_allocate_rule    >,
			pegtl::seq< pegtl::at<Instruction_call_tensor_error_rule>, Instruction_call_tensor_error_rule>,
			pegtl::seq< pegtl::at<Instruction_load_stack_arg_rule   >, Instruction_load_stack_arg_rule   >
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

  struct argument_number:
    number {};

  struct local_number:
    number {};

  struct Function_rule:
    pegtl::seq<
      seps,
      pegtl::one< '(' >,
      seps,
      function_name,
      seps,
      argument_number,
      seps,
      Instructions_rule,
      seps,
      pegtl::one< ')' >
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

  struct entry_point_rule:
    pegtl::seq<
      seps,
      pegtl::one< '(' >,
      seps,
      label,
      seps,
      Functions_rule,
      seps,
      pegtl::one< ')' >,
      seps
    > {};

  struct grammar: 
    pegtl::must< 
      entry_point_rule
    > {};

	struct function_grammar:
		pegtl::must<
			Function_rule
		> {};

	struct spill_file_grammar:
		pegtl::seq<
			seps,
			Function_rule,
			seps,
			spill_var_rule,
			seps,
			spill_prefix_rule,
			seps
		> {};

  /* 
   * actions
   */

  template< typename Rule >
  struct action : pegtl::nothing< Rule > {};

  template<> struct action < label > {
    template< typename Input >
  	static void apply( const Input & in, Program & p){
      if (p.entryPointLabel.empty()){
    		if (printActions) std::cout << "entry point label: " << in.string() << std::endl;
        p.entryPointLabel = in.string();
      } else {
        abort();
      }
    }
  };

  template<> struct action < function_name > {
    template< typename Input >
  	static void apply( const Input & in, Program & p){
    	if (printActions) std::cout << "function name: " << in.string() << std::endl;
      auto newF = new Function();
      newF->name = in.string();
      p.functions.push_back(newF);
    }
  };

  template<> struct action < argument_number > {
    template< typename Input >
  	static void apply( const Input & in, Program & p){
    	if (printActions) std::cout << "function arguments number: " << in.string() << std::endl;
      auto currentF = p.functions.back();
      currentF->arguments = std::stoll(in.string());
    }
  };

  template<> struct action < local_number > {
    template< typename Input >
  	static void apply(const Input & in, Program & p) {
    	if (printActions) std::cout << "function locals number: " << in.string() << std::endl;
      auto currentF = p.functions.back();
      currentF->locals = std::stoll(in.string());
    }
  };

  template<> struct action < str_return > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "return instruction" << std::endl;
			auto curr_f = p.functions.back();
      auto instr = new Instruction_return(curr_f->arguments > 6 ? (curr_f->arguments-6 + curr_f->locals) : curr_f->locals);
			curr_f->instructions.push_back(instr);
    }
  };

  template<> struct action < Instruction_mov_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "mov rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto dst = dynamic_cast<Variable*> (parsed_items.back());
			parsed_items.pop_back();
			if (dst) {
				auto instr = new Instruction_mov(src, dst);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < Instruction_label_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "label rule" << std::endl;
			auto curr_f = p.functions.back();
			auto label = dynamic_cast<Label*> (parsed_items.back());
			parsed_items.pop_back();
			if (label) {
				auto instr = new Instruction_label(label);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < Instruction_aop_pe_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "aop_pe rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_aop_pe(src, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_aop_me_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "aop_me rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_aop_me(src, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_aop_te_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "aop_te rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_aop_te(src, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_aop_ae_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "aop_ae rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_aop_ae(src, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_aop_pp_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "aop_pp rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_aop_pp(src);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_aop_mm_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "aop_mm rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_aop_mm(src);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_sop_lsh_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "sop_lsh rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_sop_lsh(src, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_sop_rsh_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "sop_rsh rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_sop_rsh(src, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_dir_jmp_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "dir_jmp rule" << std::endl;
			auto curr_f = p.functions.back();
			auto label = dynamic_cast<Label*> (parsed_items.back());
			parsed_items.pop_back();
			if (label) {
				auto instr = new Instruction_dir_jmp(label);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < Instruction_cmp_less_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "cmp_less rule" << std::endl;
			auto curr_f = p.functions.back();
			auto right = parsed_items.back();
			parsed_items.pop_back();
			auto left = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_cmp_less(left, right, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_cmp_le_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "cmp_le rule" << std::endl;
			auto curr_f = p.functions.back();
			auto right = parsed_items.back();
			parsed_items.pop_back();
			auto left = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_cmp_le(left, right, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_cmp_eq_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "cmp_eq rule" << std::endl;
			auto curr_f = p.functions.back();
			auto right = parsed_items.back();
			parsed_items.pop_back();
			auto left = parsed_items.back();
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			auto instr = new Instruction_cmp_eq(left, right, dst);
			curr_f->instructions.push_back(instr);
    }
  };

	template<> struct action < Instruction_cnd_jmp_less_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "cnd_jmp_less rule" << std::endl;
			auto curr_f = p.functions.back();
			auto dst = dynamic_cast<Label*> (parsed_items.back());
			parsed_items.pop_back();
			auto right = parsed_items.back();
			parsed_items.pop_back();
			auto left = parsed_items.back();
			parsed_items.pop_back();
			if (dst && dynamic_cast<Number*>(left) && dynamic_cast<Number*>(right)) {
				auto left_n = dynamic_cast<Number*>(left);
				auto right_n = dynamic_cast<Number*>(right);
				if (left_n->value < right_n->value) {
					auto instr = new Instruction_dir_jmp(dst);
					curr_f->instructions.push_back(instr);
				}
			} else if (dst) {
				auto instr = new Instruction_cnd_jmp_less(left, right, dst);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < Instruction_cnd_jmp_le_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "cnd_jmp_le rule" << std::endl;
			auto curr_f = p.functions.back();
			auto dst = dynamic_cast<Label*> (parsed_items.back());
			parsed_items.pop_back();
			auto right = parsed_items.back();
			parsed_items.pop_back();
			auto left = parsed_items.back();
			parsed_items.pop_back();
			if (dst && dynamic_cast<Number*>(left) && dynamic_cast<Number*>(right)) {
				auto left_n = dynamic_cast<Number*>(left);
				auto right_n = dynamic_cast<Number*>(right);
				if (left_n->value <= right_n->value) {
					auto instr = new Instruction_dir_jmp(dst);
					curr_f->instructions.push_back(instr);
				}
			} else if (dst) {
				auto instr = new Instruction_cnd_jmp_le(left, right, dst);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < Instruction_cnd_jmp_eq_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "cnd_jmp_eq rule" << std::endl;
			auto curr_f = p.functions.back();
			auto dst = dynamic_cast<Label*> (parsed_items.back());
			parsed_items.pop_back();
			auto right = parsed_items.back();
			parsed_items.pop_back();
			auto left = parsed_items.back();
			parsed_items.pop_back();
			if (dst && dynamic_cast<Number*>(left) && dynamic_cast<Number*>(right)) {
				auto left_n = dynamic_cast<Number*>(left);
				auto right_n = dynamic_cast<Number*>(right);
				if (left_n->value == right_n->value) {
					auto instr = new Instruction_dir_jmp(dst);
					curr_f->instructions.push_back(instr);
				}
			} else if (dst) {
				auto instr = new Instruction_cnd_jmp_eq(left, right, dst);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < Instruction_at_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "at rule" << std::endl;
			auto curr_f = p.functions.back();
			auto scale = dynamic_cast<Number*> (parsed_items.back());
			parsed_items.pop_back();
			auto index = dynamic_cast<Variable*> (parsed_items.back());
			parsed_items.pop_back();
			auto base = dynamic_cast<Variable*> (parsed_items.back());
			parsed_items.pop_back();
			auto dst = dynamic_cast<Variable*> (parsed_items.back());
			parsed_items.pop_back();
			if (scale && index && base && dst) {
				auto instr = new Instruction_at(base, index, scale, dst);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_load_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "load rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = dynamic_cast<Memory*> (parsed_items.back());
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			if (src) {
				auto instr = new Instruction_load(src, dst);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_store_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "store rule" << std::endl;
			auto curr_f = p.functions.back();
			auto src = parsed_items.back();
			parsed_items.pop_back();
			auto dst = dynamic_cast<Memory*> (parsed_items.back());
			parsed_items.pop_back();
			if (dst) {
				auto instr = new Instruction_store(src, dst);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_call_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call rule" << std::endl;
			auto curr_f = p.functions.back();
			auto args = dynamic_cast<Number*> (parsed_items.back());
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			if (args) {
				auto instr = new Instruction_call(dst, args);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_call_print_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_print rule" << std::endl;
			auto curr_f = p.functions.back();
			auto instr = new Instruction_call_print();
			curr_f->instructions.push_back(instr);
    }
  };

  template<> struct action < Instruction_call_input_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_input rule" << std::endl;
			auto curr_f = p.functions.back();
			auto instr = new Instruction_call_input();
			curr_f->instructions.push_back(instr);
    }
  };

  template<> struct action < Instruction_call_allocate_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_allocate rule" << std::endl;
			auto curr_f = p.functions.back();
			auto instr = new Instruction_call_allocate();
			curr_f->instructions.push_back(instr);
    }
  };

  template<> struct action < Instruction_call_tensor_error_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "call_tensor_error rule" << std::endl;
			auto curr_f = p.functions.back();
			auto args = dynamic_cast<Number*> (parsed_items.back());
			parsed_items.pop_back();
			if (args) {
				auto instr = new Instruction_call_tensor_error(args);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

  template<> struct action < Instruction_load_stack_arg_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "load_stack_arg rule" << std::endl;
			auto curr_f = p.functions.back();
			auto offset = dynamic_cast<Number*> (parsed_items.back());
			parsed_items.pop_back();
			auto dst = parsed_items.back();
			parsed_items.pop_back();
			if (offset) {
				auto instr = new Instruction_load_stack_arg(offset, dst);
				curr_f->instructions.push_back(instr);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < label_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "label operand (push)" << std::endl;
			auto new_item = new Label(in.string());
			parsed_items.push_back(new_item);
    }
  };

	template<> struct action < number_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "number operand (push)" << std::endl;
			auto new_item = new Number(std::stoll(in.string()));
			parsed_items.push_back(new_item);
    }
  };

	template<> struct action < memory_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "memory operand (pop x2, push x1)" << std::endl;
			auto num = dynamic_cast<Number*> (parsed_items.back());
			parsed_items.pop_back();
			auto reg = dynamic_cast<Variable*> (parsed_items.back());
			parsed_items.pop_back();
			if (num && reg)  {
				auto new_item = new Memory(reg, num);
				parsed_items.push_back(new_item);
			} else {
				std::cerr << "improper operands" << std::endl;
			}
    }
  };

	template<> struct action < variable_operand_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "variable operand (push)" << std::endl;
			auto new_item = new Variable(in.string());
			parsed_items.push_back(new_item);
    }
  };

	template<> struct action < spill_var_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "spill var" << std::endl;
			p.spill_var = new Variable(in.string());
    }
  };

	template<> struct action < spill_prefix_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "spill prefix" << std::endl;
			p.spill_prefix = in.string();
    }
  };

  template<> struct action < register_rax_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register rax (push)" << std::endl;
			auto new_reg = new Register("rax", rax);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_rbx_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register rbx (push)" << std::endl;
			auto new_reg = new Register("rbx", rbx);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_rcx_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register rcx (push)" << std::endl;
			auto new_reg = new Register("rcx", rcx);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_rdx_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register rdx (push)" << std::endl;
			auto new_reg = new Register("rdx", rdx);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_rdi_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register rdi (push)" << std::endl;
			auto new_reg = new Register("rdi", rdi);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_rsi_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register rsi (push)" << std::endl;
			auto new_reg = new Register("rsi", rsi);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_rbp_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register rbp (push)" << std::endl;
			auto new_reg = new Register("rbp", rbp);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_r8_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register r8 (push)" << std::endl;
			auto new_reg = new Register("r8", r8);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_r9_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register r9 (push)" << std::endl;
			auto new_reg = new Register("r9", r9);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_r10_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register r10 (push)" << std::endl;
			auto new_reg = new Register("r10", r10);
			parsed_items.push_back(new_reg);
    }
  };
	

  template<> struct action < register_r11_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register r11 (push)" << std::endl;
			auto new_reg = new Register("r11", r11);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_r12_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register r12 (push)" << std::endl;
			auto new_reg = new Register("r12", r12);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_r13_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register r13 (push)" << std::endl;
			auto new_reg = new Register("r13", r13);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_r14_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register r14 (push)" << std::endl;
			auto new_reg = new Register("r14", r14);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_r15_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register r15 (push)" << std::endl;
			auto new_reg = new Register("r15", r15);
			parsed_items.push_back(new_reg);
    }
  };

  template<> struct action < register_rsp_rule > {
    template< typename Input >
    static void apply(const Input & in, Program & p) {
      if (printActions) std::cout << "register rsp (push)" << std::endl;
			auto new_reg = new Register("rsp", rsp);
			parsed_items.push_back(new_reg);
    }
  };

  L2::Program parse_file (char *fileName){
    pegtl::analyze< grammar >(); 
    file_input< > fileInput(fileName);
    L2::Program p;
    parse< grammar, action >(fileInput, p);

    return p;
  }

  L2::Program parse_function_file(char* fileName) {
    pegtl::analyze< function_grammar >();
    file_input< > fileInput(fileName);
    L2::Program p;
    parse< function_grammar, action >(fileInput, p);

    return p;
  }

  L2::Program parse_spill_file(char* fileName) {
    pegtl::analyze< spill_file_grammar >();
    file_input< > fileInput(fileName);
    L2::Program p;
    parse< spill_file_grammar, action >(fileInput, p);

    return p;
  }

}
