/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 27 "surface.yy" /* yacc.c:339  */

#include <string>
#include <stack>

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	core class definitions
#include "lineNumber.hh"

//	front end class definitions
#include "token.hh"
#include "renaming.hh"
#include "moduleExpression.hh"
#include "viewExpression.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "syntacticPreModule.hh"
#include "visibleModule.hh"
#include "syntacticView.hh"
#include "userLevelRewritingContext.hh"
#include "interpreter.hh"

#include "global.hh"
#define clear()			tokenSequence.clear();
#define store(token)		tokenSequence.append(token)
#define fragClear()		fragments.contractTo(0);
#define fragStore(token)	fragments.append(token)
//#define YYPARSE_PARAM	parseResult
//#define PARSE_RESULT	(*((UserLevelRewritingContext::ParseResult*) parseResult))
#define PARSE_RESULT	(*parseResult)

#define CM		interpreter.getCurrentModule()
#define CV		interpreter.getCurrentView()

#include "lexerAux.hh"

Vector<Token> singleton(1);
Vector<Token> tokenSequence;
Vector<Token> lexerBubble;
Vector<Token> fragments;
Vector<Token> moduleExpr;
Vector<Token> opDescription;
Vector<Token> strategyCall;

Renaming* currentRenaming = 0;
SyntaxContainer* currentSyntaxContainer = 0;
SyntaxContainer* oldSyntaxContainer = 0;

Int64 number;
Int64 number2;

static void yyerror(UserLevelRewritingContext::ParseResult* parseResult, char *s);

void cleanUpModuleExpression();
void cleanUpParser();
void deepSelfDestructViewExpressionVector(Vector<ViewExpression*>* viewExpressions);
void missingSpace(const Token& token);

#line 134 "surface.c" /* yacc.c:339  */

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
   by #include "surface.h".  */
#ifndef YY_YY_SURFACE_H_INCLUDED
# define YY_YY_SURFACE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    FILE_NAME_STRING = 258,
    UNINTERPRETED_STRING = 259,
    LATEX_STRING = 260,
    KW_MOD = 261,
    KW_OMOD = 262,
    KW_VIEW = 263,
    KW_PARSE = 264,
    KW_NORMALIZE = 265,
    KW_REDUCE = 266,
    KW_REWRITE = 267,
    KW_LOOP = 268,
    KW_NARROW = 269,
    KW_XG_NARROW = 270,
    KW_MATCH = 271,
    KW_XMATCH = 272,
    KW_UNIFY = 273,
    KW_CHECK = 274,
    KW_GET = 275,
    KW_VARIANTS = 276,
    KW_VARIANT = 277,
    KW_EREWRITE = 278,
    KW_FREWRITE = 279,
    KW_SREWRITE = 280,
    KW_DSREWRITE = 281,
    KW_CONTINUE = 282,
    KW_SEARCH = 283,
    KW_SET = 284,
    KW_SHOW = 285,
    KW_ON = 286,
    KW_OFF = 287,
    KW_TRACE = 288,
    KW_BODY = 289,
    KW_BUILTIN = 290,
    KW_WHOLE = 291,
    KW_SELECT = 292,
    KW_DESELECT = 293,
    KW_CONDITION = 294,
    KW_SUBSTITUTION = 295,
    KW_PRINT = 296,
    KW_GRAPH = 297,
    KW_MIXFIX = 298,
    KW_FLAT = 299,
    KW_ATTRIBUTE = 300,
    KW_NEWLINE = 301,
    KW_WITH = 302,
    KW_PARENS = 303,
    KW_ALIASES = 304,
    KW_GC = 305,
    KW_TIME = 306,
    KW_STATS = 307,
    KW_TIMING = 308,
    KW_CMD = 309,
    KW_BREAKDOWN = 310,
    KW_BREAK = 311,
    KW_PATH = 312,
    KW_CONST = 313,
    KW_MODULE = 314,
    KW_MODULES = 315,
    KW_VIEWS = 316,
    KW_ALL = 317,
    KW_SORTS = 318,
    KW_OPS2 = 319,
    KW_VARS = 320,
    KW_MBS = 321,
    KW_EQS = 322,
    KW_RLS = 323,
    KW_STRATS = 324,
    KW_SDS = 325,
    KW_SUMMARY = 326,
    KW_KINDS = 327,
    KW_ADVISE = 328,
    KW_VERBOSE = 329,
    KW_DO = 330,
    KW_CLEAR = 331,
    KW_PROTECT = 332,
    KW_EXTEND = 333,
    KW_INCLUDE = 334,
    KW_EXCLUDE = 335,
    KW_CONCEAL = 336,
    KW_REVEAL = 337,
    KW_COMPILE = 338,
    KW_COUNT = 339,
    KW_DEBUG = 340,
    KW_IRREDUNDANT = 341,
    KW_RESUME = 342,
    KW_ABORT = 343,
    KW_STEP = 344,
    KW_WHERE = 345,
    KW_CREDUCE = 346,
    KW_SREDUCE = 347,
    KW_DUMP = 348,
    KW_PROFILE = 349,
    KW_NUMBER = 350,
    KW_RAT = 351,
    KW_COLOR = 352,
    SIMPLE_NUMBER = 353,
    KW_PWD = 354,
    KW_CD = 355,
    KW_PUSHD = 356,
    KW_POPD = 357,
    KW_LS = 358,
    KW_LL = 359,
    KW_LOAD = 360,
    KW_SLOAD = 361,
    KW_QUIT = 362,
    KW_EOF = 363,
    KW_TEST = 364,
    KW_SMT_SEARCH = 365,
    KW_VU_NARROW = 366,
    KW_FVU_NARROW = 367,
    KW_ENDM = 368,
    KW_IMPORT = 369,
    KW_ENDV = 370,
    KW_SORT = 371,
    KW_SUBSORT = 372,
    KW_OP = 373,
    KW_OPS = 374,
    KW_MSGS = 375,
    KW_VAR = 376,
    KW_CLASS = 377,
    KW_SUBCLASS = 378,
    KW_DSTRAT = 379,
    KW_MB = 380,
    KW_CMB = 381,
    KW_EQ = 382,
    KW_CEQ = 383,
    KW_RL = 384,
    KW_CRL = 385,
    KW_SD = 386,
    KW_CSD = 387,
    KW_IS = 388,
    KW_FROM = 389,
    KW_ARROW = 390,
    KW_ARROW2 = 391,
    KW_PARTIAL = 392,
    KW_IF = 393,
    KW_ASSIGN = 394,
    KW_LABEL = 395,
    KW_TO = 396,
    KW_COLON2 = 397,
    KW_ASSOC = 398,
    KW_COMM = 399,
    KW_ID = 400,
    KW_IDEM = 401,
    KW_ITER = 402,
    KW_LEFT = 403,
    KW_RIGHT = 404,
    KW_PREC = 405,
    KW_GATHER = 406,
    KW_METADATA = 407,
    KW_STRAT = 408,
    KW_ASTRAT = 409,
    KW_POLY = 410,
    KW_MEMO = 411,
    KW_FROZEN = 412,
    KW_CTOR = 413,
    KW_LATEX = 414,
    KW_SPECIAL = 415,
    KW_CONFIG = 416,
    KW_OBJ = 417,
    KW_MSG = 418,
    KW_DITTO = 419,
    KW_FORMAT = 420,
    KW_ID_HOOK = 421,
    KW_OP_HOOK = 422,
    KW_TERM_HOOK = 423,
    KW_IN = 424,
    IDENTIFIER = 425,
    NUMERIC_ID = 426,
    ENDS_IN_DOT = 427,
    FORCE_LOOKAHEAD = 428
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 98 "surface.yy" /* yacc.c:355  */

  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;
  ModuleExpression* yyModuleExpression;
  ViewExpression* yyViewExpression;
  Vector<ViewExpression*>* yyViewExpressionVector;

#line 362 "surface.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (UserLevelRewritingContext::ParseResult* parseResult);

#endif /* !YY_YY_SURFACE_H_INCLUDED  */

/* Copy the second part of user declarations.  */
#line 114 "surface.yy" /* yacc.c:358  */

int yylex(YYSTYPE* lvalp);

#line 381 "surface.c" /* yacc.c:358  */

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
#define YYFINAL  117
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2633

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  189
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  270
/* YYNRULES -- Number of rules.  */
#define YYNRULES  670
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1016

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   428

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     142,   143,   154,   153,   148,     2,   144,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   140,     2,
     145,   141,     2,     2,   188,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   146,     2,   147,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   155,   149,   156,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   150,   151,   152,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   247,   247,   249,   254,   255,   256,   257,   263,   263,
     273,   273,   283,   283,   293,   297,   297,   308,   308,   319,
     330,   330,   335,   335,   340,   345,   378,   382,   386,   387,
     388,   389,   393,   402,   403,   409,   410,   413,   414,   415,
     422,   429,   436,   445,   450,   456,   461,   473,   478,   472,
     484,   485,   488,   489,   492,   496,   500,   501,   500,   503,
     503,   510,   510,   512,   515,   516,   522,   522,   524,   527,
     528,   531,   532,   532,   534,   534,   536,   536,   543,   545,
     543,   563,   564,   567,   572,   575,   579,   580,   580,   582,
     583,   586,   587,   597,   605,   596,   614,   613,   645,   650,
     650,   664,   663,   672,   671,   699,   700,   709,   709,   716,
     718,   716,   731,   732,   741,   742,   745,   746,   749,   755,
     756,   764,   773,   774,   777,   777,   780,   781,   784,   789,
     789,   792,   792,   795,   796,   795,   799,   801,   803,   804,
     803,   807,   808,   809,   807,   812,   813,   812,   816,   817,
     818,   816,   821,   822,   821,   825,   826,   827,   825,   830,
     831,   830,   834,   835,   836,   834,   839,   839,   845,   846,
     845,   849,   855,   854,   861,   861,   864,   875,   876,   879,
     880,   883,   888,   889,   892,   893,   896,   897,   900,   901,
     902,   909,   909,   911,   912,   915,   918,   920,   926,   927,
     930,   936,   942,   945,   946,   951,   958,   965,   966,   969,
     970,   973,   974,   977,   982,   982,   989,   990,   993,   994,
     997,   998,  1001,  1005,  1009,  1015,  1019,  1023,  1023,  1025,
    1029,  1033,  1034,  1034,  1036,  1036,  1038,  1038,  1040,  1040,
    1042,  1042,  1044,  1048,  1052,  1057,  1057,  1059,  1063,  1067,
    1071,  1075,  1075,  1077,  1078,  1088,  1089,  1092,  1093,  1096,
    1097,  1100,  1101,  1102,  1103,  1109,  1111,  1117,  1119,  1127,
    1128,  1131,  1132,  1133,  1133,  1140,  1152,  1153,  1153,  1157,
    1157,  1159,  1165,  1165,  1165,  1165,  1165,  1168,  1168,  1168,
    1169,  1169,  1169,  1172,  1172,  1172,  1172,  1173,  1173,  1173,
    1173,  1173,  1173,  1176,  1176,  1176,  1177,  1177,  1177,  1177,
    1178,  1178,  1178,  1178,  1178,  1181,  1181,  1187,  1187,  1187,
    1187,  1187,  1187,  1187,  1187,  1188,  1188,  1191,  1191,  1191,
    1191,  1194,  1194,  1197,  1197,  1197,  1197,  1197,  1197,  1197,
    1198,  1198,  1198,  1199,  1199,  1199,  1199,  1199,  1199,  1199,
    1199,  1200,  1200,  1203,  1203,  1203,  1203,  1203,  1203,  1203,
    1206,  1206,  1206,  1209,  1209,  1209,  1209,  1209,  1209,  1209,
    1210,  1210,  1210,  1210,  1210,  1210,  1211,  1211,  1211,  1211,
    1212,  1212,  1212,  1212,  1213,  1213,  1213,  1240,  1240,  1245,
    1245,  1252,  1251,  1264,  1263,  1276,  1275,  1288,  1287,  1300,
    1299,  1312,  1311,  1325,  1324,  1338,  1337,  1350,  1349,  1363,
    1362,  1375,  1374,  1388,  1387,  1400,  1399,  1412,  1411,  1425,
    1424,  1436,  1441,  1440,  1457,  1456,  1467,  1473,  1473,  1479,
    1479,  1485,  1485,  1491,  1491,  1497,  1505,  1505,  1511,  1511,
    1517,  1517,  1523,  1523,  1529,  1533,  1537,  1537,  1543,  1543,
    1549,  1549,  1555,  1555,  1561,  1561,  1567,  1567,  1573,  1573,
    1579,  1579,  1585,  1585,  1591,  1591,  1597,  1601,  1605,  1609,
    1609,  1618,  1622,  1626,  1630,  1634,  1638,  1642,  1646,  1650,
    1654,  1658,  1662,  1666,  1670,  1671,  1670,  1676,  1677,  1676,
    1682,  1686,  1690,  1694,  1698,  1702,  1709,  1713,  1717,  1721,
    1728,  1732,  1739,  1739,  1746,  1747,  1748,  1749,  1750,  1751,
    1752,  1753,  1754,  1755,  1756,  1759,  1760,  1761,  1762,  1763,
    1764,  1765,  1766,  1767,  1768,  1769,  1770,  1773,  1774,  1777,
    1778,  1781,  1782,  1785,  1786,  1791,  1792,  1793,  1794,  1795,
    1796,  1799,  1800,  1803,  1804,  1807,  1808,  1811,  1812,  1815,
    1816,  1817,  1826,  1827,  1827,  1831,  1831,  1833,  1843,  1843,
    1845,  1846,  1846,  1851,  1851,  1853,  1853,  1855,  1859,  1859,
    1861,  1861,  1863,  1876,  1876,  1878,  1879,  1879,  1888,  1888,
    1890,  1890,  1892,  1892,  1894,  1902,  1902,  1904,  1904,  1906,
    1906,  1908,  1916,  1916,  1918,  1918,  1920,  1929,  1928,  1935,
    1935,  1937,  1945,  1945,  1947,  1947,  1949,  1957,  1957,  1959,
    1959,  1961,  1964,  1964,  1966,  1970,  1970,  1972,  1972,  1974,
    1985,  1985,  1985,  1985,  1985,  1985,  1985,  1988,  1988,  1988,
    1988,  1988,  1988,  1992,  1992,  1992,  1992,  1992,  1992,  1996,
    1996,  1996,  1996,  1996,  1996,  2000,  2000,  2000,  2000,  2000,
    2004,  2004,  2004,  2004,  2004,  2010,  2011,  2014,  2015,  2018,
    2025,  2026,  2029,  2034,  2041,  2041,  2041,  2041,  2041,  2041,
    2041
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FILE_NAME_STRING",
  "UNINTERPRETED_STRING", "LATEX_STRING", "KW_MOD", "KW_OMOD", "KW_VIEW",
  "KW_PARSE", "KW_NORMALIZE", "KW_REDUCE", "KW_REWRITE", "KW_LOOP",
  "KW_NARROW", "KW_XG_NARROW", "KW_MATCH", "KW_XMATCH", "KW_UNIFY",
  "KW_CHECK", "KW_GET", "KW_VARIANTS", "KW_VARIANT", "KW_EREWRITE",
  "KW_FREWRITE", "KW_SREWRITE", "KW_DSREWRITE", "KW_CONTINUE", "KW_SEARCH",
  "KW_SET", "KW_SHOW", "KW_ON", "KW_OFF", "KW_TRACE", "KW_BODY",
  "KW_BUILTIN", "KW_WHOLE", "KW_SELECT", "KW_DESELECT", "KW_CONDITION",
  "KW_SUBSTITUTION", "KW_PRINT", "KW_GRAPH", "KW_MIXFIX", "KW_FLAT",
  "KW_ATTRIBUTE", "KW_NEWLINE", "KW_WITH", "KW_PARENS", "KW_ALIASES",
  "KW_GC", "KW_TIME", "KW_STATS", "KW_TIMING", "KW_CMD", "KW_BREAKDOWN",
  "KW_BREAK", "KW_PATH", "KW_CONST", "KW_MODULE", "KW_MODULES", "KW_VIEWS",
  "KW_ALL", "KW_SORTS", "KW_OPS2", "KW_VARS", "KW_MBS", "KW_EQS", "KW_RLS",
  "KW_STRATS", "KW_SDS", "KW_SUMMARY", "KW_KINDS", "KW_ADVISE",
  "KW_VERBOSE", "KW_DO", "KW_CLEAR", "KW_PROTECT", "KW_EXTEND",
  "KW_INCLUDE", "KW_EXCLUDE", "KW_CONCEAL", "KW_REVEAL", "KW_COMPILE",
  "KW_COUNT", "KW_DEBUG", "KW_IRREDUNDANT", "KW_RESUME", "KW_ABORT",
  "KW_STEP", "KW_WHERE", "KW_CREDUCE", "KW_SREDUCE", "KW_DUMP",
  "KW_PROFILE", "KW_NUMBER", "KW_RAT", "KW_COLOR", "SIMPLE_NUMBER",
  "KW_PWD", "KW_CD", "KW_PUSHD", "KW_POPD", "KW_LS", "KW_LL", "KW_LOAD",
  "KW_SLOAD", "KW_QUIT", "KW_EOF", "KW_TEST", "KW_SMT_SEARCH",
  "KW_VU_NARROW", "KW_FVU_NARROW", "KW_ENDM", "KW_IMPORT", "KW_ENDV",
  "KW_SORT", "KW_SUBSORT", "KW_OP", "KW_OPS", "KW_MSGS", "KW_VAR",
  "KW_CLASS", "KW_SUBCLASS", "KW_DSTRAT", "KW_MB", "KW_CMB", "KW_EQ",
  "KW_CEQ", "KW_RL", "KW_CRL", "KW_SD", "KW_CSD", "KW_IS", "KW_FROM",
  "KW_ARROW", "KW_ARROW2", "KW_PARTIAL", "KW_IF", "KW_ASSIGN", "':'",
  "'='", "'('", "')'", "'.'", "'<'", "'['", "']'", "','", "'|'",
  "KW_LABEL", "KW_TO", "KW_COLON2", "'+'", "'*'", "'{'", "'}'", "KW_ASSOC",
  "KW_COMM", "KW_ID", "KW_IDEM", "KW_ITER", "KW_LEFT", "KW_RIGHT",
  "KW_PREC", "KW_GATHER", "KW_METADATA", "KW_STRAT", "KW_ASTRAT",
  "KW_POLY", "KW_MEMO", "KW_FROZEN", "KW_CTOR", "KW_LATEX", "KW_SPECIAL",
  "KW_CONFIG", "KW_OBJ", "KW_MSG", "KW_DITTO", "KW_FORMAT", "KW_ID_HOOK",
  "KW_OP_HOOK", "KW_TERM_HOOK", "KW_IN", "IDENTIFIER", "NUMERIC_ID",
  "ENDS_IN_DOT", "FORCE_LOOKAHEAD", "'@'", "$accept", "top", "item",
  "directive", "$@1", "$@2", "$@3", "$@4", "$@5", "$@6", "$@7",
  "moduleExprDot", "moduleExpr", "moduleExpr2", "moduleExpr3",
  "renameExpr", "instantExpr", "parenExpr", "viewExpr", "instantArgs",
  "renaming", "$@8", "$@9", "renaming2", "mappingList", "mapping", "$@10",
  "$@11", "$@12", "fromSpec", "$@13", "fromStratSpec", "toAttributes",
  "$@14", "toAttributeList", "toAttribute", "$@15", "$@16", "$@17", "view",
  "$@18", "$@19", "viewDecList", "skipStrayArrow", "viewDeclaration",
  "$@20", "sortDot", "viewEndOpMap", "$@21", "$@22", "$@23",
  "strategyCall", "$@24", "viewStratMap", "$@25", "$@26", "endBubble",
  "parenBubble", "$@27", "module", "$@28", "$@29", "dot", "parameters",
  "parameterList", "parameter", "colon2", "badType", "typeDot",
  "startModule", "decList", "declaration", "$@30", "$@31", "$@32", "$@33",
  "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42",
  "$@43", "$@44", "$@45", "$@46", "$@47", "$@48", "$@49", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58", "classDef",
  "cPairList", "cPair", "varNameList", "opNameList", "simpleOpName",
  "domainRangeAttr", "stratDeclKeyword", "stratIdList", "stratId",
  "stratSignature", "stratAttributes", "stratAttrList", "skipStrayColon",
  "dra2", "rangeAttr", "typeAttr", "arrow", "typeList", "typeName", "$@59",
  "sortNames", "attributes", "attributeList", "idKeyword", "attribute",
  "$@60", "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67",
  "identity", "idList", "hookList", "hook", "expectedIs", "expectedDot",
  "sortNameList", "subsortList", "$@68", "sortName", "sortNameFrag",
  "$@69", "sortNameFrags", "$@70", "token", "tokenBarDot", "tokenBarColon",
  "sortToken", "endsInDot", "inert", "identifier", "startKeyword",
  "startKeyword2", "midKeyword", "attrKeyword", "attrKeyword2", "command",
  "$@71", "$@72", "$@73", "$@74", "$@75", "$@76", "$@77", "$@78", "$@79",
  "$@80", "$@81", "$@82", "$@83", "$@84", "$@85", "$@86", "$@87", "$@88",
  "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95", "$@96", "$@97",
  "$@98", "$@99", "$@100", "$@101", "$@102", "$@103", "$@104", "$@105",
  "$@106", "$@107", "$@108", "$@109", "$@110", "$@111", "$@112", "$@113",
  "printOption", "traceOption", "polarity", "select", "exclude", "conceal",
  "search", "match", "optDebug", "optIrredundant", "optNumber",
  "importMode", "moduleAndTerm", "$@114", "inEnd", "$@115",
  "numberModuleTerm", "$@116", "$@117", "numberModuleTerm1", "$@118",
  "$@119", "numberModuleTerm2", "$@120", "$@121", "numbersModuleTerm",
  "$@122", "$@123", "numbersModuleTerm1", "$@124", "$@125", "$@126",
  "numbersModuleTerm2", "$@127", "$@128", "$@129", "numbersModuleTerm3",
  "$@130", "$@131", "numbersModuleTerm4", "$@132", "$@133",
  "numbersModuleTerm5", "$@134", "$@135", "numbersModuleTerm6", "$@136",
  "$@137", "miscEndBubble", "$@138", "initialEndBubble", "$@139", "$@140",
  "cTokenBarIn", "cTokenBarLeftIn", "cTokenBarDotNumber",
  "cTokenBarDotRight", "cTokenBarDotCommaNumber", "cTokenBarDotCommaRight",
  "opSelect", "endSelect", "badSelect", "cOpNameList", "cSimpleOpName",
  "cSimpleTokenBarDot", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
      58,    61,    40,    41,    46,    60,    91,    93,    44,   124,
     395,   396,   397,    43,    42,   123,   125,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,    64
};
# endif

#define YYPACT_NINF -795

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-795)))

#define YYTABLE_NINF -545

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     834,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,   337,   463,    37,  -795,   196,   253,    15,  -795,   -33,
       9,    31,    41,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,   232,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,   243,   117,
    1237,   315,   315,   545,   315,   193,   106,   440,     2,   252,
     292,   292,   292,   -16,  -795,  -795,  -795,   204,   292,  -795,
    -795,  -795,   256,   -44,  -795,   149,   156,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,   -82,  -795,  -795,  -795,  -795,
     133,  -795,  -795,  -795,  -795,   315,   315,   -82,   306,   312,
     314,   317,   329,   338,   315,   200,   342,  -795,  1237,   545,
    -795,  -795,  -795,  -795,   262,   348,  -795,  -795,  -795,  -795,
     275,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,   304,
     292,   292,   292,   292,   292,   292,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,   292,  -795,  -795,
    -795,   207,   335,   313,  -795,  -795,  -795,  -795,  -795,   292,
     292,  -795,  -795,   224,   242,   244,   292,   292,   292,   292,
     246,   371,   -82,   -82,   247,   260,   309,   -82,  -795,  -795,
     -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,   -82,
     -82,   -82,   456,   456,  -795,  -795,  -795,   456,   456,   266,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,   315,   545,  -795,   391,  -795,   554,
     554,   545,   545,  -795,   277,   554,   271,   -82,   -72,   -82,
     574,   -82,   371,   292,   292,   283,   285,   288,   293,   294,
     295,   300,   292,   301,  -795,  -795,   385,   316,   320,  -795,
    -795,  -795,   322,   323,   328,   334,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,   339,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,   621,  -795,  -795,
    -795,  -795,  -795,  -795,   271,  -795,  -795,  -795,   545,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  1237,   324,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,   341,
     343,  -795,  -795,  -795,  -795,  -795,  -795,  -795,   345,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,   292,  -795,  -795,
    -795,  -795,  -795,   340,   545,  -795,   630,   -82,    -1,  -795,
      85,   941,   -82,   -82,   640,   -82,   292,  -795,  -795,  -795,
     349,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  1237,  -795,  -795,  -795,   941,
     941,    59,   302,   347,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,   359,  -795,   550,   745,   824,   -82,  -795,   351,   -38,
     941,   941,   367,  1237,   315,   -82,  -795,   368,  -795,  1015,
    -795,  -795,  -795,  1311,  1311,  1533,  1237,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,   -51,  -795,  -795,
    -795,   361,    16,  -795,  -795,  -795,  -795,   370,    38,   181,
     153,  -795,   383,   184,   158,   176,   393,   395,   210,   241,
     248,   270,  -795,  2339,   400,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  1089,  -795,  -795,  -795,  -795,  -795,  1163,
    1385,  -795,  -795,  2339,   401,   407,   408,   409,   412,   416,
     414,   421,   422,   425,  2382,   -82,   315,  2391,   -82,  -795,
      28,  1237,  1237,  -795,  1015,  -795,  -795,  -795,  -795,  -795,
    -795,  1829,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  1755,  -795,   406,  -795,  -795,  -795,  -795,  1607,
    -795,  1607,   430,  -795,   417,  1755,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,   472,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,    20,
    -795,  2339,  -795,  1237,   425,  -795,   419,  -795,    18,   361,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  1607,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  1903,
    -795,  -795,  -795,  -795,  1903,  2051,   424,  -795,   -82,   431,
     -82,   434,   -82,   444,   -82,   446,  1607,  -795,  2195,  -795,
     439,   315,   -82,  2400,   -82,   442,  -795,  2339,  -795,  1533,
     425,  -795,   438,   451,   443,  -795,   454,   -57,  -795,  2267,
    2339,  -795,  2339,  -795,  -795,   453,  1681,  -795,  -795,   393,
    -795,  -795,   459,  -795,   470,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,   865,
    -795,   449,   -63,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,   465,   -75,  1459,   466,  -795,     6,  2339,  -795,   467,
    1237,   -85,  -795,  -795,  -795,   406,    40,   251,  -795,  2454,
     393,  -795,  -795,  -795,   138,  -795,  -795,  -795,  2051,  1237,
     -82,   -82,   -82,   -82,  2195,   445,   483,  -795,   315,   -82,
    1977,  -795,  -795,  -795,   430,  -795,  -795,   -85,  -795,  -795,
    -795,  -795,   480,  -795,  -795,  -795,  -795,  2339,  -795,  -795,
    -795,  -795,  -795,   473,   475,   452,   493,   460,   495,   496,
     501,  -795,   504,  -795,   506,   508,  -795,  -795,  -795,  -795,
     510,  2431,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,   393,
    -795,   -82,  1903,   -82,   512,   502,  2123,   511,   425,  2339,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,   254,  -795,  -795,  -795,   478,  -795,  2123,  -795,  -795,
    -795,  -795,   425,  2195,  -795,  -795,  -795,   406,   499,   499,
     499,   499,   499,   679,  1237,  1237,  1237,    39,  -795,   499,
    -795,  -795,  2195,   544,  -795,   131,  -795,   -59,   -58,   -54,
     -53,   -47,   547,   553,   553,   553,  -795,  -795,   -34,   546,
    -795,   515,   562,   563,   564,     4,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,   -82,   499,   702,   499,  -795,
     -22,   565,   -19,  -795,  -795,  -795
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   502,   124,   125,    78,   391,   424,   542,   541,   415,
     409,     0,     0,     0,   387,     0,     0,     0,   543,     0,
       0,     0,     0,   393,   395,   389,    14,    15,    17,    19,
      20,    22,    10,    12,    24,    25,   422,   107,     8,     0,
       2,     6,     5,   426,     4,   109,     7,   413,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   515,     0,     0,
       0,     0,     0,     0,   549,   550,   551,     0,     0,   484,
     436,   442,     0,     0,   438,     0,     0,   440,   446,   448,
     450,   452,   454,   456,   458,   460,   462,   464,   469,   529,
     530,   532,   531,   427,   429,     0,   533,   534,   433,   431,
       0,   496,   497,   498,   499,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
     397,   399,   535,   536,   546,     0,   401,   403,   405,   407,
     548,   537,   538,   539,   540,   411,   503,   349,   333,   350,
     334,   335,   336,   337,   340,   338,   341,   342,   339,   343,
     344,   345,   346,   347,   348,   351,   352,   326,   325,   355,
     358,   356,   359,   354,   357,   286,   353,   360,   361,   329,
     322,   324,   330,   323,   320,   321,   318,   319,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   379,   372,   373,
     374,   378,   375,   376,   377,   380,   381,   331,   382,   383,
     384,   385,   386,   317,   328,    79,   327,   282,   283,   332,
     284,   285,   362,   624,   615,   625,   622,   623,   626,   617,
     620,   621,   619,   392,   552,   553,   425,   630,   631,   558,
     629,   632,   627,   628,   416,   560,   561,   410,   487,     0,
       0,     0,     0,     0,     0,     0,   524,   525,   526,   517,
     519,   516,   518,   520,   521,   522,   523,     0,   508,   504,
     505,     0,     0,     0,   509,   510,   511,   512,   513,     0,
       0,   527,   528,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   444,   445,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   105,   106,   388,     0,     0,     0,
     394,   396,   390,    16,    18,    21,    23,    11,    13,   423,
     108,     9,   110,   414,     0,     0,   545,     0,   417,     0,
       0,     0,     0,   547,     0,     0,   115,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   507,   506,     0,     0,     0,   501,
     483,   490,     0,     0,     0,     0,   494,   669,   666,   667,
     670,   668,   664,   665,   485,   437,   443,   468,   466,     0,
     439,   441,   447,   449,   451,   453,   455,   457,   459,   461,
     463,   465,   470,   659,   663,   428,   656,     0,   661,   662,
     430,   434,   432,   435,   115,   398,   400,   419,     0,   573,
     402,   575,   576,   404,   406,   408,   421,   412,     0,     0,
     616,   555,   557,   618,   554,   637,   612,   634,   635,   638,
     636,   633,   563,   614,   559,   567,   565,   562,   488,     0,
       0,   478,   472,   474,   477,   475,   471,   482,     0,   480,
     514,   479,   500,   492,   495,   491,   493,     0,   467,   657,
     655,   658,   660,   266,     0,   418,     0,     0,     0,   117,
       0,     0,     0,     0,     0,     0,     0,   473,   476,   481,
       0,   265,   127,   420,   649,   646,   647,   580,   648,   645,
     578,   574,   584,   582,   577,     0,   114,   120,   119,     0,
       0,     0,    33,    35,    36,    38,    37,    39,   556,   613,
     643,   641,   568,   644,   642,   639,   640,   564,   572,   570,
     566,     0,   486,     0,     0,     0,     0,   116,   118,     0,
       0,     0,     0,     0,     0,     0,   489,     0,   111,     0,
     129,   131,   133,     0,     0,     0,     0,   174,   192,   138,
     141,   145,   148,   152,   155,   159,   162,   191,   168,   126,
     166,   602,   581,   606,   604,   654,   652,   585,   587,   653,
     650,   651,   579,   591,   589,   583,    42,   266,    34,    47,
      40,    46,     0,    44,   569,   571,   176,   286,   288,   289,
      32,   128,     0,   268,   268,   268,   268,     0,   287,   290,
     291,   292,   270,     0,     0,   298,   301,   299,   302,   300,
     296,   297,   187,     0,   185,   186,   293,   294,   295,     0,
       0,   183,   172,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    82,
       0,     0,     0,    41,     0,   267,    29,    30,    28,    26,
      27,     0,   312,   310,   311,   306,   307,   313,   314,   308,
     309,   303,     0,   273,   275,   276,   304,   305,   134,     0,
     184,     0,    84,   182,     0,     0,   139,   142,   146,   149,
     153,   156,   160,   163,   169,     0,   194,   195,   607,   603,
     611,   609,   605,   586,   592,   588,   596,   594,   590,     0,
      51,     0,    56,     0,     0,    48,    50,    53,     0,    45,
      31,    34,   112,   113,   130,   269,   272,   132,   271,   270,
     277,     0,   209,   210,   214,   121,   190,   136,   189,     0,
     212,   213,   171,    83,     0,   177,     0,   175,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   212,     0,   193,
     199,     0,     0,     0,     0,     0,    80,     0,    87,     0,
       0,    81,     0,    63,     0,    59,     0,     0,    43,     0,
       0,   135,     0,   208,   206,   219,   202,   123,   137,   268,
     288,   289,   178,   179,     0,   287,   290,   291,   292,   173,
     140,   143,   147,   150,   154,   157,   161,   164,   170,     0,
     196,     0,     0,   608,   610,   597,   593,   601,   599,   595,
      90,     0,     0,     0,     0,    89,   101,     0,    61,     0,
       0,    65,    49,    52,   274,   281,     0,     0,   217,     0,
     268,   201,   204,   205,     0,   188,   211,   122,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   167,     0,     0,
       0,    93,    96,    88,    84,   103,    99,     0,    54,   212,
      57,    55,     0,    64,   279,   278,   215,     0,   225,   226,
     222,   229,   230,     0,     0,     0,     0,     0,     0,     0,
       0,   242,   244,   243,     0,     0,   247,   248,   249,   254,
       0,     0,   227,   221,   207,   203,   180,   181,   144,   151,
     158,   165,   197,   200,   198,   598,   600,    92,    85,   268,
     212,     0,     0,     0,     0,     0,     0,    68,     0,     0,
     216,   223,   224,   231,   232,   250,   236,   238,   240,   245,
     251,     0,   234,   218,   220,   256,    91,     0,    97,    86,
     104,   100,     0,     0,    66,    58,    60,   280,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   260,     0,
     255,   228,     0,     0,    62,     0,   258,     0,     0,     0,
       0,     0,     0,   261,     0,     0,   253,   259,     0,     0,
     102,     0,     0,     0,     0,     0,    70,   233,   257,   237,
     239,   241,   246,   252,   262,   263,   264,   235,    94,    71,
      72,    76,    74,    67,    69,     0,     0,     0,     0,    95,
       0,     0,     0,    73,    77,    75
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,    65,  -460,  -795,  -795,  -517,  -516,  -502,    69,    71,
    -795,  -795,  -795,  -795,  -795,   -52,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -272,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -135,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,   -93,     0,  -795,  -795,
    -795,  -795,  -632,   319,  -795,   229,  -795,  -726,  -187,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -112,   -26,   188,  -479,  -633,  -795,  -795,
      49,  -765,  -795,  -795,  -795,  -795,   -99,  -795,  -794,  -730,
    -681,  -795,  -795,  -795,  -795,  -795,  -155,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -600,  -795,  -216,   165,
    -570,    24,   121,  -795,  -304,  -752,  -795,  -795,  -795,   -41,
    -693,  -526,  -795,  -795,  -524,  -533,   -49,  -795,  -511,  -512,
    -329,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,   -27,   729,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,   -48,  -795,  -795,
    -795,  -103,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
     -71,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,  -795,
    -795,  -795,  -795,  -461,  -795,   -46,  -795,  -795,  -795,    46,
    -488,  -614,  -795,  -795,   -67,  -795,   354,  -795,   355,  -230
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    39,    40,    41,   116,   112,   113,   108,   109,   110,
     111,   591,   501,   502,   503,   504,   505,   506,   581,   582,
     580,   640,   766,   705,   706,   707,   763,   917,   821,   819,
     859,   862,   945,   965,   985,   986,  1006,  1008,  1007,    42,
      50,   336,   699,   734,   761,   812,   908,   853,   910,  1005,
     911,   814,   914,   815,   857,   913,   433,   394,   115,    44,
     118,   404,   714,   419,   468,   469,   499,   726,   778,    45,
     523,   559,   602,   603,   604,   721,   624,   738,   625,   739,
     840,   626,   740,   627,   741,   841,   628,   742,   629,   743,
     842,   630,   744,   631,   745,   843,   633,   632,   746,   674,
     623,   736,   782,   783,   620,   613,   614,   727,   560,   685,
     686,   750,   802,   846,   834,   835,   728,   774,   729,   776,
     730,   772,   827,   830,   891,   892,   893,   935,   948,   959,
     949,   950,   951,   952,   953,   961,   967,   957,   958,   482,
     646,   651,   662,   719,   731,   664,   770,   826,   919,   507,
     596,   615,   665,   597,   206,   207,   666,   209,   210,   211,
     212,    46,    95,   107,    51,   105,   106,   324,   325,   329,
     330,   331,   332,    54,   335,   119,    53,   408,   464,   114,
      52,   302,   303,   308,   307,   282,   287,   290,   283,   291,
     292,   293,   294,   295,   296,   297,   298,   299,   300,   301,
     281,   457,   342,   476,    49,   269,   257,   273,    93,    94,
      98,   135,    47,    48,   327,   334,    69,   223,   339,   423,
     472,   234,   340,   341,   434,   474,   475,   517,   534,   535,
     410,   466,   467,   491,   525,   524,   526,   572,   636,   637,
     638,   695,   753,   754,   806,   848,   849,   562,   634,   635,
     689,   751,   752,   435,   473,   224,   337,   338,   225,   236,
     436,   519,   493,   574,   395,   460,   396,   397,   398,   399
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,   208,   306,   773,   226,   492,   237,   235,   777,   205,
     616,   616,   616,   518,   312,   598,   323,   799,   825,   621,
     691,   755,   593,   594,   647,   648,   649,   601,   600,   700,
     717,   618,   618,   618,   274,   275,   564,   595,   732,   528,
     529,   280,   784,   737,   258,   259,   260,   261,   775,   262,
     833,   374,   276,   779,   285,   747,   863,   310,   311,   701,
     263,   702,   304,   563,   573,   851,   319,   800,   421,   208,
     577,   578,   304,   235,    89,    90,   852,   322,   277,   592,
     616,   712,   481,   264,   987,   989,   616,   616,   771,   990,
     991,   100,   915,   703,   673,   836,   992,   265,   266,   267,
     687,   618,   531,   748,   305,   576,   286,   618,   618,   997,
     704,   101,   438,   798,   305,   531,    91,    92,   836,   239,
     598,  1013,   943,   713,  1015,   988,   988,   593,   594,   916,
     988,   988,   601,   600,   670,   756,   757,   988,   758,   808,
     670,   759,   595,   962,   701,   784,   702,   495,   856,   697,
     988,  1003,   687,   102,   278,   496,   240,   -98,   241,   242,
     243,   244,   988,   902,   642,   988,   642,   947,   981,   982,
     847,   765,   643,   690,   768,   103,   696,   983,   703,   245,
     937,   268,   976,   984,   711,   104,   777,   760,   864,   375,
     376,  -329,  -329,  -329,   380,   704,   865,   381,   382,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   837,
     530,   785,   531,   345,   346,   347,   348,   349,   350,   954,
     955,   956,   406,   788,   787,   497,   616,   816,   414,   415,
     351,   779,   117,   621,   353,   836,   400,   498,   271,   272,
     401,   402,   357,   358,   420,   422,   424,   618,   437,   362,
     363,   364,   365,   352,   120,   121,   836,   122,   123,   413,
     894,   136,   964,   124,   417,   125,   126,   127,   128,   129,
     130,   131,   238,   722,   667,   723,   405,    96,    97,   235,
     616,   979,   270,   411,   411,   235,   235,   673,   279,   411,
      89,    90,   807,   288,   667,   981,   982,  -316,   284,   663,
     289,   618,   645,   309,   983,   465,  -328,  -328,  -328,   313,
     984,   -38,   -38,   -38,   785,   314,   439,   440,   315,   663,
     645,   316,   667,   271,   272,   448,   788,   787,   645,   -37,
     -37,   -37,   317,   667,  -330,  -330,  -330,   -36,   -36,   936,
     667,   318,   667,   320,    55,   321,   667,   715,   326,   968,
     969,   970,   971,   132,   133,   134,   343,   344,   718,   978,
     356,   483,   235,  -327,  -327,  -327,   328,    56,   359,   208,
      57,   718,   667,   333,   494,   412,   412,   470,    58,   508,
     509,   412,   520,   354,   355,   946,   360,    59,   361,    60,
     366,   377,   667,    61,  -283,  -283,  -283,   762,   866,   867,
     667,  -284,  -284,  -284,   378,   667,  1010,   379,  1012,   963,
     403,    62,   407,    63,    64,    65,    66,   667,   235,   667,
      67,   416,   208,  -285,  -285,  -285,   418,   441,   667,   442,
     480,    68,   443,   575,   954,   955,   956,   444,   445,   446,
     667,   667,   585,   667,   447,   449,   208,   667,   450,   521,
     208,   208,   246,   811,   470,   213,   532,   214,   471,   215,
     451,   216,   217,   218,   452,   715,   453,   454,   828,    70,
     667,    71,   455,   481,   247,   248,   249,   250,   456,   251,
     252,   208,   208,   458,   208,   477,   584,   478,   667,   479,
     599,    72,   583,   522,   617,   617,   617,   208,   219,   220,
     221,   222,   533,   536,   531,   622,   253,   254,   255,   579,
     256,   367,   586,   858,  -315,   667,   641,   368,   369,   370,
      73,   667,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,   644,   645,   667,   650,
     668,   676,   692,   612,   612,   698,   909,   677,   680,   678,
     679,   537,   681,   682,   371,   372,   373,    88,   157,   158,
     683,   720,   684,   920,   617,   733,   735,   767,   789,   791,
     617,   617,   793,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   795,   667,   797,   801,   810,   667,   693,   817,
     667,   818,   208,   208,   820,   599,   367,   822,    37,   829,
     583,   583,   368,   369,   370,   157,   158,   838,   667,   203,
     839,   204,   747,   612,   667,   845,   850,   855,   860,   612,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   903,
     904,   918,   921,   667,   922,   924,   923,   926,   927,   371,
     372,   373,   393,   928,   925,   790,   929,   792,   930,   794,
     931,   796,   932,   942,   208,   941,   203,   944,   204,   804,
     748,   809,   764,   538,   539,   960,   540,   541,   542,   543,
     544,   545,   546,   547,   548,   549,   550,   551,   552,   553,
     554,   555,   556,   966,   972,   227,   786,   214,   980,   228,
     993,   229,   230,   231,   227,    37,   214,   998,   228,   999,
     409,   230,   231,   803,  1000,  1001,  1002,  1011,  1014,   710,
     617,   709,   708,  1004,   425,   823,   426,   557,   304,   912,
     427,   428,   429,   463,   527,   939,   896,   558,   219,   232,
     233,   222,   619,   813,   749,   895,   934,   219,   232,   233,
     222,   977,   639,   769,   675,    99,     0,   898,   899,   900,
     901,   461,   462,     0,     0,     0,   906,   430,   431,   432,
     305,   367,     0,    37,   617,   459,     0,   368,   369,   370,
     484,   208,   426,     0,   304,     0,   485,   486,   487,   861,
     510,     0,   426,     0,   304,     0,   511,   512,   513,   786,
     208,     0,     0,     0,     0,     0,     0,     0,   897,     0,
     905,     0,     0,     0,   371,   372,   373,   393,     0,     0,
       0,     0,     0,   488,   489,   490,   305,     0,   938,     0,
     940,     0,     0,   514,   515,   516,   305,     0,     0,     0,
       0,     0,     0,     0,    -3,     1,     0,     0,     0,     0,
       2,     3,     4,     5,     0,  -544,  -544,     6,  -544,  -544,
       7,     8,     9,    10,  -544,     0,  -544,  -544,  -544,  -544,
    -544,  -544,  -544,    11,    12,     0,     0,    13,     0,     0,
       0,    14,     0,     0,     0,    15,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   425,     0,   426,     0,   304,
      16,   427,   428,   429,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   208,   208,   208,     0,    17,
       0,     0,  1009,   973,   974,   975,     0,     0,     0,    18,
       0,    19,    20,    21,    22,    23,    24,    25,   430,   431,
     561,   305,     0,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,  -544,  -544,  -544,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   565,     0,   426,     0,   304,     0,
     566,   567,   568,   994,   995,   996,    37,     0,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   652,     0,
       0,   653,     0,   654,     0,     0,   655,   569,   570,   571,
     305,   724,     0,     0,   656,   657,   658,    38,   659,   660,
       0,     0,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,     0,   661,
       0,     0,     0,   844,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,   163,   164,   500,     0,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,     0,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,     0,   203,     0,   204,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,     0,   163,   164,   500,     0,   587,
     166,   167,   168,   588,   170,   171,   589,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,     0,   203,
       0,   590,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   605,   606,   607,   608,     0,   669,
     609,    37,     0,   610,   611,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,     0,   203,     0,   204,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   605,   606,
     607,   608,     0,   671,   609,    37,     0,   610,   611,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,     0,   203,     0,   204,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,     0,   163,   164,     0,
       0,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
       0,   203,     0,   204,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   605,   606,   607,   608,
       0,     0,   609,    37,     0,   610,   611,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,     0,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,     0,   203,     0,   204,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     605,   606,   607,   608,     0,   672,   609,     0,     0,   610,
     611,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,     0,   203,
       0,   204,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   605,   606,   607,   608,     0,   854,
     609,     0,     0,   610,   611,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,     0,   203,     0,   204,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   605,   606,
     607,   608,     0,     0,   609,     0,     0,   610,   611,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,     0,   203,     0,   204,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     652,     0,   722,   653,   723,   654,     0,     0,   655,     0,
       0,     0,     0,   724,     0,     0,   656,   657,   658,     0,
     659,   660,     0,     0,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
       0,   661,     0,   725,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   652,     0,     0,   653,     0,   654,
       0,   831,   655,     0,     0,   832,     0,   724,     0,     0,
     656,   657,   658,     0,   659,   660,     0,     0,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,     0,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,     0,   661,     0,   725,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   652,     0,
       0,   653,     0,   654,     0,     0,   655,     0,     0,   712,
     716,     0,     0,     0,   656,   657,   658,     0,   659,   660,
       0,     0,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,     0,   661,
       0,   713,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   652,     0,     0,   653,     0,   654,     0,     0,
     655,     0,     0,   712,     0,     0,     0,     0,   656,   657,
     658,     0,   659,   660,     0,     0,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,     0,   661,     0,   713,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   652,     0,     0,   653,
       0,   654,     0,     0,   655,     0,     0,     0,     0,   724,
       0,     0,   656,   657,   658,     0,   659,   660,     0,     0,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,     0,   661,     0,   725,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     652,     0,     0,   653,     0,   654,     0,     0,   655,     0,
       0,     0,     0,     0,     0,     0,   656,   657,   658,     0,
     659,   660,     0,     0,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
       0,   661,     0,   907,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
       0,   163,   164,     0,     0,     0,   166,   167,   168,   780,
     170,   171,   781,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,     0,
     189,   190,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,     0,   203,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   652,     0,   722,   653,
     723,   654,     0,     0,   655,     0,     0,     0,     0,   724,
       0,     0,   656,   657,   658,     0,   659,   660,     0,     0,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,     0,   189,   190,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,     0,   661,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   652,     0,
       0,   653,     0,   654,     0,     0,   655,     0,     0,     0,
       0,   724,     0,     0,   656,   657,   658,     0,   659,   660,
       0,     0,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,     0,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,     0,   661,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     652,     0,     0,   653,     0,   654,     0,     0,   655,     0,
       0,     0,   824,     0,     0,     0,   656,   657,   658,     0,
     659,   660,     0,     0,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,     0,   189,   190,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
       0,   661,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   652,     0,     0,   653,     0,   654,     0,     0,
     655,     0,     0,     0,     0,     0,     0,     0,   656,   657,
     658,     0,   659,   660,     0,     0,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,     0,   189,   190,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   510,   661,   426,     0,   304,     0,   511,   688,
     513,   425,     0,   426,     0,   304,     0,   427,   428,   429,
     510,     0,   426,     0,   304,     0,   511,   805,   513,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   514,   515,   516,   305,     0,
       0,     0,     0,     0,   430,   431,   694,   305,   933,     0,
       0,     0,     0,   514,   515,   516,   305,     0,   868,   869,
     870,   871,   872,   873,   874,   875,   876,   877,   878,   879,
     880,   881,   882,   883,   884,   885,   886,   887,   888,   889,
     890,   868,   869,   870,   871,   872,   873,   874,   875,   876,
     877,   878,   879,   880,   881,   882,   883,   884,   885,   886,
     887,   888,   889,   890
};

static const yytype_int16 yycheck[] =
{
       0,    50,    95,   729,    52,   466,    54,    53,   734,    50,
     543,   544,   545,   474,   107,   539,   119,   747,   770,   545,
     634,     1,   539,   539,   594,   595,   596,   539,   539,     1,
     662,   543,   544,   545,    61,    62,   524,   539,   671,   499,
     500,    68,   735,   675,    42,    43,    44,    45,   729,    47,
     776,   281,    68,   734,    98,   140,   821,   105,   106,   116,
      58,   118,   144,   524,   525,   140,   114,   748,   140,   118,
     530,   531,   144,   119,    37,    38,   151,   118,    94,   539,
     613,   144,   133,    81,   143,   143,   619,   620,   721,   143,
     143,    76,   857,   150,   620,   776,   143,    95,    96,    97,
     633,   613,   153,   188,   186,   143,   150,   619,   620,   143,
     167,   144,   342,   746,   186,   153,    79,    80,   799,    13,
     644,   143,   916,   186,   143,   184,   184,   644,   644,   859,
     184,   184,   644,   644,   613,   115,   116,   184,   118,   753,
     619,   121,   644,   937,   116,   838,   118,   148,   142,   637,
     184,   147,   685,   144,   170,   156,    50,   151,    52,    53,
      54,    55,   184,   844,   148,   184,   148,   919,   164,   165,
     802,   704,   156,   634,   156,   144,   637,   173,   150,    73,
     910,   179,   143,   179,   644,   144,   912,   167,   148,   282,
     283,   153,   154,   155,   287,   167,   156,   290,   291,   292,
     293,   294,   295,   296,   297,   298,   299,   300,   301,   779,
     151,   735,   153,   240,   241,   242,   243,   244,   245,   180,
     181,   182,   325,   735,   735,   140,   759,   760,   331,   332,
     257,   912,     0,   759,   261,   916,   303,   152,    31,    32,
     307,   308,   269,   270,   337,   338,   339,   759,   341,   276,
     277,   278,   279,    46,    11,    12,   937,    14,    15,   330,
     830,   144,   943,    20,   335,    22,    23,    24,    25,    26,
      27,    28,    79,   135,   603,   137,   324,    81,    82,   325,
     813,   962,    30,   329,   330,   331,   332,   813,    84,   335,
      37,    38,   753,   144,   623,   164,   165,   144,    42,   603,
     144,   813,   144,   170,   173,   408,   153,   154,   155,     3,
     179,   153,   154,   155,   838,     3,   343,   344,     4,   623,
     144,     4,   651,    31,    32,   352,   838,   838,   144,   153,
     154,   155,     3,   662,   153,   154,   155,   153,   154,   909,
     669,     3,   671,   143,     7,     3,   675,   651,    86,   949,
     950,   951,   952,   110,   111,   112,    52,    53,   662,   959,
      47,   464,   408,   153,   154,   155,    18,    30,   144,   418,
      33,   675,   701,    98,   467,   329,   330,   418,    41,   472,
     473,   335,   475,    48,    49,   918,   144,    50,   144,    52,
     144,   144,   721,    56,   153,   154,   155,   701,   147,   148,
     729,   153,   154,   155,   144,   734,  1006,    98,  1008,   942,
     144,    74,    21,    76,    77,    78,    79,   746,   464,   748,
      83,   144,   471,   153,   154,   155,   155,   144,   757,   144,
     457,    94,   144,   526,   180,   181,   182,   144,   144,   144,
     769,   770,   535,   772,   144,   144,   495,   776,    63,   476,
     499,   500,    12,   757,   495,   140,   154,   142,   134,   144,
     144,   146,   147,   148,   144,   769,   144,   144,   772,     6,
     799,     8,   144,   133,    34,    35,    36,    37,   144,    39,
      40,   530,   531,   144,   533,   144,   534,   144,   817,   144,
     539,    28,   533,   144,   543,   544,   545,   546,   183,   184,
     185,   186,   155,   144,   153,   546,    66,    67,    68,   142,
      70,   140,   144,   817,   144,   844,   155,   146,   147,   148,
      57,   850,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,   153,   144,   867,   144,
     140,   140,   635,   543,   544,   638,   850,   140,   136,   141,
     141,     1,   136,   139,   183,   184,   185,    94,   133,   134,
     139,   155,   140,   867,   613,   135,   149,   148,   144,   138,
     619,   620,   138,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   138,   912,   138,   146,   144,   916,   636,   151,
     919,   140,   641,   642,   151,   644,   140,   143,   142,   146,
     641,   642,   146,   147,   148,   133,   134,   148,   937,   184,
     140,   186,   140,   613,   943,   166,   151,   151,   151,   619,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   184,
     147,   151,   159,   962,   159,   142,   184,   142,   142,   183,
     184,   185,   186,   142,   184,   738,   142,   740,   142,   742,
     142,   744,   142,   151,   703,   143,   184,   146,   186,   752,
     188,   754,   703,   113,   114,   187,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   184,     5,   140,   735,   142,   144,   144,
     143,   146,   147,   148,   140,   142,   142,   151,   144,   184,
     146,   147,   148,   751,   142,   142,   142,     5,   143,   644,
     759,   642,   641,   985,   140,   767,   142,   167,   144,   854,
     146,   147,   148,   404,   495,   912,   838,   177,   183,   184,
     185,   186,   544,   759,   685,   834,   891,   183,   184,   185,
     186,   957,   577,   719,   623,    16,    -1,   840,   841,   842,
     843,   397,   397,    -1,    -1,    -1,   849,   183,   184,   185,
     186,   140,    -1,   142,   813,   144,    -1,   146,   147,   148,
     140,   820,   142,    -1,   144,    -1,   146,   147,   148,   820,
     140,    -1,   142,    -1,   144,    -1,   146,   147,   148,   838,
     839,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   839,    -1,
     848,    -1,    -1,    -1,   183,   184,   185,   186,    -1,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,    -1,   911,    -1,
     913,    -1,    -1,   183,   184,   185,   186,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,     0,     1,    -1,    -1,    -1,    -1,
       6,     7,     8,     9,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    -1,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    -1,    -1,    33,    -1,    -1,
      -1,    37,    -1,    -1,    -1,    41,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,
      56,   146,   147,   148,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   954,   955,   956,    -1,    75,
      -1,    -1,  1005,   954,   955,   956,    -1,    -1,    -1,    85,
      -1,    87,    88,    89,    90,    91,    92,    93,   183,   184,
     185,   186,    -1,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   140,    -1,   142,    -1,   144,    -1,
     146,   147,   148,   973,   974,   975,   142,    -1,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,   136,    -1,   138,    -1,    -1,   141,   183,   184,   185,
     186,   146,    -1,    -1,   149,   150,   151,   183,   153,   154,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,    -1,   184,
      -1,    -1,    -1,   188,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,   141,   142,    -1,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,    -1,   184,    -1,   186,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,   140,   141,   142,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,    -1,   184,
      -1,   186,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,   140,
     141,   142,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,    -1,   184,    -1,   186,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,   140,   141,   142,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,    -1,   184,    -1,   186,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,    -1,   140,   141,    -1,
      -1,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
      -1,   184,    -1,   186,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,    -1,   141,   142,    -1,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,    -1,   184,    -1,   186,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,    -1,   140,   141,    -1,    -1,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,    -1,   184,
      -1,   186,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,    -1,   140,
     141,    -1,    -1,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,    -1,   184,    -1,   186,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,    -1,    -1,   141,    -1,    -1,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,    -1,   184,    -1,   186,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,   135,   136,   137,   138,    -1,    -1,   141,    -1,
      -1,    -1,    -1,   146,    -1,    -1,   149,   150,   151,    -1,
     153,   154,    -1,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
      -1,   184,    -1,   186,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,    -1,   136,    -1,   138,
      -1,   140,   141,    -1,    -1,   144,    -1,   146,    -1,    -1,
     149,   150,   151,    -1,   153,   154,    -1,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,    -1,   184,    -1,   186,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,   136,    -1,   138,    -1,    -1,   141,    -1,    -1,   144,
     145,    -1,    -1,    -1,   149,   150,   151,    -1,   153,   154,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,    -1,   184,
      -1,   186,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,    -1,    -1,   136,    -1,   138,    -1,    -1,
     141,    -1,    -1,   144,    -1,    -1,    -1,    -1,   149,   150,
     151,    -1,   153,   154,    -1,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,    -1,   184,    -1,   186,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,    -1,   136,
      -1,   138,    -1,    -1,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,   149,   150,   151,    -1,   153,   154,    -1,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,    -1,   184,    -1,   186,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   149,   150,   151,    -1,
     153,   154,    -1,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
      -1,   184,    -1,   186,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
      -1,   140,   141,    -1,    -1,    -1,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,    -1,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,    -1,   184,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,    -1,   135,   136,
     137,   138,    -1,    -1,   141,    -1,    -1,    -1,    -1,   146,
      -1,    -1,   149,   150,   151,    -1,   153,   154,    -1,    -1,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,    -1,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,    -1,   184,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    -1,
      -1,   136,    -1,   138,    -1,    -1,   141,    -1,    -1,    -1,
      -1,   146,    -1,    -1,   149,   150,   151,    -1,   153,   154,
      -1,    -1,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    -1,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,    -1,   184,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,    -1,    -1,   136,    -1,   138,    -1,    -1,   141,    -1,
      -1,    -1,   145,    -1,    -1,    -1,   149,   150,   151,    -1,
     153,   154,    -1,    -1,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,    -1,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
      -1,   184,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,    -1,    -1,   136,    -1,   138,    -1,    -1,
     141,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   149,   150,
     151,    -1,   153,   154,    -1,    -1,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,    -1,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   140,   184,   142,    -1,   144,    -1,   146,   147,
     148,   140,    -1,   142,    -1,   144,    -1,   146,   147,   148,
     140,    -1,   142,    -1,   144,    -1,   146,   147,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   183,   184,   185,   186,    -1,
      -1,    -1,    -1,    -1,   183,   184,   185,   186,   147,    -1,
      -1,    -1,    -1,   183,   184,   185,   186,    -1,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    16,    17,    18,
      19,    29,    30,    33,    37,    41,    56,    75,    85,    87,
      88,    89,    90,    91,    92,    93,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   142,   183,   190,
     191,   192,   228,   246,   248,   258,   350,   401,   402,   393,
     229,   353,   369,   365,   362,     7,    30,    33,    41,    50,
      52,    56,    74,    76,    77,    78,    79,    83,    94,   405,
       6,     8,    28,    57,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    94,    37,
      38,    79,    80,   397,   398,   351,    81,    82,   399,   397,
      76,   144,   144,   144,   144,   354,   355,   352,   196,   197,
     198,   199,   194,   195,   368,   247,   193,     0,   249,   364,
      11,    12,    14,    15,    20,    22,    23,    24,    25,    26,
      27,    28,   110,   111,   112,   400,   144,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   140,   141,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   184,   186,   338,   343,   344,   345,   346,
     347,   348,   349,   140,   142,   144,   146,   147,   148,   183,
     184,   185,   186,   406,   444,   447,   406,   140,   144,   146,
     147,   148,   184,   185,   410,   444,   448,   406,    79,    13,
      50,    52,    53,    54,    55,    73,    12,    34,    35,    36,
      37,    39,    40,    66,    67,    68,    70,   395,    42,    43,
      44,    45,    47,    58,    81,    95,    96,    97,   179,   394,
      30,    31,    32,   396,   396,   396,    68,    94,   170,    84,
     396,   389,   374,   377,    42,    98,   150,   375,   144,   144,
     376,   378,   379,   380,   381,   382,   383,   384,   385,   386,
     387,   388,   370,   371,   144,   186,   245,   373,   372,   170,
     406,   406,   245,     3,     3,     4,     4,     3,     3,   406,
     143,     3,   338,   410,   356,   357,    86,   403,    18,   358,
     359,   360,   361,    98,   404,   363,   230,   445,   446,   407,
     411,   412,   391,    52,    53,   396,   396,   396,   396,   396,
     396,   396,    46,   396,    48,    49,    47,   396,   396,   144,
     144,   144,   396,   396,   396,   396,   144,   140,   146,   147,
     148,   183,   184,   185,   458,   245,   245,   144,   144,    98,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   186,   246,   453,   455,   456,   457,   458,
     453,   453,   453,   144,   250,   406,   410,    21,   366,   146,
     419,   444,   448,   419,   410,   410,   144,   419,   155,   252,
     245,   140,   245,   408,   245,   140,   142,   146,   147,   148,
     183,   184,   185,   245,   413,   442,   449,   245,   458,   396,
     396,   144,   144,   144,   144,   144,   144,   144,   396,   144,
      63,   144,   144,   144,   144,   144,   144,   390,   144,   144,
     454,   455,   457,   252,   367,   410,   420,   421,   253,   254,
     338,   134,   409,   443,   414,   415,   392,   144,   144,   144,
     396,   133,   328,   410,   140,   146,   147,   148,   183,   184,
     185,   422,   442,   451,   245,   148,   156,   140,   152,   255,
     142,   201,   202,   203,   204,   205,   206,   338,   245,   245,
     140,   146,   147,   148,   183,   184,   185,   416,   442,   450,
     245,   396,   144,   259,   424,   423,   425,   254,   201,   201,
     151,   153,   154,   155,   417,   418,   144,     1,   113,   114,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   167,   177,   260,
     297,   185,   436,   442,   449,   140,   146,   147,   148,   183,
     184,   185,   426,   442,   452,   245,   143,   201,   201,   142,
     209,   207,   208,   338,   406,   245,   144,   144,   148,   151,
     186,   200,   201,   204,   205,   206,   339,   342,   343,   345,
     347,   348,   261,   262,   263,   135,   136,   137,   138,   141,
     144,   145,   246,   294,   295,   340,   344,   345,   348,   294,
     293,   340,   338,   289,   265,   267,   270,   272,   275,   277,
     280,   282,   286,   285,   437,   438,   427,   428,   429,   328,
     210,   155,   148,   156,   153,   144,   329,   329,   329,   329,
     144,   330,   133,   136,   138,   141,   149,   150,   151,   153,
     154,   184,   331,   333,   334,   341,   345,   349,   140,   140,
     295,   140,   140,   340,   288,   331,   140,   140,   141,   141,
     136,   136,   139,   139,   140,   298,   299,   344,   147,   439,
     442,   450,   245,   406,   185,   430,   442,   449,   245,   231,
       1,   116,   118,   150,   167,   212,   213,   214,   208,   207,
     200,   201,   144,   186,   251,   333,   145,   251,   333,   332,
     155,   264,   135,   137,   146,   186,   256,   296,   305,   307,
     309,   333,   296,   135,   232,   149,   290,   251,   266,   268,
     271,   273,   276,   278,   281,   283,   287,   140,   188,   299,
     300,   440,   441,   431,   432,     1,   115,   116,   118,   121,
     167,   233,   333,   215,   338,   344,   211,   148,   156,   330,
     335,   296,   310,   256,   306,   309,   308,   256,   257,   309,
     148,   151,   291,   292,   339,   343,   345,   347,   348,   144,
     245,   138,   245,   138,   245,   138,   245,   138,   296,   308,
     309,   146,   301,   406,   245,   147,   433,   442,   450,   245,
     144,   333,   234,   293,   240,   242,   344,   151,   140,   218,
     151,   217,   143,   214,   145,   334,   336,   311,   333,   146,
     312,   140,   144,   256,   303,   304,   309,   329,   148,   140,
     269,   274,   279,   284,   188,   166,   302,   251,   434,   435,
     151,   140,   151,   236,   140,   151,   142,   243,   333,   219,
     151,   338,   220,   300,   148,   156,   147,   148,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   313,   314,   315,   329,   305,   292,   338,   245,   245,
     245,   245,   309,   184,   147,   406,   245,   186,   235,   333,
     237,   239,   232,   244,   241,   300,   308,   216,   151,   337,
     333,   159,   159,   184,   142,   184,   142,   142,   142,   142,
     142,   142,   142,   147,   315,   316,   329,   308,   245,   257,
     245,   143,   151,   307,   146,   221,   344,   334,   317,   319,
     320,   321,   322,   323,   180,   181,   182,   326,   327,   318,
     187,   324,   307,   344,   309,   222,   184,   325,   325,   325,
     325,   325,     5,   338,   338,   338,   143,   327,   325,   309,
     144,   164,   165,   173,   179,   223,   224,   143,   184,   143,
     143,   143,   143,   143,   246,   246,   246,   143,   151,   184,
     142,   142,   142,   147,   224,   238,   225,   227,   226,   245,
     325,     5,   325,   143,   143,   143
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   189,   190,   190,   191,   191,   191,   191,   193,   192,
     194,   192,   195,   192,   192,   196,   192,   197,   192,   192,
     198,   192,   199,   192,   192,   192,   200,   200,   200,   200,
     200,   200,   200,   201,   201,   202,   202,   203,   203,   203,
     204,   205,   206,   207,   207,   208,   208,   210,   211,   209,
     212,   212,   213,   213,   214,   214,   215,   216,   214,   217,
     214,   219,   218,   218,   220,   220,   222,   221,   221,   223,
     223,   224,   225,   224,   226,   224,   227,   224,   229,   230,
     228,   231,   231,   232,   232,   233,   233,   234,   233,   233,
     233,   235,   235,   237,   238,   236,   239,   236,   240,   241,
     240,   243,   242,   244,   242,   245,   245,   247,   246,   249,
     250,   248,   251,   251,   252,   252,   253,   253,   254,   255,
     255,   256,   257,   257,   258,   258,   259,   259,   260,   261,
     260,   262,   260,   263,   264,   260,   260,   260,   265,   266,
     260,   267,   268,   269,   260,   270,   271,   260,   272,   273,
     274,   260,   275,   276,   260,   277,   278,   279,   260,   280,
     281,   260,   282,   283,   284,   260,   285,   260,   286,   287,
     260,   260,   288,   260,   289,   260,   260,   290,   290,   291,
     291,   292,   293,   293,   294,   294,   295,   295,   296,   296,
     296,   297,   297,   298,   298,   299,   300,   300,   301,   301,
     302,   303,   303,   304,   304,   304,   305,   306,   306,   307,
     307,   308,   308,   309,   310,   309,   311,   311,   312,   312,
     313,   313,   314,   314,   314,   315,   315,   316,   315,   315,
     315,   315,   317,   315,   318,   315,   319,   315,   320,   315,
     321,   315,   315,   315,   315,   322,   315,   315,   315,   315,
     315,   323,   315,   315,   315,   324,   324,   325,   325,   326,
     326,   327,   327,   327,   327,   328,   328,   329,   329,   330,
     330,   331,   331,   332,   331,   333,   334,   335,   334,   337,
     336,   336,   338,   338,   338,   338,   338,   339,   339,   339,
     339,   339,   339,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   340,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   341,   342,   342,   343,   343,   343,
     343,   343,   343,   343,   343,   343,   343,   344,   344,   344,
     344,   345,   345,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   346,   346,   347,   347,   347,   347,   347,   347,   347,
     348,   348,   348,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   349,   349,   349,
     349,   349,   349,   349,   349,   349,   349,   351,   350,   352,
     350,   353,   350,   354,   350,   355,   350,   356,   350,   357,
     350,   358,   350,   359,   350,   360,   350,   361,   350,   362,
     350,   363,   350,   364,   350,   365,   350,   366,   350,   367,
     350,   350,   368,   350,   369,   350,   350,   370,   350,   371,
     350,   372,   350,   373,   350,   350,   374,   350,   375,   350,
     376,   350,   377,   350,   350,   350,   378,   350,   379,   350,
     380,   350,   381,   350,   382,   350,   383,   350,   384,   350,
     385,   350,   386,   350,   387,   350,   350,   350,   350,   388,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   389,   390,   350,   391,   392,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   393,   350,   394,   394,   394,   394,   394,   394,
     394,   394,   394,   394,   394,   395,   395,   395,   395,   395,
     395,   395,   395,   395,   395,   395,   395,   396,   396,   397,
     397,   398,   398,   399,   399,   400,   400,   400,   400,   400,
     400,   401,   401,   402,   402,   403,   403,   404,   404,   405,
     405,   405,   406,   407,   406,   409,   408,   408,   411,   410,
     410,   412,   410,   414,   413,   415,   413,   413,   417,   416,
     418,   416,   416,   420,   419,   419,   421,   419,   423,   422,
     424,   422,   425,   422,   422,   427,   426,   428,   426,   429,
     426,   426,   431,   430,   432,   430,   430,   434,   433,   435,
     433,   433,   437,   436,   438,   436,   436,   440,   439,   441,
     439,   439,   443,   442,   442,   445,   444,   446,   444,   444,
     447,   447,   447,   447,   447,   447,   447,   448,   448,   448,
     448,   448,   448,   449,   449,   449,   449,   449,   449,   450,
     450,   450,   450,   450,   450,   451,   451,   451,   451,   451,
     452,   452,   452,   452,   452,   453,   453,   454,   454,   455,
     456,   456,   457,   457,   458,   458,   458,   458,   458,   458,
     458
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     1,     1,     0,     3,
       0,     3,     0,     3,     1,     0,     3,     0,     3,     1,
       0,     3,     0,     3,     1,     1,     2,     2,     2,     2,
       2,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       3,     4,     3,     4,     1,     3,     1,     0,     0,     5,
       1,     1,     3,     1,     4,     4,     0,     0,     6,     0,
       6,     0,     5,     0,     1,     0,     0,     4,     0,     2,
       1,     2,     0,     5,     0,     5,     0,     5,     0,     0,
      12,     2,     0,     1,     0,     4,     5,     0,     3,     2,
       2,     2,     1,     0,     0,     8,     0,     3,     1,     0,
       4,     0,     6,     0,     4,     1,     1,     0,     3,     0,
       0,     8,     1,     1,     3,     0,     3,     1,     3,     1,
       1,     1,     2,     1,     1,     1,     2,     0,     2,     0,
       4,     0,     4,     0,     0,     5,     4,     5,     0,     0,
       5,     0,     0,     0,     7,     0,     0,     5,     0,     0,
       0,     7,     0,     0,     5,     0,     0,     0,     7,     0,
       0,     5,     0,     0,     0,     7,     0,     6,     0,     0,
       5,     4,     0,     5,     0,     4,     2,     1,     2,     1,
       3,     3,     2,     1,     2,     1,     1,     1,     3,     1,
       1,     1,     1,     2,     1,     1,     2,     4,     3,     0,
       2,     1,     0,     2,     1,     1,     2,     3,     1,     1,
       1,     2,     0,     1,     0,     4,     3,     1,     3,     0,
       2,     1,     1,     2,     2,     1,     1,     0,     3,     1,
       1,     2,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     1,     1,     1,     0,     5,     1,     1,     1,
       2,     0,     5,     4,     1,     1,     0,     2,     1,     2,
       1,     2,     3,     3,     3,     1,     0,     1,     0,     2,
       0,     2,     2,     0,     4,     1,     1,     0,     5,     0,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     3,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       3,     0,     4,     0,     3,     0,     3,     0,     5,     0,
       6,     4,     0,     3,     0,     3,     1,     0,     4,     0,
       4,     0,     4,     0,     4,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     3,     3,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     4,     5,     4,     0,
       4,     5,     5,     6,     5,     5,     6,     5,     5,     5,
       5,     6,     5,     4,     0,     0,     7,     0,     0,     8,
       4,     5,     5,     5,     4,     5,     2,     2,     2,     2,
       5,     4,     0,     3,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     3,     0,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     0,     1,     0,     1,     0,     1,
       1,     1,     1,     0,     3,     0,     3,     1,     0,     3,
       1,     0,     3,     0,     3,     0,     3,     1,     0,     3,
       0,     3,     1,     0,     3,     1,     0,     3,     0,     3,
       0,     3,     0,     3,     1,     0,     3,     0,     3,     0,
       3,     1,     0,     3,     0,     3,     1,     0,     3,     0,
       3,     1,     0,     3,     0,     3,     1,     0,     3,     0,
       3,     1,     0,     3,     1,     0,     3,     0,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1
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
      yyerror (parseResult, YY_("syntax error: cannot back up")); \
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
                  Type, Value, parseResult); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, UserLevelRewritingContext::ParseResult* parseResult)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (parseResult);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, UserLevelRewritingContext::ParseResult* parseResult)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, parseResult);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, UserLevelRewritingContext::ParseResult* parseResult)
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
                                              , parseResult);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, parseResult); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, UserLevelRewritingContext::ParseResult* parseResult)
{
  YYUSE (yyvaluep);
  YYUSE (parseResult);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  switch (yytype)
    {
          case 200: /* moduleExprDot  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2389 "surface.c" /* yacc.c:1257  */
        break;

    case 201: /* moduleExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2395 "surface.c" /* yacc.c:1257  */
        break;

    case 202: /* moduleExpr2  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2401 "surface.c" /* yacc.c:1257  */
        break;

    case 203: /* moduleExpr3  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2407 "surface.c" /* yacc.c:1257  */
        break;

    case 204: /* renameExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2413 "surface.c" /* yacc.c:1257  */
        break;

    case 205: /* instantExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2419 "surface.c" /* yacc.c:1257  */
        break;

    case 206: /* parenExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2425 "surface.c" /* yacc.c:1257  */
        break;

    case 207: /* viewExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyViewExpression)->deepSelfDestruct(); }
#line 2431 "surface.c" /* yacc.c:1257  */
        break;

    case 208: /* instantArgs  */
#line 113 "surface.yy" /* yacc.c:1257  */
      { deepSelfDestructViewExpressionVector(((*yyvaluep).yyViewExpressionVector)); }
#line 2437 "surface.c" /* yacc.c:1257  */
        break;


      default:
        break;
    }
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (UserLevelRewritingContext::ParseResult* parseResult)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

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
      yychar = yylex (&yylval);
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
#line 247 "surface.yy" /* yacc.c:1646  */
    { YYACCEPT; }
#line 2705 "surface.c" /* yacc.c:1646  */
    break;

  case 3:
#line 249 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			}
#line 2713 "surface.c" /* yacc.c:1646  */
    break;

  case 8:
#line 263 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2719 "surface.c" /* yacc.c:1646  */
    break;

  case 9:
#line 265 "surface.yy" /* yacc.c:1646  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			}
#line 2732 "surface.c" /* yacc.c:1646  */
    break;

  case 10:
#line 273 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2738 "surface.c" /* yacc.c:1646  */
    break;

  case 11:
#line 275 "surface.yy" /* yacc.c:1646  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2751 "surface.c" /* yacc.c:1646  */
    break;

  case 12:
#line 283 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2757 "surface.c" /* yacc.c:1646  */
    break;

  case 13:
#line 285 "surface.yy" /* yacc.c:1646  */
    {
			  int lineNr = lineNumber;
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr) &&
			      !directoryManager.alreadySeen(directory, fileName))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2770 "surface.c" /* yacc.c:1646  */
    break;

  case 14:
#line 294 "surface.yy" /* yacc.c:1646  */
    {
			  cout << directoryManager.getCwd() << '\n';
			}
#line 2778 "surface.c" /* yacc.c:1646  */
    break;

  case 15:
#line 297 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2784 "surface.c" /* yacc.c:1646  */
    break;

  case 16:
#line 299 "surface.yy" /* yacc.c:1646  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			}
#line 2798 "surface.c" /* yacc.c:1646  */
    break;

  case 17:
#line 308 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2804 "surface.c" /* yacc.c:1646  */
    break;

  case 18:
#line 310 "surface.yy" /* yacc.c:1646  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			}
#line 2818 "surface.c" /* yacc.c:1646  */
    break;

  case 19:
#line 320 "surface.yy" /* yacc.c:1646  */
    {
			  const char* path = directoryManager.popd();
			  if (path != 0)
			    cout << path << '\n';
			  else
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": popd failed");
			    }
			}
#line 2833 "surface.c" /* yacc.c:1646  */
    break;

  case 20:
#line 330 "surface.yy" /* yacc.c:1646  */
    { lexerStringMode(); }
#line 2839 "surface.c" /* yacc.c:1646  */
    break;

  case 21:
#line 332 "surface.yy" /* yacc.c:1646  */
    {
			  returnValueDump = system((string("ls") + (yyvsp[0].yyString)).c_str());
			}
#line 2847 "surface.c" /* yacc.c:1646  */
    break;

  case 22:
#line 335 "surface.yy" /* yacc.c:1646  */
    { lexerStringMode(); }
#line 2853 "surface.c" /* yacc.c:1646  */
    break;

  case 23:
#line 337 "surface.yy" /* yacc.c:1646  */
    {
			  returnValueDump = system((string("ls -l") + (yyvsp[0].yyString)).c_str());
			}
#line 2861 "surface.c" /* yacc.c:1646  */
    break;

  case 24:
#line 341 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			}
#line 2870 "surface.c" /* yacc.c:1646  */
    break;

  case 25:
#line 346 "surface.yy" /* yacc.c:1646  */
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			}
#line 2881 "surface.c" /* yacc.c:1646  */
    break;

  case 26:
#line 379 "surface.yy" /* yacc.c:1646  */
    {
                          (yyval.yyModuleExpression) =  new ModuleExpression((yyvsp[-1].yyToken));
                        }
#line 2889 "surface.c" /* yacc.c:1646  */
    break;

  case 27:
#line 383 "surface.yy" /* yacc.c:1646  */
    {
                          (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-1].yyToken));
                        }
#line 2897 "surface.c" /* yacc.c:1646  */
    break;

  case 31:
#line 390 "surface.yy" /* yacc.c:1646  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), (yyvsp[0].yyModuleExpression));
			}
#line 2905 "surface.c" /* yacc.c:1646  */
    break;

  case 32:
#line 394 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyModuleExpression) = new ModuleExpression(t);
			}
#line 2916 "surface.c" /* yacc.c:1646  */
    break;

  case 34:
#line 404 "surface.yy" /* yacc.c:1646  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), (yyvsp[0].yyModuleExpression));
			}
#line 2924 "surface.c" /* yacc.c:1646  */
    break;

  case 39:
#line 416 "surface.yy" /* yacc.c:1646  */
    {
                          (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[0].yyToken));
                        }
#line 2932 "surface.c" /* yacc.c:1646  */
    break;

  case 40:
#line 423 "surface.yy" /* yacc.c:1646  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), currentRenaming);
			  currentRenaming = 0;
			}
#line 2941 "surface.c" /* yacc.c:1646  */
    break;

  case 41:
#line 430 "surface.yy" /* yacc.c:1646  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-3].yyModuleExpression), *((yyvsp[-1].yyViewExpressionVector)));
			  delete (yyvsp[-1].yyViewExpressionVector);
			}
#line 2950 "surface.c" /* yacc.c:1646  */
    break;

  case 42:
#line 437 "surface.yy" /* yacc.c:1646  */
    {
			  (yyval.yyModuleExpression) = (yyvsp[-1].yyModuleExpression);
			}
#line 2958 "surface.c" /* yacc.c:1646  */
    break;

  case 43:
#line 446 "surface.yy" /* yacc.c:1646  */
    {
			  (yyval.yyViewExpression) = new ViewExpression((yyvsp[-3].yyViewExpression), *((yyvsp[-1].yyViewExpressionVector)));
			  delete (yyvsp[-1].yyViewExpressionVector);
			}
#line 2967 "surface.c" /* yacc.c:1646  */
    break;

  case 44:
#line 451 "surface.yy" /* yacc.c:1646  */
    {
			  (yyval.yyViewExpression) = new ViewExpression((yyvsp[0].yyToken));
			}
#line 2975 "surface.c" /* yacc.c:1646  */
    break;

  case 45:
#line 457 "surface.yy" /* yacc.c:1646  */
    {
			  (yyvsp[-2].yyViewExpressionVector)->append((yyvsp[0].yyViewExpression));
			  (yyval.yyViewExpressionVector) = (yyvsp[-2].yyViewExpressionVector);
			}
#line 2984 "surface.c" /* yacc.c:1646  */
    break;

  case 46:
#line 462 "surface.yy" /* yacc.c:1646  */
    {
			  Vector<ViewExpression*>* t =  new Vector<ViewExpression*>();
			  t->append((yyvsp[0].yyViewExpression));
			  (yyval.yyViewExpressionVector) = t;
			}
#line 2994 "surface.c" /* yacc.c:1646  */
    break;

  case 47:
#line 473 "surface.yy" /* yacc.c:1646  */
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			}
#line 3003 "surface.c" /* yacc.c:1646  */
    break;

  case 48:
#line 478 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			}
#line 3011 "surface.c" /* yacc.c:1646  */
    break;

  case 54:
#line 493 "surface.yy" /* yacc.c:1646  */
    {
			  currentRenaming->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3019 "surface.c" /* yacc.c:1646  */
    break;

  case 55:
#line 497 "surface.yy" /* yacc.c:1646  */
    {
			  currentRenaming->addLabelMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3027 "surface.c" /* yacc.c:1646  */
    break;

  case 56:
#line 500 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 3033 "surface.c" /* yacc.c:1646  */
    break;

  case 57:
#line 501 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
#line 3039 "surface.c" /* yacc.c:1646  */
    break;

  case 58:
#line 502 "surface.yy" /* yacc.c:1646  */
    {}
#line 3045 "surface.c" /* yacc.c:1646  */
    break;

  case 59:
#line 503 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->addStratMapping((yyvsp[0].yyToken)); }
#line 3051 "surface.c" /* yacc.c:1646  */
    break;

  case 60:
#line 505 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->addStratTarget((yyvsp[0].yyToken)); }
#line 3057 "surface.c" /* yacc.c:1646  */
    break;

  case 61:
#line 510 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 3063 "surface.c" /* yacc.c:1646  */
    break;

  case 62:
#line 511 "surface.yy" /* yacc.c:1646  */
    {}
#line 3069 "surface.c" /* yacc.c:1646  */
    break;

  case 63:
#line 512 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 3075 "surface.c" /* yacc.c:1646  */
    break;

  case 66:
#line 522 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 3081 "surface.c" /* yacc.c:1646  */
    break;

  case 67:
#line 523 "surface.yy" /* yacc.c:1646  */
    {}
#line 3087 "surface.c" /* yacc.c:1646  */
    break;

  case 68:
#line 524 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 3093 "surface.c" /* yacc.c:1646  */
    break;

  case 71:
#line 531 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setPrec((yyvsp[0].yyToken)); }
#line 3099 "surface.c" /* yacc.c:1646  */
    break;

  case 72:
#line 532 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3105 "surface.c" /* yacc.c:1646  */
    break;

  case 73:
#line 533 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setGather(tokenSequence); }
#line 3111 "surface.c" /* yacc.c:1646  */
    break;

  case 74:
#line 534 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3117 "surface.c" /* yacc.c:1646  */
    break;

  case 75:
#line 535 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setFormat(tokenSequence); }
#line 3123 "surface.c" /* yacc.c:1646  */
    break;

  case 76:
#line 536 "surface.yy" /* yacc.c:1646  */
    { lexerLatexMode(); }
#line 3129 "surface.c" /* yacc.c:1646  */
    break;

  case 77:
#line 537 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setLatexMacro((yyvsp[-1].yyString)); }
#line 3135 "surface.c" /* yacc.c:1646  */
    break;

  case 78:
#line 543 "surface.yy" /* yacc.c:1646  */
    { lexerIdMode(); }
#line 3141 "surface.c" /* yacc.c:1646  */
    break;

  case 79:
#line 545 "surface.yy" /* yacc.c:1646  */
    {
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			  interpreter.setCurrentView(new SyntacticView((yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CV;
			}
#line 3151 "surface.c" /* yacc.c:1646  */
    break;

  case 80:
#line 553 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addFrom((yyvsp[-5].yyModuleExpression));
			  CV->addTo((yyvsp[-3].yyModuleExpression));
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[-9].yyToken)).code(), CV);
			  CV->finishView();
			}
#line 3164 "surface.c" /* yacc.c:1646  */
    break;

  case 82:
#line 564 "surface.yy" /* yacc.c:1646  */
    {}
#line 3170 "surface.c" /* yacc.c:1646  */
    break;

  case 83:
#line 568 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping " << QUOTE("->") << " in variable declaration.");
			}
#line 3179 "surface.c" /* yacc.c:1646  */
    break;

  case 84:
#line 572 "surface.yy" /* yacc.c:1646  */
    {}
#line 3185 "surface.c" /* yacc.c:1646  */
    break;

  case 85:
#line 576 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3193 "surface.c" /* yacc.c:1646  */
    break;

  case 86:
#line 579 "surface.yy" /* yacc.c:1646  */
    {}
#line 3199 "surface.c" /* yacc.c:1646  */
    break;

  case 87:
#line 580 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 3205 "surface.c" /* yacc.c:1646  */
    break;

  case 91:
#line 586 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyToken) = (yyvsp[-1].yyToken); }
#line 3211 "surface.c" /* yacc.c:1646  */
    break;

  case 92:
#line 588 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			}
#line 3222 "surface.c" /* yacc.c:1646  */
    break;

  case 93:
#line 597 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			}
#line 3234 "surface.c" /* yacc.c:1646  */
    break;

  case 94:
#line 605 "surface.yy" /* yacc.c:1646  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3242 "surface.c" /* yacc.c:1646  */
    break;

  case 95:
#line 609 "surface.yy" /* yacc.c:1646  */
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			}
#line 3251 "surface.c" /* yacc.c:1646  */
    break;

  case 96:
#line 614 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1);
			}
#line 3265 "surface.c" /* yacc.c:1646  */
    break;

  case 97:
#line 624 "surface.yy" /* yacc.c:1646  */
    {
			  if (lexerBubble[0].code() == Token::encode("term"))
			    {
			      //
			      //	Op->term mapping.
			      //
			      CV->addOpTermMapping(opDescription, lexerBubble);
			    }
			  else
			    {
			      //
			      //	Generic op->op mapping.
			      //
			      Token::peelParens(opDescription);  // remove any enclosing parens from op name
			      CV->addOpMapping(opDescription);
			      Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			      CV->addOpTarget(lexerBubble);
			    }
			}
#line 3289 "surface.c" /* yacc.c:1646  */
    break;

  case 98:
#line 646 "surface.yy" /* yacc.c:1646  */
    {
			  strategyCall.resize(1);
			  strategyCall[0] = (yyvsp[0].yyToken);
			}
#line 3298 "surface.c" /* yacc.c:1646  */
    break;

  case 99:
#line 650 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3304 "surface.c" /* yacc.c:1646  */
    break;

  case 100:
#line 652 "surface.yy" /* yacc.c:1646  */
    {
			  // Adds the identifier and parentheses to the lexer bubble
			  int bubbleSize = lexerBubble.length();
			  strategyCall.resize(bubbleSize + 3);
			  strategyCall[0] = (yyvsp[-3].yyToken);
			  strategyCall[1] = (yyvsp[-2].yyToken);
			  for (int i = 0; i < bubbleSize; i++)
			    strategyCall[2 + i] = lexerBubble[i];
			  strategyCall[bubbleSize + 2] = (yyvsp[0].yyToken);
			}
#line 3319 "surface.c" /* yacc.c:1646  */
    break;

  case 101:
#line 664 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addStratMapping((yyvsp[0].yyToken));
			}
#line 3327 "surface.c" /* yacc.c:1646  */
    break;

  case 102:
#line 668 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addStratTarget((yyvsp[-1].yyToken));
			}
#line 3335 "surface.c" /* yacc.c:1646  */
    break;

  case 103:
#line 672 "surface.yy" /* yacc.c:1646  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3343 "surface.c" /* yacc.c:1646  */
    break;

  case 104:
#line 676 "surface.yy" /* yacc.c:1646  */
    {
			  if (lexerBubble[0].code() == Token::encode("expr"))
			    {
			      //
			      //	Strat->expr mapping.
			      //
			      CV->addStratExprMapping(strategyCall, lexerBubble);
			    }
			  else if (strategyCall.length() == 1 && lexerBubble.length() == 1)
			    {
			      //
			      //	Generic strat->strat mapping.
			      //
			      CV->addStratMapping(strategyCall[0]);
			      CV->addStratTarget(lexerBubble[0]);
			    }
			  else {
			    IssueWarning(LineNumber(strategyCall[0].lineNumber()) <<
			      ": bad syntax for strategy mapping.");
			  }
			}
#line 3369 "surface.c" /* yacc.c:1646  */
    break;

  case 105:
#line 699 "surface.yy" /* yacc.c:1646  */
    {}
#line 3375 "surface.c" /* yacc.c:1646  */
    break;

  case 106:
#line 701 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			}
#line 3386 "surface.c" /* yacc.c:1646  */
    break;

  case 107:
#line 709 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3392 "surface.c" /* yacc.c:1646  */
    break;

  case 108:
#line 710 "surface.yy" /* yacc.c:1646  */
    {}
#line 3398 "surface.c" /* yacc.c:1646  */
    break;

  case 109:
#line 716 "surface.yy" /* yacc.c:1646  */
    { lexerIdMode(); }
#line 3404 "surface.c" /* yacc.c:1646  */
    break;

  case 110:
#line 718 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3414 "surface.c" /* yacc.c:1646  */
    break;

  case 111:
#line 724 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[0].yyToken));
			}
#line 3424 "surface.c" /* yacc.c:1646  */
    break;

  case 112:
#line 731 "surface.yy" /* yacc.c:1646  */
    {}
#line 3430 "surface.c" /* yacc.c:1646  */
    break;

  case 113:
#line 733 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  store(t);
			}
#line 3441 "surface.c" /* yacc.c:1646  */
    break;

  case 114:
#line 741 "surface.yy" /* yacc.c:1646  */
    {}
#line 3447 "surface.c" /* yacc.c:1646  */
    break;

  case 115:
#line 742 "surface.yy" /* yacc.c:1646  */
    {}
#line 3453 "surface.c" /* yacc.c:1646  */
    break;

  case 118:
#line 750 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer->addParameter2((yyvsp[-2].yyToken), (yyvsp[0].yyModuleExpression));
			}
#line 3461 "surface.c" /* yacc.c:1646  */
    break;

  case 119:
#line 755 "surface.yy" /* yacc.c:1646  */
    {}
#line 3467 "surface.c" /* yacc.c:1646  */
    break;

  case 120:
#line 757 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			}
#line 3477 "surface.c" /* yacc.c:1646  */
    break;

  case 121:
#line 765 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[0].yyToken);  // needed for line number
			}
#line 3488 "surface.c" /* yacc.c:1646  */
    break;

  case 123:
#line 774 "surface.yy" /* yacc.c:1646  */
    {}
#line 3494 "surface.c" /* yacc.c:1646  */
    break;

  case 127:
#line 781 "surface.yy" /* yacc.c:1646  */
    {}
#line 3500 "surface.c" /* yacc.c:1646  */
    break;

  case 128:
#line 785 "surface.yy" /* yacc.c:1646  */
    {
			  CM->addImport((yyvsp[-1].yyToken), (yyvsp[0].yyModuleExpression));
			}
#line 3508 "surface.c" /* yacc.c:1646  */
    break;

  case 129:
#line 789 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3514 "surface.c" /* yacc.c:1646  */
    break;

  case 130:
#line 790 "surface.yy" /* yacc.c:1646  */
    { CM->addSortDecl(tokenSequence); }
#line 3520 "surface.c" /* yacc.c:1646  */
    break;

  case 131:
#line 792 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3526 "surface.c" /* yacc.c:1646  */
    break;

  case 132:
#line 793 "surface.yy" /* yacc.c:1646  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3532 "surface.c" /* yacc.c:1646  */
    break;

  case 133:
#line 795 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON, 1); }
#line 3538 "surface.c" /* yacc.c:1646  */
    break;

  case 134:
#line 796 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3544 "surface.c" /* yacc.c:1646  */
    break;

  case 135:
#line 797 "surface.yy" /* yacc.c:1646  */
    {}
#line 3550 "surface.c" /* yacc.c:1646  */
    break;

  case 136:
#line 799 "surface.yy" /* yacc.c:1646  */
    {}
#line 3556 "surface.c" /* yacc.c:1646  */
    break;

  case 137:
#line 801 "surface.yy" /* yacc.c:1646  */
    {}
#line 3562 "surface.c" /* yacc.c:1646  */
    break;

  case 138:
#line 803 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1); }
#line 3568 "surface.c" /* yacc.c:1646  */
    break;

  case 139:
#line 804 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3574 "surface.c" /* yacc.c:1646  */
    break;

  case 140:
#line 805 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3580 "surface.c" /* yacc.c:1646  */
    break;

  case 141:
#line 807 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1);  }
#line 3586 "surface.c" /* yacc.c:1646  */
    break;

  case 142:
#line 808 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3592 "surface.c" /* yacc.c:1646  */
    break;

  case 143:
#line 809 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3598 "surface.c" /* yacc.c:1646  */
    break;

  case 144:
#line 810 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3604 "surface.c" /* yacc.c:1646  */
    break;

  case 145:
#line 812 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3610 "surface.c" /* yacc.c:1646  */
    break;

  case 146:
#line 813 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3616 "surface.c" /* yacc.c:1646  */
    break;

  case 147:
#line 814 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3622 "surface.c" /* yacc.c:1646  */
    break;

  case 148:
#line 816 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3628 "surface.c" /* yacc.c:1646  */
    break;

  case 149:
#line 817 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3634 "surface.c" /* yacc.c:1646  */
    break;

  case 150:
#line 818 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3640 "surface.c" /* yacc.c:1646  */
    break;

  case 151:
#line 819 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3646 "surface.c" /* yacc.c:1646  */
    break;

  case 152:
#line 821 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3652 "surface.c" /* yacc.c:1646  */
    break;

  case 153:
#line 822 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3658 "surface.c" /* yacc.c:1646  */
    break;

  case 154:
#line 823 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3664 "surface.c" /* yacc.c:1646  */
    break;

  case 155:
#line 825 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3670 "surface.c" /* yacc.c:1646  */
    break;

  case 156:
#line 826 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3676 "surface.c" /* yacc.c:1646  */
    break;

  case 157:
#line 827 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3682 "surface.c" /* yacc.c:1646  */
    break;

  case 158:
#line 828 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3688 "surface.c" /* yacc.c:1646  */
    break;

  case 159:
#line 830 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ASSIGN, 1); }
#line 3694 "surface.c" /* yacc.c:1646  */
    break;

  case 160:
#line 831 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3700 "surface.c" /* yacc.c:1646  */
    break;

  case 161:
#line 832 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3706 "surface.c" /* yacc.c:1646  */
    break;

  case 162:
#line 834 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ASSIGN, 1); }
#line 3712 "surface.c" /* yacc.c:1646  */
    break;

  case 163:
#line 835 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3718 "surface.c" /* yacc.c:1646  */
    break;

  case 164:
#line 836 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3724 "surface.c" /* yacc.c:1646  */
    break;

  case 165:
#line 837 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3730 "surface.c" /* yacc.c:1646  */
    break;

  case 166:
#line 839 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3736 "surface.c" /* yacc.c:1646  */
    break;

  case 167:
#line 843 "surface.yy" /* yacc.c:1646  */
    {}
#line 3742 "surface.c" /* yacc.c:1646  */
    break;

  case 168:
#line 845 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON, 1); }
#line 3748 "surface.c" /* yacc.c:1646  */
    break;

  case 169:
#line 846 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3754 "surface.c" /* yacc.c:1646  */
    break;

  case 170:
#line 847 "surface.yy" /* yacc.c:1646  */
    { CM->setFlag(SymbolType::MESSAGE); }
#line 3760 "surface.c" /* yacc.c:1646  */
    break;

  case 171:
#line 850 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3768 "surface.c" /* yacc.c:1646  */
    break;

  case 172:
#line 855 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3775 "surface.c" /* yacc.c:1646  */
    break;

  case 173:
#line 858 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3782 "surface.c" /* yacc.c:1646  */
    break;

  case 174:
#line 861 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3788 "surface.c" /* yacc.c:1646  */
    break;

  case 175:
#line 862 "surface.yy" /* yacc.c:1646  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3794 "surface.c" /* yacc.c:1646  */
    break;

  case 176:
#line 865 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeDeclsConsistent();
			}
#line 3807 "surface.c" /* yacc.c:1646  */
    break;

  case 177:
#line 875 "surface.yy" /* yacc.c:1646  */
    {}
#line 3813 "surface.c" /* yacc.c:1646  */
    break;

  case 178:
#line 876 "surface.yy" /* yacc.c:1646  */
    {}
#line 3819 "surface.c" /* yacc.c:1646  */
    break;

  case 181:
#line 884 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3826 "surface.c" /* yacc.c:1646  */
    break;

  case 182:
#line 888 "surface.yy" /* yacc.c:1646  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3832 "surface.c" /* yacc.c:1646  */
    break;

  case 183:
#line 889 "surface.yy" /* yacc.c:1646  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3838 "surface.c" /* yacc.c:1646  */
    break;

  case 186:
#line 896 "surface.yy" /* yacc.c:1646  */
    { singleton[0] = (yyvsp[0].yyToken); CM->addOpDecl(singleton); }
#line 3844 "surface.c" /* yacc.c:1646  */
    break;

  case 187:
#line 897 "surface.yy" /* yacc.c:1646  */
    { CM->addOpDecl(lexerBubble); }
#line 3850 "surface.c" /* yacc.c:1646  */
    break;

  case 190:
#line 903 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			}
#line 3859 "surface.c" /* yacc.c:1646  */
    break;

  case 195:
#line 915 "surface.yy" /* yacc.c:1646  */
    { CM->addStratDecl((yyvsp[0].yyToken)); }
#line 3865 "surface.c" /* yacc.c:1646  */
    break;

  case 198:
#line 926 "surface.yy" /* yacc.c:1646  */
    {}
#line 3871 "surface.c" /* yacc.c:1646  */
    break;

  case 199:
#line 927 "surface.yy" /* yacc.c:1646  */
    {}
#line 3877 "surface.c" /* yacc.c:1646  */
    break;

  case 200:
#line 931 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 3885 "surface.c" /* yacc.c:1646  */
    break;

  case 201:
#line 937 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping stray " << QUOTE(":") << " in operator declaration.");

			}
#line 3895 "surface.c" /* yacc.c:1646  */
    break;

  case 202:
#line 942 "surface.yy" /* yacc.c:1646  */
    {}
#line 3901 "surface.c" /* yacc.c:1646  */
    break;

  case 204:
#line 947 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3910 "surface.c" /* yacc.c:1646  */
    break;

  case 205:
#line 952 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3919 "surface.c" /* yacc.c:1646  */
    break;

  case 206:
#line 959 "surface.yy" /* yacc.c:1646  */
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			}
#line 3928 "surface.c" /* yacc.c:1646  */
    break;

  case 208:
#line 966 "surface.yy" /* yacc.c:1646  */
    {}
#line 3934 "surface.c" /* yacc.c:1646  */
    break;

  case 209:
#line 969 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 3940 "surface.c" /* yacc.c:1646  */
    break;

  case 210:
#line 970 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 3946 "surface.c" /* yacc.c:1646  */
    break;

  case 212:
#line 974 "surface.yy" /* yacc.c:1646  */
    {}
#line 3952 "surface.c" /* yacc.c:1646  */
    break;

  case 213:
#line 978 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			}
#line 3961 "surface.c" /* yacc.c:1646  */
    break;

  case 214:
#line 982 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3967 "surface.c" /* yacc.c:1646  */
    break;

  case 215:
#line 984 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
#line 3975 "surface.c" /* yacc.c:1646  */
    break;

  case 216:
#line 989 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3981 "surface.c" /* yacc.c:1646  */
    break;

  case 217:
#line 990 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3987 "surface.c" /* yacc.c:1646  */
    break;

  case 218:
#line 993 "surface.yy" /* yacc.c:1646  */
    {}
#line 3993 "surface.c" /* yacc.c:1646  */
    break;

  case 219:
#line 994 "surface.yy" /* yacc.c:1646  */
    {}
#line 3999 "surface.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1002 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			}
#line 4007 "surface.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1006 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			}
#line 4015 "surface.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1010 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			}
#line 4023 "surface.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1016 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::ASSOC);
			}
#line 4031 "surface.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1020 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::COMM);
			}
#line 4039 "surface.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1023 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); }
#line 4045 "surface.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1024 "surface.yy" /* yacc.c:1646  */
    { CM->setIdentity(lexerBubble); }
#line 4051 "surface.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1026 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::IDEM);
			}
#line 4059 "surface.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1030 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::ITER);
			}
#line 4067 "surface.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1033 "surface.yy" /* yacc.c:1646  */
    { CM->setPrec((yyvsp[0].yyToken)); }
#line 4073 "surface.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1034 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 4079 "surface.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1035 "surface.yy" /* yacc.c:1646  */
    { CM->setGather(tokenSequence); }
#line 4085 "surface.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1036 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 4091 "surface.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1037 "surface.yy" /* yacc.c:1646  */
    { CM->setFormat(tokenSequence); }
#line 4097 "surface.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1038 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 4103 "surface.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1039 "surface.yy" /* yacc.c:1646  */
    { CM->setStrat(tokenSequence); }
#line 4109 "surface.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1040 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 4115 "surface.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1041 "surface.yy" /* yacc.c:1646  */
    { CM->setStrat(tokenSequence); }
#line 4121 "surface.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1042 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 4127 "surface.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1043 "surface.yy" /* yacc.c:1646  */
    { CM->setPoly(tokenSequence); }
#line 4133 "surface.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1045 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MEMO);
			}
#line 4141 "surface.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1049 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::CTOR);
			}
#line 4149 "surface.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1053 "surface.yy" /* yacc.c:1646  */
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			}
#line 4158 "surface.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1057 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 4164 "surface.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1058 "surface.yy" /* yacc.c:1646  */
    { CM->setFrozen(tokenSequence); }
#line 4170 "surface.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1060 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::CONFIG);
			}
#line 4178 "surface.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1064 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::OBJECT);
			}
#line 4186 "surface.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1068 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 4194 "surface.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1072 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 4202 "surface.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1075 "surface.yy" /* yacc.c:1646  */
    { lexerLatexMode(); }
#line 4208 "surface.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1076 "surface.yy" /* yacc.c:1646  */
    { CM->setLatexMacro((yyvsp[-1].yyString)); }
#line 4214 "surface.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1077 "surface.yy" /* yacc.c:1646  */
    {}
#line 4220 "surface.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1079 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::DITTO);
			}
#line 4228 "surface.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1089 "surface.yy" /* yacc.c:1646  */
    {}
#line 4234 "surface.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1092 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 4240 "surface.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1093 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 4246 "surface.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1096 "surface.yy" /* yacc.c:1646  */
    {}
#line 4252 "surface.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1097 "surface.yy" /* yacc.c:1646  */
    {}
#line 4258 "surface.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1100 "surface.yy" /* yacc.c:1646  */
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[0].yyToken), tokenSequence); }
#line 4264 "surface.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1101 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4270 "surface.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1102 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4276 "surface.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1103 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4282 "surface.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1109 "surface.yy" /* yacc.c:1646  */
    {}
#line 4288 "surface.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1111 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			}
#line 4297 "surface.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1117 "surface.yy" /* yacc.c:1646  */
    {}
#line 4303 "surface.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1119 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
#line 4311 "surface.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1127 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 4317 "surface.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1128 "surface.yy" /* yacc.c:1646  */
    {}
#line 4323 "surface.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1131 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 4329 "surface.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1132 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 4335 "surface.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1133 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 4341 "surface.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1134 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 4347 "surface.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1141 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  (yyval.yyToken) = t;
			}
#line 4361 "surface.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1152 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4367 "surface.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1153 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4373 "surface.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1154 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4379 "surface.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1157 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4385 "surface.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1158 "surface.yy" /* yacc.c:1646  */
    {}
#line 4391 "surface.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1159 "surface.yy" /* yacc.c:1646  */
    {}
#line 4397 "surface.c" /* yacc.c:1646  */
    break;

  case 387:
#line 1240 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 1); }
#line 4403 "surface.c" /* yacc.c:1646  */
    break;

  case 388:
#line 1242 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setCurrentModule(lexerBubble);
			}
#line 4411 "surface.c" /* yacc.c:1646  */
    break;

  case 389:
#line 1245 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 1); }
#line 4417 "surface.c" /* yacc.c:1646  */
    break;

  case 390:
#line 1247 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			}
#line 4426 "surface.c" /* yacc.c:1646  */
    break;

  case 391:
#line 1252 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4435 "surface.c" /* yacc.c:1646  */
    break;

  case 392:
#line 1257 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			}
#line 4445 "surface.c" /* yacc.c:1646  */
    break;

  case 393:
#line 1264 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4454 "surface.c" /* yacc.c:1646  */
    break;

  case 394:
#line 1269 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			}
#line 4464 "surface.c" /* yacc.c:1646  */
    break;

  case 395:
#line 1276 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4473 "surface.c" /* yacc.c:1646  */
    break;

  case 396:
#line 1281 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			}
#line 4483 "surface.c" /* yacc.c:1646  */
    break;

  case 397:
#line 1288 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4492 "surface.c" /* yacc.c:1646  */
    break;

  case 398:
#line 1293 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[-3].yyBool));
			}
#line 4502 "surface.c" /* yacc.c:1646  */
    break;

  case 399:
#line 1300 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4512 "surface.c" /* yacc.c:1646  */
    break;

  case 400:
#line 1306 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4522 "surface.c" /* yacc.c:1646  */
    break;

  case 401:
#line 1312 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4533 "surface.c" /* yacc.c:1646  */
    break;

  case 402:
#line 1319 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4543 "surface.c" /* yacc.c:1646  */
    break;

  case 403:
#line 1325 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4554 "surface.c" /* yacc.c:1646  */
    break;

  case 404:
#line 1332 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4564 "surface.c" /* yacc.c:1646  */
    break;

  case 405:
#line 1338 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4574 "surface.c" /* yacc.c:1646  */
    break;

  case 406:
#line 1344 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4584 "surface.c" /* yacc.c:1646  */
    break;

  case 407:
#line 1350 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4594 "surface.c" /* yacc.c:1646  */
    break;

  case 408:
#line 1356 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool), true);
			}
#line 4604 "surface.c" /* yacc.c:1646  */
    break;

  case 409:
#line 1363 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4613 "surface.c" /* yacc.c:1646  */
    break;

  case 410:
#line 1368 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.check(lexerBubble);
			}
#line 4623 "surface.c" /* yacc.c:1646  */
    break;

  case 411:
#line 1375 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4634 "surface.c" /* yacc.c:1646  */
    break;

  case 412:
#line 1382 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[-2].yySearchKind), (yyvsp[-3].yyBool));
			}
#line 4644 "surface.c" /* yacc.c:1646  */
    break;

  case 413:
#line 1388 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4654 "surface.c" /* yacc.c:1646  */
    break;

  case 414:
#line 1394 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[-2].yyBool), number);
			}
#line 4664 "surface.c" /* yacc.c:1646  */
    break;

  case 415:
#line 1400 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4674 "surface.c" /* yacc.c:1646  */
    break;

  case 416:
#line 1406 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number);
			}
#line 4684 "surface.c" /* yacc.c:1646  */
    break;

  case 417:
#line 1412 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4694 "surface.c" /* yacc.c:1646  */
    break;

  case 418:
#line 1418 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, (yyvsp[-4].yyBool));
			}
#line 4704 "surface.c" /* yacc.c:1646  */
    break;

  case 419:
#line 1425 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4714 "surface.c" /* yacc.c:1646  */
    break;

  case 420:
#line 1431 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.getVariants(lexerBubble, number, (yyvsp[-3].yyBool), (yyvsp[-5].yyBool));
			}
#line 4724 "surface.c" /* yacc.c:1646  */
    break;

  case 421:
#line 1437 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.cont((yyvsp[-1].yyInt64), (yyvsp[-3].yyBool));
			}
#line 4732 "surface.c" /* yacc.c:1646  */
    break;

  case 422:
#line 1441 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	test is a generic command to call code with a term for development purposes.
			  //
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4744 "surface.c" /* yacc.c:1646  */
    break;

  case 423:
#line 1449 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.test(lexerBubble);
			    //interpreter.newNarrow(lexerBubble);

			}
#line 4756 "surface.c" /* yacc.c:1646  */
    break;

  case 424:
#line 1457 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4765 "surface.c" /* yacc.c:1646  */
    break;

  case 425:
#line 1462 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			}
#line 4775 "surface.c" /* yacc.c:1646  */
    break;

  case 426:
#line 1468 "surface.yy" /* yacc.c:1646  */
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			}
#line 4785 "surface.c" /* yacc.c:1646  */
    break;

  case 427:
#line 1473 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4791 "surface.c" /* yacc.c:1646  */
    break;

  case 428:
#line 1475 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[-2].yyBool));
			}
#line 4800 "surface.c" /* yacc.c:1646  */
    break;

  case 429:
#line 1479 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4806 "surface.c" /* yacc.c:1646  */
    break;

  case 430:
#line 1481 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[-2].yyBool));
			}
#line 4815 "surface.c" /* yacc.c:1646  */
    break;

  case 431:
#line 1485 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4821 "surface.c" /* yacc.c:1646  */
    break;

  case 432:
#line 1487 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[-2].yyBool));
			}
#line 4830 "surface.c" /* yacc.c:1646  */
    break;

  case 433:
#line 1491 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4836 "surface.c" /* yacc.c:1646  */
    break;

  case 434:
#line 1493 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[-2].yyBool));
			}
#line 4845 "surface.c" /* yacc.c:1646  */
    break;

  case 435:
#line 1498 "surface.yy" /* yacc.c:1646  */
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			}
#line 4854 "surface.c" /* yacc.c:1646  */
    break;

  case 436:
#line 1505 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4860 "surface.c" /* yacc.c:1646  */
    break;

  case 437:
#line 1507 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4869 "surface.c" /* yacc.c:1646  */
    break;

  case 438:
#line 1511 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4875 "surface.c" /* yacc.c:1646  */
    break;

  case 439:
#line 1513 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4884 "surface.c" /* yacc.c:1646  */
    break;

  case 440:
#line 1517 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4890 "surface.c" /* yacc.c:1646  */
    break;

  case 441:
#line 1519 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			}
#line 4899 "surface.c" /* yacc.c:1646  */
    break;

  case 442:
#line 1523 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4905 "surface.c" /* yacc.c:1646  */
    break;

  case 443:
#line 1525 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			}
#line 4914 "surface.c" /* yacc.c:1646  */
    break;

  case 444:
#line 1530 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showModules(true);
			}
#line 4922 "surface.c" /* yacc.c:1646  */
    break;

  case 445:
#line 1534 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showViews(true);
			}
#line 4930 "surface.c" /* yacc.c:1646  */
    break;

  case 446:
#line 1537 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4936 "surface.c" /* yacc.c:1646  */
    break;

  case 447:
#line 1539 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			}
#line 4945 "surface.c" /* yacc.c:1646  */
    break;

  case 448:
#line 1543 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4951 "surface.c" /* yacc.c:1646  */
    break;

  case 449:
#line 1545 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			}
#line 4960 "surface.c" /* yacc.c:1646  */
    break;

  case 450:
#line 1549 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4966 "surface.c" /* yacc.c:1646  */
    break;

  case 451:
#line 1551 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			}
#line 4975 "surface.c" /* yacc.c:1646  */
    break;

  case 452:
#line 1555 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4981 "surface.c" /* yacc.c:1646  */
    break;

  case 453:
#line 1557 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			}
#line 4990 "surface.c" /* yacc.c:1646  */
    break;

  case 454:
#line 1561 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4996 "surface.c" /* yacc.c:1646  */
    break;

  case 455:
#line 1563 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			}
#line 5005 "surface.c" /* yacc.c:1646  */
    break;

  case 456:
#line 1567 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 5011 "surface.c" /* yacc.c:1646  */
    break;

  case 457:
#line 1569 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			}
#line 5020 "surface.c" /* yacc.c:1646  */
    break;

  case 458:
#line 1573 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 5026 "surface.c" /* yacc.c:1646  */
    break;

  case 459:
#line 1575 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showStrats();
			}
#line 5035 "surface.c" /* yacc.c:1646  */
    break;

  case 460:
#line 1579 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 5041 "surface.c" /* yacc.c:1646  */
    break;

  case 461:
#line 1581 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSds();
			}
#line 5050 "surface.c" /* yacc.c:1646  */
    break;

  case 462:
#line 1585 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 5056 "surface.c" /* yacc.c:1646  */
    break;

  case 463:
#line 1587 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			}
#line 5065 "surface.c" /* yacc.c:1646  */
    break;

  case 464:
#line 1591 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 5071 "surface.c" /* yacc.c:1646  */
    break;

  case 465:
#line 1593 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			}
#line 5080 "surface.c" /* yacc.c:1646  */
    break;

  case 466:
#line 1598 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchPath((yyvsp[-1].yyInt64));
			}
#line 5088 "surface.c" /* yacc.c:1646  */
    break;

  case 467:
#line 1602 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchPathLabels((yyvsp[-1].yyInt64));
			}
#line 5096 "surface.c" /* yacc.c:1646  */
    break;

  case 468:
#line 1606 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchGraph();
			}
#line 5104 "surface.c" /* yacc.c:1646  */
    break;

  case 469:
#line 1609 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 5110 "surface.c" /* yacc.c:1646  */
    break;

  case 470:
#line 1611 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			}
#line 5119 "surface.c" /* yacc.c:1646  */
    break;

  case 471:
#line 1619 "surface.yy" /* yacc.c:1646  */
    {
			  globalAdvisoryFlag = alwaysAdviseFlag ? true : (yyvsp[-1].yyBool);
			}
#line 5127 "surface.c" /* yacc.c:1646  */
    break;

  case 472:
#line 1623 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 5135 "surface.c" /* yacc.c:1646  */
    break;

  case 473:
#line 1627 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[-1].yyBool));
			}
#line 5143 "surface.c" /* yacc.c:1646  */
    break;

  case 474:
#line 1631 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[-1].yyBool));
			}
#line 5151 "surface.c" /* yacc.c:1646  */
    break;

  case 475:
#line 1635 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[-1].yyBool));
			}
#line 5159 "surface.c" /* yacc.c:1646  */
    break;

  case 476:
#line 1639 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[-1].yyBool));
			}
#line 5167 "surface.c" /* yacc.c:1646  */
    break;

  case 477:
#line 1643 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[-1].yyBool));
			}
#line 5175 "surface.c" /* yacc.c:1646  */
    break;

  case 478:
#line 1647 "surface.yy" /* yacc.c:1646  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 5183 "surface.c" /* yacc.c:1646  */
    break;

  case 479:
#line 1651 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setPrintFlag((yyvsp[-2].yyPrintFlags), (yyvsp[-1].yyBool));
			}
#line 5191 "surface.c" /* yacc.c:1646  */
    break;

  case 480:
#line 1655 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[-1].yyBool));
			}
#line 5199 "surface.c" /* yacc.c:1646  */
    break;

  case 481:
#line 1659 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[-1].yyBool));
			}
#line 5207 "surface.c" /* yacc.c:1646  */
    break;

  case 482:
#line 1663 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag((yyvsp[-2].yyFlags), (yyvsp[-1].yyBool));
			}
#line 5215 "surface.c" /* yacc.c:1646  */
    break;

  case 483:
#line 1667 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[-1].yyBool));
			}
#line 5223 "surface.c" /* yacc.c:1646  */
    break;

  case 484:
#line 1670 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 5229 "surface.c" /* yacc.c:1646  */
    break;

  case 485:
#line 1671 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 5235 "surface.c" /* yacc.c:1646  */
    break;

  case 486:
#line 1673 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setAutoImport((yyvsp[-5].yyImportMode), (yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 5243 "surface.c" /* yacc.c:1646  */
    break;

  case 487:
#line 1676 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 5249 "surface.c" /* yacc.c:1646  */
    break;

  case 488:
#line 1677 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 5255 "surface.c" /* yacc.c:1646  */
    break;

  case 489:
#line 1679 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setOmodInclude((yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 5263 "surface.c" /* yacc.c:1646  */
    break;

  case 490:
#line 1683 "surface.yy" /* yacc.c:1646  */
    {
			  globalVerboseFlag = (yyvsp[-1].yyBool);
			}
#line 5271 "surface.c" /* yacc.c:1646  */
    break;

  case 491:
#line 1687 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[-1].yyBool));
			}
#line 5279 "surface.c" /* yacc.c:1646  */
    break;

  case 492:
#line 1691 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[-1].yyBool));
			}
#line 5287 "surface.c" /* yacc.c:1646  */
    break;

  case 493:
#line 1695 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[-1].yyBool));
			}
#line 5295 "surface.c" /* yacc.c:1646  */
    break;

  case 494:
#line 1699 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[-1].yyBool));
			}
#line 5303 "surface.c" /* yacc.c:1646  */
    break;

  case 495:
#line 1703 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[-1].yyBool));
			}
#line 5311 "surface.c" /* yacc.c:1646  */
    break;

  case 496:
#line 1710 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			}
#line 5319 "surface.c" /* yacc.c:1646  */
    break;

  case 497:
#line 1714 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			}
#line 5327 "surface.c" /* yacc.c:1646  */
    break;

  case 498:
#line 1718 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
#line 5335 "surface.c" /* yacc.c:1646  */
    break;

  case 499:
#line 1722 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			}
#line 5343 "surface.c" /* yacc.c:1646  */
    break;

  case 500:
#line 1729 "surface.yy" /* yacc.c:1646  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 5351 "surface.c" /* yacc.c:1646  */
    break;

  case 501:
#line 1733 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 5359 "surface.c" /* yacc.c:1646  */
    break;

  case 502:
#line 1739 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 5365 "surface.c" /* yacc.c:1646  */
    break;

  case 504:
#line 1746 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; }
#line 5371 "surface.c" /* yacc.c:1646  */
    break;

  case 505:
#line 1747 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; }
#line 5377 "surface.c" /* yacc.c:1646  */
    break;

  case 506:
#line 1748 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; }
#line 5383 "surface.c" /* yacc.c:1646  */
    break;

  case 507:
#line 1749 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; }
#line 5389 "surface.c" /* yacc.c:1646  */
    break;

  case 508:
#line 1750 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; }
#line 5395 "surface.c" /* yacc.c:1646  */
    break;

  case 509:
#line 1751 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; }
#line 5401 "surface.c" /* yacc.c:1646  */
    break;

  case 510:
#line 1752 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; }
#line 5407 "surface.c" /* yacc.c:1646  */
    break;

  case 511:
#line 1753 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; }
#line 5413 "surface.c" /* yacc.c:1646  */
    break;

  case 512:
#line 1754 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; }
#line 5419 "surface.c" /* yacc.c:1646  */
    break;

  case 513:
#line 1755 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; }
#line 5425 "surface.c" /* yacc.c:1646  */
    break;

  case 514:
#line 1756 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_DISAMBIG_CONST; }
#line 5431 "surface.c" /* yacc.c:1646  */
    break;

  case 515:
#line 1759 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE; }
#line 5437 "surface.c" /* yacc.c:1646  */
    break;

  case 516:
#line 1760 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; }
#line 5443 "surface.c" /* yacc.c:1646  */
    break;

  case 517:
#line 1761 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; }
#line 5449 "surface.c" /* yacc.c:1646  */
    break;

  case 518:
#line 1762 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; }
#line 5455 "surface.c" /* yacc.c:1646  */
    break;

  case 519:
#line 1763 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; }
#line 5461 "surface.c" /* yacc.c:1646  */
    break;

  case 520:
#line 1764 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_MB; }
#line 5467 "surface.c" /* yacc.c:1646  */
    break;

  case 521:
#line 1765 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; }
#line 5473 "surface.c" /* yacc.c:1646  */
    break;

  case 522:
#line 1766 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_RL; }
#line 5479 "surface.c" /* yacc.c:1646  */
    break;

  case 523:
#line 1767 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SD; }
#line 5485 "surface.c" /* yacc.c:1646  */
    break;

  case 524:
#line 1768 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; }
#line 5491 "surface.c" /* yacc.c:1646  */
    break;

  case 525:
#line 1769 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; }
#line 5497 "surface.c" /* yacc.c:1646  */
    break;

  case 526:
#line 1770 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; }
#line 5503 "surface.c" /* yacc.c:1646  */
    break;

  case 527:
#line 1773 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5509 "surface.c" /* yacc.c:1646  */
    break;

  case 528:
#line 1774 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5515 "surface.c" /* yacc.c:1646  */
    break;

  case 529:
#line 1777 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5521 "surface.c" /* yacc.c:1646  */
    break;

  case 530:
#line 1778 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5527 "surface.c" /* yacc.c:1646  */
    break;

  case 531:
#line 1781 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5533 "surface.c" /* yacc.c:1646  */
    break;

  case 532:
#line 1782 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5539 "surface.c" /* yacc.c:1646  */
    break;

  case 533:
#line 1785 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5545 "surface.c" /* yacc.c:1646  */
    break;

  case 534:
#line 1786 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5551 "surface.c" /* yacc.c:1646  */
    break;

  case 535:
#line 1791 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::NARROW; }
#line 5557 "surface.c" /* yacc.c:1646  */
    break;

  case 536:
#line 1792 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; }
#line 5563 "surface.c" /* yacc.c:1646  */
    break;

  case 537:
#line 1793 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::SEARCH; }
#line 5569 "surface.c" /* yacc.c:1646  */
    break;

  case 538:
#line 1794 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::SMT_SEARCH; }
#line 5575 "surface.c" /* yacc.c:1646  */
    break;

  case 539:
#line 1795 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::VU_NARROW; }
#line 5581 "surface.c" /* yacc.c:1646  */
    break;

  case 540:
#line 1796 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::FVU_NARROW; }
#line 5587 "surface.c" /* yacc.c:1646  */
    break;

  case 541:
#line 1799 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5593 "surface.c" /* yacc.c:1646  */
    break;

  case 542:
#line 1800 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5599 "surface.c" /* yacc.c:1646  */
    break;

  case 543:
#line 1803 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5605 "surface.c" /* yacc.c:1646  */
    break;

  case 544:
#line 1804 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5611 "surface.c" /* yacc.c:1646  */
    break;

  case 545:
#line 1807 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5617 "surface.c" /* yacc.c:1646  */
    break;

  case 546:
#line 1808 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5623 "surface.c" /* yacc.c:1646  */
    break;

  case 547:
#line 1811 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); }
#line 5629 "surface.c" /* yacc.c:1646  */
    break;

  case 548:
#line 1812 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyInt64) = NONE; }
#line 5635 "surface.c" /* yacc.c:1646  */
    break;

  case 549:
#line 1815 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::PROTECTING; }
#line 5641 "surface.c" /* yacc.c:1646  */
    break;

  case 550:
#line 1816 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::EXTENDING; }
#line 5647 "surface.c" /* yacc.c:1646  */
    break;

  case 551:
#line 1817 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::INCLUDING; }
#line 5653 "surface.c" /* yacc.c:1646  */
    break;

  case 553:
#line 1827 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5659 "surface.c" /* yacc.c:1646  */
    break;

  case 555:
#line 1831 "surface.yy" /* yacc.c:1646  */
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); }
#line 5665 "surface.c" /* yacc.c:1646  */
    break;

  case 558:
#line 1843 "surface.yy" /* yacc.c:1646  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5671 "surface.c" /* yacc.c:1646  */
    break;

  case 561:
#line 1846 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0);  }
#line 5677 "surface.c" /* yacc.c:1646  */
    break;

  case 563:
#line 1851 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5683 "surface.c" /* yacc.c:1646  */
    break;

  case 565:
#line 1853 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5689 "surface.c" /* yacc.c:1646  */
    break;

  case 568:
#line 1859 "surface.yy" /* yacc.c:1646  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5695 "surface.c" /* yacc.c:1646  */
    break;

  case 570:
#line 1861 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5701 "surface.c" /* yacc.c:1646  */
    break;

  case 573:
#line 1876 "surface.yy" /* yacc.c:1646  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5707 "surface.c" /* yacc.c:1646  */
    break;

  case 576:
#line 1879 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5713 "surface.c" /* yacc.c:1646  */
    break;

  case 578:
#line 1888 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5719 "surface.c" /* yacc.c:1646  */
    break;

  case 580:
#line 1890 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5725 "surface.c" /* yacc.c:1646  */
    break;

  case 582:
#line 1892 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5731 "surface.c" /* yacc.c:1646  */
    break;

  case 585:
#line 1902 "surface.yy" /* yacc.c:1646  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5737 "surface.c" /* yacc.c:1646  */
    break;

  case 587:
#line 1904 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5743 "surface.c" /* yacc.c:1646  */
    break;

  case 589:
#line 1906 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5749 "surface.c" /* yacc.c:1646  */
    break;

  case 592:
#line 1916 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5755 "surface.c" /* yacc.c:1646  */
    break;

  case 594:
#line 1918 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5761 "surface.c" /* yacc.c:1646  */
    break;

  case 597:
#line 1929 "surface.yy" /* yacc.c:1646  */
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			}
#line 5771 "surface.c" /* yacc.c:1646  */
    break;

  case 599:
#line 1935 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5777 "surface.c" /* yacc.c:1646  */
    break;

  case 602:
#line 1945 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5783 "surface.c" /* yacc.c:1646  */
    break;

  case 604:
#line 1947 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5789 "surface.c" /* yacc.c:1646  */
    break;

  case 607:
#line 1957 "surface.yy" /* yacc.c:1646  */
    { number2 = Token::codeToInt64(lexerBubble[2].code()); }
#line 5795 "surface.c" /* yacc.c:1646  */
    break;

  case 609:
#line 1959 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5801 "surface.c" /* yacc.c:1646  */
    break;

  case 612:
#line 1964 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0, 1); }
#line 5807 "surface.c" /* yacc.c:1646  */
    break;

  case 615:
#line 1970 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 1, 1); }
#line 5813 "surface.c" /* yacc.c:1646  */
    break;

  case 617:
#line 1972 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON | END_COMMAND, 0); }
#line 5819 "surface.c" /* yacc.c:1646  */
    break;

  case 619:
#line 1975 "surface.yy" /* yacc.c:1646  */
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(lexerBubble[0]);
			}
#line 5829 "surface.c" /* yacc.c:1646  */
    break;

  case 657:
#line 2014 "surface.yy" /* yacc.c:1646  */
    {}
#line 5835 "surface.c" /* yacc.c:1646  */
    break;

  case 659:
#line 2019 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			}
#line 5845 "surface.c" /* yacc.c:1646  */
    break;

  case 662:
#line 2030 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  interpreter.addSelected(singleton);
			}
#line 5854 "surface.c" /* yacc.c:1646  */
    break;

  case 663:
#line 2035 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.addSelected(lexerBubble);
			}
#line 5862 "surface.c" /* yacc.c:1646  */
    break;


#line 5866 "surface.c" /* yacc.c:1646  */
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
      yyerror (parseResult, YY_("syntax error"));
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
        yyerror (parseResult, yymsgp);
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
                      yytoken, &yylval, parseResult);
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
                  yystos[yystate], yyvsp, parseResult);
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
  yyerror (parseResult, YY_("memory exhausted"));
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
                  yytoken, &yylval, parseResult);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, parseResult);
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
#line 2065 "surface.yy" /* yacc.c:1906  */


static void
yyerror(UserLevelRewritingContext::ParseResult* /*parseResult*/, char *s)
{
  if (!(UserLevelRewritingContext::interrupted()))
    IssueWarning(LineNumber(lineNumber) << ": " << s);
}

void
cleanUpModuleExpression()
{
  //cout << "cleanUpModuleExpression() called" << endl;
  //
  //	Delete pieces of a partly built module expression.
  //
  delete currentRenaming;
  currentRenaming = 0;
}

void
cleanUpParser()
{
  interpreter.makeClean(lineNumber);
}

void
deepSelfDestructViewExpressionVector(Vector<ViewExpression*>* viewExpressions)
{
  FOR_EACH_CONST(i, Vector<ViewExpression*>, *viewExpressions)
    (*i)->deepSelfDestruct();
  delete viewExpressions;
}

void
missingSpace(const Token& token)
{
  IssueWarning(LineNumber(token.lineNumber()) << ": missing space between " <<
	       QUOTE(token) << " and period.");
}
