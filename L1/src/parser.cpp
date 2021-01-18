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
  struct str_r8  : TAOCPP_PEGTL_STRING( "r8" ) {};
  struct str_r9  : TAOCPP_PEGTL_STRING( "r9" ) {};
  struct str_r10 : TAOCPP_PEGTL_STRING( "r10" ) {};
  struct str_r11 : TAOCPP_PEGTL_STRING( "r11" ) {};
  struct str_r12 : TAOCPP_PEGTL_STRING( "r12" ) {};
  struct str_r13 : TAOCPP_PEGTL_STRING( "r13" ) {};
  struct str_r14 : TAOCPP_PEGTL_STRING( "r14" ) {};
  struct str_r15 : TAOCPP_PEGTL_STRING( "r15" ) {};
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
      str_rax {};

  struct register_rcx_rule:
      str_rax {};

  struct register_rdx_rule:
      str_rax {};

  struct register_rbp_rule:
      str_rax {};

  struct register_rsi_rule:
      str_rax {};

  struct register_r8_rule:
      str_rax {};

  struct register_r9_rule:
      str_rax {};

  struct register_r10_rule:
      str_rax {};

  struct register_r11_rule:
      str_rax {};

  struct register_r12_rule:
      str_rax {};

  struct register_r13_rule:
      str_rax {};

  struct register_r14_rule:
      str_rax {};

  struct register_r15_rule:
      str_rax {};
   
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

  struct Instruction_return_rule:
    pegtl::seq<
      str_return
    > { };

  struct Instruction_assignment_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      register_rule
    > {};

  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_return_rule>            , Instruction_return_rule             >,
      pegtl::seq< pegtl::at<Instruction_assignment_rule>        , Instruction_assignment_rule         >
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

  template<> struct action < register_rdi_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rdi" << std::endl;
      Item i;
      i.type = 0;
      i.r = rdi;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rax_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register rax" << std::endl;
      Item i;
      i.type = 0;
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
      i.r = rsi;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r8_rule > {
    template< typename Input >
    static void apply( const Input & in, Program & p){
      if (printActions) std::cout << "register r8" << std::endl;
      Item i;
      i.type = 0;
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
      i.r = r15;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < label > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "entry point label" << std::endl;
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
    if (printActions) std::cout << "function name" << std::endl;
      auto newF = new Function();
      newF->name = in.string();
      p.functions.push_back(newF);
    }
  };

  template<> struct action < argument_number > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "function arguments number" << std::endl;
      auto currentF = p.functions.back();
      currentF->arguments = std::stoll(in.string());
    }
  };

  template<> struct action < local_number > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "function locals number" << std::endl;
      auto currentF = p.functions.back();
      currentF->locals = std::stoll(in.string());
    }
  };

  template<> struct action < Label_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "adding item to parsed_items" << std::endl;
      Item i;
      i.type = -1;
      i.value = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < str_return > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "return instruction" << std::endl;
      auto currentF = p.functions.back();
      auto i = new Instruction();
      i->type = 0;
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_assignment_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
    if (printActions) std::cout << "assignment instruction" << std::endl;

      // Fetch the current function. 
      auto currentF = p.functions.back();

      // Create the instruction. 
      auto i = new Instruction();
      i->type = 1;
      Item* new_item = &parsed_items.back();
      i->items.push_back(new_item);
      parsed_items.pop_back();
      new_item = &parsed_items.back();
      i->items.push_back(new_item);
      parsed_items.pop_back();

      // Add the just-created instruction to the current function.
      currentF->instructions.push_back(i);
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
