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
#include "view.hh"
#include "moduleExpression.hh"
#include "fileTable.hh"
#include "directoryManager.hh"
#include "syntacticPreModule.hh"
#include "visibleModule.hh"  // HACK
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
/*
void lexerInitialMode();
void lexerIdMode();
void lexerCmdMode();
void lexerFileNameMode();
void lexerStringMode();
void lexerLatexMode();
bool handleEof();
bool includeFile(const string& directory, const string& fileName, bool silent, int lineNr);
//void eatComment(bool firstNonWhite);
*/
Vector<Token> singleton(1);
Vector<Token> tokenSequence;
Vector<Token> lexerBubble;
Vector<Token> fragments;
Vector<Token> moduleExpr;
Vector<Token> opDescription;
stack<ModuleExpression*> moduleExpressions;
Renaming* currentRenaming = 0;
SyntaxContainer* currentSyntaxContainer = 0;
SyntaxContainer* oldSyntaxContainer = 0;

Int64 number;
Int64 number2;

static void yyerror(UserLevelRewritingContext::ParseResult* parseResult, char *s);

void cleanUpModuleExpression();
void cleanUpParser();
void missingSpace(const Token& token);

#line 141 "surface.c" /* yacc.c:339  */

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
    KW_CONTINUE = 281,
    KW_SEARCH = 282,
    KW_SET = 283,
    KW_SHOW = 284,
    KW_ON = 285,
    KW_OFF = 286,
    KW_TRACE = 287,
    KW_BODY = 288,
    KW_BUILTIN = 289,
    KW_WHOLE = 290,
    KW_SELECT = 291,
    KW_DESELECT = 292,
    KW_CONDITION = 293,
    KW_SUBSTITUTION = 294,
    KW_PRINT = 295,
    KW_GRAPH = 296,
    KW_MIXFIX = 297,
    KW_FLAT = 298,
    KW_ATTRIBUTE = 299,
    KW_NEWLINE = 300,
    KW_WITH = 301,
    KW_PARENS = 302,
    KW_ALIASES = 303,
    KW_GC = 304,
    KW_TIME = 305,
    KW_STATS = 306,
    KW_TIMING = 307,
    KW_CMD = 308,
    KW_BREAKDOWN = 309,
    KW_BREAK = 310,
    KW_PATH = 311,
    KW_MODULE = 312,
    KW_MODULES = 313,
    KW_VIEWS = 314,
    KW_ALL = 315,
    KW_SORTS = 316,
    KW_OPS2 = 317,
    KW_VARS = 318,
    KW_MBS = 319,
    KW_EQS = 320,
    KW_RLS = 321,
    KW_SUMMARY = 322,
    KW_KINDS = 323,
    KW_ADVISE = 324,
    KW_VERBOSE = 325,
    KW_DO = 326,
    KW_CLEAR = 327,
    KW_PROTECT = 328,
    KW_EXTEND = 329,
    KW_INCLUDE = 330,
    KW_EXCLUDE = 331,
    KW_CONCEAL = 332,
    KW_REVEAL = 333,
    KW_COMPILE = 334,
    KW_COUNT = 335,
    KW_DEBUG = 336,
    KW_IRREDUNDANT = 337,
    KW_RESUME = 338,
    KW_ABORT = 339,
    KW_STEP = 340,
    KW_WHERE = 341,
    KW_CREDUCE = 342,
    KW_SREDUCE = 343,
    KW_DUMP = 344,
    KW_PROFILE = 345,
    KW_NUMBER = 346,
    KW_RAT = 347,
    KW_COLOR = 348,
    SIMPLE_NUMBER = 349,
    KW_PWD = 350,
    KW_CD = 351,
    KW_PUSHD = 352,
    KW_POPD = 353,
    KW_LS = 354,
    KW_LOAD = 355,
    KW_QUIT = 356,
    KW_EOF = 357,
    KW_TEST = 358,
    KW_SMT_SEARCH = 359,
    KW_ENDM = 360,
    KW_IMPORT = 361,
    KW_ENDV = 362,
    KW_SORT = 363,
    KW_SUBSORT = 364,
    KW_OP = 365,
    KW_OPS = 366,
    KW_MSGS = 367,
    KW_VAR = 368,
    KW_CLASS = 369,
    KW_SUBCLASS = 370,
    KW_MB = 371,
    KW_CMB = 372,
    KW_EQ = 373,
    KW_CEQ = 374,
    KW_RL = 375,
    KW_CRL = 376,
    KW_IS = 377,
    KW_FROM = 378,
    KW_ARROW = 379,
    KW_ARROW2 = 380,
    KW_PARTIAL = 381,
    KW_IF = 382,
    KW_LABEL = 383,
    KW_TO = 384,
    KW_COLON2 = 385,
    KW_ASSOC = 386,
    KW_COMM = 387,
    KW_ID = 388,
    KW_IDEM = 389,
    KW_ITER = 390,
    KW_LEFT = 391,
    KW_RIGHT = 392,
    KW_PREC = 393,
    KW_GATHER = 394,
    KW_METADATA = 395,
    KW_STRAT = 396,
    KW_POLY = 397,
    KW_MEMO = 398,
    KW_FROZEN = 399,
    KW_CTOR = 400,
    KW_LATEX = 401,
    KW_SPECIAL = 402,
    KW_CONFIG = 403,
    KW_OBJ = 404,
    KW_MSG = 405,
    KW_DITTO = 406,
    KW_FORMAT = 407,
    KW_ID_HOOK = 408,
    KW_OP_HOOK = 409,
    KW_TERM_HOOK = 410,
    KW_IN = 411,
    IDENTIFIER = 412,
    NUMERIC_ID = 413,
    ENDS_IN_DOT = 414,
    FORCE_LOOKAHEAD = 415
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 105 "surface.yy" /* yacc.c:355  */

  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;

#line 353 "surface.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (UserLevelRewritingContext::ParseResult* parseResult);

#endif /* !YY_YY_SURFACE_H_INCLUDED  */

/* Copy the second part of user declarations.  */
#line 116 "surface.yy" /* yacc.c:358  */

int yylex(YYSTYPE* lvalp);

#line 370 "surface.c" /* yacc.c:358  */

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
#define YYFINAL  116
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2355

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  175
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  246
/* YYNRULES -- Number of rules.  */
#define YYNRULES  620
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  928

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   415

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
     130,   131,   142,   141,   136,     2,   132,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   128,     2,
     133,   129,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   134,     2,   135,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   143,   137,   144,     2,     2,     2,     2,
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
     125,   126,   127,   138,   139,   140,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   235,   235,   237,   242,   243,   244,   245,   251,   251,
     261,   261,   271,   275,   275,   286,   286,   297,   308,   308,
     313,   318,   351,   355,   359,   360,   361,   362,   370,   379,
     380,   390,   391,   394,   395,   396,   403,   412,   412,   421,
     424,   425,   432,   437,   431,   443,   444,   447,   448,   451,
     455,   459,   460,   459,   466,   466,   468,   474,   474,   476,
     479,   480,   483,   484,   484,   486,   486,   488,   488,   495,
     497,   505,   495,   518,   519,   522,   527,   530,   534,   535,
     535,   537,   540,   541,   551,   559,   550,   568,   567,   600,
     601,   610,   610,   617,   619,   617,   632,   633,   642,   643,
     646,   647,   650,   658,   659,   667,   676,   677,   680,   680,
     683,   684,   687,   694,   694,   697,   697,   700,   701,   700,
     704,   706,   708,   709,   708,   712,   713,   714,   712,   717,
     718,   717,   721,   722,   723,   721,   726,   727,   726,   730,
     731,   732,   730,   735,   736,   735,   739,   745,   744,   751,
     751,   754,   765,   766,   769,   770,   773,   778,   779,   782,
     783,   786,   787,   790,   791,   792,   799,   805,   808,   809,
     814,   821,   828,   829,   832,   833,   836,   837,   840,   845,
     845,   852,   853,   856,   857,   860,   861,   864,   868,   872,
     878,   882,   886,   886,   888,   892,   896,   897,   897,   899,
     899,   901,   901,   903,   903,   905,   909,   913,   918,   918,
     920,   924,   928,   932,   936,   936,   938,   939,   949,   950,
     953,   954,   957,   958,   961,   962,   963,   964,   970,   972,
     978,   980,   988,   989,   992,   993,   994,   994,  1001,  1013,
    1014,  1014,  1018,  1018,  1020,  1026,  1026,  1026,  1026,  1026,
    1029,  1029,  1029,  1030,  1030,  1030,  1033,  1033,  1033,  1033,
    1034,  1034,  1034,  1034,  1034,  1034,  1037,  1037,  1037,  1038,
    1038,  1038,  1038,  1039,  1039,  1039,  1039,  1039,  1042,  1042,
    1048,  1048,  1048,  1048,  1048,  1048,  1048,  1048,  1049,  1049,
    1052,  1052,  1052,  1052,  1055,  1055,  1058,  1058,  1058,  1058,
    1058,  1058,  1059,  1059,  1059,  1060,  1060,  1060,  1060,  1060,
    1060,  1060,  1060,  1063,  1063,  1063,  1063,  1063,  1063,  1063,
    1066,  1066,  1066,  1069,  1069,  1069,  1069,  1069,  1069,  1069,
    1070,  1070,  1070,  1070,  1070,  1070,  1071,  1071,  1071,  1071,
    1072,  1072,  1072,  1072,  1073,  1073,  1073,  1100,  1100,  1105,
    1105,  1112,  1111,  1124,  1123,  1136,  1135,  1148,  1147,  1160,
    1159,  1172,  1171,  1185,  1184,  1198,  1197,  1211,  1210,  1223,
    1222,  1236,  1235,  1248,  1247,  1260,  1259,  1273,  1272,  1284,
    1289,  1288,  1303,  1302,  1313,  1319,  1319,  1325,  1325,  1331,
    1331,  1337,  1337,  1343,  1351,  1351,  1357,  1357,  1363,  1363,
    1369,  1369,  1375,  1379,  1383,  1383,  1389,  1389,  1395,  1395,
    1401,  1401,  1407,  1407,  1413,  1413,  1419,  1419,  1425,  1425,
    1431,  1435,  1439,  1443,  1443,  1452,  1456,  1460,  1464,  1468,
    1472,  1476,  1480,  1484,  1488,  1492,  1496,  1500,  1504,  1505,
    1504,  1510,  1511,  1510,  1516,  1520,  1524,  1528,  1532,  1536,
    1543,  1547,  1551,  1555,  1562,  1566,  1573,  1573,  1580,  1581,
    1582,  1583,  1584,  1585,  1586,  1587,  1588,  1589,  1592,  1593,
    1594,  1595,  1596,  1597,  1598,  1599,  1600,  1601,  1602,  1605,
    1606,  1609,  1610,  1613,  1614,  1617,  1618,  1623,  1624,  1625,
    1626,  1629,  1630,  1633,  1634,  1637,  1638,  1641,  1642,  1645,
    1646,  1647,  1656,  1657,  1657,  1661,  1661,  1663,  1673,  1673,
    1675,  1676,  1676,  1681,  1681,  1683,  1683,  1685,  1689,  1689,
    1691,  1691,  1693,  1706,  1706,  1708,  1709,  1709,  1718,  1718,
    1720,  1720,  1722,  1722,  1724,  1732,  1732,  1734,  1734,  1736,
    1736,  1738,  1746,  1746,  1748,  1748,  1750,  1759,  1758,  1765,
    1765,  1767,  1775,  1775,  1777,  1777,  1779,  1787,  1787,  1789,
    1789,  1791,  1794,  1794,  1796,  1800,  1800,  1802,  1802,  1804,
    1815,  1815,  1815,  1815,  1815,  1815,  1815,  1818,  1818,  1818,
    1818,  1818,  1818,  1822,  1822,  1822,  1822,  1822,  1822,  1826,
    1826,  1826,  1826,  1826,  1826,  1830,  1830,  1830,  1830,  1830,
    1834,  1834,  1834,  1834,  1834,  1840,  1841,  1844,  1845,  1848,
    1855,  1856,  1859,  1864,  1871,  1871,  1871,  1871,  1871,  1871,
    1871
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
  "KW_FREWRITE", "KW_SREWRITE", "KW_CONTINUE", "KW_SEARCH", "KW_SET",
  "KW_SHOW", "KW_ON", "KW_OFF", "KW_TRACE", "KW_BODY", "KW_BUILTIN",
  "KW_WHOLE", "KW_SELECT", "KW_DESELECT", "KW_CONDITION",
  "KW_SUBSTITUTION", "KW_PRINT", "KW_GRAPH", "KW_MIXFIX", "KW_FLAT",
  "KW_ATTRIBUTE", "KW_NEWLINE", "KW_WITH", "KW_PARENS", "KW_ALIASES",
  "KW_GC", "KW_TIME", "KW_STATS", "KW_TIMING", "KW_CMD", "KW_BREAKDOWN",
  "KW_BREAK", "KW_PATH", "KW_MODULE", "KW_MODULES", "KW_VIEWS", "KW_ALL",
  "KW_SORTS", "KW_OPS2", "KW_VARS", "KW_MBS", "KW_EQS", "KW_RLS",
  "KW_SUMMARY", "KW_KINDS", "KW_ADVISE", "KW_VERBOSE", "KW_DO", "KW_CLEAR",
  "KW_PROTECT", "KW_EXTEND", "KW_INCLUDE", "KW_EXCLUDE", "KW_CONCEAL",
  "KW_REVEAL", "KW_COMPILE", "KW_COUNT", "KW_DEBUG", "KW_IRREDUNDANT",
  "KW_RESUME", "KW_ABORT", "KW_STEP", "KW_WHERE", "KW_CREDUCE",
  "KW_SREDUCE", "KW_DUMP", "KW_PROFILE", "KW_NUMBER", "KW_RAT", "KW_COLOR",
  "SIMPLE_NUMBER", "KW_PWD", "KW_CD", "KW_PUSHD", "KW_POPD", "KW_LS",
  "KW_LOAD", "KW_QUIT", "KW_EOF", "KW_TEST", "KW_SMT_SEARCH", "KW_ENDM",
  "KW_IMPORT", "KW_ENDV", "KW_SORT", "KW_SUBSORT", "KW_OP", "KW_OPS",
  "KW_MSGS", "KW_VAR", "KW_CLASS", "KW_SUBCLASS", "KW_MB", "KW_CMB",
  "KW_EQ", "KW_CEQ", "KW_RL", "KW_CRL", "KW_IS", "KW_FROM", "KW_ARROW",
  "KW_ARROW2", "KW_PARTIAL", "KW_IF", "':'", "'='", "'('", "')'", "'.'",
  "'<'", "'['", "']'", "','", "'|'", "KW_LABEL", "KW_TO", "KW_COLON2",
  "'+'", "'*'", "'{'", "'}'", "KW_ASSOC", "KW_COMM", "KW_ID", "KW_IDEM",
  "KW_ITER", "KW_LEFT", "KW_RIGHT", "KW_PREC", "KW_GATHER", "KW_METADATA",
  "KW_STRAT", "KW_POLY", "KW_MEMO", "KW_FROZEN", "KW_CTOR", "KW_LATEX",
  "KW_SPECIAL", "KW_CONFIG", "KW_OBJ", "KW_MSG", "KW_DITTO", "KW_FORMAT",
  "KW_ID_HOOK", "KW_OP_HOOK", "KW_TERM_HOOK", "KW_IN", "IDENTIFIER",
  "NUMERIC_ID", "ENDS_IN_DOT", "FORCE_LOOKAHEAD", "$accept", "top", "item",
  "directive", "$@1", "$@2", "$@3", "$@4", "$@5", "moduleExprDot",
  "moduleExpr", "moduleExpr2", "moduleExpr3", "renameExpr", "instantExpr",
  "$@6", "parenExpr", "argList", "renaming", "$@7", "$@8", "renaming2",
  "mappingList", "mapping", "$@9", "$@10", "fromSpec", "$@11",
  "toAttributes", "$@12", "toAttributeList", "toAttribute", "$@13", "$@14",
  "$@15", "view", "$@16", "$@17", "$@18", "viewDecList", "skipStrayArrow",
  "viewDeclaration", "$@19", "sortDot", "viewEndOpMap", "$@20", "$@21",
  "$@22", "endBubble", "parenBubble", "$@23", "module", "$@24", "$@25",
  "dot", "parameters", "parameterList", "parameter", "colon2", "badType",
  "typeDot", "startModule", "decList", "declaration", "$@26", "$@27",
  "$@28", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34", "$@35", "$@36",
  "$@37", "$@38", "$@39", "$@40", "$@41", "$@42", "$@43", "$@44", "$@45",
  "$@46", "$@47", "$@48", "classDef", "cPairList", "cPair", "varNameList",
  "opNameList", "simpleOpName", "domainRangeAttr", "skipStrayColon",
  "dra2", "rangeAttr", "typeAttr", "arrow", "typeList", "typeName", "$@49",
  "sortNames", "attributes", "attributeList", "idKeyword", "attribute",
  "$@50", "$@51", "$@52", "$@53", "$@54", "$@55", "$@56", "identity",
  "idList", "hookList", "hook", "expectedIs", "expectedDot",
  "sortNameList", "subsortList", "$@57", "sortName", "sortNameFrag",
  "$@58", "sortNameFrags", "$@59", "token", "tokenBarDot", "tokenBarColon",
  "sortToken", "endsInDot", "inert", "identifier", "startKeyword",
  "startKeyword2", "midKeyword", "attrKeyword", "attrKeyword2", "command",
  "$@60", "$@61", "$@62", "$@63", "$@64", "$@65", "$@66", "$@67", "$@68",
  "$@69", "$@70", "$@71", "$@72", "$@73", "$@74", "$@75", "$@76", "$@77",
  "$@78", "$@79", "$@80", "$@81", "$@82", "$@83", "$@84", "$@85", "$@86",
  "$@87", "$@88", "$@89", "$@90", "$@91", "$@92", "$@93", "$@94", "$@95",
  "$@96", "$@97", "$@98", "$@99", "printOption", "traceOption", "polarity",
  "select", "exclude", "conceal", "search", "match", "optDebug",
  "optIrredundant", "optNumber", "importMode", "moduleAndTerm", "$@100",
  "inEnd", "$@101", "numberModuleTerm", "$@102", "$@103",
  "numberModuleTerm1", "$@104", "$@105", "numberModuleTerm2", "$@106",
  "$@107", "numbersModuleTerm", "$@108", "$@109", "numbersModuleTerm1",
  "$@110", "$@111", "$@112", "numbersModuleTerm2", "$@113", "$@114",
  "$@115", "numbersModuleTerm3", "$@116", "$@117", "numbersModuleTerm4",
  "$@118", "$@119", "numbersModuleTerm5", "$@120", "$@121",
  "numbersModuleTerm6", "$@122", "$@123", "miscEndBubble", "$@124",
  "initialEndBubble", "$@125", "$@126", "cTokenBarIn", "cTokenBarLeftIn",
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
     375,   376,   377,   378,   379,   380,   381,   382,    58,    61,
      40,    41,    46,    60,    91,    93,    44,   124,   383,   384,
     385,    43,    42,   123,   125,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412,   413,   414,   415
};
# endif

#define YYPACT_NINF -748

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-748)))

#define YYTABLE_NINF -495

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     691,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   436,   471,    29,  -748,    82,   216,
       1,  -748,   -18,    12,    37,    39,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,    85,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,   572,    62,  1073,   -73,   -73,   262,   -73,    98,   227,
     389,    45,   190,   299,   299,   299,   -34,  -748,  -748,  -748,
     159,   299,  -748,  -748,  -748,   202,   -46,  -748,   125,   150,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,   -93,  -748,  -748,  -748,
    -748,   133,  -748,  -748,  -748,  -748,   -73,   -73,   -93,   289,
     292,   303,   310,   -73,   185,   321,  -748,  1073,   418,   262,
    -748,  -748,   257,   309,  -748,  -748,  -748,   238,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   228,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   296,   299,
     299,   299,   299,   299,   299,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,   299,  -748,  -748,  -748,    65,
     306,  -748,  -748,  -748,  -748,  -748,   299,   299,  -748,  -748,
     225,   242,   244,   299,   299,   299,   299,   246,   249,   -93,
     -93,   254,   255,   295,   -93,  -748,  -748,   -93,   -93,   -93,
     -93,   -93,   -93,   -93,   -93,   -93,   -93,  2160,  2160,  -748,
    -748,  -748,  2160,  2160,   259,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,   -73,   262,  -748,   374,  -748,   418,   418,   262,  -748,
     273,   797,   -93,   -64,   -93,   437,   -93,   249,   299,   299,
     274,   276,   280,   294,   304,   316,   319,   299,   320,  -748,
    -748,   324,   325,  -748,  -748,  -748,   327,   328,   334,   335,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,   338,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   495,
    -748,  -748,  -748,  -748,  -748,  -748,   266,   509,   -93,  -748,
    -748,  -748,   262,  -748,  -748,  -748,  -748,   797,   297,   329,
     298,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   340,   341,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,   342,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,   299,  -748,  -748,  -748,  -748,  -748,  1073,   353,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
     262,  -748,   -89,   797,   343,   350,  -748,   -93,   -93,   598,
     -93,   299,  -748,  -748,  -748,   349,    80,  -748,   -82,  -748,
    -748,   629,   675,   -93,  -748,  -748,  -748,   797,  -748,  -748,
    1073,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   351,  -748,  1073,  -748,  -748,  -748,
     797,   250,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,   297,    16,   112,
    -748,   -73,   -93,  -748,  -748,   297,   352,  -748,   866,  -748,
    -748,  -748,  1142,  1142,  1349,  1073,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,   684,   -93,   -73,  2142,   -93,
     353,  -748,  2100,  -748,  1073,  -748,   357,  -748,  1073,  -748,
    -748,  -748,  -748,   354,   146,   162,   118,  -748,   347,   -65,
     177,   180,   367,   368,   195,   200,   275,   288,  -748,  2100,
     361,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   935,
    -748,  -748,  -748,  -748,  -748,  1004,  1211,  -748,  -748,  2100,
     373,   375,   376,   378,   387,   388,   386,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   379,
     380,  -748,  -748,  -748,   391,   383,   393,    -7,  -748,   866,
    -748,  -748,  -748,  -748,  -748,  -748,  1625,  1556,  -748,  -748,
    1418,  -748,  1418,   416,  -748,   404,  1556,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   -73,   -93,  2151,   -93,    22,  2100,
    -748,  -748,   405,  1073,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  1418,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  1694,  -748,  -748,  -748,  -748,  1694,
    1832,   385,  -748,   -93,   420,   -93,   422,   -93,   428,  1418,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,   413,  -748,  2100,
    -748,  1349,  -748,  -748,  2100,  -748,  -748,  -748,  1966,  -748,
    2100,  -748,  -748,   425,  1487,  -748,  -748,   367,  -748,  -748,
     424,  -748,   434,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,   -73,   -93,  -748,   417,   -49,
    1280,   380,   139,  1899,   429,  -748,   245,  -748,  2189,   367,
    -748,  -748,  -748,    23,  -748,  -748,  -748,  1832,  1073,   -93,
     -93,   -93,  -748,  -748,  1763,  -748,  -748,  -748,   416,  -748,
    -748,  2033,  -748,  -748,  -748,  2100,  -748,  -748,  -748,  -748,
    -748,   419,   421,   399,   444,   406,   445,   446,  -748,   448,
    -748,   449,   451,  -748,  -748,  -748,  -748,   455,   717,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
     367,  -748,   -93,  1694,  2100,  -748,    70,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,   277,  -748,  -748,
    -748,   390,  -748,  1899,  -748,  -748,   380,   415,   457,   463,
     470,    10,  -748,   431,   431,   431,   431,   599,  1073,  1073,
    1073,    -3,  -748,   431,  -748,  -748,  2033,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,   -87,   -20,   -13,   -10,   472,   476,
     476,   476,  -748,  -748,    -4,   473,   431,   606,   431,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
      41,   482,    49,   -93,  -748,  -748,  -748,  -748
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   456,   108,   109,    69,   351,   382,   487,   488,   492,
     491,   373,   367,   489,     0,     0,     0,   347,     0,     0,
       0,   493,     0,     0,     0,     0,   353,   355,   349,    12,
      13,    15,    17,    18,    10,    20,    21,   380,   490,    91,
       8,     0,     2,     6,     5,   384,     4,    93,     7,   369,
     371,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     468,     0,     0,     0,     0,     0,     0,   499,   500,   501,
       0,     0,   438,   394,   400,     0,     0,   396,     0,     0,
     398,   404,   406,   408,   410,   412,   414,   416,   418,   423,
     481,   482,   484,   483,   385,   387,     0,   485,   486,   391,
     389,     0,   450,   451,   452,   453,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     1,     0,     0,     0,
     357,   359,   496,     0,   361,   363,   365,   498,   457,   311,
     296,   312,   297,   298,   299,   300,   302,   301,   303,   304,
     305,   306,   307,   308,   309,   310,   289,   288,   315,   318,
     316,   319,   314,   317,   249,   313,   320,   321,   292,   285,
     287,   293,   286,   283,   284,   281,   282,   323,   324,   325,
     326,   327,   328,   329,   330,   331,   339,   332,   333,   334,
     338,   335,   336,   337,   340,   341,   294,   342,   343,   344,
     345,   346,   280,   291,     0,   290,   245,   246,   295,   247,
     248,   322,   574,   565,   575,   572,   573,   576,   567,   570,
     571,   569,   352,   502,   503,   383,   580,   581,   508,   579,
     582,   577,   578,   374,   510,   511,   368,   441,     0,     0,
       0,     0,     0,     0,     0,   476,   477,   478,   470,   472,
     469,   471,   473,   474,   475,     0,   462,   458,   459,     0,
       0,   463,   464,   465,   466,   467,     0,     0,   479,   480,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   402,   403,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    89,
      90,   348,     0,     0,     0,   354,   356,   350,    14,    16,
      19,    11,   381,    92,     9,    94,   523,   370,   525,   526,
     372,     0,     0,   495,     0,   375,     0,     0,     0,   497,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   461,
     460,     0,     0,   455,   437,   444,     0,     0,     0,     0,
     448,   619,   616,   617,   620,   618,   614,   615,   439,   395,
     401,   422,   420,     0,   397,   399,   405,   407,   409,   411,
     413,   415,   417,   419,   424,   609,   613,   386,   606,     0,
     611,   612,   388,   392,   390,   393,    99,     0,     0,   358,
     360,   377,     0,   362,   364,   366,   379,     0,    70,    29,
      31,    32,    34,    33,    35,   566,   505,   507,   568,   504,
     587,   562,   584,   585,   588,   586,   583,   513,   564,   509,
     517,   515,   512,   442,     0,     0,   432,   426,   428,   431,
     429,   425,   436,     0,   434,   433,   454,   446,   449,   445,
     447,     0,   421,   607,   605,   608,   610,     0,   229,   599,
     596,   597,   530,   598,   595,   528,   524,   534,   532,   527,
       0,   376,     0,     0,     0,     0,    37,     0,     0,     0,
       0,     0,   427,   430,   435,     0,     0,   101,     0,   228,
     111,     0,     0,     0,   378,    39,    30,     0,    42,    36,
       0,   506,   563,   593,   591,   518,   594,   592,   589,   590,
     514,   522,   520,   516,     0,   440,     0,    98,   104,   103,
       0,     0,   552,   531,   556,   554,   604,   602,   535,   537,
     603,   600,   601,   529,   541,   539,   533,    71,     0,     0,
      41,     0,     0,   443,   100,   102,     0,    95,     0,   113,
     115,   117,     0,     0,     0,     0,   149,   122,   125,   129,
     132,   136,   139,   143,   110,     0,     0,     0,     0,     0,
     229,    46,     0,    51,     0,    43,    45,    48,     0,    38,
     519,   521,   151,   249,   251,   252,    28,   112,     0,   231,
     231,   231,   231,     0,   250,   253,   254,   255,   233,     0,
       0,   261,   264,   262,   265,   263,   259,   260,   162,     0,
     160,   161,   256,   257,   258,     0,     0,   158,   147,     0,
       0,     0,     0,     0,     0,     0,     0,   557,   553,   561,
     559,   555,   536,   542,   538,   546,   544,   540,    74,   275,
     273,   274,   269,   270,   276,   277,   271,   272,   266,     0,
     238,   239,   267,   268,    56,     0,     0,     0,    40,     0,
     230,    25,    26,    24,    22,    23,     0,     0,   236,   118,
       0,   159,     0,    76,   157,     0,     0,   123,   126,   130,
     133,   137,   140,   144,     0,     0,     0,     0,     0,     0,
     240,    54,     0,     0,    44,    47,    27,    30,    96,    97,
     114,   232,   235,   116,   234,   233,     0,   174,   175,   179,
     105,   165,   120,   164,     0,   177,   178,   146,    75,     0,
     152,     0,   150,     0,     0,     0,     0,     0,     0,     0,
     558,   560,   547,   543,   551,   549,   545,     0,    72,     0,
      79,     0,    73,    49,     0,   177,    52,    50,     0,   119,
       0,   173,   171,   184,   167,   107,   121,   231,   251,   252,
     153,   154,     0,   250,   253,   254,   255,   148,   124,   127,
     131,   134,   138,   141,   145,     0,     0,    81,     0,     0,
       0,   244,     0,     0,    59,   237,     0,   182,     0,   231,
     166,   169,   170,     0,   163,   176,   106,     0,     0,     0,
       0,     0,   548,   550,     0,    84,    87,    80,    76,   242,
     241,     0,    57,    53,   180,     0,   190,   191,   187,   194,
     195,     0,     0,     0,     0,     0,     0,     0,   205,   207,
     206,     0,     0,   210,   211,   212,   217,     0,     0,   192,
     186,   172,   168,   155,   156,   128,   135,   142,    83,    77,
     231,   177,     0,     0,     0,    55,     0,   181,   188,   189,
     196,   197,   213,   201,   203,   208,   214,     0,   199,   183,
     185,   219,    82,     0,    88,    78,   243,     0,     0,     0,
       0,     0,    61,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   223,     0,   218,   193,     0,    62,    63,    67,
      65,    58,    60,   221,     0,     0,     0,     0,     0,   224,
       0,     0,   216,   222,     0,     0,     0,     0,     0,   198,
     220,   202,   204,   209,   215,   225,   226,   227,   200,    85,
       0,     0,     0,     0,    64,    68,    66,    86
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   -35,
    -385,  -748,  -748,  -510,  -509,  -748,  -508,  -748,  -748,  -748,
    -748,  -748,  -748,   -32,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -255,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -181,  -748,  -748,  -748,  -748,  -748,  -748,  -748,   -95,     0,
    -748,  -748,  -748,  -748,  -511,  -748,  -748,   113,  -748,  -701,
    -219,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -161,
    -103,    89,  -558,  -626,  -748,  -748,  -150,  -748,  -747,  -708,
    -552,  -748,  -748,  -748,  -748,  -748,  -194,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -575,  -748,  -245,    78,  -566,
     -45,    33,  -748,  -412,  -710,  -748,  -748,  -748,   -43,  -679,
    -523,  -748,  -748,  -532,  -524,   -48,  -748,  -501,  -493,  -394,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,   -31,   627,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,   -53,  -748,  -748,  -748,  -108,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,    86,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,
    -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -748,  -362,
    -748,   -47,  -748,  -748,  -748,  -111,  -443,  -533,  -748,  -748,
     -51,  -748,   268,  -748,   269,  -233
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    41,    42,    43,   115,   112,   109,   110,   111,   577,
     398,   399,   400,   401,   402,   490,   403,   529,   489,   528,
     646,   565,   566,   567,   644,   774,   682,   735,   803,   846,
     871,   872,   906,   908,   907,    44,    53,   464,   560,   678,
     709,   732,   769,   839,   797,   841,   923,   842,   418,   376,
     114,    46,   117,   386,   690,   448,   476,   477,   510,   701,
     746,    47,   511,   554,   588,   589,   590,   696,   610,   713,
     611,   714,   789,   612,   715,   613,   716,   790,   614,   717,
     615,   718,   791,   616,   719,   665,   609,   711,   750,   751,
     606,   599,   600,   702,   783,   784,   703,   742,   704,   744,
     705,   740,   776,   779,   828,   829,   830,   861,   873,   883,
     874,   875,   876,   877,   885,   894,   881,   882,   480,   651,
     656,   657,   695,   706,   640,   734,   772,   844,   404,   582,
     601,   641,   583,   195,   196,   642,   198,   199,   200,   201,
      48,    96,   108,    54,   106,   107,   311,   312,   316,   317,
     318,    57,   118,   119,    56,   392,   460,   113,    55,   287,
     288,   293,   292,   269,   274,   277,   270,   278,   279,   280,
     281,   282,   283,   284,   285,   286,   268,   441,   327,   471,
      52,   256,   245,   260,    94,    95,    99,    49,    50,    51,
     314,   320,    72,   212,   324,   408,   467,   223,   325,   326,
     419,   469,   470,   500,   531,   532,   307,   387,   388,   456,
     482,   481,   483,   523,   557,   558,   559,   624,   676,   677,
     723,   765,   766,   513,   555,   556,   618,   674,   675,   420,
     468,   213,   322,   323,   214,   225,   421,   502,   458,   525,
     377,   444,   378,   379,   380,   381
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      45,   291,   215,   741,   226,   197,   584,   309,   745,   224,
     194,   310,   462,   297,   652,   653,   654,   561,   602,   602,
     602,   607,   620,   727,   771,   457,   801,   773,   579,   580,
     581,   752,   263,   261,   262,   358,   707,   586,   515,   289,
     267,   661,   485,   782,   909,   587,   508,   661,   272,   604,
     604,   604,   463,   295,   296,   202,   264,   203,   509,   204,
     302,   205,   206,   207,   406,    90,    91,   650,   289,   197,
     739,   308,   224,   101,   305,   602,   -32,   -32,   486,   795,
     290,   602,   602,   664,   910,   116,   246,   247,   248,   249,
     796,   250,   273,   764,   423,   258,   259,   208,   209,   210,
     211,   562,   527,   563,    92,    93,   604,   501,   752,   290,
     337,   911,   604,   604,   102,   626,   886,   584,   912,   514,
     524,   913,   251,   265,   562,   535,   563,   918,   902,   728,
     729,   564,   730,   863,   866,   731,   252,   253,   254,   579,
     580,   581,   745,   725,   103,   891,   693,   697,   586,   698,
     639,   910,   743,   578,   564,   712,   587,   747,   910,    97,
      98,   910,   867,   868,   878,   879,   880,   910,   643,   104,
     869,   105,   924,   227,   359,   360,   870,   658,   753,   364,
     926,   786,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   785,   619,   128,   643,   625,   658,   330,   331,
     332,   333,   334,   335,   390,   309,   309,   602,   607,   755,
     395,   255,   910,   831,   336,   643,   506,   756,   338,   257,
     910,   785,   867,   868,   507,   341,   342,   405,   407,   409,
     869,   422,   346,   347,   348,   349,   870,   382,   604,   266,
     228,   383,   384,   271,   691,   694,   602,   664,   568,   845,
    -279,   536,    90,    91,   694,   753,   569,   275,   389,  -291,
    -291,  -291,   643,   643,   687,   224,   643,   733,   643,   308,
     308,   224,   643,   197,   862,   799,   229,   604,   230,   231,
     232,   233,   276,   800,   461,   643,   755,  -292,  -292,  -292,
     294,   747,   298,   459,   756,   299,   234,   424,   425,   895,
     896,   897,   643,  -293,  -293,  -293,   433,   300,   904,   650,
     643,   785,   650,   301,   724,   643,   303,   768,   -34,   -34,
     -34,   -33,   -33,   -33,   304,   643,   691,   315,   777,   258,
     259,   920,   319,   922,   905,   643,  -290,  -290,  -290,   313,
     643,  -246,  -246,  -246,   643,   224,   643,   328,   329,   197,
     643,   321,   484,   339,   340,   537,   538,   343,   539,   540,
     541,   542,   543,   544,   545,   546,   547,   548,   549,   550,
     551,   552,   491,   492,   344,   503,   345,   351,   350,   643,
     804,   805,   840,   352,   353,   354,   361,   362,   526,   363,
     216,   385,   203,   847,   217,   391,   218,   219,   220,   197,
     643,   235,   393,   394,   478,   396,   426,   643,   427,   447,
     475,   643,   428,   224,   553,   197,  -247,  -247,  -247,   355,
     356,   357,   236,   237,   238,   239,   429,   240,   241,  -248,
    -248,  -248,   208,   221,   222,   211,   430,   571,   463,   197,
     504,   466,   197,    58,   878,   879,   880,   530,   431,   643,
     643,   432,   434,   242,   243,   244,   435,   436,   197,   437,
     438,   621,   197,   478,   627,    59,   439,   440,    60,   643,
     442,   465,   472,   473,   474,   479,    61,    73,   570,    74,
     488,   505,   487,   533,   572,    62,  -278,    63,   649,   659,
     585,    64,   643,   647,   603,   603,   603,   197,    75,   650,
     655,   667,   608,   668,   622,   669,    65,   670,    66,    67,
      68,    69,   671,   672,   673,    70,   197,   757,   679,   681,
     197,   645,   683,   680,   684,   648,    71,    76,    77,    78,
      79,    80,    81,    82,    83,    84,    85,    86,    87,    88,
     708,   710,   598,   598,   736,   767,   216,   759,   203,   761,
     217,   603,   306,   219,   220,   763,   794,   603,   603,   778,
     787,    89,   788,   802,   884,   410,   848,   411,   849,   289,
     850,   412,   413,   414,   851,   853,   854,   852,   855,   856,
     721,   857,   726,   120,   121,   858,   887,   888,   208,   221,
     222,   211,   122,   889,   123,   124,   125,   126,   127,   598,
     890,   585,   893,   914,   898,   598,    39,   415,   416,   417,
     290,   921,   919,   925,   686,   685,   892,   843,   758,   534,
     760,   720,   762,   351,   865,    39,   833,   443,   770,   352,
     353,   354,   605,   832,   860,   197,   903,   449,   628,   411,
     737,   289,   666,   450,   451,   452,   100,   445,   446,     0,
     738,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   754,     0,     0,   355,   356,   357,   375,     0,
       0,   793,     0,     0,     0,     0,     0,     0,     0,   453,
     454,   455,   290,   603,     0,     0,     0,     0,     0,     0,
       0,    -3,     1,     0,   835,   836,   837,     2,     3,     4,
       5,     0,  -494,  -494,     6,     7,     8,     9,    10,    11,
      12,  -494,   792,  -494,  -494,  -494,  -494,  -494,    13,    14,
      15,     0,   603,    16,     0,     0,   493,    17,   411,     0,
     289,    18,   494,   495,   496,     0,     0,     0,     0,   754,
     197,     0,     0,     0,     0,   834,    19,   864,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   410,     0,   411,
       0,   289,    20,   412,   413,   414,     0,     0,   497,   498,
     499,   290,    21,     0,    22,    23,    24,    25,    26,    27,
      28,     0,     0,     0,     0,     0,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    38,     0,     0,     0,   415,
     416,   512,   290,   516,     0,   411,     0,   289,     0,   517,
     518,   519,   493,     0,   411,     0,   289,     0,   494,   617,
     496,    39,     0,     0,     0,     0,     0,     0,   927,     0,
     197,   197,   197,     0,     0,   899,   900,   901,     0,     0,
       0,     0,     0,     0,     0,   520,   521,   522,   290,     0,
       0,     0,   859,     0,   497,   498,   499,   290,     0,     0,
       0,    40,   806,   807,   808,   809,   810,   811,   812,   813,
     814,   815,   816,   817,   818,   819,   820,   821,   822,   823,
     824,   825,   826,   827,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   915,
     916,   917,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   397,     0,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,     0,   192,     0,
     193,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   397,     0,   573,   155,
     156,   157,   574,   159,   160,   575,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,     0,   192,     0,   576,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   591,
     592,   593,   594,   660,   595,    39,     0,   596,   597,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,     0,   192,     0,   193,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   591,   592,
     593,   594,   662,   595,    39,     0,   596,   597,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,     0,   192,     0,   193,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,     0,     0,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   192,     0,   193,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   591,   592,   593,   594,
       0,   595,    39,     0,   596,   597,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,   192,     0,   193,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   591,   592,   593,   594,   663,
     595,     0,     0,   596,   597,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   187,   188,   189,   190,
     191,     0,   192,     0,   193,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   591,   592,   593,   594,   798,   595,
       0,     0,   596,   597,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
       0,   192,     0,   193,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   591,   592,   593,   594,     0,   595,     0,
       0,   596,   597,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,     0,
     192,     0,   193,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     629,     0,   697,   630,   698,   631,     0,   632,     0,     0,
       0,     0,   699,     0,     0,   633,   634,   635,     0,   636,
     637,     0,     0,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,   187,   188,   189,   190,   191,     0,   638,
       0,   700,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   629,
       0,     0,   630,     0,   631,   780,   632,     0,     0,   781,
       0,   699,     0,     0,   633,   634,   635,     0,   636,   637,
       0,     0,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,     0,   638,     0,
     700,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   629,     0,
       0,   630,     0,   631,     0,   632,     0,     0,   688,   692,
       0,     0,     0,   633,   634,   635,     0,   636,   637,     0,
       0,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,     0,   638,     0,   689,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   629,     0,     0,
     630,     0,   631,     0,   632,     0,     0,   688,     0,     0,
       0,     0,   633,   634,   635,     0,   636,   637,     0,     0,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,   188,   189,   190,   191,     0,   638,     0,   689,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   629,     0,     0,   630,
       0,   631,     0,   632,     0,     0,     0,     0,   699,     0,
       0,   633,   634,   635,     0,   636,   637,     0,     0,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   187,
     188,   189,   190,   191,     0,   638,     0,   700,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   629,     0,     0,   630,     0,
     631,     0,   632,     0,     0,     0,     0,     0,     0,     0,
     633,   634,   635,     0,   636,   637,     0,     0,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   638,     0,   838,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,     0,     0,     0,   155,   156,   157,   748,   159,
     160,   749,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   187,   188,   189,
     190,   191,     0,   192,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   629,     0,   697,   630,   698,   631,     0,   632,     0,
       0,     0,     0,   699,     0,     0,   633,   634,   635,     0,
     636,   637,     0,     0,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,     0,
     638,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   629,     0,
       0,   630,     0,   631,     0,   632,     0,     0,     0,   775,
       0,     0,     0,   633,   634,   635,     0,   636,   637,     0,
       0,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   188,   189,   190,   191,     0,   638,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   629,     0,     0,   630,     0,
     631,     0,   632,     0,     0,     0,     0,   699,     0,     0,
     633,   634,   635,     0,   636,   637,     0,     0,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,   187,   188,
     189,   190,   191,     0,   638,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   629,     0,     0,   630,     0,   631,     0,   632,
       0,     0,     0,     0,     0,     0,     0,   633,   634,   635,
       0,   636,   637,     0,     0,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   188,   189,   190,   191,
     410,   638,   411,     0,   289,     0,   412,   413,   414,   493,
       0,   411,     0,   289,     0,   494,   722,   496,   351,     0,
      39,     0,     0,     0,   352,   353,   354,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   415,   416,   623,   290,     0,     0,     0,     0,
       0,   497,   498,   499,   290,     0,     0,     0,     0,     0,
     355,   356,   357,   375,   806,   807,   808,   809,   810,   811,
     812,   813,   814,   815,   816,   817,   818,   819,   820,   821,
     822,   823,   824,   825,   826,   827
};

static const yytype_int16 yycheck[] =
{
       0,    96,    55,   704,    57,    53,   538,   118,   709,    56,
      53,   119,   397,   108,   580,   581,   582,     1,   542,   543,
     544,   544,   555,     1,   734,   387,   773,   735,   538,   538,
     538,   710,    66,    64,    65,   268,   662,   538,   481,   132,
      71,   599,   131,   744,   131,   538,   128,   605,    94,   542,
     543,   544,   141,   106,   107,   128,    90,   130,   140,   132,
     113,   134,   135,   136,   128,    36,    37,   132,   132,   117,
     696,   118,   119,    72,   117,   599,   141,   142,   463,   128,
     173,   605,   606,   606,   171,     0,    41,    42,    43,    44,
     139,    46,   138,   719,   327,    30,    31,   170,   171,   172,
     173,   108,   487,   110,    75,    76,   599,   469,   787,   173,
      45,   131,   605,   606,   132,   558,   863,   649,   131,   481,
     482,   131,    77,   157,   108,   510,   110,   131,   131,   107,
     108,   138,   110,   841,   844,   113,    91,    92,    93,   649,
     649,   649,   843,   676,   132,   135,   657,   124,   649,   126,
     562,   171,   704,   538,   138,   666,   649,   709,   171,    77,
      78,   171,   152,   153,   167,   168,   169,   171,   562,   132,
     160,   132,   131,    75,   269,   270,   166,   589,   710,   274,
     131,   747,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   744,   555,   132,   589,   558,   609,   229,   230,
     231,   232,   233,   234,   312,   316,   317,   731,   731,   710,
     318,   166,   171,   779,   245,   609,   136,   710,   249,    29,
     171,   773,   152,   153,   144,   256,   257,   322,   323,   324,
     160,   326,   263,   264,   265,   266,   166,   288,   731,    80,
      13,   292,   293,    41,   656,   657,   770,   770,   136,   801,
     132,     1,    36,    37,   666,   787,   144,   132,   311,   141,
     142,   143,   656,   657,   649,   312,   660,   679,   662,   316,
     317,   318,   666,   321,   840,   136,    49,   770,    51,    52,
      53,    54,   132,   144,   392,   679,   787,   141,   142,   143,
     157,   843,     3,   388,   787,     3,    69,   328,   329,   874,
     875,   876,   696,   141,   142,   143,   337,     4,   883,   132,
     704,   863,   132,     3,   676,   709,   131,   729,   141,   142,
     143,   141,   142,   143,     3,   719,   738,    18,   740,    30,
      31,   906,    94,   908,   886,   729,   141,   142,   143,    82,
     734,   141,   142,   143,   738,   392,   740,    51,    52,   397,
     744,   123,   460,    47,    48,   105,   106,   132,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   467,   468,   132,   470,   132,   128,   132,   773,
     135,   136,   794,   134,   135,   136,   132,   132,   483,    94,
     128,   132,   130,   805,   132,    21,   134,   135,   136,   447,
     794,    12,   316,   317,   447,   132,   132,   801,   132,   143,
     441,   805,   132,   460,   164,   463,   141,   142,   143,   170,
     171,   172,    33,    34,    35,    36,   132,    38,    39,   141,
     142,   143,   170,   171,   172,   173,   132,   532,   141,   487,
     471,   143,   490,     7,   167,   168,   169,   490,   132,   843,
     844,   132,   132,    64,    65,    66,   132,   132,   506,   132,
     132,   556,   510,   506,   559,    29,   132,   132,    32,   863,
     132,   142,   132,   132,   132,   122,    40,     6,   531,     8,
     130,   132,   139,   132,   132,    49,   132,    51,   141,   128,
     538,    55,   886,   136,   542,   543,   544,   545,    27,   132,
     132,   128,   545,   128,   557,   129,    70,   129,    72,    73,
      74,    75,   125,   125,   128,    79,   564,   132,   139,   128,
     568,   564,   139,   143,   131,   568,    90,    56,    57,    58,
      59,    60,    61,    62,    63,    64,    65,    66,    67,    68,
     124,   137,   542,   543,   139,   132,   128,   127,   130,   127,
     132,   599,   134,   135,   136,   127,   139,   605,   606,   134,
     136,    90,   128,   134,   174,   128,   147,   130,   147,   132,
     171,   134,   135,   136,   130,   130,   130,   171,   130,   130,
     675,   130,   677,    11,    12,   130,   171,   130,   170,   171,
     172,   173,    20,   130,    22,    23,    24,    25,    26,   599,
     130,   649,   171,   131,     5,   605,   130,   170,   171,   172,
     173,     5,   139,   131,   649,   647,   871,   798,   713,   506,
     715,   674,   717,   128,   843,   130,   787,   132,   731,   134,
     135,   136,   543,   783,   828,   683,   881,   128,   560,   130,
     683,   132,   609,   134,   135,   136,    19,   379,   379,    -1,
     695,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   710,    -1,    -1,   170,   171,   172,   173,    -1,
      -1,   766,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   170,
     171,   172,   173,   731,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,    -1,   789,   790,   791,     6,     7,     8,
       9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,   765,    22,    23,    24,    25,    26,    27,    28,
      29,    -1,   770,    32,    -1,    -1,   128,    36,   130,    -1,
     132,    40,   134,   135,   136,    -1,    -1,    -1,    -1,   787,
     788,    -1,    -1,    -1,    -1,   788,    55,   842,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   128,    -1,   130,
      -1,   132,    71,   134,   135,   136,    -1,    -1,   170,   171,
     172,   173,    81,    -1,    83,    84,    85,    86,    87,    88,
      89,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,    -1,    -1,    -1,   170,
     171,   172,   173,   128,    -1,   130,    -1,   132,    -1,   134,
     135,   136,   128,    -1,   130,    -1,   132,    -1,   134,   135,
     136,   130,    -1,    -1,    -1,    -1,    -1,    -1,   923,    -1,
     878,   879,   880,    -1,    -1,   878,   879,   880,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   170,   171,   172,   173,    -1,
      -1,    -1,   135,    -1,   170,   171,   172,   173,    -1,    -1,
      -1,   170,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   899,
     900,   901,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,    -1,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,    -1,
     173,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,    -1,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,    -1,   171,    -1,   173,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,    -1,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,   171,    -1,   173,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,    -1,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,   171,    -1,   173,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,    -1,    -1,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,    -1,   173,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,   129,   130,    -1,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,    -1,   173,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,    -1,    -1,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,    -1,   171,    -1,   173,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
      -1,    -1,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
      -1,   171,    -1,   173,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,    -1,   129,    -1,
      -1,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
     171,    -1,   173,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,   127,    -1,   129,    -1,    -1,
      -1,    -1,   134,    -1,    -1,   137,   138,   139,    -1,   141,
     142,    -1,    -1,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,    -1,   171,
      -1,   173,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      -1,    -1,   125,    -1,   127,   128,   129,    -1,    -1,   132,
      -1,   134,    -1,    -1,   137,   138,   139,    -1,   141,   142,
      -1,    -1,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,   171,    -1,
     173,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    -1,
      -1,   125,    -1,   127,    -1,   129,    -1,    -1,   132,   133,
      -1,    -1,    -1,   137,   138,   139,    -1,   141,   142,    -1,
      -1,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,    -1,   171,    -1,   173,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,    -1,
     125,    -1,   127,    -1,   129,    -1,    -1,   132,    -1,    -1,
      -1,    -1,   137,   138,   139,    -1,   141,   142,    -1,    -1,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,    -1,   171,    -1,   173,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,    -1,    -1,   125,
      -1,   127,    -1,   129,    -1,    -1,    -1,    -1,   134,    -1,
      -1,   137,   138,   139,    -1,   141,   142,    -1,    -1,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,    -1,   171,    -1,   173,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     137,   138,   139,    -1,   141,   142,    -1,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,    -1,   173,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,    -1,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,    -1,   171,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,    -1,   129,    -1,
      -1,    -1,    -1,   134,    -1,    -1,   137,   138,   139,    -1,
     141,   142,    -1,    -1,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,    -1,
     171,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,    -1,
      -1,   125,    -1,   127,    -1,   129,    -1,    -1,    -1,   133,
      -1,    -1,    -1,   137,   138,   139,    -1,   141,   142,    -1,
      -1,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,    -1,   171,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    -1,    -1,   125,    -1,
     127,    -1,   129,    -1,    -1,    -1,    -1,   134,    -1,    -1,
     137,   138,   139,    -1,   141,   142,    -1,    -1,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,    -1,   171,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,    -1,   125,    -1,   127,    -1,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   137,   138,   139,
      -1,   141,   142,    -1,    -1,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     128,   171,   130,    -1,   132,    -1,   134,   135,   136,   128,
      -1,   130,    -1,   132,    -1,   134,   135,   136,   128,    -1,
     130,    -1,    -1,    -1,   134,   135,   136,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   170,   171,   172,   173,    -1,    -1,    -1,    -1,
      -1,   170,   171,   172,   173,    -1,    -1,    -1,    -1,    -1,
     170,   171,   172,   173,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    14,    15,    16,
      17,    18,    19,    27,    28,    29,    32,    36,    40,    55,
      71,    81,    83,    84,    85,    86,    87,    88,    89,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   130,
     170,   176,   177,   178,   210,   224,   226,   236,   315,   362,
     363,   364,   355,   211,   318,   333,   329,   326,     7,    29,
      32,    40,    49,    51,    55,    70,    72,    73,    74,    75,
      79,    90,   367,     6,     8,    27,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    65,    66,    67,    68,    90,
      36,    37,    75,    76,   359,   360,   316,    77,    78,   361,
     359,    72,   132,   132,   132,   132,   319,   320,   317,   181,
     182,   183,   180,   332,   225,   179,     0,   227,   327,   328,
      11,    12,    20,    22,    23,    24,    25,    26,   132,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   171,   173,   303,   308,   309,   310,   311,   312,
     313,   314,   128,   130,   132,   134,   135,   136,   170,   171,
     172,   173,   368,   406,   409,   368,   128,   132,   134,   135,
     136,   171,   172,   372,   406,   410,   368,    75,    13,    49,
      51,    52,    53,    54,    69,    12,    33,    34,    35,    36,
      38,    39,    64,    65,    66,   357,    41,    42,    43,    44,
      46,    77,    91,    92,    93,   166,   356,    29,    30,    31,
     358,   358,   358,    66,    90,   157,    80,   358,   351,   338,
     341,    41,    94,   138,   339,   132,   132,   340,   342,   343,
     344,   345,   346,   347,   348,   349,   350,   334,   335,   132,
     173,   223,   337,   336,   157,   368,   368,   223,     3,     3,
       4,     3,   368,   131,     3,   303,   134,   381,   406,   410,
     372,   321,   322,    82,   365,    18,   323,   324,   325,    94,
     366,   123,   407,   408,   369,   373,   374,   353,    51,    52,
     358,   358,   358,   358,   358,   358,   358,    45,   358,    47,
      48,   358,   358,   132,   132,   132,   358,   358,   358,   358,
     132,   128,   134,   135,   136,   170,   171,   172,   420,   223,
     223,   132,   132,    94,   223,   223,   223,   223,   223,   223,
     223,   223,   223,   223,   223,   173,   224,   415,   417,   418,
     419,   420,   415,   415,   415,   132,   228,   382,   383,   368,
     372,    21,   330,   381,   381,   372,   132,   130,   185,   186,
     187,   188,   189,   191,   303,   223,   128,   223,   370,   223,
     128,   130,   134,   135,   136,   170,   171,   172,   223,   375,
     404,   411,   223,   420,   358,   358,   132,   132,   132,   132,
     132,   132,   132,   358,   132,   132,   132,   132,   132,   132,
     132,   352,   132,   132,   416,   417,   419,   143,   230,   128,
     134,   135,   136,   170,   171,   172,   384,   404,   413,   223,
     331,   372,   185,   141,   212,   142,   143,   371,   405,   376,
     377,   354,   132,   132,   132,   358,   231,   232,   303,   122,
     293,   386,   385,   387,   372,   131,   185,   139,   130,   193,
     190,   223,   223,   128,   134,   135,   136,   170,   171,   172,
     378,   404,   412,   223,   358,   132,   136,   144,   128,   140,
     233,   237,   172,   398,   404,   411,   128,   134,   135,   136,
     170,   171,   172,   388,   404,   414,   223,   185,   194,   192,
     303,   379,   380,   132,   232,   185,     1,   105,   106,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   164,   238,   399,   400,   389,   390,   391,
     213,     1,   108,   110,   138,   196,   197,   198,   136,   144,
     368,   223,   132,   132,   136,   139,   173,   184,   185,   188,
     189,   191,   304,   307,   308,   310,   312,   313,   239,   240,
     241,   124,   125,   126,   127,   129,   132,   133,   224,   266,
     267,   305,   309,   310,   313,   266,   265,   305,   303,   261,
     243,   245,   248,   250,   253,   255,   258,   135,   401,   404,
     412,   223,   368,   172,   392,   404,   411,   223,   293,   122,
     125,   127,   129,   137,   138,   139,   141,   142,   171,   298,
     299,   306,   310,   314,   199,   303,   195,   136,   303,   141,
     132,   294,   294,   294,   294,   132,   295,   296,   298,   128,
     128,   267,   128,   128,   305,   260,   296,   128,   128,   129,
     129,   125,   125,   128,   402,   403,   393,   394,   214,   139,
     143,   128,   201,   139,   131,   198,   184,   185,   132,   173,
     229,   298,   133,   229,   298,   297,   242,   124,   126,   134,
     173,   234,   268,   271,   273,   275,   298,   268,   124,   215,
     137,   262,   229,   244,   246,   249,   251,   254,   256,   259,
     368,   223,   135,   395,   404,   412,   223,     1,   107,   108,
     110,   113,   216,   298,   300,   202,   139,   303,   295,   268,
     276,   234,   272,   275,   274,   234,   235,   275,   136,   139,
     263,   264,   304,   308,   310,   312,   313,   132,   223,   127,
     223,   127,   223,   127,   268,   396,   397,   132,   298,   217,
     265,   299,   301,   274,   200,   133,   277,   298,   134,   278,
     128,   132,   234,   269,   270,   275,   294,   136,   128,   247,
     252,   257,   368,   223,   139,   128,   139,   219,   128,   136,
     144,   273,   134,   203,   135,   136,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   279,   280,
     281,   294,   271,   264,   303,   223,   223,   223,   173,   218,
     298,   220,   222,   215,   302,   275,   204,   298,   147,   147,
     171,   130,   171,   130,   130,   130,   130,   130,   130,   135,
     281,   282,   294,   274,   223,   235,   299,   152,   153,   160,
     166,   205,   206,   283,   285,   286,   287,   288,   167,   168,
     169,   291,   292,   284,   174,   289,   273,   171,   130,   130,
     130,   135,   206,   171,   290,   290,   290,   290,     5,   303,
     303,   303,   131,   292,   290,   275,   207,   209,   208,   131,
     171,   131,   131,   131,   131,   224,   224,   224,   131,   139,
     290,     5,   290,   221,   131,   131,   131,   223
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   175,   176,   176,   177,   177,   177,   177,   179,   178,
     180,   178,   178,   181,   178,   182,   178,   178,   183,   178,
     178,   178,   184,   184,   184,   184,   184,   184,   184,   185,
     185,   186,   186,   187,   187,   187,   188,   190,   189,   191,
     192,   192,   194,   195,   193,   196,   196,   197,   197,   198,
     198,   199,   200,   198,   202,   201,   201,   204,   203,   203,
     205,   205,   206,   207,   206,   208,   206,   209,   206,   211,
     212,   213,   210,   214,   214,   215,   215,   216,   216,   217,
     216,   216,   218,   218,   220,   221,   219,   222,   219,   223,
     223,   225,   224,   227,   228,   226,   229,   229,   230,   230,
     231,   231,   232,   233,   233,   234,   235,   235,   236,   236,
     237,   237,   238,   239,   238,   240,   238,   241,   242,   238,
     238,   238,   243,   244,   238,   245,   246,   247,   238,   248,
     249,   238,   250,   251,   252,   238,   253,   254,   238,   255,
     256,   257,   238,   258,   259,   238,   238,   260,   238,   261,
     238,   238,   262,   262,   263,   263,   264,   265,   265,   266,
     266,   267,   267,   268,   268,   268,   269,   269,   270,   270,
     270,   271,   272,   272,   273,   273,   274,   274,   275,   276,
     275,   277,   277,   278,   278,   279,   279,   280,   280,   280,
     281,   281,   282,   281,   281,   281,   281,   283,   281,   284,
     281,   285,   281,   286,   281,   281,   281,   281,   287,   281,
     281,   281,   281,   281,   288,   281,   281,   281,   289,   289,
     290,   290,   291,   291,   292,   292,   292,   292,   293,   293,
     294,   294,   295,   295,   296,   296,   297,   296,   298,   299,
     300,   299,   302,   301,   301,   303,   303,   303,   303,   303,
     304,   304,   304,   304,   304,   304,   305,   305,   305,   305,
     305,   305,   305,   305,   305,   305,   306,   306,   306,   306,
     306,   306,   306,   306,   306,   306,   306,   306,   307,   307,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     309,   309,   309,   309,   310,   310,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   312,   312,   312,   312,   312,   312,   312,
     313,   313,   313,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   314,   314,   316,   315,   317,
     315,   318,   315,   319,   315,   320,   315,   321,   315,   322,
     315,   323,   315,   324,   315,   325,   315,   326,   315,   327,
     315,   328,   315,   329,   315,   330,   315,   331,   315,   315,
     332,   315,   333,   315,   315,   334,   315,   335,   315,   336,
     315,   337,   315,   315,   338,   315,   339,   315,   340,   315,
     341,   315,   315,   315,   342,   315,   343,   315,   344,   315,
     345,   315,   346,   315,   347,   315,   348,   315,   349,   315,
     315,   315,   315,   350,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   351,   352,
     315,   353,   354,   315,   315,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   315,   355,   315,   356,   356,
     356,   356,   356,   356,   356,   356,   356,   356,   357,   357,
     357,   357,   357,   357,   357,   357,   357,   357,   357,   358,
     358,   359,   359,   360,   360,   361,   361,   362,   362,   362,
     362,   363,   363,   364,   364,   365,   365,   366,   366,   367,
     367,   367,   368,   369,   368,   371,   370,   370,   373,   372,
     372,   374,   372,   376,   375,   377,   375,   375,   379,   378,
     380,   378,   378,   382,   381,   381,   383,   381,   385,   384,
     386,   384,   387,   384,   384,   389,   388,   390,   388,   391,
     388,   388,   393,   392,   394,   392,   392,   396,   395,   397,
     395,   395,   399,   398,   400,   398,   398,   402,   401,   403,
     401,   401,   405,   404,   404,   407,   406,   408,   406,   406,
     409,   409,   409,   409,   409,   409,   409,   410,   410,   410,
     410,   410,   410,   411,   411,   411,   411,   411,   411,   412,
     412,   412,   412,   412,   412,   413,   413,   413,   413,   413,
     414,   414,   414,   414,   414,   415,   415,   416,   416,   417,
     418,   418,   419,   419,   420,   420,   420,   420,   420,   420,
     420
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     1,     1,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       1,     1,     2,     2,     2,     2,     2,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     3,     0,     5,     3,
       3,     1,     0,     0,     5,     1,     1,     3,     1,     4,
       4,     0,     0,     6,     0,     5,     0,     0,     4,     0,
       2,     1,     2,     0,     5,     0,     5,     0,     5,     0,
       0,     0,    12,     2,     0,     1,     0,     4,     5,     0,
       3,     2,     2,     1,     0,     0,     8,     0,     3,     1,
       1,     0,     3,     0,     0,     8,     1,     1,     3,     0,
       3,     1,     3,     1,     1,     1,     2,     1,     1,     1,
       2,     0,     2,     0,     4,     0,     4,     0,     0,     5,
       4,     5,     0,     0,     5,     0,     0,     0,     7,     0,
       0,     5,     0,     0,     0,     7,     0,     0,     5,     0,
       0,     0,     7,     0,     0,     5,     4,     0,     5,     0,
       4,     2,     1,     2,     1,     3,     3,     2,     1,     2,
       1,     1,     1,     3,     1,     1,     1,     0,     2,     1,
       1,     2,     3,     1,     1,     1,     2,     0,     1,     0,
       4,     3,     1,     3,     0,     2,     1,     1,     2,     2,
       1,     1,     0,     3,     1,     1,     2,     0,     5,     0,
       5,     0,     5,     0,     5,     1,     1,     1,     0,     5,
       1,     1,     1,     2,     0,     5,     4,     1,     1,     0,
       2,     1,     2,     1,     2,     3,     3,     3,     1,     0,
       1,     0,     2,     0,     2,     2,     0,     4,     1,     1,
       0,     5,     0,     4,     1,     1,     1,     1,     1,     1,
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
       4,     0,     4,     0,     4,     0,     4,     0,     3,     0,
       3,     0,     3,     0,     3,     0,     5,     0,     6,     4,
       0,     3,     0,     3,     1,     0,     4,     0,     4,     0,
       4,     0,     4,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     3,     3,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       4,     5,     4,     0,     4,     5,     5,     6,     5,     5,
       6,     5,     5,     5,     5,     6,     5,     4,     0,     0,
       7,     0,     0,     8,     4,     5,     5,     5,     4,     5,
       2,     2,     2,     2,     5,     4,     0,     3,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     0,     1,
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
  YYUSE (yytype);
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
#line 235 "surface.yy" /* yacc.c:1646  */
    { YYACCEPT; }
#line 2523 "surface.c" /* yacc.c:1646  */
    break;

  case 3:
#line 237 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			}
#line 2531 "surface.c" /* yacc.c:1646  */
    break;

  case 8:
#line 251 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2537 "surface.c" /* yacc.c:1646  */
    break;

  case 9:
#line 253 "surface.yy" /* yacc.c:1646  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			}
#line 2550 "surface.c" /* yacc.c:1646  */
    break;

  case 10:
#line 261 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2556 "surface.c" /* yacc.c:1646  */
    break;

  case 11:
#line 263 "surface.yy" /* yacc.c:1646  */
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			}
#line 2569 "surface.c" /* yacc.c:1646  */
    break;

  case 12:
#line 272 "surface.yy" /* yacc.c:1646  */
    {
			  cout << directoryManager.getCwd() << '\n';
			}
#line 2577 "surface.c" /* yacc.c:1646  */
    break;

  case 13:
#line 275 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2583 "surface.c" /* yacc.c:1646  */
    break;

  case 14:
#line 277 "surface.yy" /* yacc.c:1646  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			}
#line 2597 "surface.c" /* yacc.c:1646  */
    break;

  case 15:
#line 286 "surface.yy" /* yacc.c:1646  */
    { lexerFileNameMode(); }
#line 2603 "surface.c" /* yacc.c:1646  */
    break;

  case 16:
#line 288 "surface.yy" /* yacc.c:1646  */
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			}
#line 2617 "surface.c" /* yacc.c:1646  */
    break;

  case 17:
#line 298 "surface.yy" /* yacc.c:1646  */
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
#line 2632 "surface.c" /* yacc.c:1646  */
    break;

  case 18:
#line 308 "surface.yy" /* yacc.c:1646  */
    { lexerStringMode(); }
#line 2638 "surface.c" /* yacc.c:1646  */
    break;

  case 19:
#line 310 "surface.yy" /* yacc.c:1646  */
    {
			  system((string("ls") + (yyvsp[0].yyString)).c_str());
			}
#line 2646 "surface.c" /* yacc.c:1646  */
    break;

  case 20:
#line 314 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			}
#line 2655 "surface.c" /* yacc.c:1646  */
    break;

  case 21:
#line 319 "surface.yy" /* yacc.c:1646  */
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			}
#line 2666 "surface.c" /* yacc.c:1646  */
    break;

  case 22:
#line 352 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        }
#line 2674 "surface.c" /* yacc.c:1646  */
    break;

  case 23:
#line 356 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        }
#line 2682 "surface.c" /* yacc.c:1646  */
    break;

  case 27:
#line 363 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			}
#line 2694 "surface.c" /* yacc.c:1646  */
    break;

  case 28:
#line 371 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  moduleExpressions.push(new ModuleExpression(t));
			}
#line 2705 "surface.c" /* yacc.c:1646  */
    break;

  case 30:
#line 381 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			}
#line 2717 "surface.c" /* yacc.c:1646  */
    break;

  case 35:
#line 397 "surface.yy" /* yacc.c:1646  */
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[0].yyToken)));
                        }
#line 2725 "surface.c" /* yacc.c:1646  */
    break;

  case 36:
#line 404 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			}
#line 2736 "surface.c" /* yacc.c:1646  */
    break;

  case 37:
#line 412 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2742 "surface.c" /* yacc.c:1646  */
    break;

  case 38:
#line 414 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, tokenSequence));
			}
#line 2752 "surface.c" /* yacc.c:1646  */
    break;

  case 39:
#line 421 "surface.yy" /* yacc.c:1646  */
    {}
#line 2758 "surface.c" /* yacc.c:1646  */
    break;

  case 40:
#line 424 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 2764 "surface.c" /* yacc.c:1646  */
    break;

  case 41:
#line 425 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 2770 "surface.c" /* yacc.c:1646  */
    break;

  case 42:
#line 432 "surface.yy" /* yacc.c:1646  */
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			}
#line 2779 "surface.c" /* yacc.c:1646  */
    break;

  case 43:
#line 437 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			}
#line 2787 "surface.c" /* yacc.c:1646  */
    break;

  case 49:
#line 452 "surface.yy" /* yacc.c:1646  */
    {
			  currentRenaming->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 2795 "surface.c" /* yacc.c:1646  */
    break;

  case 50:
#line 456 "surface.yy" /* yacc.c:1646  */
    {
			  currentRenaming->addLabelMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 2803 "surface.c" /* yacc.c:1646  */
    break;

  case 51:
#line 459 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 2809 "surface.c" /* yacc.c:1646  */
    break;

  case 52:
#line 460 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
#line 2815 "surface.c" /* yacc.c:1646  */
    break;

  case 53:
#line 461 "surface.yy" /* yacc.c:1646  */
    {}
#line 2821 "surface.c" /* yacc.c:1646  */
    break;

  case 54:
#line 466 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 2827 "surface.c" /* yacc.c:1646  */
    break;

  case 55:
#line 467 "surface.yy" /* yacc.c:1646  */
    {}
#line 2833 "surface.c" /* yacc.c:1646  */
    break;

  case 56:
#line 468 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
#line 2839 "surface.c" /* yacc.c:1646  */
    break;

  case 57:
#line 474 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 2845 "surface.c" /* yacc.c:1646  */
    break;

  case 58:
#line 475 "surface.yy" /* yacc.c:1646  */
    {}
#line 2851 "surface.c" /* yacc.c:1646  */
    break;

  case 59:
#line 476 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
#line 2857 "surface.c" /* yacc.c:1646  */
    break;

  case 62:
#line 483 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setPrec((yyvsp[0].yyToken)); }
#line 2863 "surface.c" /* yacc.c:1646  */
    break;

  case 63:
#line 484 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2869 "surface.c" /* yacc.c:1646  */
    break;

  case 64:
#line 485 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setGather(tokenSequence); }
#line 2875 "surface.c" /* yacc.c:1646  */
    break;

  case 65:
#line 486 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 2881 "surface.c" /* yacc.c:1646  */
    break;

  case 66:
#line 487 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setFormat(tokenSequence); }
#line 2887 "surface.c" /* yacc.c:1646  */
    break;

  case 67:
#line 488 "surface.yy" /* yacc.c:1646  */
    { lexerLatexMode(); }
#line 2893 "surface.c" /* yacc.c:1646  */
    break;

  case 68:
#line 489 "surface.yy" /* yacc.c:1646  */
    { currentRenaming->setLatexMacro((yyvsp[-1].yyString)); }
#line 2899 "surface.c" /* yacc.c:1646  */
    break;

  case 69:
#line 495 "surface.yy" /* yacc.c:1646  */
    { lexerIdMode(); }
#line 2905 "surface.c" /* yacc.c:1646  */
    break;

  case 70:
#line 497 "surface.yy" /* yacc.c:1646  */
    {
			  fileTable.beginModule((yyvsp[-4].yyToken), (yyvsp[-2].yyToken));
			  interpreter.setCurrentView(new View((yyvsp[-2].yyToken)));
			  currentSyntaxContainer = CV;
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			}
#line 2917 "surface.c" /* yacc.c:1646  */
    break;

  case 71:
#line 505 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			}
#line 2926 "surface.c" /* yacc.c:1646  */
    break;

  case 72:
#line 510 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[-9].yyToken)).code(), CV);
			  CV->finishView();
			}
#line 2937 "surface.c" /* yacc.c:1646  */
    break;

  case 75:
#line 523 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping " << QUOTE("->") << " in variable declaration.");
			}
#line 2946 "surface.c" /* yacc.c:1646  */
    break;

  case 77:
#line 531 "surface.yy" /* yacc.c:1646  */
    {
			  CV->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 2954 "surface.c" /* yacc.c:1646  */
    break;

  case 78:
#line 534 "surface.yy" /* yacc.c:1646  */
    {}
#line 2960 "surface.c" /* yacc.c:1646  */
    break;

  case 79:
#line 535 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON | BAR_TO, 1); }
#line 2966 "surface.c" /* yacc.c:1646  */
    break;

  case 82:
#line 540 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyToken) = (yyvsp[-1].yyToken); }
#line 2972 "surface.c" /* yacc.c:1646  */
    break;

  case 83:
#line 542 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			}
#line 2983 "surface.c" /* yacc.c:1646  */
    break;

  case 84:
#line 551 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			}
#line 2995 "surface.c" /* yacc.c:1646  */
    break;

  case 85:
#line 559 "surface.yy" /* yacc.c:1646  */
    {
			  lexBubble(END_STATEMENT, 1);
			}
#line 3003 "surface.c" /* yacc.c:1646  */
    break;

  case 86:
#line 563 "surface.yy" /* yacc.c:1646  */
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			}
#line 3012 "surface.c" /* yacc.c:1646  */
    break;

  case 87:
#line 568 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1);
			}
#line 3026 "surface.c" /* yacc.c:1646  */
    break;

  case 88:
#line 578 "surface.yy" /* yacc.c:1646  */
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
#line 3050 "surface.c" /* yacc.c:1646  */
    break;

  case 89:
#line 600 "surface.yy" /* yacc.c:1646  */
    {}
#line 3056 "surface.c" /* yacc.c:1646  */
    break;

  case 90:
#line 602 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			}
#line 3067 "surface.c" /* yacc.c:1646  */
    break;

  case 91:
#line 610 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_PAREN, 1); }
#line 3073 "surface.c" /* yacc.c:1646  */
    break;

  case 92:
#line 611 "surface.yy" /* yacc.c:1646  */
    {}
#line 3079 "surface.c" /* yacc.c:1646  */
    break;

  case 93:
#line 617 "surface.yy" /* yacc.c:1646  */
    { lexerIdMode(); }
#line 3085 "surface.c" /* yacc.c:1646  */
    break;

  case 94:
#line 619 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken)));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			}
#line 3095 "surface.c" /* yacc.c:1646  */
    break;

  case 95:
#line 625 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[0].yyToken));
			}
#line 3105 "surface.c" /* yacc.c:1646  */
    break;

  case 96:
#line 632 "surface.yy" /* yacc.c:1646  */
    {}
#line 3111 "surface.c" /* yacc.c:1646  */
    break;

  case 97:
#line 634 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  store(t);
			}
#line 3122 "surface.c" /* yacc.c:1646  */
    break;

  case 98:
#line 642 "surface.yy" /* yacc.c:1646  */
    {}
#line 3128 "surface.c" /* yacc.c:1646  */
    break;

  case 102:
#line 651 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter((yyvsp[-2].yyToken), me);
			}
#line 3138 "surface.c" /* yacc.c:1646  */
    break;

  case 103:
#line 658 "surface.yy" /* yacc.c:1646  */
    {}
#line 3144 "surface.c" /* yacc.c:1646  */
    break;

  case 104:
#line 660 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			}
#line 3154 "surface.c" /* yacc.c:1646  */
    break;

  case 105:
#line 668 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[0].yyToken);  // needed for line number
			}
#line 3165 "surface.c" /* yacc.c:1646  */
    break;

  case 107:
#line 677 "surface.yy" /* yacc.c:1646  */
    {}
#line 3171 "surface.c" /* yacc.c:1646  */
    break;

  case 112:
#line 688 "surface.yy" /* yacc.c:1646  */
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport((yyvsp[-1].yyToken), me);
			}
#line 3181 "surface.c" /* yacc.c:1646  */
    break;

  case 113:
#line 694 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3187 "surface.c" /* yacc.c:1646  */
    break;

  case 114:
#line 695 "surface.yy" /* yacc.c:1646  */
    { CM->addSortDecl(tokenSequence); }
#line 3193 "surface.c" /* yacc.c:1646  */
    break;

  case 115:
#line 697 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3199 "surface.c" /* yacc.c:1646  */
    break;

  case 116:
#line 698 "surface.yy" /* yacc.c:1646  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3205 "surface.c" /* yacc.c:1646  */
    break;

  case 117:
#line 700 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON, 1); }
#line 3211 "surface.c" /* yacc.c:1646  */
    break;

  case 118:
#line 701 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3217 "surface.c" /* yacc.c:1646  */
    break;

  case 119:
#line 702 "surface.yy" /* yacc.c:1646  */
    {}
#line 3223 "surface.c" /* yacc.c:1646  */
    break;

  case 120:
#line 704 "surface.yy" /* yacc.c:1646  */
    {}
#line 3229 "surface.c" /* yacc.c:1646  */
    break;

  case 121:
#line 706 "surface.yy" /* yacc.c:1646  */
    {}
#line 3235 "surface.c" /* yacc.c:1646  */
    break;

  case 122:
#line 708 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1); }
#line 3241 "surface.c" /* yacc.c:1646  */
    break;

  case 123:
#line 709 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3247 "surface.c" /* yacc.c:1646  */
    break;

  case 124:
#line 710 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3253 "surface.c" /* yacc.c:1646  */
    break;

  case 125:
#line 712 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1);  }
#line 3259 "surface.c" /* yacc.c:1646  */
    break;

  case 126:
#line 713 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3265 "surface.c" /* yacc.c:1646  */
    break;

  case 127:
#line 714 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3271 "surface.c" /* yacc.c:1646  */
    break;

  case 128:
#line 715 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3277 "surface.c" /* yacc.c:1646  */
    break;

  case 129:
#line 717 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3283 "surface.c" /* yacc.c:1646  */
    break;

  case 130:
#line 718 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3289 "surface.c" /* yacc.c:1646  */
    break;

  case 131:
#line 719 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3295 "surface.c" /* yacc.c:1646  */
    break;

  case 132:
#line 721 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); }
#line 3301 "surface.c" /* yacc.c:1646  */
    break;

  case 133:
#line 722 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3307 "surface.c" /* yacc.c:1646  */
    break;

  case 134:
#line 723 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3313 "surface.c" /* yacc.c:1646  */
    break;

  case 135:
#line 724 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3319 "surface.c" /* yacc.c:1646  */
    break;

  case 136:
#line 726 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3325 "surface.c" /* yacc.c:1646  */
    break;

  case 137:
#line 727 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3331 "surface.c" /* yacc.c:1646  */
    break;

  case 138:
#line 728 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3337 "surface.c" /* yacc.c:1646  */
    break;

  case 139:
#line 730 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); }
#line 3343 "surface.c" /* yacc.c:1646  */
    break;

  case 140:
#line 731 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); }
#line 3349 "surface.c" /* yacc.c:1646  */
    break;

  case 141:
#line 732 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); }
#line 3355 "surface.c" /* yacc.c:1646  */
    break;

  case 142:
#line 733 "surface.yy" /* yacc.c:1646  */
    { CM->addStatement(lexerBubble); }
#line 3361 "surface.c" /* yacc.c:1646  */
    break;

  case 143:
#line 735 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_COLON, 1); }
#line 3367 "surface.c" /* yacc.c:1646  */
    break;

  case 144:
#line 736 "surface.yy" /* yacc.c:1646  */
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
#line 3373 "surface.c" /* yacc.c:1646  */
    break;

  case 145:
#line 737 "surface.yy" /* yacc.c:1646  */
    { CM->setFlag(SymbolType::MESSAGE); }
#line 3379 "surface.c" /* yacc.c:1646  */
    break;

  case 146:
#line 740 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3387 "surface.c" /* yacc.c:1646  */
    break;

  case 147:
#line 745 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3394 "surface.c" /* yacc.c:1646  */
    break;

  case 148:
#line 748 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3401 "surface.c" /* yacc.c:1646  */
    break;

  case 149:
#line 751 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3407 "surface.c" /* yacc.c:1646  */
    break;

  case 150:
#line 752 "surface.yy" /* yacc.c:1646  */
    { CM->addSubsortDecl(tokenSequence); }
#line 3413 "surface.c" /* yacc.c:1646  */
    break;

  case 151:
#line 755 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			}
#line 3426 "surface.c" /* yacc.c:1646  */
    break;

  case 152:
#line 765 "surface.yy" /* yacc.c:1646  */
    {}
#line 3432 "surface.c" /* yacc.c:1646  */
    break;

  case 153:
#line 766 "surface.yy" /* yacc.c:1646  */
    {}
#line 3438 "surface.c" /* yacc.c:1646  */
    break;

  case 156:
#line 774 "surface.yy" /* yacc.c:1646  */
    {
			}
#line 3445 "surface.c" /* yacc.c:1646  */
    break;

  case 157:
#line 778 "surface.yy" /* yacc.c:1646  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3451 "surface.c" /* yacc.c:1646  */
    break;

  case 158:
#line 779 "surface.yy" /* yacc.c:1646  */
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); }
#line 3457 "surface.c" /* yacc.c:1646  */
    break;

  case 161:
#line 786 "surface.yy" /* yacc.c:1646  */
    { singleton[0] = (yyvsp[0].yyToken); CM->addOpDecl(singleton); }
#line 3463 "surface.c" /* yacc.c:1646  */
    break;

  case 162:
#line 787 "surface.yy" /* yacc.c:1646  */
    { CM->addOpDecl(lexerBubble); }
#line 3469 "surface.c" /* yacc.c:1646  */
    break;

  case 165:
#line 793 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			}
#line 3478 "surface.c" /* yacc.c:1646  */
    break;

  case 166:
#line 800 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
				       ": skipping stray " << QUOTE(":") << " in operator declaration.");

			}
#line 3488 "surface.c" /* yacc.c:1646  */
    break;

  case 169:
#line 810 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3497 "surface.c" /* yacc.c:1646  */
    break;

  case 170:
#line 815 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
#line 3506 "surface.c" /* yacc.c:1646  */
    break;

  case 171:
#line 822 "surface.yy" /* yacc.c:1646  */
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			}
#line 3515 "surface.c" /* yacc.c:1646  */
    break;

  case 173:
#line 829 "surface.yy" /* yacc.c:1646  */
    {}
#line 3521 "surface.c" /* yacc.c:1646  */
    break;

  case 174:
#line 832 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 3527 "surface.c" /* yacc.c:1646  */
    break;

  case 175:
#line 833 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 3533 "surface.c" /* yacc.c:1646  */
    break;

  case 178:
#line 841 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			}
#line 3542 "surface.c" /* yacc.c:1646  */
    break;

  case 179:
#line 845 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3548 "surface.c" /* yacc.c:1646  */
    break;

  case 180:
#line 847 "surface.yy" /* yacc.c:1646  */
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
#line 3556 "surface.c" /* yacc.c:1646  */
    break;

  case 181:
#line 852 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3562 "surface.c" /* yacc.c:1646  */
    break;

  case 182:
#line 853 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3568 "surface.c" /* yacc.c:1646  */
    break;

  case 183:
#line 856 "surface.yy" /* yacc.c:1646  */
    {}
#line 3574 "surface.c" /* yacc.c:1646  */
    break;

  case 187:
#line 865 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			}
#line 3582 "surface.c" /* yacc.c:1646  */
    break;

  case 188:
#line 869 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			}
#line 3590 "surface.c" /* yacc.c:1646  */
    break;

  case 189:
#line 873 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			}
#line 3598 "surface.c" /* yacc.c:1646  */
    break;

  case 190:
#line 879 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::ASSOC);
			}
#line 3606 "surface.c" /* yacc.c:1646  */
    break;

  case 191:
#line 883 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::COMM);
			}
#line 3614 "surface.c" /* yacc.c:1646  */
    break;

  case 192:
#line 886 "surface.yy" /* yacc.c:1646  */
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); }
#line 3620 "surface.c" /* yacc.c:1646  */
    break;

  case 193:
#line 887 "surface.yy" /* yacc.c:1646  */
    { CM->setIdentity(lexerBubble); }
#line 3626 "surface.c" /* yacc.c:1646  */
    break;

  case 194:
#line 889 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::IDEM);
			}
#line 3634 "surface.c" /* yacc.c:1646  */
    break;

  case 195:
#line 893 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::ITER);
			}
#line 3642 "surface.c" /* yacc.c:1646  */
    break;

  case 196:
#line 896 "surface.yy" /* yacc.c:1646  */
    { CM->setPrec((yyvsp[0].yyToken)); }
#line 3648 "surface.c" /* yacc.c:1646  */
    break;

  case 197:
#line 897 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3654 "surface.c" /* yacc.c:1646  */
    break;

  case 198:
#line 898 "surface.yy" /* yacc.c:1646  */
    { CM->setGather(tokenSequence); }
#line 3660 "surface.c" /* yacc.c:1646  */
    break;

  case 199:
#line 899 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3666 "surface.c" /* yacc.c:1646  */
    break;

  case 200:
#line 900 "surface.yy" /* yacc.c:1646  */
    { CM->setFormat(tokenSequence); }
#line 3672 "surface.c" /* yacc.c:1646  */
    break;

  case 201:
#line 901 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3678 "surface.c" /* yacc.c:1646  */
    break;

  case 202:
#line 902 "surface.yy" /* yacc.c:1646  */
    { CM->setStrat(tokenSequence); }
#line 3684 "surface.c" /* yacc.c:1646  */
    break;

  case 203:
#line 903 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3690 "surface.c" /* yacc.c:1646  */
    break;

  case 204:
#line 904 "surface.yy" /* yacc.c:1646  */
    { CM->setPoly(tokenSequence); }
#line 3696 "surface.c" /* yacc.c:1646  */
    break;

  case 205:
#line 906 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MEMO);
			}
#line 3704 "surface.c" /* yacc.c:1646  */
    break;

  case 206:
#line 910 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::CTOR);
			}
#line 3712 "surface.c" /* yacc.c:1646  */
    break;

  case 207:
#line 914 "surface.yy" /* yacc.c:1646  */
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			}
#line 3721 "surface.c" /* yacc.c:1646  */
    break;

  case 208:
#line 918 "surface.yy" /* yacc.c:1646  */
    { clear(); }
#line 3727 "surface.c" /* yacc.c:1646  */
    break;

  case 209:
#line 919 "surface.yy" /* yacc.c:1646  */
    { CM->setFrozen(tokenSequence); }
#line 3733 "surface.c" /* yacc.c:1646  */
    break;

  case 210:
#line 921 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::CONFIG);
			}
#line 3741 "surface.c" /* yacc.c:1646  */
    break;

  case 211:
#line 925 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::OBJECT);
			}
#line 3749 "surface.c" /* yacc.c:1646  */
    break;

  case 212:
#line 929 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
#line 3757 "surface.c" /* yacc.c:1646  */
    break;

  case 213:
#line 933 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			}
#line 3765 "surface.c" /* yacc.c:1646  */
    break;

  case 214:
#line 936 "surface.yy" /* yacc.c:1646  */
    { lexerLatexMode(); }
#line 3771 "surface.c" /* yacc.c:1646  */
    break;

  case 215:
#line 937 "surface.yy" /* yacc.c:1646  */
    { CM->setLatexMacro((yyvsp[-1].yyString)); }
#line 3777 "surface.c" /* yacc.c:1646  */
    break;

  case 216:
#line 938 "surface.yy" /* yacc.c:1646  */
    {}
#line 3783 "surface.c" /* yacc.c:1646  */
    break;

  case 217:
#line 940 "surface.yy" /* yacc.c:1646  */
    {
			  CM->setFlag(SymbolType::DITTO);
			}
#line 3791 "surface.c" /* yacc.c:1646  */
    break;

  case 220:
#line 953 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3797 "surface.c" /* yacc.c:1646  */
    break;

  case 221:
#line 954 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3803 "surface.c" /* yacc.c:1646  */
    break;

  case 224:
#line 961 "surface.yy" /* yacc.c:1646  */
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[0].yyToken), tokenSequence); }
#line 3809 "surface.c" /* yacc.c:1646  */
    break;

  case 225:
#line 962 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3815 "surface.c" /* yacc.c:1646  */
    break;

  case 226:
#line 963 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3821 "surface.c" /* yacc.c:1646  */
    break;

  case 227:
#line 964 "surface.yy" /* yacc.c:1646  */
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[-1].yyToken), lexerBubble); }
#line 3827 "surface.c" /* yacc.c:1646  */
    break;

  case 228:
#line 970 "surface.yy" /* yacc.c:1646  */
    {}
#line 3833 "surface.c" /* yacc.c:1646  */
    break;

  case 229:
#line 972 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			}
#line 3842 "surface.c" /* yacc.c:1646  */
    break;

  case 230:
#line 978 "surface.yy" /* yacc.c:1646  */
    {}
#line 3848 "surface.c" /* yacc.c:1646  */
    break;

  case 231:
#line 980 "surface.yy" /* yacc.c:1646  */
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
#line 3856 "surface.c" /* yacc.c:1646  */
    break;

  case 232:
#line 988 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3862 "surface.c" /* yacc.c:1646  */
    break;

  case 234:
#line 992 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3868 "surface.c" /* yacc.c:1646  */
    break;

  case 235:
#line 993 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3874 "surface.c" /* yacc.c:1646  */
    break;

  case 236:
#line 994 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3880 "surface.c" /* yacc.c:1646  */
    break;

  case 237:
#line 995 "surface.yy" /* yacc.c:1646  */
    { store((yyvsp[0].yyToken)); }
#line 3886 "surface.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1002 "surface.yy" /* yacc.c:1646  */
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  (yyval.yyToken) = t;
			}
#line 3900 "surface.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1013 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 3906 "surface.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1014 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 3912 "surface.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1015 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 3918 "surface.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1018 "surface.yy" /* yacc.c:1646  */
    { fragStore((yyvsp[0].yyToken)); }
#line 3924 "surface.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1019 "surface.yy" /* yacc.c:1646  */
    {}
#line 3930 "surface.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1020 "surface.yy" /* yacc.c:1646  */
    {}
#line 3936 "surface.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1100 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 1); }
#line 3942 "surface.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1102 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setCurrentModule(lexerBubble);
			}
#line 3950 "surface.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1105 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 1); }
#line 3956 "surface.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1107 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			}
#line 3965 "surface.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1112 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 3974 "surface.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1117 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			}
#line 3984 "surface.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1124 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 3993 "surface.c" /* yacc.c:1646  */
    break;

  case 354:
#line 1129 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			}
#line 4003 "surface.c" /* yacc.c:1646  */
    break;

  case 355:
#line 1136 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4012 "surface.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1141 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			}
#line 4022 "surface.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1148 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4031 "surface.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1153 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[-3].yyBool));
			}
#line 4041 "surface.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1160 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4051 "surface.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1166 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4061 "surface.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1172 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4072 "surface.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1179 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4082 "surface.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1185 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4093 "surface.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1192 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			}
#line 4103 "surface.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1198 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4113 "surface.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1204 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			}
#line 4123 "surface.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1211 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4132 "surface.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1216 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.check(lexerBubble);
			}
#line 4142 "surface.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1223 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
#line 4153 "surface.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1230 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[-2].yySearchKind));
			}
#line 4163 "surface.c" /* yacc.c:1646  */
    break;

  case 371:
#line 1236 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4173 "surface.c" /* yacc.c:1646  */
    break;

  case 372:
#line 1242 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[-2].yyBool), number);
			}
#line 4183 "surface.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1248 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4193 "surface.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1254 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number);
			}
#line 4203 "surface.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1260 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4213 "surface.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1266 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, (yyvsp[-4].yyBool));
			}
#line 4223 "surface.c" /* yacc.c:1646  */
    break;

  case 377:
#line 1273 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
#line 4233 "surface.c" /* yacc.c:1646  */
    break;

  case 378:
#line 1279 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.getVariants(lexerBubble, number, (yyvsp[-3].yyBool), (yyvsp[-5].yyBool));
			}
#line 4243 "surface.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1285 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.cont((yyvsp[-1].yyInt64), (yyvsp[-3].yyBool));
			}
#line 4251 "surface.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1289 "surface.yy" /* yacc.c:1646  */
    {
			  //
			  //	test is a generic command to call code with a term for development purposes.
			  //
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4263 "surface.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1297 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.test(lexerBubble);
			}
#line 4273 "surface.c" /* yacc.c:1646  */
    break;

  case 382:
#line 1303 "surface.yy" /* yacc.c:1646  */
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
#line 4282 "surface.c" /* yacc.c:1646  */
    break;

  case 383:
#line 1308 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			}
#line 4292 "surface.c" /* yacc.c:1646  */
    break;

  case 384:
#line 1314 "surface.yy" /* yacc.c:1646  */
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			}
#line 4302 "surface.c" /* yacc.c:1646  */
    break;

  case 385:
#line 1319 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4308 "surface.c" /* yacc.c:1646  */
    break;

  case 386:
#line 1321 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[-2].yyBool));
			}
#line 4317 "surface.c" /* yacc.c:1646  */
    break;

  case 387:
#line 1325 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4323 "surface.c" /* yacc.c:1646  */
    break;

  case 388:
#line 1327 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[-2].yyBool));
			}
#line 4332 "surface.c" /* yacc.c:1646  */
    break;

  case 389:
#line 1331 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4338 "surface.c" /* yacc.c:1646  */
    break;

  case 390:
#line 1333 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[-2].yyBool));
			}
#line 4347 "surface.c" /* yacc.c:1646  */
    break;

  case 391:
#line 1337 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4353 "surface.c" /* yacc.c:1646  */
    break;

  case 392:
#line 1339 "surface.yy" /* yacc.c:1646  */
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[-2].yyBool));
			}
#line 4362 "surface.c" /* yacc.c:1646  */
    break;

  case 393:
#line 1344 "surface.yy" /* yacc.c:1646  */
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			}
#line 4371 "surface.c" /* yacc.c:1646  */
    break;

  case 394:
#line 1351 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4377 "surface.c" /* yacc.c:1646  */
    break;

  case 395:
#line 1353 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4386 "surface.c" /* yacc.c:1646  */
    break;

  case 396:
#line 1357 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4392 "surface.c" /* yacc.c:1646  */
    break;

  case 397:
#line 1359 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
#line 4401 "surface.c" /* yacc.c:1646  */
    break;

  case 398:
#line 1363 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4407 "surface.c" /* yacc.c:1646  */
    break;

  case 399:
#line 1365 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			}
#line 4416 "surface.c" /* yacc.c:1646  */
    break;

  case 400:
#line 1369 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4422 "surface.c" /* yacc.c:1646  */
    break;

  case 401:
#line 1371 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			}
#line 4431 "surface.c" /* yacc.c:1646  */
    break;

  case 402:
#line 1376 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showModules(true);
			}
#line 4439 "surface.c" /* yacc.c:1646  */
    break;

  case 403:
#line 1380 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showNamedViews();
			}
#line 4447 "surface.c" /* yacc.c:1646  */
    break;

  case 404:
#line 1383 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4453 "surface.c" /* yacc.c:1646  */
    break;

  case 405:
#line 1385 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			}
#line 4462 "surface.c" /* yacc.c:1646  */
    break;

  case 406:
#line 1389 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4468 "surface.c" /* yacc.c:1646  */
    break;

  case 407:
#line 1391 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			}
#line 4477 "surface.c" /* yacc.c:1646  */
    break;

  case 408:
#line 1395 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4483 "surface.c" /* yacc.c:1646  */
    break;

  case 409:
#line 1397 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			}
#line 4492 "surface.c" /* yacc.c:1646  */
    break;

  case 410:
#line 1401 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4498 "surface.c" /* yacc.c:1646  */
    break;

  case 411:
#line 1403 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			}
#line 4507 "surface.c" /* yacc.c:1646  */
    break;

  case 412:
#line 1407 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4513 "surface.c" /* yacc.c:1646  */
    break;

  case 413:
#line 1409 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			}
#line 4522 "surface.c" /* yacc.c:1646  */
    break;

  case 414:
#line 1413 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4528 "surface.c" /* yacc.c:1646  */
    break;

  case 415:
#line 1415 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			}
#line 4537 "surface.c" /* yacc.c:1646  */
    break;

  case 416:
#line 1419 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4543 "surface.c" /* yacc.c:1646  */
    break;

  case 417:
#line 1421 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			}
#line 4552 "surface.c" /* yacc.c:1646  */
    break;

  case 418:
#line 1425 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4558 "surface.c" /* yacc.c:1646  */
    break;

  case 419:
#line 1427 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			}
#line 4567 "surface.c" /* yacc.c:1646  */
    break;

  case 420:
#line 1432 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchPath((yyvsp[-1].yyInt64));
			}
#line 4575 "surface.c" /* yacc.c:1646  */
    break;

  case 421:
#line 1436 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchPathLabels((yyvsp[-1].yyInt64));
			}
#line 4583 "surface.c" /* yacc.c:1646  */
    break;

  case 422:
#line 1440 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.showSearchGraph();
			}
#line 4591 "surface.c" /* yacc.c:1646  */
    break;

  case 423:
#line 1443 "surface.yy" /* yacc.c:1646  */
    { lexBubble(END_COMMAND, 0); }
#line 4597 "surface.c" /* yacc.c:1646  */
    break;

  case 424:
#line 1445 "surface.yy" /* yacc.c:1646  */
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			}
#line 4606 "surface.c" /* yacc.c:1646  */
    break;

  case 425:
#line 1453 "surface.yy" /* yacc.c:1646  */
    {
			  globalAdvisoryFlag = alwaysAdviseFlag ? true : (yyvsp[-1].yyBool);
			}
#line 4614 "surface.c" /* yacc.c:1646  */
    break;

  case 426:
#line 1457 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 4622 "surface.c" /* yacc.c:1646  */
    break;

  case 427:
#line 1461 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[-1].yyBool));
			}
#line 4630 "surface.c" /* yacc.c:1646  */
    break;

  case 428:
#line 1465 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[-1].yyBool));
			}
#line 4638 "surface.c" /* yacc.c:1646  */
    break;

  case 429:
#line 1469 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[-1].yyBool));
			}
#line 4646 "surface.c" /* yacc.c:1646  */
    break;

  case 430:
#line 1473 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[-1].yyBool));
			}
#line 4654 "surface.c" /* yacc.c:1646  */
    break;

  case 431:
#line 1477 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[-1].yyBool));
			}
#line 4662 "surface.c" /* yacc.c:1646  */
    break;

  case 432:
#line 1481 "surface.yy" /* yacc.c:1646  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 4670 "surface.c" /* yacc.c:1646  */
    break;

  case 433:
#line 1485 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setPrintFlag((yyvsp[-2].yyPrintFlags), (yyvsp[-1].yyBool));
			}
#line 4678 "surface.c" /* yacc.c:1646  */
    break;

  case 434:
#line 1489 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[-1].yyBool));
			}
#line 4686 "surface.c" /* yacc.c:1646  */
    break;

  case 435:
#line 1493 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[-1].yyBool));
			}
#line 4694 "surface.c" /* yacc.c:1646  */
    break;

  case 436:
#line 1497 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag((yyvsp[-2].yyFlags), (yyvsp[-1].yyBool));
			}
#line 4702 "surface.c" /* yacc.c:1646  */
    break;

  case 437:
#line 1501 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[-1].yyBool));
			}
#line 4710 "surface.c" /* yacc.c:1646  */
    break;

  case 438:
#line 1504 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4716 "surface.c" /* yacc.c:1646  */
    break;

  case 439:
#line 1505 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4722 "surface.c" /* yacc.c:1646  */
    break;

  case 440:
#line 1507 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setAutoImport((yyvsp[-5].yyImportMode), (yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 4730 "surface.c" /* yacc.c:1646  */
    break;

  case 441:
#line 1510 "surface.yy" /* yacc.c:1646  */
    { lexerCmdMode(); }
#line 4736 "surface.c" /* yacc.c:1646  */
    break;

  case 442:
#line 1511 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4742 "surface.c" /* yacc.c:1646  */
    break;

  case 443:
#line 1513 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setOmodInclude((yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			}
#line 4750 "surface.c" /* yacc.c:1646  */
    break;

  case 444:
#line 1517 "surface.yy" /* yacc.c:1646  */
    {
			  globalVerboseFlag = (yyvsp[-1].yyBool);
			}
#line 4758 "surface.c" /* yacc.c:1646  */
    break;

  case 445:
#line 1521 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[-1].yyBool));
			}
#line 4766 "surface.c" /* yacc.c:1646  */
    break;

  case 446:
#line 1525 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[-1].yyBool));
			}
#line 4774 "surface.c" /* yacc.c:1646  */
    break;

  case 447:
#line 1529 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[-1].yyBool));
			}
#line 4782 "surface.c" /* yacc.c:1646  */
    break;

  case 448:
#line 1533 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[-1].yyBool));
			}
#line 4790 "surface.c" /* yacc.c:1646  */
    break;

  case 449:
#line 1537 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[-1].yyBool));
			}
#line 4798 "surface.c" /* yacc.c:1646  */
    break;

  case 450:
#line 1544 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			}
#line 4806 "surface.c" /* yacc.c:1646  */
    break;

  case 451:
#line 1548 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			}
#line 4814 "surface.c" /* yacc.c:1646  */
    break;

  case 452:
#line 1552 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
#line 4822 "surface.c" /* yacc.c:1646  */
    break;

  case 453:
#line 1556 "surface.yy" /* yacc.c:1646  */
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			}
#line 4830 "surface.c" /* yacc.c:1646  */
    break;

  case 454:
#line 1563 "surface.yy" /* yacc.c:1646  */
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			}
#line 4838 "surface.c" /* yacc.c:1646  */
    break;

  case 455:
#line 1567 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			}
#line 4846 "surface.c" /* yacc.c:1646  */
    break;

  case 456:
#line 1573 "surface.yy" /* yacc.c:1646  */
    { lexerInitialMode(); }
#line 4852 "surface.c" /* yacc.c:1646  */
    break;

  case 458:
#line 1580 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; }
#line 4858 "surface.c" /* yacc.c:1646  */
    break;

  case 459:
#line 1581 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; }
#line 4864 "surface.c" /* yacc.c:1646  */
    break;

  case 460:
#line 1582 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; }
#line 4870 "surface.c" /* yacc.c:1646  */
    break;

  case 461:
#line 1583 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; }
#line 4876 "surface.c" /* yacc.c:1646  */
    break;

  case 462:
#line 1584 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; }
#line 4882 "surface.c" /* yacc.c:1646  */
    break;

  case 463:
#line 1585 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; }
#line 4888 "surface.c" /* yacc.c:1646  */
    break;

  case 464:
#line 1586 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; }
#line 4894 "surface.c" /* yacc.c:1646  */
    break;

  case 465:
#line 1587 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; }
#line 4900 "surface.c" /* yacc.c:1646  */
    break;

  case 466:
#line 1588 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; }
#line 4906 "surface.c" /* yacc.c:1646  */
    break;

  case 467:
#line 1589 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; }
#line 4912 "surface.c" /* yacc.c:1646  */
    break;

  case 468:
#line 1592 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE; }
#line 4918 "surface.c" /* yacc.c:1646  */
    break;

  case 469:
#line 1593 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; }
#line 4924 "surface.c" /* yacc.c:1646  */
    break;

  case 470:
#line 1594 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; }
#line 4930 "surface.c" /* yacc.c:1646  */
    break;

  case 471:
#line 1595 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; }
#line 4936 "surface.c" /* yacc.c:1646  */
    break;

  case 472:
#line 1596 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; }
#line 4942 "surface.c" /* yacc.c:1646  */
    break;

  case 473:
#line 1597 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_MB; }
#line 4948 "surface.c" /* yacc.c:1646  */
    break;

  case 474:
#line 1598 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; }
#line 4954 "surface.c" /* yacc.c:1646  */
    break;

  case 475:
#line 1599 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_RL; }
#line 4960 "surface.c" /* yacc.c:1646  */
    break;

  case 476:
#line 1600 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; }
#line 4966 "surface.c" /* yacc.c:1646  */
    break;

  case 477:
#line 1601 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; }
#line 4972 "surface.c" /* yacc.c:1646  */
    break;

  case 478:
#line 1602 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; }
#line 4978 "surface.c" /* yacc.c:1646  */
    break;

  case 479:
#line 1605 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 4984 "surface.c" /* yacc.c:1646  */
    break;

  case 480:
#line 1606 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 4990 "surface.c" /* yacc.c:1646  */
    break;

  case 481:
#line 1609 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 4996 "surface.c" /* yacc.c:1646  */
    break;

  case 482:
#line 1610 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5002 "surface.c" /* yacc.c:1646  */
    break;

  case 483:
#line 1613 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5008 "surface.c" /* yacc.c:1646  */
    break;

  case 484:
#line 1614 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5014 "surface.c" /* yacc.c:1646  */
    break;

  case 485:
#line 1617 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5020 "surface.c" /* yacc.c:1646  */
    break;

  case 486:
#line 1618 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5026 "surface.c" /* yacc.c:1646  */
    break;

  case 487:
#line 1623 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::NARROW; }
#line 5032 "surface.c" /* yacc.c:1646  */
    break;

  case 488:
#line 1624 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; }
#line 5038 "surface.c" /* yacc.c:1646  */
    break;

  case 489:
#line 1625 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::SEARCH; }
#line 5044 "surface.c" /* yacc.c:1646  */
    break;

  case 490:
#line 1626 "surface.yy" /* yacc.c:1646  */
    { (yyval.yySearchKind) = Interpreter::SMT_SEARCH; }
#line 5050 "surface.c" /* yacc.c:1646  */
    break;

  case 491:
#line 1629 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5056 "surface.c" /* yacc.c:1646  */
    break;

  case 492:
#line 1630 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5062 "surface.c" /* yacc.c:1646  */
    break;

  case 493:
#line 1633 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5068 "surface.c" /* yacc.c:1646  */
    break;

  case 494:
#line 1634 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5074 "surface.c" /* yacc.c:1646  */
    break;

  case 495:
#line 1637 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = true; }
#line 5080 "surface.c" /* yacc.c:1646  */
    break;

  case 496:
#line 1638 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyBool) = false; }
#line 5086 "surface.c" /* yacc.c:1646  */
    break;

  case 497:
#line 1641 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); }
#line 5092 "surface.c" /* yacc.c:1646  */
    break;

  case 498:
#line 1642 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyInt64) = NONE; }
#line 5098 "surface.c" /* yacc.c:1646  */
    break;

  case 499:
#line 1645 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::PROTECTING; }
#line 5104 "surface.c" /* yacc.c:1646  */
    break;

  case 500:
#line 1646 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::EXTENDING; }
#line 5110 "surface.c" /* yacc.c:1646  */
    break;

  case 501:
#line 1647 "surface.yy" /* yacc.c:1646  */
    { (yyval.yyImportMode) = ImportModule::INCLUDING; }
#line 5116 "surface.c" /* yacc.c:1646  */
    break;

  case 503:
#line 1657 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5122 "surface.c" /* yacc.c:1646  */
    break;

  case 505:
#line 1661 "surface.yy" /* yacc.c:1646  */
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); }
#line 5128 "surface.c" /* yacc.c:1646  */
    break;

  case 508:
#line 1673 "surface.yy" /* yacc.c:1646  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5134 "surface.c" /* yacc.c:1646  */
    break;

  case 511:
#line 1676 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0);  }
#line 5140 "surface.c" /* yacc.c:1646  */
    break;

  case 513:
#line 1681 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5146 "surface.c" /* yacc.c:1646  */
    break;

  case 515:
#line 1683 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5152 "surface.c" /* yacc.c:1646  */
    break;

  case 518:
#line 1689 "surface.yy" /* yacc.c:1646  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5158 "surface.c" /* yacc.c:1646  */
    break;

  case 520:
#line 1691 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5164 "surface.c" /* yacc.c:1646  */
    break;

  case 523:
#line 1706 "surface.yy" /* yacc.c:1646  */
    { lexSave((yyvsp[0].yyToken)); }
#line 5170 "surface.c" /* yacc.c:1646  */
    break;

  case 526:
#line 1709 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5176 "surface.c" /* yacc.c:1646  */
    break;

  case 528:
#line 1718 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5182 "surface.c" /* yacc.c:1646  */
    break;

  case 530:
#line 1720 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5188 "surface.c" /* yacc.c:1646  */
    break;

  case 532:
#line 1722 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5194 "surface.c" /* yacc.c:1646  */
    break;

  case 535:
#line 1732 "surface.yy" /* yacc.c:1646  */
    { number = Token::codeToInt64(lexerBubble[1].code()); }
#line 5200 "surface.c" /* yacc.c:1646  */
    break;

  case 537:
#line 1734 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5206 "surface.c" /* yacc.c:1646  */
    break;

  case 539:
#line 1736 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5212 "surface.c" /* yacc.c:1646  */
    break;

  case 542:
#line 1746 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5218 "surface.c" /* yacc.c:1646  */
    break;

  case 544:
#line 1748 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5224 "surface.c" /* yacc.c:1646  */
    break;

  case 547:
#line 1759 "surface.yy" /* yacc.c:1646  */
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			}
#line 5234 "surface.c" /* yacc.c:1646  */
    break;

  case 549:
#line 1765 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5240 "surface.c" /* yacc.c:1646  */
    break;

  case 552:
#line 1775 "surface.yy" /* yacc.c:1646  */
    { lexContinueSave((yyvsp[0].yyToken)); }
#line 5246 "surface.c" /* yacc.c:1646  */
    break;

  case 554:
#line 1777 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5252 "surface.c" /* yacc.c:1646  */
    break;

  case 557:
#line 1787 "surface.yy" /* yacc.c:1646  */
    { number2 = Token::codeToInt64(lexerBubble[2].code()); }
#line 5258 "surface.c" /* yacc.c:1646  */
    break;

  case 559:
#line 1789 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); }
#line 5264 "surface.c" /* yacc.c:1646  */
    break;

  case 562:
#line 1794 "surface.yy" /* yacc.c:1646  */
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0, 1); }
#line 5270 "surface.c" /* yacc.c:1646  */
    break;

  case 565:
#line 1800 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 1, 1); }
#line 5276 "surface.c" /* yacc.c:1646  */
    break;

  case 567:
#line 1802 "surface.yy" /* yacc.c:1646  */
    { lexBubble((yyvsp[0].yyToken), BAR_COLON | END_COMMAND, 0); }
#line 5282 "surface.c" /* yacc.c:1646  */
    break;

  case 569:
#line 1805 "surface.yy" /* yacc.c:1646  */
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(lexerBubble[0]);
			}
#line 5292 "surface.c" /* yacc.c:1646  */
    break;

  case 607:
#line 1844 "surface.yy" /* yacc.c:1646  */
    {}
#line 5298 "surface.c" /* yacc.c:1646  */
    break;

  case 609:
#line 1849 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			}
#line 5308 "surface.c" /* yacc.c:1646  */
    break;

  case 612:
#line 1860 "surface.yy" /* yacc.c:1646  */
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  interpreter.addSelected(singleton);
			}
#line 5317 "surface.c" /* yacc.c:1646  */
    break;

  case 613:
#line 1865 "surface.yy" /* yacc.c:1646  */
    {
			  interpreter.addSelected(lexerBubble);
			}
#line 5325 "surface.c" /* yacc.c:1646  */
    break;


#line 5329 "surface.c" /* yacc.c:1646  */
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
#line 1895 "surface.yy" /* yacc.c:1906  */


static void
yyerror(UserLevelRewritingContext::ParseResult* /*parseResult*/, char *s)
{
  if (!(UserLevelRewritingContext::interrupted()))
    IssueWarning(LineNumber(lineNumber) << ": " << s);
}

void
cleanUpModuleExpression()
{
  //
  //	Delete pieces of a partly built module expression.
  //
  delete currentRenaming;
  currentRenaming = 0;
  while (!moduleExpressions.empty())
    {
      moduleExpressions.top()->deepSelfDestruct();
      moduleExpressions.pop();
    }
}

void
cleanUpParser()
{
  interpreter.makeClean(lineNumber);
}

void
missingSpace(const Token& token)
{
  IssueWarning(LineNumber(token.lineNumber()) << ": missing space between " <<
	       QUOTE(token) << " and period.");
}
