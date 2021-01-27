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

#include <L2.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace L2 {

  bool printActions = false;

  /* 
   * Data required to parse
   */ 
  std::vector<Item> parsed_items;

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
   * Keywords.
   */
  struct str_rdi : TAOCPP_PEGTL_STRING( "rdi" ) {};
  struct str_rax : TAOCPP_PEGTL_STRING( "rax" ) {};
  struct str_rbx : TAOCPP_PEGTL_STRING( "rbx" ) {};
  struct str_rcx : TAOCPP_PEGTL_STRING( "rcx" ) {};
  struct str_rdx : TAOCPP_PEGTL_STRING( "rdx" ) {};
  struct str_rbp : TAOCPP_PEGTL_STRING( "rbp" ) {};
  struct str_rsi : TAOCPP_PEGTL_STRING( "rsi" ) {};
  struct str_rsp : TAOCPP_PEGTL_STRING( "rsp" ) {};
  struct str_r8  : TAOCPP_PEGTL_STRING( "r8"  ) {};
  struct str_r9  : TAOCPP_PEGTL_STRING( "r9"  ) {};
  struct str_r10 : TAOCPP_PEGTL_STRING( "r10" ) {};
  struct str_r11 : TAOCPP_PEGTL_STRING( "r11" ) {};
  struct str_r12 : TAOCPP_PEGTL_STRING( "r12" ) {};
  struct str_r13 : TAOCPP_PEGTL_STRING( "r13" ) {};
  struct str_r14 : TAOCPP_PEGTL_STRING( "r14" ) {};
  struct str_r15 : TAOCPP_PEGTL_STRING( "r15" ) {};
  struct str_mem : TAOCPP_PEGTL_STRING( "mem" ) {};
  struct str_pe  : TAOCPP_PEGTL_STRING( "+="  ) {};
  struct str_me  : TAOCPP_PEGTL_STRING( "-="  ) {};
  struct str_te  : TAOCPP_PEGTL_STRING( "*="  ) {};
  struct str_ae  : TAOCPP_PEGTL_STRING( "&="  ) {};
  struct str_pp  : TAOCPP_PEGTL_STRING( "++"  ) {};
  struct str_mm  : TAOCPP_PEGTL_STRING( "--"  ) {};
  struct str_eq  : TAOCPP_PEGTL_STRING( "="   ) {};
  struct str_le  : TAOCPP_PEGTL_STRING( "<="  ) {};
  struct str_less: TAOCPP_PEGTL_STRING( "<"   ) {};
  struct str_lsh : TAOCPP_PEGTL_STRING( "<<=" ) {};
  struct str_rsh : TAOCPP_PEGTL_STRING( ">>=" ) {};
  struct str_at  : TAOCPP_PEGTL_STRING( "@"   ) {};
  struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};
  struct str_goto : TAOCPP_PEGTL_STRING( "goto" ) {};
  struct str_cjump: TAOCPP_PEGTL_STRING( "cjump" ) {};
  struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};
  struct str_print: TAOCPP_PEGTL_STRING( "print" ) {};
  struct str_alloc: TAOCPP_PEGTL_STRING( "allocate" ) {};
  struct str_inp  : TAOCPP_PEGTL_STRING( "input" ) {};
  struct str_tensor: TAOCPP_PEGTL_STRING( "tensor-error" ) {};
  struct str_stack_arg: TAOCPP_PEGTL_STRING( "stack-arg" ) {};
  struct str_arrow : TAOCPP_PEGTL_STRING( "<-" ) {};

  struct comment: 
    pegtl::disable< 
      TAOCPP_PEGTL_STRING( "//" ), 
      pegtl::until< pegtl::eolf > 
    > {};

  struct label:
    pegtl::seq<
      pegtl::one<':'>,
      name
    > {};

  struct variable:
    pegtl::seq<
      pegtl::one<'%'>,
      name
    > {};

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

  struct function_name:
    label {};

  struct argument_number:
    number {};

  struct local_number:
    number {} ;

  struct seps: 
    pegtl::star< 
      pegtl::sor< 
        pegtl::ascii::space, 
        comment 
      > 
    > {};

  struct Label_rule:
    label {};

  struct label_operand_rule:
    label {};

  struct number_operand_rule:
    number {};

  struct mem_operand_rule:
    pegtl::seq<
      seps,
      str_mem,
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      number_operand_rule,
      seps
    > {};

  struct aop_pe_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        mem_operand_rule,
        variable
      >,
      seps,
      str_pe,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        mem_operand_rule,
        variable
      >,
      seps
    > {};

  struct aop_me_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        mem_operand_rule,
        variable
      >,
      seps,
      str_me,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        mem_operand_rule,
        variable
      >,
      seps
    > {};

  struct aop_te_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_te,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct aop_ae_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_ae,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct aop_pp_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_pp,
      seps
    > {};

  struct aop_mm_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_mm,
      seps
    > {};

  struct sop_lsh_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_lsh,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct sop_rsh_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_rsh,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct instr_aop_rule:
    pegtl::sor<
      aop_pe_rule,
      aop_me_rule,
      aop_te_rule,
      aop_ae_rule,
      aop_pp_rule,
      aop_mm_rule
    > {};

  struct instr_sop_rule:
    pegtl::sor<
      sop_lsh_rule,
      sop_rsh_rule
    > {};

  struct comp_less_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      str_less,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct comp_le_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      str_le,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct comp_eq_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      str_eq,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct cond_less_jump_rule:
    pegtl::seq<
      seps,
      str_cjump,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      str_less,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      Label_rule,
      seps
    > {};

  struct cond_le_jump_rule:
    pegtl::seq<
      seps,
      str_cjump,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      str_le,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      Label_rule,
      seps
    > {};

  struct cond_eq_jump_rule:
    pegtl::seq<
      seps,
      str_cjump,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      str_eq,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule,
        variable
      >,
      seps,
      Label_rule,
      seps
    > {};

  struct instr_comp_rule:
    pegtl::sor<
      comp_less_rule,
      comp_le_rule,
      comp_eq_rule
    > {};

  struct instr_dir_jump_rule:
    pegtl::seq<
      seps,
      str_goto,
      seps,
      Label_rule,
      seps
    > {};

  struct instr_cond_jump_rule:
    pegtl::sor<
      cond_less_jump_rule,
      cond_le_jump_rule,
      cond_eq_jump_rule
    > {};

  struct instr_at_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_at,
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      number_operand_rule,
      seps
    > {};

  struct instr_load_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_arrow,
      seps,
      str_mem,
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      number_operand_rule,
      seps
    > {};

  struct instr_store_rule:
    pegtl::seq<
      seps,
      str_mem,
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      number_operand_rule,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        register_rule,
        label_operand_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct instr_call_local_rule:
    pegtl::seq<
      seps,
      str_call,
      seps,
      pegtl::sor<
        Label_rule,
        register_rule,
        variable
      >,
      seps,
      number_operand_rule,
      seps
    > {};

  struct runtime_call_rule:
    pegtl::sor<
      str_print,
      str_alloc,
      str_inp,
      str_tensor
    > {};

  struct instr_call_runtime_rule:
    pegtl::seq<
      seps,
      str_call,
      seps,
      runtime_call_rule,
      seps,
      number_operand_rule,
      seps
    > {};

  struct instr_call_rule:
    pegtl::sor<
      instr_call_local_rule,
      instr_call_runtime_rule
    > {};

  struct Instr_label_defn_rule:
    pegtl::seq<
      seps,
      Label_rule,
      seps
    > {};

  struct Instr_return_rule:
    pegtl::seq<
      str_return
    > { };

  struct Instr_assignment_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        register_rule,
        label_operand_rule,
        number_operand_rule,
        variable
      >,
      seps
    > {};

  struct instr_load_stack_arg_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        register_rule,
        variable
      >,
      seps,
      str_arrow,
      seps,
      str_stack_arg,
      seps,
      number_operand_rule,
      seps
    > {};

  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instr_return_rule>         , Instr_return_rule         >,
      pegtl::seq< pegtl::at<instr_comp_rule>           , instr_comp_rule           >,
      pegtl::seq< pegtl::at<instr_cond_jump_rule>      , instr_cond_jump_rule      >,
      pegtl::seq< pegtl::at<instr_load_rule>           , instr_load_rule           >,
      pegtl::seq< pegtl::at<instr_store_rule>          , instr_store_rule          >,
      pegtl::seq< pegtl::at<Instr_assignment_rule>     , Instr_assignment_rule     >,
      pegtl::seq< pegtl::at<Instr_label_defn_rule>     , Instr_label_defn_rule     >,
      pegtl::seq< pegtl::at<instr_aop_rule>            , instr_aop_rule            >,
      pegtl::seq< pegtl::at<instr_sop_rule>            , instr_sop_rule            >,
      pegtl::seq< pegtl::at<instr_dir_jump_rule>       , instr_dir_jump_rule       >,
      pegtl::seq< pegtl::at<instr_at_rule>             , instr_at_rule             >,
      pegtl::seq< pegtl::at<instr_call_rule>           , instr_call_rule           >,
      pegtl::seq< pegtl::at<instr_load_stack_arg_rule> , instr_load_stack_arg_rule >
    > {};

  struct Instructions_rule:
    pegtl::plus<
      pegtl::seq<
        seps,
        Instruction_rule,
        seps
      >
    > { };

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
    > { };

  struct grammar : 
    pegtl::must< 
      entry_point_rule
    > {};

  struct function_grammar:
    pegtl::must<
      Function_rule
    > {};

  /* 
   * Actions attached to grammar rules.
   */
  template< typename Rule >
  struct action : pegtl::nothing< Rule > {};

  template<> struct action < label > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "entry point label: " << in.string() << std::endl;
      if (p.entryPointLabel.empty()){
        p.entryPointLabel = in.string();
      } else {
        abort();
      }
    }
  };

  template<> struct action < variable > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "variable (push): " << in.string() << std::endl;
      Item i;
      i.type = 6;
      i.value = in.string();
      parsed_items.push_back(i);
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

  template<> struct action < Label_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "adding label to parsed_items (push): " << in.string() << std::endl;
      Item i;
      i.type = -1;
      i.value = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < label_operand_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "label used as operand (push): " << in.string() << std::endl;
      Item i;
      i.type = 3;
      i.value = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < number_operand_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "number used as operand (push): " << in.string() << std::endl;
      Item i;
      i.type = 2;
      i.value = in.string();
      if (i.value.front() == '+')
          i.value = i.value.substr(1);
      parsed_items.push_back(i);
    }
  };

  template<> struct action < mem_operand_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "memory access operand (popx2, push): " << in.string() << std::endl;
      Item i;
      i.type = 1;
      i.value = parsed_items.back().value;
      parsed_items.pop_back();
      if (parsed_items.back().type == 0)
        i.register_name = parsed_items.back().register_name;
      else if (parsed_items.back().type == 6)
        i.register_name = parsed_items.back().value;
      parsed_items.pop_back();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < instr_call_local_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "instr_call_local_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = call_local;
      auto src = new Item();
      auto num = new Item();

      num->type = parsed_items.back().type;
      num->value = parsed_items.back().value;
      parsed_items.pop_back();

      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(num);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < runtime_call_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "runtime call (push): " << in.string() << std::endl;
      Item i;
      i.type = 5;
      i.value = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < instr_call_runtime_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "instr_call_runtime_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = call_runtime;
      auto src = new Item();
      auto num = new Item();

      num->type = parsed_items.back().type;
      num->value = parsed_items.back().value;
      parsed_items.pop_back();

      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(num);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < Instr_label_defn_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "label defined (pop): " << parsed_items.back().value << std::endl;

      // fetch current function
      auto currentF = p.functions.back();
      // create new instruction
      auto i = new Instruction();
      i->op = label_def;
      //create and populate new item
      auto label = new Item();
      label->type = 4;
      label->value = parsed_items.back().value;
      parsed_items.pop_back();
      // item -> instruction -> function
      i->items.push_back(label);
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < instr_load_stack_arg_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "instr_load_stack_arg_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = load_stack;
      auto src = new Item();
      auto dst = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < instr_load_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "instr_load_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = load;
      auto src = new Item();
      auto dst = new Item();
      src->type = 1;
      // get offset from number at back of vector
      src->value = parsed_items.back().value;
      parsed_items.pop_back();
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < instr_store_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "instr_store_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = store;
      auto src = new Item();
      auto dst = new Item();

      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = 1;
      dst->value = parsed_items.back().value;
      parsed_items.pop_back();
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < Instr_assignment_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "generating assignment rule (pop x2)" << std::endl;

      // fetch the current function
      auto currentF = p.functions.back();
      // create the instruction 
      auto instr = new Instruction();
      instr->op = mov;
      // create and populate new items
      auto src = new Item();
      auto dst = new Item();

      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < aop_pe_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "aop_pe_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = aop_pe;
      auto src = new Item();
      auto dst = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < aop_me_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "aop_me_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = aop_me;
      auto src = new Item();
      auto dst = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < aop_te_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "aop_te_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = aop_te;
      auto src = new Item();
      auto dst = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < aop_ae_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "aop_ae_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = aop_ae;
      auto src = new Item();
      auto dst = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < aop_pp_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "aop_pp_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = aop_pp;
      auto src = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < aop_mm_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "aop_mm_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = aop_mm;
      auto src = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < sop_lsh_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "sop_lsh_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = sop_lsh;
      auto src = new Item();
      auto dst = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      src->r = parsed_items.back().r;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < sop_rsh_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "sop_rsh_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = sop_rsh;
      auto src = new Item();
      auto dst = new Item();
      src->type = parsed_items.back().type;
      src->value = parsed_items.back().value;
      src->register_name = parsed_items.back().register_name;
      src->r = parsed_items.back().r;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // add items to instr
      instr->items.push_back(src);
      instr->items.push_back(dst);
      // add the just-created instruction to the current function
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < instr_dir_jump_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "direct jump" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = dir_jmp;
      auto dst = new Item();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      parsed_items.pop_back();

      instr->items.push_back(dst);
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < comp_less_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "comp_less_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = cmp_less;
      auto src_rhs = new Item();
      auto src_lhs = new Item();
      auto dst = new Item();

      // right side of the comparison
      src_rhs->type = parsed_items.back().type;
      src_rhs->value = parsed_items.back().value;
      src_rhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      // left side of the comparison
      src_lhs->type = parsed_items.back().type;
      src_lhs->value = parsed_items.back().value;
      src_lhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      dst->r = parsed_items.back().r;
      parsed_items.pop_back();

      instr->items.push_back(src_rhs);
      instr->items.push_back(src_lhs);
      instr->items.push_back(dst);
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < comp_le_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "comp_le_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = cmp_le;
      auto src_rhs = new Item();
      auto src_lhs = new Item();
      auto dst = new Item();

      src_rhs->type = parsed_items.back().type;
      src_rhs->value = parsed_items.back().value;
      src_rhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      src_lhs->type = parsed_items.back().type;
      src_lhs->value = parsed_items.back().value;
      src_lhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      dst->r = parsed_items.back().r;
      parsed_items.pop_back();

      instr->items.push_back(src_rhs);
      instr->items.push_back(src_lhs);
      instr->items.push_back(dst);
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < comp_eq_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "comp_eq_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = cmp_eq;
      auto src_rhs = new Item();
      auto src_lhs = new Item();
      auto dst = new Item();

      src_rhs->type = parsed_items.back().type;
      src_rhs->value = parsed_items.back().value;
      src_rhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      src_lhs->type = parsed_items.back().type;
      src_lhs->value = parsed_items.back().value;
      src_lhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dst->type = parsed_items.back().type;
      dst->value = parsed_items.back().value;
      dst->register_name = parsed_items.back().register_name;
      dst->r = parsed_items.back().r;
      parsed_items.pop_back();

      instr->items.push_back(src_rhs);
      instr->items.push_back(src_lhs);
      instr->items.push_back(dst);
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < cond_less_jump_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "cond_less_jump_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = cond_less_jmp;
      auto src_rhs = new Item();
      auto src_lhs = new Item();
      auto lbl = new Item();

      lbl->type = parsed_items.back().type;
      lbl->value = parsed_items.back().value;
      parsed_items.pop_back();

      src_rhs->type = parsed_items.back().type;
      src_rhs->value = parsed_items.back().value;
      src_rhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      src_lhs->type = parsed_items.back().type;
      src_lhs->value = parsed_items.back().value;
      src_lhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      instr->items.push_back(src_rhs);
      instr->items.push_back(src_lhs);
      instr->items.push_back(lbl);
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < cond_le_jump_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "cond_le_jump_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = cond_le_jmp;
      auto src_rhs = new Item();
      auto src_lhs = new Item();
      auto lbl = new Item();

      lbl->type = parsed_items.back().type;
      lbl->value = parsed_items.back().value;
      parsed_items.pop_back();

      src_rhs->type = parsed_items.back().type;
      src_rhs->value = parsed_items.back().value;
      src_rhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      src_lhs->type = parsed_items.back().type;
      src_lhs->value = parsed_items.back().value;
      src_lhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      instr->items.push_back(src_rhs);
      instr->items.push_back(src_lhs);
      instr->items.push_back(lbl);
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < cond_eq_jump_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "cond_eq_jump_rule" << std::endl;
      auto currentF = p.functions.back();
      auto instr = new Instruction();
      instr->op = cond_eq_jmp;
      auto src_rhs = new Item();
      auto src_lhs = new Item();
      auto lbl = new Item();

      lbl->type = parsed_items.back().type;
      lbl->value = parsed_items.back().value;
      parsed_items.pop_back();

      src_rhs->type = parsed_items.back().type;
      src_rhs->value = parsed_items.back().value;
      src_rhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      src_lhs->type = parsed_items.back().type;
      src_lhs->value = parsed_items.back().value;
      src_lhs->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      instr->items.push_back(src_rhs);
      instr->items.push_back(src_lhs);
      instr->items.push_back(lbl);
      currentF->instructions.push_back(instr);
    }
  };

  template<> struct action < instr_at_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p) {
      if (printActions) std::cout << "at rule (lea)" << std::endl;
      auto currentF = p.functions.back();
      auto i = new Instruction();
      auto scale = new Item();
      auto offset_register = new Item();
      auto base_register = new Item();
      auto dest_register = new Item();

      i->op = at;

      scale->type = parsed_items.back().type;
      scale->value = parsed_items.back().value;
      parsed_items.pop_back();

      offset_register->type = parsed_items.back().type;
      offset_register->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      base_register->type = parsed_items.back().type;
      base_register->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      dest_register->type = parsed_items.back().type;
      dest_register->register_name = parsed_items.back().register_name;
      parsed_items.pop_back();

      i->items.push_back(scale);
      i->items.push_back(offset_register);
      i->items.push_back(base_register);
      i->items.push_back(dest_register);

      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < str_return > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "return instruction" << std::endl;
      auto currentF = p.functions.back();
      auto i = new Instruction();
      i->op = ret;
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < register_rdi_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rdi (push)" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "rdi";
      i.r = rdi;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rax_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rax (push)" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "rax";
      i.r = rax;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rbx_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rbx" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "rbx";
      i.r = rbx;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rcx_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rcx" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "rcx";
      i.r = rcx;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rdx_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rdx" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "rdx";
      i.r = rdx;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rbp_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rbp" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "rbp";
      i.r = rbp;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rsi_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rsi" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "rsi";
      i.r = rsi;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rsp_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rsp" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "rsp";
      i.r = rsp;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r8_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r8" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "r8";
      i.r = r8;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r9_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r9" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "r9";
      i.r = r9;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r10_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r10" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "r10";
      i.r = r10;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r11_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r11" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "r11";
      i.r = r11;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r12_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r12" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "r12";
      i.r = r12;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r13_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r13" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "r13";
      i.r = r13;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r14_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r14" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "r14";
      i.r = r14;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r15_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r15" << std::endl;
      Item i;
      i.type = 0;
      i.register_name = "r15";
      i.r = r15;
      parsed_items.push_back(i);
    }
  };

  L2::Program parse_file (char *fileName){

    /* 
     * Check the grammar for some possible issues.
     */
    pegtl::analyze< grammar >();

    /*
     * Parse.
     */   
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

}
