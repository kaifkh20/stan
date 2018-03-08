#ifndef STAN_LANG_GENERATOR_GENERATE_VALIDATE_TPARAM_INITS_HPP
#define STAN_LANG_GENERATOR_GENERATE_VALIDATE_TPARAM_INITS_HPP

#include <stan/lang/ast.hpp>
#include <stan/lang/generator/constants.hpp>
#include <stan/lang/generator/generate_indent.hpp>
#include <stan/lang/generator/write_begin_all_dims_row_maj_loop.hpp>
#include <stan/lang/generator/write_end_loop.hpp>
#include <stan/lang/generator/write_var_idx_all_dims.hpp>
#include <stan/lang/generator/write_var_idx_all_dims_msg.hpp>
#include <ostream>
#include <vector>

namespace stan {
  namespace lang {

    // check initialization
    
    /**
     * Generate code to validate the specified variable declaration
     * using the specified indentation level and stream.
     * Checks any defined bounds or constraints on specialized types.
     * NOTE:  bounded / specialized types are mutually exclusive
     *
     * @param[in] decl variable declaration
     * @param[in] indent indentation level
     * @param[in,out] o stream for generating
     */
    void generate_validate_tparam_inits(const block_var_decl decl,
                                        int indent, std::ostream& o) {

      write_begin_all_dims_row_maj_loop(decl, true, indent, o);
      
      // innermost loop stmt: do check, throw exception
      generate_indent(indent + decl.type().num_dims(), o);
      o << "if (stan::math::is_uninitialized(" << decl.name();
      write_var_idx_all_dims(decl.type().num_dims(),
                             decl.type().num_dims() - decl.type().array_dims(),
                             o);
      o << ")) {" << EOL;

      generate_indent(indent + decl.type().num_dims() + 1, o);
      o << "std::stringstream msg__;" << EOL;

      generate_indent(indent + decl.type().num_dims() + 1, o);
      o << "msg__ << \"Undefined transformed parameter: " << decl.name() << "\"";
      write_var_idx_all_dims_msg(decl.type().num_dims(),
                                 decl.type().num_dims() - decl.type().array_dims(),
                                 o);
      o << ";" << EOL;
      
      generate_indent(indent + decl.type().num_dims() + 1, o);
      o << "stan::lang::rethrow_located("
        << "std::runtime_error(std::string(\"Error initializing variable "
        << decl.name()
        << ": \") + e.what()), current_statement_begin__, prog_reader__());"
        << EOL;

      generate_indent(indent + decl.type().num_dims(), o);
      o << "}" << EOL;
      
      write_end_loop(decl.type().num_dims(), indent, o);
    }
  }
}
#endif
