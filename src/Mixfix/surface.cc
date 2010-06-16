
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 27 "surface.yy"

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
#define YYPARSE_PARAM	parseResult
#define PARSE_RESULT	(*((UserLevelRewritingContext::ParseResult*) parseResult))

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

static void yyerror(char *s);

void cleanUpModuleExpression();
void cleanUpParser();
void missingSpace(const Token& token);


/* Line 189 of yacc.c  */
#line 148 "surface.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
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
     KW_EREWRITE = 274,
     KW_FREWRITE = 275,
     KW_SREWRITE = 276,
     KW_CONTINUE = 277,
     KW_SEARCH = 278,
     KW_SET = 279,
     KW_SHOW = 280,
     KW_ON = 281,
     KW_OFF = 282,
     KW_TRACE = 283,
     KW_BODY = 284,
     KW_BUILTIN = 285,
     KW_WHOLE = 286,
     KW_SELECT = 287,
     KW_DESELECT = 288,
     KW_CONDITION = 289,
     KW_SUBSTITUTION = 290,
     KW_PRINT = 291,
     KW_GRAPH = 292,
     KW_MIXFIX = 293,
     KW_FLAT = 294,
     KW_ATTRIBUTE = 295,
     KW_NEWLINE = 296,
     KW_WITH = 297,
     KW_PARENS = 298,
     KW_ALIASES = 299,
     KW_GC = 300,
     KW_TIME = 301,
     KW_STATS = 302,
     KW_TIMING = 303,
     KW_CMD = 304,
     KW_BREAKDOWN = 305,
     KW_BREAK = 306,
     KW_PATH = 307,
     KW_MODULE = 308,
     KW_MODULES = 309,
     KW_VIEWS = 310,
     KW_ALL = 311,
     KW_SORTS = 312,
     KW_OPS2 = 313,
     KW_VARS = 314,
     KW_MBS = 315,
     KW_EQS = 316,
     KW_RLS = 317,
     KW_SUMMARY = 318,
     KW_KINDS = 319,
     KW_ADVISE = 320,
     KW_VERBOSE = 321,
     KW_DO = 322,
     KW_CLEAR = 323,
     KW_PROTECT = 324,
     KW_EXTEND = 325,
     KW_INCLUDE = 326,
     KW_EXCLUDE = 327,
     KW_CONCEAL = 328,
     KW_REVEAL = 329,
     KW_COMPILE = 330,
     KW_COUNT = 331,
     KW_DEBUG = 332,
     KW_RESUME = 333,
     KW_ABORT = 334,
     KW_STEP = 335,
     KW_WHERE = 336,
     KW_CREDUCE = 337,
     KW_DUMP = 338,
     KW_PROFILE = 339,
     KW_NUMBER = 340,
     KW_RAT = 341,
     KW_COLOR = 342,
     SIMPLE_NUMBER = 343,
     KW_PWD = 344,
     KW_CD = 345,
     KW_PUSHD = 346,
     KW_POPD = 347,
     KW_LS = 348,
     KW_LOAD = 349,
     KW_QUIT = 350,
     KW_EOF = 351,
     KW_ENDM = 352,
     KW_IMPORT = 353,
     KW_ENDV = 354,
     KW_SORT = 355,
     KW_SUBSORT = 356,
     KW_OP = 357,
     KW_OPS = 358,
     KW_MSGS = 359,
     KW_VAR = 360,
     KW_CLASS = 361,
     KW_SUBCLASS = 362,
     KW_MB = 363,
     KW_CMB = 364,
     KW_EQ = 365,
     KW_CEQ = 366,
     KW_RL = 367,
     KW_CRL = 368,
     KW_IS = 369,
     KW_FROM = 370,
     KW_ARROW = 371,
     KW_ARROW2 = 372,
     KW_PARTIAL = 373,
     KW_IF = 374,
     KW_LABEL = 375,
     KW_TO = 376,
     KW_COLON2 = 377,
     KW_ASSOC = 378,
     KW_COMM = 379,
     KW_ID = 380,
     KW_IDEM = 381,
     KW_ITER = 382,
     KW_LEFT = 383,
     KW_RIGHT = 384,
     KW_PREC = 385,
     KW_GATHER = 386,
     KW_METADATA = 387,
     KW_STRAT = 388,
     KW_POLY = 389,
     KW_MEMO = 390,
     KW_FROZEN = 391,
     KW_CTOR = 392,
     KW_LATEX = 393,
     KW_SPECIAL = 394,
     KW_CONFIG = 395,
     KW_OBJ = 396,
     KW_MSG = 397,
     KW_DITTO = 398,
     KW_FORMAT = 399,
     KW_ID_HOOK = 400,
     KW_OP_HOOK = 401,
     KW_TERM_HOOK = 402,
     KW_IN = 403,
     IDENTIFIER = 404,
     NUMERIC_ID = 405,
     ENDS_IN_DOT = 406,
     FORCE_LOOKAHEAD = 407
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 103 "surface.yy"

  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;



/* Line 214 of yacc.c  */
#line 349 "surface.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 264 of yacc.c  */
#line 114 "surface.yy"

int yylex(YYSTYPE* lvalp);


/* Line 264 of yacc.c  */
#line 366 "surface.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  109
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2330

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  167
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  238
/* YYNRULES -- Number of rules.  */
#define YYNRULES  603
/* YYNRULES -- Number of states.  */
#define YYNSTATES  903

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   407

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     122,   123,   134,   133,   128,     2,   124,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   120,     2,
     125,   121,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   126,     2,   127,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   135,   129,   136,     2,     2,     2,     2,
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
     115,   116,   117,   118,   119,   130,   131,   132,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    10,    12,    14,    15,
      19,    20,    24,    26,    27,    31,    32,    36,    38,    39,
      43,    45,    47,    50,    53,    56,    59,    62,    66,    68,
      70,    74,    76,    78,    80,    82,    84,    88,    89,    95,
      99,   103,   105,   106,   107,   113,   115,   117,   121,   123,
     128,   133,   134,   135,   142,   143,   149,   150,   151,   156,
     157,   160,   162,   165,   166,   172,   173,   179,   180,   186,
     187,   188,   189,   202,   205,   206,   211,   216,   217,   221,
     224,   227,   229,   230,   231,   240,   241,   245,   247,   249,
     250,   254,   255,   256,   265,   267,   269,   273,   274,   278,
     280,   284,   286,   288,   290,   293,   295,   297,   299,   302,
     303,   306,   307,   312,   313,   318,   319,   320,   326,   331,
     336,   337,   338,   344,   345,   346,   347,   355,   356,   357,
     363,   364,   365,   366,   374,   375,   376,   382,   383,   384,
     385,   393,   394,   395,   401,   406,   407,   413,   414,   419,
     422,   424,   427,   429,   433,   437,   440,   442,   445,   447,
     449,   451,   455,   457,   459,   461,   463,   465,   468,   472,
     474,   476,   478,   481,   482,   484,   485,   490,   494,   496,
     500,   501,   504,   506,   508,   511,   514,   516,   518,   519,
     523,   525,   527,   530,   531,   537,   538,   544,   545,   551,
     552,   558,   560,   562,   564,   565,   571,   573,   575,   577,
     580,   581,   587,   592,   594,   596,   597,   600,   602,   605,
     607,   610,   614,   618,   622,   624,   625,   627,   628,   631,
     632,   635,   638,   639,   644,   646,   648,   649,   655,   656,
     661,   663,   665,   667,   669,   671,   673,   675,   677,   679,
     681,   683,   685,   687,   689,   691,   693,   695,   697,   699,
     701,   703,   705,   707,   709,   711,   713,   715,   717,   719,
     721,   723,   725,   727,   729,   731,   733,   735,   737,   739,
     741,   743,   745,   747,   749,   751,   753,   755,   757,   759,
     761,   763,   765,   767,   769,   771,   773,   775,   777,   779,
     781,   783,   785,   787,   789,   791,   793,   795,   797,   799,
     801,   803,   805,   807,   809,   811,   813,   815,   817,   819,
     821,   823,   825,   827,   829,   831,   833,   835,   837,   839,
     841,   843,   845,   847,   849,   851,   853,   855,   857,   859,
     861,   863,   865,   867,   868,   872,   873,   877,   878,   882,
     883,   887,   888,   893,   894,   899,   900,   905,   906,   911,
     912,   917,   918,   922,   923,   927,   928,   932,   937,   938,
     942,   944,   945,   950,   951,   956,   957,   962,   963,   968,
     973,   974,   979,   980,   985,   986,   991,   992,   997,  1001,
    1005,  1006,  1011,  1012,  1017,  1018,  1023,  1024,  1029,  1030,
    1035,  1036,  1041,  1042,  1047,  1048,  1053,  1058,  1064,  1069,
    1070,  1075,  1081,  1087,  1094,  1100,  1106,  1113,  1119,  1125,
    1131,  1137,  1144,  1150,  1155,  1156,  1157,  1165,  1166,  1167,
    1176,  1181,  1187,  1193,  1199,  1204,  1210,  1213,  1216,  1219,
    1222,  1228,  1233,  1234,  1238,  1240,  1242,  1245,  1248,  1250,
    1252,  1254,  1256,  1258,  1260,  1261,  1263,  1265,  1267,  1269,
    1271,  1273,  1275,  1277,  1279,  1281,  1283,  1285,  1287,  1289,
    1291,  1293,  1295,  1297,  1299,  1301,  1303,  1305,  1307,  1309,
    1310,  1312,  1313,  1315,  1317,  1319,  1321,  1322,  1326,  1327,
    1331,  1333,  1334,  1338,  1340,  1341,  1345,  1346,  1350,  1351,
    1355,  1357,  1358,  1362,  1363,  1367,  1369,  1370,  1374,  1376,
    1377,  1381,  1382,  1386,  1387,  1391,  1392,  1396,  1398,  1399,
    1403,  1404,  1408,  1409,  1413,  1415,  1416,  1420,  1421,  1425,
    1427,  1428,  1432,  1433,  1437,  1439,  1440,  1444,  1445,  1449,
    1451,  1452,  1456,  1457,  1461,  1463,  1464,  1468,  1470,  1471,
    1475,  1476,  1480,  1482,  1484,  1486,  1488,  1490,  1492,  1494,
    1496,  1498,  1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,
    1516,  1518,  1520,  1522,  1524,  1526,  1528,  1530,  1532,  1534,
    1536,  1538,  1540,  1542,  1544,  1546,  1548,  1550,  1552,  1555,
    1557,  1559,  1561,  1563,  1566,  1568,  1570,  1572,  1574,  1576,
    1578,  1580,  1582,  1584
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     168,     0,    -1,   169,    -1,    -1,   217,    -1,   202,    -1,
     170,    -1,   305,    -1,    -1,   162,   171,     3,    -1,    -1,
      94,   172,     3,    -1,    89,    -1,    -1,    90,   173,     3,
      -1,    -1,    91,   174,     3,    -1,    92,    -1,    -1,    93,
     175,     4,    -1,    95,    -1,    96,    -1,   294,   284,    -1,
     297,   124,    -1,   183,   284,    -1,   180,   284,    -1,   181,
     284,    -1,   177,   133,   176,    -1,   165,    -1,   178,    -1,
     177,   133,   177,    -1,   179,    -1,   180,    -1,   183,    -1,
     181,    -1,   293,    -1,   178,   134,   185,    -1,    -1,   179,
     135,   182,   184,   136,    -1,   122,   177,   123,    -1,   184,
     128,   293,    -1,   293,    -1,    -1,    -1,   122,   186,   188,
     187,   123,    -1,   189,    -1,     1,    -1,   189,   128,   190,
      -1,   190,    -1,   100,   288,   131,   288,    -1,   130,   293,
     131,   293,    -1,    -1,    -1,   102,   191,   193,   131,   192,
     195,    -1,    -1,   120,   194,   264,   263,   265,    -1,    -1,
      -1,   126,   196,   197,   127,    -1,    -1,   197,   198,    -1,
     198,    -1,   144,   163,    -1,    -1,   145,   122,   199,   280,
     123,    -1,    -1,   158,   122,   200,   280,   123,    -1,    -1,
     152,   122,   201,     5,   123,    -1,    -1,    -1,    -1,     8,
     203,   293,   115,   177,   204,   131,   177,   205,   283,   206,
      99,    -1,   206,   207,    -1,    -1,   100,   288,   131,   209,
      -1,   105,   256,   120,   226,    -1,    -1,   102,   208,   210,
      -1,     1,   124,    -1,   288,   284,    -1,   165,    -1,    -1,
      -1,   120,   211,   264,   263,   265,   131,   212,   214,    -1,
      -1,   131,   213,   214,    -1,   124,    -1,   165,    -1,    -1,
     122,   216,   123,    -1,    -1,    -1,   227,   218,   293,   219,
     221,   283,   228,    97,    -1,   124,    -1,   165,    -1,   135,
     222,   136,    -1,    -1,   222,   128,   223,    -1,   223,    -1,
     293,   224,   177,    -1,   132,    -1,   120,    -1,   165,    -1,
     265,   284,    -1,   225,    -1,     6,    -1,     7,    -1,   228,
     229,    -1,    -1,    98,   176,    -1,    -1,   100,   230,   285,
     220,    -1,    -1,   101,   231,   286,   220,    -1,    -1,    -1,
     102,   232,   120,   233,   259,    -1,   103,   257,   120,   259,
      -1,   105,   256,   120,   226,    -1,    -1,    -1,   108,   234,
     120,   235,   214,    -1,    -1,    -1,    -1,   109,   236,   120,
     237,   119,   238,   214,    -1,    -1,    -1,   110,   239,   121,
     240,   214,    -1,    -1,    -1,    -1,   111,   241,   121,   242,
     119,   243,   214,    -1,    -1,    -1,   112,   244,   117,   245,
     214,    -1,    -1,    -1,    -1,   113,   246,   117,   247,   119,
     248,   214,    -1,    -1,    -1,   156,   249,   120,   250,   259,
      -1,   104,   257,   120,   259,    -1,    -1,   106,   293,   251,
     253,   124,    -1,    -1,   107,   252,   286,   220,    -1,     1,
     124,    -1,   129,    -1,   129,   254,    -1,   255,    -1,   254,
     128,   255,    -1,   294,   120,   293,    -1,   256,   295,    -1,
     295,    -1,   257,   258,    -1,   258,    -1,   295,    -1,   215,
      -1,   265,   264,   260,    -1,   261,    -1,   225,    -1,   261,
      -1,   124,    -1,   225,    -1,   263,   262,    -1,   265,   268,
     284,    -1,   225,    -1,   116,    -1,   118,    -1,   264,   265,
      -1,    -1,   288,    -1,    -1,   126,   266,   267,   127,    -1,
     267,   128,   288,    -1,   288,    -1,   126,   269,   127,    -1,
      -1,   269,   271,    -1,   271,    -1,   139,    -1,   142,   139,
      -1,   143,   139,    -1,   137,    -1,   138,    -1,    -1,   270,
     272,   279,    -1,   140,    -1,   141,    -1,   144,   163,    -1,
      -1,   145,   122,   273,   280,   123,    -1,    -1,   158,   122,
     274,   280,   123,    -1,    -1,   147,   122,   275,   280,   123,
      -1,    -1,   148,   122,   276,   280,   123,    -1,   149,    -1,
     151,    -1,   150,    -1,    -1,   150,   122,   277,   280,   123,
      -1,   154,    -1,   155,    -1,   156,    -1,   146,   163,    -1,
      -1,   152,   122,   278,     5,   123,    -1,   153,   122,   281,
     123,    -1,   157,    -1,   166,    -1,    -1,   280,   163,    -1,
     163,    -1,   281,   282,    -1,   282,    -1,   159,   293,    -1,
     159,   293,   215,    -1,   160,   293,   215,    -1,   161,   293,
     215,    -1,   114,    -1,    -1,   124,    -1,    -1,   285,   288,
      -1,    -1,   286,   288,    -1,   286,   125,    -1,    -1,   288,
     287,   285,   125,    -1,   289,    -1,   296,    -1,    -1,   289,
     135,   290,   291,   136,    -1,    -1,   291,   128,   292,   289,
      -1,   289,    -1,   299,    -1,   300,    -1,   302,    -1,   303,
      -1,   124,    -1,   298,    -1,   128,    -1,   131,    -1,   300,
      -1,   302,    -1,   303,    -1,   299,    -1,   300,    -1,   303,
      -1,   124,    -1,   125,    -1,   116,    -1,   118,    -1,   121,
      -1,   117,    -1,   119,    -1,   163,    -1,   300,    -1,   304,
      -1,   121,    -1,   129,    -1,   133,    -1,   134,    -1,   117,
      -1,   119,    -1,   114,    -1,   130,    -1,   131,    -1,   124,
      -1,   165,    -1,   163,    -1,   135,    -1,   136,    -1,   133,
      -1,   134,    -1,   129,    -1,   132,    -1,   130,    -1,   115,
      -1,   114,    -1,   298,    -1,   165,    -1,   128,    -1,   131,
      -1,   156,    -1,   301,    -1,    98,    -1,   100,    -1,   101,
      -1,   102,    -1,   103,    -1,   105,    -1,   104,    -1,   106,
      -1,   107,    -1,   108,    -1,   109,    -1,   110,    -1,   111,
      -1,   112,    -1,   113,    -1,    97,    -1,    99,    -1,   125,
      -1,   120,    -1,   116,    -1,   118,    -1,   121,    -1,   117,
      -1,   119,    -1,   126,    -1,   127,    -1,   304,    -1,   137,
      -1,   138,    -1,   139,    -1,   140,    -1,   141,    -1,   142,
      -1,   143,    -1,   144,    -1,   145,    -1,   147,    -1,   148,
      -1,   149,    -1,   151,    -1,   152,    -1,   153,    -1,   150,
      -1,   146,    -1,   154,    -1,   155,    -1,   157,    -1,   158,
      -1,   159,    -1,   160,    -1,   161,    -1,    -1,    32,   306,
     214,    -1,    -1,    83,   307,   214,    -1,    -1,     9,   308,
     352,    -1,    -1,    82,   309,   352,    -1,    -1,   349,    11,
     310,   352,    -1,    -1,   349,    12,   311,   356,    -1,    -1,
     349,    19,   312,   365,    -1,    -1,   349,    20,   313,   365,
      -1,    -1,   349,    21,   314,   356,    -1,    -1,   347,   315,
     365,    -1,    -1,   348,   316,   356,    -1,    -1,    18,   317,
     356,    -1,   349,    22,   350,   124,    -1,    -1,    13,   318,
     352,    -1,   215,    -1,    -1,    28,   344,   319,   399,    -1,
      -1,    28,   345,   320,   399,    -1,    -1,    51,   344,   321,
     399,    -1,    -1,    36,   346,   322,   399,    -1,    67,    68,
     149,   124,    -1,    -1,    25,     6,   323,   214,    -1,    -1,
      25,    53,   324,   214,    -1,    -1,    25,    56,   325,   214,
      -1,    -1,    25,     8,   326,   214,    -1,    25,    54,   124,
      -1,    25,    55,   124,    -1,    -1,    25,    57,   327,   214,
      -1,    -1,    25,    58,   328,   214,    -1,    -1,    25,    59,
     329,   214,    -1,    -1,    25,    60,   330,   214,    -1,    -1,
      25,    61,   331,   214,    -1,    -1,    25,    62,   332,   214,
      -1,    -1,    25,    63,   333,   214,    -1,    -1,    25,    64,
     334,   214,    -1,    25,    52,    88,   124,    -1,    25,    52,
     130,    88,   124,    -1,    25,    23,    37,   124,    -1,    -1,
      25,    84,   335,   214,    -1,    24,    25,    65,   343,   124,
      -1,    24,    25,    47,   343,   124,    -1,    24,    25,    13,
      47,   343,   124,    -1,    24,    25,    48,   343,   124,    -1,
      24,    25,    50,   343,   124,    -1,    24,    25,    13,    48,
     343,   124,    -1,    24,    25,    49,   343,   124,    -1,    24,
      25,    45,   343,   124,    -1,    24,    36,   341,   343,   124,
      -1,    24,    36,    40,   343,   124,    -1,    24,    36,    40,
      41,   343,   124,    -1,    24,    28,   342,   343,   124,    -1,
      24,    51,   343,   124,    -1,    -1,    -1,    24,   351,   336,
     404,   337,   343,   124,    -1,    -1,    -1,    24,     7,    71,
     338,   404,   339,   343,   124,    -1,    24,    66,   343,   124,
      -1,    24,    68,   149,   343,   124,    -1,    24,    68,    62,
     343,   124,    -1,    24,    75,    76,   343,   124,    -1,    24,
      84,   343,   124,    -1,    24,    68,    84,   343,   124,    -1,
      78,   124,    -1,    79,   124,    -1,    80,   124,    -1,    81,
     124,    -1,    24,    45,    25,   343,   124,    -1,    24,    47,
     343,   124,    -1,    -1,     1,   340,   124,    -1,    38,    -1,
      39,    -1,    42,    44,    -1,    42,    43,    -1,    37,    -1,
      73,    -1,    85,    -1,    86,    -1,    87,    -1,   158,    -1,
      -1,    34,    -1,    31,    -1,    35,    -1,    32,    -1,    60,
      -1,    61,    -1,    62,    -1,    12,    -1,    29,    -1,    30,
      -1,    26,    -1,    27,    -1,    32,    -1,    33,    -1,    72,
      -1,    71,    -1,    73,    -1,    74,    -1,    14,    -1,    15,
      -1,    23,    -1,    17,    -1,    16,    -1,    77,    -1,    -1,
      88,    -1,    -1,    69,    -1,    70,    -1,    71,    -1,   390,
      -1,    -1,   393,   353,   214,    -1,    -1,   120,   355,   214,
      -1,   214,    -1,    -1,   126,   357,   359,    -1,   390,    -1,
      -1,   394,   358,   214,    -1,    -1,   164,   360,   362,    -1,
      -1,   395,   361,   214,    -1,   388,    -1,    -1,   127,   363,
     352,    -1,    -1,   396,   364,   214,    -1,   388,    -1,    -1,
     126,   366,   368,    -1,   390,    -1,    -1,   394,   367,   214,
      -1,    -1,   164,   369,   372,    -1,    -1,   128,   370,   382,
      -1,    -1,   397,   371,   214,    -1,   388,    -1,    -1,   127,
     373,   352,    -1,    -1,   128,   374,   376,    -1,    -1,   398,
     375,   214,    -1,   388,    -1,    -1,   164,   377,   379,    -1,
      -1,   395,   378,   214,    -1,   388,    -1,    -1,   127,   380,
     352,    -1,    -1,   396,   381,   214,    -1,   388,    -1,    -1,
     164,   383,   385,    -1,    -1,   395,   384,   214,    -1,   388,
      -1,    -1,   127,   386,   352,    -1,    -1,   396,   387,   214,
      -1,   388,    -1,    -1,   122,   389,   214,    -1,   214,    -1,
      -1,   122,   391,   214,    -1,    -1,   162,   392,   354,    -1,
     165,    -1,   163,    -1,   164,    -1,   126,    -1,   127,    -1,
     120,    -1,   124,    -1,   128,    -1,   163,    -1,   164,    -1,
     127,    -1,   120,    -1,   124,    -1,   128,    -1,   163,    -1,
     126,    -1,   127,    -1,   162,    -1,   120,    -1,   128,    -1,
     163,    -1,   164,    -1,   126,    -1,   162,    -1,   120,    -1,
     128,    -1,   163,    -1,   126,    -1,   127,    -1,   162,    -1,
     120,    -1,   163,    -1,   164,    -1,   126,    -1,   162,    -1,
     120,    -1,   402,   400,    -1,   401,    -1,   124,    -1,   401,
      -1,   165,    -1,   402,   403,    -1,   403,    -1,   404,    -1,
     215,    -1,   163,    -1,   164,    -1,   126,    -1,   127,    -1,
     162,    -1,   120,    -1,   128,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   232,   232,   234,   239,   240,   241,   242,   248,   248,
     258,   258,   268,   272,   272,   283,   283,   294,   305,   305,
     310,   315,   348,   352,   356,   357,   358,   359,   367,   376,
     377,   387,   388,   391,   392,   393,   400,   409,   409,   418,
     421,   422,   429,   434,   428,   440,   441,   444,   445,   448,
     452,   456,   457,   456,   463,   463,   465,   471,   471,   473,
     476,   477,   480,   481,   481,   483,   483,   485,   485,   492,
     494,   502,   492,   515,   516,   519,   523,   524,   524,   526,
     529,   530,   540,   548,   539,   557,   556,   589,   590,   599,
     599,   606,   608,   606,   621,   622,   631,   632,   635,   636,
     639,   647,   648,   656,   665,   666,   669,   669,   672,   673,
     676,   683,   683,   686,   686,   689,   690,   689,   693,   695,
     697,   698,   697,   701,   702,   703,   701,   706,   707,   706,
     710,   711,   712,   710,   715,   716,   715,   719,   720,   721,
     719,   724,   725,   724,   728,   734,   733,   740,   740,   743,
     754,   755,   758,   759,   762,   767,   768,   771,   772,   775,
     776,   779,   780,   781,   788,   789,   794,   801,   808,   809,
     812,   813,   816,   817,   820,   825,   825,   832,   833,   836,
     837,   840,   841,   844,   848,   852,   858,   862,   866,   866,
     868,   872,   876,   877,   877,   879,   879,   881,   881,   883,
     883,   885,   889,   893,   898,   898,   900,   904,   908,   912,
     916,   916,   918,   919,   929,   930,   933,   934,   937,   938,
     941,   942,   943,   944,   950,   952,   958,   960,   968,   969,
     972,   973,   974,   974,   981,   993,   994,   994,   998,   998,
    1000,  1006,  1006,  1006,  1006,  1006,  1009,  1009,  1009,  1010,
    1010,  1010,  1013,  1013,  1013,  1013,  1014,  1014,  1014,  1014,
    1014,  1014,  1017,  1017,  1017,  1018,  1018,  1018,  1018,  1019,
    1019,  1019,  1019,  1019,  1022,  1022,  1028,  1028,  1028,  1028,
    1028,  1028,  1028,  1028,  1029,  1029,  1032,  1032,  1032,  1032,
    1035,  1035,  1038,  1038,  1038,  1038,  1038,  1038,  1039,  1039,
    1039,  1040,  1040,  1040,  1040,  1040,  1040,  1040,  1040,  1043,
    1043,  1043,  1043,  1043,  1043,  1043,  1046,  1046,  1046,  1049,
    1049,  1049,  1049,  1049,  1049,  1049,  1050,  1050,  1050,  1050,
    1050,  1050,  1051,  1051,  1051,  1051,  1052,  1052,  1052,  1052,
    1053,  1053,  1053,  1080,  1080,  1085,  1085,  1092,  1091,  1104,
    1103,  1116,  1115,  1128,  1127,  1140,  1139,  1153,  1152,  1166,
    1165,  1178,  1177,  1191,  1190,  1203,  1202,  1214,  1219,  1218,
    1229,  1235,  1235,  1241,  1241,  1247,  1247,  1253,  1253,  1259,
    1267,  1267,  1273,  1273,  1279,  1279,  1285,  1285,  1291,  1295,
    1299,  1299,  1305,  1305,  1311,  1311,  1317,  1317,  1323,  1323,
    1329,  1329,  1335,  1335,  1341,  1341,  1347,  1351,  1355,  1359,
    1359,  1368,  1372,  1376,  1380,  1384,  1388,  1392,  1396,  1400,
    1404,  1408,  1412,  1416,  1420,  1421,  1420,  1426,  1427,  1426,
    1432,  1436,  1440,  1444,  1448,  1452,  1459,  1463,  1467,  1471,
    1478,  1482,  1489,  1489,  1496,  1497,  1498,  1499,  1500,  1501,
    1502,  1503,  1504,  1505,  1508,  1509,  1510,  1511,  1512,  1513,
    1514,  1515,  1516,  1517,  1518,  1521,  1522,  1525,  1526,  1529,
    1530,  1533,  1534,  1539,  1540,  1541,  1544,  1545,  1548,  1549,
    1552,  1553,  1556,  1557,  1558,  1567,  1568,  1568,  1572,  1572,
    1574,  1584,  1584,  1586,  1587,  1587,  1592,  1592,  1594,  1594,
    1596,  1600,  1600,  1602,  1602,  1604,  1617,  1617,  1619,  1620,
    1620,  1629,  1629,  1631,  1631,  1633,  1633,  1635,  1643,  1643,
    1645,  1645,  1647,  1647,  1649,  1657,  1657,  1659,  1659,  1661,
    1670,  1669,  1676,  1676,  1678,  1686,  1686,  1688,  1688,  1690,
    1698,  1698,  1700,  1700,  1702,  1705,  1705,  1707,  1711,  1711,
    1713,  1713,  1715,  1726,  1726,  1726,  1726,  1726,  1726,  1726,
    1729,  1729,  1729,  1729,  1729,  1729,  1733,  1733,  1733,  1733,
    1733,  1733,  1737,  1737,  1737,  1737,  1737,  1737,  1741,  1741,
    1741,  1741,  1741,  1745,  1745,  1745,  1745,  1745,  1751,  1752,
    1755,  1756,  1759,  1766,  1767,  1770,  1775,  1782,  1782,  1782,
    1782,  1782,  1782,  1782
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FILE_NAME_STRING",
  "UNINTERPRETED_STRING", "LATEX_STRING", "KW_MOD", "KW_OMOD", "KW_VIEW",
  "KW_PARSE", "KW_NORMALIZE", "KW_REDUCE", "KW_REWRITE", "KW_LOOP",
  "KW_NARROW", "KW_XG_NARROW", "KW_MATCH", "KW_XMATCH", "KW_UNIFY",
  "KW_EREWRITE", "KW_FREWRITE", "KW_SREWRITE", "KW_CONTINUE", "KW_SEARCH",
  "KW_SET", "KW_SHOW", "KW_ON", "KW_OFF", "KW_TRACE", "KW_BODY",
  "KW_BUILTIN", "KW_WHOLE", "KW_SELECT", "KW_DESELECT", "KW_CONDITION",
  "KW_SUBSTITUTION", "KW_PRINT", "KW_GRAPH", "KW_MIXFIX", "KW_FLAT",
  "KW_ATTRIBUTE", "KW_NEWLINE", "KW_WITH", "KW_PARENS", "KW_ALIASES",
  "KW_GC", "KW_TIME", "KW_STATS", "KW_TIMING", "KW_CMD", "KW_BREAKDOWN",
  "KW_BREAK", "KW_PATH", "KW_MODULE", "KW_MODULES", "KW_VIEWS", "KW_ALL",
  "KW_SORTS", "KW_OPS2", "KW_VARS", "KW_MBS", "KW_EQS", "KW_RLS",
  "KW_SUMMARY", "KW_KINDS", "KW_ADVISE", "KW_VERBOSE", "KW_DO", "KW_CLEAR",
  "KW_PROTECT", "KW_EXTEND", "KW_INCLUDE", "KW_EXCLUDE", "KW_CONCEAL",
  "KW_REVEAL", "KW_COMPILE", "KW_COUNT", "KW_DEBUG", "KW_RESUME",
  "KW_ABORT", "KW_STEP", "KW_WHERE", "KW_CREDUCE", "KW_DUMP", "KW_PROFILE",
  "KW_NUMBER", "KW_RAT", "KW_COLOR", "SIMPLE_NUMBER", "KW_PWD", "KW_CD",
  "KW_PUSHD", "KW_POPD", "KW_LS", "KW_LOAD", "KW_QUIT", "KW_EOF",
  "KW_ENDM", "KW_IMPORT", "KW_ENDV", "KW_SORT", "KW_SUBSORT", "KW_OP",
  "KW_OPS", "KW_MSGS", "KW_VAR", "KW_CLASS", "KW_SUBCLASS", "KW_MB",
  "KW_CMB", "KW_EQ", "KW_CEQ", "KW_RL", "KW_CRL", "KW_IS", "KW_FROM",
  "KW_ARROW", "KW_ARROW2", "KW_PARTIAL", "KW_IF", "':'", "'='", "'('",
  "')'", "'.'", "'<'", "'['", "']'", "','", "'|'", "KW_LABEL", "KW_TO",
  "KW_COLON2", "'+'", "'*'", "'{'", "'}'", "KW_ASSOC", "KW_COMM", "KW_ID",
  "KW_IDEM", "KW_ITER", "KW_LEFT", "KW_RIGHT", "KW_PREC", "KW_GATHER",
  "KW_METADATA", "KW_STRAT", "KW_POLY", "KW_MEMO", "KW_FROZEN", "KW_CTOR",
  "KW_LATEX", "KW_SPECIAL", "KW_CONFIG", "KW_OBJ", "KW_MSG", "KW_DITTO",
  "KW_FORMAT", "KW_ID_HOOK", "KW_OP_HOOK", "KW_TERM_HOOK", "KW_IN",
  "IDENTIFIER", "NUMERIC_ID", "ENDS_IN_DOT", "FORCE_LOOKAHEAD", "$accept",
  "top", "item", "directive", "$@1", "$@2", "$@3", "$@4", "$@5",
  "moduleExprDot", "moduleExpr", "moduleExpr2", "moduleExpr3",
  "renameExpr", "instantExpr", "$@6", "parenExpr", "argList", "renaming",
  "$@7", "$@8", "renaming2", "mappingList", "mapping", "$@9", "$@10",
  "fromSpec", "$@11", "toAttributes", "$@12", "toAttributeList",
  "toAttribute", "$@13", "$@14", "$@15", "view", "$@16", "$@17", "$@18",
  "viewDecList", "viewDeclaration", "$@19", "sortDot", "viewEndOpMap",
  "$@20", "$@21", "$@22", "endBubble", "parenBubble", "$@23", "module",
  "$@24", "$@25", "dot", "parameters", "parameterList", "parameter",
  "colon2", "badType", "typeDot", "startModule", "decList", "declaration",
  "$@26", "$@27", "$@28", "$@29", "$@30", "$@31", "$@32", "$@33", "$@34",
  "$@35", "$@36", "$@37", "$@38", "$@39", "$@40", "$@41", "$@42", "$@43",
  "$@44", "$@45", "$@46", "$@47", "$@48", "classDef", "cPairList", "cPair",
  "varNameList", "opNameList", "simpleOpName", "domainRangeAttr", "dra2",
  "rangeAttr", "typeAttr", "arrow", "typeList", "typeName", "$@49",
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
  "$@87", "$@88", "$@89", "$@90", "$@91", "$@92", "$@93", "$@94",
  "printOption", "traceOption", "polarity", "select", "exclude", "conceal",
  "search", "match", "optDebug", "optNumber", "importMode",
  "moduleAndTerm", "$@95", "inEnd", "$@96", "numberModuleTerm", "$@97",
  "$@98", "numberModuleTerm1", "$@99", "$@100", "numberModuleTerm2",
  "$@101", "$@102", "numbersModuleTerm", "$@103", "$@104",
  "numbersModuleTerm1", "$@105", "$@106", "$@107", "numbersModuleTerm2",
  "$@108", "$@109", "$@110", "numbersModuleTerm3", "$@111", "$@112",
  "numbersModuleTerm4", "$@113", "$@114", "numbersModuleTerm5", "$@115",
  "$@116", "numbersModuleTerm6", "$@117", "$@118", "miscEndBubble",
  "$@119", "initialEndBubble", "$@120", "$@121", "cTokenBarIn",
  "cTokenBarLeftIn", "cTokenBarDotNumber", "cTokenBarDotRight",
  "cTokenBarDotCommaNumber", "cTokenBarDotCommaRight", "opSelect",
  "endSelect", "badSelect", "cOpNameList", "cSimpleOpName",
  "cSimpleTokenBarDot", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
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
      58,    61,    40,    41,    46,    60,    91,    93,    44,   124,
     375,   376,   377,    43,    42,   123,   125,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   167,   168,   168,   169,   169,   169,   169,   171,   170,
     172,   170,   170,   173,   170,   174,   170,   170,   175,   170,
     170,   170,   176,   176,   176,   176,   176,   176,   176,   177,
     177,   178,   178,   179,   179,   179,   180,   182,   181,   183,
     184,   184,   186,   187,   185,   188,   188,   189,   189,   190,
     190,   191,   192,   190,   194,   193,   193,   196,   195,   195,
     197,   197,   198,   199,   198,   200,   198,   201,   198,   203,
     204,   205,   202,   206,   206,   207,   207,   208,   207,   207,
     209,   209,   211,   212,   210,   213,   210,   214,   214,   216,
     215,   218,   219,   217,   220,   220,   221,   221,   222,   222,
     223,   224,   224,   225,   226,   226,   227,   227,   228,   228,
     229,   230,   229,   231,   229,   232,   233,   229,   229,   229,
     234,   235,   229,   236,   237,   238,   229,   239,   240,   229,
     241,   242,   243,   229,   244,   245,   229,   246,   247,   248,
     229,   249,   250,   229,   229,   251,   229,   252,   229,   229,
     253,   253,   254,   254,   255,   256,   256,   257,   257,   258,
     258,   259,   259,   259,   260,   260,   260,   261,   262,   262,
     263,   263,   264,   264,   265,   266,   265,   267,   267,   268,
     268,   269,   269,   270,   270,   270,   271,   271,   272,   271,
     271,   271,   271,   273,   271,   274,   271,   275,   271,   276,
     271,   271,   271,   271,   277,   271,   271,   271,   271,   271,
     278,   271,   271,   271,   279,   279,   280,   280,   281,   281,
     282,   282,   282,   282,   283,   283,   284,   284,   285,   285,
     286,   286,   287,   286,   288,   289,   290,   289,   292,   291,
     291,   293,   293,   293,   293,   293,   294,   294,   294,   294,
     294,   294,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   296,   296,   296,   296,   296,   296,   296,   296,
     296,   296,   296,   296,   297,   297,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   299,   299,   299,   299,
     300,   300,   301,   301,   301,   301,   301,   301,   301,   301,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   302,
     302,   302,   302,   302,   302,   302,   303,   303,   303,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   306,   305,   307,   305,   308,   305,   309,
     305,   310,   305,   311,   305,   312,   305,   313,   305,   314,
     305,   315,   305,   316,   305,   317,   305,   305,   318,   305,
     305,   319,   305,   320,   305,   321,   305,   322,   305,   305,
     323,   305,   324,   305,   325,   305,   326,   305,   305,   305,
     327,   305,   328,   305,   329,   305,   330,   305,   331,   305,
     332,   305,   333,   305,   334,   305,   305,   305,   305,   335,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   305,   305,   336,   337,   305,   338,   339,   305,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     305,   305,   340,   305,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   341,   342,   342,   342,   342,   342,   342,
     342,   342,   342,   342,   342,   343,   343,   344,   344,   345,
     345,   346,   346,   347,   347,   347,   348,   348,   349,   349,
     350,   350,   351,   351,   351,   352,   353,   352,   355,   354,
     354,   357,   356,   356,   358,   356,   360,   359,   361,   359,
     359,   363,   362,   364,   362,   362,   366,   365,   365,   367,
     365,   369,   368,   370,   368,   371,   368,   368,   373,   372,
     374,   372,   375,   372,   372,   377,   376,   378,   376,   376,
     380,   379,   381,   379,   379,   383,   382,   384,   382,   382,
     386,   385,   387,   385,   385,   389,   388,   388,   391,   390,
     392,   390,   390,   393,   393,   393,   393,   393,   393,   393,
     394,   394,   394,   394,   394,   394,   395,   395,   395,   395,
     395,   395,   396,   396,   396,   396,   396,   396,   397,   397,
     397,   397,   397,   398,   398,   398,   398,   398,   399,   399,
     400,   400,   401,   402,   402,   403,   403,   404,   404,   404,
     404,   404,   404,   404
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     1,     1,     1,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       1,     1,     2,     2,     2,     2,     2,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     3,     0,     5,     3,
       3,     1,     0,     0,     5,     1,     1,     3,     1,     4,
       4,     0,     0,     6,     0,     5,     0,     0,     4,     0,
       2,     1,     2,     0,     5,     0,     5,     0,     5,     0,
       0,     0,    12,     2,     0,     4,     4,     0,     3,     2,
       2,     1,     0,     0,     8,     0,     3,     1,     1,     0,
       3,     0,     0,     8,     1,     1,     3,     0,     3,     1,
       3,     1,     1,     1,     2,     1,     1,     1,     2,     0,
       2,     0,     4,     0,     4,     0,     0,     5,     4,     4,
       0,     0,     5,     0,     0,     0,     7,     0,     0,     5,
       0,     0,     0,     7,     0,     0,     5,     0,     0,     0,
       7,     0,     0,     5,     4,     0,     5,     0,     4,     2,
       1,     2,     1,     3,     3,     2,     1,     2,     1,     1,
       1,     3,     1,     1,     1,     1,     1,     2,     3,     1,
       1,     1,     2,     0,     1,     0,     4,     3,     1,     3,
       0,     2,     1,     1,     2,     2,     1,     1,     0,     3,
       1,     1,     2,     0,     5,     0,     5,     0,     5,     0,
       5,     1,     1,     1,     0,     5,     1,     1,     1,     2,
       0,     5,     4,     1,     1,     0,     2,     1,     2,     1,
       2,     3,     3,     3,     1,     0,     1,     0,     2,     0,
       2,     2,     0,     4,     1,     1,     0,     5,     0,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     0,     3,     0,     3,     0,     3,     0,
       3,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       4,     0,     3,     0,     3,     0,     3,     4,     0,     3,
       1,     0,     4,     0,     4,     0,     4,     0,     4,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     3,     3,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     4,     5,     4,     0,
       4,     5,     5,     6,     5,     5,     6,     5,     5,     5,
       5,     6,     5,     4,     0,     0,     7,     0,     0,     8,
       4,     5,     5,     5,     4,     5,     2,     2,     2,     2,
       5,     4,     0,     3,     1,     1,     2,     2,     1,     1,
       1,     1,     1,     1,     0,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     0,     1,     1,     1,     1,     0,     3,     0,     3,
       1,     0,     3,     1,     0,     3,     0,     3,     0,     3,
       1,     0,     3,     0,     3,     1,     0,     3,     1,     0,
       3,     0,     3,     0,     3,     0,     3,     1,     0,     3,
       0,     3,     0,     3,     1,     0,     3,     0,     3,     1,
       0,     3,     0,     3,     1,     0,     3,     0,     3,     1,
       0,     3,     0,     3,     1,     0,     3,     1,     0,     3,
       0,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       1,     1,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   442,   106,   107,    69,   347,   368,   473,   474,   477,
     476,   365,   475,     0,     0,     0,   343,     0,     0,     0,
     478,     0,     0,     0,     0,   349,   345,    12,    13,    15,
      17,    18,    10,    20,    21,    89,     8,     0,     2,     6,
       5,   370,     4,    91,     7,   361,   363,     0,     0,     0,
       0,     0,     0,     0,     0,   454,     0,     0,     0,     0,
       0,     0,   482,   483,   484,     0,     0,   424,   380,   386,
       0,     0,   382,     0,     0,   384,   390,   392,   394,   396,
     398,   400,   402,   404,   409,   467,   468,   470,   469,   371,
     373,     0,   471,   472,   377,   375,     0,   436,   437,   438,
     439,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,     0,   351,   353,   355,   357,   359,   481,   443,
     307,   292,   308,   293,   294,   295,   296,   298,   297,   299,
     300,   301,   302,   303,   304,   305,   306,   285,   284,   311,
     314,   312,   315,   310,   313,   245,   309,   316,   317,   288,
     281,   283,   289,   282,   279,   280,   277,   278,   319,   320,
     321,   322,   323,   324,   325,   326,   327,   335,   328,   329,
     330,   334,   331,   332,   333,   336,   337,   290,   338,   339,
     340,   341,   342,   276,   287,     0,   286,   241,   242,   291,
     243,   244,   318,   557,   548,   558,   555,   556,   559,   550,
     553,   554,   552,   348,   485,   486,   369,   563,   564,   491,
     562,   565,   560,   561,   366,   493,   494,   427,     0,     0,
       0,     0,     0,     0,     0,   462,   463,   464,   456,   458,
     455,   457,   459,   460,   461,     0,   448,   444,   445,     0,
       0,   449,   450,   451,   452,   453,     0,     0,   465,   466,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   388,   389,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,   344,     0,     0,     0,   350,   346,    14,    16,    19,
      11,    90,     9,    92,   506,   362,   508,   509,   364,     0,
       0,     0,     0,     0,   480,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   447,   446,     0,     0,   441,   423,
     430,     0,     0,     0,     0,   434,   602,   599,   600,   603,
     601,   597,   598,   425,   381,   387,   408,   406,     0,   383,
     385,   391,   393,   395,   397,   399,   401,   403,   405,   410,
     592,   596,   372,   589,     0,   594,   595,   374,   378,   376,
     379,    97,     0,     0,   352,   354,   356,   358,   360,   367,
       0,    70,    29,    31,    32,    34,    33,    35,   549,   488,
     490,   551,   487,   570,   545,   567,   568,   571,   569,   566,
     496,   547,   492,   500,   498,   495,   428,     0,     0,   418,
     412,   414,   417,   415,   411,   422,     0,   420,   419,   440,
     432,   435,   431,   433,     0,   407,   590,   588,   591,   593,
       0,   225,   582,   579,   580,   513,   581,   578,   511,   507,
     517,   515,   510,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,   413,   416,   421,     0,     0,    99,     0,
     224,   109,     0,     0,     0,    39,    30,     0,    42,    36,
       0,   489,   546,   576,   574,   501,   577,   575,   572,   573,
     497,   505,   503,   499,     0,   426,     0,    96,   102,   101,
       0,     0,   535,   514,   539,   537,   587,   585,   518,   520,
     586,   583,   584,   512,   524,   522,   516,    71,     0,     0,
      41,     0,     0,   429,    98,   100,     0,    93,     0,   111,
     113,   115,     0,     0,     0,     0,   147,   120,   123,   127,
     130,   134,   137,   141,   108,     0,     0,     0,     0,     0,
     225,    46,     0,    51,     0,    43,    45,    48,     0,    38,
     502,   504,   149,   245,   247,   248,    28,   110,     0,   227,
     227,   227,   227,     0,   246,   249,   250,   251,   229,     0,
       0,   257,   260,   258,   261,   259,   255,   256,   160,     0,
     158,   159,   252,   253,   254,     0,     0,   156,   145,     0,
       0,     0,     0,     0,     0,     0,     0,   540,   536,   544,
     542,   538,   519,   525,   521,   529,   527,   523,    74,   271,
     269,   270,   265,   266,   272,   273,   267,   268,   262,     0,
     234,   235,   263,   264,    56,     0,     0,     0,    40,     0,
     226,    25,    26,    24,    22,    23,     0,     0,   232,   116,
       0,   157,     0,     0,   155,     0,     0,   121,   124,   128,
     131,   135,   138,   142,     0,     0,     0,     0,     0,     0,
     236,    54,     0,     0,    44,    47,    27,    30,    94,    95,
     112,   228,   231,   114,   230,   229,     0,   170,   171,   175,
     103,   163,   118,   162,     0,   173,   174,   144,   105,   119,
     227,   150,     0,   148,     0,     0,     0,     0,     0,     0,
       0,   541,   543,   530,   526,   534,   532,   528,     0,    72,
       0,    77,     0,    73,    49,     0,   173,    52,    50,     0,
     117,     0,   169,   167,   180,     0,   104,   247,   248,   151,
     152,     0,   246,   249,   250,   251,   146,   122,   125,   129,
     132,   136,   139,   143,     0,     0,    79,     0,     0,     0,
     240,     0,     0,    59,   233,     0,   178,     0,   227,   165,
     166,   161,   164,   172,     0,     0,     0,     0,     0,   531,
     533,     0,    82,    85,    78,     0,   238,   237,     0,    57,
      53,   176,     0,   186,   187,   183,   190,   191,     0,     0,
       0,     0,     0,     0,     0,   201,   203,   202,     0,     0,
     206,   207,   208,   213,     0,     0,   188,   182,   168,   153,
     154,   126,   133,   140,    81,    75,   227,   173,     0,    76,
       0,    55,     0,   177,   184,   185,   192,   193,   209,   197,
     199,   204,   210,     0,   195,   179,   181,   215,    80,     0,
      86,   239,     0,     0,     0,     0,     0,    61,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   219,     0,   214,
     189,     0,    62,    63,    67,    65,    58,    60,   217,     0,
       0,     0,     0,     0,   220,     0,     0,   212,   218,     0,
       0,     0,     0,     0,   194,   216,   198,   200,   205,   211,
     221,   222,   223,   196,    83,     0,     0,     0,     0,    64,
      68,    66,    84
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    37,    38,    39,   108,   106,   103,   104,   105,   557,
     381,   382,   383,   384,   385,   470,   386,   509,   469,   508,
     626,   545,   546,   547,   624,   753,   662,   716,   780,   822,
     846,   847,   881,   883,   882,    40,    49,   445,   540,   658,
     713,   748,   815,   774,   817,   898,   818,   401,   361,   107,
      42,   110,   371,   670,   431,   457,   458,   490,   681,   689,
      43,   491,   534,   568,   569,   570,   676,   590,   694,   591,
     695,   766,   592,   696,   593,   697,   767,   594,   698,   595,
     699,   768,   596,   700,   645,   589,   692,   729,   730,   586,
     579,   580,   682,   761,   683,   723,   684,   725,   685,   721,
     755,   758,   805,   806,   807,   837,   848,   858,   849,   850,
     851,   852,   860,   869,   856,   857,   461,   631,   636,   637,
     675,   686,   620,   715,   751,   820,   387,   562,   581,   621,
     563,   186,   187,   622,   189,   190,   191,   192,    44,    91,
     102,    50,   101,   299,   300,   301,   302,   303,   111,   112,
      52,    51,   277,   278,   283,   282,   259,   264,   267,   260,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   258,
     424,   312,   452,    48,   246,   235,   250,    89,    90,    94,
      45,    46,    47,   305,    67,   203,   309,   391,   448,   214,
     310,   311,   402,   450,   451,   480,   511,   512,   295,   372,
     373,   439,   463,   462,   464,   503,   537,   538,   539,   604,
     656,   657,   704,   744,   745,   493,   535,   536,   598,   654,
     655,   403,   449,   204,   307,   308,   205,   216,   404,   482,
     441,   505,   362,   427,   363,   364,   365,   366
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -729
static const yytype_int16 yypact[] =
{
     621,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,   320,   262,   133,  -729,   -18,    38,    16,
    -729,   -34,    -9,    -4,    20,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,    98,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,   386,    22,  1056,
     292,   292,   369,   100,   328,   272,    68,   182,   166,   166,
     166,   -19,  -729,  -729,  -729,   138,   166,  -729,  -729,  -729,
     172,   -47,  -729,   103,   124,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,   -86,  -729,  -729,  -729,  -729,    75,  -729,  -729,  -729,
    -729,   292,   -86,   243,   248,   252,   256,   141,   277,  -729,
    1056,   448,   369,  -729,  -729,  -729,  -729,  -729,   194,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,   173,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,   113,   166,
     166,   166,   166,   166,   166,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,   166,  -729,  -729,  -729,   122,
     233,  -729,  -729,  -729,  -729,  -729,   166,   166,  -729,  -729,
     167,   169,   171,   166,   166,   166,   166,   176,   314,   -86,
     -86,   181,   215,   259,   -86,  -729,  -729,   -86,   -86,   -86,
     -86,   -86,   -86,   -86,   -86,   -86,   -86,   303,   303,  -729,
    -729,  -729,   303,   303,   216,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,   292,
     369,   448,   448,   369,  -729,   225,   780,   -86,   -70,   -86,
     609,   -86,   314,   166,   166,   226,   236,   242,   244,   245,
     261,   270,   166,   275,  -729,  -729,   279,   285,  -729,  -729,
    -729,   286,   291,   293,   297,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,   315,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,   622,  -729,  -729,  -729,  -729,  -729,
    -729,   222,   631,   -86,  -729,  -729,  -729,  -729,  -729,  -729,
     780,   239,   304,   309,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,   321,   329,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,   348,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,   166,  -729,  -729,  -729,  -729,  -729,
    1056,   282,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,    35,   780,   295,   351,  -729,   -86,   -86,
     640,   -86,   166,  -729,  -729,  -729,   362,   -14,  -729,   -71,
    -729,  -729,   698,   707,   -86,  -729,  -729,   780,  -729,  -729,
    1056,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,   366,  -729,  1056,  -729,  -729,  -729,
     780,   451,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,   239,    21,    36,
    -729,   292,   -86,  -729,  -729,   239,   368,  -729,   849,  -729,
    -729,  -729,  1125,  1125,  1332,  1056,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  2125,   -86,   292,  2134,   -86,
     282,  -729,  2083,  -729,  1056,  -729,   374,  -729,  1056,  -729,
    -729,  -729,  -729,   370,   229,   249,   175,  -729,   371,   163,
     220,   246,   379,   383,   302,   326,   336,   350,  -729,  2083,
     388,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,   918,
    -729,  -729,  -729,  -729,  -729,   987,  1194,  -729,  -729,  2083,
     389,   391,   392,   393,   395,   398,   396,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,   387,
     382,  -729,  -729,  -729,   399,   390,   397,   -26,  -729,   849,
    -729,  -729,  -729,  -729,  -729,  -729,  1608,  1539,  -729,  -729,
    1470,  -729,  1470,  1677,  -729,   400,  1539,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,   292,   -86,  2143,   -86,    57,  2083,
    -729,  -729,   394,  1056,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  1470,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  1677,  -729,  -729,  -729,  -729,  -729,
     379,  1815,   402,  -729,   -86,   405,   -86,   408,   -86,   409,
    1470,  -729,  -729,  -729,  -729,  -729,  -729,  -729,   406,  -729,
    2083,  -729,  1332,  -729,  -729,  2083,  -729,  -729,  -729,  1949,
    -729,  2083,  -729,  -729,   411,  1401,  -729,  -729,  -729,   407,
    -729,   418,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,   292,   -86,  -729,   410,   -69,  1263,
     382,   111,  1882,   413,  -729,   185,  -729,  2172,   379,  -729,
    -729,  -729,  -729,  -729,  1815,  1056,   -86,   -86,   -86,  -729,
    -729,  1746,  -729,  -729,  -729,  1677,  -729,  -729,  2016,  -729,
    -729,  -729,  2083,  -729,  -729,  -729,  -729,  -729,   401,   427,
     381,   423,   384,   424,   428,  -729,   447,  -729,   449,   455,
    -729,  -729,  -729,  -729,   456,   701,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,   379,  -729,   -86,  -729,
    2083,  -729,   134,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,   339,  -729,  -729,  -729,   414,  -729,  1882,
    -729,   382,   419,   459,   461,   462,   306,  -729,   425,   425,
     425,   425,   568,  1056,  1056,  1056,    29,  -729,   425,  -729,
    -729,  2016,  -729,  -729,  -729,  -729,  -729,  -729,  -729,   -97,
     -77,   -66,   -54,   464,   467,   467,   467,  -729,  -729,   -46,
     460,   425,   585,   425,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,   -45,   469,   -20,   -86,  -729,
    -729,  -729,  -729
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,   -36,
    -371,  -729,  -729,  -505,  -493,  -729,  -491,  -729,  -729,  -729,
    -729,  -729,  -729,   -33,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -251,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,   -90,     0,  -729,
    -729,  -729,  -729,  -402,  -729,  -729,   110,  -729,  -640,  -178,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -166,  -113,
      77,  -526,  -608,  -729,  -124,  -729,  -728,  -688,  -636,  -729,
    -729,  -729,  -729,  -729,  -203,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -609,  -729,  -253,    69,  -545,   -61,    26,
    -729,  -308,  -707,  -729,  -729,  -729,   -38,  -652,  -484,  -729,
    -729,  -497,  -492,   -43,  -729,  -489,  -504,  -509,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,   -24,   598,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,   -49,  -729,  -729,  -729,   -67,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,    41,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,  -729,
    -729,  -362,  -729,   -48,  -729,  -729,  -729,   -64,  -439,  -530,
    -729,  -729,    53,  -729,   253,  -729,   254,  -221
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -480
static const yytype_int16 yytable[] =
{
      41,   281,   206,   688,   215,   600,   188,   690,   750,   443,
     440,   185,   286,   559,   567,   632,   633,   634,   584,   584,
     584,   564,   541,   495,   778,   560,   884,   561,   752,   566,
     582,   582,   582,   623,   687,   251,   252,   343,   279,   731,
     587,   262,   257,   253,   722,   298,   886,   297,   724,   488,
     389,   772,   285,   641,   279,    92,    93,   887,   708,   641,
     623,   489,   773,   296,   215,   254,   885,   188,   720,   888,
      85,    86,   293,   466,   542,   584,   543,   893,   899,   280,
     623,   584,   584,   263,    96,   760,   885,   582,   481,   763,
      97,   406,   743,   582,   582,   280,   507,   885,   109,   606,
     494,   504,   644,   901,   544,   236,   237,   238,   239,   885,
     240,   861,   731,   841,   486,    98,   763,   885,   885,   515,
      99,   542,   487,   543,   559,   567,   706,   623,   623,   839,
     255,   623,   564,   623,   623,   688,   560,   623,   561,   690,
     566,   241,   821,   885,   100,   726,   119,   558,   248,   249,
     623,   544,   877,   242,   243,   244,   709,   710,   465,   711,
     313,   314,   712,   322,   548,    85,    86,   623,   444,   344,
     345,   217,   549,   599,   349,   623,   605,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   735,   853,   854,
     855,   623,   248,   249,   732,   315,   316,   317,   318,   319,
     320,   623,   734,   763,    87,    88,   623,   247,   584,   261,
     623,   321,   623,   808,   256,   323,   623,   388,   390,   392,
     582,   405,   326,   327,   284,   880,   245,   265,   587,   331,
     332,   333,   334,   375,   619,   673,   378,   297,   297,   776,
     870,   871,   872,   623,   693,   584,   287,   777,   266,   879,
     374,   288,   215,   296,   296,   215,   289,   582,   667,   290,
     735,   638,   623,   188,   291,   644,   623,   732,    68,   623,
      69,   838,   895,   623,   897,   734,   324,   325,   842,   843,
     292,   638,   304,   442,   225,    70,   844,   630,   306,   407,
     408,   328,   845,   329,   705,   330,   -32,   -32,   416,  -275,
     335,   226,   227,   228,   229,   346,   230,   231,  -287,  -287,
    -287,   623,   781,   782,    71,    72,    73,    74,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    53,   671,   674,
     623,   367,   232,   233,   234,   368,   369,   188,   674,   347,
     370,   218,   376,   377,   630,    54,    84,   348,    55,   379,
     409,   714,   623,   -34,   -34,   -34,    56,   430,   471,   472,
     410,   483,  -288,  -288,  -288,    57,   411,    58,   412,   413,
     630,    59,   444,   219,   506,   220,   221,   222,   223,   -33,
     -33,   -33,  -289,  -289,  -289,   414,    60,   188,    61,    62,
      63,    64,   459,   224,   415,    65,   460,   113,   114,   417,
     456,   188,   747,   418,    66,   115,   116,   117,   118,   419,
     420,   671,   193,   756,   194,   421,   195,   422,   196,   197,
     198,   423,   551,   336,   188,    35,   467,   188,   484,   337,
     338,   339,   510,   866,   336,  -286,  -286,  -286,   446,   425,
     337,   338,   339,   188,   447,   453,   601,   188,   459,   607,
     842,   843,   516,   454,   199,   200,   201,   202,   844,  -242,
    -242,  -242,   550,   816,   845,   340,   341,   342,   360,  -243,
    -243,  -243,   455,   468,   823,   565,   340,   341,   342,   583,
     583,   583,   188,  -244,  -244,  -244,   485,   588,   602,   207,
     513,   194,   552,   208,  -274,   209,   210,   211,   853,   854,
     855,   188,   627,   630,   629,   188,   625,   635,   639,   647,
     628,   648,   651,   649,   650,   652,   653,   660,   659,   661,
     664,   663,   578,   578,   738,   717,   736,   740,   742,   691,
     746,   199,   212,   213,   202,   764,   583,   757,   765,   779,
     824,   771,   583,   583,   826,   827,   829,   828,   517,   518,
     830,   519,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   531,   532,   702,   825,   707,   207,   831,
     194,   832,   208,   873,   294,   210,   211,   833,   834,   578,
     859,   863,   862,   864,   865,   578,   565,   889,   868,    35,
     896,   894,   900,   666,   665,   867,   514,   819,   809,   749,
     585,   762,   836,   878,   737,   701,   739,   533,   741,   608,
     199,   212,   213,   202,   719,   646,    95,   428,   429,     0,
     188,    -3,     1,     0,     0,   718,     0,     2,     3,     4,
       5,     0,  -479,  -479,     6,     7,     8,     9,    10,    11,
    -479,  -479,  -479,  -479,    12,    13,    14,     0,   733,    15,
       0,     0,     0,    16,     0,   770,     0,    17,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   583,
       0,     0,    18,     0,     0,     0,   811,   812,   813,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    19,     0,
       0,     0,     0,     0,     0,   769,     0,     0,    20,    21,
      22,    23,    24,    25,    26,     0,   583,     0,     0,     0,
      27,    28,    29,    30,    31,    32,    33,    34,     0,     0,
       0,   733,   188,     0,     0,     0,     0,   810,   840,   393,
       0,   394,     0,   279,     0,   395,   396,   397,     0,     0,
       0,     0,   336,    35,    35,     0,   426,     0,   337,   338,
     339,   432,     0,   394,     0,   279,     0,   433,   434,   435,
     473,     0,   394,     0,   279,     0,   474,   475,   476,     0,
       0,   398,   399,   400,   280,     0,     0,     0,     0,     0,
       0,     0,     0,    36,   340,   341,   342,   360,     0,     0,
       0,     0,     0,   436,   437,   438,   280,     0,     0,     0,
       0,     0,   477,   478,   479,   280,     0,     0,   902,     0,
     188,   188,   188,     0,     0,   874,   875,   876,   393,     0,
     394,     0,   279,     0,   395,   396,   397,   496,   835,   394,
       0,   279,     0,   497,   498,   499,     0,     0,   783,   784,
     785,   786,   787,   788,   789,   790,   791,   792,   793,   794,
     795,   796,   797,   798,   799,   800,   801,   802,   803,   804,
     398,   399,   492,   280,     0,     0,     0,     0,     0,   500,
     501,   502,   280,     0,   890,   891,   892,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   380,     0,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,     0,   183,     0,   184,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   380,     0,   553,   146,   147,   148,   554,   150,   151,
     555,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,   183,     0,   556,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   571,   572,   573,   574,   640,   575,
      35,     0,   576,   577,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
       0,   183,     0,   184,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   571,   572,   573,   574,   642,   575,    35,
       0,   576,   577,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
     183,     0,   184,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,     0,     0,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,     0,   183,
       0,   184,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   571,   572,   573,   574,     0,   575,    35,     0,   576,
     577,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,   183,     0,
     184,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     571,   572,   573,   574,   643,   575,     0,     0,   576,   577,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,     0,   183,     0,   184,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   571,
     572,   573,   574,   775,   575,     0,     0,   576,   577,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,   183,     0,   184,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   571,   572,
     573,   574,     0,   575,     0,     0,   576,   577,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,     0,   183,     0,   184,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   609,     0,   677,   610,   678,
     611,     0,   612,     0,     0,   759,     0,   679,     0,     0,
     613,   614,   615,     0,   616,   617,     0,     0,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,     0,   618,     0,   680,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   609,     0,   677,   610,   678,   611,
       0,   612,     0,     0,     0,     0,   679,     0,     0,   613,
     614,   615,     0,   616,   617,     0,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,     0,   618,     0,   680,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   609,     0,     0,   610,     0,   611,     0,
     612,     0,     0,   668,   672,     0,     0,     0,   613,   614,
     615,     0,   616,   617,     0,     0,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,   618,     0,   669,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   609,     0,     0,   610,     0,   611,     0,   612,
       0,     0,   668,     0,     0,     0,     0,   613,   614,   615,
       0,   616,   617,     0,     0,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
       0,   618,     0,   669,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   609,     0,     0,   610,     0,   611,     0,   612,     0,
       0,     0,     0,   679,     0,     0,   613,   614,   615,     0,
     616,   617,     0,     0,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
     618,     0,   680,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     609,     0,     0,   610,     0,   611,     0,   612,     0,     0,
       0,     0,     0,     0,     0,   613,   614,   615,     0,   616,
     617,     0,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,     0,   618,
       0,   814,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,     0,     0,     0,
     146,   147,   148,   727,   150,   151,   728,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,   183,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   609,     0,   677,   610,
     678,   611,     0,   612,     0,     0,     0,     0,   679,     0,
       0,   613,   614,   615,     0,   616,   617,     0,     0,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,     0,   618,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   609,     0,     0,   610,     0,   611,     0,
     612,     0,     0,     0,   754,     0,     0,     0,   613,   614,
     615,     0,   616,   617,     0,     0,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,   618,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     609,     0,     0,   610,     0,   611,     0,   612,     0,     0,
       0,     0,   679,     0,     0,   613,   614,   615,     0,   616,
     617,     0,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,     0,   618,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   609,     0,     0,
     610,     0,   611,     0,   612,     0,     0,     0,     0,     0,
       0,     0,   613,   614,   615,     0,   616,   617,     0,     0,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   473,   618,   394,     0,   279,
       0,   474,   597,   476,   393,     0,   394,     0,   279,     0,
     395,   396,   397,   473,     0,   394,     0,   279,     0,   474,
     703,   476,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   477,   478,   479,
     280,     0,     0,     0,     0,     0,   398,   399,   603,   280,
       0,     0,     0,     0,     0,   477,   478,   479,   280,   783,
     784,   785,   786,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,   797,   798,   799,   800,   801,   802,   803,
     804
};

static const yytype_int16 yycheck[] =
{
       0,    91,    51,   643,    52,   535,    49,   643,   715,   380,
     372,    49,   102,   518,   518,   560,   561,   562,   522,   523,
     524,   518,     1,   462,   752,   518,   123,   518,   716,   518,
     522,   523,   524,   542,   642,    59,    60,   258,   124,   691,
     524,    88,    66,    62,   684,   112,   123,   111,   684,   120,
     120,   120,   101,   579,   124,    73,    74,   123,     1,   585,
     569,   132,   131,   111,   112,    84,   163,   110,   676,   123,
      32,    33,   110,   444,   100,   579,   102,   123,   123,   165,
     589,   585,   586,   130,    68,   725,   163,   579,   450,   725,
     124,   312,   700,   585,   586,   165,   467,   163,     0,   538,
     462,   463,   586,   123,   130,    37,    38,    39,    40,   163,
      42,   839,   764,   820,   128,   124,   752,   163,   163,   490,
     124,   100,   136,   102,   629,   629,   656,   636,   637,   817,
     149,   640,   629,   642,   643,   775,   629,   646,   629,   775,
     629,    73,   778,   163,   124,   690,   124,   518,    26,    27,
     659,   130,   123,    85,    86,    87,    99,   100,   123,   102,
      47,    48,   105,    41,   128,    32,    33,   676,   133,   259,
     260,    71,   136,   535,   264,   684,   538,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   691,   159,   160,
     161,   700,    26,    27,   691,   219,   220,   221,   222,   223,
     224,   710,   691,   839,    71,    72,   715,    25,   712,    37,
     719,   235,   721,   758,    76,   239,   725,   307,   308,   309,
     712,   311,   246,   247,   149,   861,   158,   124,   712,   253,
     254,   255,   256,   300,   542,   637,   303,   301,   302,   128,
     849,   850,   851,   752,   646,   749,     3,   136,   124,   858,
     299,     3,   300,   301,   302,   303,     4,   749,   629,     3,
     764,   569,   771,   306,   123,   749,   775,   764,     6,   778,
       8,   816,   881,   782,   883,   764,    43,    44,   144,   145,
       3,   589,    88,   373,    12,    23,   152,   124,   115,   313,
     314,   124,   158,   124,   656,   124,   133,   134,   322,   124,
     124,    29,    30,    31,    32,   124,    34,    35,   133,   134,
     135,   820,   127,   128,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,     7,   636,   637,
     839,   278,    60,    61,    62,   282,   283,   380,   646,   124,
     124,    13,   301,   302,   124,    25,    84,    88,    28,   124,
     124,   659,   861,   133,   134,   135,    36,   135,   448,   449,
     124,   451,   133,   134,   135,    45,   124,    47,   124,   124,
     124,    51,   133,    45,   464,    47,    48,    49,    50,   133,
     134,   135,   133,   134,   135,   124,    66,   430,    68,    69,
      70,    71,   430,    65,   124,    75,   114,    11,    12,   124,
     424,   444,   710,   124,    84,    19,    20,    21,    22,   124,
     124,   719,   120,   721,   122,   124,   124,   124,   126,   127,
     128,   124,   512,   120,   467,   122,   131,   470,   452,   126,
     127,   128,   470,   127,   120,   133,   134,   135,   134,   124,
     126,   127,   128,   486,   135,   124,   536,   490,   486,   539,
     144,   145,     1,   124,   162,   163,   164,   165,   152,   133,
     134,   135,   511,   771,   158,   162,   163,   164,   165,   133,
     134,   135,   124,   122,   782,   518,   162,   163,   164,   522,
     523,   524,   525,   133,   134,   135,   124,   525,   537,   120,
     124,   122,   124,   124,   124,   126,   127,   128,   159,   160,
     161,   544,   128,   124,   133,   548,   544,   124,   120,   120,
     548,   120,   117,   121,   121,   117,   120,   135,   131,   120,
     123,   131,   522,   523,   119,   131,   124,   119,   119,   129,
     124,   162,   163,   164,   165,   128,   579,   126,   120,   126,
     139,   131,   585,   586,   163,   122,   122,   163,    97,    98,
     122,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   655,   139,   657,   120,   122,
     122,   122,   124,     5,   126,   127,   128,   122,   122,   579,
     166,   122,   163,   122,   122,   585,   629,   123,   163,   122,
       5,   131,   123,   629,   627,   846,   486,   775,   764,   712,
     523,   725,   805,   856,   694,   654,   696,   156,   698,   540,
     162,   163,   164,   165,   675,   589,    18,   364,   364,    -1,
     663,     0,     1,    -1,    -1,   663,    -1,     6,     7,     8,
       9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,   691,    28,
      -1,    -1,    -1,    32,    -1,   745,    -1,    36,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   712,
      -1,    -1,    51,    -1,    -1,    -1,   766,   767,   768,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,
      -1,    -1,    -1,    -1,    -1,   744,    -1,    -1,    77,    78,
      79,    80,    81,    82,    83,    -1,   749,    -1,    -1,    -1,
      89,    90,    91,    92,    93,    94,    95,    96,    -1,    -1,
      -1,   764,   765,    -1,    -1,    -1,    -1,   765,   818,   120,
      -1,   122,    -1,   124,    -1,   126,   127,   128,    -1,    -1,
      -1,    -1,   120,   122,   122,    -1,   124,    -1,   126,   127,
     128,   120,    -1,   122,    -1,   124,    -1,   126,   127,   128,
     120,    -1,   122,    -1,   124,    -1,   126,   127,   128,    -1,
      -1,   162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   162,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,    -1,   898,    -1,
     853,   854,   855,    -1,    -1,   853,   854,   855,   120,    -1,
     122,    -1,   124,    -1,   126,   127,   128,   120,   127,   122,
      -1,   124,    -1,   126,   127,   128,    -1,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,   874,   875,   876,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    -1,   165,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,    -1,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   163,    -1,   165,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,    -1,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,    -1,   165,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
      -1,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,    -1,   165,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,    -1,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,   163,
      -1,   165,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,   121,   122,    -1,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,   163,    -1,
     165,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,    -1,    -1,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,   165,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,    -1,   165,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,   121,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,   165,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,   116,   117,   118,
     119,    -1,   121,    -1,    -1,   124,    -1,   126,    -1,    -1,
     129,   130,   131,    -1,   133,   134,    -1,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,    -1,   165,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
      -1,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,
     130,   131,    -1,   133,   134,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    -1,   165,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,
     121,    -1,    -1,   124,   125,    -1,    -1,    -1,   129,   130,
     131,    -1,   133,   134,    -1,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   163,    -1,   165,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,   121,
      -1,    -1,   124,    -1,    -1,    -1,    -1,   129,   130,   131,
      -1,   133,   134,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,    -1,   165,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,    -1,   121,    -1,
      -1,    -1,    -1,   126,    -1,    -1,   129,   130,   131,    -1,
     133,   134,    -1,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,    -1,   165,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,   121,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,   133,
     134,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,   163,
      -1,   165,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,    -1,    -1,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,   163,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,    -1,   116,   117,
     118,   119,    -1,   121,    -1,    -1,    -1,    -1,   126,    -1,
      -1,   129,   130,   131,    -1,   133,   134,    -1,    -1,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,    -1,   117,    -1,   119,    -1,
     121,    -1,    -1,    -1,   125,    -1,    -1,    -1,   129,   130,
     131,    -1,   133,   134,    -1,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   163,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,   121,    -1,    -1,
      -1,    -1,   126,    -1,    -1,   129,   130,   131,    -1,   133,
     134,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,   163,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,    -1,    -1,
     117,    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   129,   130,   131,    -1,   133,   134,    -1,    -1,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   120,   163,   122,    -1,   124,
      -1,   126,   127,   128,   120,    -1,   122,    -1,   124,    -1,
     126,   127,   128,   120,    -1,   122,    -1,   124,    -1,   126,
     127,   128,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    14,    15,    16,
      17,    18,    23,    24,    25,    28,    32,    36,    51,    67,
      77,    78,    79,    80,    81,    82,    83,    89,    90,    91,
      92,    93,    94,    95,    96,   122,   162,   168,   169,   170,
     202,   215,   217,   227,   305,   347,   348,   349,   340,   203,
     308,   318,   317,     7,    25,    28,    36,    45,    47,    51,
      66,    68,    69,    70,    71,    75,    84,   351,     6,     8,
      23,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    84,    32,    33,    71,    72,   344,
     345,   306,    73,    74,   346,   344,    68,   124,   124,   124,
     124,   309,   307,   173,   174,   175,   172,   216,   171,     0,
     218,   315,   316,    11,    12,    19,    20,    21,    22,   124,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   163,   165,   293,   298,   299,   300,   301,
     302,   303,   304,   120,   122,   124,   126,   127,   128,   162,
     163,   164,   165,   352,   390,   393,   352,   120,   124,   126,
     127,   128,   163,   164,   356,   390,   394,    71,    13,    45,
      47,    48,    49,    50,    65,    12,    29,    30,    31,    32,
      34,    35,    60,    61,    62,   342,    37,    38,    39,    40,
      42,    73,    85,    86,    87,   158,   341,    25,    26,    27,
     343,   343,   343,    62,    84,   149,    76,   343,   336,   323,
     326,    37,    88,   130,   324,   124,   124,   325,   327,   328,
     329,   330,   331,   332,   333,   334,   335,   319,   320,   124,
     165,   214,   322,   321,   149,   352,   214,     3,     3,     4,
       3,   123,     3,   293,   126,   365,   390,   394,   356,   310,
     311,   312,   313,   314,    88,   350,   115,   391,   392,   353,
     357,   358,   338,    47,    48,   343,   343,   343,   343,   343,
     343,   343,    41,   343,    43,    44,   343,   343,   124,   124,
     124,   343,   343,   343,   343,   124,   120,   126,   127,   128,
     162,   163,   164,   404,   214,   214,   124,   124,    88,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     165,   215,   399,   401,   402,   403,   404,   399,   399,   399,
     124,   219,   366,   367,   352,   356,   365,   365,   356,   124,
     122,   177,   178,   179,   180,   181,   183,   293,   214,   120,
     214,   354,   214,   120,   122,   126,   127,   128,   162,   163,
     164,   214,   359,   388,   395,   214,   404,   343,   343,   124,
     124,   124,   124,   124,   124,   124,   343,   124,   124,   124,
     124,   124,   124,   124,   337,   124,   124,   400,   401,   403,
     135,   221,   120,   126,   127,   128,   162,   163,   164,   368,
     388,   397,   214,   177,   133,   204,   134,   135,   355,   389,
     360,   361,   339,   124,   124,   124,   343,   222,   223,   293,
     114,   283,   370,   369,   371,   123,   177,   131,   122,   185,
     182,   214,   214,   120,   126,   127,   128,   162,   163,   164,
     362,   388,   396,   214,   343,   124,   128,   136,   120,   132,
     224,   228,   164,   382,   388,   395,   120,   126,   127,   128,
     162,   163,   164,   372,   388,   398,   214,   177,   186,   184,
     293,   363,   364,   124,   223,   177,     1,    97,    98,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   156,   229,   383,   384,   373,   374,   375,
     205,     1,   100,   102,   130,   188,   189,   190,   128,   136,
     352,   214,   124,   124,   128,   131,   165,   176,   177,   180,
     181,   183,   294,   297,   298,   300,   302,   303,   230,   231,
     232,   116,   117,   118,   119,   121,   124,   125,   215,   257,
     258,   295,   299,   300,   303,   257,   256,   295,   293,   252,
     234,   236,   239,   241,   244,   246,   249,   127,   385,   388,
     396,   214,   352,   164,   376,   388,   395,   214,   283,   114,
     117,   119,   121,   129,   130,   131,   133,   134,   163,   288,
     289,   296,   300,   304,   191,   293,   187,   128,   293,   133,
     124,   284,   284,   284,   284,   124,   285,   286,   288,   120,
     120,   258,   120,   120,   295,   251,   286,   120,   120,   121,
     121,   117,   117,   120,   386,   387,   377,   378,   206,   131,
     135,   120,   193,   131,   123,   190,   176,   177,   124,   165,
     220,   288,   125,   220,   288,   287,   233,   116,   118,   126,
     165,   225,   259,   261,   263,   265,   288,   259,   225,   226,
     265,   129,   253,   220,   235,   237,   240,   242,   245,   247,
     250,   352,   214,   127,   379,   388,   396,   214,     1,    99,
     100,   102,   105,   207,   288,   290,   194,   131,   293,   285,
     259,   266,   225,   262,   265,   264,   284,   128,   131,   254,
     255,   294,   298,   300,   302,   303,   124,   214,   119,   214,
     119,   214,   119,   259,   380,   381,   124,   288,   208,   256,
     289,   291,   264,   192,   125,   267,   288,   126,   268,   124,
     225,   260,   261,   265,   128,   120,   238,   243,   248,   352,
     214,   131,   120,   131,   210,   120,   128,   136,   263,   126,
     195,   127,   128,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   269,   270,   271,   284,   255,
     293,   214,   214,   214,   165,   209,   288,   211,   213,   226,
     292,   265,   196,   288,   139,   139,   163,   122,   163,   122,
     122,   122,   122,   122,   122,   127,   271,   272,   284,   264,
     214,   289,   144,   145,   152,   158,   197,   198,   273,   275,
     276,   277,   278,   159,   160,   161,   281,   282,   274,   166,
     279,   263,   163,   122,   122,   122,   127,   198,   163,   280,
     280,   280,   280,     5,   293,   293,   293,   123,   282,   280,
     265,   199,   201,   200,   123,   163,   123,   123,   123,   123,
     215,   215,   215,   123,   131,   280,     5,   280,   212,   123,
     123,   123,   214
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */





/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

    /* Number of syntax errors so far.  */
    int yynerrs;

    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

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

/* Line 1455 of yacc.c  */
#line 232 "surface.yy"
    { YYACCEPT; ;}
    break;

  case 3:

/* Line 1455 of yacc.c  */
#line 234 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			;}
    break;

  case 8:

/* Line 1455 of yacc.c  */
#line 248 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 9:

/* Line 1455 of yacc.c  */
#line 250 "surface.yy"
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[(3) - (3)].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			;}
    break;

  case 10:

/* Line 1455 of yacc.c  */
#line 258 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 11:

/* Line 1455 of yacc.c  */
#line 260 "surface.yy"
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[(3) - (3)].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			;}
    break;

  case 12:

/* Line 1455 of yacc.c  */
#line 269 "surface.yy"
    {
			  cout << directoryManager.getCwd() << '\n';
			;}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 272 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 274 "surface.yy"
    {
			  string directory;
			  directoryManager.realPath((yyvsp[(3) - (3)].yyString), directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			;}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 283 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 285 "surface.yy"
    {
			  string directory;
			  directoryManager.realPath((yyvsp[(3) - (3)].yyString), directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			;}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 295 "surface.yy"
    {
			  const char* path = directoryManager.popd();
			  if (path != 0)
			    cout << path << '\n';
			  else
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": popd failed");
			    }
			;}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 305 "surface.yy"
    { lexerStringMode(); ;}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 307 "surface.yy"
    {
			  system((string("ls") + (yyvsp[(3) - (3)].yyString)).c_str());
			;}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 311 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			;}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 316 "surface.yy"
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			;}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 349 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[(1) - (2)].yyToken)));
                        ;}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 353 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[(1) - (2)].yyToken)));
                        ;}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 360 "surface.yy"
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			;}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 368 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  moduleExpressions.push(new ModuleExpression(t));
			;}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 378 "surface.yy"
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			;}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 394 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[(1) - (1)].yyToken)));
                        ;}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 401 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			;}
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 409 "surface.yy"
    { clear(); ;}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 411 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, tokenSequence));
			;}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 418 "surface.yy"
    {;}
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 421 "surface.yy"
    { store((yyvsp[(3) - (3)].yyToken)); ;}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 422 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 429 "surface.yy"
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			;}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 434 "surface.yy"
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			;}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 449 "surface.yy"
    {
			  currentRenaming->addSortMapping((yyvsp[(2) - (4)].yyToken), (yyvsp[(4) - (4)].yyToken));
			;}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 453 "surface.yy"
    {
			  currentRenaming->addLabelMapping((yyvsp[(2) - (4)].yyToken), (yyvsp[(4) - (4)].yyToken));
			;}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 456 "surface.yy"
    { lexBubble(BAR_COLON | BAR_TO, 1); ;}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 457 "surface.yy"
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); ;}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 458 "surface.yy"
    {;}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 463 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); ;}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 464 "surface.yy"
    {;}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 465 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); ;}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 471 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); ;}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 472 "surface.yy"
    {;}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 473 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); ;}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 480 "surface.yy"
    { currentRenaming->setPrec((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 481 "surface.yy"
    { clear(); ;}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 482 "surface.yy"
    { currentRenaming->setGather(tokenSequence); ;}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 483 "surface.yy"
    { clear(); ;}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 484 "surface.yy"
    { currentRenaming->setFormat(tokenSequence); ;}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 485 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 486 "surface.yy"
    { currentRenaming->setLatexMacro((yyvsp[(4) - (5)].yyString)); ;}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 492 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 494 "surface.yy"
    {
			  fileTable.beginModule((yyvsp[(1) - (5)].yyToken), (yyvsp[(3) - (5)].yyToken));
			  interpreter.setCurrentView(new View((yyvsp[(3) - (5)].yyToken)));
			  currentSyntaxContainer = CV;
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			;}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 502 "surface.yy"
    {
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			;}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 507 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[(3) - (12)].yyToken)).code(), CV);
			  CV->finishView();
			;}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 520 "surface.yy"
    {
			  CV->addSortMapping((yyvsp[(2) - (4)].yyToken), (yyvsp[(4) - (4)].yyToken));
			;}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 523 "surface.yy"
    {;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 524 "surface.yy"
    { lexBubble(BAR_COLON | BAR_TO, 1); ;}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 529 "surface.yy"
    { (yyval.yyToken) = (yyvsp[(1) - (2)].yyToken); ;}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 531 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 540 "surface.yy"
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 548 "surface.yy"
    {
			  lexBubble(END_STATEMENT, 1);
			;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 552 "surface.yy"
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 557 "surface.yy"
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1)
			;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 567 "surface.yy"
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
			;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 589 "surface.yy"
    {;}
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 591 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			;}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 599 "surface.yy"
    { lexBubble(BAR_RIGHT_PAREN, 1); ;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 600 "surface.yy"
    {;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 606 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 608 "surface.yy"
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[(1) - (3)].yyToken), (yyvsp[(3) - (3)].yyToken)));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[(1) - (3)].yyToken), (yyvsp[(3) - (3)].yyToken));
			;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 614 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[(8) - (8)].yyToken));
			;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 621 "surface.yy"
    {;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 623 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  store(t);
			;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 631 "surface.yy"
    {;}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 640 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter((yyvsp[(1) - (3)].yyToken), me);
			;}
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 647 "surface.yy"
    {;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 649 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 657 "surface.yy"
    {
			  singleton[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[(1) - (1)].yyToken);  // needed for line number
			;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 666 "surface.yy"
    {;}
    break;

  case 110:

/* Line 1455 of yacc.c  */
#line 677 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport((yyvsp[(1) - (2)].yyToken), me);
			;}
    break;

  case 111:

/* Line 1455 of yacc.c  */
#line 683 "surface.yy"
    { clear(); ;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 684 "surface.yy"
    { CM->addSortDecl(tokenSequence); ;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 686 "surface.yy"
    { clear(); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 687 "surface.yy"
    { CM->addSubsortDecl(tokenSequence); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 689 "surface.yy"
    { lexBubble(BAR_COLON, 1); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 690 "surface.yy"
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 691 "surface.yy"
    {;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 693 "surface.yy"
    {;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 695 "surface.yy"
    {;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 697 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON, 1); ;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 698 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 699 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 701 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON, 1);  ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 702 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 703 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 704 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 706 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_EQUALS, 1); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 707 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 708 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 710 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_EQUALS, 1); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 711 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 712 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 713 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 715 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_ARROW2, 1); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 716 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 717 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 719 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_ARROW2, 1); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 720 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 721 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 722 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 724 "surface.yy"
    { lexBubble(BAR_COLON, 1); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 725 "surface.yy"
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 726 "surface.yy"
    { CM->setFlag(SymbolType::MESSAGE); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 729 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 734 "surface.yy"
    {
			;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 737 "surface.yy"
    {
			;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 740 "surface.yy"
    { clear(); ;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 741 "surface.yy"
    { CM->addSubsortDecl(tokenSequence); ;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 744 "surface.yy"
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 754 "surface.yy"
    {;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 755 "surface.yy"
    {;}
    break;

  case 154:

/* Line 1455 of yacc.c  */
#line 763 "surface.yy"
    {
			;}
    break;

  case 155:

/* Line 1455 of yacc.c  */
#line 767 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 768 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 159:

/* Line 1455 of yacc.c  */
#line 775 "surface.yy"
    { singleton[0] = (yyvsp[(1) - (1)].yyToken); CM->addOpDecl(singleton); ;}
    break;

  case 160:

/* Line 1455 of yacc.c  */
#line 776 "surface.yy"
    { CM->addOpDecl(lexerBubble); ;}
    break;

  case 163:

/* Line 1455 of yacc.c  */
#line 782 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 790 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 795 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			;}
    break;

  case 167:

/* Line 1455 of yacc.c  */
#line 802 "surface.yy"
    {
			  if ((yyvsp[(1) - (2)].yyBool))
			    CM->convertSortsToKinds();
			;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 809 "surface.yy"
    {;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 812 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 813 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 821 "surface.yy"
    {
			  singleton[0] = (yyvsp[(1) - (1)].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 825 "surface.yy"
    { clear(); ;}
    break;

  case 176:

/* Line 1455 of yacc.c  */
#line 827 "surface.yy"
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			;}
    break;

  case 177:

/* Line 1455 of yacc.c  */
#line 832 "surface.yy"
    { store((yyvsp[(3) - (3)].yyToken)); ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 833 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 836 "surface.yy"
    {;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 845 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			;}
    break;

  case 184:

/* Line 1455 of yacc.c  */
#line 849 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			;}
    break;

  case 185:

/* Line 1455 of yacc.c  */
#line 853 "surface.yy"
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			;}
    break;

  case 186:

/* Line 1455 of yacc.c  */
#line 859 "surface.yy"
    {
			  CM->setFlag(SymbolType::ASSOC);
			;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 863 "surface.yy"
    {
			  CM->setFlag(SymbolType::COMM);
			;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 866 "surface.yy"
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); ;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 867 "surface.yy"
    { CM->setIdentity(lexerBubble); ;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 869 "surface.yy"
    {
			  CM->setFlag(SymbolType::IDEM);
			;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 873 "surface.yy"
    {
			  CM->setFlag(SymbolType::ITER);
			;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 876 "surface.yy"
    { CM->setPrec((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 877 "surface.yy"
    { clear(); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 878 "surface.yy"
    { CM->setGather(tokenSequence); ;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 879 "surface.yy"
    { clear(); ;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 880 "surface.yy"
    { CM->setFormat(tokenSequence); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 881 "surface.yy"
    { clear(); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 882 "surface.yy"
    { CM->setStrat(tokenSequence); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 883 "surface.yy"
    { clear(); ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 884 "surface.yy"
    { CM->setPoly(tokenSequence); ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 886 "surface.yy"
    {
			  CM->setFlag(SymbolType::MEMO);
			;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 890 "surface.yy"
    {
			  CM->setFlag(SymbolType::CTOR);
			;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 894 "surface.yy"
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 898 "surface.yy"
    { clear(); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 899 "surface.yy"
    { CM->setFrozen(tokenSequence); ;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 901 "surface.yy"
    {
			  CM->setFlag(SymbolType::CONFIG);
			;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 905 "surface.yy"
    {
			  CM->setFlag(SymbolType::OBJECT);
			;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 909 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 913 "surface.yy"
    {
			  CM->setMetadata((yyvsp[(2) - (2)].yyToken));
			;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 916 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 917 "surface.yy"
    { CM->setLatexMacro((yyvsp[(4) - (5)].yyString)); ;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 918 "surface.yy"
    {;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 920 "surface.yy"
    {
			  CM->setFlag(SymbolType::DITTO);
			;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 933 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 934 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 941 "surface.yy"
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[(2) - (2)].yyToken), tokenSequence); ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 942 "surface.yy"
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); ;}
    break;

  case 222:

/* Line 1455 of yacc.c  */
#line 943 "surface.yy"
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); ;}
    break;

  case 223:

/* Line 1455 of yacc.c  */
#line 944 "surface.yy"
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 950 "surface.yy"
    {;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 952 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 958 "surface.yy"
    {;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 960 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 968 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 972 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 973 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 974 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 233:

/* Line 1455 of yacc.c  */
#line 975 "surface.yy"
    { store((yyvsp[(4) - (4)].yyToken)); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 982 "surface.yy"
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  (yyval.yyToken) = t;
			;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 993 "surface.yy"
    { fragStore((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 994 "surface.yy"
    { fragStore((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 995 "surface.yy"
    { fragStore((yyvsp[(5) - (5)].yyToken)); ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 998 "surface.yy"
    { fragStore((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 239:

/* Line 1455 of yacc.c  */
#line 999 "surface.yy"
    {;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1000 "surface.yy"
    {;}
    break;

  case 343:

/* Line 1455 of yacc.c  */
#line 1080 "surface.yy"
    { lexBubble(END_COMMAND, 1) ;}
    break;

  case 344:

/* Line 1455 of yacc.c  */
#line 1082 "surface.yy"
    {
			  interpreter.setCurrentModule(lexerBubble);
			;}
    break;

  case 345:

/* Line 1455 of yacc.c  */
#line 1085 "surface.yy"
    { lexBubble(END_COMMAND, 1) ;}
    break;

  case 346:

/* Line 1455 of yacc.c  */
#line 1087 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 1092 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 1097 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 1104 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 1109 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 1116 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 1121 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 1128 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 1134 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 1140 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 1147 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 1153 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 1160 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 1166 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 1172 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 1178 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 1185 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[(1) - (3)].yySearchKind));
			;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 1191 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 1197 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[(1) - (3)].yyBool), number);
			;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 1203 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 1209 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number);
			;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 1215 "surface.yy"
    {
			  interpreter.cont((yyvsp[(3) - (4)].yyInt64), (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 1219 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 1224 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 1230 "surface.yy"
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 1235 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 1237 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[(2) - (4)].yyBool));
			;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 1241 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 1243 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[(2) - (4)].yyBool));
			;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 1247 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 1249 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[(2) - (4)].yyBool));
			;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 1253 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 1255 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[(2) - (4)].yyBool));
			;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 1260 "surface.yy"
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 1267 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 1269 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 1273 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 1275 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 1279 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 1281 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 1285 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 1287 "surface.yy"
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 1292 "surface.yy"
    {
			  interpreter.showModules(true);
			;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 1296 "surface.yy"
    {
			  interpreter.showNamedViews();
			;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 1299 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 1301 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 1305 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 1307 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 1311 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 1313 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 1317 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 1319 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 1323 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 1325 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 1329 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 1331 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 1335 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 1337 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 1341 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 1343 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 1348 "surface.yy"
    {
			  interpreter.showSearchPath((yyvsp[(3) - (4)].yyInt64));
			;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 1352 "surface.yy"
    {
			  interpreter.showSearchPathLabels((yyvsp[(4) - (5)].yyInt64));
			;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 1356 "surface.yy"
    {
			  interpreter.showSearchGraph();
			;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 1359 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 1361 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 1369 "surface.yy"
    {
			  globalAdvisoryFlag = (yyvsp[(4) - (5)].yyBool);
			;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 1373 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 1377 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[(5) - (6)].yyBool));
			;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 1381 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 1385 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 1389 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[(5) - (6)].yyBool));
			;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 1393 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 1397 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 1401 "surface.yy"
    {
			  interpreter.setPrintFlag((yyvsp[(3) - (5)].yyPrintFlags), (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 1405 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 1409 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[(5) - (6)].yyBool));
			;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 1413 "surface.yy"
    {
			  interpreter.setFlag((yyvsp[(3) - (5)].yyFlags), (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 1417 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[(3) - (4)].yyBool));
			;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 1420 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 1421 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 1423 "surface.yy"
    {
			  interpreter.setAutoImport((yyvsp[(2) - (7)].yyImportMode), (yyvsp[(4) - (7)].yyToken), (yyvsp[(6) - (7)].yyBool));
			;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 1426 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 1427 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 1429 "surface.yy"
    {
			  interpreter.setOmodInclude((yyvsp[(5) - (8)].yyToken), (yyvsp[(7) - (8)].yyBool));
			;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 1433 "surface.yy"
    {
			  globalVerboseFlag = (yyvsp[(3) - (4)].yyBool);
			;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 1437 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 1441 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 1445 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 1449 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[(3) - (4)].yyBool));
			;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 1453 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 1460 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 1464 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 1468 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 1472 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 1479 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 1483 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[(3) - (4)].yyBool));
			;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 1489 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 1496 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; ;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 1497 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; ;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 1498 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; ;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 1499 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; ;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 1500 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; ;}
    break;

  case 449:

/* Line 1455 of yacc.c  */
#line 1501 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 1502 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 1503 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 1504 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 1505 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 1508 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE; ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 1509 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 1510 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 1511 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 1512 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 1513 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_MB; ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 1514 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 1515 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_RL; ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 1516 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 1517 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 1518 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 1521 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 1522 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 1525 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 1526 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 1529 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 1530 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 1533 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 1534 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 1539 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::NARROW; ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 1540 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 1541 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::SEARCH; ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 1544 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 1545 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 1548 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 1549 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 1552 "surface.yy"
    { (yyval.yyInt64) = (yyvsp[(1) - (1)].yyInt64); ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 1553 "surface.yy"
    { (yyval.yyInt64) = NONE; ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 1556 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::PROTECTING; ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 1557 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::EXTENDING; ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 1558 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::INCLUDING; ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 1568 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 1572 "surface.yy"
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); ;}
    break;

  case 491:

/* Line 1455 of yacc.c  */
#line 1584 "surface.yy"
    { lexSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 1587 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0);  ;}
    break;

  case 496:

/* Line 1455 of yacc.c  */
#line 1592 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 498:

/* Line 1455 of yacc.c  */
#line 1594 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 501:

/* Line 1455 of yacc.c  */
#line 1600 "surface.yy"
    { number = Token::codeToInt64(lexerBubble[1].code()); ;}
    break;

  case 503:

/* Line 1455 of yacc.c  */
#line 1602 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 506:

/* Line 1455 of yacc.c  */
#line 1617 "surface.yy"
    { lexSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 1620 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 511:

/* Line 1455 of yacc.c  */
#line 1629 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 513:

/* Line 1455 of yacc.c  */
#line 1631 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 1633 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 518:

/* Line 1455 of yacc.c  */
#line 1643 "surface.yy"
    { number = Token::codeToInt64(lexerBubble[1].code()); ;}
    break;

  case 520:

/* Line 1455 of yacc.c  */
#line 1645 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 522:

/* Line 1455 of yacc.c  */
#line 1647 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 525:

/* Line 1455 of yacc.c  */
#line 1657 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 527:

/* Line 1455 of yacc.c  */
#line 1659 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 530:

/* Line 1455 of yacc.c  */
#line 1670 "surface.yy"
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			;}
    break;

  case 532:

/* Line 1455 of yacc.c  */
#line 1676 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 535:

/* Line 1455 of yacc.c  */
#line 1686 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 537:

/* Line 1455 of yacc.c  */
#line 1688 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 540:

/* Line 1455 of yacc.c  */
#line 1698 "surface.yy"
    { number2 = Token::codeToInt64(lexerBubble[2].code()); ;}
    break;

  case 542:

/* Line 1455 of yacc.c  */
#line 1700 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 545:

/* Line 1455 of yacc.c  */
#line 1705 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0, 1); ;}
    break;

  case 548:

/* Line 1455 of yacc.c  */
#line 1711 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 1, 1); ;}
    break;

  case 550:

/* Line 1455 of yacc.c  */
#line 1713 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON | END_COMMAND, 0); ;}
    break;

  case 552:

/* Line 1455 of yacc.c  */
#line 1716 "surface.yy"
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(lexerBubble[0]);
			;}
    break;

  case 590:

/* Line 1455 of yacc.c  */
#line 1755 "surface.yy"
    {;}
    break;

  case 592:

/* Line 1455 of yacc.c  */
#line 1760 "surface.yy"
    {
			  singleton[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			;}
    break;

  case 595:

/* Line 1455 of yacc.c  */
#line 1771 "surface.yy"
    {
			  singleton[0] = (yyvsp[(1) - (1)].yyToken);
			  interpreter.addSelected(singleton);
			;}
    break;

  case 596:

/* Line 1455 of yacc.c  */
#line 1776 "surface.yy"
    {
			  interpreter.addSelected(lexerBubble);
			;}
    break;



/* Line 1455 of yacc.c  */
#line 5854 "surface.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
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

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
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
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1806 "surface.yy"


static void
yyerror(char *s)
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

