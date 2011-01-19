
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
     KW_SREDUCE = 338,
     KW_DUMP = 339,
     KW_PROFILE = 340,
     KW_NUMBER = 341,
     KW_RAT = 342,
     KW_COLOR = 343,
     SIMPLE_NUMBER = 344,
     KW_PWD = 345,
     KW_CD = 346,
     KW_PUSHD = 347,
     KW_POPD = 348,
     KW_LS = 349,
     KW_LOAD = 350,
     KW_QUIT = 351,
     KW_EOF = 352,
     KW_ENDM = 353,
     KW_IMPORT = 354,
     KW_ENDV = 355,
     KW_SORT = 356,
     KW_SUBSORT = 357,
     KW_OP = 358,
     KW_OPS = 359,
     KW_MSGS = 360,
     KW_VAR = 361,
     KW_CLASS = 362,
     KW_SUBCLASS = 363,
     KW_MB = 364,
     KW_CMB = 365,
     KW_EQ = 366,
     KW_CEQ = 367,
     KW_RL = 368,
     KW_CRL = 369,
     KW_IS = 370,
     KW_FROM = 371,
     KW_ARROW = 372,
     KW_ARROW2 = 373,
     KW_PARTIAL = 374,
     KW_IF = 375,
     KW_LABEL = 376,
     KW_TO = 377,
     KW_COLON2 = 378,
     KW_ASSOC = 379,
     KW_COMM = 380,
     KW_ID = 381,
     KW_IDEM = 382,
     KW_ITER = 383,
     KW_LEFT = 384,
     KW_RIGHT = 385,
     KW_PREC = 386,
     KW_GATHER = 387,
     KW_METADATA = 388,
     KW_STRAT = 389,
     KW_POLY = 390,
     KW_MEMO = 391,
     KW_FROZEN = 392,
     KW_CTOR = 393,
     KW_LATEX = 394,
     KW_SPECIAL = 395,
     KW_CONFIG = 396,
     KW_OBJ = 397,
     KW_MSG = 398,
     KW_DITTO = 399,
     KW_FORMAT = 400,
     KW_ID_HOOK = 401,
     KW_OP_HOOK = 402,
     KW_TERM_HOOK = 403,
     KW_IN = 404,
     IDENTIFIER = 405,
     NUMERIC_ID = 406,
     ENDS_IN_DOT = 407,
     FORCE_LOOKAHEAD = 408
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
#line 350 "surface.c"
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
#line 367 "surface.c"

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
#define YYFINAL  111
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2316

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  168
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  241
/* YYNRULES -- Number of rules.  */
#define YYNRULES  609
/* YYNRULES -- Number of states.  */
#define YYNSTATES  911

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   408

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     123,   124,   135,   134,   129,     2,   125,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   121,     2,
     126,   122,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   127,     2,   128,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   136,   130,   137,     2,     2,     2,     2,
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
     115,   116,   117,   118,   119,   120,   131,   132,   133,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167
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
     187,   188,   189,   202,   205,   206,   208,   209,   214,   220,
     221,   225,   228,   231,   233,   234,   235,   244,   245,   249,
     251,   253,   254,   258,   259,   260,   269,   271,   273,   277,
     278,   282,   284,   288,   290,   292,   294,   297,   299,   301,
     303,   306,   307,   310,   311,   316,   317,   322,   323,   324,
     330,   335,   341,   342,   343,   349,   350,   351,   352,   360,
     361,   362,   368,   369,   370,   371,   379,   380,   381,   387,
     388,   389,   390,   398,   399,   400,   406,   411,   412,   418,
     419,   424,   427,   429,   432,   434,   438,   442,   445,   447,
     450,   452,   454,   456,   460,   462,   464,   466,   467,   470,
     472,   474,   477,   481,   483,   485,   487,   490,   491,   493,
     494,   499,   503,   505,   509,   510,   513,   515,   517,   520,
     523,   525,   527,   528,   532,   534,   536,   539,   540,   546,
     547,   553,   554,   560,   561,   567,   569,   571,   573,   574,
     580,   582,   584,   586,   589,   590,   596,   601,   603,   605,
     606,   609,   611,   614,   616,   619,   623,   627,   631,   633,
     634,   636,   637,   640,   641,   644,   647,   648,   653,   655,
     657,   658,   664,   665,   670,   672,   674,   676,   678,   680,
     682,   684,   686,   688,   690,   692,   694,   696,   698,   700,
     702,   704,   706,   708,   710,   712,   714,   716,   718,   720,
     722,   724,   726,   728,   730,   732,   734,   736,   738,   740,
     742,   744,   746,   748,   750,   752,   754,   756,   758,   760,
     762,   764,   766,   768,   770,   772,   774,   776,   778,   780,
     782,   784,   786,   788,   790,   792,   794,   796,   798,   800,
     802,   804,   806,   808,   810,   812,   814,   816,   818,   820,
     822,   824,   826,   828,   830,   832,   834,   836,   838,   840,
     842,   844,   846,   848,   850,   852,   854,   856,   858,   860,
     862,   864,   866,   868,   870,   872,   874,   876,   877,   881,
     882,   886,   887,   891,   892,   896,   897,   901,   902,   907,
     908,   913,   914,   919,   920,   925,   926,   931,   932,   936,
     937,   941,   942,   946,   951,   952,   956,   958,   959,   964,
     965,   970,   971,   976,   977,   982,   987,   988,   993,   994,
     999,  1000,  1005,  1006,  1011,  1015,  1019,  1020,  1025,  1026,
    1031,  1032,  1037,  1038,  1043,  1044,  1049,  1050,  1055,  1056,
    1061,  1062,  1067,  1072,  1078,  1083,  1084,  1089,  1095,  1101,
    1108,  1114,  1120,  1127,  1133,  1139,  1145,  1151,  1158,  1164,
    1169,  1170,  1171,  1179,  1180,  1181,  1190,  1195,  1201,  1207,
    1213,  1218,  1224,  1227,  1230,  1233,  1236,  1242,  1247,  1248,
    1252,  1254,  1256,  1259,  1262,  1264,  1266,  1268,  1270,  1272,
    1274,  1275,  1277,  1279,  1281,  1283,  1285,  1287,  1289,  1291,
    1293,  1295,  1297,  1299,  1301,  1303,  1305,  1307,  1309,  1311,
    1313,  1315,  1317,  1319,  1321,  1323,  1324,  1326,  1327,  1329,
    1331,  1333,  1335,  1336,  1340,  1341,  1345,  1347,  1348,  1352,
    1354,  1355,  1359,  1360,  1364,  1365,  1369,  1371,  1372,  1376,
    1377,  1381,  1383,  1384,  1388,  1390,  1391,  1395,  1396,  1400,
    1401,  1405,  1406,  1410,  1412,  1413,  1417,  1418,  1422,  1423,
    1427,  1429,  1430,  1434,  1435,  1439,  1441,  1442,  1446,  1447,
    1451,  1453,  1454,  1458,  1459,  1463,  1465,  1466,  1470,  1471,
    1475,  1477,  1478,  1482,  1484,  1485,  1489,  1490,  1494,  1496,
    1498,  1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,  1516,
    1518,  1520,  1522,  1524,  1526,  1528,  1530,  1532,  1534,  1536,
    1538,  1540,  1542,  1544,  1546,  1548,  1550,  1552,  1554,  1556,
    1558,  1560,  1562,  1564,  1566,  1569,  1571,  1573,  1575,  1577,
    1580,  1582,  1584,  1586,  1588,  1590,  1592,  1594,  1596,  1598
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     169,     0,    -1,   170,    -1,    -1,   219,    -1,   203,    -1,
     171,    -1,   308,    -1,    -1,   163,   172,     3,    -1,    -1,
      95,   173,     3,    -1,    90,    -1,    -1,    91,   174,     3,
      -1,    -1,    92,   175,     3,    -1,    93,    -1,    -1,    94,
     176,     4,    -1,    96,    -1,    97,    -1,   297,   287,    -1,
     300,   125,    -1,   184,   287,    -1,   181,   287,    -1,   182,
     287,    -1,   178,   134,   177,    -1,   166,    -1,   179,    -1,
     178,   134,   178,    -1,   180,    -1,   181,    -1,   184,    -1,
     182,    -1,   296,    -1,   179,   135,   186,    -1,    -1,   180,
     136,   183,   185,   137,    -1,   123,   178,   124,    -1,   185,
     129,   296,    -1,   296,    -1,    -1,    -1,   123,   187,   189,
     188,   124,    -1,   190,    -1,     1,    -1,   190,   129,   191,
      -1,   191,    -1,   101,   291,   132,   291,    -1,   131,   296,
     132,   296,    -1,    -1,    -1,   103,   192,   194,   132,   193,
     196,    -1,    -1,   121,   195,   267,   266,   268,    -1,    -1,
      -1,   127,   197,   198,   128,    -1,    -1,   198,   199,    -1,
     199,    -1,   145,   164,    -1,    -1,   146,   123,   200,   283,
     124,    -1,    -1,   159,   123,   201,   283,   124,    -1,    -1,
     153,   123,   202,     5,   124,    -1,    -1,    -1,    -1,     8,
     204,   296,   116,   178,   205,   132,   178,   206,   286,   207,
     100,    -1,   207,   209,    -1,    -1,   117,    -1,    -1,   101,
     291,   132,   211,    -1,   106,   258,   121,   208,   228,    -1,
      -1,   103,   210,   212,    -1,     1,   125,    -1,   291,   287,
      -1,   166,    -1,    -1,    -1,   121,   213,   267,   266,   268,
     132,   214,   216,    -1,    -1,   132,   215,   216,    -1,   125,
      -1,   166,    -1,    -1,   123,   218,   124,    -1,    -1,    -1,
     229,   220,   296,   221,   223,   286,   230,    98,    -1,   125,
      -1,   166,    -1,   136,   224,   137,    -1,    -1,   224,   129,
     225,    -1,   225,    -1,   296,   226,   178,    -1,   133,    -1,
     121,    -1,   166,    -1,   268,   287,    -1,   227,    -1,     6,
      -1,     7,    -1,   230,   231,    -1,    -1,    99,   177,    -1,
      -1,   101,   232,   288,   222,    -1,    -1,   102,   233,   289,
     222,    -1,    -1,    -1,   103,   234,   121,   235,   261,    -1,
     104,   259,   121,   261,    -1,   106,   258,   121,   208,   228,
      -1,    -1,    -1,   109,   236,   121,   237,   216,    -1,    -1,
      -1,    -1,   110,   238,   121,   239,   120,   240,   216,    -1,
      -1,    -1,   111,   241,   122,   242,   216,    -1,    -1,    -1,
      -1,   112,   243,   122,   244,   120,   245,   216,    -1,    -1,
      -1,   113,   246,   118,   247,   216,    -1,    -1,    -1,    -1,
     114,   248,   118,   249,   120,   250,   216,    -1,    -1,    -1,
     157,   251,   121,   252,   261,    -1,   105,   259,   121,   261,
      -1,    -1,   107,   296,   253,   255,   125,    -1,    -1,   108,
     254,   289,   222,    -1,     1,   125,    -1,   130,    -1,   130,
     256,    -1,   257,    -1,   256,   129,   257,    -1,   297,   121,
     296,    -1,   258,   298,    -1,   298,    -1,   259,   260,    -1,
     260,    -1,   298,    -1,   217,    -1,   268,   267,   263,    -1,
     264,    -1,   227,    -1,   121,    -1,    -1,   262,   264,    -1,
     125,    -1,   227,    -1,   266,   265,    -1,   268,   271,   287,
      -1,   227,    -1,   117,    -1,   119,    -1,   267,   268,    -1,
      -1,   291,    -1,    -1,   127,   269,   270,   128,    -1,   270,
     129,   291,    -1,   291,    -1,   127,   272,   128,    -1,    -1,
     272,   274,    -1,   274,    -1,   140,    -1,   143,   140,    -1,
     144,   140,    -1,   138,    -1,   139,    -1,    -1,   273,   275,
     282,    -1,   141,    -1,   142,    -1,   145,   164,    -1,    -1,
     146,   123,   276,   283,   124,    -1,    -1,   159,   123,   277,
     283,   124,    -1,    -1,   148,   123,   278,   283,   124,    -1,
      -1,   149,   123,   279,   283,   124,    -1,   150,    -1,   152,
      -1,   151,    -1,    -1,   151,   123,   280,   283,   124,    -1,
     155,    -1,   156,    -1,   157,    -1,   147,   164,    -1,    -1,
     153,   123,   281,     5,   124,    -1,   154,   123,   284,   124,
      -1,   158,    -1,   167,    -1,    -1,   283,   164,    -1,   164,
      -1,   284,   285,    -1,   285,    -1,   160,   296,    -1,   160,
     296,   217,    -1,   161,   296,   217,    -1,   162,   296,   217,
      -1,   115,    -1,    -1,   125,    -1,    -1,   288,   291,    -1,
      -1,   289,   291,    -1,   289,   126,    -1,    -1,   291,   290,
     288,   126,    -1,   292,    -1,   299,    -1,    -1,   292,   136,
     293,   294,   137,    -1,    -1,   294,   129,   295,   292,    -1,
     292,    -1,   302,    -1,   303,    -1,   305,    -1,   306,    -1,
     125,    -1,   301,    -1,   129,    -1,   132,    -1,   303,    -1,
     305,    -1,   306,    -1,   302,    -1,   303,    -1,   306,    -1,
     125,    -1,   126,    -1,   117,    -1,   119,    -1,   122,    -1,
     118,    -1,   120,    -1,   164,    -1,   303,    -1,   307,    -1,
     122,    -1,   130,    -1,   134,    -1,   135,    -1,   118,    -1,
     120,    -1,   115,    -1,   131,    -1,   132,    -1,   125,    -1,
     166,    -1,   164,    -1,   136,    -1,   137,    -1,   134,    -1,
     135,    -1,   130,    -1,   133,    -1,   131,    -1,   116,    -1,
     115,    -1,   301,    -1,   166,    -1,   129,    -1,   132,    -1,
     157,    -1,   304,    -1,    99,    -1,   101,    -1,   102,    -1,
     103,    -1,   104,    -1,   106,    -1,   105,    -1,   107,    -1,
     108,    -1,   109,    -1,   110,    -1,   111,    -1,   112,    -1,
     113,    -1,   114,    -1,    98,    -1,   100,    -1,   126,    -1,
     121,    -1,   117,    -1,   119,    -1,   122,    -1,   118,    -1,
     120,    -1,   127,    -1,   128,    -1,   307,    -1,   138,    -1,
     139,    -1,   140,    -1,   141,    -1,   142,    -1,   143,    -1,
     144,    -1,   145,    -1,   146,    -1,   148,    -1,   149,    -1,
     150,    -1,   152,    -1,   153,    -1,   154,    -1,   151,    -1,
     147,    -1,   155,    -1,   156,    -1,   158,    -1,   159,    -1,
     160,    -1,   161,    -1,   162,    -1,    -1,    32,   309,   216,
      -1,    -1,    84,   310,   216,    -1,    -1,     9,   311,   356,
      -1,    -1,    82,   312,   356,    -1,    -1,    83,   313,   356,
      -1,    -1,   353,    11,   314,   356,    -1,    -1,   353,    12,
     315,   360,    -1,    -1,   353,    19,   316,   369,    -1,    -1,
     353,    20,   317,   369,    -1,    -1,   353,    21,   318,   360,
      -1,    -1,   351,   319,   369,    -1,    -1,   352,   320,   360,
      -1,    -1,    18,   321,   360,    -1,   353,    22,   354,   125,
      -1,    -1,    13,   322,   356,    -1,   217,    -1,    -1,    28,
     348,   323,   403,    -1,    -1,    28,   349,   324,   403,    -1,
      -1,    51,   348,   325,   403,    -1,    -1,    36,   350,   326,
     403,    -1,    67,    68,   150,   125,    -1,    -1,    25,     6,
     327,   216,    -1,    -1,    25,    53,   328,   216,    -1,    -1,
      25,    56,   329,   216,    -1,    -1,    25,     8,   330,   216,
      -1,    25,    54,   125,    -1,    25,    55,   125,    -1,    -1,
      25,    57,   331,   216,    -1,    -1,    25,    58,   332,   216,
      -1,    -1,    25,    59,   333,   216,    -1,    -1,    25,    60,
     334,   216,    -1,    -1,    25,    61,   335,   216,    -1,    -1,
      25,    62,   336,   216,    -1,    -1,    25,    63,   337,   216,
      -1,    -1,    25,    64,   338,   216,    -1,    25,    52,    89,
     125,    -1,    25,    52,   131,    89,   125,    -1,    25,    23,
      37,   125,    -1,    -1,    25,    85,   339,   216,    -1,    24,
      25,    65,   347,   125,    -1,    24,    25,    47,   347,   125,
      -1,    24,    25,    13,    47,   347,   125,    -1,    24,    25,
      48,   347,   125,    -1,    24,    25,    50,   347,   125,    -1,
      24,    25,    13,    48,   347,   125,    -1,    24,    25,    49,
     347,   125,    -1,    24,    25,    45,   347,   125,    -1,    24,
      36,   345,   347,   125,    -1,    24,    36,    40,   347,   125,
      -1,    24,    36,    40,    41,   347,   125,    -1,    24,    28,
     346,   347,   125,    -1,    24,    51,   347,   125,    -1,    -1,
      -1,    24,   355,   340,   408,   341,   347,   125,    -1,    -1,
      -1,    24,     7,    71,   342,   408,   343,   347,   125,    -1,
      24,    66,   347,   125,    -1,    24,    68,   150,   347,   125,
      -1,    24,    68,    62,   347,   125,    -1,    24,    75,    76,
     347,   125,    -1,    24,    85,   347,   125,    -1,    24,    68,
      85,   347,   125,    -1,    78,   125,    -1,    79,   125,    -1,
      80,   125,    -1,    81,   125,    -1,    24,    45,    25,   347,
     125,    -1,    24,    47,   347,   125,    -1,    -1,     1,   344,
     125,    -1,    38,    -1,    39,    -1,    42,    44,    -1,    42,
      43,    -1,    37,    -1,    73,    -1,    86,    -1,    87,    -1,
      88,    -1,   159,    -1,    -1,    34,    -1,    31,    -1,    35,
      -1,    32,    -1,    60,    -1,    61,    -1,    62,    -1,    12,
      -1,    29,    -1,    30,    -1,    26,    -1,    27,    -1,    32,
      -1,    33,    -1,    72,    -1,    71,    -1,    73,    -1,    74,
      -1,    14,    -1,    15,    -1,    23,    -1,    17,    -1,    16,
      -1,    77,    -1,    -1,    89,    -1,    -1,    69,    -1,    70,
      -1,    71,    -1,   394,    -1,    -1,   397,   357,   216,    -1,
      -1,   121,   359,   216,    -1,   216,    -1,    -1,   127,   361,
     363,    -1,   394,    -1,    -1,   398,   362,   216,    -1,    -1,
     165,   364,   366,    -1,    -1,   399,   365,   216,    -1,   392,
      -1,    -1,   128,   367,   356,    -1,    -1,   400,   368,   216,
      -1,   392,    -1,    -1,   127,   370,   372,    -1,   394,    -1,
      -1,   398,   371,   216,    -1,    -1,   165,   373,   376,    -1,
      -1,   129,   374,   386,    -1,    -1,   401,   375,   216,    -1,
     392,    -1,    -1,   128,   377,   356,    -1,    -1,   129,   378,
     380,    -1,    -1,   402,   379,   216,    -1,   392,    -1,    -1,
     165,   381,   383,    -1,    -1,   399,   382,   216,    -1,   392,
      -1,    -1,   128,   384,   356,    -1,    -1,   400,   385,   216,
      -1,   392,    -1,    -1,   165,   387,   389,    -1,    -1,   399,
     388,   216,    -1,   392,    -1,    -1,   128,   390,   356,    -1,
      -1,   400,   391,   216,    -1,   392,    -1,    -1,   123,   393,
     216,    -1,   216,    -1,    -1,   123,   395,   216,    -1,    -1,
     163,   396,   358,    -1,   166,    -1,   164,    -1,   165,    -1,
     127,    -1,   128,    -1,   121,    -1,   125,    -1,   129,    -1,
     164,    -1,   165,    -1,   128,    -1,   121,    -1,   125,    -1,
     129,    -1,   164,    -1,   127,    -1,   128,    -1,   163,    -1,
     121,    -1,   129,    -1,   164,    -1,   165,    -1,   127,    -1,
     163,    -1,   121,    -1,   129,    -1,   164,    -1,   127,    -1,
     128,    -1,   163,    -1,   121,    -1,   164,    -1,   165,    -1,
     127,    -1,   163,    -1,   121,    -1,   406,   404,    -1,   405,
      -1,   125,    -1,   405,    -1,   166,    -1,   406,   407,    -1,
     407,    -1,   408,    -1,   217,    -1,   164,    -1,   165,    -1,
     127,    -1,   128,    -1,   163,    -1,   121,    -1,   129,    -1
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
     494,   502,   492,   515,   516,   519,   524,   527,   531,   532,
     532,   534,   537,   538,   548,   556,   547,   565,   564,   597,
     598,   607,   607,   614,   616,   614,   629,   630,   639,   640,
     643,   644,   647,   655,   656,   664,   673,   674,   677,   677,
     680,   681,   684,   691,   691,   694,   694,   697,   698,   697,
     701,   703,   705,   706,   705,   709,   710,   711,   709,   714,
     715,   714,   718,   719,   720,   718,   723,   724,   723,   727,
     728,   729,   727,   732,   733,   732,   736,   742,   741,   748,
     748,   751,   762,   763,   766,   767,   770,   775,   776,   779,
     780,   783,   784,   787,   788,   789,   796,   802,   805,   806,
     811,   818,   825,   826,   829,   830,   833,   834,   837,   842,
     842,   849,   850,   853,   854,   857,   858,   861,   865,   869,
     875,   879,   883,   883,   885,   889,   893,   894,   894,   896,
     896,   898,   898,   900,   900,   902,   906,   910,   915,   915,
     917,   921,   925,   929,   933,   933,   935,   936,   946,   947,
     950,   951,   954,   955,   958,   959,   960,   961,   967,   969,
     975,   977,   985,   986,   989,   990,   991,   991,   998,  1010,
    1011,  1011,  1015,  1015,  1017,  1023,  1023,  1023,  1023,  1023,
    1026,  1026,  1026,  1027,  1027,  1027,  1030,  1030,  1030,  1030,
    1031,  1031,  1031,  1031,  1031,  1031,  1034,  1034,  1034,  1035,
    1035,  1035,  1035,  1036,  1036,  1036,  1036,  1036,  1039,  1039,
    1045,  1045,  1045,  1045,  1045,  1045,  1045,  1045,  1046,  1046,
    1049,  1049,  1049,  1049,  1052,  1052,  1055,  1055,  1055,  1055,
    1055,  1055,  1056,  1056,  1056,  1057,  1057,  1057,  1057,  1057,
    1057,  1057,  1057,  1060,  1060,  1060,  1060,  1060,  1060,  1060,
    1063,  1063,  1063,  1066,  1066,  1066,  1066,  1066,  1066,  1066,
    1067,  1067,  1067,  1067,  1067,  1067,  1068,  1068,  1068,  1068,
    1069,  1069,  1069,  1069,  1070,  1070,  1070,  1097,  1097,  1102,
    1102,  1109,  1108,  1121,  1120,  1133,  1132,  1145,  1144,  1157,
    1156,  1169,  1168,  1182,  1181,  1195,  1194,  1207,  1206,  1220,
    1219,  1232,  1231,  1243,  1248,  1247,  1258,  1264,  1264,  1270,
    1270,  1276,  1276,  1282,  1282,  1288,  1296,  1296,  1302,  1302,
    1308,  1308,  1314,  1314,  1320,  1324,  1328,  1328,  1334,  1334,
    1340,  1340,  1346,  1346,  1352,  1352,  1358,  1358,  1364,  1364,
    1370,  1370,  1376,  1380,  1384,  1388,  1388,  1397,  1401,  1405,
    1409,  1413,  1417,  1421,  1425,  1429,  1433,  1437,  1441,  1445,
    1449,  1450,  1449,  1455,  1456,  1455,  1461,  1465,  1469,  1473,
    1477,  1481,  1488,  1492,  1496,  1500,  1507,  1511,  1518,  1518,
    1525,  1526,  1527,  1528,  1529,  1530,  1531,  1532,  1533,  1534,
    1537,  1538,  1539,  1540,  1541,  1542,  1543,  1544,  1545,  1546,
    1547,  1550,  1551,  1554,  1555,  1558,  1559,  1562,  1563,  1568,
    1569,  1570,  1573,  1574,  1577,  1578,  1581,  1582,  1585,  1586,
    1587,  1596,  1597,  1597,  1601,  1601,  1603,  1613,  1613,  1615,
    1616,  1616,  1621,  1621,  1623,  1623,  1625,  1629,  1629,  1631,
    1631,  1633,  1646,  1646,  1648,  1649,  1649,  1658,  1658,  1660,
    1660,  1662,  1662,  1664,  1672,  1672,  1674,  1674,  1676,  1676,
    1678,  1686,  1686,  1688,  1688,  1690,  1699,  1698,  1705,  1705,
    1707,  1715,  1715,  1717,  1717,  1719,  1727,  1727,  1729,  1729,
    1731,  1734,  1734,  1736,  1740,  1740,  1742,  1742,  1744,  1755,
    1755,  1755,  1755,  1755,  1755,  1755,  1758,  1758,  1758,  1758,
    1758,  1758,  1762,  1762,  1762,  1762,  1762,  1762,  1766,  1766,
    1766,  1766,  1766,  1766,  1770,  1770,  1770,  1770,  1770,  1774,
    1774,  1774,  1774,  1774,  1780,  1781,  1784,  1785,  1788,  1795,
    1796,  1799,  1804,  1811,  1811,  1811,  1811,  1811,  1811,  1811
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
  "KW_ABORT", "KW_STEP", "KW_WHERE", "KW_CREDUCE", "KW_SREDUCE", "KW_DUMP",
  "KW_PROFILE", "KW_NUMBER", "KW_RAT", "KW_COLOR", "SIMPLE_NUMBER",
  "KW_PWD", "KW_CD", "KW_PUSHD", "KW_POPD", "KW_LS", "KW_LOAD", "KW_QUIT",
  "KW_EOF", "KW_ENDM", "KW_IMPORT", "KW_ENDV", "KW_SORT", "KW_SUBSORT",
  "KW_OP", "KW_OPS", "KW_MSGS", "KW_VAR", "KW_CLASS", "KW_SUBCLASS",
  "KW_MB", "KW_CMB", "KW_EQ", "KW_CEQ", "KW_RL", "KW_CRL", "KW_IS",
  "KW_FROM", "KW_ARROW", "KW_ARROW2", "KW_PARTIAL", "KW_IF", "':'", "'='",
  "'('", "')'", "'.'", "'<'", "'['", "']'", "','", "'|'", "KW_LABEL",
  "KW_TO", "KW_COLON2", "'+'", "'*'", "'{'", "'}'", "KW_ASSOC", "KW_COMM",
  "KW_ID", "KW_IDEM", "KW_ITER", "KW_LEFT", "KW_RIGHT", "KW_PREC",
  "KW_GATHER", "KW_METADATA", "KW_STRAT", "KW_POLY", "KW_MEMO",
  "KW_FROZEN", "KW_CTOR", "KW_LATEX", "KW_SPECIAL", "KW_CONFIG", "KW_OBJ",
  "KW_MSG", "KW_DITTO", "KW_FORMAT", "KW_ID_HOOK", "KW_OP_HOOK",
  "KW_TERM_HOOK", "KW_IN", "IDENTIFIER", "NUMERIC_ID", "ENDS_IN_DOT",
  "FORCE_LOOKAHEAD", "$accept", "top", "item", "directive", "$@1", "$@2",
  "$@3", "$@4", "$@5", "moduleExprDot", "moduleExpr", "moduleExpr2",
  "moduleExpr3", "renameExpr", "instantExpr", "$@6", "parenExpr",
  "argList", "renaming", "$@7", "$@8", "renaming2", "mappingList",
  "mapping", "$@9", "$@10", "fromSpec", "$@11", "toAttributes", "$@12",
  "toAttributeList", "toAttribute", "$@13", "$@14", "$@15", "view", "$@16",
  "$@17", "$@18", "viewDecList", "skipStrayArrow", "viewDeclaration",
  "$@19", "sortDot", "viewEndOpMap", "$@20", "$@21", "$@22", "endBubble",
  "parenBubble", "$@23", "module", "$@24", "$@25", "dot", "parameters",
  "parameterList", "parameter", "colon2", "badType", "typeDot",
  "startModule", "decList", "declaration", "$@26", "$@27", "$@28", "$@29",
  "$@30", "$@31", "$@32", "$@33", "$@34", "$@35", "$@36", "$@37", "$@38",
  "$@39", "$@40", "$@41", "$@42", "$@43", "$@44", "$@45", "$@46", "$@47",
  "$@48", "classDef", "cPairList", "cPair", "varNameList", "opNameList",
  "simpleOpName", "domainRangeAttr", "skipStrayColon", "dra2", "rangeAttr",
  "typeAttr", "arrow", "typeList", "typeName", "$@49", "sortNames",
  "attributes", "attributeList", "idKeyword", "attribute", "$@50", "$@51",
  "$@52", "$@53", "$@54", "$@55", "$@56", "identity", "idList", "hookList",
  "hook", "expectedIs", "expectedDot", "sortNameList", "subsortList",
  "$@57", "sortName", "sortNameFrag", "$@58", "sortNameFrags", "$@59",
  "token", "tokenBarDot", "tokenBarColon", "sortToken", "endsInDot",
  "inert", "identifier", "startKeyword", "startKeyword2", "midKeyword",
  "attrKeyword", "attrKeyword2", "command", "$@60", "$@61", "$@62", "$@63",
  "$@64", "$@65", "$@66", "$@67", "$@68", "$@69", "$@70", "$@71", "$@72",
  "$@73", "$@74", "$@75", "$@76", "$@77", "$@78", "$@79", "$@80", "$@81",
  "$@82", "$@83", "$@84", "$@85", "$@86", "$@87", "$@88", "$@89", "$@90",
  "$@91", "$@92", "$@93", "$@94", "$@95", "printOption", "traceOption",
  "polarity", "select", "exclude", "conceal", "search", "match",
  "optDebug", "optNumber", "importMode", "moduleAndTerm", "$@96", "inEnd",
  "$@97", "numberModuleTerm", "$@98", "$@99", "numberModuleTerm1", "$@100",
  "$@101", "numberModuleTerm2", "$@102", "$@103", "numbersModuleTerm",
  "$@104", "$@105", "numbersModuleTerm1", "$@106", "$@107", "$@108",
  "numbersModuleTerm2", "$@109", "$@110", "$@111", "numbersModuleTerm3",
  "$@112", "$@113", "numbersModuleTerm4", "$@114", "$@115",
  "numbersModuleTerm5", "$@116", "$@117", "numbersModuleTerm6", "$@118",
  "$@119", "miscEndBubble", "$@120", "initialEndBubble", "$@121", "$@122",
  "cTokenBarIn", "cTokenBarLeftIn", "cTokenBarDotNumber",
  "cTokenBarDotRight", "cTokenBarDotCommaNumber", "cTokenBarDotCommaRight",
  "opSelect", "endSelect", "badSelect", "cOpNameList", "cSimpleOpName",
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
     375,    58,    61,    40,    41,    46,    60,    91,    93,    44,
     124,   376,   377,   378,    43,    42,   123,   125,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   168,   169,   169,   170,   170,   170,   170,   172,   171,
     173,   171,   171,   174,   171,   175,   171,   171,   176,   171,
     171,   171,   177,   177,   177,   177,   177,   177,   177,   178,
     178,   179,   179,   180,   180,   180,   181,   183,   182,   184,
     185,   185,   187,   188,   186,   189,   189,   190,   190,   191,
     191,   192,   193,   191,   195,   194,   194,   197,   196,   196,
     198,   198,   199,   200,   199,   201,   199,   202,   199,   204,
     205,   206,   203,   207,   207,   208,   208,   209,   209,   210,
     209,   209,   211,   211,   213,   214,   212,   215,   212,   216,
     216,   218,   217,   220,   221,   219,   222,   222,   223,   223,
     224,   224,   225,   226,   226,   227,   228,   228,   229,   229,
     230,   230,   231,   232,   231,   233,   231,   234,   235,   231,
     231,   231,   236,   237,   231,   238,   239,   240,   231,   241,
     242,   231,   243,   244,   245,   231,   246,   247,   231,   248,
     249,   250,   231,   251,   252,   231,   231,   253,   231,   254,
     231,   231,   255,   255,   256,   256,   257,   258,   258,   259,
     259,   260,   260,   261,   261,   261,   262,   262,   263,   263,
     263,   264,   265,   265,   266,   266,   267,   267,   268,   269,
     268,   270,   270,   271,   271,   272,   272,   273,   273,   273,
     274,   274,   275,   274,   274,   274,   274,   276,   274,   277,
     274,   278,   274,   279,   274,   274,   274,   274,   280,   274,
     274,   274,   274,   274,   281,   274,   274,   274,   282,   282,
     283,   283,   284,   284,   285,   285,   285,   285,   286,   286,
     287,   287,   288,   288,   289,   289,   290,   289,   291,   292,
     293,   292,   295,   294,   294,   296,   296,   296,   296,   296,
     297,   297,   297,   297,   297,   297,   298,   298,   298,   298,
     298,   298,   298,   298,   298,   298,   299,   299,   299,   299,
     299,   299,   299,   299,   299,   299,   299,   299,   300,   300,
     301,   301,   301,   301,   301,   301,   301,   301,   301,   301,
     302,   302,   302,   302,   303,   303,   304,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   304,   305,   305,   305,   305,   305,   305,   305,
     306,   306,   306,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   307,   307,   307,
     307,   307,   307,   307,   307,   307,   307,   309,   308,   310,
     308,   311,   308,   312,   308,   313,   308,   314,   308,   315,
     308,   316,   308,   317,   308,   318,   308,   319,   308,   320,
     308,   321,   308,   308,   322,   308,   308,   323,   308,   324,
     308,   325,   308,   326,   308,   308,   327,   308,   328,   308,
     329,   308,   330,   308,   308,   308,   331,   308,   332,   308,
     333,   308,   334,   308,   335,   308,   336,   308,   337,   308,
     338,   308,   308,   308,   308,   339,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     340,   341,   308,   342,   343,   308,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   344,   308,
     345,   345,   345,   345,   345,   345,   345,   345,   345,   345,
     346,   346,   346,   346,   346,   346,   346,   346,   346,   346,
     346,   347,   347,   348,   348,   349,   349,   350,   350,   351,
     351,   351,   352,   352,   353,   353,   354,   354,   355,   355,
     355,   356,   357,   356,   359,   358,   358,   361,   360,   360,
     362,   360,   364,   363,   365,   363,   363,   367,   366,   368,
     366,   366,   370,   369,   369,   371,   369,   373,   372,   374,
     372,   375,   372,   372,   377,   376,   378,   376,   379,   376,
     376,   381,   380,   382,   380,   380,   384,   383,   385,   383,
     383,   387,   386,   388,   386,   386,   390,   389,   391,   389,
     389,   393,   392,   392,   395,   394,   396,   394,   394,   397,
     397,   397,   397,   397,   397,   397,   398,   398,   398,   398,
     398,   398,   399,   399,   399,   399,   399,   399,   400,   400,
     400,   400,   400,   400,   401,   401,   401,   401,   401,   402,
     402,   402,   402,   402,   403,   403,   404,   404,   405,   406,
     406,   407,   407,   408,   408,   408,   408,   408,   408,   408
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
       3,     0,     3,     4,     0,     3,     1,     0,     4,     0,
       4,     0,     4,     0,     4,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     3,     3,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     4,     5,     4,     0,     4,     5,     5,     6,
       5,     5,     6,     5,     5,     5,     5,     6,     5,     4,
       0,     0,     7,     0,     0,     8,     4,     5,     5,     5,
       4,     5,     2,     2,     2,     2,     5,     4,     0,     3,
       1,     1,     2,     2,     1,     1,     1,     1,     1,     1,
       0,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     0,     1,     0,     1,     1,
       1,     1,     0,     3,     0,     3,     1,     0,     3,     1,
       0,     3,     0,     3,     0,     3,     1,     0,     3,     0,
       3,     1,     0,     3,     1,     0,     3,     0,     3,     0,
       3,     0,     3,     1,     0,     3,     0,     3,     0,     3,
       1,     0,     3,     0,     3,     1,     0,     3,     0,     3,
       1,     0,     3,     0,     3,     1,     0,     3,     0,     3,
       1,     0,     3,     1,     0,     3,     0,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   448,   108,   109,    69,   351,   374,   479,   480,   483,
     482,   371,   481,     0,     0,     0,   347,     0,     0,     0,
     484,     0,     0,     0,     0,   353,   355,   349,    12,    13,
      15,    17,    18,    10,    20,    21,    91,     8,     0,     2,
       6,     5,   376,     4,    93,     7,   367,   369,     0,     0,
       0,     0,     0,     0,     0,     0,   460,     0,     0,     0,
       0,     0,     0,   488,   489,   490,     0,     0,   430,   386,
     392,     0,     0,   388,     0,     0,   390,   396,   398,   400,
     402,   404,   406,   408,   410,   415,   473,   474,   476,   475,
     377,   379,     0,   477,   478,   383,   381,     0,   442,   443,
     444,   445,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,   357,   359,   361,   363,   365,
     487,   449,   311,   296,   312,   297,   298,   299,   300,   302,
     301,   303,   304,   305,   306,   307,   308,   309,   310,   289,
     288,   315,   318,   316,   319,   314,   317,   249,   313,   320,
     321,   292,   285,   287,   293,   286,   283,   284,   281,   282,
     323,   324,   325,   326,   327,   328,   329,   330,   331,   339,
     332,   333,   334,   338,   335,   336,   337,   340,   341,   294,
     342,   343,   344,   345,   346,   280,   291,     0,   290,   245,
     246,   295,   247,   248,   322,   563,   554,   564,   561,   562,
     565,   556,   559,   560,   558,   352,   491,   492,   375,   569,
     570,   497,   568,   571,   566,   567,   372,   499,   500,   433,
       0,     0,     0,     0,     0,     0,     0,   468,   469,   470,
     462,   464,   461,   463,   465,   466,   467,     0,   454,   450,
     451,     0,     0,   455,   456,   457,   458,   459,     0,     0,
     471,   472,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   394,   395,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    89,    90,   348,     0,     0,     0,   354,   356,   350,
      14,    16,    19,    11,    92,     9,    94,   512,   368,   514,
     515,   370,     0,     0,     0,     0,     0,   486,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   453,   452,     0,
       0,   447,   429,   436,     0,     0,     0,     0,   440,   608,
     605,   606,   609,   607,   603,   604,   431,   387,   393,   414,
     412,     0,   389,   391,   397,   399,   401,   403,   405,   407,
     409,   411,   416,   598,   602,   378,   595,     0,   600,   601,
     380,   384,   382,   385,    99,     0,     0,   358,   360,   362,
     364,   366,   373,     0,    70,    29,    31,    32,    34,    33,
      35,   555,   494,   496,   557,   493,   576,   551,   573,   574,
     577,   575,   572,   502,   553,   498,   506,   504,   501,   434,
       0,     0,   424,   418,   420,   423,   421,   417,   428,     0,
     426,   425,   446,   438,   441,   437,   439,     0,   413,   596,
     594,   597,   599,     0,   229,   588,   585,   586,   519,   587,
     584,   517,   513,   523,   521,   516,     0,     0,     0,     0,
      37,     0,     0,     0,     0,     0,   419,   422,   427,     0,
       0,   101,     0,   228,   111,     0,     0,     0,    39,    30,
       0,    42,    36,     0,   495,   552,   582,   580,   507,   583,
     581,   578,   579,   503,   511,   509,   505,     0,   432,     0,
      98,   104,   103,     0,     0,   541,   520,   545,   543,   593,
     591,   524,   526,   592,   589,   590,   518,   530,   528,   522,
      71,     0,     0,    41,     0,     0,   435,   100,   102,     0,
      95,     0,   113,   115,   117,     0,     0,     0,     0,   149,
     122,   125,   129,   132,   136,   139,   143,   110,     0,     0,
       0,     0,     0,   229,    46,     0,    51,     0,    43,    45,
      48,     0,    38,   508,   510,   151,   249,   251,   252,    28,
     112,     0,   231,   231,   231,   231,     0,   250,   253,   254,
     255,   233,     0,     0,   261,   264,   262,   265,   263,   259,
     260,   162,     0,   160,   161,   256,   257,   258,     0,     0,
     158,   147,     0,     0,     0,     0,     0,     0,     0,     0,
     546,   542,   550,   548,   544,   525,   531,   527,   535,   533,
     529,    74,   275,   273,   274,   269,   270,   276,   277,   271,
     272,   266,     0,   238,   239,   267,   268,    56,     0,     0,
       0,    40,     0,   230,    25,    26,    24,    22,    23,     0,
       0,   236,   118,     0,   159,     0,    76,   157,     0,     0,
     123,   126,   130,   133,   137,   140,   144,     0,     0,     0,
       0,     0,     0,   240,    54,     0,     0,    44,    47,    27,
      30,    96,    97,   114,   232,   235,   116,   234,   233,     0,
     174,   175,   179,   105,   165,   120,   164,     0,   177,   178,
     146,    75,     0,   152,     0,   150,     0,     0,     0,     0,
       0,     0,     0,   547,   549,   536,   532,   540,   538,   534,
       0,    72,     0,    79,     0,    73,    49,     0,   177,    52,
      50,     0,   119,     0,   173,   171,   184,   167,   107,   121,
     231,   251,   252,   153,   154,     0,   250,   253,   254,   255,
     148,   124,   127,   131,   134,   138,   141,   145,     0,     0,
      81,     0,     0,     0,   244,     0,     0,    59,   237,     0,
     182,     0,   231,   166,   169,   170,     0,   163,   176,   106,
       0,     0,     0,     0,     0,   537,   539,     0,    84,    87,
      80,    76,   242,   241,     0,    57,    53,   180,     0,   190,
     191,   187,   194,   195,     0,     0,     0,     0,     0,     0,
       0,   205,   207,   206,     0,     0,   210,   211,   212,   217,
       0,     0,   192,   186,   172,   168,   155,   156,   128,   135,
     142,    83,    77,   231,   177,     0,     0,     0,    55,     0,
     181,   188,   189,   196,   197,   213,   201,   203,   208,   214,
       0,   199,   183,   185,   219,    82,     0,    88,    78,   243,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   223,     0,   218,   193,     0,
      62,    63,    67,    65,    58,    60,   221,     0,     0,     0,
       0,     0,   224,     0,     0,   216,   222,     0,     0,     0,
       0,     0,   198,   220,   202,   204,   209,   215,   225,   226,
     227,   200,    85,     0,     0,     0,     0,    64,    68,    66,
      86
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    38,    39,    40,   110,   108,   105,   106,   107,   560,
     384,   385,   386,   387,   388,   473,   389,   512,   472,   511,
     629,   548,   549,   550,   627,   757,   665,   718,   786,   829,
     854,   855,   889,   891,   890,    41,    50,   448,   543,   661,
     692,   715,   752,   822,   780,   824,   906,   825,   404,   364,
     109,    43,   112,   374,   673,   434,   460,   461,   493,   684,
     729,    44,   494,   537,   571,   572,   573,   679,   593,   696,
     594,   697,   772,   595,   698,   596,   699,   773,   597,   700,
     598,   701,   774,   599,   702,   648,   592,   694,   733,   734,
     589,   582,   583,   685,   766,   767,   686,   725,   687,   727,
     688,   723,   759,   762,   811,   812,   813,   844,   856,   866,
     857,   858,   859,   860,   868,   877,   864,   865,   464,   634,
     639,   640,   678,   689,   623,   717,   755,   827,   390,   565,
     584,   624,   566,   188,   189,   625,   191,   192,   193,   194,
      45,    92,   104,    51,   102,   103,   302,   303,   304,   305,
     306,   113,   114,    53,    52,   279,   280,   285,   284,   261,
     266,   269,   262,   270,   271,   272,   273,   274,   275,   276,
     277,   278,   260,   427,   315,   455,    49,   248,   237,   252,
      90,    91,    95,    46,    47,    48,   308,    68,   205,   312,
     394,   451,   216,   313,   314,   405,   453,   454,   483,   514,
     515,   298,   375,   376,   442,   466,   465,   467,   506,   540,
     541,   542,   607,   659,   660,   706,   748,   749,   496,   538,
     539,   601,   657,   658,   406,   452,   206,   310,   311,   207,
     218,   407,   485,   444,   508,   365,   430,   366,   367,   368,
     369
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -725
static const yytype_int16 yypact[] =
{
     624,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,   392,   325,    18,  -725,    -6,    55,   -29,
    -725,   -65,   -55,   -34,    27,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   104,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   508,    51,
    1063,   118,   118,   369,   122,   264,   440,   279,   164,   140,
     140,   140,   -27,  -725,  -725,  -725,   120,   140,  -725,  -725,
    -725,   168,   -37,  -725,    93,    97,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,   -81,  -725,  -725,  -725,  -725,    78,  -725,  -725,
    -725,  -725,   118,   118,   -81,   241,   249,   255,   252,   138,
     286,  -725,  1063,   389,   369,  -725,  -725,  -725,  -725,  -725,
     187,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,   184,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
     275,   140,   140,   140,   140,   140,   140,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,   140,  -725,  -725,
    -725,    35,   293,  -725,  -725,  -725,  -725,  -725,   140,   140,
    -725,  -725,   153,   177,   183,   140,   140,   140,   140,   190,
     -64,   -81,   -81,   202,   209,   254,   -81,  -725,  -725,   -81,
     -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,   -81,  2150,
    2150,  -725,  -725,  -725,  2150,  2150,   239,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,   118,   369,   389,   389,   369,  -725,   250,   787,
     -81,   -80,   -81,   415,   -81,   -64,   140,   140,   269,   276,
     280,   289,   290,   291,   294,   140,   296,  -725,  -725,   297,
     298,  -725,  -725,  -725,   300,   301,   304,   307,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,   309,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,   435,  -725,  -725,
    -725,  -725,  -725,  -725,   223,   447,   -81,  -725,  -725,  -725,
    -725,  -725,  -725,   787,   170,   227,   282,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
     310,   311,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   315,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,   140,  -725,  -725,
    -725,  -725,  -725,  1063,   285,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,     9,   787,   321,   318,
    -725,   -81,   -81,   612,   -81,   140,  -725,  -725,  -725,   317,
     127,  -725,   -62,  -725,  -725,   645,   661,   -81,  -725,  -725,
     787,  -725,  -725,  1063,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,   322,  -725,  1063,
    -725,  -725,  -725,   787,   650,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
     170,     5,   162,  -725,   118,   -81,  -725,  -725,   170,   329,
    -725,   856,  -725,  -725,  -725,  1132,  1132,  1339,  1063,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   670,   -81,
     118,  2132,   -81,   285,  -725,  2090,  -725,  1063,  -725,   326,
    -725,  1063,  -725,  -725,  -725,  -725,   331,   215,   222,    26,
    -725,   323,    39,   102,   115,   334,   341,   234,   237,   262,
     273,  -725,  2090,   347,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,   925,  -725,  -725,  -725,  -725,  -725,   994,  1201,
    -725,  -725,  2090,   352,   355,   357,   358,   363,   368,   372,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,   367,   353,  -725,  -725,  -725,   374,   371,   381,
     111,  -725,   856,  -725,  -725,  -725,  -725,  -725,  -725,  1615,
    1546,  -725,  -725,  1408,  -725,  1408,   396,  -725,   376,  1546,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,   118,   -81,  2141,
     -81,    41,  2090,  -725,  -725,   377,  1063,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  1408,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  1684,  -725,  -725,
    -725,  -725,  1684,  1822,   390,  -725,   -81,   401,   -81,   402,
     -81,   403,  1408,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
     399,  -725,  2090,  -725,  1339,  -725,  -725,  2090,  -725,  -725,
    -725,  1956,  -725,  2090,  -725,  -725,   404,  1477,  -725,  -725,
     334,  -725,  -725,   408,  -725,   420,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   118,   -81,
    -725,   416,    37,  1270,   353,   169,  1889,   422,  -725,   225,
    -725,   722,   334,  -725,  -725,  -725,   156,  -725,  -725,  -725,
    1822,  1063,   -81,   -81,   -81,  -725,  -725,  1753,  -725,  -725,
    -725,   396,  -725,  -725,  2023,  -725,  -725,  -725,  2090,  -725,
    -725,  -725,  -725,  -725,   407,   410,   387,   434,   395,   438,
     442,  -725,   443,  -725,   448,   450,  -725,  -725,  -725,  -725,
     454,   700,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,   334,  -725,   -81,  1684,  2090,  -725,   -21,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
     251,  -725,  -725,  -725,   417,  -725,  1889,  -725,  -725,   353,
     419,   462,   463,   464,   179,  -725,   426,   426,   426,   426,
     586,  1063,  1063,  1063,   -42,  -725,   426,  -725,  -725,  2023,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,   -69,   -68,   -19,
     -11,   468,   470,   470,   470,  -725,  -725,   -10,   465,   426,
     589,   426,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,    -9,   471,    -7,   -81,  -725,  -725,  -725,
    -725
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   -36,
    -372,  -725,  -725,  -506,  -504,  -725,  -497,  -725,  -725,  -725,
    -725,  -725,  -725,   -28,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -251,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -177,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   -91,     0,
    -725,  -725,  -725,  -725,  -503,  -725,  -725,   117,  -725,  -678,
    -212,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -155,
     -98,    91,  -432,  -605,  -725,  -725,  -148,  -725,  -724,  -693,
    -654,  -725,  -725,  -725,  -725,  -725,  -192,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -563,  -725,  -244,    79,  -536,
     -57,    36,  -725,  -433,  -687,  -725,  -725,  -725,   -40,  -659,
    -523,  -725,  -725,  -516,  -479,   -43,  -725,  -498,  -505,  -382,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   -24,
     609,  -725,  -725,  -725,  -725,  -725,  -725,  -725,   -49,  -725,
    -725,  -725,   -95,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,    87,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,  -725,
    -725,  -725,  -725,  -725,  -373,  -725,   -35,  -725,  -725,  -725,
    -101,  -434,  -530,  -725,  -725,    62,  -725,   267,  -725,   287,
    -234
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -486
static const yytype_int16 yytable[] =
{
      42,   283,   443,   208,   590,   567,   544,   190,   603,   724,
     187,   446,   300,   289,   728,   562,   570,   563,   217,   301,
     587,   587,   587,   569,   564,   756,   346,   635,   636,   637,
     754,   498,   784,   726,   735,   255,   253,   254,   730,    97,
     690,   392,   710,   259,   281,   281,   585,   585,   585,   765,
      86,    87,   264,   287,   288,   892,   894,   339,   256,   491,
      98,   250,   251,   340,   341,   342,   647,    93,    94,   190,
      99,   492,   296,   768,   722,   469,   325,   587,   299,   217,
     484,   409,   885,   587,   587,   282,   282,    86,    87,    88,
      89,   100,   497,   507,   265,   893,   893,   747,   510,   343,
     344,   345,   768,   585,   111,   895,   545,   609,   546,   585,
     585,   735,   622,   896,   901,   907,   567,   909,   861,   862,
     863,   518,   869,   257,   850,   851,   562,   570,   563,   708,
     828,   846,   852,   468,   569,   564,   547,   676,   853,   641,
     849,   711,   712,   447,   713,   893,   695,   714,   728,   561,
     644,  -279,   101,   893,   893,   893,   644,   893,   778,   641,
    -291,  -291,  -291,   626,   633,   602,   250,   251,   608,   779,
     347,   348,   730,   -32,   -32,   352,   121,   736,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   739,   249,
     626,   590,   768,   219,   769,   738,   258,   318,   319,   320,
     321,   322,   323,   300,   300,   263,   674,   677,   378,   587,
     626,   381,   545,   324,   546,   888,   677,   326,   267,   391,
     393,   395,   268,   408,   329,   330,   814,   633,   286,   716,
     647,   334,   335,   336,   337,   585,   -34,   -34,   -34,   195,
     633,   196,   547,   197,   290,   198,   199,   200,   587,   -33,
     -33,   -33,   291,   377,   736,   293,   489,   626,   626,   292,
     670,   626,   294,   626,   490,   739,   190,   626,   217,   299,
     299,   217,   738,   680,   585,   681,   307,   220,   331,   751,
     626,   201,   202,   203,   204,   445,   707,   845,   674,   295,
     760,   551,   410,   411,   878,   879,   880,   626,   782,   552,
     309,   419,   332,   887,   447,   626,   783,   874,   333,   221,
     626,   222,   223,   224,   225,   338,   238,   239,   240,   241,
     626,   242,   316,   317,   850,   851,   903,   349,   905,   226,
     626,    69,   852,    70,   350,   626,   327,   328,   853,   626,
     190,   626,   370,   351,   823,   626,   371,   372,    71,  -292,
    -292,  -292,   243,   787,   788,   830,  -293,  -293,  -293,   433,
     474,   475,   449,   486,   373,   244,   245,   246,  -290,  -290,
    -290,  -246,  -246,  -246,   626,   382,   509,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     190,   379,   380,   462,   412,   626,  -247,  -247,  -247,    54,
     463,   413,   626,   459,   190,   414,   626,  -248,  -248,  -248,
      85,   861,   862,   863,   415,   416,   417,    55,   450,   418,
      56,   420,   421,   422,   554,   423,   424,   190,    57,   425,
     190,   487,   426,   513,   428,   456,   457,    58,   247,    59,
     458,   471,   488,    60,   626,   626,   190,   516,   604,   462,
     190,   610,   227,   470,   555,   630,  -278,   632,    61,   633,
      62,    63,    64,    65,   626,   553,   638,    66,   642,   228,
     229,   230,   231,   650,   232,   233,   651,    67,   568,   652,
     653,   654,   586,   586,   586,   190,   655,   626,   591,   663,
     209,   605,   196,   656,   210,   664,   211,   212,   213,   662,
     234,   235,   236,   666,   190,   667,   693,   628,   190,   719,
     209,   631,   196,   691,   210,   740,   297,   212,   213,   115,
     116,   742,   744,   746,   750,   581,   581,   117,   118,   119,
     120,   761,   201,   214,   215,   204,   396,   770,   397,   586,
     281,   771,   398,   399,   400,   586,   586,   831,   777,   785,
     832,   833,   201,   214,   215,   204,   339,   834,    36,   835,
     429,   836,   340,   341,   342,   837,   838,   704,   435,   709,
     397,   839,   281,   840,   436,   437,   438,   841,   401,   402,
     403,   282,   581,   870,   867,   871,   872,   873,   581,   568,
     876,   881,   897,    36,   904,   908,   669,   902,   343,   344,
     345,   363,   668,   875,   826,   741,   517,   743,   703,   745,
     439,   440,   441,   282,   848,   816,   753,   588,   815,   843,
     886,   721,   611,   190,    -3,     1,   720,    96,   649,     0,
       2,     3,     4,     5,   431,  -485,  -485,     6,     7,     8,
       9,    10,    11,  -485,  -485,  -485,  -485,    12,    13,    14,
     737,   519,    15,     0,   432,     0,    16,     0,   776,     0,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   586,     0,     0,     0,    18,     0,     0,     0,     0,
       0,   818,   819,   820,     0,     0,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,   775,
       0,    20,    21,    22,    23,    24,    25,    26,    27,     0,
     586,     0,     0,     0,    28,    29,    30,    31,    32,    33,
      34,    35,     0,     0,     0,     0,     0,   737,   190,     0,
       0,   817,     0,   476,   847,   397,     0,   281,     0,   477,
     478,   479,     0,     0,     0,     0,     0,    36,   520,   521,
       0,   522,   523,   524,   525,   526,   527,   528,   529,   530,
     531,   532,   533,   534,   535,     0,   396,     0,   397,     0,
     281,     0,   398,   399,   400,   480,   481,   482,   282,     0,
       0,     0,   499,     0,   397,     0,   281,    37,   500,   501,
     502,   476,     0,   397,     0,   281,     0,   477,   600,   479,
       0,     0,     0,     0,     0,     0,     0,   536,   401,   402,
     495,   282,     0,     0,     0,   910,     0,     0,   190,   190,
     190,   882,   883,   884,   503,   504,   505,   282,   842,     0,
       0,     0,     0,   480,   481,   482,   282,     0,   789,   790,
     791,   792,   793,   794,   795,   796,   797,   798,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   810,
     789,   790,   791,   792,   793,   794,   795,   796,   797,   798,
     799,   800,   801,   802,   803,   804,   805,   806,   807,   808,
     809,   810,   898,   899,   900,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     383,     0,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
       0,   185,     0,   186,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   383,
       0,   556,   148,   149,   150,   557,   152,   153,   558,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,     0,
     185,     0,   559,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   574,   575,   576,   577,   643,   578,    36,     0,
     579,   580,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,     0,   185,
       0,   186,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   574,   575,   576,   577,   645,   578,    36,     0,   579,
     580,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,     0,   185,     0,
     186,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,     0,     0,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,     0,   185,     0,   186,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   574,
     575,   576,   577,     0,   578,    36,     0,   579,   580,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,     0,   185,     0,   186,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   574,   575,
     576,   577,   646,   578,     0,     0,   579,   580,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,     0,   185,     0,   186,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   574,   575,   576,
     577,   781,   578,     0,     0,   579,   580,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,     0,   185,     0,   186,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   574,   575,   576,   577,
       0,   578,     0,     0,   579,   580,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,     0,   185,     0,   186,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   612,     0,   680,   613,   681,   614,     0,
     615,     0,     0,     0,     0,   682,     0,     0,   616,   617,
     618,     0,   619,   620,     0,     0,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,     0,   621,     0,   683,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   612,     0,     0,   613,     0,   614,   763,   615,
       0,     0,   764,     0,   682,     0,     0,   616,   617,   618,
       0,   619,   620,     0,     0,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
       0,   621,     0,   683,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   612,     0,     0,   613,     0,   614,     0,   615,     0,
       0,   671,   675,     0,     0,     0,   616,   617,   618,     0,
     619,   620,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,     0,
     621,     0,   672,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     612,     0,     0,   613,     0,   614,     0,   615,     0,     0,
     671,     0,     0,     0,     0,   616,   617,   618,     0,   619,
     620,     0,     0,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,     0,   621,
       0,   672,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   612,
       0,     0,   613,     0,   614,     0,   615,     0,     0,     0,
       0,   682,     0,     0,   616,   617,   618,     0,   619,   620,
       0,     0,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,     0,   621,     0,
     683,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   612,     0,
       0,   613,     0,   614,     0,   615,     0,     0,     0,     0,
       0,     0,     0,   616,   617,   618,     0,   619,   620,     0,
       0,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,     0,   621,     0,   821,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,     0,     0,     0,   148,   149,
     150,   731,   152,   153,   732,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,     0,   185,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   612,     0,   680,   613,   681,   614,
       0,   615,     0,     0,     0,     0,   682,     0,     0,   616,
     617,   618,     0,   619,   620,     0,     0,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,     0,   621,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   612,     0,     0,   613,     0,   614,     0,   615,     0,
       0,     0,   758,     0,     0,     0,   616,   617,   618,     0,
     619,   620,     0,     0,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,     0,
     621,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   612,     0,
       0,   613,     0,   614,     0,   615,     0,     0,     0,     0,
     682,     0,     0,   616,   617,   618,     0,   619,   620,     0,
       0,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,     0,   621,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   612,     0,     0,   613,     0,
     614,     0,   615,     0,     0,     0,     0,     0,     0,     0,
     616,   617,   618,     0,   619,   620,     0,     0,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   396,   621,   397,     0,   281,     0,   398,
     399,   400,   476,     0,   397,     0,   281,     0,   477,   705,
     479,   339,     0,    36,     0,     0,     0,   340,   341,   342,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   401,   402,   606,   282,     0,
       0,     0,     0,     0,   480,   481,   482,   282,     0,     0,
       0,     0,     0,   343,   344,   345,   363
};

static const yytype_int16 yycheck[] =
{
       0,    92,   375,    52,   527,   521,     1,    50,   538,   687,
      50,   383,   113,   104,   692,   521,   521,   521,    53,   114,
     525,   526,   527,   521,   521,   718,   260,   563,   564,   565,
     717,   465,   756,   687,   693,    62,    60,    61,   692,    68,
     645,   121,     1,    67,   125,   125,   525,   526,   527,   727,
      32,    33,    89,   102,   103,   124,   124,   121,    85,   121,
     125,    26,    27,   127,   128,   129,   589,    73,    74,   112,
     125,   133,   112,   727,   679,   447,    41,   582,   113,   114,
     453,   315,   124,   588,   589,   166,   166,    32,    33,    71,
      72,   125,   465,   466,   131,   164,   164,   702,   470,   163,
     164,   165,   756,   582,     0,   124,   101,   541,   103,   588,
     589,   770,   545,   124,   124,   124,   632,   124,   160,   161,
     162,   493,   846,   150,   145,   146,   632,   632,   632,   659,
     784,   824,   153,   124,   632,   632,   131,   640,   159,   572,
     827,   100,   101,   134,   103,   164,   649,   106,   826,   521,
     582,   125,   125,   164,   164,   164,   588,   164,   121,   592,
     134,   135,   136,   545,   125,   538,    26,    27,   541,   132,
     261,   262,   826,   134,   135,   266,   125,   693,   269,   270,
     271,   272,   273,   274,   275,   276,   277,   278,   693,    25,
     572,   714,   846,    71,   730,   693,    76,   221,   222,   223,
     224,   225,   226,   304,   305,    37,   639,   640,   303,   714,
     592,   306,   101,   237,   103,   869,   649,   241,   125,   310,
     311,   312,   125,   314,   248,   249,   762,   125,   150,   662,
     753,   255,   256,   257,   258,   714,   134,   135,   136,   121,
     125,   123,   131,   125,     3,   127,   128,   129,   753,   134,
     135,   136,     3,   302,   770,     3,   129,   639,   640,     4,
     632,   643,   124,   645,   137,   770,   309,   649,   303,   304,
     305,   306,   770,   117,   753,   119,    89,    13,   125,   712,
     662,   163,   164,   165,   166,   376,   659,   823,   721,     3,
     723,   129,   316,   317,   857,   858,   859,   679,   129,   137,
     116,   325,   125,   866,   134,   687,   137,   128,   125,    45,
     692,    47,    48,    49,    50,   125,    37,    38,    39,    40,
     702,    42,    47,    48,   145,   146,   889,   125,   891,    65,
     712,     6,   153,     8,   125,   717,    43,    44,   159,   721,
     383,   723,   280,    89,   777,   727,   284,   285,    23,   134,
     135,   136,    73,   128,   129,   788,   134,   135,   136,   136,
     451,   452,   135,   454,   125,    86,    87,    88,   134,   135,
     136,   134,   135,   136,   756,   125,   467,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
     433,   304,   305,   433,   125,   777,   134,   135,   136,     7,
     115,   125,   784,   427,   447,   125,   788,   134,   135,   136,
      85,   160,   161,   162,   125,   125,   125,    25,   136,   125,
      28,   125,   125,   125,   515,   125,   125,   470,    36,   125,
     473,   455,   125,   473,   125,   125,   125,    45,   159,    47,
     125,   123,   125,    51,   826,   827,   489,   125,   539,   489,
     493,   542,    12,   132,   125,   129,   125,   134,    66,   125,
      68,    69,    70,    71,   846,   514,   125,    75,   121,    29,
      30,    31,    32,   121,    34,    35,   121,    85,   521,   122,
     122,   118,   525,   526,   527,   528,   118,   869,   528,   136,
     121,   540,   123,   121,   125,   121,   127,   128,   129,   132,
      60,    61,    62,   132,   547,   124,   130,   547,   551,   132,
     121,   551,   123,   117,   125,   125,   127,   128,   129,    11,
      12,   120,   120,   120,   125,   525,   526,    19,    20,    21,
      22,   127,   163,   164,   165,   166,   121,   129,   123,   582,
     125,   121,   127,   128,   129,   588,   589,   140,   132,   127,
     140,   164,   163,   164,   165,   166,   121,   123,   123,   164,
     125,   123,   127,   128,   129,   123,   123,   658,   121,   660,
     123,   123,   125,   123,   127,   128,   129,   123,   163,   164,
     165,   166,   582,   164,   167,   123,   123,   123,   588,   632,
     164,     5,   124,   123,     5,   124,   632,   132,   163,   164,
     165,   166,   630,   854,   781,   696,   489,   698,   657,   700,
     163,   164,   165,   166,   826,   770,   714,   526,   766,   811,
     864,   678,   543,   666,     0,     1,   666,    18,   592,    -1,
       6,     7,     8,     9,   367,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
     693,     1,    28,    -1,   367,    -1,    32,    -1,   749,    -1,
      36,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   714,    -1,    -1,    -1,    51,    -1,    -1,    -1,    -1,
      -1,   772,   773,   774,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    67,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   748,
      -1,    77,    78,    79,    80,    81,    82,    83,    84,    -1,
     753,    -1,    -1,    -1,    90,    91,    92,    93,    94,    95,
      96,    97,    -1,    -1,    -1,    -1,    -1,   770,   771,    -1,
      -1,   771,    -1,   121,   825,   123,    -1,   125,    -1,   127,
     128,   129,    -1,    -1,    -1,    -1,    -1,   123,    98,    99,
      -1,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,   121,    -1,   123,    -1,
     125,    -1,   127,   128,   129,   163,   164,   165,   166,    -1,
      -1,    -1,   121,    -1,   123,    -1,   125,   163,   127,   128,
     129,   121,    -1,   123,    -1,   125,    -1,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   157,   163,   164,
     165,   166,    -1,    -1,    -1,   906,    -1,    -1,   861,   862,
     863,   861,   862,   863,   163,   164,   165,   166,   128,    -1,
      -1,    -1,    -1,   163,   164,   165,   166,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   882,   883,   884,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,    -1,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,    -1,   166,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
      -1,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,    -1,   166,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,    -1,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
      -1,   166,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,    -1,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,   164,    -1,
     166,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,    -1,   166,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,    -1,   122,   123,    -1,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,   164,    -1,   166,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,    -1,    -1,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,    -1,   164,    -1,   166,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,    -1,    -1,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,    -1,   164,    -1,   166,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
      -1,   122,    -1,    -1,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,    -1,   166,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,   117,   118,   119,   120,    -1,
     122,    -1,    -1,    -1,    -1,   127,    -1,    -1,   130,   131,
     132,    -1,   134,   135,    -1,    -1,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,    -1,   164,    -1,   166,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,    -1,   118,    -1,   120,   121,   122,
      -1,    -1,   125,    -1,   127,    -1,    -1,   130,   131,   132,
      -1,   134,   135,    -1,    -1,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
      -1,   164,    -1,   166,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,   122,    -1,
      -1,   125,   126,    -1,    -1,    -1,   130,   131,   132,    -1,
     134,   135,    -1,    -1,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,    -1,   166,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,    -1,    -1,   118,    -1,   120,    -1,   122,    -1,    -1,
     125,    -1,    -1,    -1,    -1,   130,   131,   132,    -1,   134,
     135,    -1,    -1,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,    -1,   164,
      -1,   166,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,    -1,   118,    -1,   120,    -1,   122,    -1,    -1,    -1,
      -1,   127,    -1,    -1,   130,   131,   132,    -1,   134,   135,
      -1,    -1,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,    -1,   164,    -1,
     166,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,   122,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   130,   131,   132,    -1,   134,   135,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,    -1,   166,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,    -1,    -1,    -1,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,    -1,   164,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,    -1,   117,   118,   119,   120,
      -1,   122,    -1,    -1,    -1,    -1,   127,    -1,    -1,   130,
     131,   132,    -1,   134,   135,    -1,    -1,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,    -1,   164,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,    -1,    -1,   118,    -1,   120,    -1,   122,    -1,
      -1,    -1,   126,    -1,    -1,    -1,   130,   131,   132,    -1,
     134,   135,    -1,    -1,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,    -1,
     164,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,    -1,
      -1,   118,    -1,   120,    -1,   122,    -1,    -1,    -1,    -1,
     127,    -1,    -1,   130,   131,   132,    -1,   134,   135,    -1,
      -1,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,    -1,   164,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,    -1,    -1,   118,    -1,
     120,    -1,   122,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     130,   131,   132,    -1,   134,   135,    -1,    -1,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   121,   164,   123,    -1,   125,    -1,   127,
     128,   129,   121,    -1,   123,    -1,   125,    -1,   127,   128,
     129,   121,    -1,   123,    -1,    -1,    -1,   127,   128,   129,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   163,   164,   165,   166,    -1,
      -1,    -1,    -1,    -1,   163,   164,   165,   166,    -1,    -1,
      -1,    -1,    -1,   163,   164,   165,   166
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    14,    15,    16,
      17,    18,    23,    24,    25,    28,    32,    36,    51,    67,
      77,    78,    79,    80,    81,    82,    83,    84,    90,    91,
      92,    93,    94,    95,    96,    97,   123,   163,   169,   170,
     171,   203,   217,   219,   229,   308,   351,   352,   353,   344,
     204,   311,   322,   321,     7,    25,    28,    36,    45,    47,
      51,    66,    68,    69,    70,    71,    75,    85,   355,     6,
       8,    23,    52,    53,    54,    55,    56,    57,    58,    59,
      60,    61,    62,    63,    64,    85,    32,    33,    71,    72,
     348,   349,   309,    73,    74,   350,   348,    68,   125,   125,
     125,   125,   312,   313,   310,   174,   175,   176,   173,   218,
     172,     0,   220,   319,   320,    11,    12,    19,    20,    21,
      22,   125,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   164,   166,   296,   301,   302,
     303,   304,   305,   306,   307,   121,   123,   125,   127,   128,
     129,   163,   164,   165,   166,   356,   394,   397,   356,   121,
     125,   127,   128,   129,   164,   165,   360,   394,   398,    71,
      13,    45,    47,    48,    49,    50,    65,    12,    29,    30,
      31,    32,    34,    35,    60,    61,    62,   346,    37,    38,
      39,    40,    42,    73,    86,    87,    88,   159,   345,    25,
      26,    27,   347,   347,   347,    62,    85,   150,    76,   347,
     340,   327,   330,    37,    89,   131,   328,   125,   125,   329,
     331,   332,   333,   334,   335,   336,   337,   338,   339,   323,
     324,   125,   166,   216,   326,   325,   150,   356,   356,   216,
       3,     3,     4,     3,   124,     3,   296,   127,   369,   394,
     398,   360,   314,   315,   316,   317,   318,    89,   354,   116,
     395,   396,   357,   361,   362,   342,    47,    48,   347,   347,
     347,   347,   347,   347,   347,    41,   347,    43,    44,   347,
     347,   125,   125,   125,   347,   347,   347,   347,   125,   121,
     127,   128,   129,   163,   164,   165,   408,   216,   216,   125,
     125,    89,   216,   216,   216,   216,   216,   216,   216,   216,
     216,   216,   216,   166,   217,   403,   405,   406,   407,   408,
     403,   403,   403,   125,   221,   370,   371,   356,   360,   369,
     369,   360,   125,   123,   178,   179,   180,   181,   182,   184,
     296,   216,   121,   216,   358,   216,   121,   123,   127,   128,
     129,   163,   164,   165,   216,   363,   392,   399,   216,   408,
     347,   347,   125,   125,   125,   125,   125,   125,   125,   347,
     125,   125,   125,   125,   125,   125,   125,   341,   125,   125,
     404,   405,   407,   136,   223,   121,   127,   128,   129,   163,
     164,   165,   372,   392,   401,   216,   178,   134,   205,   135,
     136,   359,   393,   364,   365,   343,   125,   125,   125,   347,
     224,   225,   296,   115,   286,   374,   373,   375,   124,   178,
     132,   123,   186,   183,   216,   216,   121,   127,   128,   129,
     163,   164,   165,   366,   392,   400,   216,   347,   125,   129,
     137,   121,   133,   226,   230,   165,   386,   392,   399,   121,
     127,   128,   129,   163,   164,   165,   376,   392,   402,   216,
     178,   187,   185,   296,   367,   368,   125,   225,   178,     1,
      98,    99,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   157,   231,   387,   388,
     377,   378,   379,   206,     1,   101,   103,   131,   189,   190,
     191,   129,   137,   356,   216,   125,   125,   129,   132,   166,
     177,   178,   181,   182,   184,   297,   300,   301,   303,   305,
     306,   232,   233,   234,   117,   118,   119,   120,   122,   125,
     126,   217,   259,   260,   298,   302,   303,   306,   259,   258,
     298,   296,   254,   236,   238,   241,   243,   246,   248,   251,
     128,   389,   392,   400,   216,   356,   165,   380,   392,   399,
     216,   286,   115,   118,   120,   122,   130,   131,   132,   134,
     135,   164,   291,   292,   299,   303,   307,   192,   296,   188,
     129,   296,   134,   125,   287,   287,   287,   287,   125,   288,
     289,   291,   121,   121,   260,   121,   121,   298,   253,   289,
     121,   121,   122,   122,   118,   118,   121,   390,   391,   381,
     382,   207,   132,   136,   121,   194,   132,   124,   191,   177,
     178,   125,   166,   222,   291,   126,   222,   291,   290,   235,
     117,   119,   127,   166,   227,   261,   264,   266,   268,   291,
     261,   117,   208,   130,   255,   222,   237,   239,   242,   244,
     247,   249,   252,   356,   216,   128,   383,   392,   400,   216,
       1,   100,   101,   103,   106,   209,   291,   293,   195,   132,
     296,   288,   261,   269,   227,   265,   268,   267,   227,   228,
     268,   129,   132,   256,   257,   297,   301,   303,   305,   306,
     125,   216,   120,   216,   120,   216,   120,   261,   384,   385,
     125,   291,   210,   258,   292,   294,   267,   193,   126,   270,
     291,   127,   271,   121,   125,   227,   262,   263,   268,   287,
     129,   121,   240,   245,   250,   356,   216,   132,   121,   132,
     212,   121,   129,   137,   266,   127,   196,   128,   129,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   272,   273,   274,   287,   264,   257,   296,   216,   216,
     216,   166,   211,   291,   213,   215,   208,   295,   268,   197,
     291,   140,   140,   164,   123,   164,   123,   123,   123,   123,
     123,   123,   128,   274,   275,   287,   267,   216,   228,   292,
     145,   146,   153,   159,   198,   199,   276,   278,   279,   280,
     281,   160,   161,   162,   284,   285,   277,   167,   282,   266,
     164,   123,   123,   123,   128,   199,   164,   283,   283,   283,
     283,     5,   296,   296,   296,   124,   285,   283,   268,   200,
     202,   201,   124,   164,   124,   124,   124,   124,   217,   217,
     217,   124,   132,   283,     5,   283,   214,   124,   124,   124,
     216
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
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
				       ": skipping " << QUOTE("->") << " in variable declaration.");
			;}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 528 "surface.yy"
    {
			  CV->addSortMapping((yyvsp[(2) - (4)].yyToken), (yyvsp[(4) - (4)].yyToken));
			;}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 531 "surface.yy"
    {;}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 532 "surface.yy"
    { lexBubble(BAR_COLON | BAR_TO, 1); ;}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 537 "surface.yy"
    { (yyval.yyToken) = (yyvsp[(1) - (2)].yyToken); ;}
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 539 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			;}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 548 "surface.yy"
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			;}
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 556 "surface.yy"
    {
			  lexBubble(END_STATEMENT, 1);
			;}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 560 "surface.yy"
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			;}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 565 "surface.yy"
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

  case 88:

/* Line 1455 of yacc.c  */
#line 575 "surface.yy"
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

  case 89:

/* Line 1455 of yacc.c  */
#line 597 "surface.yy"
    {;}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 599 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			;}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 607 "surface.yy"
    { lexBubble(BAR_RIGHT_PAREN, 1); ;}
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 608 "surface.yy"
    {;}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 614 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 616 "surface.yy"
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[(1) - (3)].yyToken), (yyvsp[(3) - (3)].yyToken)));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[(1) - (3)].yyToken), (yyvsp[(3) - (3)].yyToken));
			;}
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 622 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[(8) - (8)].yyToken));
			;}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 629 "surface.yy"
    {;}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 631 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  store(t);
			;}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 639 "surface.yy"
    {;}
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 648 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter((yyvsp[(1) - (3)].yyToken), me);
			;}
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 655 "surface.yy"
    {;}
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 657 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			;}
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 665 "surface.yy"
    {
			  singleton[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[(1) - (1)].yyToken);  // needed for line number
			;}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 674 "surface.yy"
    {;}
    break;

  case 112:

/* Line 1455 of yacc.c  */
#line 685 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport((yyvsp[(1) - (2)].yyToken), me);
			;}
    break;

  case 113:

/* Line 1455 of yacc.c  */
#line 691 "surface.yy"
    { clear(); ;}
    break;

  case 114:

/* Line 1455 of yacc.c  */
#line 692 "surface.yy"
    { CM->addSortDecl(tokenSequence); ;}
    break;

  case 115:

/* Line 1455 of yacc.c  */
#line 694 "surface.yy"
    { clear(); ;}
    break;

  case 116:

/* Line 1455 of yacc.c  */
#line 695 "surface.yy"
    { CM->addSubsortDecl(tokenSequence); ;}
    break;

  case 117:

/* Line 1455 of yacc.c  */
#line 697 "surface.yy"
    { lexBubble(BAR_COLON, 1); ;}
    break;

  case 118:

/* Line 1455 of yacc.c  */
#line 698 "surface.yy"
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); ;}
    break;

  case 119:

/* Line 1455 of yacc.c  */
#line 699 "surface.yy"
    {;}
    break;

  case 120:

/* Line 1455 of yacc.c  */
#line 701 "surface.yy"
    {;}
    break;

  case 121:

/* Line 1455 of yacc.c  */
#line 703 "surface.yy"
    {;}
    break;

  case 122:

/* Line 1455 of yacc.c  */
#line 705 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON, 1); ;}
    break;

  case 123:

/* Line 1455 of yacc.c  */
#line 706 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 124:

/* Line 1455 of yacc.c  */
#line 707 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 125:

/* Line 1455 of yacc.c  */
#line 709 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON, 1);  ;}
    break;

  case 126:

/* Line 1455 of yacc.c  */
#line 710 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); ;}
    break;

  case 127:

/* Line 1455 of yacc.c  */
#line 711 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 128:

/* Line 1455 of yacc.c  */
#line 712 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 129:

/* Line 1455 of yacc.c  */
#line 714 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_EQUALS, 1); ;}
    break;

  case 130:

/* Line 1455 of yacc.c  */
#line 715 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 131:

/* Line 1455 of yacc.c  */
#line 716 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 132:

/* Line 1455 of yacc.c  */
#line 718 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_EQUALS, 1); ;}
    break;

  case 133:

/* Line 1455 of yacc.c  */
#line 719 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); ;}
    break;

  case 134:

/* Line 1455 of yacc.c  */
#line 720 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 135:

/* Line 1455 of yacc.c  */
#line 721 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 136:

/* Line 1455 of yacc.c  */
#line 723 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_ARROW2, 1); ;}
    break;

  case 137:

/* Line 1455 of yacc.c  */
#line 724 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 138:

/* Line 1455 of yacc.c  */
#line 725 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 139:

/* Line 1455 of yacc.c  */
#line 727 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_ARROW2, 1); ;}
    break;

  case 140:

/* Line 1455 of yacc.c  */
#line 728 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); ;}
    break;

  case 141:

/* Line 1455 of yacc.c  */
#line 729 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); ;}
    break;

  case 142:

/* Line 1455 of yacc.c  */
#line 730 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 143:

/* Line 1455 of yacc.c  */
#line 732 "surface.yy"
    { lexBubble(BAR_COLON, 1); ;}
    break;

  case 144:

/* Line 1455 of yacc.c  */
#line 733 "surface.yy"
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); ;}
    break;

  case 145:

/* Line 1455 of yacc.c  */
#line 734 "surface.yy"
    { CM->setFlag(SymbolType::MESSAGE); ;}
    break;

  case 146:

/* Line 1455 of yacc.c  */
#line 737 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 147:

/* Line 1455 of yacc.c  */
#line 742 "surface.yy"
    {
			;}
    break;

  case 148:

/* Line 1455 of yacc.c  */
#line 745 "surface.yy"
    {
			;}
    break;

  case 149:

/* Line 1455 of yacc.c  */
#line 748 "surface.yy"
    { clear(); ;}
    break;

  case 150:

/* Line 1455 of yacc.c  */
#line 749 "surface.yy"
    { CM->addSubsortDecl(tokenSequence); ;}
    break;

  case 151:

/* Line 1455 of yacc.c  */
#line 752 "surface.yy"
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			;}
    break;

  case 152:

/* Line 1455 of yacc.c  */
#line 762 "surface.yy"
    {;}
    break;

  case 153:

/* Line 1455 of yacc.c  */
#line 763 "surface.yy"
    {;}
    break;

  case 156:

/* Line 1455 of yacc.c  */
#line 771 "surface.yy"
    {
			;}
    break;

  case 157:

/* Line 1455 of yacc.c  */
#line 775 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 158:

/* Line 1455 of yacc.c  */
#line 776 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 161:

/* Line 1455 of yacc.c  */
#line 783 "surface.yy"
    { singleton[0] = (yyvsp[(1) - (1)].yyToken); CM->addOpDecl(singleton); ;}
    break;

  case 162:

/* Line 1455 of yacc.c  */
#line 784 "surface.yy"
    { CM->addOpDecl(lexerBubble); ;}
    break;

  case 165:

/* Line 1455 of yacc.c  */
#line 790 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			;}
    break;

  case 166:

/* Line 1455 of yacc.c  */
#line 797 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
				       ": skipping stray " << QUOTE(":") << " in operator declaration.");

			;}
    break;

  case 169:

/* Line 1455 of yacc.c  */
#line 807 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			;}
    break;

  case 170:

/* Line 1455 of yacc.c  */
#line 812 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			;}
    break;

  case 171:

/* Line 1455 of yacc.c  */
#line 819 "surface.yy"
    {
			  if ((yyvsp[(1) - (2)].yyBool))
			    CM->convertSortsToKinds();
			;}
    break;

  case 173:

/* Line 1455 of yacc.c  */
#line 826 "surface.yy"
    {;}
    break;

  case 174:

/* Line 1455 of yacc.c  */
#line 829 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 175:

/* Line 1455 of yacc.c  */
#line 830 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 178:

/* Line 1455 of yacc.c  */
#line 838 "surface.yy"
    {
			  singleton[0] = (yyvsp[(1) - (1)].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			;}
    break;

  case 179:

/* Line 1455 of yacc.c  */
#line 842 "surface.yy"
    { clear(); ;}
    break;

  case 180:

/* Line 1455 of yacc.c  */
#line 844 "surface.yy"
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			;}
    break;

  case 181:

/* Line 1455 of yacc.c  */
#line 849 "surface.yy"
    { store((yyvsp[(3) - (3)].yyToken)); ;}
    break;

  case 182:

/* Line 1455 of yacc.c  */
#line 850 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 183:

/* Line 1455 of yacc.c  */
#line 853 "surface.yy"
    {;}
    break;

  case 187:

/* Line 1455 of yacc.c  */
#line 862 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			;}
    break;

  case 188:

/* Line 1455 of yacc.c  */
#line 866 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			;}
    break;

  case 189:

/* Line 1455 of yacc.c  */
#line 870 "surface.yy"
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			;}
    break;

  case 190:

/* Line 1455 of yacc.c  */
#line 876 "surface.yy"
    {
			  CM->setFlag(SymbolType::ASSOC);
			;}
    break;

  case 191:

/* Line 1455 of yacc.c  */
#line 880 "surface.yy"
    {
			  CM->setFlag(SymbolType::COMM);
			;}
    break;

  case 192:

/* Line 1455 of yacc.c  */
#line 883 "surface.yy"
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); ;}
    break;

  case 193:

/* Line 1455 of yacc.c  */
#line 884 "surface.yy"
    { CM->setIdentity(lexerBubble); ;}
    break;

  case 194:

/* Line 1455 of yacc.c  */
#line 886 "surface.yy"
    {
			  CM->setFlag(SymbolType::IDEM);
			;}
    break;

  case 195:

/* Line 1455 of yacc.c  */
#line 890 "surface.yy"
    {
			  CM->setFlag(SymbolType::ITER);
			;}
    break;

  case 196:

/* Line 1455 of yacc.c  */
#line 893 "surface.yy"
    { CM->setPrec((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 197:

/* Line 1455 of yacc.c  */
#line 894 "surface.yy"
    { clear(); ;}
    break;

  case 198:

/* Line 1455 of yacc.c  */
#line 895 "surface.yy"
    { CM->setGather(tokenSequence); ;}
    break;

  case 199:

/* Line 1455 of yacc.c  */
#line 896 "surface.yy"
    { clear(); ;}
    break;

  case 200:

/* Line 1455 of yacc.c  */
#line 897 "surface.yy"
    { CM->setFormat(tokenSequence); ;}
    break;

  case 201:

/* Line 1455 of yacc.c  */
#line 898 "surface.yy"
    { clear(); ;}
    break;

  case 202:

/* Line 1455 of yacc.c  */
#line 899 "surface.yy"
    { CM->setStrat(tokenSequence); ;}
    break;

  case 203:

/* Line 1455 of yacc.c  */
#line 900 "surface.yy"
    { clear(); ;}
    break;

  case 204:

/* Line 1455 of yacc.c  */
#line 901 "surface.yy"
    { CM->setPoly(tokenSequence); ;}
    break;

  case 205:

/* Line 1455 of yacc.c  */
#line 903 "surface.yy"
    {
			  CM->setFlag(SymbolType::MEMO);
			;}
    break;

  case 206:

/* Line 1455 of yacc.c  */
#line 907 "surface.yy"
    {
			  CM->setFlag(SymbolType::CTOR);
			;}
    break;

  case 207:

/* Line 1455 of yacc.c  */
#line 911 "surface.yy"
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			;}
    break;

  case 208:

/* Line 1455 of yacc.c  */
#line 915 "surface.yy"
    { clear(); ;}
    break;

  case 209:

/* Line 1455 of yacc.c  */
#line 916 "surface.yy"
    { CM->setFrozen(tokenSequence); ;}
    break;

  case 210:

/* Line 1455 of yacc.c  */
#line 918 "surface.yy"
    {
			  CM->setFlag(SymbolType::CONFIG);
			;}
    break;

  case 211:

/* Line 1455 of yacc.c  */
#line 922 "surface.yy"
    {
			  CM->setFlag(SymbolType::OBJECT);
			;}
    break;

  case 212:

/* Line 1455 of yacc.c  */
#line 926 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 213:

/* Line 1455 of yacc.c  */
#line 930 "surface.yy"
    {
			  CM->setMetadata((yyvsp[(2) - (2)].yyToken));
			;}
    break;

  case 214:

/* Line 1455 of yacc.c  */
#line 933 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 215:

/* Line 1455 of yacc.c  */
#line 934 "surface.yy"
    { CM->setLatexMacro((yyvsp[(4) - (5)].yyString)); ;}
    break;

  case 216:

/* Line 1455 of yacc.c  */
#line 935 "surface.yy"
    {;}
    break;

  case 217:

/* Line 1455 of yacc.c  */
#line 937 "surface.yy"
    {
			  CM->setFlag(SymbolType::DITTO);
			;}
    break;

  case 220:

/* Line 1455 of yacc.c  */
#line 950 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 221:

/* Line 1455 of yacc.c  */
#line 951 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 224:

/* Line 1455 of yacc.c  */
#line 958 "surface.yy"
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[(2) - (2)].yyToken), tokenSequence); ;}
    break;

  case 225:

/* Line 1455 of yacc.c  */
#line 959 "surface.yy"
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); ;}
    break;

  case 226:

/* Line 1455 of yacc.c  */
#line 960 "surface.yy"
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); ;}
    break;

  case 227:

/* Line 1455 of yacc.c  */
#line 961 "surface.yy"
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); ;}
    break;

  case 228:

/* Line 1455 of yacc.c  */
#line 967 "surface.yy"
    {;}
    break;

  case 229:

/* Line 1455 of yacc.c  */
#line 969 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			;}
    break;

  case 230:

/* Line 1455 of yacc.c  */
#line 975 "surface.yy"
    {;}
    break;

  case 231:

/* Line 1455 of yacc.c  */
#line 977 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			;}
    break;

  case 232:

/* Line 1455 of yacc.c  */
#line 985 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 234:

/* Line 1455 of yacc.c  */
#line 989 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 235:

/* Line 1455 of yacc.c  */
#line 990 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 236:

/* Line 1455 of yacc.c  */
#line 991 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 237:

/* Line 1455 of yacc.c  */
#line 992 "surface.yy"
    { store((yyvsp[(4) - (4)].yyToken)); ;}
    break;

  case 238:

/* Line 1455 of yacc.c  */
#line 999 "surface.yy"
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

  case 239:

/* Line 1455 of yacc.c  */
#line 1010 "surface.yy"
    { fragStore((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 240:

/* Line 1455 of yacc.c  */
#line 1011 "surface.yy"
    { fragStore((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 241:

/* Line 1455 of yacc.c  */
#line 1012 "surface.yy"
    { fragStore((yyvsp[(5) - (5)].yyToken)); ;}
    break;

  case 242:

/* Line 1455 of yacc.c  */
#line 1015 "surface.yy"
    { fragStore((yyvsp[(2) - (2)].yyToken)); ;}
    break;

  case 243:

/* Line 1455 of yacc.c  */
#line 1016 "surface.yy"
    {;}
    break;

  case 244:

/* Line 1455 of yacc.c  */
#line 1017 "surface.yy"
    {;}
    break;

  case 347:

/* Line 1455 of yacc.c  */
#line 1097 "surface.yy"
    { lexBubble(END_COMMAND, 1) ;}
    break;

  case 348:

/* Line 1455 of yacc.c  */
#line 1099 "surface.yy"
    {
			  interpreter.setCurrentModule(lexerBubble);
			;}
    break;

  case 349:

/* Line 1455 of yacc.c  */
#line 1102 "surface.yy"
    { lexBubble(END_COMMAND, 1) ;}
    break;

  case 350:

/* Line 1455 of yacc.c  */
#line 1104 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			;}
    break;

  case 351:

/* Line 1455 of yacc.c  */
#line 1109 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 352:

/* Line 1455 of yacc.c  */
#line 1114 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			;}
    break;

  case 353:

/* Line 1455 of yacc.c  */
#line 1121 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 354:

/* Line 1455 of yacc.c  */
#line 1126 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			;}
    break;

  case 355:

/* Line 1455 of yacc.c  */
#line 1133 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 356:

/* Line 1455 of yacc.c  */
#line 1138 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			;}
    break;

  case 357:

/* Line 1455 of yacc.c  */
#line 1145 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 358:

/* Line 1455 of yacc.c  */
#line 1150 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 359:

/* Line 1455 of yacc.c  */
#line 1157 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 360:

/* Line 1455 of yacc.c  */
#line 1163 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 361:

/* Line 1455 of yacc.c  */
#line 1169 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 362:

/* Line 1455 of yacc.c  */
#line 1176 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 363:

/* Line 1455 of yacc.c  */
#line 1182 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 364:

/* Line 1455 of yacc.c  */
#line 1189 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 365:

/* Line 1455 of yacc.c  */
#line 1195 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 366:

/* Line 1455 of yacc.c  */
#line 1201 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 367:

/* Line 1455 of yacc.c  */
#line 1207 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 368:

/* Line 1455 of yacc.c  */
#line 1214 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[(1) - (3)].yySearchKind));
			;}
    break;

  case 369:

/* Line 1455 of yacc.c  */
#line 1220 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 370:

/* Line 1455 of yacc.c  */
#line 1226 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[(1) - (3)].yyBool), number);
			;}
    break;

  case 371:

/* Line 1455 of yacc.c  */
#line 1232 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 372:

/* Line 1455 of yacc.c  */
#line 1238 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number);
			;}
    break;

  case 373:

/* Line 1455 of yacc.c  */
#line 1244 "surface.yy"
    {
			  interpreter.cont((yyvsp[(3) - (4)].yyInt64), (yyvsp[(1) - (4)].yyBool));
			;}
    break;

  case 374:

/* Line 1455 of yacc.c  */
#line 1248 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 375:

/* Line 1455 of yacc.c  */
#line 1253 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			;}
    break;

  case 376:

/* Line 1455 of yacc.c  */
#line 1259 "surface.yy"
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			;}
    break;

  case 377:

/* Line 1455 of yacc.c  */
#line 1264 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 378:

/* Line 1455 of yacc.c  */
#line 1266 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[(2) - (4)].yyBool));
			;}
    break;

  case 379:

/* Line 1455 of yacc.c  */
#line 1270 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 380:

/* Line 1455 of yacc.c  */
#line 1272 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[(2) - (4)].yyBool));
			;}
    break;

  case 381:

/* Line 1455 of yacc.c  */
#line 1276 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 382:

/* Line 1455 of yacc.c  */
#line 1278 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[(2) - (4)].yyBool));
			;}
    break;

  case 383:

/* Line 1455 of yacc.c  */
#line 1282 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 384:

/* Line 1455 of yacc.c  */
#line 1284 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[(2) - (4)].yyBool));
			;}
    break;

  case 385:

/* Line 1455 of yacc.c  */
#line 1289 "surface.yy"
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			;}
    break;

  case 386:

/* Line 1455 of yacc.c  */
#line 1296 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 387:

/* Line 1455 of yacc.c  */
#line 1298 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			;}
    break;

  case 388:

/* Line 1455 of yacc.c  */
#line 1302 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 389:

/* Line 1455 of yacc.c  */
#line 1304 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			;}
    break;

  case 390:

/* Line 1455 of yacc.c  */
#line 1308 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 391:

/* Line 1455 of yacc.c  */
#line 1310 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			;}
    break;

  case 392:

/* Line 1455 of yacc.c  */
#line 1314 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 393:

/* Line 1455 of yacc.c  */
#line 1316 "surface.yy"
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			;}
    break;

  case 394:

/* Line 1455 of yacc.c  */
#line 1321 "surface.yy"
    {
			  interpreter.showModules(true);
			;}
    break;

  case 395:

/* Line 1455 of yacc.c  */
#line 1325 "surface.yy"
    {
			  interpreter.showNamedViews();
			;}
    break;

  case 396:

/* Line 1455 of yacc.c  */
#line 1328 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 397:

/* Line 1455 of yacc.c  */
#line 1330 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			;}
    break;

  case 398:

/* Line 1455 of yacc.c  */
#line 1334 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 399:

/* Line 1455 of yacc.c  */
#line 1336 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			;}
    break;

  case 400:

/* Line 1455 of yacc.c  */
#line 1340 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 401:

/* Line 1455 of yacc.c  */
#line 1342 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			;}
    break;

  case 402:

/* Line 1455 of yacc.c  */
#line 1346 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 403:

/* Line 1455 of yacc.c  */
#line 1348 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			;}
    break;

  case 404:

/* Line 1455 of yacc.c  */
#line 1352 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 405:

/* Line 1455 of yacc.c  */
#line 1354 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			;}
    break;

  case 406:

/* Line 1455 of yacc.c  */
#line 1358 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 407:

/* Line 1455 of yacc.c  */
#line 1360 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			;}
    break;

  case 408:

/* Line 1455 of yacc.c  */
#line 1364 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 409:

/* Line 1455 of yacc.c  */
#line 1366 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			;}
    break;

  case 410:

/* Line 1455 of yacc.c  */
#line 1370 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 411:

/* Line 1455 of yacc.c  */
#line 1372 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			;}
    break;

  case 412:

/* Line 1455 of yacc.c  */
#line 1377 "surface.yy"
    {
			  interpreter.showSearchPath((yyvsp[(3) - (4)].yyInt64));
			;}
    break;

  case 413:

/* Line 1455 of yacc.c  */
#line 1381 "surface.yy"
    {
			  interpreter.showSearchPathLabels((yyvsp[(4) - (5)].yyInt64));
			;}
    break;

  case 414:

/* Line 1455 of yacc.c  */
#line 1385 "surface.yy"
    {
			  interpreter.showSearchGraph();
			;}
    break;

  case 415:

/* Line 1455 of yacc.c  */
#line 1388 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 416:

/* Line 1455 of yacc.c  */
#line 1390 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			;}
    break;

  case 417:

/* Line 1455 of yacc.c  */
#line 1398 "surface.yy"
    {
			  globalAdvisoryFlag = (yyvsp[(4) - (5)].yyBool);
			;}
    break;

  case 418:

/* Line 1455 of yacc.c  */
#line 1402 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 419:

/* Line 1455 of yacc.c  */
#line 1406 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[(5) - (6)].yyBool));
			;}
    break;

  case 420:

/* Line 1455 of yacc.c  */
#line 1410 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 421:

/* Line 1455 of yacc.c  */
#line 1414 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 422:

/* Line 1455 of yacc.c  */
#line 1418 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[(5) - (6)].yyBool));
			;}
    break;

  case 423:

/* Line 1455 of yacc.c  */
#line 1422 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 424:

/* Line 1455 of yacc.c  */
#line 1426 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 425:

/* Line 1455 of yacc.c  */
#line 1430 "surface.yy"
    {
			  interpreter.setPrintFlag((yyvsp[(3) - (5)].yyPrintFlags), (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 426:

/* Line 1455 of yacc.c  */
#line 1434 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 427:

/* Line 1455 of yacc.c  */
#line 1438 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[(5) - (6)].yyBool));
			;}
    break;

  case 428:

/* Line 1455 of yacc.c  */
#line 1442 "surface.yy"
    {
			  interpreter.setFlag((yyvsp[(3) - (5)].yyFlags), (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 429:

/* Line 1455 of yacc.c  */
#line 1446 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[(3) - (4)].yyBool));
			;}
    break;

  case 430:

/* Line 1455 of yacc.c  */
#line 1449 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 431:

/* Line 1455 of yacc.c  */
#line 1450 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 432:

/* Line 1455 of yacc.c  */
#line 1452 "surface.yy"
    {
			  interpreter.setAutoImport((yyvsp[(2) - (7)].yyImportMode), (yyvsp[(4) - (7)].yyToken), (yyvsp[(6) - (7)].yyBool));
			;}
    break;

  case 433:

/* Line 1455 of yacc.c  */
#line 1455 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 434:

/* Line 1455 of yacc.c  */
#line 1456 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 435:

/* Line 1455 of yacc.c  */
#line 1458 "surface.yy"
    {
			  interpreter.setOmodInclude((yyvsp[(5) - (8)].yyToken), (yyvsp[(7) - (8)].yyBool));
			;}
    break;

  case 436:

/* Line 1455 of yacc.c  */
#line 1462 "surface.yy"
    {
			  globalVerboseFlag = (yyvsp[(3) - (4)].yyBool);
			;}
    break;

  case 437:

/* Line 1455 of yacc.c  */
#line 1466 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 438:

/* Line 1455 of yacc.c  */
#line 1470 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 439:

/* Line 1455 of yacc.c  */
#line 1474 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 440:

/* Line 1455 of yacc.c  */
#line 1478 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[(3) - (4)].yyBool));
			;}
    break;

  case 441:

/* Line 1455 of yacc.c  */
#line 1482 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 442:

/* Line 1455 of yacc.c  */
#line 1489 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			;}
    break;

  case 443:

/* Line 1455 of yacc.c  */
#line 1493 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			;}
    break;

  case 444:

/* Line 1455 of yacc.c  */
#line 1497 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			;}
    break;

  case 445:

/* Line 1455 of yacc.c  */
#line 1501 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			;}
    break;

  case 446:

/* Line 1455 of yacc.c  */
#line 1508 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[(4) - (5)].yyBool));
			;}
    break;

  case 447:

/* Line 1455 of yacc.c  */
#line 1512 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[(3) - (4)].yyBool));
			;}
    break;

  case 448:

/* Line 1455 of yacc.c  */
#line 1518 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 450:

/* Line 1455 of yacc.c  */
#line 1525 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; ;}
    break;

  case 451:

/* Line 1455 of yacc.c  */
#line 1526 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; ;}
    break;

  case 452:

/* Line 1455 of yacc.c  */
#line 1527 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; ;}
    break;

  case 453:

/* Line 1455 of yacc.c  */
#line 1528 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; ;}
    break;

  case 454:

/* Line 1455 of yacc.c  */
#line 1529 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; ;}
    break;

  case 455:

/* Line 1455 of yacc.c  */
#line 1530 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; ;}
    break;

  case 456:

/* Line 1455 of yacc.c  */
#line 1531 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; ;}
    break;

  case 457:

/* Line 1455 of yacc.c  */
#line 1532 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; ;}
    break;

  case 458:

/* Line 1455 of yacc.c  */
#line 1533 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; ;}
    break;

  case 459:

/* Line 1455 of yacc.c  */
#line 1534 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; ;}
    break;

  case 460:

/* Line 1455 of yacc.c  */
#line 1537 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE; ;}
    break;

  case 461:

/* Line 1455 of yacc.c  */
#line 1538 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; ;}
    break;

  case 462:

/* Line 1455 of yacc.c  */
#line 1539 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; ;}
    break;

  case 463:

/* Line 1455 of yacc.c  */
#line 1540 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; ;}
    break;

  case 464:

/* Line 1455 of yacc.c  */
#line 1541 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; ;}
    break;

  case 465:

/* Line 1455 of yacc.c  */
#line 1542 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_MB; ;}
    break;

  case 466:

/* Line 1455 of yacc.c  */
#line 1543 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; ;}
    break;

  case 467:

/* Line 1455 of yacc.c  */
#line 1544 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_RL; ;}
    break;

  case 468:

/* Line 1455 of yacc.c  */
#line 1545 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; ;}
    break;

  case 469:

/* Line 1455 of yacc.c  */
#line 1546 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; ;}
    break;

  case 470:

/* Line 1455 of yacc.c  */
#line 1547 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; ;}
    break;

  case 471:

/* Line 1455 of yacc.c  */
#line 1550 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 472:

/* Line 1455 of yacc.c  */
#line 1551 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 473:

/* Line 1455 of yacc.c  */
#line 1554 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 474:

/* Line 1455 of yacc.c  */
#line 1555 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 475:

/* Line 1455 of yacc.c  */
#line 1558 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 476:

/* Line 1455 of yacc.c  */
#line 1559 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 477:

/* Line 1455 of yacc.c  */
#line 1562 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 478:

/* Line 1455 of yacc.c  */
#line 1563 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 479:

/* Line 1455 of yacc.c  */
#line 1568 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::NARROW; ;}
    break;

  case 480:

/* Line 1455 of yacc.c  */
#line 1569 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; ;}
    break;

  case 481:

/* Line 1455 of yacc.c  */
#line 1570 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::SEARCH; ;}
    break;

  case 482:

/* Line 1455 of yacc.c  */
#line 1573 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 483:

/* Line 1455 of yacc.c  */
#line 1574 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 484:

/* Line 1455 of yacc.c  */
#line 1577 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 485:

/* Line 1455 of yacc.c  */
#line 1578 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 486:

/* Line 1455 of yacc.c  */
#line 1581 "surface.yy"
    { (yyval.yyInt64) = (yyvsp[(1) - (1)].yyInt64); ;}
    break;

  case 487:

/* Line 1455 of yacc.c  */
#line 1582 "surface.yy"
    { (yyval.yyInt64) = NONE; ;}
    break;

  case 488:

/* Line 1455 of yacc.c  */
#line 1585 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::PROTECTING; ;}
    break;

  case 489:

/* Line 1455 of yacc.c  */
#line 1586 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::EXTENDING; ;}
    break;

  case 490:

/* Line 1455 of yacc.c  */
#line 1587 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::INCLUDING; ;}
    break;

  case 492:

/* Line 1455 of yacc.c  */
#line 1597 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 494:

/* Line 1455 of yacc.c  */
#line 1601 "surface.yy"
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); ;}
    break;

  case 497:

/* Line 1455 of yacc.c  */
#line 1613 "surface.yy"
    { lexSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 500:

/* Line 1455 of yacc.c  */
#line 1616 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0);  ;}
    break;

  case 502:

/* Line 1455 of yacc.c  */
#line 1621 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 504:

/* Line 1455 of yacc.c  */
#line 1623 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 507:

/* Line 1455 of yacc.c  */
#line 1629 "surface.yy"
    { number = Token::codeToInt64(lexerBubble[1].code()); ;}
    break;

  case 509:

/* Line 1455 of yacc.c  */
#line 1631 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 512:

/* Line 1455 of yacc.c  */
#line 1646 "surface.yy"
    { lexSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 515:

/* Line 1455 of yacc.c  */
#line 1649 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 517:

/* Line 1455 of yacc.c  */
#line 1658 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 519:

/* Line 1455 of yacc.c  */
#line 1660 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 521:

/* Line 1455 of yacc.c  */
#line 1662 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 524:

/* Line 1455 of yacc.c  */
#line 1672 "surface.yy"
    { number = Token::codeToInt64(lexerBubble[1].code()); ;}
    break;

  case 526:

/* Line 1455 of yacc.c  */
#line 1674 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 528:

/* Line 1455 of yacc.c  */
#line 1676 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 531:

/* Line 1455 of yacc.c  */
#line 1686 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 533:

/* Line 1455 of yacc.c  */
#line 1688 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 536:

/* Line 1455 of yacc.c  */
#line 1699 "surface.yy"
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			;}
    break;

  case 538:

/* Line 1455 of yacc.c  */
#line 1705 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 541:

/* Line 1455 of yacc.c  */
#line 1715 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); ;}
    break;

  case 543:

/* Line 1455 of yacc.c  */
#line 1717 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 546:

/* Line 1455 of yacc.c  */
#line 1727 "surface.yy"
    { number2 = Token::codeToInt64(lexerBubble[2].code()); ;}
    break;

  case 548:

/* Line 1455 of yacc.c  */
#line 1729 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); ;}
    break;

  case 551:

/* Line 1455 of yacc.c  */
#line 1734 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0, 1); ;}
    break;

  case 554:

/* Line 1455 of yacc.c  */
#line 1740 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 1, 1); ;}
    break;

  case 556:

/* Line 1455 of yacc.c  */
#line 1742 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON | END_COMMAND, 0); ;}
    break;

  case 558:

/* Line 1455 of yacc.c  */
#line 1745 "surface.yy"
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(lexerBubble[0]);
			;}
    break;

  case 596:

/* Line 1455 of yacc.c  */
#line 1784 "surface.yy"
    {;}
    break;

  case 598:

/* Line 1455 of yacc.c  */
#line 1789 "surface.yy"
    {
			  singleton[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			;}
    break;

  case 601:

/* Line 1455 of yacc.c  */
#line 1800 "surface.yy"
    {
			  singleton[0] = (yyvsp[(1) - (1)].yyToken);
			  interpreter.addSelected(singleton);
			;}
    break;

  case 602:

/* Line 1455 of yacc.c  */
#line 1805 "surface.yy"
    {
			  interpreter.addSelected(lexerBubble);
			;}
    break;



/* Line 1455 of yacc.c  */
#line 5899 "surface.c"
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
#line 1835 "surface.yy"


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

