/*
 * SUGGESTION FROM THE CC TEAM:
 * double check the order of actions that are fired.
 * You can do this in (at least) two ways:
 * 1) by using gdb adding breakpoints to actions
 * 2) by adding printing statements in each action
 *
 * For 2), we suggest writing the code to make it straightforward to enable/disable all of them 
 * (e.g., assuming shouldIPrint is a global variable
 *    if (shouldIPrint) std::cerr << "MY OUTPUT" << std::endl;
 * )
 */
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

#include <L1.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace L1 {

  bool printActions = true;

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
  struct str_return : TAOCPP_PEGTL_STRING( "return" ) {}; 
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

  struct mem_access_operand_rule:
    pegtl::seq<
      seps,
      str_mem,
      seps,
      register_rule,
      seps,
      number_operand_rule
    > {};

  struct aop_pe_rule:
    pegtl::seq<
      seps,
      register_rule,
      seps,
      str_pe,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule
      >,
      seps
    > {};

  struct aop_me_rule:
    pegtl::seq<
      seps,
      register_rule,
      seps,
      str_me,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule
      >,
      seps
    > {};

  struct aop_te_rule:
    pegtl::seq<
      seps,
      register_rule,
      seps,
      str_te,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule
      >,
      seps
    > {};

  struct aop_ae_rule:
    pegtl::seq<
      seps,
      register_rule,
      seps,
      str_ae,
      seps,
      pegtl::sor<
        register_rule,
        number_operand_rule
      >,
      seps
    > {};

  struct instr_aop_rule:
    pegtl::sor<
      aop_pe_rule,
      aop_me_rule,
      aop_te_rule,
      aop_ae_rule
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
        pegtl::seq< pegtl::at<mem_access_operand_rule>, mem_access_operand_rule >,
        pegtl::seq< pegtl::at<register_rule>          , register_rule           >
      >,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        pegtl::seq< pegtl::at<mem_access_operand_rule>, mem_access_operand_rule >,
        pegtl::seq< pegtl::at<register_rule>          , register_rule           >,
        pegtl::seq< pegtl::at<label_operand_rule>     , label_operand_rule      >,
        pegtl::seq< pegtl::at<number_operand_rule>    , number_operand_rule     >
      >
    > {};

  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instr_return_rule>    , Instr_return_rule     >,
      pegtl::seq< pegtl::at<Instr_assignment_rule>, Instr_assignment_rule >,
      pegtl::seq< pegtl::at<Instr_label_defn_rule>, Instr_label_defn_rule >,
      pegtl::seq< pegtl::at<instr_aop_rule>       , instr_aop_rule        >
    > { };

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
      local_number,
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
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "function locals number: " << in.string() << std::endl;
      auto currentF = p.functions.back();
      currentF->locals = std::stoll(in.string());
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
      parsed_items.push_back(i);
    }
  };

  template<> struct action < mem_access_operand_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "memory access operand (pop x2, push): " << in.string() << std::endl;
      Item i;
      i.type = 1;
      i.value = parsed_items.back().value;
      parsed_items.pop_back();
      i.register_name = parsed_items.back().register_name;
      parsed_items.pop_back();
      parsed_items.push_back(i);
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
      std::cout << "label defn added: " << currentF->instructions.back()->items[0]->value << std::endl;
    }
  };

// NOTE: this is highly repetitive, replace if you have time

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
      parsed_items.push_back(i);
    }
  };

  Program parse_file (char *fileName){

    /* 
     * Check the grammar for some possible issues.
     */
    pegtl::analyze< grammar >();

    /*
     * Parse.
     */   
    file_input< > fileInput(fileName);
    Program p;
    parse< grammar, action >(fileInput, p);

    return p;
  }

}
