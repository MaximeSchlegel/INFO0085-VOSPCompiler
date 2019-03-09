// A Bison parser, made by GNU Bison 3.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// Undocumented macros, especially those whose name start with YY_,
// are private implementation details.  Do not rely on them.





#include "vsopc.tab.hh"




#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Suppress unused-variable warnings by "using" E.
#define YYUSE(E) ((void) (E))

// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yystack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YYUSE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)


namespace yy {
#line 119 "vsopc.tab.cc" // lalr1.cc:512

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
     :yydebug_ (false),
      yycdebug_ (&std::cerr)
#endif
  {}

  parser::~parser ()
  {}


  /*---------------.
  | Symbol types.  |
  `---------------*/



  // by_state.
  parser::by_state::by_state ()
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& other)
    : state (other.state)
  {}

  void
  parser::by_state::clear ()
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s)
    : state (s)
  {}

  parser::symbol_number_type
  parser::by_state::type_get () const
  {
    if (state == empty_state)
      return empty_symbol;
    else
      return yystos_[state];
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.type_get ())
    {
      default:
        break;
    }

#if defined __cplusplus && 201103L <= __cplusplus
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.type_get ())
    {
      default:
        break;
    }

    // that is emptied.
    that.type = empty_symbol;
  }

#if defined __cplusplus && __cplusplus < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.type_get ())
    {
      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo,
                                     const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YYUSE (yyoutput);
    symbol_number_type yytype = yysym.type_get ();
    // Avoid a (spurious) G++ 4.8 warning about "array subscript is
    // below array bounds".
    if (yysym.empty ())
      std::abort ();
    yyo << (yytype < yyntokens_ ? "token" : "nterm")
        << ' ' << yytname_[yytype] << " (";
    YYUSE (yytype);
    yyo << ')';
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if defined __cplusplus && 201103L <= __cplusplus
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n)
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - yyntokens_] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - yyntokens_];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue)
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue)
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    // State.
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

    // A new symbol was pushed on the stack.
  yynewstate:
    YYCDEBUG << "Entering state " << yystack_[0].state << '\n';

    // Accept?
    if (yystack_[0].state == yyfinal_)
      goto yyacceptlab;

    goto yybackup;

    // Backup.
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token: ";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.type_get ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.type_get ())
      goto yydefault;

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", yyn, YY_MOVE (yyla));
    goto yynewstate;

  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;

  /*-----------------------------.
  | yyreduce -- Do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {

#line 461 "vsopc.tab.cc" // lalr1.cc:906
            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;
      YY_STACK_PRINT ();

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;

  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        error (yysyntax_error_ (yystack_[0].state, yyla));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.type_get () == yyeof_)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:

    /* Pacify compilers like GCC when the user code never invokes
       YYERROR and the label yyerrorlab therefore never appears in user
       code.  */
    if (false)
      goto yyerrorlab;
    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    goto yyerrlab1;

  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    {
      stack_symbol_type error_token;
      for (;;)
        {
          yyn = yypact_[yystack_[0].state];
          if (!yy_pact_value_is_default_ (yyn))
            {
              yyn += yyterror_;
              if (0 <= yyn && yyn <= yylast_ && yycheck_[yyn] == yyterror_)
                {
                  yyn = yytable_[yyn];
                  if (0 < yyn)
                    break;
                }
            }

          // Pop the current state because it cannot handle the error token.
          if (yystack_.size () == 1)
            YYABORT;

          yy_destroy_ ("Error: popping", yystack_[0]);
          yypop_ ();
          YY_STACK_PRINT ();
        }


      // Shift the error token.
      error_token.state = yyn;
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;

    // Accept.
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;

    // Abort.
  yyabortlab:
    yyresult = 1;
    goto yyreturn;

  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (state_type, const symbol_type&) const
  {
    return YY_("syntax error");
  }


  const signed char parser::yypact_ninf_ = -34;

  const signed char parser::yytable_ninf_ = -1;

  const short
  parser::yypact_[] =
  {
     -10,     6,     3,   -34,   -14,   -34,   -34,    12,   -34,   -16,
      28,   -34,    48,   -34,   -34,   -34,    59,     8,    24,   -34,
      15,    -8,   -34,   -34,   -34,   -34,   -34,   -34,   -22,    24,
      26,     8,   -34,   245,   -34,    24,   -34,   -34,   -34,   -34,
     -34,   245,   245,   245,    34,    42,   245,   -26,   245,   179,
     245,   -34,    27,    46,   145,    58,   145,    47,   -34,    67,
     201,   245,    -9,   145,   -34,    97,   145,   245,   -34,   245,
     245,   245,   245,   245,    55,   245,   245,   245,   245,   245,
      24,   245,   -34,   145,    19,   145,   -34,   245,   -34,   145,
     145,   145,   145,   145,   145,    57,   145,   145,   145,    16,
     106,   -19,   145,   -34,   245,   145,   223,   -34,   245,   245,
     245,   145,   -34,    25,   145,   145,   136,   -34,   245,   145
  };

  const unsigned char
  parser::yydefact_[] =
  {
       0,     0,     0,    11,     0,     1,    12,     0,    15,     0,
       0,    15,     0,    13,    16,    17,     0,     0,     0,    14,
       0,     0,    19,     2,     3,     4,     5,     6,     0,     0,
       0,     0,    24,     0,    21,     0,    20,     7,     8,     9,
      10,     0,     0,     0,     0,     0,     0,    49,     0,     0,
       0,    50,     0,     0,    32,     0,    43,     0,    48,     0,
       0,     0,     0,    22,    51,     0,    42,     0,    25,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    44,    54,     0,    31,    53,     0,    52,    33,
      37,    38,    39,    40,    41,     0,    34,    35,    36,     0,
      26,     0,    28,    45,     0,    23,     0,    18,     0,     0,
       0,    55,    46,     0,    27,    29,     0,    47,     0,    30
  };

  const signed char
  parser::yypgoto_[] =
  {
     -34,   -28,   -34,   -34,    87,    79,   -34,   -34,    60,    14,
     -34,   -33,     7
  };

  const signed char
  parser::yydefgoto_[] =
  {
      -1,    28,    51,     2,     3,    10,    14,    21,    22,    62,
      15,    63,    84
  };

  const unsigned char
  parser::yytable_[] =
  {
      52,    34,   109,     5,    60,     7,     1,    53,    54,    55,
      56,    32,    11,    59,     8,     4,    65,    66,    61,     1,
      86,     9,    33,    30,    87,   110,    31,    83,    85,    23,
      24,    25,    26,    27,    89,    20,    90,    91,    92,    93,
      94,    67,    96,    97,    98,   107,   100,    29,   102,    87,
     103,    58,   101,   104,   105,    12,   117,    13,    35,   104,
      68,    57,    69,    70,    71,    72,    73,    74,    75,    76,
      77,   111,    67,    83,    78,   114,   115,   116,    17,    80,
      18,    67,    95,    79,    81,   119,    12,   106,    19,     6,
      16,    36,    99,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    67,     0,   113,     0,     0,     0,     0,     0,     0,
      67,     0,     0,     0,   108,     0,     0,     0,    88,     0,
       0,     0,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      67,     0,     0,     0,     0,     0,     0,   118,     0,    67,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      69,    70,    71,    72,    73,    74,    75,    76,    77,    37,
      38,    39,    40,     0,    41,     0,     0,     0,     0,    42,
       0,    43,    44,    45,     0,    46,    47,    48,     0,    49,
      64,    37,    38,    39,    40,    50,    41,     0,     0,     0,
       0,    42,     0,    43,    44,    45,     0,    46,    47,    48,
       0,    49,    82,    37,    38,    39,    40,    50,    41,     0,
       0,     0,     0,    42,     0,    43,    44,    45,     0,    46,
      47,    48,     0,    49,   112,    37,    38,    39,    40,    50,
      41,     0,     0,     0,     0,    42,     0,    43,    44,    45,
       0,    46,    47,    48,     0,    49,     0,     0,     0,     0,
       0,    50
  };

  const signed char
  parser::yycheck_[] =
  {
      33,    29,    21,     0,    30,    19,    16,    35,    41,    42,
      43,    33,    28,    46,    28,     9,    49,    50,    44,    16,
      29,     9,    44,    31,    33,    44,    34,    60,    61,     5,
       6,     7,     8,     9,    67,    27,    69,    70,    71,    72,
      73,    14,    75,    76,    77,    29,    79,    32,    81,    33,
      31,     9,    80,    34,    87,    27,    31,    29,    32,    34,
      33,    27,    35,    36,    37,    38,    39,    40,    41,    42,
      43,   104,    14,   106,    28,   108,   109,   110,    30,    32,
      32,    14,    27,    25,    17,   118,    27,    30,    29,     2,
      11,    31,    78,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    14,    -1,   106,    -1,    -1,    -1,    -1,    -1,    -1,
      14,    -1,    -1,    -1,    18,    -1,    -1,    -1,    31,    -1,
      -1,    -1,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      14,    -1,    -1,    -1,    -1,    -1,    -1,    21,    -1,    14,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    10,
      11,    12,    13,    -1,    15,    -1,    -1,    -1,    -1,    20,
      -1,    22,    23,    24,    -1,    26,    27,    28,    -1,    30,
      31,    10,    11,    12,    13,    36,    15,    -1,    -1,    -1,
      -1,    20,    -1,    22,    23,    24,    -1,    26,    27,    28,
      -1,    30,    31,    10,    11,    12,    13,    36,    15,    -1,
      -1,    -1,    -1,    20,    -1,    22,    23,    24,    -1,    26,
      27,    28,    -1,    30,    31,    10,    11,    12,    13,    36,
      15,    -1,    -1,    -1,    -1,    20,    -1,    22,    23,    24,
      -1,    26,    27,    28,    -1,    30,    -1,    -1,    -1,    -1,
      -1,    36
  };

  const unsigned char
  parser::yystos_[] =
  {
       0,    16,    48,    49,     9,     0,    49,    19,    28,     9,
      50,    28,    27,    29,    51,    55,    50,    30,    32,    29,
      27,    52,    53,     5,     6,     7,     8,     9,    46,    32,
      31,    34,    33,    44,    46,    32,    53,    10,    11,    12,
      13,    15,    20,    22,    23,    24,    26,    27,    28,    30,
      36,    47,    56,    46,    56,    56,    56,    27,     9,    56,
      30,    44,    54,    56,    31,    56,    56,    14,    33,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    28,    25,
      32,    17,    31,    56,    57,    56,    29,    33,    31,    56,
      56,    56,    56,    56,    56,    27,    56,    56,    56,    54,
      56,    46,    56,    31,    34,    56,    30,    29,    18,    21,
      44,    56,    31,    57,    56,    56,    56,    31,    21,    56
  };

  const unsigned char
  parser::yyr1_[] =
  {
       0,    45,    46,    46,    46,    46,    46,    47,    47,    47,
      47,    48,    48,    49,    49,    50,    50,    50,    51,    52,
      52,    53,    54,    54,    55,    55,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    56,    56,    56,    56,    56,    56,
      56,    56,    56,    56,    57,    57
  };

  const unsigned char
  parser::yyr2_[] =
  {
       0,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     2,     5,     7,     0,     2,     2,     9,     1,
       3,     3,     1,     3,     4,     6,     4,     6,     4,     6,
       8,     3,     2,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     3,     4,     5,     6,     2,     1,
       1,     2,     3,     3,     1,     3
  };


#if YYDEBUG
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a yyntokens_, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "$end", "error", "$undefined", "ERROR", "\"end-of-file\"", "BOOL",
  "INT32", "STRING", "UNIT", "TYPEID", "TRUE", "FALSE", "INTLITERAL",
  "STRLITERAL", "AND", "NOT", "CLASS", "DO", "ELSE", "EXTENDS", "IF", "IN",
  "ISNULL", "LET", "NEW", "THEN", "WHILE", "OBJECTID", "LBRACE", "RBRACE",
  "LPAR", "RPAR", "COLON", "SEMICOLON", "COMMA", "PLUS", "MINUS", "TIMES",
  "DIV", "POW", "DOT", "EQUAL", "LOWER", "LOWEREQUAL", "ASSIGN", "$accept",
  "type", "literal", "program", "class", "class_body", "method", "formals",
  "formal", "block", "field", "expr", "args", YY_NULLPTR
  };


  const unsigned char
  parser::yyrline_[] =
  {
       0,    60,    60,    61,    62,    63,    64,    67,    68,    69,
      70,    73,    74,    77,    78,    80,    82,    83,    87,    90,
      91,    94,    97,    98,   101,   102,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   135,   136
  };

  // Print the state stack on the debug stream.
  void
  parser::yystack_print_ ()
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << i->state;
    *yycdebug_ << '\n';
  }

  // Report on the debug stream that the rule \a yyrule is going to be reduced.
  void
  parser::yy_reduce_print_ (int yyrule)
  {
    unsigned yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG



} // yy
#line 845 "vsopc.tab.cc" // lalr1.cc:1217
#line 140 "parser/vsopc.yy" // lalr1.cc:1218
