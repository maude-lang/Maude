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

#line 138 "surface.c" /* yacc.c:337  */
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
    KW_ENDM = 369,
    KW_IMPORT = 370,
    KW_ENDV = 371,
    KW_SORT = 372,
    KW_SUBSORT = 373,
    KW_OP = 374,
    KW_OPS = 375,
    KW_MSGS = 376,
    KW_VAR = 377,
    KW_CLASS = 378,
    KW_SUBCLASS = 379,
    KW_DSTRAT = 380,
    KW_MB = 381,
    KW_CMB = 382,
    KW_EQ = 383,
    KW_CEQ = 384,
    KW_RL = 385,
    KW_CRL = 386,
    KW_SD = 387,
    KW_CSD = 388,
    KW_IS = 389,
    KW_FROM = 390,
    KW_ARROW = 391,
    KW_ARROW2 = 392,
    KW_PARTIAL = 393,
    KW_IF = 394,
    KW_ASSIGN = 395,
    KW_LABEL = 396,
    KW_TO = 397,
    KW_COLON2 = 398,
    KW_ASSOC = 399,
    KW_COMM = 400,
    KW_ID = 401,
    KW_IDEM = 402,
    KW_ITER = 403,
    KW_LEFT = 404,
    KW_RIGHT = 405,
    KW_PREC = 406,
    KW_GATHER = 407,
    KW_METADATA = 408,
    KW_STRAT = 409,
    KW_ASTRAT = 410,
    KW_POLY = 411,
    KW_MEMO = 412,
    KW_FROZEN = 413,
    KW_CTOR = 414,
    KW_LATEX = 415,
    KW_SPECIAL = 416,
    KW_CONFIG = 417,
    KW_OBJ = 418,
    KW_MSG = 419,
    KW_DITTO = 420,
    KW_FORMAT = 421,
    KW_ID_HOOK = 422,
    KW_OP_HOOK = 423,
    KW_TERM_HOOK = 424,
    KW_IN = 425,
    IDENTIFIER = 426,
    NUMERIC_ID = 427,
    ENDS_IN_DOT = 428,
    FORCE_LOOKAHEAD = 429
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 98 "surface.yy" /* yacc.c:352  */

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

#line 370 "surface.c" /* yacc.c:352  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (UserLevelRewritingContext::ParseResult* parseResult);

#endif /* !YY_YY_SURFACE_H_INCLUDED  */

/* Second part of user prologue.  */
#line 114 "surface.yy" /* yacc.c:354  */

int yylex(YYSTYPE* lvalp);

#line 389 "surface.c" /* yacc.c:354  */

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
#define YYLAST   2675

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  190
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  273
/* YYNRULES -- Number of rules.  */
#define YYNRULES  675
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  1026

#define YYUNDEFTOK  2
#define YYMAXUTOK   429

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
     143,   144,   155,   154,   149,     2,   145,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   141,     2,
     146,   142,     2,     2,   189,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   147,     2,   148,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   156,   150,   157,     2,     2,     2,     2,
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
     135,   136,   137,   138,   139,   140,   151,   152,   153,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   248,   248,   250,   255,   256,   257,   258,   264,   264,
     274,   274,   284,   284,   294,   298,   298,   309,   309,   320,
     331,   331,   336,   336,   341,   346,   379,   383,   387,   388,
     389,   390,   394,   403,   404,   410,   411,   414,   415,   416,
     423,   430,   437,   446,   451,   457,   462,   474,   479,   473,
     485,   486,   489,   490,   493,   497,   501,   502,   501,   504,
     504,   511,   511,   513,   516,   517,   523,   523,   525,   528,
     529,   532,   533,   533,   535,   535,   537,   537,   544,   546,
     544,   564,   565,   568,   573,   576,   580,   581,   581,   583,
     584,   587,   588,   598,   606,   597,   615,   614,   646,   651,
     651,   665,   664,   673,   672,   700,   701,   710,   710,   717,
     719,   717,   732,   733,   742,   743,   746,   747,   750,   756,
     757,   765,   774,   775,   778,   778,   781,   782,   785,   790,
     790,   793,   793,   796,   797,   796,   800,   802,   804,   805,
     804,   808,   809,   810,   808,   813,   814,   813,   817,   818,
     819,   817,   822,   823,   822,   826,   827,   828,   826,   831,
     832,   831,   835,   836,   837,   835,   840,   840,   846,   847,
     846,   850,   856,   855,   862,   862,   865,   876,   877,   880,
     881,   884,   889,   890,   893,   894,   897,   898,   901,   902,
     903,   910,   910,   912,   913,   916,   919,   921,   927,   928,
     931,   937,   943,   946,   947,   952,   959,   966,   967,   970,
     971,   974,   975,   978,   983,   983,   990,   991,   994,   995,
     998,   999,  1002,  1006,  1010,  1016,  1020,  1024,  1024,  1026,
    1030,  1034,  1035,  1035,  1037,  1037,  1039,  1039,  1041,  1041,
    1043,  1043,  1045,  1049,  1053,  1058,  1058,  1060,  1064,  1068,
    1072,  1076,  1076,  1078,  1079,  1089,  1090,  1093,  1094,  1097,
    1098,  1101,  1102,  1103,  1104,  1110,  1112,  1118,  1120,  1128,
    1129,  1132,  1133,  1134,  1134,  1141,  1153,  1154,  1154,  1158,
    1158,  1160,  1166,  1166,  1166,  1166,  1166,  1169,  1169,  1169,
    1170,  1170,  1170,  1173,  1173,  1173,  1173,  1174,  1174,  1174,
    1174,  1174,  1174,  1177,  1177,  1177,  1178,  1178,  1178,  1178,
    1179,  1179,  1179,  1179,  1179,  1182,  1182,  1188,  1188,  1188,
    1188,  1188,  1188,  1188,  1188,  1189,  1189,  1192,  1192,  1192,
    1192,  1195,  1195,  1198,  1198,  1198,  1198,  1198,  1198,  1198,
    1199,  1199,  1199,  1200,  1200,  1200,  1200,  1200,  1200,  1200,
    1200,  1201,  1201,  1204,  1204,  1204,  1204,  1204,  1204,  1204,
    1207,  1207,  1207,  1210,  1210,  1210,  1210,  1210,  1210,  1210,
    1211,  1211,  1211,  1211,  1211,  1211,  1212,  1212,  1212,  1212,
    1213,  1213,  1213,  1213,  1214,  1214,  1214,  1241,  1241,  1246,
    1246,  1253,  1252,  1265,  1264,  1277,  1276,  1289,  1288,  1301,
    1300,  1313,  1312,  1326,  1325,  1339,  1338,  1351,  1350,  1364,
    1363,  1376,  1375,  1389,  1388,  1401,  1400,  1413,  1412,  1425,
    1424,  1437,  1436,  1450,  1449,  1461,  1466,  1465,  1482,  1481,
    1492,  1498,  1498,  1504,  1504,  1510,  1510,  1516,  1516,  1522,
    1522,  1531,  1531,  1537,  1537,  1543,  1543,  1549,  1549,  1555,
    1559,  1563,  1563,  1569,  1569,  1575,  1575,  1581,  1581,  1587,
    1587,  1593,  1593,  1599,  1599,  1605,  1605,  1611,  1611,  1617,
    1617,  1623,  1627,  1631,  1635,  1635,  1644,  1648,  1652,  1656,
    1660,  1664,  1668,  1672,  1676,  1680,  1684,  1688,  1692,  1696,
    1697,  1696,  1702,  1703,  1702,  1708,  1712,  1716,  1720,  1724,
    1728,  1735,  1739,  1743,  1747,  1754,  1758,  1765,  1765,  1772,
    1773,  1774,  1775,  1776,  1777,  1778,  1779,  1780,  1781,  1782,
    1785,  1786,  1787,  1788,  1789,  1790,  1791,  1792,  1793,  1794,
    1795,  1796,  1799,  1800,  1803,  1804,  1807,  1808,  1811,  1812,
    1817,  1818,  1819,  1820,  1821,  1822,  1825,  1826,  1829,  1830,
    1833,  1834,  1837,  1838,  1841,  1842,  1843,  1852,  1853,  1853,
    1857,  1857,  1859,  1869,  1869,  1871,  1872,  1872,  1877,  1877,
    1879,  1879,  1881,  1885,  1885,  1887,  1887,  1889,  1902,  1902,
    1904,  1905,  1905,  1914,  1914,  1916,  1916,  1918,  1918,  1920,
    1928,  1928,  1930,  1930,  1932,  1932,  1934,  1942,  1942,  1944,
    1944,  1946,  1955,  1954,  1961,  1961,  1963,  1971,  1971,  1973,
    1973,  1975,  1983,  1983,  1985,  1985,  1987,  1990,  1990,  1992,
    1996,  1996,  1998,  1998,  2000,  2011,  2011,  2011,  2011,  2011,
    2011,  2011,  2014,  2014,  2014,  2014,  2014,  2014,  2018,  2018,
    2018,  2018,  2018,  2018,  2022,  2022,  2022,  2022,  2022,  2022,
    2026,  2026,  2026,  2026,  2026,  2030,  2030,  2030,  2030,  2030,
    2036,  2037,  2040,  2041,  2044,  2051,  2052,  2055,  2060,  2067,
    2067,  2067,  2067,  2067,  2067,  2067
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
  "$@114", "$@115", "$@116", "printOption", "traceOption", "polarity",
  "select", "exclude", "conceal", "search", "match", "optDebug",
  "optIrredundant", "optNumber", "importMode", "moduleAndTerm", "$@117",
  "inEnd", "$@118", "numberModuleTerm", "$@119", "$@120",
  "numberModuleTerm1", "$@121", "$@122", "numberModuleTerm2", "$@123",
  "$@124", "numbersModuleTerm", "$@125", "$@126", "numbersModuleTerm1",
  "$@127", "$@128", "$@129", "numbersModuleTerm2", "$@130", "$@131",
  "$@132", "numbersModuleTerm3", "$@133", "$@134", "numbersModuleTerm4",
  "$@135", "$@136", "numbersModuleTerm5", "$@137", "$@138",
  "numbersModuleTerm6", "$@139", "$@140", "miscEndBubble", "$@141",
  "initialEndBubble", "$@142", "$@143", "cTokenBarIn", "cTokenBarLeftIn",
  "cTokenBarDotNumber", "cTokenBarDotRight", "cTokenBarDotCommaNumber",
  "cTokenBarDotCommaRight", "opSelect", "endSelect", "badSelect",
  "cOpNameList", "cSimpleOpName", "cSimpleTokenBarDot", YY_NULLPTR
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
     395,    58,    61,    40,    41,    46,    60,    91,    93,    44,
     124,   396,   397,   398,    43,    42,   123,   125,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415,   416,   417,   418,   419,   420,
     421,   422,   423,   424,   425,   426,   427,   428,   429,    64
};
# endif

#define YYPACT_NINF -891

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-891)))

#define YYTABLE_NINF -552

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     843,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
     465,   668,    70,  -891,   164,   259,    -9,  -891,  -891,   -68,
     -20,    28,    39,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,   252,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,   280,   199,
      87,  1246,   468,   468,   468,   192,   108,   528,   230,   251,
     290,   290,   290,   -36,  -891,  -891,  -891,   215,   290,  -891,
    -891,  -891,   241,   -56,  -891,   142,   165,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,   -93,  -891,  -891,  -891,  -891,
     147,  -891,  -891,  -891,  -891,   468,   468,   -93,   321,   329,
     331,   335,   338,   346,   468,   201,   353,  -891,  1246,   530,
    -891,  -891,  -891,  -891,   282,    48,  -891,  -891,  -891,  -891,
     267,  -891,   341,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,   342,   290,   290,   290,   290,   290,   290,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,   290,
    -891,  -891,  -891,   233,   375,   332,  -891,  -891,  -891,  -891,
    -891,   290,   290,  -891,  -891,   237,   239,   257,   290,   290,
     290,   290,   261,    75,   -93,   -93,   263,   269,   305,   -93,
    -891,  -891,   -93,   -93,   -93,   -93,   -93,   -93,   -93,   -93,
     -93,   -93,   -93,   -93,   264,   264,  -891,  -891,  -891,   264,
     264,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,   468,   530,   395,  -891,  -891,
     557,   557,   530,   530,  -891,   301,   400,   557,   530,   291,
     -93,   -87,   -93,    75,   290,   290,   307,   308,   311,   312,
     313,   317,   318,   290,   319,  -891,  -891,   402,   322,   324,
    -891,  -891,  -891,   325,   326,   328,   330,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
     334,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,   604,  -891,
    -891,  -891,  -891,  -891,   -93,   291,   627,   -93,  -891,  -891,
    -891,   530,   530,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  1246,   339,  -891,  -891,  -891,  -891,
    -891,  -891,   336,   337,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,   343,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
     290,  -891,  -891,  -891,  -891,  -891,  -891,   349,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,   530,  -891,  -891,   636,   -93,   530,    79,  -891,   -10,
     950,   -93,   290,  -891,  -891,  -891,   344,  -891,  -891,   -93,
     833,   -93,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  1246,  -891,  -891,  -891,   950,
     950,   -14,   345,   310,  -891,  -891,  -891,  -891,  -891,   347,
    -891,   515,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  2391,  2400,   -93,  -891,   323,   -81,
     950,   950,   333,  1246,  -891,   351,  -891,  1024,  -891,  -891,
    -891,  1320,  1320,  1542,  1246,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,   468,
     -93,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,    -7,  -891,  -891,
    -891,   355,   129,  -891,  -891,   360,   112,   222,   197,  -891,
     340,    21,   203,   217,   362,   367,   232,   265,   273,   277,
    -891,  2348,   373,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  1098,  -891,  -891,  -891,  -891,  -891,  1172,  1394,  -891,
    -891,  2348,   381,   391,   392,   396,   350,   408,   393,   397,
     405,   374,  -891,  -891,  2409,   -93,   468,  2418,   -93,  -891,
      34,  1246,  1246,  -891,  1024,  -891,  -891,  -891,  -891,  -891,
    -891,  1838,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  1764,  -891,   394,  -891,  -891,  -891,  -891,  1616,
    -891,  1616,   348,  -891,   399,  1764,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,   753,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,    20,
    -891,  2348,  -891,  1246,   374,  -891,   398,  -891,   136,   355,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  1616,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  1912,
    -891,  -891,  -891,  -891,  1912,  2060,   409,  -891,   -93,   416,
     -93,   418,   -93,   419,   -93,   427,  1616,  -891,  2204,  -891,
     422,   468,   -93,  2465,   -93,   425,  -891,  2348,  -891,  1542,
     374,  -891,   420,   432,   423,  -891,   430,   -71,  -891,  2276,
    2348,  -891,  2348,  -891,  -891,   429,  1690,  -891,  -891,   362,
    -891,  -891,   431,  -891,   438,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,   874,
    -891,   415,   -83,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,   434,     7,  1468,   435,  -891,   -60,  2348,  -891,   437,
    1246,   -90,  -891,  -891,  -891,   394,   152,   294,  -891,  2495,
     362,  -891,  -891,  -891,    19,  -891,  -891,  -891,  2060,  1246,
     -93,   -93,   -93,   -93,  2204,   295,   436,  -891,   468,   -93,
    1986,  -891,  -891,  -891,   348,  -891,  -891,   -90,  -891,  -891,
    -891,  -891,   439,  -891,  -891,  -891,  -891,  2348,  -891,  -891,
    -891,  -891,  -891,   433,   440,   407,   447,   414,   460,   462,
     467,  -891,   471,  -891,   475,   476,  -891,  -891,  -891,  -891,
     477,  2459,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,   362,
    -891,   -93,  1912,   -93,   478,   472,  2132,   479,   374,  2348,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,   253,  -891,  -891,  -891,   461,  -891,  2132,  -891,  -891,
    -891,  -891,   374,  2204,  -891,  -891,  -891,   394,   443,   443,
     443,   443,   443,   578,  1246,  1246,  1246,    -3,  -891,   443,
    -891,  -891,  2204,   480,  -891,    73,  -891,   -97,   -95,   -84,
     -63,   -62,   506,   508,   508,   508,  -891,  -891,   -51,   505,
    -891,   474,   518,   521,   522,   181,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,   -93,   443,   618,   443,  -891,
     -31,   523,   -29,  -891,  -891,  -891
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   507,   124,   125,    78,   391,   428,   547,   546,   409,
       0,     0,     0,   387,     0,     0,     0,   548,   550,     0,
       0,     0,     0,   393,   395,   389,    14,    15,    17,    19,
      20,    22,    10,    12,    24,    25,   426,   107,     8,     0,
       2,     6,     5,   430,     4,   109,     7,   413,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   520,     0,     0,
       0,     0,     0,     0,   554,   555,   556,     0,     0,   489,
     441,   447,     0,     0,   443,     0,     0,   445,   451,   453,
     455,   457,   459,   461,   463,   465,   467,   469,   474,   534,
     535,   537,   536,   431,   433,     0,   538,   539,   437,   435,
       0,   501,   502,   503,   504,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     1,     0,     0,
     397,   399,   540,   541,   551,     0,   401,   403,   405,   407,
     553,   542,     0,   543,   544,   545,   411,   415,   508,   349,
     333,   350,   334,   335,   336,   337,   340,   338,   341,   342,
     339,   343,   344,   345,   346,   347,   348,   351,   352,   326,
     325,   355,   358,   356,   359,   354,   357,   286,   353,   360,
     361,   329,   322,   324,   330,   323,   320,   321,   318,   319,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   379,
     372,   373,   374,   378,   375,   376,   377,   380,   381,   331,
     382,   383,   384,   385,   386,   317,   328,    79,   327,   282,
     283,   332,   284,   285,   362,   629,   620,   630,   627,   628,
     631,   622,   625,   626,   624,   392,   557,   558,   429,   410,
     492,     0,     0,     0,     0,     0,     0,     0,   529,   530,
     531,   522,   524,   521,   523,   525,   526,   527,   528,     0,
     513,   509,   510,     0,     0,     0,   514,   515,   516,   517,
     518,     0,     0,   532,   533,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     449,   450,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   105,   106,   388,     0,
       0,   439,   394,   396,   390,    16,    18,    21,    23,    11,
      13,   427,   108,     9,   110,   635,   636,   563,   634,   637,
     632,   633,   414,   565,   566,     0,     0,     0,   421,   417,
       0,     0,     0,     0,   552,     0,     0,     0,     0,   115,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   512,   511,     0,     0,     0,
     506,   488,   495,     0,     0,     0,     0,   499,   674,   671,
     672,   675,   673,   669,   670,   490,   442,   448,   473,   471,
       0,   444,   446,   452,   454,   456,   458,   460,   462,   464,
     466,   468,   470,   475,   664,   668,   432,   661,     0,   666,
     667,   434,   438,   436,     0,   115,     0,     0,   398,   400,
     423,     0,     0,   578,   402,   580,   581,   404,   406,   408,
     425,   419,   412,   416,     0,     0,   621,   560,   562,   623,
     559,   493,     0,     0,   483,   477,   479,   482,   480,   476,
     487,     0,   485,   519,   484,   505,   497,   500,   496,   498,
       0,   472,   662,   660,   663,   665,   440,   266,   642,   617,
     639,   640,   643,   641,   638,   568,   619,   564,   572,   570,
     567,     0,   422,   418,     0,     0,     0,     0,   117,     0,
       0,     0,     0,   478,   481,   486,     0,   265,   127,     0,
       0,     0,   424,   654,   651,   652,   585,   653,   650,   583,
     579,   589,   587,   582,   420,     0,   114,   120,   119,     0,
       0,     0,    33,    35,    36,    38,    37,    39,   561,     0,
     491,     0,   618,   648,   646,   573,   649,   647,   644,   645,
     569,   577,   575,   571,     0,     0,     0,   116,   118,     0,
       0,     0,     0,     0,   494,     0,   111,     0,   129,   131,
     133,     0,     0,     0,     0,   174,   192,   138,   141,   145,
     148,   152,   155,   159,   162,   191,   168,   126,   166,     0,
       0,   607,   586,   611,   609,   659,   657,   590,   592,   658,
     655,   656,   584,   596,   594,   588,    42,   266,    34,    47,
      40,    46,     0,    44,   176,   286,   288,   289,    32,   128,
       0,   268,   268,   268,   268,     0,   287,   290,   291,   292,
     270,     0,     0,   298,   301,   299,   302,   300,   296,   297,
     187,     0,   185,   186,   293,   294,   295,     0,     0,   183,
     172,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   574,   576,     0,     0,     0,     0,     0,    82,
       0,     0,     0,    41,     0,   267,    29,    30,    28,    26,
      27,     0,   312,   310,   311,   306,   307,   313,   314,   308,
     309,   303,     0,   273,   275,   276,   304,   305,   134,     0,
     184,     0,    84,   182,     0,     0,   139,   142,   146,   149,
     153,   156,   160,   163,   169,     0,   194,   195,   612,   608,
     616,   614,   610,   591,   597,   593,   601,   599,   595,     0,
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
     196,     0,     0,   613,   615,   602,   598,   606,   604,   600,
      90,     0,     0,     0,     0,    89,   101,     0,    61,     0,
       0,    65,    49,    52,   274,   281,     0,     0,   217,     0,
     268,   201,   204,   205,     0,   188,   211,   122,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   167,     0,     0,
       0,    93,    96,    88,    84,   103,    99,     0,    54,   212,
      57,    55,     0,    64,   279,   278,   215,     0,   225,   226,
     222,   229,   230,     0,     0,     0,     0,     0,     0,     0,
       0,   242,   244,   243,     0,     0,   247,   248,   249,   254,
       0,     0,   227,   221,   207,   203,   180,   181,   144,   151,
     158,   165,   197,   200,   198,   603,   605,    92,    85,   268,
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
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,    12,  -423,  -891,  -891,  -524,  -509,  -508,    16,    29,
    -891,  -891,  -891,  -891,  -891,   -96,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -313,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -180,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,   -92,     0,  -891,  -891,
    -891,  -891,  -635,   281,  -891,   180,  -891,  -733,  -235,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -160,   -80,   138,  -457,  -640,  -891,  -891,
      -4,  -756,  -891,  -891,  -891,  -891,  -152,  -891,  -871,  -755,
    -572,  -891,  -891,  -891,  -891,  -891,  -207,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -890,  -891,  -270,   114,
    -586,   -30,    72,  -891,  -342,  -761,  -891,  -891,  -891,   -44,
    -716,  -539,  -891,  -891,  -535,  -543,   -50,  -891,  -534,  -525,
    -341,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
     -28,   692,  -891,  -891,  -891,  -891,  -891,   584,  -891,  -891,
     -49,  -891,  -891,  -891,  -302,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -102,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,  -891,
    -891,  -891,  -891,  -891,  -891,  -891,  -429,  -891,   -75,  -891,
    -891,  -891,  -118,  -514,  -619,  -891,  -891,    65,  -891,   315,
    -891,   320,  -231
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    39,    40,    41,   116,   112,   113,   108,   109,   110,
     111,   599,   511,   512,   513,   514,   515,   516,   591,   592,
     590,   650,   776,   715,   716,   717,   773,   927,   831,   829,
     869,   872,   955,   975,   995,   996,  1016,  1018,  1017,    42,
      51,   339,   709,   744,   771,   822,   918,   863,   920,  1015,
     921,   824,   924,   825,   867,   923,   466,   395,   115,    44,
     118,   405,   724,   425,   477,   478,   509,   736,   788,    45,
     521,   567,   610,   611,   612,   731,   632,   748,   633,   749,
     850,   634,   750,   635,   751,   851,   636,   752,   637,   753,
     852,   638,   754,   639,   755,   853,   641,   640,   756,   684,
     631,   746,   792,   793,   628,   621,   622,   737,   568,   695,
     696,   760,   812,   856,   844,   845,   738,   784,   739,   786,
     740,   782,   837,   840,   901,   902,   903,   945,   958,   969,
     959,   960,   961,   962,   963,   971,   977,   967,   968,   488,
     656,   661,   672,   729,   741,   674,   780,   836,   929,   517,
     604,   623,   675,   605,   208,   209,   676,   211,   212,   213,
     214,    46,    95,   107,    52,   105,   106,   325,   326,   330,
     331,   332,   333,    54,   337,   119,   338,   412,   476,   411,
     471,   114,    53,   294,   295,   300,   299,   404,   274,   279,
     282,   275,   283,   284,   285,   286,   287,   288,   289,   290,
     291,   292,   293,   273,   450,   343,   482,    50,   261,   249,
     265,    93,    94,    98,   136,    47,    48,    49,   335,    69,
     225,   342,   429,   481,   322,   406,   407,   467,   490,   491,
     530,   569,   570,   414,   474,   475,   500,   535,   534,   536,
     582,   646,   647,   648,   705,   763,   764,   816,   858,   859,
     572,   644,   645,   699,   761,   762,   468,   489,   226,   340,
     341,   227,   324,   469,   532,   502,   584,   396,   453,   397,
     398,   399,   400
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,   210,   809,   298,   228,   229,   783,   207,   624,   624,
     624,   787,   606,   608,   629,   304,   657,   658,   659,   835,
     574,   765,   609,   601,   409,   701,   626,   626,   626,   794,
     418,   419,   268,   266,   267,   710,   423,   727,   602,   603,
     272,   742,   375,   277,   323,   501,   711,   997,   712,   999,
     747,   757,   296,   843,   427,   953,   302,   303,   296,   269,
    1000,   531,   722,   586,   328,   311,   329,   100,   210,   978,
     979,   980,   981,   541,   314,   873,   972,   101,   624,   988,
     713,  1001,  1002,   866,   624,   624,   538,   539,   998,   683,
     998,   781,   -98,  1007,   297,   278,   626,   714,   697,   758,
     297,   998,   626,   626,   723,   573,   583,    89,    90,   472,
     473,   925,   431,  1023,   926,  1025,   808,   587,   588,   606,
     608,   231,   998,   998,   600,   102,  1020,   487,  1022,   609,
     601,   507,   794,   707,   998,   270,   766,   767,   540,   768,
     541,   986,   769,   508,   818,   602,   603,   541,   861,    91,
      92,   711,   697,   712,   998,   732,   998,   733,   232,   862,
     233,   234,   235,   236,   680,   947,   655,   785,   957,   492,
     680,   775,   789,   103,   504,   -36,   -36,   857,   964,   965,
     966,   237,   376,   377,   104,   713,   810,   381,   770,   787,
     382,   383,   384,   385,   386,   387,   388,   389,   390,   391,
     392,   393,   714,   847,   346,   347,   348,   349,   350,   351,
     795,   797,   416,   416,   846,   700,   368,   137,   706,   416,
     798,   352,   369,   370,   371,   354,   624,   826,   505,   417,
     629,   721,   138,   358,   359,   422,   506,   846,   991,   992,
     363,   364,   365,   366,   626,    96,    97,   993,   426,   428,
     430,   323,   117,   994,   904,   415,   415,   323,   323,   372,
     373,   374,   415,   323,   263,   264,  -329,  -329,  -329,   673,
     677,   230,   250,   251,   252,   253,   408,   254,   652,   353,
     624,   262,   912,   276,   683,   652,   653,   280,   255,   673,
     677,   120,   121,   778,   122,   123,    89,    90,   626,   271,
     124,   874,   125,   126,   127,   128,   129,   130,   131,   875,
     281,   256,   456,   795,   797,   470,   432,   433,   301,   725,
     677,   263,   264,   798,   305,   441,   257,   258,   259,  1013,
     728,   677,   306,   946,   817,   307,   323,   323,   677,   308,
     677,   309,  -316,   728,   677,   312,   991,   992,   655,   310,
     789,  -328,  -328,  -328,   846,   993,   313,   -38,   -38,   -38,
     401,   994,   655,   336,   402,   403,   334,   132,    18,   772,
     677,   -37,   -37,   -37,   210,   846,  -330,  -330,  -330,   357,
     479,   974,   360,   503,   361,   956,  -327,  -327,  -327,   518,
     677,   133,   134,   135,   344,   345,   323,   522,   677,   533,
     989,   323,   362,   677,   380,   368,   367,    37,   378,   973,
     260,   369,   370,   371,   379,   677,   410,   677,   421,  -283,
    -283,  -283,   486,   355,   356,   821,   677,  -284,  -284,  -284,
     210,  -285,  -285,  -285,   964,   965,   966,   725,   677,   677,
     838,   677,   876,   877,   585,   677,   420,   424,   372,   373,
     374,   394,   434,   435,   519,   210,   436,   437,   438,   210,
     210,   479,   439,   440,   442,   443,   543,   444,   677,   445,
     446,   447,    55,   448,   480,   449,   589,   541,   643,   451,
     913,   483,   484,   487,   743,   868,   677,   690,   485,   520,
     210,   210,   544,   210,   654,    56,   594,   607,    57,   593,
     542,   625,   625,   625,   210,  -315,    58,   655,   159,   160,
     630,   651,   660,   677,   678,    59,   545,    60,   919,   677,
     642,    61,   686,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   687,   692,   688,   930,   677,   693,   689,    62,
     238,    63,    64,    65,    66,   691,   694,   777,    67,   745,
     730,   620,   620,   702,   799,   801,   708,   803,   805,   205,
      68,   206,   239,   240,   241,   242,   807,   243,   244,   811,
     820,   625,   827,   828,   832,   830,   839,   625,   625,   849,
     848,   677,   855,   982,   914,   677,   860,   865,   677,   870,
     934,   928,   933,   931,   245,   246,   247,   703,   248,   935,
     932,   210,   210,   936,   607,   937,   677,   593,   593,   215,
     938,   216,   677,   217,   939,   218,   219,   220,   940,   941,
     942,   620,   951,  1021,   952,   990,   954,   620,   976,   546,
     547,   677,   548,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   562,   563,   564,   970,
    1003,    37,   221,   222,   223,   224,   800,  1008,   802,  1009,
     804,  1010,   806,   210,  1011,  1012,   720,  1024,   719,   774,
     814,   315,   819,   216,    70,   316,    71,   317,   318,   319,
     718,   833,  1014,   565,   922,   537,   457,   949,   906,   823,
     627,   759,   905,   566,   944,   796,    72,   987,   315,   779,
     216,   649,   316,   685,   413,   318,   319,    99,   327,     0,
       0,     0,   813,   454,   221,   320,   321,   224,   455,   625,
       0,     0,     0,     0,     0,    73,     0,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    86,
      87,   221,   320,   321,   224,   368,     0,    37,     0,   452,
       0,   369,   370,   371,     0,     0,     0,     0,   908,   909,
     910,   911,     0,    88,     0,     0,     0,   916,   458,     0,
     459,     0,   296,   625,   460,   461,   462,   493,     0,   459,
     210,   296,     0,   494,   495,   496,   871,     0,   372,   373,
     374,   394,     0,     0,     0,     0,     0,     0,   796,   210,
       0,     0,     0,     0,     0,   907,     0,     0,     0,   915,
       0,   463,   464,   465,   297,     0,     0,     0,     0,     0,
     497,   498,   499,   297,     0,     0,     0,     0,     0,   948,
       0,   950,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    -3,     1,     0,     0,     0,     0,     2,
       3,     4,     5,     0,  -549,  -549,     6,  -549,  -549,     7,
       8,  -551,     9,  -549,     0,  -549,  -549,  -549,  -549,  -549,
    -549,  -549,    10,    11,     0,     0,    12,     0,     0,     0,
      13,     0,     0,     0,    14,     0,     0,   159,   160,     0,
       0,     0,     0,     0,   757,     0,     0,     0,     0,    15,
       0,     0,   171,   172,   173,   174,   175,   176,   177,   178,
     179,     0,     0,     0,   210,   210,   210,     0,    16,     0,
     983,   984,   985,  1019,     0,     0,     0,     0,    17,    18,
    -549,    19,    20,    21,    22,    23,    24,    25,   205,     0,
     206,     0,   758,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,  -549,  -549,  -549,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   523,     0,   459,     0,   296,     0,
     524,   525,   526,  1004,  1005,  1006,    37,     0,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   662,     0,
       0,   663,     0,   664,     0,     0,   665,   527,   528,   529,
     297,   734,     0,     0,   666,   667,   668,    38,   669,   670,
       0,     0,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   671,
       0,     0,     0,   854,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
       0,   165,   166,   510,     0,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   205,     0,   206,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,     0,   165,   166,   510,     0,   595,
     168,   169,   170,   596,   172,   173,   597,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   205,
       0,   598,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   613,   614,   615,   616,     0,   679,
     617,    37,     0,   618,   619,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     0,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   205,     0,   206,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   613,   614,
     615,   616,     0,   681,   617,    37,     0,   618,   619,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,     0,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   205,     0,   206,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,     0,   165,   166,     0,
       0,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,     0,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   205,     0,   206,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   613,   614,   615,   616,
       0,     0,   617,    37,     0,   618,   619,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   205,     0,   206,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     613,   614,   615,   616,     0,   682,   617,     0,     0,   618,
     619,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   205,
       0,   206,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   613,   614,   615,   616,     0,   864,
     617,     0,     0,   618,   619,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     0,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   205,     0,   206,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   613,   614,
     615,   616,     0,     0,   617,     0,     0,   618,   619,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,     0,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   205,     0,   206,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     662,     0,   732,   663,   733,   664,     0,     0,   665,     0,
       0,     0,     0,   734,     0,     0,   666,   667,   668,     0,
     669,   670,     0,     0,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,     0,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   671,     0,   735,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   662,     0,     0,   663,     0,   664,
       0,   841,   665,     0,     0,   842,     0,   734,     0,     0,
     666,   667,   668,     0,   669,   670,     0,     0,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   671,     0,   735,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   662,     0,
       0,   663,     0,   664,     0,     0,   665,     0,     0,   722,
     726,     0,     0,     0,   666,   667,   668,     0,   669,   670,
       0,     0,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   671,
       0,   723,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   662,     0,     0,   663,     0,   664,     0,     0,
     665,     0,     0,   722,     0,     0,     0,     0,   666,   667,
     668,     0,   669,   670,     0,     0,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     0,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,     0,   671,     0,   723,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   662,     0,     0,   663,
       0,   664,     0,     0,   665,     0,     0,     0,     0,   734,
       0,     0,   666,   667,   668,     0,   669,   670,     0,     0,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,     0,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   671,     0,   735,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     662,     0,     0,   663,     0,   664,     0,     0,   665,     0,
       0,     0,     0,     0,     0,     0,   666,   667,   668,     0,
     669,   670,     0,     0,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,     0,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   671,     0,   917,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
       0,   165,   166,     0,     0,     0,   168,   169,   170,   790,
     172,   173,   791,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,     0,
     191,   192,   193,   194,   195,   196,   197,   198,   199,   200,
     201,   202,   203,   204,     0,   205,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   662,     0,   732,   663,
     733,   664,     0,     0,   665,     0,     0,     0,     0,   734,
       0,     0,   666,   667,   668,     0,   669,   670,     0,     0,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,     0,   191,   192,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,   671,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   662,     0,
       0,   663,     0,   664,     0,     0,   665,     0,     0,     0,
       0,   734,     0,     0,   666,   667,   668,     0,   669,   670,
       0,     0,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,     0,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,     0,   671,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     662,     0,     0,   663,     0,   664,     0,     0,   665,     0,
       0,     0,   834,     0,     0,     0,   666,   667,   668,     0,
     669,   670,     0,     0,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,     0,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
       0,   671,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   662,     0,     0,   663,     0,   664,     0,     0,
     665,     0,     0,     0,     0,     0,     0,     0,   666,   667,
     668,     0,   669,   670,     0,     0,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,     0,   191,   192,
     193,   194,   195,   196,   197,   198,   199,   200,   201,   202,
     203,   204,   458,   671,   459,     0,   296,     0,   460,   461,
     462,   575,     0,   459,     0,   296,     0,   576,   577,   578,
     523,     0,   459,     0,   296,     0,   524,   698,   526,   458,
       0,   459,     0,   296,     0,   460,   461,   462,     0,     0,
       0,     0,     0,     0,     0,   463,   464,   571,   297,     0,
       0,     0,     0,     0,   579,   580,   581,   297,     0,     0,
       0,     0,     0,   527,   528,   529,   297,     0,     0,     0,
       0,     0,   463,   464,   704,   297,   523,   943,   459,     0,
     296,     0,   524,   815,   526,     0,     0,   878,   879,   880,
     881,   882,   883,   884,   885,   886,   887,   888,   889,   890,
     891,   892,   893,   894,   895,   896,   897,   898,   899,   900,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   527,
     528,   529,   297,   878,   879,   880,   881,   882,   883,   884,
     885,   886,   887,   888,   889,   890,   891,   892,   893,   894,
     895,   896,   897,   898,   899,   900
};

static const yytype_int16 yycheck[] =
{
       0,    51,   757,    95,    53,    54,   739,    51,   551,   552,
     553,   744,   547,   547,   553,   107,   602,   603,   604,   780,
     534,     1,   547,   547,   326,   644,   551,   552,   553,   745,
     332,   333,    68,    61,    62,     1,   338,   672,   547,   547,
      68,   681,   273,    99,   119,   474,   117,   144,   119,   144,
     685,   141,   145,   786,   141,   926,   105,   106,   145,    95,
     144,   490,   145,   144,    16,   114,    18,    76,   118,   959,
     960,   961,   962,   154,   118,   831,   947,   145,   621,   969,
     151,   144,   144,   143,   627,   628,   509,   510,   185,   628,
     185,   731,   152,   144,   187,   151,   621,   168,   641,   189,
     187,   185,   627,   628,   187,   534,   535,    37,    38,   411,
     412,   867,   343,   144,   869,   144,   756,   540,   541,   654,
     654,    13,   185,   185,   547,   145,  1016,   134,  1018,   654,
     654,   141,   848,   647,   185,   171,   116,   117,   152,   119,
     154,   144,   122,   153,   763,   654,   654,   154,   141,    79,
      80,   117,   695,   119,   185,   136,   185,   138,    50,   152,
      52,    53,    54,    55,   621,   920,   145,   739,   929,   471,
     627,   714,   744,   145,   476,   154,   155,   812,   181,   182,
     183,    73,   274,   275,   145,   151,   758,   279,   168,   922,
     282,   283,   284,   285,   286,   287,   288,   289,   290,   291,
     292,   293,   168,   789,   232,   233,   234,   235,   236,   237,
     745,   745,   330,   331,   786,   644,   141,    18,   647,   337,
     745,   249,   147,   148,   149,   253,   769,   770,   149,   331,
     769,   654,   145,   261,   262,   337,   157,   809,   165,   166,
     268,   269,   270,   271,   769,    81,    82,   174,   340,   341,
     342,   326,     0,   180,   840,   330,   331,   332,   333,   184,
     185,   186,   337,   338,    31,    32,   154,   155,   156,   611,
     611,    79,    42,    43,    44,    45,   325,    47,   149,    46,
     823,    30,   854,    42,   823,   149,   157,   145,    58,   631,
     631,    11,    12,   157,    14,    15,    37,    38,   823,    84,
      20,   149,    22,    23,    24,    25,    26,    27,    28,   157,
     145,    81,   404,   848,   848,   407,   344,   345,   171,   661,
     661,    31,    32,   848,     3,   353,    96,    97,    98,   148,
     672,   672,     3,   919,   763,     4,   411,   412,   679,     4,
     681,     3,   145,   685,   685,   144,   165,   166,   145,     3,
     922,   154,   155,   156,   926,   174,     3,   154,   155,   156,
     295,   180,   145,    22,   299,   300,    99,    87,    86,   711,
     711,   154,   155,   156,   424,   947,   154,   155,   156,    47,
     424,   953,   145,   475,   145,   928,   154,   155,   156,   481,
     731,   111,   112,   113,    52,    53,   471,   489,   739,   491,
     972,   476,   145,   744,    99,   141,   145,   143,   145,   952,
     180,   147,   148,   149,   145,   756,    21,   758,    18,   154,
     155,   156,   450,    48,    49,   767,   767,   154,   155,   156,
     480,   154,   155,   156,   181,   182,   183,   779,   779,   780,
     782,   782,   148,   149,   536,   786,   145,   156,   184,   185,
     186,   187,   145,   145,   482,   505,   145,   145,   145,   509,
     510,   505,   145,   145,   145,    63,   156,   145,   809,   145,
     145,   145,     7,   145,   135,   145,   143,   154,   570,   145,
     185,   145,   145,   134,   136,   827,   827,   137,   145,   145,
     540,   541,   145,   543,   154,    30,   145,   547,    33,   543,
     155,   551,   552,   553,   554,   145,    41,   145,   134,   135,
     554,   156,   145,   854,   141,    50,     1,    52,   860,   860,
     569,    56,   141,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   141,   140,   142,   877,   877,   140,   142,    74,
      12,    76,    77,    78,    79,   137,   141,   149,    83,   150,
     156,   551,   552,   645,   145,   139,   648,   139,   139,   185,
      95,   187,    34,    35,    36,    37,   139,    39,    40,   147,
     145,   621,   152,   141,   144,   152,   147,   627,   628,   141,
     149,   922,   167,     5,   148,   926,   152,   152,   929,   152,
     143,   152,   185,   160,    66,    67,    68,   646,    70,   185,
     160,   651,   652,   143,   654,   143,   947,   651,   652,   141,
     143,   143,   953,   145,   143,   147,   148,   149,   143,   143,
     143,   621,   144,     5,   152,   145,   147,   627,   185,   114,
     115,   972,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   188,
     144,   143,   184,   185,   186,   187,   748,   152,   750,   185,
     752,   143,   754,   713,   143,   143,   654,   144,   652,   713,
     762,   141,   764,   143,     6,   145,     8,   147,   148,   149,
     651,   777,   995,   168,   864,   505,   405,   922,   848,   769,
     552,   695,   844,   178,   901,   745,    28,   967,   141,   729,
     143,   587,   145,   631,   147,   148,   149,    15,   124,    -1,
      -1,    -1,   761,   398,   184,   185,   186,   187,   398,   769,
      -1,    -1,    -1,    -1,    -1,    57,    -1,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    70,    71,
      72,   184,   185,   186,   187,   141,    -1,   143,    -1,   145,
      -1,   147,   148,   149,    -1,    -1,    -1,    -1,   850,   851,
     852,   853,    -1,    95,    -1,    -1,    -1,   859,   141,    -1,
     143,    -1,   145,   823,   147,   148,   149,   141,    -1,   143,
     830,   145,    -1,   147,   148,   149,   830,    -1,   184,   185,
     186,   187,    -1,    -1,    -1,    -1,    -1,    -1,   848,   849,
      -1,    -1,    -1,    -1,    -1,   849,    -1,    -1,    -1,   858,
      -1,   184,   185,   186,   187,    -1,    -1,    -1,    -1,    -1,
     184,   185,   186,   187,    -1,    -1,    -1,    -1,    -1,   921,
      -1,   923,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,     0,     1,    -1,    -1,    -1,    -1,     6,
       7,     8,     9,    -1,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    -1,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    -1,    -1,    33,    -1,    -1,    -1,
      37,    -1,    -1,    -1,    41,    -1,    -1,   134,   135,    -1,
      -1,    -1,    -1,    -1,   141,    -1,    -1,    -1,    -1,    56,
      -1,    -1,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    -1,    -1,    -1,   964,   965,   966,    -1,    75,    -1,
     964,   965,   966,  1015,    -1,    -1,    -1,    -1,    85,    86,
      87,    88,    89,    90,    91,    92,    93,    94,   185,    -1,
     187,    -1,   189,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   141,    -1,   143,    -1,   145,    -1,
     147,   148,   149,   983,   984,   985,   143,    -1,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,   137,    -1,   139,    -1,    -1,   142,   184,   185,   186,
     187,   147,    -1,    -1,   150,   151,   152,   184,   154,   155,
      -1,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      -1,    -1,    -1,   189,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,   141,   142,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    -1,   187,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,   141,   142,   143,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      -1,   187,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,   141,
     142,   143,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    -1,   187,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,   141,   142,   143,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    -1,   187,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,    -1,   141,   142,    -1,
      -1,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    -1,   187,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,    -1,   142,   143,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    -1,   187,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,    -1,   141,   142,    -1,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      -1,   187,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,    -1,   141,
     142,    -1,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    -1,   187,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    -1,   187,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,   136,   137,   138,   139,    -1,    -1,   142,    -1,
      -1,    -1,    -1,   147,    -1,    -1,   150,   151,   152,    -1,
     154,   155,    -1,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    -1,   187,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,    -1,    -1,   137,    -1,   139,
      -1,   141,   142,    -1,    -1,   145,    -1,   147,    -1,    -1,
     150,   151,   152,    -1,   154,   155,    -1,    -1,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,    -1,   187,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,   137,    -1,   139,    -1,    -1,   142,    -1,    -1,   145,
     146,    -1,    -1,    -1,   150,   151,   152,    -1,   154,   155,
      -1,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
      -1,   187,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,   137,    -1,   139,    -1,    -1,
     142,    -1,    -1,   145,    -1,    -1,    -1,    -1,   150,   151,
     152,    -1,   154,   155,    -1,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,    -1,   185,    -1,   187,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,    -1,   137,
      -1,   139,    -1,    -1,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,   150,   151,   152,    -1,   154,   155,    -1,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,    -1,   187,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,   137,    -1,   139,    -1,    -1,   142,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   150,   151,   152,    -1,
     154,   155,    -1,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,    -1,   187,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
      -1,   141,   142,    -1,    -1,    -1,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,    -1,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,    -1,   185,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,    -1,   136,   137,
     138,   139,    -1,    -1,   142,    -1,    -1,    -1,    -1,   147,
      -1,    -1,   150,   151,   152,    -1,   154,   155,    -1,    -1,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,    -1,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,    -1,   185,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,    -1,
      -1,   137,    -1,   139,    -1,    -1,   142,    -1,    -1,    -1,
      -1,   147,    -1,    -1,   150,   151,   152,    -1,   154,   155,
      -1,    -1,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,    -1,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,    -1,   185,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,    -1,    -1,   137,    -1,   139,    -1,    -1,   142,    -1,
      -1,    -1,   146,    -1,    -1,    -1,   150,   151,   152,    -1,
     154,   155,    -1,    -1,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,    -1,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
      -1,   185,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,    -1,    -1,   137,    -1,   139,    -1,    -1,
     142,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   150,   151,
     152,    -1,   154,   155,    -1,    -1,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,    -1,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   141,   185,   143,    -1,   145,    -1,   147,   148,
     149,   141,    -1,   143,    -1,   145,    -1,   147,   148,   149,
     141,    -1,   143,    -1,   145,    -1,   147,   148,   149,   141,
      -1,   143,    -1,   145,    -1,   147,   148,   149,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   184,   185,   186,   187,    -1,
      -1,    -1,    -1,    -1,   184,   185,   186,   187,    -1,    -1,
      -1,    -1,    -1,   184,   185,   186,   187,    -1,    -1,    -1,
      -1,    -1,   184,   185,   186,   187,   141,   148,   143,    -1,
     145,    -1,   147,   148,   149,    -1,    -1,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   184,
     185,   186,   187,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    16,    17,    19,
      29,    30,    33,    37,    41,    56,    75,    85,    86,    88,
      89,    90,    91,    92,    93,    94,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   143,   184,   191,
     192,   193,   229,   247,   249,   259,   351,   405,   406,   407,
     397,   230,   354,   372,   363,     7,    30,    33,    41,    50,
      52,    56,    74,    76,    77,    78,    79,    83,    95,   409,
       6,     8,    28,    57,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    95,    37,
      38,    79,    80,   401,   402,   352,    81,    82,   403,   401,
      76,   145,   145,   145,   145,   355,   356,   353,   197,   198,
     199,   200,   195,   196,   371,   248,   194,     0,   250,   365,
      11,    12,    14,    15,    20,    22,    23,    24,    25,    26,
      27,    28,    87,   111,   112,   113,   404,    18,   145,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   141,   142,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   185,   187,   339,   344,   345,
     346,   347,   348,   349,   350,   141,   143,   145,   147,   148,
     149,   184,   185,   186,   187,   410,   448,   451,   410,   410,
      79,    13,    50,    52,    53,    54,    55,    73,    12,    34,
      35,    36,    37,    39,    40,    66,    67,    68,    70,   399,
      42,    43,    44,    45,    47,    58,    81,    96,    97,    98,
     180,   398,    30,    31,    32,   400,   400,   400,    68,    95,
     171,    84,   400,   393,   378,   381,    42,    99,   151,   379,
     145,   145,   380,   382,   383,   384,   385,   386,   387,   388,
     389,   390,   391,   392,   373,   374,   145,   187,   246,   376,
     375,   171,   410,   410,   246,     3,     3,     4,     4,     3,
       3,   410,   144,     3,   339,   141,   145,   147,   148,   149,
     185,   186,   414,   448,   452,   357,   358,   407,    16,    18,
     359,   360,   361,   362,    99,   408,    22,   364,   366,   231,
     449,   450,   411,   395,    52,    53,   400,   400,   400,   400,
     400,   400,   400,    46,   400,    48,    49,    47,   400,   400,
     145,   145,   145,   400,   400,   400,   400,   145,   141,   147,
     148,   149,   184,   185,   186,   462,   246,   246,   145,   145,
      99,   246,   246,   246,   246,   246,   246,   246,   246,   246,
     246,   246,   246,   246,   187,   247,   457,   459,   460,   461,
     462,   457,   457,   457,   377,   251,   415,   416,   410,   414,
      21,   369,   367,   147,   423,   448,   452,   423,   414,   414,
     145,    18,   423,   414,   156,   253,   246,   141,   246,   412,
     246,   462,   400,   400,   145,   145,   145,   145,   145,   145,
     145,   400,   145,    63,   145,   145,   145,   145,   145,   145,
     394,   145,   145,   458,   459,   461,   246,   253,   141,   143,
     147,   148,   149,   184,   185,   186,   246,   417,   446,   453,
     246,   370,   414,   414,   424,   425,   368,   254,   255,   339,
     135,   413,   396,   145,   145,   145,   400,   134,   329,   447,
     418,   419,   414,   141,   147,   148,   149,   184,   185,   186,
     426,   446,   455,   246,   414,   149,   157,   141,   153,   256,
     143,   202,   203,   204,   205,   206,   207,   339,   246,   400,
     145,   260,   246,   141,   147,   148,   149,   184,   185,   186,
     420,   446,   454,   246,   428,   427,   429,   255,   202,   202,
     152,   154,   155,   156,   145,     1,   114,   115,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   168,   178,   261,   298,   421,
     422,   186,   440,   446,   453,   141,   147,   148,   149,   184,
     185,   186,   430,   446,   456,   246,   144,   202,   202,   143,
     210,   208,   209,   339,   145,   145,   149,   152,   187,   201,
     202,   205,   206,   207,   340,   343,   344,   346,   348,   349,
     262,   263,   264,   136,   137,   138,   139,   142,   145,   146,
     247,   295,   296,   341,   345,   346,   349,   295,   294,   341,
     339,   290,   266,   268,   271,   273,   276,   278,   281,   283,
     287,   286,   410,   246,   441,   442,   431,   432,   433,   329,
     211,   156,   149,   157,   154,   145,   330,   330,   330,   330,
     145,   331,   134,   137,   139,   142,   150,   151,   152,   154,
     155,   185,   332,   334,   335,   342,   346,   350,   141,   141,
     296,   141,   141,   341,   289,   332,   141,   141,   142,   142,
     137,   137,   140,   140,   141,   299,   300,   345,   148,   443,
     446,   454,   246,   410,   186,   434,   446,   453,   246,   232,
       1,   117,   119,   151,   168,   213,   214,   215,   209,   208,
     201,   202,   145,   187,   252,   334,   146,   252,   334,   333,
     156,   265,   136,   138,   147,   187,   257,   297,   306,   308,
     310,   334,   297,   136,   233,   150,   291,   252,   267,   269,
     272,   274,   277,   279,   282,   284,   288,   141,   189,   300,
     301,   444,   445,   435,   436,     1,   116,   117,   119,   122,
     168,   234,   334,   216,   339,   345,   212,   149,   157,   331,
     336,   297,   311,   257,   307,   310,   309,   257,   258,   310,
     149,   152,   292,   293,   340,   344,   346,   348,   349,   145,
     246,   139,   246,   139,   246,   139,   246,   139,   297,   309,
     310,   147,   302,   410,   246,   148,   437,   446,   454,   246,
     145,   334,   235,   294,   241,   243,   345,   152,   141,   219,
     152,   218,   144,   215,   146,   335,   337,   312,   334,   147,
     313,   141,   145,   257,   304,   305,   310,   330,   149,   141,
     270,   275,   280,   285,   189,   167,   303,   252,   438,   439,
     152,   141,   152,   237,   141,   152,   143,   244,   334,   220,
     152,   339,   221,   301,   149,   157,   148,   149,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   314,   315,   316,   330,   306,   293,   339,   246,   246,
     246,   246,   310,   185,   148,   410,   246,   187,   236,   334,
     238,   240,   233,   245,   242,   301,   309,   217,   152,   338,
     334,   160,   160,   185,   143,   185,   143,   143,   143,   143,
     143,   143,   143,   148,   316,   317,   330,   309,   246,   258,
     246,   144,   152,   308,   147,   222,   345,   335,   318,   320,
     321,   322,   323,   324,   181,   182,   183,   327,   328,   319,
     188,   325,   308,   345,   310,   223,   185,   326,   326,   326,
     326,   326,     5,   339,   339,   339,   144,   328,   326,   310,
     145,   165,   166,   174,   180,   224,   225,   144,   185,   144,
     144,   144,   144,   144,   247,   247,   247,   144,   152,   185,
     143,   143,   143,   148,   225,   239,   226,   228,   227,   246,
     326,     5,   326,   144,   144,   144
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   190,   191,   191,   192,   192,   192,   192,   194,   193,
     195,   193,   196,   193,   193,   197,   193,   198,   193,   193,
     199,   193,   200,   193,   193,   193,   201,   201,   201,   201,
     201,   201,   201,   202,   202,   203,   203,   204,   204,   204,
     205,   206,   207,   208,   208,   209,   209,   211,   212,   210,
     213,   213,   214,   214,   215,   215,   216,   217,   215,   218,
     215,   220,   219,   219,   221,   221,   223,   222,   222,   224,
     224,   225,   226,   225,   227,   225,   228,   225,   230,   231,
     229,   232,   232,   233,   233,   234,   234,   235,   234,   234,
     234,   236,   236,   238,   239,   237,   240,   237,   241,   242,
     241,   244,   243,   245,   243,   246,   246,   248,   247,   250,
     251,   249,   252,   252,   253,   253,   254,   254,   255,   256,
     256,   257,   258,   258,   259,   259,   260,   260,   261,   262,
     261,   263,   261,   264,   265,   261,   261,   261,   266,   267,
     261,   268,   269,   270,   261,   271,   272,   261,   273,   274,
     275,   261,   276,   277,   261,   278,   279,   280,   261,   281,
     282,   261,   283,   284,   285,   261,   286,   261,   287,   288,
     261,   261,   289,   261,   290,   261,   261,   291,   291,   292,
     292,   293,   294,   294,   295,   295,   296,   296,   297,   297,
     297,   298,   298,   299,   299,   300,   301,   301,   302,   302,
     303,   304,   304,   305,   305,   305,   306,   307,   307,   308,
     308,   309,   309,   310,   311,   310,   312,   312,   313,   313,
     314,   314,   315,   315,   315,   316,   316,   317,   316,   316,
     316,   316,   318,   316,   319,   316,   320,   316,   321,   316,
     322,   316,   316,   316,   316,   323,   316,   316,   316,   316,
     316,   324,   316,   316,   316,   325,   325,   326,   326,   327,
     327,   328,   328,   328,   328,   329,   329,   330,   330,   331,
     331,   332,   332,   333,   332,   334,   335,   336,   335,   338,
     337,   337,   339,   339,   339,   339,   339,   340,   340,   340,
     340,   340,   340,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   342,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   343,   343,   344,   344,   344,
     344,   344,   344,   344,   344,   344,   344,   345,   345,   345,
     345,   346,   346,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   347,   347,   347,   347,   347,   347,   347,
     347,   347,   347,   348,   348,   348,   348,   348,   348,   348,
     349,   349,   349,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   350,   350,   350,
     350,   350,   350,   350,   350,   350,   350,   352,   351,   353,
     351,   354,   351,   355,   351,   356,   351,   357,   351,   358,
     351,   359,   351,   360,   351,   361,   351,   362,   351,   363,
     351,   364,   351,   365,   351,   366,   351,   367,   351,   368,
     351,   369,   351,   370,   351,   351,   371,   351,   372,   351,
     351,   373,   351,   374,   351,   375,   351,   376,   351,   377,
     351,   378,   351,   379,   351,   380,   351,   381,   351,   351,
     351,   382,   351,   383,   351,   384,   351,   385,   351,   386,
     351,   387,   351,   388,   351,   389,   351,   390,   351,   391,
     351,   351,   351,   351,   392,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   351,   351,   393,
     394,   351,   395,   396,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   351,   351,   351,   397,   351,   398,
     398,   398,   398,   398,   398,   398,   398,   398,   398,   398,
     399,   399,   399,   399,   399,   399,   399,   399,   399,   399,
     399,   399,   400,   400,   401,   401,   402,   402,   403,   403,
     404,   404,   404,   404,   404,   404,   405,   405,   406,   406,
     407,   407,   408,   408,   409,   409,   409,   410,   411,   410,
     413,   412,   412,   415,   414,   414,   416,   414,   418,   417,
     419,   417,   417,   421,   420,   422,   420,   420,   424,   423,
     423,   425,   423,   427,   426,   428,   426,   429,   426,   426,
     431,   430,   432,   430,   433,   430,   430,   435,   434,   436,
     434,   434,   438,   437,   439,   437,   437,   441,   440,   442,
     440,   440,   444,   443,   445,   443,   443,   447,   446,   446,
     449,   448,   450,   448,   448,   451,   451,   451,   451,   451,
     451,   451,   452,   452,   452,   452,   452,   452,   453,   453,
     453,   453,   453,   453,   454,   454,   454,   454,   454,   454,
     455,   455,   455,   455,   455,   456,   456,   456,   456,   456,
     457,   457,   458,   458,   459,   460,   460,   461,   461,   462,
     462,   462,   462,   462,   462,   462
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
       3,     0,     4,     0,     3,     0,     4,     0,     5,     0,
       6,     0,     5,     0,     6,     4,     0,     3,     0,     3,
       1,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       5,     0,     4,     0,     4,     0,     4,     0,     4,     3,
       3,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     4,     5,     4,     0,     4,     5,     5,     6,     5,
       5,     6,     5,     5,     5,     5,     6,     5,     4,     0,
       0,     7,     0,     0,     8,     4,     5,     5,     5,     4,
       5,     2,     2,     2,     2,     5,     4,     0,     3,     1,
       1,     2,     2,     1,     1,     1,     1,     1,     1,     3,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     0,     1,     0,     1,     1,     1,     1,     0,     3,
       0,     3,     1,     0,     3,     1,     0,     3,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       1,     0,     3,     0,     3,     0,     3,     0,     3,     1,
       0,     3,     0,     3,     0,     3,     1,     0,     3,     0,
       3,     1,     0,     3,     0,     3,     1,     0,     3,     0,
       3,     1,     0,     3,     0,     3,     1,     0,     3,     1,
       0,     3,     0,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     1,     1,     1,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
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
    case 201: /* moduleExprDot  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2406 "surface.c" /* yacc.c:1257  */
        break;

    case 202: /* moduleExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2412 "surface.c" /* yacc.c:1257  */
        break;

    case 203: /* moduleExpr2  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2418 "surface.c" /* yacc.c:1257  */
        break;

    case 204: /* moduleExpr3  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2424 "surface.c" /* yacc.c:1257  */
        break;

    case 205: /* renameExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2430 "surface.c" /* yacc.c:1257  */
        break;

    case 206: /* instantExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2436 "surface.c" /* yacc.c:1257  */
        break;

    case 207: /* parenExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyModuleExpression)->deepSelfDestruct(); }
#line 2442 "surface.c" /* yacc.c:1257  */
        break;

    case 208: /* viewExpr  */
#line 112 "surface.yy" /* yacc.c:1257  */
      { ((*yyvaluep).yyViewExpression)->deepSelfDestruct(); }
#line 2448 "surface.c" /* yacc.c:1257  */
        break;

    case 209: /* instantArgs  */
#line 113 "surface.yy" /* yacc.c:1257  */
      { deepSelfDestructViewExpressionVector(((*yyvaluep).yyViewExpressionVector)); }
#line 2454 "surface.c" /* yacc.c:1257  */
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
#line 248 "surface.yy" /* yacc.c:1652  */
    { YYACCEPT; }
#line 2725 "surface.c" /* yacc.c:1652  */
    break;

  case 3:
#line 250 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			}
#line 2733 "surface.c" /* yacc.c:1652  */
    break;

  case 8:
#line 264 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2739 "surface.c" /* yacc.c:1652  */
    break;

  case 9:
#line 266 "surface.yy" /* yacc.c:1652  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			}
#line 2752 "surface.c" /* yacc.c:1652  */
    break;

  case 10:
#line 274 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2758 "surface.c" /* yacc.c:1652  */
    break;

  case 11:
#line 276 "surface.yy" /* yacc.c:1652  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2771 "surface.c" /* yacc.c:1652  */
    break;

  case 12:
#line 284 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2777 "surface.c" /* yacc.c:1652  */
    break;

  case 13:
#line 286 "surface.yy" /* yacc.c:1652  */
    {
			  int lineNr = lineNumber;
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr) &&
			      !directoryManager.alreadySeen(directory, fileName))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2790 "surface.c" /* yacc.c:1652  */
    break;

  case 14:
#line 295 "surface.yy" /* yacc.c:1652  */
    {
			  cout << directoryManager.getCwd() << '\n';
			}
#line 2798 "surface.c" /* yacc.c:1652  */
    break;

  case 15:
#line 298 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2804 "surface.c" /* yacc.c:1652  */
    break;

  case 16:
#line 300 "surface.yy" /* yacc.c:1652  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			}
#line 2818 "surface.c" /* yacc.c:1652  */
    break;

  case 17:
#line 309 "surface.yy" /* yacc.c:1652  */
    { lexerFileNameMode(); }
#line 2824 "surface.c" /* yacc.c:1652  */
    break;

  case 18:
#line 311 "surface.yy" /* yacc.c:1652  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			}
#line 2838 "surface.c" /* yacc.c:1652  */
    break;

  case 19:
#line 321 "surface.yy" /* yacc.c:1652  */
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
#line 2853 "surface.c" /* yacc.c:1652  */
    break;

  case 20:
#line 331 "surface.yy" /* yacc.c:1652  */
    { lexerStringMode(); }
#line 2859 "surface.c" /* yacc.c:1652  */
    break;

  case 21:
#line 333 "surface.yy" /* yacc.c:1652  */
    {
			  returnValueDump = system((string("ls") + (yyvsp[0].yyString)).c_str());
			}
#line 2867 "surface.c" /* yacc.c:1652  */
    break;

  case 22:
#line 336 "surface.yy" /* yacc.c:1652  */
    { lexerStringMode(); }
#line 2873 "surface.c" /* yacc.c:1652  */
    break;

  case 23:
#line 338 "surface.yy" /* yacc.c:1652  */
    {
			  returnValueDump = system((string("ls -l") + (yyvsp[0].yyString)).c_str());
			}
#line 2881 "surface.c" /* yacc.c:1652  */
    break;

  case 24:
#line 342 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			}
#line 2890 "surface.c" /* yacc.c:1652  */
    break;

  case 25:
#line 347 "surface.yy" /* yacc.c:1652  */
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			}
#line 2901 "surface.c" /* yacc.c:1652  */
    break;

  case 26:
#line 380 "surface.yy" /* yacc.c:1652  */
    {
                          (yyval.yyModuleExpression) =  new ModuleExpression((yyvsp[-1].yyToken));
                        }
#line 2909 "surface.c" /* yacc.c:1652  */
    break;

  case 27:
#line 384 "surface.yy" /* yacc.c:1652  */
    {
                          (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-1].yyToken));
                        }
#line 2917 "surface.c" /* yacc.c:1652  */
    break;

  case 31:
#line 391 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), (yyvsp[0].yyModuleExpression));
			}
#line 2925 "surface.c" /* yacc.c:1652  */
    break;

  case 32:
#line 395 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyModuleExpression) = new ModuleExpression(t);
			}
#line 2936 "surface.c" /* yacc.c:1652  */
    break;

  case 34:
#line 405 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), (yyvsp[0].yyModuleExpression));
			}
#line 2944 "surface.c" /* yacc.c:1652  */
    break;

  case 39:
#line 417 "surface.yy" /* yacc.c:1652  */
    {
                          (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[0].yyToken));
                        }
#line 2952 "surface.c" /* yacc.c:1652  */
    break;

  case 40:
#line 424 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-2].yyModuleExpression), currentRenaming);
			  currentRenaming = 0;
			}
#line 2961 "surface.c" /* yacc.c:1652  */
    break;

  case 41:
#line 431 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = new ModuleExpression((yyvsp[-3].yyModuleExpression), *((yyvsp[-1].yyViewExpressionVector)));
			  delete (yyvsp[-1].yyViewExpressionVector);
			}
#line 2970 "surface.c" /* yacc.c:1652  */
    break;

  case 42:
#line 438 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyModuleExpression) = (yyvsp[-1].yyModuleExpression);
			}
#line 2978 "surface.c" /* yacc.c:1652  */
    break;

  case 43:
#line 447 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyViewExpression) = new ViewExpression((yyvsp[-3].yyViewExpression), *((yyvsp[-1].yyViewExpressionVector)));
			  delete (yyvsp[-1].yyViewExpressionVector);
			}
#line 2987 "surface.c" /* yacc.c:1652  */
    break;

  case 44:
#line 452 "surface.yy" /* yacc.c:1652  */
    {
			  (yyval.yyViewExpression) = new ViewExpression((yyvsp[0].yyToken));
			}
#line 2995 "surface.c" /* yacc.c:1652  */
    break;

  case 45:
#line 458 "surface.yy" /* yacc.c:1652  */
    {
			  (yyvsp[-2].yyViewExpressionVector)->append((yyvsp[0].yyViewExpression));
			  (yyval.yyViewExpressionVector) = (yyvsp[-2].yyViewExpressionVector);
			}
#line 3004 "surface.c" /* yacc.c:1652  */
    break;

  case 46:
#line 463 "surface.yy" /* yacc.c:1652  */
    {
			  Vector<ViewExpression*>* t =  new Vector<ViewExpression*>();
			  t->append((yyvsp[0].yyViewExpression));
			  (yyval.yyViewExpressionVector) = t;
			}
#line 3014 "surface.c" /* yacc.c:1652  */
    break;

  case 47:
#line 474 "surface.yy" /* yacc.c:1652  */
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			}
#line 3023 "surface.c" /* yacc.c:1652  */
    break;

  case 48:
#line 479 "surface.yy" /* yacc.c:1652  */
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			}
#line 3031 "surface.c" /* yacc.c:1652  */
    break;

  case 54:
#line 494 "surface.yy" /* yacc.c:1652  */
    {
			  currentRenaming->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3039 "surface.c" /* yacc.c:1652  */
    break;

  case 55:
#line 498 "surface.yy" /* yacc.c:1652  */
    {
			  currentRenaming->addLabelMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3047 "surface.c" /* yacc.c:1652  */
    break;

  case 56:
#line 501 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 3053 "surface.c" /* yacc.c:1652  */
    break;

  case 57:
#line 502 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
#line 3059 "surface.c" /* yacc.c:1652  */
    break;

  case 58:
#line 503 "surface.yy" /* yacc.c:1652  */
    {}
#line 3065 "surface.c" /* yacc.c:1652  */
    break;

  case 59:
#line 504 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->addStratMapping((yyvsp[0].yyToken)); }
#line 3071 "surface.c" /* yacc.c:1652  */
    break;

  case 60:
#line 506 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->addStratTarget((yyvsp[0].yyToken)); }
#line 3077 "surface.c" /* yacc.c:1652  */
    break;

  case 61:
#line 511 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 3083 "surface.c" /* yacc.c:1652  */
    break;

  case 62:
#line 512 "surface.yy" /* yacc.c:1652  */
    {}
#line 3089 "surface.c" /* yacc.c:1652  */
    break;

  case 63:
#line 513 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 3095 "surface.c" /* yacc.c:1652  */
    break;

  case 66:
#line 523 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 3101 "surface.c" /* yacc.c:1652  */
    break;

  case 67:
#line 524 "surface.yy" /* yacc.c:1652  */
    {}
#line 3107 "surface.c" /* yacc.c:1652  */
    break;

  case 68:
#line 525 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 3113 "surface.c" /* yacc.c:1652  */
    break;

  case 71:
#line 532 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->setPrec((yyvsp[0].yyToken)); }
#line 3119 "surface.c" /* yacc.c:1652  */
    break;

  case 72:
#line 533 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3125 "surface.c" /* yacc.c:1652  */
    break;

  case 73:
#line 534 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->setGather(tokenSequence); }
#line 3131 "surface.c" /* yacc.c:1652  */
    break;

  case 74:
#line 535 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3137 "surface.c" /* yacc.c:1652  */
    break;

  case 75:
#line 536 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->setFormat(tokenSequence); }
#line 3143 "surface.c" /* yacc.c:1652  */
    break;

  case 76:
#line 537 "surface.yy" /* yacc.c:1652  */
    { lexerLatexMode(); }
#line 3149 "surface.c" /* yacc.c:1652  */
    break;

  case 77:
#line 538 "surface.yy" /* yacc.c:1652  */
    { currentRenaming->setLatexMacro((yyvsp[-1].yyString)); }
#line 3155 "surface.c" /* yacc.c:1652  */
    break;

  case 78:
#line 544 "surface.yy" /* yacc.c:1652  */
    { lexerIdMode(); }
#line 3161 "surface.c" /* yacc.c:1652  */
    break;

  case 79:
#line 546 "surface.yy" /* yacc.c:1652  */
    {
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			  interpreter.setCurrentView(new SyntacticView((yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CV;
			}
#line 3171 "surface.c" /* yacc.c:1652  */
    break;

  case 80:
#line 554 "surface.yy" /* yacc.c:1652  */
    {
			  CV->addFrom((yyvsp[-5].yyModuleExpression));
			  CV->addTo((yyvsp[-3].yyModuleExpression));
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[-9].yyToken)).code(), CV);
			  CV->finishView();
			}
#line 3184 "surface.c" /* yacc.c:1652  */
    break;

  case 82:
#line 565 "surface.yy" /* yacc.c:1652  */
    {}
#line 3190 "surface.c" /* yacc.c:1652  */
    break;

  case 83:
#line 569 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping " << QUOTE("->") << " in variable declaration.");
			}
#line 3199 "surface.c" /* yacc.c:1652  */
    break;

  case 84:
#line 573 "surface.yy" /* yacc.c:1652  */
    {}
#line 3205 "surface.c" /* yacc.c:1652  */
    break;

  case 85:
#line 577 "surface.yy" /* yacc.c:1652  */
    {
			  CV->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3213 "surface.c" /* yacc.c:1652  */
    break;

  case 86:
#line 580 "surface.yy" /* yacc.c:1652  */
    {}
#line 3219 "surface.c" /* yacc.c:1652  */
    break;

  case 87:
#line 581 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 3225 "surface.c" /* yacc.c:1652  */
    break;

  case 91:
#line 587 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyToken) = (yyvsp[-1].yyToken); }
#line 3231 "surface.c" /* yacc.c:1652  */
    break;

  case 92:
#line 589 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			}
#line 3242 "surface.c" /* yacc.c:1652  */
    break;

  case 93:
#line 598 "surface.yy" /* yacc.c:1652  */
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			}
#line 3254 "surface.c" /* yacc.c:1652  */
    break;

  case 94:
#line 606 "surface.yy" /* yacc.c:1652  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3262 "surface.c" /* yacc.c:1652  */
    break;

  case 95:
#line 610 "surface.yy" /* yacc.c:1652  */
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			}
#line 3271 "surface.c" /* yacc.c:1652  */
    break;

  case 96:
#line 615 "surface.yy" /* yacc.c:1652  */
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1);
			}
#line 3285 "surface.c" /* yacc.c:1652  */
    break;

  case 97:
#line 625 "surface.yy" /* yacc.c:1652  */
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
#line 3309 "surface.c" /* yacc.c:1652  */
    break;

  case 98:
#line 647 "surface.yy" /* yacc.c:1652  */
    {
			  strategyCall.resize(1);
			  strategyCall[0] = (yyvsp[0].yyToken);
			}
#line 3318 "surface.c" /* yacc.c:1652  */
    break;

  case 99:
#line 651 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3324 "surface.c" /* yacc.c:1652  */
    break;

  case 100:
#line 653 "surface.yy" /* yacc.c:1652  */
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
#line 3339 "surface.c" /* yacc.c:1652  */
    break;

  case 101:
#line 665 "surface.yy" /* yacc.c:1652  */
    {
			  CV->addStratMapping((yyvsp[0].yyToken));
			}
#line 3347 "surface.c" /* yacc.c:1652  */
    break;

  case 102:
#line 669 "surface.yy" /* yacc.c:1652  */
    {
			  CV->addStratTarget((yyvsp[-1].yyToken));
			}
#line 3355 "surface.c" /* yacc.c:1652  */
    break;

  case 103:
#line 673 "surface.yy" /* yacc.c:1652  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3363 "surface.c" /* yacc.c:1652  */
    break;

  case 104:
#line 677 "surface.yy" /* yacc.c:1652  */
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
#line 3389 "surface.c" /* yacc.c:1652  */
    break;

  case 105:
#line 700 "surface.yy" /* yacc.c:1652  */
    {}
#line 3395 "surface.c" /* yacc.c:1652  */
    break;

  case 106:
#line 702 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			}
#line 3406 "surface.c" /* yacc.c:1652  */
    break;

  case 107:
#line 710 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3412 "surface.c" /* yacc.c:1652  */
    break;

  case 108:
#line 711 "surface.yy" /* yacc.c:1652  */
    {}
#line 3418 "surface.c" /* yacc.c:1652  */
    break;

  case 109:
#line 717 "surface.yy" /* yacc.c:1652  */
    { lexerIdMode(); }
#line 3424 "surface.c" /* yacc.c:1652  */
    break;

  case 110:
#line 719 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken), &interpreter));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3434 "surface.c" /* yacc.c:1652  */
    break;

  case 111:
#line 725 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[0].yyToken));
			}
#line 3444 "surface.c" /* yacc.c:1652  */
    break;

  case 112:
#line 732 "surface.yy" /* yacc.c:1652  */
    {}
#line 3450 "surface.c" /* yacc.c:1652  */
    break;

  case 113:
#line 734 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  store(t);
			}
#line 3461 "surface.c" /* yacc.c:1652  */
    break;

  case 114:
#line 742 "surface.yy" /* yacc.c:1652  */
    {}
#line 3467 "surface.c" /* yacc.c:1652  */
    break;

  case 115:
#line 743 "surface.yy" /* yacc.c:1652  */
    {}
#line 3473 "surface.c" /* yacc.c:1652  */
    break;

  case 118:
#line 751 "surface.yy" /* yacc.c:1652  */
    {
			  currentSyntaxContainer->addParameter2((yyvsp[-2].yyToken), (yyvsp[0].yyModuleExpression));
			}
#line 3481 "surface.c" /* yacc.c:1652  */
    break;

  case 119:
#line 756 "surface.yy" /* yacc.c:1652  */
    {}
#line 3487 "surface.c" /* yacc.c:1652  */
    break;

  case 120:
#line 758 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			}
#line 3497 "surface.c" /* yacc.c:1652  */
    break;

  case 121:
#line 766 "surface.yy" /* yacc.c:1652  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[0].yyToken);  // needed for line number
			}
#line 3508 "surface.c" /* yacc.c:1652  */
    break;

  case 123:
#line 775 "surface.yy" /* yacc.c:1652  */
    {}
#line 3514 "surface.c" /* yacc.c:1652  */
    break;

  case 127:
#line 782 "surface.yy" /* yacc.c:1652  */
    {}
#line 3520 "surface.c" /* yacc.c:1652  */
    break;

  case 128:
#line 786 "surface.yy" /* yacc.c:1652  */
    {
			  CM->addImport((yyvsp[-1].yyToken), (yyvsp[0].yyModuleExpression));
			}
#line 3528 "surface.c" /* yacc.c:1652  */
    break;

  case 129:
#line 790 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3534 "surface.c" /* yacc.c:1652  */
    break;

  case 130:
#line 791 "surface.yy" /* yacc.c:1652  */
    { CM->addSortDecl(tokenSequence); }
#line 3540 "surface.c" /* yacc.c:1652  */
    break;

  case 131:
#line 793 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3546 "surface.c" /* yacc.c:1652  */
    break;

  case 132:
#line 794 "surface.yy" /* yacc.c:1652  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3552 "surface.c" /* yacc.c:1652  */
    break;

  case 133:
#line 796 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COLON, 1); }
#line 3558 "surface.c" /* yacc.c:1652  */
    break;

  case 134:
#line 797 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3564 "surface.c" /* yacc.c:1652  */
    break;

  case 135:
#line 798 "surface.yy" /* yacc.c:1652  */
    {}
#line 3570 "surface.c" /* yacc.c:1652  */
    break;

  case 136:
#line 800 "surface.yy" /* yacc.c:1652  */
    {}
#line 3576 "surface.c" /* yacc.c:1652  */
    break;

  case 137:
#line 802 "surface.yy" /* yacc.c:1652  */
    {}
#line 3582 "surface.c" /* yacc.c:1652  */
    break;

  case 138:
#line 804 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1); }
#line 3588 "surface.c" /* yacc.c:1652  */
    break;

  case 139:
#line 805 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3594 "surface.c" /* yacc.c:1652  */
    break;

  case 140:
#line 806 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3600 "surface.c" /* yacc.c:1652  */
    break;

  case 141:
#line 808 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1);  }
#line 3606 "surface.c" /* yacc.c:1652  */
    break;

  case 142:
#line 809 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3612 "surface.c" /* yacc.c:1652  */
    break;

  case 143:
#line 810 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3618 "surface.c" /* yacc.c:1652  */
    break;

  case 144:
#line 811 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3624 "surface.c" /* yacc.c:1652  */
    break;

  case 145:
#line 813 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3630 "surface.c" /* yacc.c:1652  */
    break;

  case 146:
#line 814 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3636 "surface.c" /* yacc.c:1652  */
    break;

  case 147:
#line 815 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3642 "surface.c" /* yacc.c:1652  */
    break;

  case 148:
#line 817 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3648 "surface.c" /* yacc.c:1652  */
    break;

  case 149:
#line 818 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3654 "surface.c" /* yacc.c:1652  */
    break;

  case 150:
#line 819 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3660 "surface.c" /* yacc.c:1652  */
    break;

  case 151:
#line 820 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3666 "surface.c" /* yacc.c:1652  */
    break;

  case 152:
#line 822 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3672 "surface.c" /* yacc.c:1652  */
    break;

  case 153:
#line 823 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3678 "surface.c" /* yacc.c:1652  */
    break;

  case 154:
#line 824 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3684 "surface.c" /* yacc.c:1652  */
    break;

  case 155:
#line 826 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3690 "surface.c" /* yacc.c:1652  */
    break;

  case 156:
#line 827 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3696 "surface.c" /* yacc.c:1652  */
    break;

  case 157:
#line 828 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3702 "surface.c" /* yacc.c:1652  */
    break;

  case 158:
#line 829 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3708 "surface.c" /* yacc.c:1652  */
    break;

  case 159:
#line 831 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_ASSIGN, 1); }
#line 3714 "surface.c" /* yacc.c:1652  */
    break;

  case 160:
#line 832 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3720 "surface.c" /* yacc.c:1652  */
    break;

  case 161:
#line 833 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3726 "surface.c" /* yacc.c:1652  */
    break;

  case 162:
#line 835 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_ASSIGN, 1); }
#line 3732 "surface.c" /* yacc.c:1652  */
    break;

  case 163:
#line 836 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3738 "surface.c" /* yacc.c:1652  */
    break;

  case 164:
#line 837 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3744 "surface.c" /* yacc.c:1652  */
    break;

  case 165:
#line 838 "surface.yy" /* yacc.c:1652  */
    { CM->addStatement(lexerBubble); }
#line 3750 "surface.c" /* yacc.c:1652  */
    break;

  case 166:
#line 840 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3756 "surface.c" /* yacc.c:1652  */
    break;

  case 167:
#line 844 "surface.yy" /* yacc.c:1652  */
    {}
#line 3762 "surface.c" /* yacc.c:1652  */
    break;

  case 168:
#line 846 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_COLON, 1); }
#line 3768 "surface.c" /* yacc.c:1652  */
    break;

  case 169:
#line 847 "surface.yy" /* yacc.c:1652  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3774 "surface.c" /* yacc.c:1652  */
    break;

  case 170:
#line 848 "surface.yy" /* yacc.c:1652  */
    { CM->setFlag(SymbolType::MESSAGE); }
#line 3780 "surface.c" /* yacc.c:1652  */
    break;

  case 171:
#line 851 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3788 "surface.c" /* yacc.c:1652  */
    break;

  case 172:
#line 856 "surface.yy" /* yacc.c:1652  */
    {
			}
#line 3795 "surface.c" /* yacc.c:1652  */
    break;

  case 173:
#line 859 "surface.yy" /* yacc.c:1652  */
    {
			}
#line 3802 "surface.c" /* yacc.c:1652  */
    break;

  case 174:
#line 862 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3808 "surface.c" /* yacc.c:1652  */
    break;

  case 175:
#line 863 "surface.yy" /* yacc.c:1652  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3814 "surface.c" /* yacc.c:1652  */
    break;

  case 176:
#line 866 "surface.yy" /* yacc.c:1652  */
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeDeclsConsistent();
			}
#line 3827 "surface.c" /* yacc.c:1652  */
    break;

  case 177:
#line 876 "surface.yy" /* yacc.c:1652  */
    {}
#line 3833 "surface.c" /* yacc.c:1652  */
    break;

  case 178:
#line 877 "surface.yy" /* yacc.c:1652  */
    {}
#line 3839 "surface.c" /* yacc.c:1652  */
    break;

  case 181:
#line 885 "surface.yy" /* yacc.c:1652  */
    {
			}
#line 3846 "surface.c" /* yacc.c:1652  */
    break;

  case 182:
#line 889 "surface.yy" /* yacc.c:1652  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3852 "surface.c" /* yacc.c:1652  */
    break;

  case 183:
#line 890 "surface.yy" /* yacc.c:1652  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3858 "surface.c" /* yacc.c:1652  */
    break;

  case 186:
#line 897 "surface.yy" /* yacc.c:1652  */
    { singleton[0] = (yyvsp[0].yyToken); CM->addOpDecl(singleton); }
#line 3864 "surface.c" /* yacc.c:1652  */
    break;

  case 187:
#line 898 "surface.yy" /* yacc.c:1652  */
    { CM->addOpDecl(lexerBubble); }
#line 3870 "surface.c" /* yacc.c:1652  */
    break;

  case 190:
#line 904 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			}
#line 3879 "surface.c" /* yacc.c:1652  */
    break;

  case 195:
#line 916 "surface.yy" /* yacc.c:1652  */
    { CM->addStratDecl((yyvsp[0].yyToken)); }
#line 3885 "surface.c" /* yacc.c:1652  */
    break;

  case 198:
#line 927 "surface.yy" /* yacc.c:1652  */
    {}
#line 3891 "surface.c" /* yacc.c:1652  */
    break;

  case 199:
#line 928 "surface.yy" /* yacc.c:1652  */
    {}
#line 3897 "surface.c" /* yacc.c:1652  */
    break;

  case 200:
#line 932 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 3905 "surface.c" /* yacc.c:1652  */
    break;

  case 201:
#line 938 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping stray " << QUOTE(":") << " in operator declaration.");

			}
#line 3915 "surface.c" /* yacc.c:1652  */
    break;

  case 202:
#line 943 "surface.yy" /* yacc.c:1652  */
    {}
#line 3921 "surface.c" /* yacc.c:1652  */
    break;

  case 204:
#line 948 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3930 "surface.c" /* yacc.c:1652  */
    break;

  case 205:
#line 953 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3939 "surface.c" /* yacc.c:1652  */
    break;

  case 206:
#line 960 "surface.yy" /* yacc.c:1652  */
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			}
#line 3948 "surface.c" /* yacc.c:1652  */
    break;

  case 208:
#line 967 "surface.yy" /* yacc.c:1652  */
    {}
#line 3954 "surface.c" /* yacc.c:1652  */
    break;

  case 209:
#line 970 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 3960 "surface.c" /* yacc.c:1652  */
    break;

  case 210:
#line 971 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 3966 "surface.c" /* yacc.c:1652  */
    break;

  case 212:
#line 975 "surface.yy" /* yacc.c:1652  */
    {}
#line 3972 "surface.c" /* yacc.c:1652  */
    break;

  case 213:
#line 979 "surface.yy" /* yacc.c:1652  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			}
#line 3981 "surface.c" /* yacc.c:1652  */
    break;

  case 214:
#line 983 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 3987 "surface.c" /* yacc.c:1652  */
    break;

  case 215:
#line 985 "surface.yy" /* yacc.c:1652  */
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
#line 3995 "surface.c" /* yacc.c:1652  */
    break;

  case 216:
#line 990 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4001 "surface.c" /* yacc.c:1652  */
    break;

  case 217:
#line 991 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4007 "surface.c" /* yacc.c:1652  */
    break;

  case 218:
#line 994 "surface.yy" /* yacc.c:1652  */
    {}
#line 4013 "surface.c" /* yacc.c:1652  */
    break;

  case 219:
#line 995 "surface.yy" /* yacc.c:1652  */
    {}
#line 4019 "surface.c" /* yacc.c:1652  */
    break;

  case 222:
#line 1003 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			}
#line 4027 "surface.c" /* yacc.c:1652  */
    break;

  case 223:
#line 1007 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			}
#line 4035 "surface.c" /* yacc.c:1652  */
    break;

  case 224:
#line 1011 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			}
#line 4043 "surface.c" /* yacc.c:1652  */
    break;

  case 225:
#line 1017 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::ASSOC);
			}
#line 4051 "surface.c" /* yacc.c:1652  */
    break;

  case 226:
#line 1021 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::COMM);
			}
#line 4059 "surface.c" /* yacc.c:1652  */
    break;

  case 227:
#line 1024 "surface.yy" /* yacc.c:1652  */
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); }
#line 4065 "surface.c" /* yacc.c:1652  */
    break;

  case 228:
#line 1025 "surface.yy" /* yacc.c:1652  */
    { CM->setIdentity(lexerBubble); }
#line 4071 "surface.c" /* yacc.c:1652  */
    break;

  case 229:
#line 1027 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::IDEM);
			}
#line 4079 "surface.c" /* yacc.c:1652  */
    break;

  case 230:
#line 1031 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::ITER);
			}
#line 4087 "surface.c" /* yacc.c:1652  */
    break;

  case 231:
#line 1034 "surface.yy" /* yacc.c:1652  */
    { CM->setPrec((yyvsp[0].yyToken)); }
#line 4093 "surface.c" /* yacc.c:1652  */
    break;

  case 232:
#line 1035 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4099 "surface.c" /* yacc.c:1652  */
    break;

  case 233:
#line 1036 "surface.yy" /* yacc.c:1652  */
    { CM->setGather(tokenSequence); }
#line 4105 "surface.c" /* yacc.c:1652  */
    break;

  case 234:
#line 1037 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4111 "surface.c" /* yacc.c:1652  */
    break;

  case 235:
#line 1038 "surface.yy" /* yacc.c:1652  */
    { CM->setFormat(tokenSequence); }
#line 4117 "surface.c" /* yacc.c:1652  */
    break;

  case 236:
#line 1039 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4123 "surface.c" /* yacc.c:1652  */
    break;

  case 237:
#line 1040 "surface.yy" /* yacc.c:1652  */
    { CM->setStrat(tokenSequence); }
#line 4129 "surface.c" /* yacc.c:1652  */
    break;

  case 238:
#line 1041 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4135 "surface.c" /* yacc.c:1652  */
    break;

  case 239:
#line 1042 "surface.yy" /* yacc.c:1652  */
    { CM->setStrat(tokenSequence); }
#line 4141 "surface.c" /* yacc.c:1652  */
    break;

  case 240:
#line 1043 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4147 "surface.c" /* yacc.c:1652  */
    break;

  case 241:
#line 1044 "surface.yy" /* yacc.c:1652  */
    { CM->setPoly(tokenSequence); }
#line 4153 "surface.c" /* yacc.c:1652  */
    break;

  case 242:
#line 1046 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::MEMO);
			}
#line 4161 "surface.c" /* yacc.c:1652  */
    break;

  case 243:
#line 1050 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::CTOR);
			}
#line 4169 "surface.c" /* yacc.c:1652  */
    break;

  case 244:
#line 1054 "surface.yy" /* yacc.c:1652  */
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			}
#line 4178 "surface.c" /* yacc.c:1652  */
    break;

  case 245:
#line 1058 "surface.yy" /* yacc.c:1652  */
    { clear(); }
#line 4184 "surface.c" /* yacc.c:1652  */
    break;

  case 246:
#line 1059 "surface.yy" /* yacc.c:1652  */
    { CM->setFrozen(tokenSequence); }
#line 4190 "surface.c" /* yacc.c:1652  */
    break;

  case 247:
#line 1061 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::CONFIG);
			}
#line 4198 "surface.c" /* yacc.c:1652  */
    break;

  case 248:
#line 1065 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::OBJECT);
			}
#line 4206 "surface.c" /* yacc.c:1652  */
    break;

  case 249:
#line 1069 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 4214 "surface.c" /* yacc.c:1652  */
    break;

  case 250:
#line 1073 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 4222 "surface.c" /* yacc.c:1652  */
    break;

  case 251:
#line 1076 "surface.yy" /* yacc.c:1652  */
    { lexerLatexMode(); }
#line 4228 "surface.c" /* yacc.c:1652  */
    break;

  case 252:
#line 1077 "surface.yy" /* yacc.c:1652  */
    { CM->setLatexMacro((yyvsp[-1].yyString)); }
#line 4234 "surface.c" /* yacc.c:1652  */
    break;

  case 253:
#line 1078 "surface.yy" /* yacc.c:1652  */
    {}
#line 4240 "surface.c" /* yacc.c:1652  */
    break;

  case 254:
#line 1080 "surface.yy" /* yacc.c:1652  */
    {
			  CM->setFlag(SymbolType::DITTO);
			}
#line 4248 "surface.c" /* yacc.c:1652  */
    break;

  case 256:
#line 1090 "surface.yy" /* yacc.c:1652  */
    {}
#line 4254 "surface.c" /* yacc.c:1652  */
    break;

  case 257:
#line 1093 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4260 "surface.c" /* yacc.c:1652  */
    break;

  case 258:
#line 1094 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4266 "surface.c" /* yacc.c:1652  */
    break;

  case 259:
#line 1097 "surface.yy" /* yacc.c:1652  */
    {}
#line 4272 "surface.c" /* yacc.c:1652  */
    break;

  case 260:
#line 1098 "surface.yy" /* yacc.c:1652  */
    {}
#line 4278 "surface.c" /* yacc.c:1652  */
    break;

  case 261:
#line 1101 "surface.yy" /* yacc.c:1652  */
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[0].yyToken), tokenSequence); }
#line 4284 "surface.c" /* yacc.c:1652  */
    break;

  case 262:
#line 1102 "surface.yy" /* yacc.c:1652  */
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4290 "surface.c" /* yacc.c:1652  */
    break;

  case 263:
#line 1103 "surface.yy" /* yacc.c:1652  */
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4296 "surface.c" /* yacc.c:1652  */
    break;

  case 264:
#line 1104 "surface.yy" /* yacc.c:1652  */
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 4302 "surface.c" /* yacc.c:1652  */
    break;

  case 265:
#line 1110 "surface.yy" /* yacc.c:1652  */
    {}
#line 4308 "surface.c" /* yacc.c:1652  */
    break;

  case 266:
#line 1112 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			}
#line 4317 "surface.c" /* yacc.c:1652  */
    break;

  case 267:
#line 1118 "surface.yy" /* yacc.c:1652  */
    {}
#line 4323 "surface.c" /* yacc.c:1652  */
    break;

  case 268:
#line 1120 "surface.yy" /* yacc.c:1652  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
#line 4331 "surface.c" /* yacc.c:1652  */
    break;

  case 269:
#line 1128 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4337 "surface.c" /* yacc.c:1652  */
    break;

  case 270:
#line 1129 "surface.yy" /* yacc.c:1652  */
    {}
#line 4343 "surface.c" /* yacc.c:1652  */
    break;

  case 271:
#line 1132 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4349 "surface.c" /* yacc.c:1652  */
    break;

  case 272:
#line 1133 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4355 "surface.c" /* yacc.c:1652  */
    break;

  case 273:
#line 1134 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4361 "surface.c" /* yacc.c:1652  */
    break;

  case 274:
#line 1135 "surface.yy" /* yacc.c:1652  */
    { store((yyvsp[0].yyToken)); }
#line 4367 "surface.c" /* yacc.c:1652  */
    break;

  case 275:
#line 1142 "surface.yy" /* yacc.c:1652  */
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  (yyval.yyToken) = t;
			}
#line 4381 "surface.c" /* yacc.c:1652  */
    break;

  case 276:
#line 1153 "surface.yy" /* yacc.c:1652  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4387 "surface.c" /* yacc.c:1652  */
    break;

  case 277:
#line 1154 "surface.yy" /* yacc.c:1652  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4393 "surface.c" /* yacc.c:1652  */
    break;

  case 278:
#line 1155 "surface.yy" /* yacc.c:1652  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4399 "surface.c" /* yacc.c:1652  */
    break;

  case 279:
#line 1158 "surface.yy" /* yacc.c:1652  */
    { fragStore((yyvsp[0].yyToken)); }
#line 4405 "surface.c" /* yacc.c:1652  */
    break;

  case 280:
#line 1159 "surface.yy" /* yacc.c:1652  */
    {}
#line 4411 "surface.c" /* yacc.c:1652  */
    break;

  case 281:
#line 1160 "surface.yy" /* yacc.c:1652  */
    {}
#line 4417 "surface.c" /* yacc.c:1652  */
    break;

  case 387:
#line 1241 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 1); }
#line 4423 "surface.c" /* yacc.c:1652  */
    break;

  case 388:
#line 1243 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setCurrentModule(lexerBubble);
			}
#line 4431 "surface.c" /* yacc.c:1652  */
    break;

  case 389:
#line 1246 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 1); }
#line 4437 "surface.c" /* yacc.c:1652  */
    break;

  case 390:
#line 1248 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			}
#line 4446 "surface.c" /* yacc.c:1652  */
    break;

  case 391:
#line 1253 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4455 "surface.c" /* yacc.c:1652  */
    break;

  case 392:
#line 1258 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			}
#line 4465 "surface.c" /* yacc.c:1652  */
    break;

  case 393:
#line 1265 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4474 "surface.c" /* yacc.c:1652  */
    break;

  case 394:
#line 1270 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			}
#line 4484 "surface.c" /* yacc.c:1652  */
    break;

  case 395:
#line 1277 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4493 "surface.c" /* yacc.c:1652  */
    break;

  case 396:
#line 1282 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			}
#line 4503 "surface.c" /* yacc.c:1652  */
    break;

  case 397:
#line 1289 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4512 "surface.c" /* yacc.c:1652  */
    break;

  case 398:
#line 1294 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[-3].yyBool));
			}
#line 4522 "surface.c" /* yacc.c:1652  */
    break;

  case 399:
#line 1301 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4532 "surface.c" /* yacc.c:1652  */
    break;

  case 400:
#line 1307 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4542 "surface.c" /* yacc.c:1652  */
    break;

  case 401:
#line 1313 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4553 "surface.c" /* yacc.c:1652  */
    break;

  case 402:
#line 1320 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4563 "surface.c" /* yacc.c:1652  */
    break;

  case 403:
#line 1326 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4574 "surface.c" /* yacc.c:1652  */
    break;

  case 404:
#line 1333 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4584 "surface.c" /* yacc.c:1652  */
    break;

  case 405:
#line 1339 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4594 "surface.c" /* yacc.c:1652  */
    break;

  case 406:
#line 1345 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4604 "surface.c" /* yacc.c:1652  */
    break;

  case 407:
#line 1351 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4614 "surface.c" /* yacc.c:1652  */
    break;

  case 408:
#line 1357 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool), true);
			}
#line 4624 "surface.c" /* yacc.c:1652  */
    break;

  case 409:
#line 1364 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4633 "surface.c" /* yacc.c:1652  */
    break;

  case 410:
#line 1369 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.check(lexerBubble);
			}
#line 4643 "surface.c" /* yacc.c:1652  */
    break;

  case 411:
#line 1376 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4654 "surface.c" /* yacc.c:1652  */
    break;

  case 412:
#line 1383 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[-2].yySearchKind), (yyvsp[-3].yyBool));
			}
#line 4664 "surface.c" /* yacc.c:1652  */
    break;

  case 413:
#line 1389 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4674 "surface.c" /* yacc.c:1652  */
    break;

  case 414:
#line 1395 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[-2].yyBool), number);
			}
#line 4684 "surface.c" /* yacc.c:1652  */
    break;

  case 415:
#line 1401 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4694 "surface.c" /* yacc.c:1652  */
    break;

  case 416:
#line 1407 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4704 "surface.c" /* yacc.c:1652  */
    break;

  case 417:
#line 1413 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4714 "surface.c" /* yacc.c:1652  */
    break;

  case 418:
#line 1419 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, false, (yyvsp[-4].yyBool));
			}
#line 4724 "surface.c" /* yacc.c:1652  */
    break;

  case 419:
#line 1425 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4734 "surface.c" /* yacc.c:1652  */
    break;

  case 420:
#line 1431 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, true, (yyvsp[-5].yyBool));
			}
#line 4744 "surface.c" /* yacc.c:1652  */
    break;

  case 421:
#line 1437 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4754 "surface.c" /* yacc.c:1652  */
    break;

  case 422:
#line 1443 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantMatch(lexerBubble, number, (yyvsp[-4].yyBool));
			}
#line 4764 "surface.c" /* yacc.c:1652  */
    break;

  case 423:
#line 1450 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4774 "surface.c" /* yacc.c:1652  */
    break;

  case 424:
#line 1456 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.getVariants(lexerBubble, number, (yyvsp[-3].yyBool), (yyvsp[-5].yyBool));
			}
#line 4784 "surface.c" /* yacc.c:1652  */
    break;

  case 425:
#line 1462 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.cont((yyvsp[-1].yyInt64), (yyvsp[-3].yyBool));
			}
#line 4792 "surface.c" /* yacc.c:1652  */
    break;

  case 426:
#line 1466 "surface.yy" /* yacc.c:1652  */
    {
			  //
			  //	test is a generic command to call code with a term for development purposes.
			  //
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4804 "surface.c" /* yacc.c:1652  */
    break;

  case 427:
#line 1474 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.test(lexerBubble);
			    //interpreter.newNarrow(lexerBubble);

			}
#line 4816 "surface.c" /* yacc.c:1652  */
    break;

  case 428:
#line 1482 "surface.yy" /* yacc.c:1652  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4825 "surface.c" /* yacc.c:1652  */
    break;

  case 429:
#line 1487 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			}
#line 4835 "surface.c" /* yacc.c:1652  */
    break;

  case 430:
#line 1493 "surface.yy" /* yacc.c:1652  */
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			}
#line 4845 "surface.c" /* yacc.c:1652  */
    break;

  case 431:
#line 1498 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 4851 "surface.c" /* yacc.c:1652  */
    break;

  case 432:
#line 1500 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[-2].yyBool));
			}
#line 4860 "surface.c" /* yacc.c:1652  */
    break;

  case 433:
#line 1504 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 4866 "surface.c" /* yacc.c:1652  */
    break;

  case 434:
#line 1506 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[-2].yyBool));
			}
#line 4875 "surface.c" /* yacc.c:1652  */
    break;

  case 435:
#line 1510 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 4881 "surface.c" /* yacc.c:1652  */
    break;

  case 436:
#line 1512 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[-2].yyBool));
			}
#line 4890 "surface.c" /* yacc.c:1652  */
    break;

  case 437:
#line 1516 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 4896 "surface.c" /* yacc.c:1652  */
    break;

  case 438:
#line 1518 "surface.yy" /* yacc.c:1652  */
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[-2].yyBool));
			}
#line 4905 "surface.c" /* yacc.c:1652  */
    break;

  case 439:
#line 1522 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 4911 "surface.c" /* yacc.c:1652  */
    break;

  case 440:
#line 1524 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->getFlatSignature()->clearMemo();
			}
#line 4920 "surface.c" /* yacc.c:1652  */
    break;

  case 441:
#line 1531 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 4926 "surface.c" /* yacc.c:1652  */
    break;

  case 442:
#line 1533 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4935 "surface.c" /* yacc.c:1652  */
    break;

  case 443:
#line 1537 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 4941 "surface.c" /* yacc.c:1652  */
    break;

  case 444:
#line 1539 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4950 "surface.c" /* yacc.c:1652  */
    break;

  case 445:
#line 1543 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 4956 "surface.c" /* yacc.c:1652  */
    break;

  case 446:
#line 1545 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			}
#line 4965 "surface.c" /* yacc.c:1652  */
    break;

  case 447:
#line 1549 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 4971 "surface.c" /* yacc.c:1652  */
    break;

  case 448:
#line 1551 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			}
#line 4980 "surface.c" /* yacc.c:1652  */
    break;

  case 449:
#line 1556 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showModules(true);
			}
#line 4988 "surface.c" /* yacc.c:1652  */
    break;

  case 450:
#line 1560 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showViews(true);
			}
#line 4996 "surface.c" /* yacc.c:1652  */
    break;

  case 451:
#line 1563 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5002 "surface.c" /* yacc.c:1652  */
    break;

  case 452:
#line 1565 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			}
#line 5011 "surface.c" /* yacc.c:1652  */
    break;

  case 453:
#line 1569 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5017 "surface.c" /* yacc.c:1652  */
    break;

  case 454:
#line 1571 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			}
#line 5026 "surface.c" /* yacc.c:1652  */
    break;

  case 455:
#line 1575 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5032 "surface.c" /* yacc.c:1652  */
    break;

  case 456:
#line 1577 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			}
#line 5041 "surface.c" /* yacc.c:1652  */
    break;

  case 457:
#line 1581 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5047 "surface.c" /* yacc.c:1652  */
    break;

  case 458:
#line 1583 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			}
#line 5056 "surface.c" /* yacc.c:1652  */
    break;

  case 459:
#line 1587 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5062 "surface.c" /* yacc.c:1652  */
    break;

  case 460:
#line 1589 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			}
#line 5071 "surface.c" /* yacc.c:1652  */
    break;

  case 461:
#line 1593 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5077 "surface.c" /* yacc.c:1652  */
    break;

  case 462:
#line 1595 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			}
#line 5086 "surface.c" /* yacc.c:1652  */
    break;

  case 463:
#line 1599 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5092 "surface.c" /* yacc.c:1652  */
    break;

  case 464:
#line 1601 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showStrats();
			}
#line 5101 "surface.c" /* yacc.c:1652  */
    break;

  case 465:
#line 1605 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5107 "surface.c" /* yacc.c:1652  */
    break;

  case 466:
#line 1607 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSds();
			}
#line 5116 "surface.c" /* yacc.c:1652  */
    break;

  case 467:
#line 1611 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5122 "surface.c" /* yacc.c:1652  */
    break;

  case 468:
#line 1613 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			}
#line 5131 "surface.c" /* yacc.c:1652  */
    break;

  case 469:
#line 1617 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5137 "surface.c" /* yacc.c:1652  */
    break;

  case 470:
#line 1619 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			}
#line 5146 "surface.c" /* yacc.c:1652  */
    break;

  case 471:
#line 1624 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showSearchPath((yyvsp[-1].yyInt64));
			}
#line 5154 "surface.c" /* yacc.c:1652  */
    break;

  case 472:
#line 1628 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showSearchPathLabels((yyvsp[-1].yyInt64));
			}
#line 5162 "surface.c" /* yacc.c:1652  */
    break;

  case 473:
#line 1632 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.showSearchGraph();
			}
#line 5170 "surface.c" /* yacc.c:1652  */
    break;

  case 474:
#line 1635 "surface.yy" /* yacc.c:1652  */
    { lexBubble(END_COMMAND, 0); }
#line 5176 "surface.c" /* yacc.c:1652  */
    break;

  case 475:
#line 1637 "surface.yy" /* yacc.c:1652  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			}
#line 5185 "surface.c" /* yacc.c:1652  */
    break;

  case 476:
#line 1645 "surface.yy" /* yacc.c:1652  */
    {
			  globalAdvisoryFlag = alwaysAdviseFlag ? true : (yyvsp[-1].yyBool);
			}
#line 5193 "surface.c" /* yacc.c:1652  */
    break;

  case 477:
#line 1649 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 5201 "surface.c" /* yacc.c:1652  */
    break;

  case 478:
#line 1653 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[-1].yyBool));
			}
#line 5209 "surface.c" /* yacc.c:1652  */
    break;

  case 479:
#line 1657 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[-1].yyBool));
			}
#line 5217 "surface.c" /* yacc.c:1652  */
    break;

  case 480:
#line 1661 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[-1].yyBool));
			}
#line 5225 "surface.c" /* yacc.c:1652  */
    break;

  case 481:
#line 1665 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[-1].yyBool));
			}
#line 5233 "surface.c" /* yacc.c:1652  */
    break;

  case 482:
#line 1669 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[-1].yyBool));
			}
#line 5241 "surface.c" /* yacc.c:1652  */
    break;

  case 483:
#line 1673 "surface.yy" /* yacc.c:1652  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 5249 "surface.c" /* yacc.c:1652  */
    break;

  case 484:
#line 1677 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setPrintFlag((yyvsp[-2].yyPrintFlags), (yyvsp[-1].yyBool));
			}
#line 5257 "surface.c" /* yacc.c:1652  */
    break;

  case 485:
#line 1681 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[-1].yyBool));
			}
#line 5265 "surface.c" /* yacc.c:1652  */
    break;

  case 486:
#line 1685 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[-1].yyBool));
			}
#line 5273 "surface.c" /* yacc.c:1652  */
    break;

  case 487:
#line 1689 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag((yyvsp[-2].yyFlags), (yyvsp[-1].yyBool));
			}
#line 5281 "surface.c" /* yacc.c:1652  */
    break;

  case 488:
#line 1693 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[-1].yyBool));
			}
#line 5289 "surface.c" /* yacc.c:1652  */
    break;

  case 489:
#line 1696 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 5295 "surface.c" /* yacc.c:1652  */
    break;

  case 490:
#line 1697 "surface.yy" /* yacc.c:1652  */
    { lexerInitialMode(); }
#line 5301 "surface.c" /* yacc.c:1652  */
    break;

  case 491:
#line 1699 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setAutoImport((yyvsp[-5].yyImportMode), (yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 5309 "surface.c" /* yacc.c:1652  */
    break;

  case 492:
#line 1702 "surface.yy" /* yacc.c:1652  */
    { lexerCmdMode(); }
#line 5315 "surface.c" /* yacc.c:1652  */
    break;

  case 493:
#line 1703 "surface.yy" /* yacc.c:1652  */
    { lexerInitialMode(); }
#line 5321 "surface.c" /* yacc.c:1652  */
    break;

  case 494:
#line 1705 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setOmodInclude((yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 5329 "surface.c" /* yacc.c:1652  */
    break;

  case 495:
#line 1709 "surface.yy" /* yacc.c:1652  */
    {
			  globalVerboseFlag = (yyvsp[-1].yyBool);
			}
#line 5337 "surface.c" /* yacc.c:1652  */
    break;

  case 496:
#line 1713 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[-1].yyBool));
			}
#line 5345 "surface.c" /* yacc.c:1652  */
    break;

  case 497:
#line 1717 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[-1].yyBool));
			}
#line 5353 "surface.c" /* yacc.c:1652  */
    break;

  case 498:
#line 1721 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[-1].yyBool));
			}
#line 5361 "surface.c" /* yacc.c:1652  */
    break;

  case 499:
#line 1725 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[-1].yyBool));
			}
#line 5369 "surface.c" /* yacc.c:1652  */
    break;

  case 500:
#line 1729 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[-1].yyBool));
			}
#line 5377 "surface.c" /* yacc.c:1652  */
    break;

  case 501:
#line 1736 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			}
#line 5385 "surface.c" /* yacc.c:1652  */
    break;

  case 502:
#line 1740 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			}
#line 5393 "surface.c" /* yacc.c:1652  */
    break;

  case 503:
#line 1744 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
#line 5401 "surface.c" /* yacc.c:1652  */
    break;

  case 504:
#line 1748 "surface.yy" /* yacc.c:1652  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			}
#line 5409 "surface.c" /* yacc.c:1652  */
    break;

  case 505:
#line 1755 "surface.yy" /* yacc.c:1652  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 5417 "surface.c" /* yacc.c:1652  */
    break;

  case 506:
#line 1759 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 5425 "surface.c" /* yacc.c:1652  */
    break;

  case 507:
#line 1765 "surface.yy" /* yacc.c:1652  */
    { lexerInitialMode(); }
#line 5431 "surface.c" /* yacc.c:1652  */
    break;

  case 509:
#line 1772 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; }
#line 5437 "surface.c" /* yacc.c:1652  */
    break;

  case 510:
#line 1773 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; }
#line 5443 "surface.c" /* yacc.c:1652  */
    break;

  case 511:
#line 1774 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; }
#line 5449 "surface.c" /* yacc.c:1652  */
    break;

  case 512:
#line 1775 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; }
#line 5455 "surface.c" /* yacc.c:1652  */
    break;

  case 513:
#line 1776 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; }
#line 5461 "surface.c" /* yacc.c:1652  */
    break;

  case 514:
#line 1777 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; }
#line 5467 "surface.c" /* yacc.c:1652  */
    break;

  case 515:
#line 1778 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; }
#line 5473 "surface.c" /* yacc.c:1652  */
    break;

  case 516:
#line 1779 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; }
#line 5479 "surface.c" /* yacc.c:1652  */
    break;

  case 517:
#line 1780 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; }
#line 5485 "surface.c" /* yacc.c:1652  */
    break;

  case 518:
#line 1781 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; }
#line 5491 "surface.c" /* yacc.c:1652  */
    break;

  case 519:
#line 1782 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_DISAMBIG_CONST; }
#line 5497 "surface.c" /* yacc.c:1652  */
    break;

  case 520:
#line 1785 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE; }
#line 5503 "surface.c" /* yacc.c:1652  */
    break;

  case 521:
#line 1786 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; }
#line 5509 "surface.c" /* yacc.c:1652  */
    break;

  case 522:
#line 1787 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; }
#line 5515 "surface.c" /* yacc.c:1652  */
    break;

  case 523:
#line 1788 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; }
#line 5521 "surface.c" /* yacc.c:1652  */
    break;

  case 524:
#line 1789 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; }
#line 5527 "surface.c" /* yacc.c:1652  */
    break;

  case 525:
#line 1790 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_MB; }
#line 5533 "surface.c" /* yacc.c:1652  */
    break;

  case 526:
#line 1791 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; }
#line 5539 "surface.c" /* yacc.c:1652  */
    break;

  case 527:
#line 1792 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_RL; }
#line 5545 "surface.c" /* yacc.c:1652  */
    break;

  case 528:
#line 1793 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_SD; }
#line 5551 "surface.c" /* yacc.c:1652  */
    break;

  case 529:
#line 1794 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; }
#line 5557 "surface.c" /* yacc.c:1652  */
    break;

  case 530:
#line 1795 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; }
#line 5563 "surface.c" /* yacc.c:1652  */
    break;

  case 531:
#line 1796 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; }
#line 5569 "surface.c" /* yacc.c:1652  */
    break;

  case 532:
#line 1799 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5575 "surface.c" /* yacc.c:1652  */
    break;

  case 533:
#line 1800 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5581 "surface.c" /* yacc.c:1652  */
    break;

  case 534:
#line 1803 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5587 "surface.c" /* yacc.c:1652  */
    break;

  case 535:
#line 1804 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5593 "surface.c" /* yacc.c:1652  */
    break;

  case 536:
#line 1807 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5599 "surface.c" /* yacc.c:1652  */
    break;

  case 537:
#line 1808 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5605 "surface.c" /* yacc.c:1652  */
    break;

  case 538:
#line 1811 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5611 "surface.c" /* yacc.c:1652  */
    break;

  case 539:
#line 1812 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5617 "surface.c" /* yacc.c:1652  */
    break;

  case 540:
#line 1817 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::NARROW; }
#line 5623 "surface.c" /* yacc.c:1652  */
    break;

  case 541:
#line 1818 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; }
#line 5629 "surface.c" /* yacc.c:1652  */
    break;

  case 542:
#line 1819 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::SEARCH; }
#line 5635 "surface.c" /* yacc.c:1652  */
    break;

  case 543:
#line 1820 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::SMT_SEARCH; }
#line 5641 "surface.c" /* yacc.c:1652  */
    break;

  case 544:
#line 1821 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::VU_NARROW; }
#line 5647 "surface.c" /* yacc.c:1652  */
    break;

  case 545:
#line 1822 "surface.yy" /* yacc.c:1652  */
    { (yyval.yySearchKind) = Interpreter::FVU_NARROW; }
#line 5653 "surface.c" /* yacc.c:1652  */
    break;

  case 546:
#line 1825 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5659 "surface.c" /* yacc.c:1652  */
    break;

  case 547:
#line 1826 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5665 "surface.c" /* yacc.c:1652  */
    break;

  case 548:
#line 1829 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5671 "surface.c" /* yacc.c:1652  */
    break;

  case 549:
#line 1830 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5677 "surface.c" /* yacc.c:1652  */
    break;

  case 550:
#line 1833 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = true; }
#line 5683 "surface.c" /* yacc.c:1652  */
    break;

  case 551:
#line 1834 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyBool) = false; }
#line 5689 "surface.c" /* yacc.c:1652  */
    break;

  case 552:
#line 1837 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); }
#line 5695 "surface.c" /* yacc.c:1652  */
    break;

  case 553:
#line 1838 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyInt64) = NONE; }
#line 5701 "surface.c" /* yacc.c:1652  */
    break;

  case 554:
#line 1841 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyImportMode) = ImportModule::PROTECTING; }
#line 5707 "surface.c" /* yacc.c:1652  */
    break;

  case 555:
#line 1842 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyImportMode) = ImportModule::EXTENDING; }
#line 5713 "surface.c" /* yacc.c:1652  */
    break;

  case 556:
#line 1843 "surface.yy" /* yacc.c:1652  */
    { (yyval.yyImportMode) = ImportModule::INCLUDING; }
#line 5719 "surface.c" /* yacc.c:1652  */
    break;

  case 558:
#line 1853 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5725 "surface.c" /* yacc.c:1652  */
    break;

  case 560:
#line 1857 "surface.yy" /* yacc.c:1652  */
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); }
#line 5731 "surface.c" /* yacc.c:1652  */
    break;

  case 563:
#line 1869 "surface.yy" /* yacc.c:1652  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5737 "surface.c" /* yacc.c:1652  */
    break;

  case 566:
#line 1872 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0);  }
#line 5743 "surface.c" /* yacc.c:1652  */
    break;

  case 568:
#line 1877 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5749 "surface.c" /* yacc.c:1652  */
    break;

  case 570:
#line 1879 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5755 "surface.c" /* yacc.c:1652  */
    break;

  case 573:
#line 1885 "surface.yy" /* yacc.c:1652  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5761 "surface.c" /* yacc.c:1652  */
    break;

  case 575:
#line 1887 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5767 "surface.c" /* yacc.c:1652  */
    break;

  case 578:
#line 1902 "surface.yy" /* yacc.c:1652  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5773 "surface.c" /* yacc.c:1652  */
    break;

  case 581:
#line 1905 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5779 "surface.c" /* yacc.c:1652  */
    break;

  case 583:
#line 1914 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5785 "surface.c" /* yacc.c:1652  */
    break;

  case 585:
#line 1916 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5791 "surface.c" /* yacc.c:1652  */
    break;

  case 587:
#line 1918 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5797 "surface.c" /* yacc.c:1652  */
    break;

  case 590:
#line 1928 "surface.yy" /* yacc.c:1652  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5803 "surface.c" /* yacc.c:1652  */
    break;

  case 592:
#line 1930 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5809 "surface.c" /* yacc.c:1652  */
    break;

  case 594:
#line 1932 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5815 "surface.c" /* yacc.c:1652  */
    break;

  case 597:
#line 1942 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5821 "surface.c" /* yacc.c:1652  */
    break;

  case 599:
#line 1944 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5827 "surface.c" /* yacc.c:1652  */
    break;

  case 602:
#line 1955 "surface.yy" /* yacc.c:1652  */
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			}
#line 5837 "surface.c" /* yacc.c:1652  */
    break;

  case 604:
#line 1961 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5843 "surface.c" /* yacc.c:1652  */
    break;

  case 607:
#line 1971 "surface.yy" /* yacc.c:1652  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5849 "surface.c" /* yacc.c:1652  */
    break;

  case 609:
#line 1973 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5855 "surface.c" /* yacc.c:1652  */
    break;

  case 612:
#line 1983 "surface.yy" /* yacc.c:1652  */
    { number2 = Token::codeToInt64(lexerBubble[2].code()); }
#line 5861 "surface.c" /* yacc.c:1652  */
    break;

  case 614:
#line 1985 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5867 "surface.c" /* yacc.c:1652  */
    break;

  case 617:
#line 1990 "surface.yy" /* yacc.c:1652  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0, 1); }
#line 5873 "surface.c" /* yacc.c:1652  */
    break;

  case 620:
#line 1996 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 1, 1); }
#line 5879 "surface.c" /* yacc.c:1652  */
    break;

  case 622:
#line 1998 "surface.yy" /* yacc.c:1652  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON | END_COMMAND, 0); }
#line 5885 "surface.c" /* yacc.c:1652  */
    break;

  case 624:
#line 2001 "surface.yy" /* yacc.c:1652  */
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(lexerBubble[0]);
			}
#line 5895 "surface.c" /* yacc.c:1652  */
    break;

  case 662:
#line 2040 "surface.yy" /* yacc.c:1652  */
    {}
#line 5901 "surface.c" /* yacc.c:1652  */
    break;

  case 664:
#line 2045 "surface.yy" /* yacc.c:1652  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			}
#line 5911 "surface.c" /* yacc.c:1652  */
    break;

  case 667:
#line 2056 "surface.yy" /* yacc.c:1652  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  interpreter.addSelected(singleton);
			}
#line 5920 "surface.c" /* yacc.c:1652  */
    break;

  case 668:
#line 2061 "surface.yy" /* yacc.c:1652  */
    {
			  interpreter.addSelected(lexerBubble);
			}
#line 5928 "surface.c" /* yacc.c:1652  */
    break;


#line 5932 "surface.c" /* yacc.c:1652  */
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
#line 2091 "surface.yy" /* yacc.c:1918  */


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
