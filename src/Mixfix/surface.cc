/* A Bison parser, made by GNU Bison 3.3.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2019 Free Software Foundation,
   Inc.

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.3.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 27 "surface.yy" /* yacc.c:337  */

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

//	interface class definitions
#include "dagNode.hh"

//	core class definitions
#include "lineNumber.hh"
#include "module.hh"

//	higher class definitions
#include "narrowingSequenceSearch3.hh"

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
int variantOptions;

static void yyerror(UserLevelRewritingContext::ParseResult* parseResult, char *s);

void cleanUpModuleExpression();
void cleanUpParser();
void deepSelfDestructViewExpressionVector(Vector<ViewExpression*>* viewExpressions);
void missingSpace(const Token& token);

#line 146 "surface.c" /* yacc.c:337  */
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
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
    KW_FILTERED = 342,
    KW_RESUME = 343,
    KW_ABORT = 344,
    KW_STEP = 345,
    KW_WHERE = 346,
    KW_CREDUCE = 347,
    KW_SREDUCE = 348,
    KW_DUMP = 349,
    KW_PROFILE = 350,
    KW_NUMBER = 351,
    KW_RAT = 352,
    KW_COLOR = 353,
    SIMPLE_NUMBER = 354,
    KW_PWD = 355,
    KW_CD = 356,
    KW_PUSHD = 357,
    KW_POPD = 358,
    KW_LS = 359,
    KW_LL = 360,
    KW_LOAD = 361,
    KW_SLOAD = 362,
    KW_QUIT = 363,
    KW_EOF = 364,
    KW_TEST = 365,
    KW_SMT_SEARCH = 366,
    KW_VU_NARROW = 367,
    KW_FVU_NARROW = 368,
    KW_FOLD = 369,
    KW_ENDM = 370,
    KW_IMPORT = 371,
    KW_ENDV = 372,
    KW_SORT = 373,
    KW_SUBSORT = 374,
    KW_OP = 375,
    KW_OPS = 376,
    KW_MSGS = 377,
    KW_VAR = 378,
    KW_CLASS = 379,
    KW_SUBCLASS = 380,
    KW_DSTRAT = 381,
    KW_MB = 382,
    KW_CMB = 383,
    KW_EQ = 384,
    KW_CEQ = 385,
    KW_RL = 386,
    KW_CRL = 387,
    KW_SD = 388,
    KW_CSD = 389,
    KW_IS = 390,
    KW_FROM = 391,
    KW_ARROW = 392,
    KW_ARROW2 = 393,
    KW_PARTIAL = 394,
    KW_IF = 395,
    KW_ASSIGN = 396,
    KW_LABEL = 397,
    KW_TO = 398,
    KW_COLON2 = 399,
    KW_ASSOC = 400,
    KW_COMM = 401,
    KW_ID = 402,
    KW_IDEM = 403,
    KW_ITER = 404,
    KW_LEFT = 405,
    KW_RIGHT = 406,
    KW_PREC = 407,
    KW_GATHER = 408,
    KW_METADATA = 409,
    KW_STRAT = 410,
    KW_ASTRAT = 411,
    KW_POLY = 412,
    KW_MEMO = 413,
    KW_FROZEN = 414,
    KW_CTOR = 415,
    KW_LATEX = 416,
    KW_SPECIAL = 417,
    KW_CONFIG = 418,
    KW_OBJ = 419,
    KW_MSG = 420,
    KW_DITTO = 421,
    KW_FORMAT = 422,
    KW_ID_HOOK = 423,
    KW_OP_HOOK = 424,
    KW_TERM_HOOK = 425,
    KW_IN = 426,
    KW_FILTER = 427,
    KW_DELAY = 428,
    IDENTIFIER = 429,
    NUMERIC_ID = 430,
    ENDS_IN_DOT = 431,
    FORCE_LOOKAHEAD = 432
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 106 "surface.yy" /* yacc.c:352  */

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

#line 381 "surface.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (UserLevelRewritingContext::ParseResult* parseResult);

#endif /* !YY_YY_SURFACE_H_INCLUDED  */

/* Second part of user prologue.  */
#line 122 "surface.yy" /* yacc.c:354  */

int yylex(YYSTYPE* lvalp);

#line 400 "surface.c" /* yacc.c:354  */

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
typedef unsigned short yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short yytype_int16;
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
#  define YYSIZE_T unsigned
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

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
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
#define YYLAST   2966

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  193
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  292
/* YYNRULES -- Number of rules.  */
#define YYNRULES  749
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1113

#define YYUNDEFTOK  2
#define YYMAXUTOK   432

/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                                \
  ((unsigned) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     144,   145,   156,   155,   150,     2,   146,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   142,     2,
     147,   143,     2,     2,   192,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   148,     2,   149,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   157,   151,   158,     2,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   141,   152,   153,   154,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   257,   257,   259,   264,   265,   266,   267,   273,   273,
     283,   283,   293,   293,   303,   307,   307,   318,   318,   329,
     340,   340,   345,   345,   350,   355,   388,   392,   396,   397,
     398,   399,   403,   412,   413,   419,   420,   423,   424,   425,
     432,   439,   446,   455,   460,   466,   471,   483,   488,   482,
     494,   495,   498,   499,   502,   506,   510,   511,   510,   513,
     513,   520,   520,   522,   525,   526,   532,   532,   534,   537,
     538,   541,   542,   542,   544,   544,   546,   546,   553,   555,
     553,   573,   574,   577,   582,   585,   589,   590,   590,   592,
     593,   596,   597,   607,   615,   606,   624,   623,   655,   660,
     660,   674,   673,   682,   681,   709,   710,   719,   719,   726,
     728,   726,   741,   742,   751,   752,   755,   756,   759,   765,
     766,   774,   783,   784,   787,   787,   790,   791,   794,   799,
     799,   802,   802,   805,   806,   805,   809,   811,   813,   814,
     813,   817,   818,   819,   817,   822,   823,   822,   826,   827,
     828,   826,   831,   832,   831,   835,   836,   837,   835,   840,
     841,   840,   844,   845,   846,   844,   849,   849,   855,   856,
     855,   859,   865,   864,   871,   871,   874,   885,   886,   889,
     890,   893,   898,   899,   902,   903,   906,   907,   910,   911,
     912,   919,   919,   921,   922,   925,   928,   930,   936,   937,
     940,   946,   952,   955,   956,   961,   968,   975,   976,   979,
     980,   983,   984,   987,   992,   992,   999,  1000,  1003,  1004,
    1007,  1008,  1011,  1015,  1019,  1025,  1029,  1033,  1033,  1035,
    1039,  1043,  1044,  1044,  1046,  1046,  1048,  1048,  1050,  1050,
    1052,  1052,  1054,  1058,  1062,  1067,  1067,  1069,  1073,  1077,
    1081,  1085,  1085,  1087,  1088,  1098,  1099,  1102,  1103,  1106,
    1107,  1110,  1111,  1112,  1113,  1119,  1121,  1127,  1129,  1137,
    1138,  1141,  1142,  1143,  1143,  1150,  1162,  1163,  1163,  1167,
    1167,  1169,  1175,  1175,  1175,  1175,  1175,  1178,  1178,  1178,
    1179,  1179,  1179,  1182,  1182,  1182,  1182,  1183,  1183,  1183,
    1183,  1183,  1183,  1186,  1186,  1186,  1187,  1187,  1187,  1187,
    1188,  1188,  1188,  1188,  1188,  1191,  1191,  1197,  1197,  1197,
    1197,  1197,  1197,  1197,  1197,  1198,  1198,  1201,  1201,  1201,
    1201,  1204,  1204,  1207,  1207,  1207,  1207,  1207,  1207,  1207,
    1208,  1208,  1208,  1209,  1209,  1209,  1209,  1209,  1209,  1209,
    1209,  1210,  1210,  1213,  1213,  1213,  1213,  1213,  1213,  1213,
    1216,  1216,  1216,  1219,  1219,  1219,  1219,  1219,  1219,  1219,
    1220,  1220,  1220,  1220,  1220,  1220,  1221,  1221,  1221,  1221,
    1222,  1222,  1222,  1222,  1223,  1223,  1223,  1250,  1250,  1255,
    1255,  1262,  1261,  1274,  1273,  1286,  1285,  1298,  1297,  1310,
    1309,  1322,  1321,  1335,  1334,  1348,  1347,  1360,  1359,  1373,
    1372,  1385,  1384,  1398,  1397,  1415,  1414,  1427,  1426,  1439,
    1438,  1451,  1450,  1463,  1462,  1476,  1475,  1487,  1492,  1491,
    1508,  1507,  1518,  1524,  1524,  1530,  1530,  1536,  1536,  1542,
    1542,  1548,  1548,  1557,  1557,  1563,  1563,  1569,  1569,  1575,
    1575,  1581,  1585,  1589,  1589,  1595,  1595,  1601,  1601,  1607,
    1607,  1613,  1613,  1619,  1619,  1625,  1625,  1631,  1631,  1637,
    1637,  1643,  1643,  1649,  1653,  1657,  1661,  1661,  1670,  1674,
    1678,  1682,  1686,  1690,  1694,  1698,  1702,  1706,  1710,  1714,
    1718,  1722,  1723,  1722,  1728,  1729,  1728,  1734,  1738,  1742,
    1746,  1750,  1754,  1761,  1765,  1769,  1773,  1780,  1784,  1791,
    1791,  1798,  1799,  1800,  1801,  1802,  1803,  1804,  1805,  1806,
    1807,  1808,  1811,  1812,  1813,  1814,  1815,  1816,  1817,  1818,
    1819,  1820,  1821,  1822,  1825,  1826,  1829,  1830,  1833,  1834,
    1837,  1838,  1843,  1844,  1845,  1846,  1847,  1850,  1851,  1854,
    1855,  1858,  1859,  1862,  1863,  1866,  1867,  1870,  1871,  1874,
    1877,  1878,  1879,  1888,  1889,  1889,  1893,  1893,  1895,  1905,
    1905,  1907,  1908,  1908,  1913,  1913,  1915,  1915,  1917,  1921,
    1921,  1923,  1923,  1925,  1939,  1939,  1941,  1941,  1943,  1944,
    1944,  1950,  1949,  1956,  1955,  1961,  1961,  1963,  1967,  1969,
    1969,  1971,  1971,  1973,  1980,  1980,  1982,  1983,  1983,  1992,
    1992,  1994,  1994,  1996,  1996,  1998,  2006,  2006,  2008,  2008,
    2010,  2010,  2012,  2020,  2020,  2022,  2022,  2024,  2033,  2032,
    2039,  2039,  2041,  2049,  2049,  2051,  2051,  2053,  2061,  2061,
    2063,  2063,  2065,  2068,  2068,  2070,  2079,  2079,  2081,  2081,
    2083,  2094,  2094,  2094,  2094,  2094,  2094,  2094,  2094,  2094,
    2094,  2097,  2097,  2097,  2097,  2097,  2097,  2097,  2097,  2097,
    2101,  2101,  2101,  2101,  2101,  2101,  2101,  2101,  2105,  2105,
    2105,  2105,  2105,  2105,  2105,  2105,  2105,  2109,  2109,  2109,
    2109,  2109,  2109,  2109,  2109,  2109,  2113,  2113,  2113,  2113,
    2113,  2113,  2113,  2113,  2117,  2117,  2117,  2117,  2117,  2117,
    2117,  2117,  2121,  2121,  2121,  2121,  2121,  2121,  2121,  2121,
    2125,  2125,  2125,  2125,  2125,  2125,  2125,  2125,  2125,  2131,
    2132,  2135,  2136,  2139,  2146,  2147,  2150,  2155,  2162,  2162,
    2162,  2162,  2162,  2162,  2162,  2162,  2162,  2162,  2165,  2166
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
  "KW_COUNT", "KW_DEBUG", "KW_IRREDUNDANT", "KW_FILTERED", "KW_RESUME",
  "KW_ABORT", "KW_STEP", "KW_WHERE", "KW_CREDUCE", "KW_SREDUCE", "KW_DUMP",
  "KW_PROFILE", "KW_NUMBER", "KW_RAT", "KW_COLOR", "SIMPLE_NUMBER",
  "KW_PWD", "KW_CD", "KW_PUSHD", "KW_POPD", "KW_LS", "KW_LL", "KW_LOAD",
  "KW_SLOAD", "KW_QUIT", "KW_EOF", "KW_TEST", "KW_SMT_SEARCH",
  "KW_VU_NARROW", "KW_FVU_NARROW", "KW_FOLD", "KW_ENDM", "KW_IMPORT",
  "KW_ENDV", "KW_SORT", "KW_SUBSORT", "KW_OP", "KW_OPS", "KW_MSGS",
  "KW_VAR", "KW_CLASS", "KW_SUBCLASS", "KW_DSTRAT", "KW_MB", "KW_CMB",
  "KW_EQ", "KW_CEQ", "KW_RL", "KW_CRL", "KW_SD", "KW_CSD", "KW_IS",
  "KW_FROM", "KW_ARROW", "KW_ARROW2", "KW_PARTIAL", "KW_IF", "KW_ASSIGN",
  "':'", "'='", "'('", "')'", "'.'", "'<'", "'['", "']'", "','", "'|'",
  "KW_LABEL", "KW_TO", "KW_COLON2", "'+'", "'*'", "'{'", "'}'", "KW_ASSOC",
  "KW_COMM", "KW_ID", "KW_IDEM", "KW_ITER", "KW_LEFT", "KW_RIGHT",
  "KW_PREC", "KW_GATHER", "KW_METADATA", "KW_STRAT", "KW_ASTRAT",
  "KW_POLY", "KW_MEMO", "KW_FROZEN", "KW_CTOR", "KW_LATEX", "KW_SPECIAL",
  "KW_CONFIG", "KW_OBJ", "KW_MSG", "KW_DITTO", "KW_FORMAT", "KW_ID_HOOK",
  "KW_OP_HOOK", "KW_TERM_HOOK", "KW_IN", "KW_FILTER", "KW_DELAY",
  "IDENTIFIER", "NUMERIC_ID", "ENDS_IN_DOT", "FORCE_LOOKAHEAD", "'@'",
  "$accept", "top", "item", "directive", "$@1", "$@2", "$@3", "$@4", "$@5",
  "$@6", "$@7", "moduleExprDot", "moduleExpr", "moduleExpr2",
  "moduleExpr3", "renameExpr", "instantExpr", "parenExpr", "viewExpr",
  "instantArgs", "renaming", "$@8", "$@9", "renaming2", "mappingList",
  "mapping", "$@10", "$@11", "$@12", "fromSpec", "$@13", "fromStratSpec",
  "toAttributes", "$@14", "toAttributeList", "toAttribute", "$@15", "$@16",
  "$@17", "view", "$@18", "$@19", "viewDecList", "skipStrayArrow",
  "viewDeclaration", "$@20", "sortDot", "viewEndOpMap", "$@21", "$@22",
  "$@23", "strategyCall", "$@24", "viewStratMap", "$@25", "$@26",
  "endBubble", "parenBubble", "$@27", "module", "$@28", "$@29", "dot",
  "parameters", "parameterList", "parameter", "colon2", "badType",
  "typeDot", "startModule", "decList", "declaration", "$@30", "$@31",
  "$@32", "$@33", "$@34", "$@35", "$@36", "$@37", "$@38", "$@39", "$@40",
  "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47", "$@48", "$@49",
  "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "$@57", "$@58",
  "classDef", "cPairList", "cPair", "varNameList", "opNameList",
  "simpleOpName", "domainRangeAttr", "stratDeclKeyword", "stratIdList",
  "stratId", "stratSignature", "stratAttributes", "stratAttrList",
  "skipStrayColon", "dra2", "rangeAttr", "typeAttr", "arrow", "typeList",
  "typeName", "$@59", "sortNames", "attributes", "attributeList",
  "idKeyword", "attribute", "$@60", "$@61", "$@62", "$@63", "$@64", "$@65",
  "$@66", "$@67", "identity", "idList", "hookList", "hook", "expectedIs",
  "expectedDot", "sortNameList", "subsortList", "$@68", "sortName",
  "sortNameFrag", "$@69", "sortNameFrags", "$@70", "token", "tokenBarDot",
  "tokenBarColon", "sortToken", "endsInDot", "inert", "identifier",
  "startKeyword", "startKeyword2", "midKeyword", "attrKeyword",
  "attrKeyword2", "command", "$@71", "$@72", "$@73", "$@74", "$@75",
  "$@76", "$@77", "$@78", "$@79", "$@80", "$@81", "$@82", "$@83", "$@84",
  "$@85", "$@86", "$@87", "$@88", "$@89", "$@90", "$@91", "$@92", "$@93",
  "$@94", "$@95", "$@96", "$@97", "$@98", "$@99", "$@100", "$@101",
  "$@102", "$@103", "$@104", "$@105", "$@106", "$@107", "$@108", "$@109",
  "$@110", "$@111", "$@112", "$@113", "$@114", "$@115", "$@116", "$@117",
  "printOption", "traceOption", "polarity", "select", "exclude", "conceal",
  "search", "match", "optDebug", "optIrredundant", "optNumber",
  "optOptions", "optionsList", "option", "importMode", "moduleAndTerm",
  "$@118", "inEnd", "$@119", "numberModuleTerm", "$@120", "$@121",
  "numberModuleTerm1", "$@122", "$@123", "numberModuleTerm2", "$@124",
  "$@125", "optionsNumbersModuleTerm", "$@126", "$@127", "$@128",
  "optionsNumbersModuleTerm1", "$@129", "$@130", "$@131",
  "optionsNumbersModuleTerm2", "$@132", "$@133", "numbersModuleTerm",
  "$@134", "$@135", "numbersModuleTerm1", "$@136", "$@137", "$@138",
  "numbersModuleTerm2", "$@139", "$@140", "$@141", "numbersModuleTerm3",
  "$@142", "$@143", "numbersModuleTerm4", "$@144", "$@145",
  "numbersModuleTerm5", "$@146", "$@147", "numbersModuleTerm6", "$@148",
  "$@149", "miscEndBubble", "$@150", "initialEndBubble", "$@151", "$@152",
  "cTokenBarIn", "cTokenBarLeftIn", "cTokenBarOpenLeftIn",
  "cTokenBarDotNumber", "cTokenBarDotRight", "cTokenBarDotCommaNumber",
  "cTokenBarDotCommaRight", "cTokenBarDotCommaClose",
  "cTokenBarDotOptionToken", "opSelect", "endSelect", "badSelect",
  "cOpNameList", "cSimpleOpName", "cSimpleTokenBarDot", "cOptionToken", YY_NULLPTR
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
     395,   396,    58,    61,    40,    41,    46,    60,    91,    93,
      44,   124,   397,   398,   399,    43,    42,   123,   125,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,   430,
     431,   432,    64
};
# endif

#define YYPACT_NINF -958

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-958)))

#define YYTABLE_NINF -553

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     932,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
     335,   563,    42,  -958,   126,   242,   -11,  -958,  -958,   -76,
     -65,   -39,    -3,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,    85,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,   497,   128,
      20,  1357,   732,   732,   732,    91,    36,   637,   363,   152,
     273,   273,   273,   -17,  -958,  -958,  -958,   108,   273,  -958,
    -958,  -958,   188,   -46,  -958,    93,   110,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,   -87,  -958,  -958,  -958,  -958,
      98,  -958,  -958,  -958,  -958,   732,   732,   -87,   274,   290,
     316,   318,   325,   345,   732,   208,   356,  -958,  1357,  2532,
    -958,  -958,  -958,  -958,   281,   202,  -958,  -958,  -958,  -958,
     270,  -958,   348,  -958,  -958,   258,  -958,   261,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,   257,   273,   273,
     273,   273,   273,   273,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,   273,  -958,  -958,  -958,    29,
     265,   328,  -958,  -958,  -958,  -958,  -958,   273,   273,  -958,
    -958,   238,   240,   244,   273,   273,   273,   273,   246,   494,
     -87,   -87,   249,   256,   305,   -87,  -958,  -958,   -87,   -87,
     -87,   -87,   -87,   -87,   -87,   -87,   -87,   -87,   -87,   -87,
     391,   391,  -958,  -958,  -958,   391,   391,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,   732,  2532,   394,  -958,  -958,  2549,
    2549,  2532,  2532,  -958,   271,   398,  -958,     9,  -958,  2549,
    -958,  2532,   262,   -87,   -78,   -87,   494,   273,   273,   278,
     280,   282,   283,   286,   288,   292,   273,   293,  -958,  -958,
     364,   295,   297,  -958,  -958,  -958,   299,   301,   303,   304,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,   307,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  2566,  -958,  -958,  -958,  -958,  -958,
     -87,   262,  2583,   -87,  -958,  -958,  -958,  2532,  2532,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,   258,  -958,
    -958,  2600,  -958,  1357,   300,  -958,  -958,  -958,  -958,  -958,
    -958,   308,   309,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
     310,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,   273,
    -958,  -958,  -958,  -958,  -958,  -958,   322,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  2532,  -958,  -958,  2617,   -87,  2532,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,    35,  -958,   -60,  1053,   -87,   273,  -958,
    -958,  -958,   317,  -958,  -958,   -87,  2634,   -87,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  2617,  2651,   -87,  1357,  -958,  -958,
    -958,  1053,  1053,   121,   306,   294,  -958,  -958,  -958,  -958,
    -958,   319,  -958,   694,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  2668,
    2685,   -87,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,   311,
       3,  1053,  1053,   320,  1357,  -958,   323,  -958,  1129,  -958,
    -958,  -958,  1433,  1433,  1661,  1357,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
     732,   -87,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    2702,  2702,   -87,  -958,   -83,  -958,  -958,  -958,   314,    59,
    -958,  -958,   326,   -15,   132,   177,  -958,   313,     7,   181,
     195,   329,   330,   140,   144,   189,   206,  -958,  2489,   332,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  1205,  -958,
    -958,  -958,  -958,  -958,  1281,  1509,  -958,  -958,  2489,   337,
     338,   334,   339,   343,   346,   342,   344,   347,   576,  -958,
    -958,  2719,   -87,   732,  2736,   -87,  -958,  -958,  -958,  -958,
    -958,  2549,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,   155,  1357,  1357,  -958,  1129,  -958,  -958,  -958,
    -958,  -958,  -958,  1965,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  1889,  -958,   331,  -958,  -958,  -958,
    -958,  1737,  -958,  1737,   349,  -958,   341,  1889,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,   839,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  2651,  -958,   -87,    11,  -958,  2489,  -958,  1357,   576,
    -958,   340,  -958,    71,   314,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  1737,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  2041,  -958,  -958,  -958,  -958,  2041,
    2193,   351,  -958,   -87,   353,   -87,   354,   -87,   355,   -87,
     359,  1737,  -958,  2341,  -958,   352,   732,   -87,  2753,   -87,
    -958,  -958,   358,  -958,  2489,  -958,  1661,   576,  -958,   357,
     365,   360,  -958,   361,   -22,  -958,  2415,  2489,  -958,  2489,
    -958,  -958,   366,  1813,  -958,  -958,   329,  -958,  -958,   376,
    -958,   373,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,   975,  -958,   333,   -79,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,   374,    -4,
    1585,   375,  -958,    21,  2489,  -958,   377,  1357,   -92,  -958,
    -958,  -958,   331,    96,   207,  -958,   887,   329,  -958,  -958,
    -958,   179,  -958,  -958,  -958,  2193,  1357,   -87,   -87,   -87,
     -87,  2341,   350,   367,  -958,   732,   -87,  2117,  -958,  -958,
    -958,   349,  -958,  -958,   -92,  -958,  -958,  -958,  -958,   381,
    -958,  -958,  -958,  -958,  2489,  -958,  -958,  -958,  -958,  -958,
     368,   382,   362,   392,   369,   401,   402,   403,  -958,   410,
    -958,   412,   416,  -958,  -958,  -958,  -958,   417,  2785,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,   329,  -958,   -87,  2041,
     -87,   397,   414,  2267,   422,   576,  2489,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,   215,  -958,
    -958,  -958,   383,  -958,  2267,  -958,  -958,  -958,  -958,   576,
    2341,  -958,  -958,  -958,   331,   387,   387,   387,   387,   387,
     578,  1357,  1357,  1357,     6,  -958,   387,  -958,  -958,  2341,
     439,  -958,    70,  -958,   -91,   -72,   -62,   -61,   -31,   441,
     444,   444,   444,  -958,  -958,   -28,   436,  -958,   405,   446,
     450,   451,   117,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,   -87,   387,   591,   387,  -958,   -10,   452,    -9,
    -958,  -958,  -958
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   509,   124,   125,    78,   391,   430,   548,   547,   409,
       0,     0,     0,   387,     0,     0,     0,   549,   551,     0,
       0,     0,     0,   393,   395,   389,    14,    15,    17,    19,
      20,    22,    10,    12,    24,    25,   428,   107,     8,     0,
       2,     6,     5,   432,     4,   109,     7,   415,   556,     0,
       0,     0,     0,     0,     0,     0,     0,   522,     0,     0,
       0,     0,     0,     0,   560,   561,   562,     0,     0,   491,
     443,   449,     0,     0,   445,     0,     0,   447,   453,   455,
     457,   459,   461,   463,   465,   467,   469,   471,   476,   536,
     537,   539,   538,   433,   435,     0,   540,   541,   439,   437,
       0,   503,   504,   505,   506,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
     397,   399,   542,   543,   552,     0,   401,   403,   405,   407,
     554,   544,     0,   545,   546,     0,   411,     0,   417,   510,
     349,   333,   350,   334,   335,   336,   337,   340,   338,   341,
     342,   339,   343,   344,   345,   346,   347,   348,   351,   352,
     326,   325,   355,   358,   356,   359,   354,   357,   286,   353,
     360,   361,   329,   322,   324,   330,   323,   320,   321,   318,
     319,   363,   364,   365,   366,   367,   368,   369,   370,   371,
     379,   372,   373,   374,   378,   375,   376,   377,   380,   381,
     331,   382,   383,   384,   385,   386,   317,   328,    79,   327,
     282,   283,   332,   284,   285,   362,   658,   646,   659,   656,
     657,   660,   654,   655,   648,   748,   749,   652,   653,   650,
     392,   563,   564,   651,   431,   410,   494,     0,     0,     0,
       0,     0,     0,     0,   531,   532,   533,   524,   526,   523,
     525,   527,   528,   529,   530,     0,   515,   511,   512,     0,
       0,     0,   516,   517,   518,   519,   520,     0,     0,   534,
     535,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   451,   452,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   105,   106,   388,     0,     0,   441,   394,   396,
     390,    16,    18,    21,    23,    11,    13,   429,   108,     9,
     110,   667,   668,   569,   666,   669,   664,   665,   662,   663,
     416,   571,   572,   661,     0,     0,     0,   423,   419,     0,
       0,     0,     0,   553,     0,     0,   559,     0,   557,     0,
     413,     0,   115,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   514,   513,
       0,     0,     0,   508,   490,   497,     0,     0,     0,     0,
     501,   746,   743,   744,   747,   741,   742,   745,   739,   740,
     492,   738,   444,   450,   475,   473,     0,   446,   448,   454,
     456,   458,   460,   462,   464,   466,   468,   470,   472,   477,
     733,   737,   434,   730,     0,   735,   736,   436,   440,   438,
       0,   115,     0,     0,   398,   400,   425,     0,     0,   604,
     402,   606,   607,   404,   406,   408,   427,   421,     0,   555,
     412,     0,   418,     0,     0,   647,   566,   568,   649,   565,
     495,     0,     0,   485,   479,   481,   484,   482,   478,   489,
       0,   487,   521,   486,   507,   499,   502,   498,   500,     0,
     474,   731,   729,   732,   734,   442,   266,   685,   643,   682,
     683,   686,   680,   681,   684,   679,   574,   645,   570,   578,
     576,   678,   573,     0,   424,   420,     0,     0,     0,   558,
     675,   676,   586,   674,   677,   584,   673,   671,   672,   414,
     588,   589,   670,     0,   117,     0,     0,     0,     0,   480,
     483,   488,     0,   265,   127,     0,     0,     0,   426,   703,
     700,   701,   611,   698,   699,   702,   697,   609,   605,   615,
     613,   696,   608,   422,     0,     0,     0,     0,   114,   120,
     119,     0,     0,     0,    33,    35,    36,    38,    37,    39,
     567,     0,   493,     0,   644,   694,   692,   579,   695,   690,
     691,   693,   688,   689,   575,   583,   581,   687,   577,     0,
       0,     0,   587,   727,   724,   725,   728,   722,   723,   726,
     591,   593,   720,   721,   585,   597,   595,   590,   116,   118,
       0,     0,     0,     0,     0,   496,     0,   111,     0,   129,
     131,   133,     0,     0,     0,     0,   174,   192,   138,   141,
     145,   148,   152,   155,   159,   162,   191,   168,   126,   166,
       0,     0,   633,   612,   637,   635,   711,   709,   616,   618,
     707,   708,   710,   705,   706,   610,   622,   620,   704,   614,
       0,     0,     0,    42,   266,    34,    47,    40,    46,     0,
      44,   176,   286,   288,   289,    32,   128,     0,   268,   268,
     268,   268,     0,   287,   290,   291,   292,   270,     0,     0,
     298,   301,   299,   302,   300,   296,   297,   187,     0,   185,
     186,   293,   294,   295,     0,     0,   183,   172,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   580,
     582,     0,     0,     0,     0,     0,   719,   716,   717,   599,
     715,     0,   718,   713,   714,   592,   603,   601,   712,   594,
     596,    82,     0,     0,     0,    41,     0,   267,    29,    30,
      28,    26,    27,     0,   312,   310,   311,   306,   307,   313,
     314,   308,   309,   303,     0,   273,   275,   276,   304,   305,
     134,     0,   184,     0,    84,   182,     0,     0,   139,   142,
     146,   149,   153,   156,   160,   163,   169,     0,   194,   195,
     638,   634,   642,   640,   636,   617,   623,   619,   627,   625,
     621,     0,   598,     0,     0,    51,     0,    56,     0,     0,
      48,    50,    53,     0,    45,    31,    34,   112,   113,   130,
     269,   272,   132,   271,   270,   277,     0,   209,   210,   214,
     121,   190,   136,   189,     0,   212,   213,   171,    83,     0,
     177,     0,   175,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   212,     0,   193,   199,     0,     0,     0,     0,
     600,   602,     0,    80,     0,    87,     0,     0,    81,     0,
      63,     0,    59,     0,     0,    43,     0,     0,   135,     0,
     208,   206,   219,   202,   123,   137,   268,   288,   289,   178,
     179,     0,   287,   290,   291,   292,   173,   140,   143,   147,
     150,   154,   157,   161,   164,   170,     0,   196,     0,     0,
     639,   641,   628,   624,   632,   630,   626,    90,     0,     0,
       0,     0,    89,   101,     0,    61,     0,     0,    65,    49,
      52,   274,   281,     0,     0,   217,     0,   268,   201,   204,
     205,     0,   188,   211,   122,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   167,     0,     0,     0,    93,    96,
      88,    84,   103,    99,     0,    54,   212,    57,    55,     0,
      64,   279,   278,   215,     0,   225,   226,   222,   229,   230,
       0,     0,     0,     0,     0,     0,     0,     0,   242,   244,
     243,     0,     0,   247,   248,   249,   254,     0,     0,   227,
     221,   207,   203,   180,   181,   144,   151,   158,   165,   197,
     200,   198,   629,   631,    92,    85,   268,   212,     0,     0,
       0,     0,     0,     0,    68,     0,     0,   216,   223,   224,
     231,   232,   250,   236,   238,   240,   245,   251,     0,   234,
     218,   220,   256,    91,     0,    97,    86,   104,   100,     0,
       0,    66,    58,    60,   280,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   260,     0,   255,   228,     0,
       0,    62,     0,   258,     0,     0,     0,     0,     0,     0,
     261,     0,     0,   253,   259,     0,     0,   102,     0,     0,
       0,     0,     0,    70,   233,   257,   237,   239,   241,   246,
     252,   262,   263,   264,   235,    94,    71,    72,    76,    74,
      67,    69,     0,     0,     0,     0,    95,     0,     0,     0,
      73,    77,    75
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -138,  -509,  -958,  -958,  -572,  -568,  -563,  -135,  -133,
    -958,  -958,  -958,  -958,  -958,  -263,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -478,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -345,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,   -94,     0,  -958,  -958,
    -958,  -958,  -723,   186,  -958,    62,  -958,  -799,  -398,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -321,  -241,     4,  -549,  -708,  -958,  -958,
    -161,  -831,  -958,  -958,  -958,  -958,  -312,  -958,  -957,  -824,
    -719,  -958,  -958,  -958,  -958,  -958,  -350,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -668,  -958,  -414,   -13,
    -642,  -167,   -45,  -958,  -396,  -841,  -958,  -958,  -958,   -42,
    -783,  -591,  -958,  -958,  -592,  -593,   -49,  -958,  -586,  -575,
    -106,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,   -27,   635,  -958,  -958,  -958,  -958,  -958,   532,  -958,
    -958,  -958,   221,  -958,   -43,  -958,  -958,  -958,   -73,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -131,  -958,  -958,  -958,    13,  -958,  -958,  -325,  -958,
    -958,   118,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,  -958,
    -958,  -408,  -958,  -116,  -958,  -958,  -958,  -332,  -958,  -533,
    -679,  -958,  -958,  -958,  -958,   -34,  -958,   253,  -958,   254,
    -231,   -48
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    39,    40,    41,   116,   112,   113,   108,   109,   110,
     111,   666,   553,   554,   555,   556,   557,   558,   658,   659,
     657,   732,   863,   800,   801,   802,   860,  1014,   918,   916,
     956,   959,  1042,  1062,  1082,  1083,  1103,  1105,  1104,    42,
      51,   352,   794,   829,   858,   909,  1005,   950,  1007,  1102,
    1008,   911,  1011,   912,   954,  1010,   487,   411,   115,    44,
     118,   421,   809,   444,   513,   514,   551,   821,   875,    45,
     563,   628,   677,   678,   679,   816,   699,   833,   700,   834,
     937,   701,   835,   702,   836,   938,   703,   837,   704,   838,
     939,   705,   839,   706,   840,   940,   708,   707,   841,   766,
     698,   831,   879,   880,   695,   688,   689,   822,   629,   777,
     778,   845,   899,   943,   931,   932,   823,   871,   824,   873,
     825,   869,   924,   927,   988,   989,   990,  1032,  1045,  1056,
    1046,  1047,  1048,  1049,  1050,  1058,  1064,  1054,  1055,   524,
     738,   743,   754,   814,   826,   756,   867,   923,  1016,   559,
     671,   690,   757,   672,   209,   210,   758,   212,   213,   214,
     215,    46,    95,   107,    52,   105,   106,   334,   335,   339,
     340,   341,   342,    54,   349,   441,   119,   351,   428,   498,
     427,   493,   114,    53,   300,   301,   306,   305,   420,   280,
     285,   288,   281,   289,   290,   291,   292,   293,   294,   295,
     296,   297,   298,   299,   279,   469,   356,   518,    50,   267,
     255,   271,    93,    94,    98,   136,    47,    48,    49,   344,
     137,   347,   348,    69,   230,   355,   448,   517,   330,   422,
     423,   488,   526,   527,   574,   630,   631,   509,   545,   544,
     546,   594,   650,   651,   652,   725,   791,   793,   430,   496,
     497,   538,   580,   579,   581,   645,   713,   714,   715,   787,
     848,   849,   903,   945,   946,   633,   711,   712,   781,   846,
     847,   539,   525,   231,   353,   354,   232,   332,   511,   490,
     576,   540,   647,   727,   596,   412,   472,   413,   414,   415,
     416,   333
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,   304,   211,   331,   233,   233,   233,   432,   432,   208,
     234,   235,   852,   310,   489,   433,   673,   432,   896,   691,
     691,   691,   675,   696,   440,   870,   922,   739,   740,   741,
     874,   812,   783,   676,   272,   273,   668,   693,   693,   693,
     669,   278,   599,   600,   832,   670,   635,   881,   390,   237,
     842,   274,   523,   283,  1084,   827,  1040,   233,   233,   302,
     269,   270,   308,   309,   446,   100,   233,   807,   302,   211,
     101,   317,   602,  1086,   930,   366,   320,  1059,   275,    89,
      90,   102,   549,  1087,  1088,   117,   238,   960,   239,   240,
     241,   242,   654,   655,   550,   691,   796,  1085,   797,   667,
     843,   691,   691,   303,   765,   872,   284,   103,   868,   243,
     876,   808,   303,   693,  1089,   779,  1085,  1094,   575,   693,
     693,    91,    92,  1012,   897,   450,  1085,  1085,   853,   854,
     798,   855,  1013,   895,   856,  1110,  1112,   595,   948,   762,
    -329,  -329,  -329,   104,   673,   762,   138,   799,   653,   949,
     675,  1073,   881,   737,   933,   276,   795,  1085,   602,   438,
    1085,   676,   -36,   -36,   668,   953,   139,   439,   669,   905,
     236,   634,   646,   670,   -98,  1044,   944,   933,  1085,  1085,
     857,   789,   268,  1034,   779,   547,   392,   393,  1051,  1052,
    1053,   397,   277,   548,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   407,   408,   409,   862,    96,    97,   734,
     874,   359,   360,   361,   362,   363,   364,   735,   337,   331,
     338,   734,   999,   431,   431,   331,   331,   806,   365,   865,
     282,   391,   367,   431,   934,   331,  1078,  1079,   882,   286,
     371,   372,   726,   726,   884,  1080,   961,   376,   377,   378,
     379,  1081,   391,   391,   962,   885,   287,   391,   391,   445,
     447,   449,   425,   691,   913,   696,  1100,   417,   434,   435,
     307,   418,   419,   796,   601,   797,   602,   311,   442,    89,
      90,   693,   755,  1078,  1079,   991,   233,  -330,  -330,  -330,
     876,   424,  1080,   312,   933,  -327,  -327,  -327,  1081,  -283,
    -283,  -283,   755,   782,   269,   270,   788,   798,   391,   357,
     358,   331,   331,   368,   369,   933,   817,   691,   818,   765,
     313,  1061,   314,  -316,   799,   510,   475,   737,   315,   492,
     451,   452,  -328,  -328,  -328,   693,   -38,   -38,   -38,   460,
    1076,   737,    55,   882,  -284,  -284,  -284,   810,   316,   884,
     -37,   -37,   -37,   318,   494,   495,   963,   964,   813,   319,
     885,  -285,  -285,  -285,  1033,    56,   391,    18,    57,   343,
     345,   813,   346,   350,   491,   370,    58,   331,  1065,  1066,
    1067,  1068,   331,   595,   373,    59,   374,    60,  1075,   432,
     375,    61,   380,   512,   211,   394,   792,  1051,  1052,  1053,
     859,   515,   395,   542,   396,   256,   257,   258,   259,    62,
     260,    63,    64,    65,    66,   426,   437,   436,    67,   443,
     528,   261,  1043,   560,   453,   543,   454,   462,   455,   456,
      68,   564,   457,   578,   458,  1107,   516,  1109,   459,   461,
     904,   463,   522,   464,   262,   465,  1060,   466,   541,   467,
     468,   604,   597,   470,   519,   520,   521,   523,   908,   263,
     264,   265,   603,   562,   656,   605,   602,   211,   736,   661,
     810,   733,  -315,   925,   760,   737,   742,   770,   577,   768,
     769,   772,   771,   774,   773,   775,   828,   649,   815,   776,
     864,   561,   830,   888,   890,   892,   541,   886,   211,   894,
     898,   942,   211,   211,   907,   515,   919,   915,   120,   121,
     914,   122,   123,   917,   926,   936,  1001,   124,   955,   125,
     126,   127,   128,   129,   130,   131,   935,   947,   952,  1018,
     957,   491,   648,   381,  1015,    37,  1021,   710,  1000,   382,
     383,   384,  1038,  1019,   266,  1023,  1024,  1025,   385,   386,
    1020,  1006,   211,   211,  1026,   211,  1027,  1022,   730,   674,
    1028,  1029,   660,   692,   692,   692,   211,  1039,  1017,    70,
    1041,    71,   759,   697,  1057,  1063,   387,   225,   226,   388,
     389,   410,   233,  1069,   132,  1077,  1090,   709,    37,  1095,
    1097,    72,   759,  1096,  1098,  1099,  1108,  1111,   805,   804,
     803,   920,   728,   728,  1101,   431,  1009,   476,   133,   598,
     134,  1036,   687,   687,   993,   910,   844,   694,   784,   992,
      73,   790,    74,    75,    76,    77,    78,    79,    80,    81,
      82,    83,    84,    85,    86,    87,   381,   759,  1031,   692,
    1074,   731,   382,   383,   384,   692,   692,   866,   759,   244,
      99,   385,   386,   767,   135,   759,   336,   759,    88,   499,
     850,   759,   582,   577,   729,   233,   491,   473,   474,     0,
     785,   245,   246,   247,   248,     0,   249,   250,     0,   387,
     225,   226,   388,   389,   211,   211,     0,   674,   687,     0,
     759,   660,   660,     0,   687,   606,     0,     0,     0,   851,
       0,     0,     0,   251,   252,   253,     0,   254,     0,     0,
     759,   160,   161,     0,     0,     0,     0,     0,   759,     0,
       0,     0,     0,   759,     0,     0,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   759,     0,   759,     0,   887,
       0,   889,     0,   891,     0,   893,     0,     0,   759,   211,
       0,     0,     0,   901,     0,   906,   861,     0,     0,     0,
     759,   759,     0,   759,   206,     0,   207,   759,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   883,     0,     0,     0,     0,     0,     0,     0,     0,
     759,     0,     0,     0,     0,     0,     0,     0,   233,     0,
     577,     0,     0,   900,     0,     0,     0,   692,   759,   607,
     608,     0,   609,   610,   611,   612,   613,   614,   615,   616,
     617,   618,   619,   620,   621,   622,   623,   624,   625,     0,
       0,     0,     0,     0,     0,   759,     0,     0,     0,     0,
       0,   759,     0,   995,   996,   997,   998,     0,     0,     0,
       0,     0,  1003,     0,     0,     0,     0,     0,   759,     0,
       0,   692,     0,   626,     0,     0,     0,     0,   211,     0,
       0,     0,     0,   627,   216,   958,   217,     0,   218,     0,
     219,   220,   221,     0,     0,     0,   883,   211,     0,   222,
     223,     0,     0,     0,   994,     0,     0,   233,     0,     0,
       0,     0,  1002,   759,     0,     0,     0,   759,     0,     0,
     759,     0,     0,     0,  1035,     0,  1037,   224,   225,   226,
     227,   228,   229,     0,     0,     0,     0,     0,   759,     0,
       0,     0,    -3,     1,   759,     0,     0,     0,     2,     3,
       4,     5,     0,  -550,  -550,     6,  -550,  -550,     7,     8,
    -552,     9,  -550,   759,  -550,  -550,  -550,  -550,  -550,  -550,
    -550,    10,    11,     0,     0,    12,     0,     0,     0,    13,
       0,     0,     0,    14,   160,   161,     0,     0,     0,     0,
       0,   842,     0,     0,     0,     0,     0,     0,    15,   172,
     173,   174,   175,   176,   177,   178,   179,   180,     0,     0,
       0,     0,   211,   211,   211,     0,     0,    16,  1106,  1070,
    1071,  1072,     0,     0,     0,     0,     0,    17,    18,  -550,
      19,    20,    21,    22,    23,    24,    25,   206,     0,   207,
       0,   843,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    36,  -550,  -550,  -550,   965,   966,   967,   968,
     969,   970,   971,   972,   973,   974,   975,   976,   977,   978,
     979,   980,   981,   982,   983,   984,   985,   986,   987,     0,
    1091,  1092,  1093,     0,     0,     0,    37,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,  -550,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     744,     0,     0,   745,     0,   746,     0,    38,   747,     0,
       0,     0,     0,   819,     0,     0,   748,   749,   750,     0,
     751,   752,     0,     0,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,     0,     0,   753,     0,     0,     0,   941,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,   166,   167,   552,     0,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,     0,     0,
       0,   206,     0,   207,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
       0,   166,   167,   552,     0,   662,   169,   170,   171,   663,
     173,   174,   664,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,     0,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,     0,     0,     0,   206,     0,   665,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   680,   681,   682,   683,     0,   761,   684,    37,
       0,   685,   686,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,     0,     0,   206,     0,   207,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   680,   681,
     682,   683,     0,   763,   684,    37,     0,   685,   686,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,     0,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,     0,     0,     0,   206,
       0,   207,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,     0,   166,
     167,     0,     0,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,     0,     0,     0,   206,     0,   207,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     680,   681,   682,   683,     0,     0,   684,    37,     0,   685,
     686,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,     0,     0,
       0,   206,     0,   207,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   680,   681,   682,   683,
       0,   764,   684,     0,     0,   685,   686,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,     0,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,     0,     0,     0,   206,     0,   207,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   680,   681,   682,   683,     0,   951,   684,     0,
       0,   685,   686,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,     0,     0,   206,     0,   207,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   680,   681,
     682,   683,     0,     0,   684,     0,     0,   685,   686,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,     0,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,     0,     0,     0,   206,
       0,   207,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   744,     0,   817,   745,   818,   746,     0,     0,
     747,     0,     0,     0,     0,   819,     0,     0,   748,   749,
     750,     0,   751,   752,     0,     0,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,     0,     0,     0,   753,     0,   820,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   744,     0,
       0,   745,     0,   746,     0,   928,   747,     0,     0,   929,
       0,   819,     0,     0,   748,   749,   750,     0,   751,   752,
       0,     0,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,     0,     0,
       0,   753,     0,   820,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   744,     0,     0,   745,     0,   746,
       0,     0,   747,     0,     0,   807,   811,     0,     0,     0,
     748,   749,   750,     0,   751,   752,     0,     0,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,     0,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,     0,     0,     0,   753,     0,   808,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     744,     0,     0,   745,     0,   746,     0,     0,   747,     0,
       0,   807,     0,     0,     0,     0,   748,   749,   750,     0,
     751,   752,     0,     0,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,     0,     0,   753,     0,   808,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   744,     0,     0,   745,
       0,   746,     0,     0,   747,     0,     0,     0,     0,   819,
       0,     0,   748,   749,   750,     0,   751,   752,     0,     0,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,     0,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,     0,     0,     0,   753,
       0,   820,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   744,     0,     0,   745,     0,   746,     0,     0,
     747,     0,     0,     0,     0,     0,     0,     0,   748,   749,
     750,     0,   751,   752,     0,     0,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,     0,     0,     0,   753,     0,  1004,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,     0,   166,   167,     0,     0,     0,
     169,   170,   171,   877,   173,   174,   878,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   192,   193,   194,   195,   196,   197,
     198,   199,   200,   201,   202,   203,   204,   205,     0,     0,
       0,   206,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   744,     0,   817,   745,   818,   746,     0,     0,
     747,     0,     0,     0,     0,   819,     0,     0,   748,   749,
     750,     0,   751,   752,     0,     0,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,     0,   192,   193,
     194,   195,   196,   197,   198,   199,   200,   201,   202,   203,
     204,   205,     0,     0,     0,   753,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   744,     0,     0,   745,
       0,   746,     0,     0,   747,     0,     0,     0,     0,   819,
       0,     0,   748,   749,   750,     0,   751,   752,     0,     0,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,     0,   192,   193,   194,   195,   196,   197,   198,   199,
     200,   201,   202,   203,   204,   205,     0,     0,     0,   753,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     744,     0,     0,   745,     0,   746,     0,     0,   747,     0,
       0,     0,   921,     0,     0,     0,   748,   749,   750,     0,
     751,   752,     0,     0,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,     0,   192,   193,   194,   195,
     196,   197,   198,   199,   200,   201,   202,   203,   204,   205,
       0,     0,     0,   753,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   744,     0,     0,   745,     0,   746,
       0,     0,   747,     0,     0,     0,     0,     0,     0,     0,
     748,   749,   750,     0,   751,   752,     0,     0,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,     0,
     192,   193,   194,   195,   196,   197,   198,   199,   200,   201,
     202,   203,   204,   205,   321,     0,   217,   753,   322,     0,
     323,   324,   325,     0,     0,     0,     0,     0,     0,   326,
     327,   321,     0,   217,     0,   322,     0,   429,   324,   325,
       0,     0,     0,     0,     0,     0,   326,   327,   381,     0,
      37,     0,   471,     0,   382,   383,   384,   224,   225,   226,
     328,   329,   229,   385,   386,   477,     0,   478,     0,   302,
       0,   479,   480,   481,   224,   225,   226,   328,   329,   229,
     482,   483,   500,     0,   217,     0,   501,     0,   502,   503,
     504,   387,   225,   226,   388,   389,   410,   505,   506,   529,
       0,   478,     0,   302,     0,   530,   531,   532,   484,   225,
     226,   485,   486,   303,   533,   534,   565,     0,   478,     0,
     302,     0,   566,   567,   568,   224,   225,   226,   507,   508,
     229,   569,   570,   583,     0,   478,     0,   302,     0,   584,
     585,   586,   535,   225,   226,   536,   537,   303,   587,   588,
     477,     0,   478,     0,   302,     0,   479,   480,   481,   571,
     225,   226,   572,   573,   303,   482,   483,   636,     0,   478,
       0,   302,     0,   637,   638,   639,   589,   590,   591,   592,
     593,   303,   640,   641,   716,     0,   478,     0,   302,     0,
     717,   718,   719,   484,   225,   226,   485,   632,   303,   720,
     721,   565,     0,   478,     0,   302,     0,   566,   780,   568,
     642,   225,   226,   643,   644,   303,   569,   570,   477,     0,
     478,     0,   302,     0,   479,   480,   481,   722,   225,   226,
     723,   724,   303,   482,   483,   565,     0,   478,     0,   302,
       0,   566,   902,   568,   571,   225,   226,   572,   573,   303,
     569,   570,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   484,   225,   226,   485,   786,   303,     0,     0,     0,
       0,     0,     0,     0,  1030,     0,     0,     0,   571,   225,
     226,   572,   573,   303,   965,   966,   967,   968,   969,   970,
     971,   972,   973,   974,   975,   976,   977,   978,   979,   980,
     981,   982,   983,   984,   985,   986,   987
};

static const yytype_int16 yycheck[] =
{
       0,    95,    51,   119,    52,    53,    54,   339,   340,    51,
      53,    54,     1,   107,   422,   340,   608,   349,   842,   612,
     613,   614,   608,   614,   349,   824,   867,   669,   670,   671,
     829,   754,   711,   608,    61,    62,   608,   612,   613,   614,
     608,    68,   551,   552,   767,   608,   579,   830,   279,    13,
     142,    68,   135,    99,   145,   763,  1013,   105,   106,   146,
      31,    32,   105,   106,   142,    76,   114,   146,   146,   118,
     146,   114,   155,   145,   873,    46,   118,  1034,    95,    37,
      38,   146,   142,   145,   145,     0,    50,   918,    52,    53,
      54,    55,   601,   602,   154,   688,   118,   188,   120,   608,
     192,   694,   695,   190,   695,   824,   152,   146,   816,    73,
     829,   190,   190,   688,   145,   708,   188,   145,   526,   694,
     695,    79,    80,   954,   843,   356,   188,   188,   117,   118,
     152,   120,   956,   841,   123,   145,   145,   545,   142,   688,
     155,   156,   157,   146,   736,   694,    18,   169,   145,   153,
     736,   145,   935,   146,   873,   172,     1,   188,   155,   150,
     188,   736,   155,   156,   736,   144,   146,   158,   736,   848,
      79,   579,   580,   736,   153,  1016,   899,   896,   188,   188,
     169,   714,    30,  1007,   777,   150,   280,   281,   182,   183,
     184,   285,    84,   158,   288,   289,   290,   291,   292,   293,
     294,   295,   296,   297,   298,   299,   799,    81,    82,   150,
    1009,   238,   239,   240,   241,   242,   243,   158,    16,   335,
      18,   150,   941,   339,   340,   341,   342,   736,   255,   158,
      42,   279,   259,   349,   876,   351,   166,   167,   830,   146,
     267,   268,   650,   651,   830,   175,   150,   274,   275,   276,
     277,   181,   300,   301,   158,   830,   146,   305,   306,   353,
     354,   355,   335,   856,   857,   856,   149,   301,   341,   342,
     172,   305,   306,   118,   153,   120,   155,     3,   351,    37,
      38,   856,   678,   166,   167,   927,   334,   155,   156,   157,
    1009,   334,   175,     3,  1013,   155,   156,   157,   181,   155,
     156,   157,   698,   711,    31,    32,   714,   152,   356,    52,
      53,   427,   428,    48,    49,  1034,   137,   910,   139,   910,
       4,  1040,     4,   146,   169,   441,   420,   146,     3,   423,
     357,   358,   155,   156,   157,   910,   155,   156,   157,   366,
    1059,   146,     7,   935,   155,   156,   157,   743,     3,   935,
     155,   156,   157,   145,   427,   428,   149,   150,   754,     3,
     935,   155,   156,   157,  1006,    30,   414,    86,    33,    99,
      22,   767,   114,   112,   422,    47,    41,   493,  1046,  1047,
    1048,  1049,   498,   791,   146,    50,   146,    52,  1056,   721,
     146,    56,   146,   441,   443,   146,   721,   182,   183,   184,
     796,   443,   146,   497,    99,    42,    43,    44,    45,    74,
      47,    76,    77,    78,    79,    21,    18,   146,    83,   157,
     493,    58,  1015,   517,   146,   498,   146,    63,   146,   146,
      95,   525,   146,   527,   146,  1103,   136,  1105,   146,   146,
     848,   146,   469,   146,    81,   146,  1039,   146,   496,   146,
     146,   157,   546,   146,   146,   146,   146,   135,   854,    96,
      97,    98,   156,   146,   144,   146,   155,   516,   155,   146,
     866,   157,   146,   869,   142,   146,   146,   143,   526,   142,
     142,   138,   143,   141,   138,   141,   137,   581,   157,   142,
     150,   518,   151,   140,   140,   140,   544,   146,   547,   140,
     148,   168,   551,   552,   146,   547,   145,   142,    11,    12,
     153,    14,    15,   153,   148,   142,   149,    20,   914,    22,
      23,    24,    25,    26,    27,    28,   150,   153,   153,   161,
     153,   579,   580,   142,   153,   144,   144,   631,   188,   148,
     149,   150,   145,   161,   181,   144,   144,   144,   157,   158,
     188,   947,   601,   602,   144,   604,   144,   188,   652,   608,
     144,   144,   604,   612,   613,   614,   615,   153,   964,     6,
     148,     8,   678,   615,   191,   188,   185,   186,   187,   188,
     189,   190,   630,     5,    87,   146,   145,   630,   144,   153,
     144,    28,   698,   188,   144,   144,     5,   145,   736,   734,
     733,   864,   650,   651,  1082,   721,   951,   421,   111,   547,
     113,  1009,   612,   613,   935,   856,   777,   613,   712,   931,
      57,   715,    59,    60,    61,    62,    63,    64,    65,    66,
      67,    68,    69,    70,    71,    72,   142,   743,   988,   688,
    1054,   654,   148,   149,   150,   694,   695,   814,   754,    12,
      15,   157,   158,   698,   157,   761,   124,   763,    95,   438,
     791,   767,   544,   711,   651,   713,   714,   414,   414,    -1,
     713,    34,    35,    36,    37,    -1,    39,    40,    -1,   185,
     186,   187,   188,   189,   733,   734,    -1,   736,   688,    -1,
     796,   733,   734,    -1,   694,     1,    -1,    -1,    -1,   793,
      -1,    -1,    -1,    66,    67,    68,    -1,    70,    -1,    -1,
     816,   135,   136,    -1,    -1,    -1,    -1,    -1,   824,    -1,
      -1,    -1,    -1,   829,    -1,    -1,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   841,    -1,   843,    -1,   833,
      -1,   835,    -1,   837,    -1,   839,    -1,    -1,   854,   798,
      -1,    -1,    -1,   847,    -1,   849,   798,    -1,    -1,    -1,
     866,   867,    -1,   869,   188,    -1,   190,   873,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   830,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     896,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   846,    -1,
     848,    -1,    -1,   846,    -1,    -1,    -1,   856,   914,   115,
     116,    -1,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,    -1,    -1,    -1,    -1,   941,    -1,    -1,    -1,    -1,
      -1,   947,    -1,   937,   938,   939,   940,    -1,    -1,    -1,
      -1,    -1,   946,    -1,    -1,    -1,    -1,    -1,   964,    -1,
      -1,   910,    -1,   169,    -1,    -1,    -1,    -1,   917,    -1,
      -1,    -1,    -1,   179,   142,   917,   144,    -1,   146,    -1,
     148,   149,   150,    -1,    -1,    -1,   935,   936,    -1,   157,
     158,    -1,    -1,    -1,   936,    -1,    -1,   945,    -1,    -1,
      -1,    -1,   945,  1009,    -1,    -1,    -1,  1013,    -1,    -1,
    1016,    -1,    -1,    -1,  1008,    -1,  1010,   185,   186,   187,
     188,   189,   190,    -1,    -1,    -1,    -1,    -1,  1034,    -1,
      -1,    -1,     0,     1,  1040,    -1,    -1,    -1,     6,     7,
       8,     9,    -1,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,  1059,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    -1,    -1,    33,    -1,    -1,    -1,    37,
      -1,    -1,    -1,    41,   135,   136,    -1,    -1,    -1,    -1,
      -1,   142,    -1,    -1,    -1,    -1,    -1,    -1,    56,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,    -1,
      -1,    -1,  1051,  1052,  1053,    -1,    -1,    75,  1102,  1051,
    1052,  1053,    -1,    -1,    -1,    -1,    -1,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,   188,    -1,   190,
      -1,   192,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,    -1,
    1070,  1071,  1072,    -1,    -1,    -1,   144,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,   138,    -1,   140,    -1,   185,   143,    -1,
      -1,    -1,    -1,   148,    -1,    -1,   151,   152,   153,    -1,
     155,   156,    -1,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      -1,    -1,    -1,   188,    -1,    -1,    -1,   192,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,    -1,   142,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,    -1,    -1,
      -1,   188,    -1,   190,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
      -1,   142,   143,   144,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,    -1,    -1,    -1,   188,    -1,   190,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,    -1,   142,   143,   144,
      -1,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      -1,    -1,    -1,   188,    -1,   190,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,    -1,   142,   143,   144,    -1,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,    -1,    -1,    -1,   188,
      -1,   190,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,    -1,   142,
     143,    -1,    -1,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,    -1,    -1,    -1,   188,    -1,   190,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,    -1,    -1,   143,   144,    -1,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,    -1,    -1,
      -1,   188,    -1,   190,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
      -1,   142,   143,    -1,    -1,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,    -1,    -1,    -1,   188,    -1,   190,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,    -1,   142,   143,    -1,
      -1,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      -1,    -1,    -1,   188,    -1,   190,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,    -1,    -1,   143,    -1,    -1,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,    -1,    -1,    -1,   188,
      -1,   190,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,   137,   138,   139,   140,    -1,    -1,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,   155,   156,    -1,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,    -1,    -1,    -1,   188,    -1,   190,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,    -1,
      -1,   138,    -1,   140,    -1,   142,   143,    -1,    -1,   146,
      -1,   148,    -1,    -1,   151,   152,   153,    -1,   155,   156,
      -1,    -1,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,    -1,    -1,
      -1,   188,    -1,   190,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,   138,    -1,   140,
      -1,    -1,   143,    -1,    -1,   146,   147,    -1,    -1,    -1,
     151,   152,   153,    -1,   155,   156,    -1,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,    -1,    -1,    -1,   188,    -1,   190,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,   138,    -1,   140,    -1,    -1,   143,    -1,
      -1,   146,    -1,    -1,    -1,    -1,   151,   152,   153,    -1,
     155,   156,    -1,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      -1,    -1,    -1,   188,    -1,   190,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,   138,
      -1,   140,    -1,    -1,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,   155,   156,    -1,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,    -1,    -1,    -1,   188,
      -1,   190,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,    -1,   138,    -1,   140,    -1,    -1,
     143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   151,   152,
     153,    -1,   155,   156,    -1,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,    -1,    -1,    -1,   188,    -1,   190,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,    -1,   142,   143,    -1,    -1,    -1,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,    -1,    -1,
      -1,   188,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,    -1,   137,   138,   139,   140,    -1,    -1,
     143,    -1,    -1,    -1,    -1,   148,    -1,    -1,   151,   152,
     153,    -1,   155,   156,    -1,    -1,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,    -1,    -1,    -1,   188,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,    -1,    -1,   138,
      -1,   140,    -1,    -1,   143,    -1,    -1,    -1,    -1,   148,
      -1,    -1,   151,   152,   153,    -1,   155,   156,    -1,    -1,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,    -1,    -1,    -1,   188,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,    -1,    -1,   138,    -1,   140,    -1,    -1,   143,    -1,
      -1,    -1,   147,    -1,    -1,    -1,   151,   152,   153,    -1,
     155,   156,    -1,    -1,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
      -1,    -1,    -1,   188,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,    -1,    -1,   138,    -1,   140,
      -1,    -1,   143,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     151,   152,   153,    -1,   155,   156,    -1,    -1,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   142,    -1,   144,   188,   146,    -1,
     148,   149,   150,    -1,    -1,    -1,    -1,    -1,    -1,   157,
     158,   142,    -1,   144,    -1,   146,    -1,   148,   149,   150,
      -1,    -1,    -1,    -1,    -1,    -1,   157,   158,   142,    -1,
     144,    -1,   146,    -1,   148,   149,   150,   185,   186,   187,
     188,   189,   190,   157,   158,   142,    -1,   144,    -1,   146,
      -1,   148,   149,   150,   185,   186,   187,   188,   189,   190,
     157,   158,   142,    -1,   144,    -1,   146,    -1,   148,   149,
     150,   185,   186,   187,   188,   189,   190,   157,   158,   142,
      -1,   144,    -1,   146,    -1,   148,   149,   150,   185,   186,
     187,   188,   189,   190,   157,   158,   142,    -1,   144,    -1,
     146,    -1,   148,   149,   150,   185,   186,   187,   188,   189,
     190,   157,   158,   142,    -1,   144,    -1,   146,    -1,   148,
     149,   150,   185,   186,   187,   188,   189,   190,   157,   158,
     142,    -1,   144,    -1,   146,    -1,   148,   149,   150,   185,
     186,   187,   188,   189,   190,   157,   158,   142,    -1,   144,
      -1,   146,    -1,   148,   149,   150,   185,   186,   187,   188,
     189,   190,   157,   158,   142,    -1,   144,    -1,   146,    -1,
     148,   149,   150,   185,   186,   187,   188,   189,   190,   157,
     158,   142,    -1,   144,    -1,   146,    -1,   148,   149,   150,
     185,   186,   187,   188,   189,   190,   157,   158,   142,    -1,
     144,    -1,   146,    -1,   148,   149,   150,   185,   186,   187,
     188,   189,   190,   157,   158,   142,    -1,   144,    -1,   146,
      -1,   148,   149,   150,   185,   186,   187,   188,   189,   190,
     157,   158,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   185,   186,   187,   188,   189,   190,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   149,    -1,    -1,    -1,   185,   186,
     187,   188,   189,   190,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    16,    17,    19,
      29,    30,    33,    37,    41,    56,    75,    85,    86,    88,
      89,    90,    91,    92,    93,    94,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   144,   185,   194,
     195,   196,   232,   250,   252,   262,   354,   409,   410,   411,
     401,   233,   357,   376,   366,     7,    30,    33,    41,    50,
      52,    56,    74,    76,    77,    78,    79,    83,    95,   416,
       6,     8,    28,    57,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    95,    37,
      38,    79,    80,   405,   406,   355,    81,    82,   407,   405,
      76,   146,   146,   146,   146,   358,   359,   356,   200,   201,
     202,   203,   198,   199,   375,   251,   197,     0,   253,   369,
      11,    12,    14,    15,    20,    22,    23,    24,    25,    26,
      27,    28,    87,   111,   113,   157,   408,   413,    18,   146,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   142,   143,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   188,   190,   342,   347,
     348,   349,   350,   351,   352,   353,   142,   144,   146,   148,
     149,   150,   157,   158,   185,   186,   187,   188,   189,   190,
     417,   466,   469,   484,   417,   417,    79,    13,    50,    52,
      53,    54,    55,    73,    12,    34,    35,    36,    37,    39,
      40,    66,    67,    68,    70,   403,    42,    43,    44,    45,
      47,    58,    81,    96,    97,    98,   181,   402,    30,    31,
      32,   404,   404,   404,    68,    95,   172,    84,   404,   397,
     382,   385,    42,    99,   152,   383,   146,   146,   384,   386,
     387,   388,   389,   390,   391,   392,   393,   394,   395,   396,
     377,   378,   146,   190,   249,   380,   379,   172,   417,   417,
     249,     3,     3,     4,     4,     3,     3,   417,   145,     3,
     342,   142,   146,   148,   149,   150,   157,   158,   188,   189,
     421,   466,   470,   484,   360,   361,   411,    16,    18,   362,
     363,   364,   365,    99,   412,    22,   114,   414,   415,   367,
     112,   370,   234,   467,   468,   418,   399,    52,    53,   404,
     404,   404,   404,   404,   404,   404,    46,   404,    48,    49,
      47,   404,   404,   146,   146,   146,   404,   404,   404,   404,
     146,   142,   148,   149,   150,   157,   158,   185,   188,   189,
     483,   484,   249,   249,   146,   146,    99,   249,   249,   249,
     249,   249,   249,   249,   249,   249,   249,   249,   249,   249,
     190,   250,   478,   480,   481,   482,   483,   478,   478,   478,
     381,   254,   422,   423,   417,   421,    21,   373,   371,   148,
     441,   466,   470,   441,   421,   421,   146,    18,   150,   158,
     441,   368,   421,   157,   256,   249,   142,   249,   419,   249,
     483,   404,   404,   146,   146,   146,   146,   146,   146,   146,
     404,   146,    63,   146,   146,   146,   146,   146,   146,   398,
     146,   146,   479,   480,   482,   249,   256,   142,   144,   148,
     149,   150,   157,   158,   185,   188,   189,   249,   424,   464,
     472,   484,   249,   374,   421,   421,   442,   443,   372,   415,
     142,   146,   148,   149,   150,   157,   158,   188,   189,   430,
     466,   471,   484,   257,   258,   342,   136,   420,   400,   146,
     146,   146,   404,   135,   332,   465,   425,   426,   421,   142,
     148,   149,   150,   157,   158,   185,   188,   189,   444,   464,
     474,   484,   249,   421,   432,   431,   433,   150,   158,   142,
     154,   259,   144,   205,   206,   207,   208,   209,   210,   342,
     249,   404,   146,   263,   249,   142,   148,   149,   150,   157,
     158,   185,   188,   189,   427,   464,   473,   484,   249,   446,
     445,   447,   444,   142,   148,   149,   150,   157,   158,   185,
     186,   187,   188,   189,   434,   464,   477,   249,   258,   205,
     205,   153,   155,   156,   157,   146,     1,   115,   116,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   169,   179,   264,   301,
     428,   429,   189,   458,   464,   472,   142,   148,   149,   150,
     157,   158,   185,   188,   189,   448,   464,   475,   484,   249,
     435,   436,   437,   145,   205,   205,   144,   213,   211,   212,
     342,   146,   146,   150,   153,   190,   204,   205,   208,   209,
     210,   343,   346,   347,   349,   351,   352,   265,   266,   267,
     137,   138,   139,   140,   143,   146,   147,   250,   298,   299,
     344,   348,   349,   352,   298,   297,   344,   342,   293,   269,
     271,   274,   276,   279,   281,   284,   286,   290,   289,   417,
     249,   459,   460,   449,   450,   451,   142,   148,   149,   150,
     157,   158,   185,   188,   189,   438,   464,   476,   484,   438,
     249,   332,   214,   157,   150,   158,   155,   146,   333,   333,
     333,   333,   146,   334,   135,   138,   140,   143,   151,   152,
     153,   155,   156,   188,   335,   337,   338,   345,   349,   353,
     142,   142,   299,   142,   142,   344,   292,   335,   142,   142,
     143,   143,   138,   138,   141,   141,   142,   302,   303,   348,
     149,   461,   464,   473,   249,   417,   189,   452,   464,   472,
     249,   439,   441,   440,   235,     1,   118,   120,   152,   169,
     216,   217,   218,   212,   211,   204,   205,   146,   190,   255,
     337,   147,   255,   337,   336,   157,   268,   137,   139,   148,
     190,   260,   300,   309,   311,   313,   337,   300,   137,   236,
     151,   294,   255,   270,   272,   275,   277,   280,   282,   285,
     287,   291,   142,   192,   303,   304,   462,   463,   453,   454,
     434,   249,     1,   117,   118,   120,   123,   169,   237,   337,
     219,   342,   348,   215,   150,   158,   334,   339,   300,   314,
     260,   310,   313,   312,   260,   261,   313,   150,   153,   295,
     296,   343,   347,   349,   351,   352,   146,   249,   140,   249,
     140,   249,   140,   249,   140,   300,   312,   313,   148,   305,
     417,   249,   149,   455,   464,   473,   249,   146,   337,   238,
     297,   244,   246,   348,   153,   142,   222,   153,   221,   145,
     218,   147,   338,   340,   315,   337,   148,   316,   142,   146,
     260,   307,   308,   313,   333,   150,   142,   273,   278,   283,
     288,   192,   168,   306,   255,   456,   457,   153,   142,   153,
     240,   142,   153,   144,   247,   337,   223,   153,   342,   224,
     304,   150,   158,   149,   150,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   317,   318,
     319,   333,   309,   296,   342,   249,   249,   249,   249,   313,
     188,   149,   417,   249,   190,   239,   337,   241,   243,   236,
     248,   245,   304,   312,   220,   153,   341,   337,   161,   161,
     188,   144,   188,   144,   144,   144,   144,   144,   144,   144,
     149,   319,   320,   333,   312,   249,   261,   249,   145,   153,
     311,   148,   225,   348,   338,   321,   323,   324,   325,   326,
     327,   182,   183,   184,   330,   331,   322,   191,   328,   311,
     348,   313,   226,   188,   329,   329,   329,   329,   329,     5,
     342,   342,   342,   145,   331,   329,   313,   146,   166,   167,
     175,   181,   227,   228,   145,   188,   145,   145,   145,   145,
     145,   250,   250,   250,   145,   153,   188,   144,   144,   144,
     149,   228,   242,   229,   231,   230,   249,   329,     5,   329,
     145,   145,   145
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   193,   194,   194,   195,   195,   195,   195,   197,   196,
     198,   196,   199,   196,   196,   200,   196,   201,   196,   196,
     202,   196,   203,   196,   196,   196,   204,   204,   204,   204,
     204,   204,   204,   205,   205,   206,   206,   207,   207,   207,
     208,   209,   210,   211,   211,   212,   212,   214,   215,   213,
     216,   216,   217,   217,   218,   218,   219,   220,   218,   221,
     218,   223,   222,   222,   224,   224,   226,   225,   225,   227,
     227,   228,   229,   228,   230,   228,   231,   228,   233,   234,
     232,   235,   235,   236,   236,   237,   237,   238,   237,   237,
     237,   239,   239,   241,   242,   240,   243,   240,   244,   245,
     244,   247,   246,   248,   246,   249,   249,   251,   250,   253,
     254,   252,   255,   255,   256,   256,   257,   257,   258,   259,
     259,   260,   261,   261,   262,   262,   263,   263,   264,   265,
     264,   266,   264,   267,   268,   264,   264,   264,   269,   270,
     264,   271,   272,   273,   264,   274,   275,   264,   276,   277,
     278,   264,   279,   280,   264,   281,   282,   283,   264,   284,
     285,   264,   286,   287,   288,   264,   289,   264,   290,   291,
     264,   264,   292,   264,   293,   264,   264,   294,   294,   295,
     295,   296,   297,   297,   298,   298,   299,   299,   300,   300,
     300,   301,   301,   302,   302,   303,   304,   304,   305,   305,
     306,   307,   307,   308,   308,   308,   309,   310,   310,   311,
     311,   312,   312,   313,   314,   313,   315,   315,   316,   316,
     317,   317,   318,   318,   318,   319,   319,   320,   319,   319,
     319,   319,   321,   319,   322,   319,   323,   319,   324,   319,
     325,   319,   319,   319,   319,   326,   319,   319,   319,   319,
     319,   327,   319,   319,   319,   328,   328,   329,   329,   330,
     330,   331,   331,   331,   331,   332,   332,   333,   333,   334,
     334,   335,   335,   336,   335,   337,   338,   339,   338,   341,
     340,   340,   342,   342,   342,   342,   342,   343,   343,   343,
     343,   343,   343,   344,   344,   344,   344,   344,   344,   344,
     344,   344,   344,   345,   345,   345,   345,   345,   345,   345,
     345,   345,   345,   345,   345,   346,   346,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   348,   348,   348,
     348,   349,   349,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   351,   351,   351,   351,   351,   351,   351,
     352,   352,   352,   353,   353,   353,   353,   353,   353,   353,
     353,   353,   353,   353,   353,   353,   353,   353,   353,   353,
     353,   353,   353,   353,   353,   353,   353,   355,   354,   356,
     354,   357,   354,   358,   354,   359,   354,   360,   354,   361,
     354,   362,   354,   363,   354,   364,   354,   365,   354,   366,
     354,   367,   354,   368,   354,   369,   354,   370,   354,   371,
     354,   372,   354,   373,   354,   374,   354,   354,   375,   354,
     376,   354,   354,   377,   354,   378,   354,   379,   354,   380,
     354,   381,   354,   382,   354,   383,   354,   384,   354,   385,
     354,   354,   354,   386,   354,   387,   354,   388,   354,   389,
     354,   390,   354,   391,   354,   392,   354,   393,   354,   394,
     354,   395,   354,   354,   354,   354,   396,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   354,
     354,   397,   398,   354,   399,   400,   354,   354,   354,   354,
     354,   354,   354,   354,   354,   354,   354,   354,   354,   401,
     354,   402,   402,   402,   402,   402,   402,   402,   402,   402,
     402,   402,   403,   403,   403,   403,   403,   403,   403,   403,
     403,   403,   403,   403,   404,   404,   405,   405,   406,   406,
     407,   407,   408,   408,   408,   408,   408,   409,   409,   410,
     410,   411,   411,   412,   412,   413,   413,   414,   414,   415,
     416,   416,   416,   417,   418,   417,   420,   419,   419,   422,
     421,   421,   423,   421,   425,   424,   426,   424,   424,   428,
     427,   429,   427,   427,   431,   430,   432,   430,   430,   433,
     430,   435,   434,   436,   434,   437,   434,   434,   438,   439,
     438,   440,   438,   438,   442,   441,   441,   443,   441,   445,
     444,   446,   444,   447,   444,   444,   449,   448,   450,   448,
     451,   448,   448,   453,   452,   454,   452,   452,   456,   455,
     457,   455,   455,   459,   458,   460,   458,   458,   462,   461,
     463,   461,   461,   465,   464,   464,   467,   466,   468,   466,
     466,   469,   469,   469,   469,   469,   469,   469,   469,   469,
     469,   470,   470,   470,   470,   470,   470,   470,   470,   470,
     471,   471,   471,   471,   471,   471,   471,   471,   472,   472,
     472,   472,   472,   472,   472,   472,   472,   473,   473,   473,
     473,   473,   473,   473,   473,   473,   474,   474,   474,   474,
     474,   474,   474,   474,   475,   475,   475,   475,   475,   475,
     475,   475,   476,   476,   476,   476,   476,   476,   476,   476,
     477,   477,   477,   477,   477,   477,   477,   477,   477,   478,
     478,   479,   479,   480,   481,   481,   482,   482,   483,   483,
     483,   483,   483,   483,   483,   483,   483,   483,   484,   484
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
       3,     0,     4,     0,     5,     0,     3,     0,     4,     0,
       5,     0,     6,     0,     5,     0,     6,     4,     0,     3,
       0,     3,     1,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     5,     0,     4,     0,     4,     0,     4,     0,
       4,     3,     3,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     4,     4,     5,     4,     0,     4,     5,     5,
       6,     5,     5,     6,     5,     5,     5,     5,     6,     5,
       4,     0,     0,     7,     0,     0,     8,     4,     5,     5,
       5,     4,     5,     2,     2,     2,     2,     5,     4,     0,
       3,     1,     1,     2,     2,     1,     1,     1,     1,     1,
       1,     3,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       0,     1,     0,     1,     0,     3,     0,     1,     3,     1,
       1,     1,     1,     1,     0,     3,     0,     3,     1,     0,
       3,     1,     0,     3,     0,     3,     0,     3,     1,     0,
       3,     0,     3,     1,     0,     3,     0,     3,     1,     0,
       3,     0,     3,     0,     3,     0,     3,     1,     2,     0,
       3,     0,     3,     1,     0,     3,     1,     0,     3,     0,
       3,     0,     3,     0,     3,     1,     0,     3,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       0,     3,     1,     0,     3,     1,     0,     3,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, UserLevelRewritingContext::ParseResult* parseResult)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (parseResult);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo, int yytype, YYSTYPE const * const yyvaluep, UserLevelRewritingContext::ParseResult* parseResult)
{
  YYFPRINTF (yyo, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyo, yytype, yyvaluep, parseResult);
  YYFPRINTF (yyo, ")");
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
  unsigned long yylno = yyrline[yyrule];
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
                       &yyvsp[(yyi + 1) - (yynrhs)]
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
            else
              goto append;

          append:
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

  return (YYSIZE_T) (yystpcpy (yyres, yystr) - yyres);
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
                  if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
                    yysize = yysize1;
                  else
                    return 2;
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
    default: /* Avoid compiler warnings. */
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
    if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
      yysize = yysize1;
    else
      return 2;
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
    case 204: /* moduleExprDot  */
#line 120 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2534 "surface.c" /* yacc.c:1257  */
        break;

    case 205: /* moduleExpr  */
#line 120 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2540 "surface.c" /* yacc.c:1257  */
        break;

    case 206: /* moduleExpr2  */
#line 120 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2546 "surface.c" /* yacc.c:1257  */
        break;

    case 207: /* moduleExpr3  */
#line 120 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2552 "surface.c" /* yacc.c:1257  */
        break;

    case 208: /* renameExpr  */
#line 120 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2558 "surface.c" /* yacc.c:1257  */
        break;

    case 209: /* instantExpr  */
#line 120 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2564 "surface.c" /* yacc.c:1257  */
        break;

    case 210: /* parenExpr  */
#line 120 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2570 "surface.c" /* yacc.c:1257  */
        break;

    case 211: /* viewExpr  */
#line 120 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyViewExpression)->deepSelfDestruct(); }
#line 2576 "surface.c" /* yacc.c:1257  */
        break;

    case 212: /* instantArgs  */
#line 121 "surface.yy" /* yacc.c:1257  */
      { deepSelfDestructViewExpressionVector(((*yyvaluep).yyViewExpressionVector)); }
#line 2582 "surface.c" /* yacc.c:1257  */
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
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yynewstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  *yyssp = (yytype_int16) yystate;

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    goto yyexhaustedlab;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = (YYSIZE_T) (yyssp - yyss + 1);

# if defined yyoverflow
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
# else /* defined YYSTACK_RELOCATE */
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
# undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */

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
| yyreduce -- do a reduction.  |
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
#line 257 "surface.yy" /* yacc.c:1652  */
    { YYACCEPT; }
#line 2853 "surface.c" /* yacc.c:1652  */
    break;

  case 3:
#line 259 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			}
#line 2861 "surface.c" /* yacc.c:1652  */
    break;

  case 8:
#line 273 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2867 "surface.c" /* yacc.c:1652  */
    break;

  case 9:
#line 275 "surface.yy" /* yacc.c:1652  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			}
#line 2880 "surface.c" /* yacc.c:1652  */
    break;

  case 10:
#line 283 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2886 "surface.c" /* yacc.c:1652  */
    break;

  case 11:
#line 285 "surface.yy" /* yacc.c:1652  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2899 "surface.c" /* yacc.c:1652  */
    break;

  case 12:
#line 293 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2905 "surface.c" /* yacc.c:1652  */
    break;

  case 13:
#line 295 "surface.yy" /* yacc.c:1652  */
    {
			  int lineNr = lineNumber;
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr) &&
			      !directoryManager.alreadySeen(directory, fileName))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2918 "surface.c" /* yacc.c:1652  */
    break;

  case 14:
#line 304 "surface.yy" /* yacc.c:1652  */
    {
			  cout << directoryManager.getCwd() << '\n';
			}
#line 2926 "surface.c" /* yacc.c:1652  */
    break;

  case 15:
#line 307 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2932 "surface.c" /* yacc.c:1652  */
    break;

  case 16:
#line 309 "surface.yy" /* yacc.c:1652  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			}
#line 2946 "surface.c" /* yacc.c:1652  */
    break;

  case 17:
#line 318 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2952 "surface.c" /* yacc.c:1652  */
    break;

  case 18:
#line 320 "surface.yy" /* yacc.c:1652  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			}
#line 2966 "surface.c" /* yacc.c:1652  */
    break;

  case 19:
#line 330 "surface.yy" /* yacc.c:1652  */
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
#line 2981 "surface.c" /* yacc.c:1652  */
    break;

  case 20:
#line 340 "surface.yy" /* yacc.c:1652  */
    { lexerStringMode(); }
#line 2987 "surface.c" /* yacc.c:1652  */
    break;

  case 21:
#line 342 "surface.yy" /* yacc.c:1652  */
    {
			  returnValueDump = system((string("ls") + (yyvsp[0].yyString)).c_str());
			}
#line 2995 "surface.c" /* yacc.c:1652  */
    break;

  case 22:
#line 345 "surface.yy" /* yacc.c:1652  */
    { lexerStringMode(); }
#line 3001 "surface.c" /* yacc.c:1652  */
    break;

  case 23:
#line 347 "surface.yy" /* yacc.c:1652  */
    {
			  returnValueDump = system((string("ls -l") + (yyvsp[0].yyString)).c_str());
			}
#line 3009 "surface.c" /* yacc.c:1652  */
    break;

  case 24:
#line 351 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			}
#line 3018 "surface.c" /* yacc.c:1652  */
    break;

  case 25:
#line 356 "surface.yy" /* yacc.c:1652  */
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			}
#line 3029 "surface.c" /* yacc.c:1652  */
    break;

  case 26:
#line 389 "surface.yy" /* yacc.c:1652  */
    {
                          (yyval.yyModuleExpression) =  new ModuleExpression((yyvsp[-1].yyToken));
                        }
#line 3037 "surface.c" /* yacc.c:1652  */
    break;

  case 27:
#line 393 "surface.yy" /* yacc.c:1652  */
    {
                          (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-1].yyToken));
                        }
#line 3045 "surface.c" /* yacc.c:1652  */
    break;

  case 31:
#line 400 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), (yyvsp[0].yyModuleExpression));
			}
#line 3053 "surface.c" /* yacc.c:1652  */
    break;

  case 32:
#line 404 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyModuleExpression) = new ModuleExpression(t);
			}
#line 3064 "surface.c" /* yacc.c:1652  */
    break;

  case 34:
#line 414 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), (yyvsp[0].yyModuleExpression));
			}
#line 3072 "surface.c" /* yacc.c:1652  */
    break;

  case 39:
#line 426 "surface.yy" /* yacc.c:1652  */
    {
                          (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[0].yyToken));
                        }
#line 3080 "surface.c" /* yacc.c:1652  */
    break;

  case 40:
#line 433 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), currentRenaming);
			  currentRenaming = 0;
			}
#line 3089 "surface.c" /* yacc.c:1652  */
    break;

  case 41:
#line 440 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-3].yyModuleExpression), *((yyvsp[-1].yyViewExpressionVector)));
			  delete (yyvsp[-1].yyViewExpressionVector);
			}
#line 3098 "surface.c" /* yacc.c:1652  */
    break;

  case 42:
#line 447 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = (yyvsp[-1].yyModuleExpression);
			}
#line 3106 "surface.c" /* yacc.c:1652  */
    break;

  case 43:
#line 456 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyViewExpression) = new ViewExpression((yyvsp[-3].yyViewExpression), *((yyvsp[-1].yyViewExpressionVector)));
			  delete (yyvsp[-1].yyViewExpressionVector);
			}
#line 3115 "surface.c" /* yacc.c:1652  */
    break;

  case 44:
#line 461 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyViewExpression) = new ViewExpression((yyvsp[0].yyToken));
			}
#line 3123 "surface.c" /* yacc.c:1652  */
    break;

  case 45:
#line 467 "surface.yy" /* yacc.c:1652  */
    {
			  (yyvsp[-2].yyViewExpressionVector)->append((yyvsp[0].yyViewExpression));
			  (yyval.yyViewExpressionVector) = (yyvsp[-2].yyViewExpressionVector);
			}
#line 3132 "surface.c" /* yacc.c:1652  */
    break;

  case 46:
#line 472 "surface.yy" /* yacc.c:1652  */
    {
			  Vector<ViewExpression*>* t =  new Vector<ViewExpression*>();
			  t->append((yyvsp[0].yyViewExpression));
			  (yyval.yyViewExpressionVector) = t;
			}
#line 3142 "surface.c" /* yacc.c:1652  */
    break;

  case 47:
#line 483 "surface.yy" /* yacc.c:1652  */
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			}
#line 3151 "surface.c" /* yacc.c:1652  */
    break;

  case 48:
#line 488 "surface.yy" /* yacc.c:1652  */
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			}
#line 3159 "surface.c" /* yacc.c:1652  */
    break;

  case 54:
#line 503 "surface.yy" /* yacc.c:1652  */
    {
			  currentRenaming->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3167 "surface.c" /* yacc.c:1652  */
    break;

  case 55:
#line 507 "surface.yy" /* yacc.c:1652  */
    {
			  currentRenaming->addLabelMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3175 "surface.c" /* yacc.c:1652  */
    break;

  case 56:
#line 510 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 3181 "surface.c" /* yacc.c:1652  */
    break;

  case 57:
#line 511 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
#line 3187 "surface.c" /* yacc.c:1652  */
    break;

  case 58:
#line 512 "surface.yy" /* yacc.c:1652  */
    {}
#line 3193 "surface.c" /* yacc.c:1652  */
    break;

  case 59:
#line 513 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->addStratMapping((yyvsp[0].yyToken)); }
#line 3199 "surface.c" /* yacc.c:1652  */
    break;

  case 60:
#line 515 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->addStratTarget((yyvsp[0].yyToken)); }
#line 3205 "surface.c" /* yacc.c:1652  */
    break;

  case 61:
#line 520 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 3211 "surface.c" /* yacc.c:1652  */
    break;

  case 62:
#line 521 "surface.yy" /* yacc.c:1652  */
    {}
#line 3217 "surface.c" /* yacc.c:1652  */
    break;

  case 63:
#line 522 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 3223 "surface.c" /* yacc.c:1652  */
    break;

  case 66:
#line 532 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 3229 "surface.c" /* yacc.c:1652  */
    break;

  case 67:
#line 533 "surface.yy" /* yacc.c:1652  */
    {}
#line 3235 "surface.c" /* yacc.c:1652  */
    break;

  case 68:
#line 534 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 3241 "surface.c" /* yacc.c:1652  */
    break;

  case 71:
#line 541 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->setPrec((yyvsp[0].yyToken)); }
#line 3247 "surface.c" /* yacc.c:1652  */
    break;

  case 72:
#line 542 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3253 "surface.c" /* yacc.c:1652  */
    break;

  case 73:
#line 543 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->setGather(tokenSequence); }
#line 3259 "surface.c" /* yacc.c:1652  */
    break;

  case 74:
#line 544 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3265 "surface.c" /* yacc.c:1652  */
    break;

  case 75:
#line 545 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->setFormat(tokenSequence); }
#line 3271 "surface.c" /* yacc.c:1652  */
    break;

  case 76:
#line 546 "surface.yy" /* yacc.c:1652  */
    { lexerLatexMode(); }
#line 3277 "surface.c" /* yacc.c:1652  */
    break;

  case 77:
#line 547 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->setLatexMacro((yyvsp[-1].yyString)); }
#line 3283 "surface.c" /* yacc.c:1652  */
    break;

  case 78:
#line 553 "surface.yy" /* yacc.c:1652  */
    { lexerIdMode(); }
#line 3289 "surface.c" /* yacc.c:1652  */
    break;

  case 79:
#line 555 "surface.yy" /* yacc.c:1652  */
    {
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			  interpreter.setCurrentView(new SyntacticView((yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CV;
			}
#line 3299 "surface.c" /* yacc.c:1652  */
    break;

  case 80:
#line 563 "surface.yy" /* yacc.c:1652  */
    {
			  CV->addFrom((yyvsp[-5].yyModuleExpression));
			  CV->addTo((yyvsp[-3].yyModuleExpression));
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[-9].yyToken)).code(), CV);
			  CV->finishView();
			}
#line 3312 "surface.c" /* yacc.c:1652  */
    break;

  case 82:
#line 574 "surface.yy" /* yacc.c:1652  */
    {}
#line 3318 "surface.c" /* yacc.c:1652  */
    break;

  case 83:
#line 578 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping " << QUOTE("->") << " in variable declaration.");
			}
#line 3327 "surface.c" /* yacc.c:1652  */
    break;

  case 84:
#line 582 "surface.yy" /* yacc.c:1652  */
    {}
#line 3333 "surface.c" /* yacc.c:1652  */
    break;

  case 85:
#line 586 "surface.yy" /* yacc.c:1652  */
    {
			  CV->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3341 "surface.c" /* yacc.c:1652  */
    break;

  case 86:
#line 589 "surface.yy" /* yacc.c:1652  */
    {}
#line 3347 "surface.c" /* yacc.c:1652  */
    break;

  case 87:
#line 590 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 3353 "surface.c" /* yacc.c:1652  */
    break;

  case 91:
#line 596 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyToken) = (yyvsp[-1].yyToken); }
#line 3359 "surface.c" /* yacc.c:1652  */
    break;

  case 92:
#line 598 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			}
#line 3370 "surface.c" /* yacc.c:1652  */
    break;

  case 93:
#line 607 "surface.yy" /* yacc.c:1652  */
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			}
#line 3382 "surface.c" /* yacc.c:1652  */
    break;

  case 94:
#line 615 "surface.yy" /* yacc.c:1652  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3390 "surface.c" /* yacc.c:1652  */
    break;

  case 95:
#line 619 "surface.yy" /* yacc.c:1652  */
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			}
#line 3399 "surface.c" /* yacc.c:1652  */
    break;

  case 96:
#line 624 "surface.yy" /* yacc.c:1652  */
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1);
			}
#line 3413 "surface.c" /* yacc.c:1652  */
    break;

  case 97:
#line 634 "surface.yy" /* yacc.c:1652  */
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
#line 3437 "surface.c" /* yacc.c:1652  */
    break;

  case 98:
#line 656 "surface.yy" /* yacc.c:1652  */
    {
			  strategyCall.resize(1);
			  strategyCall[0] = (yyvsp[0].yyToken);
			}
#line 3446 "surface.c" /* yacc.c:1652  */
    break;

  case 99:
#line 660 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3452 "surface.c" /* yacc.c:1652  */
    break;

  case 100:
#line 662 "surface.yy" /* yacc.c:1652  */
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
#line 3467 "surface.c" /* yacc.c:1652  */
    break;

  case 101:
#line 674 "surface.yy" /* yacc.c:1652  */
    {
			  CV->addStratMapping((yyvsp[0].yyToken));
			}
#line 3475 "surface.c" /* yacc.c:1652  */
    break;

  case 102:
#line 678 "surface.yy" /* yacc.c:1652  */
    {
			  CV->addStratTarget((yyvsp[-1].yyToken));
			}
#line 3483 "surface.c" /* yacc.c:1652  */
    break;

  case 103:
#line 682 "surface.yy" /* yacc.c:1652  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3491 "surface.c" /* yacc.c:1652  */
    break;

  case 104:
#line 686 "surface.yy" /* yacc.c:1652  */
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
#line 3517 "surface.c" /* yacc.c:1652  */
    break;

  case 105:
#line 709 "surface.yy" /* yacc.c:1652  */
    {}
#line 3523 "surface.c" /* yacc.c:1652  */
    break;

  case 106:
#line 711 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			}
#line 3534 "surface.c" /* yacc.c:1652  */
    break;

  case 107:
#line 719 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3540 "surface.c" /* yacc.c:1652  */
    break;

  case 108:
#line 720 "surface.yy" /* yacc.c:1652  */
    {}
#line 3546 "surface.c" /* yacc.c:1652  */
    break;

  case 109:
#line 726 "surface.yy" /* yacc.c:1652  */
    { lexerIdMode(); }
#line 3552 "surface.c" /* yacc.c:1652  */
    break;

  case 110:
#line 728 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3562 "surface.c" /* yacc.c:1652  */
    break;

  case 111:
#line 734 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[0].yyToken));
			}
#line 3572 "surface.c" /* yacc.c:1652  */
    break;

  case 112:
#line 741 "surface.yy" /* yacc.c:1652  */
    {}
#line 3578 "surface.c" /* yacc.c:1652  */
    break;

  case 113:
#line 743 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  store(t);
			}
#line 3589 "surface.c" /* yacc.c:1652  */
    break;

  case 114:
#line 751 "surface.yy" /* yacc.c:1652  */
    {}
#line 3595 "surface.c" /* yacc.c:1652  */
    break;

  case 115:
#line 752 "surface.yy" /* yacc.c:1652  */
    {}
#line 3601 "surface.c" /* yacc.c:1652  */
    break;

  case 118:
#line 760 "surface.yy" /* yacc.c:1652  */
    {
			  currentSyntaxContainer->addParameter2((yyvsp[-2].yyToken), (yyvsp[0].yyModuleExpression));
			}
#line 3609 "surface.c" /* yacc.c:1652  */
    break;

  case 119:
#line 765 "surface.yy" /* yacc.c:1652  */
    {}
#line 3615 "surface.c" /* yacc.c:1652  */
    break;

  case 120:
#line 767 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			}
#line 3625 "surface.c" /* yacc.c:1652  */
    break;

  case 121:
#line 775 "surface.yy" /* yacc.c:1652  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[0].yyToken);  // needed for line number
			}
#line 3636 "surface.c" /* yacc.c:1652  */
    break;

  case 123:
#line 784 "surface.yy" /* yacc.c:1652  */
    {}
#line 3642 "surface.c" /* yacc.c:1652  */
    break;

  case 127:
#line 791 "surface.yy" /* yacc.c:1652  */
    {}
#line 3648 "surface.c" /* yacc.c:1652  */
    break;

  case 128:
#line 795 "surface.yy" /* yacc.c:1652  */
    {
			  CM->addImport((yyvsp[-1].yyToken), (yyvsp[0].yyModuleExpression));
			}
#line 3656 "surface.c" /* yacc.c:1652  */
    break;

  case 129:
#line 799 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3662 "surface.c" /* yacc.c:1652  */
    break;

  case 130:
#line 800 "surface.yy" /* yacc.c:1652  */
    { CM->addSortDecl(tokenSequence); }
#line 3668 "surface.c" /* yacc.c:1652  */
    break;

  case 131:
#line 802 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3674 "surface.c" /* yacc.c:1652  */
    break;

  case 132:
#line 803 "surface.yy" /* yacc.c:1652  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3680 "surface.c" /* yacc.c:1652  */
    break;

  case 133:
#line 805 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COLON, 1); }
#line 3686 "surface.c" /* yacc.c:1652  */
    break;

  case 134:
#line 806 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3692 "surface.c" /* yacc.c:1652  */
    break;

  case 135:
#line 807 "surface.yy" /* yacc.c:1652  */
    {}
#line 3698 "surface.c" /* yacc.c:1652  */
    break;

  case 136:
#line 809 "surface.yy" /* yacc.c:1652  */
    {}
#line 3704 "surface.c" /* yacc.c:1652  */
    break;

  case 137:
#line 811 "surface.yy" /* yacc.c:1652  */
    {}
#line 3710 "surface.c" /* yacc.c:1652  */
    break;

  case 138:
#line 813 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1); }
#line 3716 "surface.c" /* yacc.c:1652  */
    break;

  case 139:
#line 814 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3722 "surface.c" /* yacc.c:1652  */
    break;

  case 140:
#line 815 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3728 "surface.c" /* yacc.c:1652  */
    break;

  case 141:
#line 817 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1);  }
#line 3734 "surface.c" /* yacc.c:1652  */
    break;

  case 142:
#line 818 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3740 "surface.c" /* yacc.c:1652  */
    break;

  case 143:
#line 819 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3746 "surface.c" /* yacc.c:1652  */
    break;

  case 144:
#line 820 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3752 "surface.c" /* yacc.c:1652  */
    break;

  case 145:
#line 822 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3758 "surface.c" /* yacc.c:1652  */
    break;

  case 146:
#line 823 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3764 "surface.c" /* yacc.c:1652  */
    break;

  case 147:
#line 824 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3770 "surface.c" /* yacc.c:1652  */
    break;

  case 148:
#line 826 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3776 "surface.c" /* yacc.c:1652  */
    break;

  case 149:
#line 827 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3782 "surface.c" /* yacc.c:1652  */
    break;

  case 150:
#line 828 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3788 "surface.c" /* yacc.c:1652  */
    break;

  case 151:
#line 829 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3794 "surface.c" /* yacc.c:1652  */
    break;

  case 152:
#line 831 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3800 "surface.c" /* yacc.c:1652  */
    break;

  case 153:
#line 832 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3806 "surface.c" /* yacc.c:1652  */
    break;

  case 154:
#line 833 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3812 "surface.c" /* yacc.c:1652  */
    break;

  case 155:
#line 835 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3818 "surface.c" /* yacc.c:1652  */
    break;

  case 156:
#line 836 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3824 "surface.c" /* yacc.c:1652  */
    break;

  case 157:
#line 837 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3830 "surface.c" /* yacc.c:1652  */
    break;

  case 158:
#line 838 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3836 "surface.c" /* yacc.c:1652  */
    break;

  case 159:
#line 840 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_ASSIGN, 1); }
#line 3842 "surface.c" /* yacc.c:1652  */
    break;

  case 160:
#line 841 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3848 "surface.c" /* yacc.c:1652  */
    break;

  case 161:
#line 842 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3854 "surface.c" /* yacc.c:1652  */
    break;

  case 162:
#line 844 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_ASSIGN, 1); }
#line 3860 "surface.c" /* yacc.c:1652  */
    break;

  case 163:
#line 845 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3866 "surface.c" /* yacc.c:1652  */
    break;

  case 164:
#line 846 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3872 "surface.c" /* yacc.c:1652  */
    break;

  case 165:
#line 847 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3878 "surface.c" /* yacc.c:1652  */
    break;

  case 166:
#line 849 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3884 "surface.c" /* yacc.c:1652  */
    break;

  case 167:
#line 853 "surface.yy" /* yacc.c:1652  */
    {}
#line 3890 "surface.c" /* yacc.c:1652  */
    break;

  case 168:
#line 855 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COLON, 1); }
#line 3896 "surface.c" /* yacc.c:1652  */
    break;

  case 169:
#line 856 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3902 "surface.c" /* yacc.c:1652  */
    break;

  case 170:
#line 857 "surface.yy" /* yacc.c:1652  */
    { CM->setFlag(SymbolType::MESSAGE); }
#line 3908 "surface.c" /* yacc.c:1652  */
    break;

  case 171:
#line 860 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3916 "surface.c" /* yacc.c:1652  */
    break;

  case 172:
#line 865 "surface.yy" /* yacc.c:1652  */
    {
			}
#line 3923 "surface.c" /* yacc.c:1652  */
    break;

  case 173:
#line 868 "surface.yy" /* yacc.c:1652  */
    {
			}
#line 3930 "surface.c" /* yacc.c:1652  */
    break;

  case 174:
#line 871 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3936 "surface.c" /* yacc.c:1652  */
    break;

  case 175:
#line 872 "surface.yy" /* yacc.c:1652  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3942 "surface.c" /* yacc.c:1652  */
    break;

  case 176:
#line 875 "surface.yy" /* yacc.c:1652  */
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeDeclsConsistent();
			}
#line 3955 "surface.c" /* yacc.c:1652  */
    break;

  case 177:
#line 885 "surface.yy" /* yacc.c:1652  */
    {}
#line 3961 "surface.c" /* yacc.c:1652  */
    break;

  case 178:
#line 886 "surface.yy" /* yacc.c:1652  */
    {}
#line 3967 "surface.c" /* yacc.c:1652  */
    break;

  case 181:
#line 894 "surface.yy" /* yacc.c:1652  */
    {
			}
#line 3974 "surface.c" /* yacc.c:1652  */
    break;

  case 182:
#line 898 "surface.yy" /* yacc.c:1652  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3980 "surface.c" /* yacc.c:1652  */
    break;

  case 183:
#line 899 "surface.yy" /* yacc.c:1652  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3986 "surface.c" /* yacc.c:1652  */
    break;

  case 186:
#line 906 "surface.yy" /* yacc.c:1652  */
    { singleton[0] = (yyvsp[0].yyToken); CM->addOpDecl(singleton); }
#line 3992 "surface.c" /* yacc.c:1652  */
    break;

  case 187:
#line 907 "surface.yy" /* yacc.c:1652  */
    { CM->addOpDecl(lexerBubble); }
#line 3998 "surface.c" /* yacc.c:1652  */
    break;

  case 190:
#line 913 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			}
#line 4007 "surface.c" /* yacc.c:1652  */
    break;

  case 195:
#line 925 "surface.yy" /* yacc.c:1652  */
    { CM->addStratDecl((yyvsp[0].yyToken)); }
#line 4013 "surface.c" /* yacc.c:1652  */
    break;

  case 198:
#line 936 "surface.yy" /* yacc.c:1652  */
    {}
#line 4019 "surface.c" /* yacc.c:1652  */
    break;

  case 199:
#line 937 "surface.yy" /* yacc.c:1652  */
    {}
#line 4025 "surface.c" /* yacc.c:1652  */
    break;

  case 200:
#line 941 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 4033 "surface.c" /* yacc.c:1652  */
    break;

  case 201:
#line 947 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping stray " << QUOTE(":") << " in operator declaration.");

			}
#line 4043 "surface.c" /* yacc.c:1652  */
    break;

  case 202:
#line 952 "surface.yy" /* yacc.c:1652  */
    {}
#line 4049 "surface.c" /* yacc.c:1652  */
    break;

  case 204:
#line 957 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 4058 "surface.c" /* yacc.c:1652  */
    break;

  case 205:
#line 962 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 4067 "surface.c" /* yacc.c:1652  */
    break;

  case 206:
#line 969 "surface.yy" /* yacc.c:1652  */
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			}
#line 4076 "surface.c" /* yacc.c:1652  */
    break;

  case 208:
#line 976 "surface.yy" /* yacc.c:1652  */
    {}
#line 4082 "surface.c" /* yacc.c:1652  */
    break;

  case 209:
#line 979 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 4088 "surface.c" /* yacc.c:1652  */
    break;

  case 210:
#line 980 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 4094 "surface.c" /* yacc.c:1652  */
    break;

  case 212:
#line 984 "surface.yy" /* yacc.c:1652  */
    {}
#line 4100 "surface.c" /* yacc.c:1652  */
    break;

  case 213:
#line 988 "surface.yy" /* yacc.c:1652  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			}
#line 4109 "surface.c" /* yacc.c:1652  */
    break;

  case 214:
#line 992 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4115 "surface.c" /* yacc.c:1652  */
    break;

  case 215:
#line 994 "surface.yy" /* yacc.c:1652  */
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
#line 4123 "surface.c" /* yacc.c:1652  */
    break;

  case 216:
#line 999 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4129 "surface.c" /* yacc.c:1652  */
    break;

  case 217:
#line 1000 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4135 "surface.c" /* yacc.c:1652  */
    break;

  case 218:
#line 1003 "surface.yy" /* yacc.c:1652  */
    {}
#line 4141 "surface.c" /* yacc.c:1652  */
    break;

  case 219:
#line 1004 "surface.yy" /* yacc.c:1652  */
    {}
#line 4147 "surface.c" /* yacc.c:1652  */
    break;

  case 222:
#line 1012 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			}
#line 4155 "surface.c" /* yacc.c:1652  */
    break;

  case 223:
#line 1016 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			}
#line 4163 "surface.c" /* yacc.c:1652  */
    break;

  case 224:
#line 1020 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			}
#line 4171 "surface.c" /* yacc.c:1652  */
    break;

  case 225:
#line 1026 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::ASSOC);
			}
#line 4179 "surface.c" /* yacc.c:1652  */
    break;

  case 226:
#line 1030 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::COMM);
			}
#line 4187 "surface.c" /* yacc.c:1652  */
    break;

  case 227:
#line 1033 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); }
#line 4193 "surface.c" /* yacc.c:1652  */
    break;

  case 228:
#line 1034 "surface.yy" /* yacc.c:1652  */
    { CM->setIdentity(lexerBubble); }
#line 4199 "surface.c" /* yacc.c:1652  */
    break;

  case 229:
#line 1036 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::IDEM);
			}
#line 4207 "surface.c" /* yacc.c:1652  */
    break;

  case 230:
#line 1040 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::ITER);
			}
#line 4215 "surface.c" /* yacc.c:1652  */
    break;

  case 231:
#line 1043 "surface.yy" /* yacc.c:1652  */
    { CM->setPrec((yyvsp[0].yyToken)); }
#line 4221 "surface.c" /* yacc.c:1652  */
    break;

  case 232:
#line 1044 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4227 "surface.c" /* yacc.c:1652  */
    break;

  case 233:
#line 1045 "surface.yy" /* yacc.c:1652  */
    { CM->setGather(tokenSequence); }
#line 4233 "surface.c" /* yacc.c:1652  */
    break;

  case 234:
#line 1046 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4239 "surface.c" /* yacc.c:1652  */
    break;

  case 235:
#line 1047 "surface.yy" /* yacc.c:1652  */
    { CM->setFormat(tokenSequence); }
#line 4245 "surface.c" /* yacc.c:1652  */
    break;

  case 236:
#line 1048 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4251 "surface.c" /* yacc.c:1652  */
    break;

  case 237:
#line 1049 "surface.yy" /* yacc.c:1652  */
    { CM->setStrat(tokenSequence); }
#line 4257 "surface.c" /* yacc.c:1652  */
    break;

  case 238:
#line 1050 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4263 "surface.c" /* yacc.c:1652  */
    break;

  case 239:
#line 1051 "surface.yy" /* yacc.c:1652  */
    { CM->setStrat(tokenSequence); }
#line 4269 "surface.c" /* yacc.c:1652  */
    break;

  case 240:
#line 1052 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4275 "surface.c" /* yacc.c:1652  */
    break;

  case 241:
#line 1053 "surface.yy" /* yacc.c:1652  */
    { CM->setPoly(tokenSequence); }
#line 4281 "surface.c" /* yacc.c:1652  */
    break;

  case 242:
#line 1055 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::MEMO);
			}
#line 4289 "surface.c" /* yacc.c:1652  */
    break;

  case 243:
#line 1059 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::CTOR);
			}
#line 4297 "surface.c" /* yacc.c:1652  */
    break;

  case 244:
#line 1063 "surface.yy" /* yacc.c:1652  */
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			}
#line 4306 "surface.c" /* yacc.c:1652  */
    break;

  case 245:
#line 1067 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4312 "surface.c" /* yacc.c:1652  */
    break;

  case 246:
#line 1068 "surface.yy" /* yacc.c:1652  */
    { CM->setFrozen(tokenSequence); }
#line 4318 "surface.c" /* yacc.c:1652  */
    break;

  case 247:
#line 1070 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::CONFIG);
			}
#line 4326 "surface.c" /* yacc.c:1652  */
    break;

  case 248:
#line 1074 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::OBJECT);
			}
#line 4334 "surface.c" /* yacc.c:1652  */
    break;

  case 249:
#line 1078 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 4342 "surface.c" /* yacc.c:1652  */
    break;

  case 250:
#line 1082 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 4350 "surface.c" /* yacc.c:1652  */
    break;

  case 251:
#line 1085 "surface.yy" /* yacc.c:1652  */
    { lexerLatexMode(); }
#line 4356 "surface.c" /* yacc.c:1652  */
    break;

  case 252:
#line 1086 "surface.yy" /* yacc.c:1652  */
    { CM->setLatexMacro((yyvsp[-1].yyString)); }
#line 4362 "surface.c" /* yacc.c:1652  */
    break;

  case 253:
#line 1087 "surface.yy" /* yacc.c:1652  */
    {}
#line 4368 "surface.c" /* yacc.c:1652  */
    break;

  case 254:
#line 1089 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::DITTO);
			}
#line 4376 "surface.c" /* yacc.c:1652  */
    break;

  case 256:
#line 1099 "surface.yy" /* yacc.c:1652  */
    {}
#line 4382 "surface.c" /* yacc.c:1652  */
    break;

  case 257:
#line 1102 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4388 "surface.c" /* yacc.c:1652  */
    break;

  case 258:
#line 1103 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4394 "surface.c" /* yacc.c:1652  */
    break;

  case 259:
#line 1106 "surface.yy" /* yacc.c:1652  */
    {}
#line 4400 "surface.c" /* yacc.c:1652  */
    break;

  case 260:
#line 1107 "surface.yy" /* yacc.c:1652  */
    {}
#line 4406 "surface.c" /* yacc.c:1652  */
    break;

  case 261:
#line 1110 "surface.yy" /* yacc.c:1652  */
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[0].yyToken), tokenSequence); }
#line 4412 "surface.c" /* yacc.c:1652  */
    break;

  case 262:
#line 1111 "surface.yy" /* yacc.c:1652  */
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4418 "surface.c" /* yacc.c:1652  */
    break;

  case 263:
#line 1112 "surface.yy" /* yacc.c:1652  */
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4424 "surface.c" /* yacc.c:1652  */
    break;

  case 264:
#line 1113 "surface.yy" /* yacc.c:1652  */
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4430 "surface.c" /* yacc.c:1652  */
    break;

  case 265:
#line 1119 "surface.yy" /* yacc.c:1652  */
    {}
#line 4436 "surface.c" /* yacc.c:1652  */
    break;

  case 266:
#line 1121 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			}
#line 4445 "surface.c" /* yacc.c:1652  */
    break;

  case 267:
#line 1127 "surface.yy" /* yacc.c:1652  */
    {}
#line 4451 "surface.c" /* yacc.c:1652  */
    break;

  case 268:
#line 1129 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
#line 4459 "surface.c" /* yacc.c:1652  */
    break;

  case 269:
#line 1137 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4465 "surface.c" /* yacc.c:1652  */
    break;

  case 270:
#line 1138 "surface.yy" /* yacc.c:1652  */
    {}
#line 4471 "surface.c" /* yacc.c:1652  */
    break;

  case 271:
#line 1141 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4477 "surface.c" /* yacc.c:1652  */
    break;

  case 272:
#line 1142 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4483 "surface.c" /* yacc.c:1652  */
    break;

  case 273:
#line 1143 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4489 "surface.c" /* yacc.c:1652  */
    break;

  case 274:
#line 1144 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4495 "surface.c" /* yacc.c:1652  */
    break;

  case 275:
#line 1151 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  (yyval.yyToken) = t;
			}
#line 4509 "surface.c" /* yacc.c:1652  */
    break;

  case 276:
#line 1162 "surface.yy" /* yacc.c:1652  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4515 "surface.c" /* yacc.c:1652  */
    break;

  case 277:
#line 1163 "surface.yy" /* yacc.c:1652  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4521 "surface.c" /* yacc.c:1652  */
    break;

  case 278:
#line 1164 "surface.yy" /* yacc.c:1652  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4527 "surface.c" /* yacc.c:1652  */
    break;

  case 279:
#line 1167 "surface.yy" /* yacc.c:1652  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4533 "surface.c" /* yacc.c:1652  */
    break;

  case 280:
#line 1168 "surface.yy" /* yacc.c:1652  */
    {}
#line 4539 "surface.c" /* yacc.c:1652  */
    break;

  case 281:
#line 1169 "surface.yy" /* yacc.c:1652  */
    {}
#line 4545 "surface.c" /* yacc.c:1652  */
    break;

  case 387:
#line 1250 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 1); }
#line 4551 "surface.c" /* yacc.c:1652  */
    break;

  case 388:
#line 1252 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setCurrentModule(lexerBubble);
			}
#line 4559 "surface.c" /* yacc.c:1652  */
    break;

  case 389:
#line 1255 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 1); }
#line 4565 "surface.c" /* yacc.c:1652  */
    break;

  case 390:
#line 1257 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			}
#line 4574 "surface.c" /* yacc.c:1652  */
    break;

  case 391:
#line 1262 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4583 "surface.c" /* yacc.c:1652  */
    break;

  case 392:
#line 1267 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			}
#line 4593 "surface.c" /* yacc.c:1652  */
    break;

  case 393:
#line 1274 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4602 "surface.c" /* yacc.c:1652  */
    break;

  case 394:
#line 1279 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			}
#line 4612 "surface.c" /* yacc.c:1652  */
    break;

  case 395:
#line 1286 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4621 "surface.c" /* yacc.c:1652  */
    break;

  case 396:
#line 1291 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			}
#line 4631 "surface.c" /* yacc.c:1652  */
    break;

  case 397:
#line 1298 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4640 "surface.c" /* yacc.c:1652  */
    break;

  case 398:
#line 1303 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[-3].yyBool));
			}
#line 4650 "surface.c" /* yacc.c:1652  */
    break;

  case 399:
#line 1310 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4660 "surface.c" /* yacc.c:1652  */
    break;

  case 400:
#line 1316 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4670 "surface.c" /* yacc.c:1652  */
    break;

  case 401:
#line 1322 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4681 "surface.c" /* yacc.c:1652  */
    break;

  case 402:
#line 1329 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4691 "surface.c" /* yacc.c:1652  */
    break;

  case 403:
#line 1335 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4702 "surface.c" /* yacc.c:1652  */
    break;

  case 404:
#line 1342 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4712 "surface.c" /* yacc.c:1652  */
    break;

  case 405:
#line 1348 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4722 "surface.c" /* yacc.c:1652  */
    break;

  case 406:
#line 1354 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4732 "surface.c" /* yacc.c:1652  */
    break;

  case 407:
#line 1360 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4742 "surface.c" /* yacc.c:1652  */
    break;

  case 408:
#line 1366 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool), true);
			}
#line 4752 "surface.c" /* yacc.c:1652  */
    break;

  case 409:
#line 1373 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4761 "surface.c" /* yacc.c:1652  */
    break;

  case 410:
#line 1378 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.check(lexerBubble);
			}
#line 4771 "surface.c" /* yacc.c:1652  */
    break;

  case 411:
#line 1385 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4782 "surface.c" /* yacc.c:1652  */
    break;

  case 412:
#line 1392 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[-2].yySearchKind), (yyvsp[-3].yyBool));
			}
#line 4792 "surface.c" /* yacc.c:1652  */
    break;

  case 413:
#line 1398 "surface.yy" /* yacc.c:1652  */
    {
			  variantOptions = (yyvsp[-1].yyInt64);
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;			  
			}
#line 4804 "surface.c" /* yacc.c:1652  */
    break;

  case 414:
#line 1406 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    {
			      interpreter.search(lexerBubble, number, number2,
			                         Interpreter::VU_NARROW, (yyvsp[-4].yyBool), variantOptions);
			    }
			}
#line 4817 "surface.c" /* yacc.c:1652  */
    break;

  case 415:
#line 1415 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4827 "surface.c" /* yacc.c:1652  */
    break;

  case 416:
#line 1421 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[-2].yyBool), number);
			}
#line 4837 "surface.c" /* yacc.c:1652  */
    break;

  case 417:
#line 1427 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4847 "surface.c" /* yacc.c:1652  */
    break;

  case 418:
#line 1433 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4857 "surface.c" /* yacc.c:1652  */
    break;

  case 419:
#line 1439 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4867 "surface.c" /* yacc.c:1652  */
    break;

  case 420:
#line 1445 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, false, (yyvsp[-4].yyBool));
			}
#line 4877 "surface.c" /* yacc.c:1652  */
    break;

  case 421:
#line 1451 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4887 "surface.c" /* yacc.c:1652  */
    break;

  case 422:
#line 1457 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, true, (yyvsp[-5].yyBool));
			}
#line 4897 "surface.c" /* yacc.c:1652  */
    break;

  case 423:
#line 1463 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4907 "surface.c" /* yacc.c:1652  */
    break;

  case 424:
#line 1469 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantMatch(lexerBubble, number, (yyvsp[-4].yyBool));
			}
#line 4917 "surface.c" /* yacc.c:1652  */
    break;

  case 425:
#line 1476 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4927 "surface.c" /* yacc.c:1652  */
    break;

  case 426:
#line 1482 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.getVariants(lexerBubble, number, (yyvsp[-3].yyBool), (yyvsp[-5].yyBool));
			}
#line 4937 "surface.c" /* yacc.c:1652  */
    break;

  case 427:
#line 1488 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.cont((yyvsp[-1].yyInt64), (yyvsp[-3].yyBool));
			}
#line 4945 "surface.c" /* yacc.c:1652  */
    break;

  case 428:
#line 1492 "surface.yy" /* yacc.c:1652  */
    {
			  //
			  //	test is a generic command to call code with a term for development purposes.
			  //
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4957 "surface.c" /* yacc.c:1652  */
    break;

  case 429:
#line 1500 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.test(lexerBubble);
			    //interpreter.newNarrow(lexerBubble);

			}
#line 4969 "surface.c" /* yacc.c:1652  */
    break;

  case 430:
#line 1508 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4978 "surface.c" /* yacc.c:1652  */
    break;

  case 431:
#line 1513 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			}
#line 4988 "surface.c" /* yacc.c:1652  */
    break;

  case 432:
#line 1519 "surface.yy" /* yacc.c:1652  */
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			}
#line 4998 "surface.c" /* yacc.c:1652  */
    break;

  case 433:
#line 1524 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 5004 "surface.c" /* yacc.c:1652  */
    break;

  case 434:
#line 1526 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[-2].yyBool));
			}
#line 5013 "surface.c" /* yacc.c:1652  */
    break;

  case 435:
#line 1530 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 5019 "surface.c" /* yacc.c:1652  */
    break;

  case 436:
#line 1532 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[-2].yyBool));
			}
#line 5028 "surface.c" /* yacc.c:1652  */
    break;

  case 437:
#line 1536 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 5034 "surface.c" /* yacc.c:1652  */
    break;

  case 438:
#line 1538 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[-2].yyBool));
			}
#line 5043 "surface.c" /* yacc.c:1652  */
    break;

  case 439:
#line 1542 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 5049 "surface.c" /* yacc.c:1652  */
    break;

  case 440:
#line 1544 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[-2].yyBool));
			}
#line 5058 "surface.c" /* yacc.c:1652  */
    break;

  case 441:
#line 1548 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5064 "surface.c" /* yacc.c:1652  */
    break;

  case 442:
#line 1550 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->getFlatSignature()->clearMemo();
			}
#line 5073 "surface.c" /* yacc.c:1652  */
    break;

  case 443:
#line 1557 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5079 "surface.c" /* yacc.c:1652  */
    break;

  case 444:
#line 1559 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 5088 "surface.c" /* yacc.c:1652  */
    break;

  case 445:
#line 1563 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5094 "surface.c" /* yacc.c:1652  */
    break;

  case 446:
#line 1565 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 5103 "surface.c" /* yacc.c:1652  */
    break;

  case 447:
#line 1569 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5109 "surface.c" /* yacc.c:1652  */
    break;

  case 448:
#line 1571 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			}
#line 5118 "surface.c" /* yacc.c:1652  */
    break;

  case 449:
#line 1575 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5124 "surface.c" /* yacc.c:1652  */
    break;

  case 450:
#line 1577 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			}
#line 5133 "surface.c" /* yacc.c:1652  */
    break;

  case 451:
#line 1582 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showModules(true);
			}
#line 5141 "surface.c" /* yacc.c:1652  */
    break;

  case 452:
#line 1586 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showViews(true);
			}
#line 5149 "surface.c" /* yacc.c:1652  */
    break;

  case 453:
#line 1589 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5155 "surface.c" /* yacc.c:1652  */
    break;

  case 454:
#line 1591 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			}
#line 5164 "surface.c" /* yacc.c:1652  */
    break;

  case 455:
#line 1595 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5170 "surface.c" /* yacc.c:1652  */
    break;

  case 456:
#line 1597 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			}
#line 5179 "surface.c" /* yacc.c:1652  */
    break;

  case 457:
#line 1601 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5185 "surface.c" /* yacc.c:1652  */
    break;

  case 458:
#line 1603 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			}
#line 5194 "surface.c" /* yacc.c:1652  */
    break;

  case 459:
#line 1607 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5200 "surface.c" /* yacc.c:1652  */
    break;

  case 460:
#line 1609 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			}
#line 5209 "surface.c" /* yacc.c:1652  */
    break;

  case 461:
#line 1613 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5215 "surface.c" /* yacc.c:1652  */
    break;

  case 462:
#line 1615 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			}
#line 5224 "surface.c" /* yacc.c:1652  */
    break;

  case 463:
#line 1619 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5230 "surface.c" /* yacc.c:1652  */
    break;

  case 464:
#line 1621 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			}
#line 5239 "surface.c" /* yacc.c:1652  */
    break;

  case 465:
#line 1625 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5245 "surface.c" /* yacc.c:1652  */
    break;

  case 466:
#line 1627 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showStrats();
			}
#line 5254 "surface.c" /* yacc.c:1652  */
    break;

  case 467:
#line 1631 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5260 "surface.c" /* yacc.c:1652  */
    break;

  case 468:
#line 1633 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSds();
			}
#line 5269 "surface.c" /* yacc.c:1652  */
    break;

  case 469:
#line 1637 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5275 "surface.c" /* yacc.c:1652  */
    break;

  case 470:
#line 1639 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			}
#line 5284 "surface.c" /* yacc.c:1652  */
    break;

  case 471:
#line 1643 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5290 "surface.c" /* yacc.c:1652  */
    break;

  case 472:
#line 1645 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			}
#line 5299 "surface.c" /* yacc.c:1652  */
    break;

  case 473:
#line 1650 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showSearchPath((yyvsp[-1].yyInt64));
			}
#line 5307 "surface.c" /* yacc.c:1652  */
    break;

  case 474:
#line 1654 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showSearchPathLabels((yyvsp[-1].yyInt64));
			}
#line 5315 "surface.c" /* yacc.c:1652  */
    break;

  case 475:
#line 1658 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showSearchGraph();
			}
#line 5323 "surface.c" /* yacc.c:1652  */
    break;

  case 476:
#line 1661 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5329 "surface.c" /* yacc.c:1652  */
    break;

  case 477:
#line 1663 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			}
#line 5338 "surface.c" /* yacc.c:1652  */
    break;

  case 478:
#line 1671 "surface.yy" /* yacc.c:1652  */
    {
			  globalAdvisoryFlag = alwaysAdviseFlag ? true : (yyvsp[-1].yyBool);
			}
#line 5346 "surface.c" /* yacc.c:1652  */
    break;

  case 479:
#line 1675 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 5354 "surface.c" /* yacc.c:1652  */
    break;

  case 480:
#line 1679 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[-1].yyBool));
			}
#line 5362 "surface.c" /* yacc.c:1652  */
    break;

  case 481:
#line 1683 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[-1].yyBool));
			}
#line 5370 "surface.c" /* yacc.c:1652  */
    break;

  case 482:
#line 1687 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[-1].yyBool));
			}
#line 5378 "surface.c" /* yacc.c:1652  */
    break;

  case 483:
#line 1691 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[-1].yyBool));
			}
#line 5386 "surface.c" /* yacc.c:1652  */
    break;

  case 484:
#line 1695 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[-1].yyBool));
			}
#line 5394 "surface.c" /* yacc.c:1652  */
    break;

  case 485:
#line 1699 "surface.yy" /* yacc.c:1652  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 5402 "surface.c" /* yacc.c:1652  */
    break;

  case 486:
#line 1703 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setPrintFlag((yyvsp[-2].yyPrintFlags), (yyvsp[-1].yyBool));
			}
#line 5410 "surface.c" /* yacc.c:1652  */
    break;

  case 487:
#line 1707 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[-1].yyBool));
			}
#line 5418 "surface.c" /* yacc.c:1652  */
    break;

  case 488:
#line 1711 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[-1].yyBool));
			}
#line 5426 "surface.c" /* yacc.c:1652  */
    break;

  case 489:
#line 1715 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag((yyvsp[-2].yyFlags), (yyvsp[-1].yyBool));
			}
#line 5434 "surface.c" /* yacc.c:1652  */
    break;

  case 490:
#line 1719 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[-1].yyBool));
			}
#line 5442 "surface.c" /* yacc.c:1652  */
    break;

  case 491:
#line 1722 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 5448 "surface.c" /* yacc.c:1652  */
    break;

  case 492:
#line 1723 "surface.yy" /* yacc.c:1652  */
    { lexerInitialMode(); }
#line 5454 "surface.c" /* yacc.c:1652  */
    break;

  case 493:
#line 1725 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setAutoImport((yyvsp[-5].yyImportMode), (yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 5462 "surface.c" /* yacc.c:1652  */
    break;

  case 494:
#line 1728 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 5468 "surface.c" /* yacc.c:1652  */
    break;

  case 495:
#line 1729 "surface.yy" /* yacc.c:1652  */
    { lexerInitialMode(); }
#line 5474 "surface.c" /* yacc.c:1652  */
    break;

  case 496:
#line 1731 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setOmodInclude((yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 5482 "surface.c" /* yacc.c:1652  */
    break;

  case 497:
#line 1735 "surface.yy" /* yacc.c:1652  */
    {
			  globalVerboseFlag = (yyvsp[-1].yyBool);
			}
#line 5490 "surface.c" /* yacc.c:1652  */
    break;

  case 498:
#line 1739 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[-1].yyBool));
			}
#line 5498 "surface.c" /* yacc.c:1652  */
    break;

  case 499:
#line 1743 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[-1].yyBool));
			}
#line 5506 "surface.c" /* yacc.c:1652  */
    break;

  case 500:
#line 1747 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[-1].yyBool));
			}
#line 5514 "surface.c" /* yacc.c:1652  */
    break;

  case 501:
#line 1751 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[-1].yyBool));
			}
#line 5522 "surface.c" /* yacc.c:1652  */
    break;

  case 502:
#line 1755 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[-1].yyBool));
			}
#line 5530 "surface.c" /* yacc.c:1652  */
    break;

  case 503:
#line 1762 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			}
#line 5538 "surface.c" /* yacc.c:1652  */
    break;

  case 504:
#line 1766 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			}
#line 5546 "surface.c" /* yacc.c:1652  */
    break;

  case 505:
#line 1770 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
#line 5554 "surface.c" /* yacc.c:1652  */
    break;

  case 506:
#line 1774 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			}
#line 5562 "surface.c" /* yacc.c:1652  */
    break;

  case 507:
#line 1781 "surface.yy" /* yacc.c:1652  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 5570 "surface.c" /* yacc.c:1652  */
    break;

  case 508:
#line 1785 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 5578 "surface.c" /* yacc.c:1652  */
    break;

  case 509:
#line 1791 "surface.yy" /* yacc.c:1652  */
    { lexerInitialMode(); }
#line 5584 "surface.c" /* yacc.c:1652  */
    break;

  case 511:
#line 1798 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; }
#line 5590 "surface.c" /* yacc.c:1652  */
    break;

  case 512:
#line 1799 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; }
#line 5596 "surface.c" /* yacc.c:1652  */
    break;

  case 513:
#line 1800 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; }
#line 5602 "surface.c" /* yacc.c:1652  */
    break;

  case 514:
#line 1801 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; }
#line 5608 "surface.c" /* yacc.c:1652  */
    break;

  case 515:
#line 1802 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; }
#line 5614 "surface.c" /* yacc.c:1652  */
    break;

  case 516:
#line 1803 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; }
#line 5620 "surface.c" /* yacc.c:1652  */
    break;

  case 517:
#line 1804 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; }
#line 5626 "surface.c" /* yacc.c:1652  */
    break;

  case 518:
#line 1805 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; }
#line 5632 "surface.c" /* yacc.c:1652  */
    break;

  case 519:
#line 1806 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; }
#line 5638 "surface.c" /* yacc.c:1652  */
    break;

  case 520:
#line 1807 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; }
#line 5644 "surface.c" /* yacc.c:1652  */
    break;

  case 521:
#line 1808 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_DISAMBIG_CONST; }
#line 5650 "surface.c" /* yacc.c:1652  */
    break;

  case 522:
#line 1811 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE; }
#line 5656 "surface.c" /* yacc.c:1652  */
    break;

  case 523:
#line 1812 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; }
#line 5662 "surface.c" /* yacc.c:1652  */
    break;

  case 524:
#line 1813 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; }
#line 5668 "surface.c" /* yacc.c:1652  */
    break;

  case 525:
#line 1814 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; }
#line 5674 "surface.c" /* yacc.c:1652  */
    break;

  case 526:
#line 1815 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; }
#line 5680 "surface.c" /* yacc.c:1652  */
    break;

  case 527:
#line 1816 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_MB; }
#line 5686 "surface.c" /* yacc.c:1652  */
    break;

  case 528:
#line 1817 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; }
#line 5692 "surface.c" /* yacc.c:1652  */
    break;

  case 529:
#line 1818 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_RL; }
#line 5698 "surface.c" /* yacc.c:1652  */
    break;

  case 530:
#line 1819 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_SD; }
#line 5704 "surface.c" /* yacc.c:1652  */
    break;

  case 531:
#line 1820 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; }
#line 5710 "surface.c" /* yacc.c:1652  */
    break;

  case 532:
#line 1821 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; }
#line 5716 "surface.c" /* yacc.c:1652  */
    break;

  case 533:
#line 1822 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; }
#line 5722 "surface.c" /* yacc.c:1652  */
    break;

  case 534:
#line 1825 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5728 "surface.c" /* yacc.c:1652  */
    break;

  case 535:
#line 1826 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5734 "surface.c" /* yacc.c:1652  */
    break;

  case 536:
#line 1829 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5740 "surface.c" /* yacc.c:1652  */
    break;

  case 537:
#line 1830 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5746 "surface.c" /* yacc.c:1652  */
    break;

  case 538:
#line 1833 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5752 "surface.c" /* yacc.c:1652  */
    break;

  case 539:
#line 1834 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5758 "surface.c" /* yacc.c:1652  */
    break;

  case 540:
#line 1837 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5764 "surface.c" /* yacc.c:1652  */
    break;

  case 541:
#line 1838 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5770 "surface.c" /* yacc.c:1652  */
    break;

  case 542:
#line 1843 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::NARROW; }
#line 5776 "surface.c" /* yacc.c:1652  */
    break;

  case 543:
#line 1844 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; }
#line 5782 "surface.c" /* yacc.c:1652  */
    break;

  case 544:
#line 1845 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::SEARCH; }
#line 5788 "surface.c" /* yacc.c:1652  */
    break;

  case 545:
#line 1846 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::SMT_SEARCH; }
#line 5794 "surface.c" /* yacc.c:1652  */
    break;

  case 546:
#line 1847 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::FVU_NARROW; }
#line 5800 "surface.c" /* yacc.c:1652  */
    break;

  case 547:
#line 1850 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5806 "surface.c" /* yacc.c:1652  */
    break;

  case 548:
#line 1851 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5812 "surface.c" /* yacc.c:1652  */
    break;

  case 549:
#line 1854 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5818 "surface.c" /* yacc.c:1652  */
    break;

  case 550:
#line 1855 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5824 "surface.c" /* yacc.c:1652  */
    break;

  case 551:
#line 1858 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5830 "surface.c" /* yacc.c:1652  */
    break;

  case 552:
#line 1859 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5836 "surface.c" /* yacc.c:1652  */
    break;

  case 553:
#line 1862 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); }
#line 5842 "surface.c" /* yacc.c:1652  */
    break;

  case 554:
#line 1863 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = NONE; }
#line 5848 "surface.c" /* yacc.c:1652  */
    break;

  case 555:
#line 1866 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = (yyvsp[-1].yyInt64); }
#line 5854 "surface.c" /* yacc.c:1652  */
    break;

  case 556:
#line 1867 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = 0; }
#line 5860 "surface.c" /* yacc.c:1652  */
    break;

  case 557:
#line 1870 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); }
#line 5866 "surface.c" /* yacc.c:1652  */
    break;

  case 558:
#line 1871 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = (yyvsp[-2].yyInt64) | (yyvsp[0].yyInt64); }
#line 5872 "surface.c" /* yacc.c:1652  */
    break;

  case 559:
#line 1874 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = NarrowingSequenceSearch3::FOLD; }
#line 5878 "surface.c" /* yacc.c:1652  */
    break;

  case 560:
#line 1877 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyImportMode) = ImportModule::PROTECTING; }
#line 5884 "surface.c" /* yacc.c:1652  */
    break;

  case 561:
#line 1878 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyImportMode) = ImportModule::EXTENDING; }
#line 5890 "surface.c" /* yacc.c:1652  */
    break;

  case 562:
#line 1879 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyImportMode) = ImportModule::INCLUDING; }
#line 5896 "surface.c" /* yacc.c:1652  */
    break;

  case 564:
#line 1889 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5902 "surface.c" /* yacc.c:1652  */
    break;

  case 566:
#line 1893 "surface.yy" /* yacc.c:1652  */
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); }
#line 5908 "surface.c" /* yacc.c:1652  */
    break;

  case 569:
#line 1905 "surface.yy" /* yacc.c:1652  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5914 "surface.c" /* yacc.c:1652  */
    break;

  case 572:
#line 1908 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0);  }
#line 5920 "surface.c" /* yacc.c:1652  */
    break;

  case 574:
#line 1913 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5926 "surface.c" /* yacc.c:1652  */
    break;

  case 576:
#line 1915 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5932 "surface.c" /* yacc.c:1652  */
    break;

  case 579:
#line 1921 "surface.yy" /* yacc.c:1652  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5938 "surface.c" /* yacc.c:1652  */
    break;

  case 581:
#line 1923 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5944 "surface.c" /* yacc.c:1652  */
    break;

  case 584:
#line 1939 "surface.yy" /* yacc.c:1652  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5950 "surface.c" /* yacc.c:1652  */
    break;

  case 586:
#line 1941 "surface.yy" /* yacc.c:1652  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5956 "surface.c" /* yacc.c:1652  */
    break;

  case 589:
#line 1944 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5962 "surface.c" /* yacc.c:1652  */
    break;

  case 591:
#line 1950 "surface.yy" /* yacc.c:1652  */
    {
			  lexContinueSave((yyvsp[0].yyToken));
			  variantOptions |= VariantUnificationProblem::FILTER_VARIANT_UNIFIERS;
			}
#line 5971 "surface.c" /* yacc.c:1652  */
    break;

  case 593:
#line 1956 "surface.yy" /* yacc.c:1652  */
    {
			  lexContinueSave((yyvsp[0].yyToken));
			  variantOptions |= VariantSearch::IRREDUNDANT_MODE;
			}
#line 5980 "surface.c" /* yacc.c:1652  */
    break;

  case 595:
#line 1961 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5986 "surface.c" /* yacc.c:1652  */
    break;

  case 599:
#line 1969 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5992 "surface.c" /* yacc.c:1652  */
    break;

  case 601:
#line 1971 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5998 "surface.c" /* yacc.c:1652  */
    break;

  case 604:
#line 1980 "surface.yy" /* yacc.c:1652  */
    { lexSave((yyvsp[0].yyToken)); }
#line 6004 "surface.c" /* yacc.c:1652  */
    break;

  case 607:
#line 1983 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 6010 "surface.c" /* yacc.c:1652  */
    break;

  case 609:
#line 1992 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 6016 "surface.c" /* yacc.c:1652  */
    break;

  case 611:
#line 1994 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 6022 "surface.c" /* yacc.c:1652  */
    break;

  case 613:
#line 1996 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 6028 "surface.c" /* yacc.c:1652  */
    break;

  case 616:
#line 2006 "surface.yy" /* yacc.c:1652  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 6034 "surface.c" /* yacc.c:1652  */
    break;

  case 618:
#line 2008 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 6040 "surface.c" /* yacc.c:1652  */
    break;

  case 620:
#line 2010 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 6046 "surface.c" /* yacc.c:1652  */
    break;

  case 623:
#line 2020 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 6052 "surface.c" /* yacc.c:1652  */
    break;

  case 625:
#line 2022 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 6058 "surface.c" /* yacc.c:1652  */
    break;

  case 628:
#line 2033 "surface.yy" /* yacc.c:1652  */
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			}
#line 6068 "surface.c" /* yacc.c:1652  */
    break;

  case 630:
#line 2039 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 6074 "surface.c" /* yacc.c:1652  */
    break;

  case 633:
#line 2049 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 6080 "surface.c" /* yacc.c:1652  */
    break;

  case 635:
#line 2051 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 6086 "surface.c" /* yacc.c:1652  */
    break;

  case 638:
#line 2061 "surface.yy" /* yacc.c:1652  */
    { number2 = Token::codeToInt64(lexerBubble[2].code()); }
#line 6092 "surface.c" /* yacc.c:1652  */
    break;

  case 640:
#line 2063 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 6098 "surface.c" /* yacc.c:1652  */
    break;

  case 643:
#line 2068 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0, 1); }
#line 6104 "surface.c" /* yacc.c:1652  */
    break;

  case 646:
#line 2079 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 1, 1); }
#line 6110 "surface.c" /* yacc.c:1652  */
    break;

  case 648:
#line 2081 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON | END_COMMAND, 0); }
#line 6116 "surface.c" /* yacc.c:1652  */
    break;

  case 650:
#line 2084 "surface.yy" /* yacc.c:1652  */
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(lexerBubble[0]);
			}
#line 6126 "surface.c" /* yacc.c:1652  */
    break;

  case 731:
#line 2135 "surface.yy" /* yacc.c:1652  */
    {}
#line 6132 "surface.c" /* yacc.c:1652  */
    break;

  case 733:
#line 2140 "surface.yy" /* yacc.c:1652  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			}
#line 6142 "surface.c" /* yacc.c:1652  */
    break;

  case 736:
#line 2151 "surface.yy" /* yacc.c:1652  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  interpreter.addSelected(singleton);
			}
#line 6151 "surface.c" /* yacc.c:1652  */
    break;

  case 737:
#line 2156 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.addSelected(lexerBubble);
			}
#line 6159 "surface.c" /* yacc.c:1652  */
    break;


#line 6163 "surface.c" /* yacc.c:1652  */
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
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;

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


/*-----------------------------------------------------.
| yyreturn -- parsing is finished, return the result.  |
`-----------------------------------------------------*/
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
#line 2190 "surface.yy" /* yacc.c:1918  */


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
