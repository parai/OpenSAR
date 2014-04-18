/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:339  */

#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>

#include "runtime.h"
#include  "analizator.h"
#include <typeinfo>

void yyerror(const char *str)
{
        fprintf(stderr,"Parse error\tline: %d, %s in %s\n", yylineno, str, yytext);
}
 

#line 83 "bison.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "bison.tab.h".  */
#ifndef YY_YY_BISON_TAB_H_INCLUDED
# define YY_YY_BISON_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FUNCTION = 258,
    ARRAY_DECLARATION = 259,
    IDENTIFIER = 260,
    TK_INTEGER = 261,
    TK_FLOATPOINT = 262,
    STRING_DEFINITION = 263,
    MORE_OR_EQUAL = 264,
    LESS_OR_EQUAL = 265,
    NOT_EQUAL = 266,
    EQUAL = 267,
    RETURN = 268,
    INCLUDE = 269,
    REQUIRE = 270,
    IF = 271,
    ELSE = 272,
    TRUE = 273,
    FALSE = 274,
    INC = 275,
    DEC = 276,
    WHILE = 277,
    FOR = 278,
    BREAK = 279,
    UNSET = 280
  };
#endif

/* Value type.  */


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_BISON_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 155 "bison.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  63
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   274

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  135

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   280

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    37,    43,     2,     2,
      34,    35,    28,    26,    36,    27,    30,    29,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    31,
      41,    40,    42,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    39,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    32,     2,    33,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    79,    79,    88,    90,    97,    99,   101,   103,   107,
     109,   113,   115,   119,   123,   128,   130,   132,   134,   136,
     140,   142,   144,   148,   151,   158,   165,   170,   177,   181,
     189,   194,   196,   198,   200,   202,   204,   206,   210,   214,
     216,   218,   220,   223,   227,   229,   234,   237,   242,   244,
     248,   251,   257,   262,   266,   268,   271,   273,   275,   277,
     279,   281,   283,   285,   287,   292,   294,   296,   300,   302,
     304,   306,   311,   313,   317,   320
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FUNCTION", "ARRAY_DECLARATION",
  "IDENTIFIER", "TK_INTEGER", "TK_FLOATPOINT", "STRING_DEFINITION",
  "MORE_OR_EQUAL", "LESS_OR_EQUAL", "NOT_EQUAL", "EQUAL", "RETURN",
  "INCLUDE", "REQUIRE", "IF", "ELSE", "TRUE", "FALSE", "INC", "DEC",
  "WHILE", "FOR", "BREAK", "UNSET", "'+'", "'-'", "'*'", "'/'", "'.'",
  "';'", "'{'", "'}'", "'('", "')'", "','", "'$'", "'['", "']'", "'='",
  "'<'", "'>'", "'%'", "$accept", "program", "body", "top_level_cmd",
  "instructions", "instruction", "command", "condition_statement", "block",
  "function_declaration", "function_declaration_arguments",
  "function_call_arguments", "value", "explicit_value", "break", "unar_op",
  "array", "var", "unset", "function_call", "assign_value", "return_value",
  "loop_for", "loop_while", "if_stmt", "else_stmt", "expresion",
  "math_expr", "math_hight_expr", "string_expr", "include", "require", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,    43,    45,    42,    47,
      46,    59,   123,   125,    40,    41,    44,    36,    91,    93,
      61,    60,    62,    37
};
# endif

#define YYPACT_NINF -77

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-77)))

#define YYTABLE_NINF -9

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     121,   -28,    12,    -3,   204,   204,   204,     2,   -17,   -17,
       5,    20,   -77,   -17,    17,    29,    27,   -77,    79,   -77,
       6,   -77,   -77,   -77,   -77,   -14,   -77,   -77,   -77,   -77,
     -77,   -77,   -77,    15,    22,   171,   -77,    21,   204,   -77,
     -77,   -77,   -77,   -77,   -77,   204,   -77,   -77,   -77,   -77,
     144,   -12,    16,    26,   -77,   -77,   204,   -77,   -77,   204,
     215,   -77,    23,   -77,   -77,   -77,   -77,   -77,   -77,   -77,
     196,   -77,   -77,   146,   -17,    34,    36,    56,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
      35,    46,    31,   204,   -77,   -77,   -77,   -77,    48,    54,
     -77,   204,   -77,   -12,   -12,   -12,   -12,   -12,   -12,    16,
      16,    26,    26,    26,   -77,   -28,   -28,   204,    57,   -28,
     -17,   -77,    41,   -77,    76,   -77,   -77,   -77,   -28,   -77,
     215,   -77,    70,   -28,   -77
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    19,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    38,     0,     0,     0,     0,     3,     0,    10,
       0,    12,     5,    18,    13,     0,    17,    14,    15,    16,
      20,    21,    22,     0,     0,     0,    25,     0,     0,    64,
      33,    34,    35,    31,    32,     0,    50,    72,    36,    37,
      30,    56,    65,    68,    74,    75,     0,    42,    41,     0,
       0,    46,    44,     1,     4,    19,     9,    11,    39,    40,
       0,     6,     7,     0,     0,     0,    29,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    43,    48,    49,    23,     0,    27,
      47,     0,    63,    60,    61,    62,    59,    57,    58,    66,
      67,    69,    70,    71,    73,     0,     0,     0,     0,     0,
       0,    28,    54,    52,     0,    45,    24,    26,     0,    53,
       0,    55,     0,     0,    51
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -77,   -77,   -77,    90,    73,   -16,   -59,   -77,    -5,   -77,
     -11,    14,    19,    24,   -77,   -77,   -77,     0,   -77,     3,
     -77,   -77,   -77,   -77,   -77,   -77,   -40,   168,   -51,   -76,
     -77,   -77
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    15,    16,    17,    18,    19,    20,    21,    36,    22,
      98,    75,    76,    47,    23,    24,    96,    48,    26,    49,
      28,    29,    30,    31,    32,   129,    50,    51,    52,    53,
      33,    34
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      25,    92,    66,    27,    35,    77,    68,    69,    57,    58,
     111,   112,   113,    61,    84,    85,    25,    37,    25,    27,
      14,    27,    62,    46,    54,    55,    70,    -2,     1,    63,
       2,    38,     3,   109,   110,    25,    56,    67,    27,    59,
       4,     5,     6,     7,    86,    87,    71,     8,     9,    10,
      11,    12,    13,    72,    60,    74,    89,    66,   128,    88,
      25,    93,   117,    27,    14,    78,    79,    80,    81,   100,
     115,   132,   101,    25,    99,    90,    27,   124,    91,    -8,
      65,   116,    -8,   119,     3,    78,    79,    80,    81,    95,
     120,   102,     4,    -8,    -8,     7,   125,    82,    83,     8,
       9,    10,    11,    12,    13,   133,    64,   130,    73,   127,
     122,   123,   118,   114,   126,   121,    14,    82,    83,     0,
      99,     0,     1,   131,     2,     0,     3,     0,   134,     0,
      25,     0,     0,    27,     4,     5,     6,     7,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    65,     0,     0,
       0,     3,     0,    78,    79,    80,    81,     0,    14,     4,
       0,     0,     7,     0,     0,     0,     8,     9,    10,    11,
      12,    13,    65,     0,     0,     0,     3,     0,     0,    97,
       0,     0,     0,    14,     4,    82,    83,     7,     0,     0,
       0,     8,     9,    10,    11,    12,    13,    39,     0,     0,
      94,     3,    40,    41,    42,    39,     0,     0,    14,     3,
      40,    41,    42,     0,    43,    44,    65,     0,     0,     0,
       3,     0,    43,    44,     0,     0,     0,     0,     4,     0,
      45,     0,     0,    14,     0,     8,     9,     0,    45,    12,
      13,    14,     3,    40,    41,    42,   103,   104,   105,   106,
     107,   108,    14,     0,     0,    43,    44,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    14
};

static const yytype_int16 yycheck[] =
{
       0,    60,    18,     0,    32,    45,    20,    21,     8,     9,
      86,    87,    88,    13,    26,    27,    16,     5,    18,    16,
      37,    18,     5,     4,     5,     6,    40,     0,     1,     0,
       3,    34,     5,    84,    85,    35,    34,    31,    35,    34,
      13,    14,    15,    16,    28,    29,    31,    20,    21,    22,
      23,    24,    25,    31,    34,    34,    30,    73,    17,    43,
      60,    38,    31,    60,    37,     9,    10,    11,    12,    35,
      35,   130,    36,    73,    74,    56,    73,   117,    59,     0,
       1,    35,     3,    35,     5,     9,    10,    11,    12,    70,
      36,    35,    13,    14,    15,    16,    39,    41,    42,    20,
      21,    22,    23,    24,    25,    35,    16,    31,    35,   120,
     115,   116,    93,    89,   119,   101,    37,    41,    42,    -1,
     120,    -1,     1,   128,     3,    -1,     5,    -1,   133,    -1,
     130,    -1,    -1,   130,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,     1,    -1,    -1,
      -1,     5,    -1,     9,    10,    11,    12,    -1,    37,    13,
      -1,    -1,    16,    -1,    -1,    -1,    20,    21,    22,    23,
      24,    25,     1,    -1,    -1,    -1,     5,    -1,    -1,    33,
      -1,    -1,    -1,    37,    13,    41,    42,    16,    -1,    -1,
      -1,    20,    21,    22,    23,    24,    25,     1,    -1,    -1,
       4,     5,     6,     7,     8,     1,    -1,    -1,    37,     5,
       6,     7,     8,    -1,    18,    19,     1,    -1,    -1,    -1,
       5,    -1,    18,    19,    -1,    -1,    -1,    -1,    13,    -1,
      34,    -1,    -1,    37,    -1,    20,    21,    -1,    34,    24,
      25,    37,     5,     6,     7,     8,    78,    79,    80,    81,
      82,    83,    37,    -1,    -1,    18,    19,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    37
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     3,     5,    13,    14,    15,    16,    20,    21,
      22,    23,    24,    25,    37,    45,    46,    47,    48,    49,
      50,    51,    53,    58,    59,    61,    62,    63,    64,    65,
      66,    67,    68,    74,    75,    32,    52,     5,    34,     1,
       6,     7,     8,    18,    19,    34,    56,    57,    61,    63,
      70,    71,    72,    73,    56,    56,    34,    61,    61,    34,
      34,    61,     5,     0,    47,     1,    49,    31,    20,    21,
      40,    31,    31,    48,    34,    55,    56,    70,     9,    10,
      11,    12,    41,    42,    26,    27,    28,    29,    43,    30,
      56,    56,    50,    38,     4,    56,    60,    33,    54,    61,
      35,    36,    35,    71,    71,    71,    71,    71,    71,    72,
      72,    73,    73,    73,    57,    35,    35,    31,    56,    35,
      36,    55,    52,    52,    70,    39,    52,    54,    17,    69,
      31,    52,    50,    35,    52
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    46,    46,    47,    47,    47,    47,    48,
      48,    49,    49,    50,    50,    50,    50,    50,    50,    50,
      51,    51,    51,    52,    53,    53,    54,    54,    55,    55,
      56,    57,    57,    57,    57,    57,    57,    57,    58,    59,
      59,    59,    59,    60,    61,    61,    62,    63,    64,    64,
      65,    66,    67,    68,    69,    69,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    71,    71,    71,    72,    72,
      72,    72,    73,    73,    74,    75
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     2,     2,     1,     2,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     6,     2,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     1,     2,     5,     2,     4,     3,     3,
       2,     9,     5,     6,     0,     2,     1,     3,     3,     3,
       3,     3,     3,     3,     1,     1,     3,     3,     1,     3,
       3,     3,     1,     3,     2,     2
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 80 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
   runtime_t::get_instance()->set_main_block((block_t*) (yyvsp[0].oper));
   runtime_t::get_instance()->semantic_analys();
   runtime_t::get_instance()->interpretate();
  }
#line 1362 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 90 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {    
      (yyval.oper) = new block_t((yyvsp[-1].oper), (yyvsp[0].oper));
  }
#line 1370 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 103 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = (yyvsp[0].oper); }
#line 1376 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 107 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new block_t((yyvsp[-1].oper), (yyvsp[0].oper)); }
#line 1382 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 109 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { }
#line 1388 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 113 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = (yyvsp[-1].oper); }
#line 1394 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 115 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = (yyvsp[0].oper); }
#line 1400 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 119 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    (yyval.oper) =  (oper_t*)((yyvsp[0].expr)); 
  }
#line 1408 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 123 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    /* Function call is expr_t and oper_t child */
      (yyval.oper) =  (oper_t*)((yyvsp[0].expr)); 
    }
#line 1417 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 136 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { yyerrok; yyclearin;  std::cerr <<"Error in command running statement\n"; }
#line 1423 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 148 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new block_t((yyvsp[-1].oper)); }
#line 1429 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 152 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    function_declaration_t* fd = new function_declaration_t((yyvsp[-4].str), (yyvsp[-2].args), (yyvsp[0].oper));
    (yyval.oper) = fd;
    runtime_t::get_instance()->add_function_declaration(fd);
  }
#line 1439 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 158 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    yyerrok;
    yyclearin;
    std::cerr << "Function declaration error!\n";
  }
#line 1449 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 165 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    (yyval.args) = std::list<expr_t*>((yyvsp[0].args));
    (yyval.args).push_back((yyvsp[-2].expr));
  }
#line 1458 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 170 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {  
     (yyval.args) = std::list<expr_t*>();
     (yyval.args).push_back((yyvsp[0].expr));
  }
#line 1467 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 177 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    (yyval.args) = std::list<expr_t*>((yyvsp[0].args));
    (yyval.args).push_back((yyvsp[-2].expr));
  }
#line 1476 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 182 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {  
     (yyval.args) = std::list<expr_t*>();
     (yyval.args).push_back((yyvsp[0].expr));
  }
#line 1485 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 189 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1491 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 194 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new value_t((yyvsp[0].str)); }
#line 1497 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 196 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new value_t((yyvsp[0].str)); }
#line 1503 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 198 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new value_t((yyvsp[0].str)); }
#line 1509 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 200 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new value_t((yyvsp[0].str)); }
#line 1515 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 202 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new value_t((yyvsp[0].str)); }
#line 1521 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 210 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new break_op_t(); }
#line 1527 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 214 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new unary_t("++", (yyvsp[-1].expr)); }
#line 1533 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 216 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new unary_t("--", (yyvsp[-1].expr)); }
#line 1539 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 218 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new unary_t("--", (yyvsp[0].expr)); }
#line 1545 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 220 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new unary_t("++", (yyvsp[0].expr)); }
#line 1551 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 223 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new array_init_t(); }
#line 1557 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 227 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new var_t((yyvsp[0].str)); }
#line 1563 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 229 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { 
    (yyval.expr) = new array_t((yyvsp[-3].str), (yyvsp[-1].expr)); 
  }
#line 1571 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 234 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new unset_t((yyvsp[0].expr)); }
#line 1577 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 237 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    (yyval.expr) = new function_call_t((yyvsp[-3].str), (yyvsp[-1].args));
  }
#line 1585 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 242 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new assign_t((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1591 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 244 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new assign_t((yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1597 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 248 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new return_op_t((yyvsp[0].expr)); }
#line 1603 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 252 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    (yyval.oper) = new for_op_t((yyvsp[-6].oper), (yyvsp[-4].expr), (yyvsp[-2].oper), (yyvsp[0].oper));
  }
#line 1611 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 257 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    {
    (yyval.oper) = new while_op_t((yyvsp[-2].expr), (yyvsp[0].oper)); 
  }
#line 1619 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 262 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new if_op_t((yyvsp[-3].expr), (yyvsp[-1].oper), (yyvsp[0].oper)); }
#line 1625 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 266 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = 0x0; }
#line 1631 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 268 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new block_t((yyvsp[0].oper)); }
#line 1637 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 271 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[0].expr); }
#line 1643 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 273 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("<", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1649 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 275 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t(">", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1655 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 277 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("==", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1661 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 279 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t(">=", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1667 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 281 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("<=", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1673 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 283 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("!=", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1679 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 285 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 1685 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 287 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { std::cerr << "Expresion error\n"; }
#line 1691 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 294 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("+", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1697 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 296 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("-", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1703 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 302 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("*", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1709 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 304 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("/", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1715 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 306 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t("%", (yyvsp[-2].expr), (yyvsp[0].expr)); }
#line 1721 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 313 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.expr) = new binary_t(".", (yyvsp[-2].expr), (yyvsp[0].expr)); /* Yes, I change it, to make it more look like class string */}
#line 1727 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 317 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new include_t((yyvsp[0].expr)); }
#line 1733 "bison.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 320 "../../tools/arvfb/source/smalvl/bison.y" /* yacc.c:1646  */
    { (yyval.oper) = new require_t((yyvsp[0].expr)); }
#line 1739 "bison.tab.c" /* yacc.c:1646  */
    break;


#line 1743 "bison.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
