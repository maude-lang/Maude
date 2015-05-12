/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

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

/* Line 268 of yacc.c  */
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


/* Line 268 of yacc.c  */
#line 146 "surface.c"

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
     KW_GET = 274,
     KW_VARIANTS = 275,
     KW_VARIANT = 276,
     KW_EREWRITE = 277,
     KW_FREWRITE = 278,
     KW_SREWRITE = 279,
     KW_CONTINUE = 280,
     KW_SEARCH = 281,
     KW_SET = 282,
     KW_SHOW = 283,
     KW_ON = 284,
     KW_OFF = 285,
     KW_TRACE = 286,
     KW_BODY = 287,
     KW_BUILTIN = 288,
     KW_WHOLE = 289,
     KW_SELECT = 290,
     KW_DESELECT = 291,
     KW_CONDITION = 292,
     KW_SUBSTITUTION = 293,
     KW_PRINT = 294,
     KW_GRAPH = 295,
     KW_MIXFIX = 296,
     KW_FLAT = 297,
     KW_ATTRIBUTE = 298,
     KW_NEWLINE = 299,
     KW_WITH = 300,
     KW_PARENS = 301,
     KW_ALIASES = 302,
     KW_GC = 303,
     KW_TIME = 304,
     KW_STATS = 305,
     KW_TIMING = 306,
     KW_CMD = 307,
     KW_BREAKDOWN = 308,
     KW_BREAK = 309,
     KW_PATH = 310,
     KW_MODULE = 311,
     KW_MODULES = 312,
     KW_VIEWS = 313,
     KW_ALL = 314,
     KW_SORTS = 315,
     KW_OPS2 = 316,
     KW_VARS = 317,
     KW_MBS = 318,
     KW_EQS = 319,
     KW_RLS = 320,
     KW_SUMMARY = 321,
     KW_KINDS = 322,
     KW_ADVISE = 323,
     KW_VERBOSE = 324,
     KW_DO = 325,
     KW_CLEAR = 326,
     KW_PROTECT = 327,
     KW_EXTEND = 328,
     KW_INCLUDE = 329,
     KW_EXCLUDE = 330,
     KW_CONCEAL = 331,
     KW_REVEAL = 332,
     KW_COMPILE = 333,
     KW_COUNT = 334,
     KW_DEBUG = 335,
     KW_IRREDUNDANT = 336,
     KW_RESUME = 337,
     KW_ABORT = 338,
     KW_STEP = 339,
     KW_WHERE = 340,
     KW_CREDUCE = 341,
     KW_SREDUCE = 342,
     KW_DUMP = 343,
     KW_PROFILE = 344,
     KW_NUMBER = 345,
     KW_RAT = 346,
     KW_COLOR = 347,
     SIMPLE_NUMBER = 348,
     KW_PWD = 349,
     KW_CD = 350,
     KW_PUSHD = 351,
     KW_POPD = 352,
     KW_LS = 353,
     KW_LOAD = 354,
     KW_QUIT = 355,
     KW_EOF = 356,
     KW_ENDM = 357,
     KW_IMPORT = 358,
     KW_ENDV = 359,
     KW_SORT = 360,
     KW_SUBSORT = 361,
     KW_OP = 362,
     KW_OPS = 363,
     KW_MSGS = 364,
     KW_VAR = 365,
     KW_CLASS = 366,
     KW_SUBCLASS = 367,
     KW_MB = 368,
     KW_CMB = 369,
     KW_EQ = 370,
     KW_CEQ = 371,
     KW_RL = 372,
     KW_CRL = 373,
     KW_IS = 374,
     KW_FROM = 375,
     KW_ARROW = 376,
     KW_ARROW2 = 377,
     KW_PARTIAL = 378,
     KW_IF = 379,
     KW_LABEL = 380,
     KW_TO = 381,
     KW_COLON2 = 382,
     KW_ASSOC = 383,
     KW_COMM = 384,
     KW_ID = 385,
     KW_IDEM = 386,
     KW_ITER = 387,
     KW_LEFT = 388,
     KW_RIGHT = 389,
     KW_PREC = 390,
     KW_GATHER = 391,
     KW_METADATA = 392,
     KW_STRAT = 393,
     KW_POLY = 394,
     KW_MEMO = 395,
     KW_FROZEN = 396,
     KW_CTOR = 397,
     KW_LATEX = 398,
     KW_SPECIAL = 399,
     KW_CONFIG = 400,
     KW_OBJ = 401,
     KW_MSG = 402,
     KW_DITTO = 403,
     KW_FORMAT = 404,
     KW_ID_HOOK = 405,
     KW_OP_HOOK = 406,
     KW_TERM_HOOK = 407,
     KW_IN = 408,
     IDENTIFIER = 409,
     NUMERIC_ID = 410,
     ENDS_IN_DOT = 411,
     FORCE_LOOKAHEAD = 412
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 103 "surface.yy"

  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;



/* Line 293 of yacc.c  */
#line 352 "surface.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 343 of yacc.c  */
#line 114 "surface.yy"

int yylex(YYSTYPE* lvalp);


/* Line 343 of yacc.c  */
#line 369 "surface.c"

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
# if defined YYENABLE_NLS && YYENABLE_NLS
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
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

# define YYCOPY_NEEDED 1

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

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
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
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  111
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2394

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  172
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  244
/* YYNRULES -- Number of rules.  */
#define YYNRULES  615
/* YYNRULES -- Number of states.  */
#define YYNSTATES  921

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   412

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     127,   128,   139,   138,   133,     2,   129,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   125,     2,
     130,   126,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   131,     2,   132,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   140,   134,   141,     2,     2,     2,     2,
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
     135,   136,   137,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171
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
     937,   941,   942,   946,   947,   953,   954,   961,   966,   967,
     971,   973,   974,   979,   980,   985,   986,   991,   992,   997,
    1002,  1003,  1008,  1009,  1014,  1015,  1020,  1021,  1026,  1030,
    1034,  1035,  1040,  1041,  1046,  1047,  1052,  1053,  1058,  1059,
    1064,  1065,  1070,  1071,  1076,  1077,  1082,  1087,  1093,  1098,
    1099,  1104,  1110,  1116,  1123,  1129,  1135,  1142,  1148,  1154,
    1160,  1166,  1173,  1179,  1184,  1185,  1186,  1194,  1195,  1196,
    1205,  1210,  1216,  1222,  1228,  1233,  1239,  1242,  1245,  1248,
    1251,  1257,  1262,  1263,  1267,  1269,  1271,  1274,  1277,  1279,
    1281,  1283,  1285,  1287,  1289,  1290,  1292,  1294,  1296,  1298,
    1300,  1302,  1304,  1306,  1308,  1310,  1312,  1314,  1316,  1318,
    1320,  1322,  1324,  1326,  1328,  1330,  1332,  1334,  1336,  1338,
    1339,  1341,  1342,  1344,  1345,  1347,  1349,  1351,  1353,  1354,
    1358,  1359,  1363,  1365,  1366,  1370,  1372,  1373,  1377,  1378,
    1382,  1383,  1387,  1389,  1390,  1394,  1395,  1399,  1401,  1402,
    1406,  1408,  1409,  1413,  1414,  1418,  1419,  1423,  1424,  1428,
    1430,  1431,  1435,  1436,  1440,  1441,  1445,  1447,  1448,  1452,
    1453,  1457,  1459,  1460,  1464,  1465,  1469,  1471,  1472,  1476,
    1477,  1481,  1483,  1484,  1488,  1489,  1493,  1495,  1496,  1500,
    1502,  1503,  1507,  1508,  1512,  1514,  1516,  1518,  1520,  1522,
    1524,  1526,  1528,  1530,  1532,  1534,  1536,  1538,  1540,  1542,
    1544,  1546,  1548,  1550,  1552,  1554,  1556,  1558,  1560,  1562,
    1564,  1566,  1568,  1570,  1572,  1574,  1576,  1578,  1580,  1582,
    1584,  1587,  1589,  1591,  1593,  1595,  1598,  1600,  1602,  1604,
    1606,  1608,  1610,  1612,  1614,  1616
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     173,     0,    -1,   174,    -1,    -1,   223,    -1,   207,    -1,
     175,    -1,   312,    -1,    -1,   167,   176,     3,    -1,    -1,
      99,   177,     3,    -1,    94,    -1,    -1,    95,   178,     3,
      -1,    -1,    96,   179,     3,    -1,    97,    -1,    -1,    98,
     180,     4,    -1,   100,    -1,   101,    -1,   301,   291,    -1,
     304,   129,    -1,   188,   291,    -1,   185,   291,    -1,   186,
     291,    -1,   182,   138,   181,    -1,   170,    -1,   183,    -1,
     182,   138,   182,    -1,   184,    -1,   185,    -1,   188,    -1,
     186,    -1,   300,    -1,   183,   139,   190,    -1,    -1,   184,
     140,   187,   189,   141,    -1,   127,   182,   128,    -1,   189,
     133,   300,    -1,   300,    -1,    -1,    -1,   127,   191,   193,
     192,   128,    -1,   194,    -1,     1,    -1,   194,   133,   195,
      -1,   195,    -1,   105,   295,   136,   295,    -1,   135,   300,
     136,   300,    -1,    -1,    -1,   107,   196,   198,   136,   197,
     200,    -1,    -1,   125,   199,   271,   270,   272,    -1,    -1,
      -1,   131,   201,   202,   132,    -1,    -1,   202,   203,    -1,
     203,    -1,   149,   168,    -1,    -1,   150,   127,   204,   287,
     128,    -1,    -1,   163,   127,   205,   287,   128,    -1,    -1,
     157,   127,   206,     5,   128,    -1,    -1,    -1,    -1,     8,
     208,   300,   120,   182,   209,   136,   182,   210,   290,   211,
     104,    -1,   211,   213,    -1,    -1,   121,    -1,    -1,   105,
     295,   136,   215,    -1,   110,   262,   125,   212,   232,    -1,
      -1,   107,   214,   216,    -1,     1,   129,    -1,   295,   291,
      -1,   170,    -1,    -1,    -1,   125,   217,   271,   270,   272,
     136,   218,   220,    -1,    -1,   136,   219,   220,    -1,   129,
      -1,   170,    -1,    -1,   127,   222,   128,    -1,    -1,    -1,
     233,   224,   300,   225,   227,   290,   234,   102,    -1,   129,
      -1,   170,    -1,   140,   228,   141,    -1,    -1,   228,   133,
     229,    -1,   229,    -1,   300,   230,   182,    -1,   137,    -1,
     125,    -1,   170,    -1,   272,   291,    -1,   231,    -1,     6,
      -1,     7,    -1,   234,   235,    -1,    -1,   103,   181,    -1,
      -1,   105,   236,   292,   226,    -1,    -1,   106,   237,   293,
     226,    -1,    -1,    -1,   107,   238,   125,   239,   265,    -1,
     108,   263,   125,   265,    -1,   110,   262,   125,   212,   232,
      -1,    -1,    -1,   113,   240,   125,   241,   220,    -1,    -1,
      -1,    -1,   114,   242,   125,   243,   124,   244,   220,    -1,
      -1,    -1,   115,   245,   126,   246,   220,    -1,    -1,    -1,
      -1,   116,   247,   126,   248,   124,   249,   220,    -1,    -1,
      -1,   117,   250,   122,   251,   220,    -1,    -1,    -1,    -1,
     118,   252,   122,   253,   124,   254,   220,    -1,    -1,    -1,
     161,   255,   125,   256,   265,    -1,   109,   263,   125,   265,
      -1,    -1,   111,   300,   257,   259,   129,    -1,    -1,   112,
     258,   293,   226,    -1,     1,   129,    -1,   134,    -1,   134,
     260,    -1,   261,    -1,   260,   133,   261,    -1,   301,   125,
     300,    -1,   262,   302,    -1,   302,    -1,   263,   264,    -1,
     264,    -1,   302,    -1,   221,    -1,   272,   271,   267,    -1,
     268,    -1,   231,    -1,   125,    -1,    -1,   266,   268,    -1,
     129,    -1,   231,    -1,   270,   269,    -1,   272,   275,   291,
      -1,   231,    -1,   121,    -1,   123,    -1,   271,   272,    -1,
      -1,   295,    -1,    -1,   131,   273,   274,   132,    -1,   274,
     133,   295,    -1,   295,    -1,   131,   276,   132,    -1,    -1,
     276,   278,    -1,   278,    -1,   144,    -1,   147,   144,    -1,
     148,   144,    -1,   142,    -1,   143,    -1,    -1,   277,   279,
     286,    -1,   145,    -1,   146,    -1,   149,   168,    -1,    -1,
     150,   127,   280,   287,   128,    -1,    -1,   163,   127,   281,
     287,   128,    -1,    -1,   152,   127,   282,   287,   128,    -1,
      -1,   153,   127,   283,   287,   128,    -1,   154,    -1,   156,
      -1,   155,    -1,    -1,   155,   127,   284,   287,   128,    -1,
     159,    -1,   160,    -1,   161,    -1,   151,   168,    -1,    -1,
     157,   127,   285,     5,   128,    -1,   158,   127,   288,   128,
      -1,   162,    -1,   171,    -1,    -1,   287,   168,    -1,   168,
      -1,   288,   289,    -1,   289,    -1,   164,   300,    -1,   164,
     300,   221,    -1,   165,   300,   221,    -1,   166,   300,   221,
      -1,   119,    -1,    -1,   129,    -1,    -1,   292,   295,    -1,
      -1,   293,   295,    -1,   293,   130,    -1,    -1,   295,   294,
     292,   130,    -1,   296,    -1,   303,    -1,    -1,   296,   140,
     297,   298,   141,    -1,    -1,   298,   133,   299,   296,    -1,
     296,    -1,   306,    -1,   307,    -1,   309,    -1,   310,    -1,
     129,    -1,   305,    -1,   133,    -1,   136,    -1,   307,    -1,
     309,    -1,   310,    -1,   306,    -1,   307,    -1,   310,    -1,
     129,    -1,   130,    -1,   121,    -1,   123,    -1,   126,    -1,
     122,    -1,   124,    -1,   168,    -1,   307,    -1,   311,    -1,
     126,    -1,   134,    -1,   138,    -1,   139,    -1,   122,    -1,
     124,    -1,   119,    -1,   135,    -1,   136,    -1,   129,    -1,
     170,    -1,   168,    -1,   140,    -1,   141,    -1,   138,    -1,
     139,    -1,   134,    -1,   137,    -1,   135,    -1,   120,    -1,
     119,    -1,   305,    -1,   170,    -1,   133,    -1,   136,    -1,
     161,    -1,   308,    -1,   103,    -1,   105,    -1,   106,    -1,
     107,    -1,   108,    -1,   110,    -1,   109,    -1,   111,    -1,
     112,    -1,   113,    -1,   114,    -1,   115,    -1,   116,    -1,
     117,    -1,   118,    -1,   102,    -1,   104,    -1,   130,    -1,
     125,    -1,   121,    -1,   123,    -1,   126,    -1,   122,    -1,
     124,    -1,   131,    -1,   132,    -1,   311,    -1,   142,    -1,
     143,    -1,   144,    -1,   145,    -1,   146,    -1,   147,    -1,
     148,    -1,   149,    -1,   150,    -1,   152,    -1,   153,    -1,
     154,    -1,   156,    -1,   157,    -1,   158,    -1,   155,    -1,
     151,    -1,   159,    -1,   160,    -1,   162,    -1,   163,    -1,
     164,    -1,   165,    -1,   166,    -1,    -1,    35,   313,   220,
      -1,    -1,    88,   314,   220,    -1,    -1,     9,   315,   363,
      -1,    -1,    86,   316,   363,    -1,    -1,    87,   317,   363,
      -1,    -1,   359,    11,   318,   363,    -1,    -1,   359,    12,
     319,   367,    -1,    -1,   359,    22,   320,   376,    -1,    -1,
     359,    23,   321,   376,    -1,    -1,   359,    24,   322,   367,
      -1,    -1,   357,   323,   376,    -1,    -1,   358,   324,   367,
      -1,    -1,    18,   325,   367,    -1,    -1,   359,    21,    18,
     326,   367,    -1,    -1,   359,    19,   360,    20,   327,   367,
      -1,   359,    25,   361,   129,    -1,    -1,    13,   328,   363,
      -1,   221,    -1,    -1,    31,   354,   329,   410,    -1,    -1,
      31,   355,   330,   410,    -1,    -1,    54,   354,   331,   410,
      -1,    -1,    39,   356,   332,   410,    -1,    70,    71,   154,
     129,    -1,    -1,    28,     6,   333,   220,    -1,    -1,    28,
      56,   334,   220,    -1,    -1,    28,    59,   335,   220,    -1,
      -1,    28,     8,   336,   220,    -1,    28,    57,   129,    -1,
      28,    58,   129,    -1,    -1,    28,    60,   337,   220,    -1,
      -1,    28,    61,   338,   220,    -1,    -1,    28,    62,   339,
     220,    -1,    -1,    28,    63,   340,   220,    -1,    -1,    28,
      64,   341,   220,    -1,    -1,    28,    65,   342,   220,    -1,
      -1,    28,    66,   343,   220,    -1,    -1,    28,    67,   344,
     220,    -1,    28,    55,    93,   129,    -1,    28,    55,   135,
      93,   129,    -1,    28,    26,    40,   129,    -1,    -1,    28,
      89,   345,   220,    -1,    27,    28,    68,   353,   129,    -1,
      27,    28,    50,   353,   129,    -1,    27,    28,    13,    50,
     353,   129,    -1,    27,    28,    51,   353,   129,    -1,    27,
      28,    53,   353,   129,    -1,    27,    28,    13,    51,   353,
     129,    -1,    27,    28,    52,   353,   129,    -1,    27,    28,
      48,   353,   129,    -1,    27,    39,   351,   353,   129,    -1,
      27,    39,    43,   353,   129,    -1,    27,    39,    43,    44,
     353,   129,    -1,    27,    31,   352,   353,   129,    -1,    27,
      54,   353,   129,    -1,    -1,    -1,    27,   362,   346,   415,
     347,   353,   129,    -1,    -1,    -1,    27,     7,    74,   348,
     415,   349,   353,   129,    -1,    27,    69,   353,   129,    -1,
      27,    71,   154,   353,   129,    -1,    27,    71,    65,   353,
     129,    -1,    27,    78,    79,   353,   129,    -1,    27,    89,
     353,   129,    -1,    27,    71,    89,   353,   129,    -1,    82,
     129,    -1,    83,   129,    -1,    84,   129,    -1,    85,   129,
      -1,    27,    48,    28,   353,   129,    -1,    27,    50,   353,
     129,    -1,    -1,     1,   350,   129,    -1,    41,    -1,    42,
      -1,    45,    47,    -1,    45,    46,    -1,    40,    -1,    76,
      -1,    90,    -1,    91,    -1,    92,    -1,   163,    -1,    -1,
      37,    -1,    34,    -1,    38,    -1,    35,    -1,    63,    -1,
      64,    -1,    65,    -1,    12,    -1,    32,    -1,    33,    -1,
      29,    -1,    30,    -1,    35,    -1,    36,    -1,    75,    -1,
      74,    -1,    76,    -1,    77,    -1,    14,    -1,    15,    -1,
      26,    -1,    17,    -1,    16,    -1,    80,    -1,    -1,    81,
      -1,    -1,    93,    -1,    -1,    72,    -1,    73,    -1,    74,
      -1,   401,    -1,    -1,   404,   364,   220,    -1,    -1,   125,
     366,   220,    -1,   220,    -1,    -1,   131,   368,   370,    -1,
     401,    -1,    -1,   405,   369,   220,    -1,    -1,   169,   371,
     373,    -1,    -1,   406,   372,   220,    -1,   399,    -1,    -1,
     132,   374,   363,    -1,    -1,   407,   375,   220,    -1,   399,
      -1,    -1,   131,   377,   379,    -1,   401,    -1,    -1,   405,
     378,   220,    -1,    -1,   169,   380,   383,    -1,    -1,   133,
     381,   393,    -1,    -1,   408,   382,   220,    -1,   399,    -1,
      -1,   132,   384,   363,    -1,    -1,   133,   385,   387,    -1,
      -1,   409,   386,   220,    -1,   399,    -1,    -1,   169,   388,
     390,    -1,    -1,   406,   389,   220,    -1,   399,    -1,    -1,
     132,   391,   363,    -1,    -1,   407,   392,   220,    -1,   399,
      -1,    -1,   169,   394,   396,    -1,    -1,   406,   395,   220,
      -1,   399,    -1,    -1,   132,   397,   363,    -1,    -1,   407,
     398,   220,    -1,   399,    -1,    -1,   127,   400,   220,    -1,
     220,    -1,    -1,   127,   402,   220,    -1,    -1,   167,   403,
     365,    -1,   170,    -1,   168,    -1,   169,    -1,   131,    -1,
     132,    -1,   125,    -1,   129,    -1,   133,    -1,   168,    -1,
     169,    -1,   132,    -1,   125,    -1,   129,    -1,   133,    -1,
     168,    -1,   131,    -1,   132,    -1,   167,    -1,   125,    -1,
     133,    -1,   168,    -1,   169,    -1,   131,    -1,   167,    -1,
     125,    -1,   133,    -1,   168,    -1,   131,    -1,   132,    -1,
     167,    -1,   125,    -1,   168,    -1,   169,    -1,   131,    -1,
     167,    -1,   125,    -1,   413,   411,    -1,   412,    -1,   129,
      -1,   412,    -1,   170,    -1,   413,   414,    -1,   414,    -1,
     415,    -1,   221,    -1,   168,    -1,   169,    -1,   131,    -1,
     132,    -1,   167,    -1,   125,    -1,   133,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   233,   233,   235,   240,   241,   242,   243,   249,   249,
     259,   259,   269,   273,   273,   284,   284,   295,   306,   306,
     311,   316,   349,   353,   357,   358,   359,   360,   368,   377,
     378,   388,   389,   392,   393,   394,   401,   410,   410,   419,
     422,   423,   430,   435,   429,   441,   442,   445,   446,   449,
     453,   457,   458,   457,   464,   464,   466,   472,   472,   474,
     477,   478,   481,   482,   482,   484,   484,   486,   486,   493,
     495,   503,   493,   516,   517,   520,   525,   528,   532,   533,
     533,   535,   538,   539,   549,   557,   548,   566,   565,   598,
     599,   608,   608,   615,   617,   615,   630,   631,   640,   641,
     644,   645,   648,   656,   657,   665,   674,   675,   678,   678,
     681,   682,   685,   692,   692,   695,   695,   698,   699,   698,
     702,   704,   706,   707,   706,   710,   711,   712,   710,   715,
     716,   715,   719,   720,   721,   719,   724,   725,   724,   728,
     729,   730,   728,   733,   734,   733,   737,   743,   742,   749,
     749,   752,   763,   764,   767,   768,   771,   776,   777,   780,
     781,   784,   785,   788,   789,   790,   797,   803,   806,   807,
     812,   819,   826,   827,   830,   831,   834,   835,   838,   843,
     843,   850,   851,   854,   855,   858,   859,   862,   866,   870,
     876,   880,   884,   884,   886,   890,   894,   895,   895,   897,
     897,   899,   899,   901,   901,   903,   907,   911,   916,   916,
     918,   922,   926,   930,   934,   934,   936,   937,   947,   948,
     951,   952,   955,   956,   959,   960,   961,   962,   968,   970,
     976,   978,   986,   987,   990,   991,   992,   992,   999,  1011,
    1012,  1012,  1016,  1016,  1018,  1024,  1024,  1024,  1024,  1024,
    1027,  1027,  1027,  1028,  1028,  1028,  1031,  1031,  1031,  1031,
    1032,  1032,  1032,  1032,  1032,  1032,  1035,  1035,  1035,  1036,
    1036,  1036,  1036,  1037,  1037,  1037,  1037,  1037,  1040,  1040,
    1046,  1046,  1046,  1046,  1046,  1046,  1046,  1046,  1047,  1047,
    1050,  1050,  1050,  1050,  1053,  1053,  1056,  1056,  1056,  1056,
    1056,  1056,  1057,  1057,  1057,  1058,  1058,  1058,  1058,  1058,
    1058,  1058,  1058,  1061,  1061,  1061,  1061,  1061,  1061,  1061,
    1064,  1064,  1064,  1067,  1067,  1067,  1067,  1067,  1067,  1067,
    1068,  1068,  1068,  1068,  1068,  1068,  1069,  1069,  1069,  1069,
    1070,  1070,  1070,  1070,  1071,  1071,  1071,  1098,  1098,  1103,
    1103,  1110,  1109,  1122,  1121,  1134,  1133,  1146,  1145,  1158,
    1157,  1170,  1169,  1183,  1182,  1196,  1195,  1208,  1207,  1221,
    1220,  1233,  1232,  1245,  1244,  1258,  1257,  1269,  1274,  1273,
    1284,  1290,  1290,  1296,  1296,  1302,  1302,  1308,  1308,  1314,
    1322,  1322,  1328,  1328,  1334,  1334,  1340,  1340,  1346,  1350,
    1354,  1354,  1360,  1360,  1366,  1366,  1372,  1372,  1378,  1378,
    1384,  1384,  1390,  1390,  1396,  1396,  1402,  1406,  1410,  1414,
    1414,  1423,  1427,  1431,  1435,  1439,  1443,  1447,  1451,  1455,
    1459,  1463,  1467,  1471,  1475,  1476,  1475,  1481,  1482,  1481,
    1487,  1491,  1495,  1499,  1503,  1507,  1514,  1518,  1522,  1526,
    1533,  1537,  1544,  1544,  1551,  1552,  1553,  1554,  1555,  1556,
    1557,  1558,  1559,  1560,  1563,  1564,  1565,  1566,  1567,  1568,
    1569,  1570,  1571,  1572,  1573,  1576,  1577,  1580,  1581,  1584,
    1585,  1588,  1589,  1594,  1595,  1596,  1599,  1600,  1603,  1604,
    1607,  1608,  1611,  1612,  1615,  1616,  1617,  1626,  1627,  1627,
    1631,  1631,  1633,  1643,  1643,  1645,  1646,  1646,  1651,  1651,
    1653,  1653,  1655,  1659,  1659,  1661,  1661,  1663,  1676,  1676,
    1678,  1679,  1679,  1688,  1688,  1690,  1690,  1692,  1692,  1694,
    1702,  1702,  1704,  1704,  1706,  1706,  1708,  1716,  1716,  1718,
    1718,  1720,  1729,  1728,  1735,  1735,  1737,  1745,  1745,  1747,
    1747,  1749,  1757,  1757,  1759,  1759,  1761,  1764,  1764,  1766,
    1770,  1770,  1772,  1772,  1774,  1785,  1785,  1785,  1785,  1785,
    1785,  1785,  1788,  1788,  1788,  1788,  1788,  1788,  1792,  1792,
    1792,  1792,  1792,  1792,  1796,  1796,  1796,  1796,  1796,  1796,
    1800,  1800,  1800,  1800,  1800,  1804,  1804,  1804,  1804,  1804,
    1810,  1811,  1814,  1815,  1818,  1825,  1826,  1829,  1834,  1841,
    1841,  1841,  1841,  1841,  1841,  1841
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
  "KW_GET", "KW_VARIANTS", "KW_VARIANT", "KW_EREWRITE", "KW_FREWRITE",
  "KW_SREWRITE", "KW_CONTINUE", "KW_SEARCH", "KW_SET", "KW_SHOW", "KW_ON",
  "KW_OFF", "KW_TRACE", "KW_BODY", "KW_BUILTIN", "KW_WHOLE", "KW_SELECT",
  "KW_DESELECT", "KW_CONDITION", "KW_SUBSTITUTION", "KW_PRINT", "KW_GRAPH",
  "KW_MIXFIX", "KW_FLAT", "KW_ATTRIBUTE", "KW_NEWLINE", "KW_WITH",
  "KW_PARENS", "KW_ALIASES", "KW_GC", "KW_TIME", "KW_STATS", "KW_TIMING",
  "KW_CMD", "KW_BREAKDOWN", "KW_BREAK", "KW_PATH", "KW_MODULE",
  "KW_MODULES", "KW_VIEWS", "KW_ALL", "KW_SORTS", "KW_OPS2", "KW_VARS",
  "KW_MBS", "KW_EQS", "KW_RLS", "KW_SUMMARY", "KW_KINDS", "KW_ADVISE",
  "KW_VERBOSE", "KW_DO", "KW_CLEAR", "KW_PROTECT", "KW_EXTEND",
  "KW_INCLUDE", "KW_EXCLUDE", "KW_CONCEAL", "KW_REVEAL", "KW_COMPILE",
  "KW_COUNT", "KW_DEBUG", "KW_IRREDUNDANT", "KW_RESUME", "KW_ABORT",
  "KW_STEP", "KW_WHERE", "KW_CREDUCE", "KW_SREDUCE", "KW_DUMP",
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
  "$@91", "$@92", "$@93", "$@94", "$@95", "$@96", "$@97", "printOption",
  "traceOption", "polarity", "select", "exclude", "conceal", "search",
  "match", "optDebug", "optIrredundant", "optNumber", "importMode",
  "moduleAndTerm", "$@98", "inEnd", "$@99", "numberModuleTerm", "$@100",
  "$@101", "numberModuleTerm1", "$@102", "$@103", "numberModuleTerm2",
  "$@104", "$@105", "numbersModuleTerm", "$@106", "$@107",
  "numbersModuleTerm1", "$@108", "$@109", "$@110", "numbersModuleTerm2",
  "$@111", "$@112", "$@113", "numbersModuleTerm3", "$@114", "$@115",
  "numbersModuleTerm4", "$@116", "$@117", "numbersModuleTerm5", "$@118",
  "$@119", "numbersModuleTerm6", "$@120", "$@121", "miscEndBubble",
  "$@122", "initialEndBubble", "$@123", "$@124", "cTokenBarIn",
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
     375,   376,   377,   378,   379,    58,    61,    40,    41,    46,
      60,    91,    93,    44,   124,   380,   381,   382,    43,    42,
     123,   125,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,   408,   409,   410,
     411,   412
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   172,   173,   173,   174,   174,   174,   174,   176,   175,
     177,   175,   175,   178,   175,   179,   175,   175,   180,   175,
     175,   175,   181,   181,   181,   181,   181,   181,   181,   182,
     182,   183,   183,   184,   184,   184,   185,   187,   186,   188,
     189,   189,   191,   192,   190,   193,   193,   194,   194,   195,
     195,   196,   197,   195,   199,   198,   198,   201,   200,   200,
     202,   202,   203,   204,   203,   205,   203,   206,   203,   208,
     209,   210,   207,   211,   211,   212,   212,   213,   213,   214,
     213,   213,   215,   215,   217,   218,   216,   219,   216,   220,
     220,   222,   221,   224,   225,   223,   226,   226,   227,   227,
     228,   228,   229,   230,   230,   231,   232,   232,   233,   233,
     234,   234,   235,   236,   235,   237,   235,   238,   239,   235,
     235,   235,   240,   241,   235,   242,   243,   244,   235,   245,
     246,   235,   247,   248,   249,   235,   250,   251,   235,   252,
     253,   254,   235,   255,   256,   235,   235,   257,   235,   258,
     235,   235,   259,   259,   260,   260,   261,   262,   262,   263,
     263,   264,   264,   265,   265,   265,   266,   266,   267,   267,
     267,   268,   269,   269,   270,   270,   271,   271,   272,   273,
     272,   274,   274,   275,   275,   276,   276,   277,   277,   277,
     278,   278,   279,   278,   278,   278,   278,   280,   278,   281,
     278,   282,   278,   283,   278,   278,   278,   278,   284,   278,
     278,   278,   278,   278,   285,   278,   278,   278,   286,   286,
     287,   287,   288,   288,   289,   289,   289,   289,   290,   290,
     291,   291,   292,   292,   293,   293,   294,   293,   295,   296,
     297,   296,   299,   298,   298,   300,   300,   300,   300,   300,
     301,   301,   301,   301,   301,   301,   302,   302,   302,   302,
     302,   302,   302,   302,   302,   302,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   304,   304,
     305,   305,   305,   305,   305,   305,   305,   305,   305,   305,
     306,   306,   306,   306,   307,   307,   308,   308,   308,   308,
     308,   308,   308,   308,   308,   308,   308,   308,   308,   308,
     308,   308,   308,   309,   309,   309,   309,   309,   309,   309,
     310,   310,   310,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   311,
     311,   311,   311,   311,   311,   311,   311,   313,   312,   314,
     312,   315,   312,   316,   312,   317,   312,   318,   312,   319,
     312,   320,   312,   321,   312,   322,   312,   323,   312,   324,
     312,   325,   312,   326,   312,   327,   312,   312,   328,   312,
     312,   329,   312,   330,   312,   331,   312,   332,   312,   312,
     333,   312,   334,   312,   335,   312,   336,   312,   312,   312,
     337,   312,   338,   312,   339,   312,   340,   312,   341,   312,
     342,   312,   343,   312,   344,   312,   312,   312,   312,   345,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   312,   312,   346,   347,   312,   348,   349,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   312,
     312,   312,   350,   312,   351,   351,   351,   351,   351,   351,
     351,   351,   351,   351,   352,   352,   352,   352,   352,   352,
     352,   352,   352,   352,   352,   353,   353,   354,   354,   355,
     355,   356,   356,   357,   357,   357,   358,   358,   359,   359,
     360,   360,   361,   361,   362,   362,   362,   363,   364,   363,
     366,   365,   365,   368,   367,   367,   369,   367,   371,   370,
     372,   370,   370,   374,   373,   375,   373,   373,   377,   376,
     376,   378,   376,   380,   379,   381,   379,   382,   379,   379,
     384,   383,   385,   383,   386,   383,   383,   388,   387,   389,
     387,   387,   391,   390,   392,   390,   390,   394,   393,   395,
     393,   393,   397,   396,   398,   396,   396,   400,   399,   399,
     402,   401,   403,   401,   401,   404,   404,   404,   404,   404,
     404,   404,   405,   405,   405,   405,   405,   405,   406,   406,
     406,   406,   406,   406,   407,   407,   407,   407,   407,   407,
     408,   408,   408,   408,   408,   409,   409,   409,   409,   409,
     410,   410,   411,   411,   412,   413,   413,   414,   414,   415,
     415,   415,   415,   415,   415,   415
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
       3,     0,     3,     0,     5,     0,     6,     4,     0,     3,
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

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   452,   108,   109,    69,   351,   378,   483,   484,   487,
     486,   371,   485,     0,     0,     0,   347,     0,     0,     0,
     488,     0,     0,     0,     0,   353,   355,   349,    12,    13,
      15,    17,    18,    10,    20,    21,    91,     8,     0,     2,
       6,     5,   380,     4,    93,     7,   367,   369,     0,     0,
       0,     0,     0,     0,     0,     0,   464,     0,     0,     0,
       0,     0,     0,   494,   495,   496,     0,     0,   434,   390,
     396,     0,     0,   392,     0,     0,   394,   400,   402,   404,
     406,   408,   410,   412,   414,   419,   477,   478,   480,   479,
     381,   383,     0,   481,   482,   387,   385,     0,   446,   447,
     448,   449,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     1,     0,     0,     0,   357,   359,   491,     0,   361,
     363,   365,   493,   453,   311,   296,   312,   297,   298,   299,
     300,   302,   301,   303,   304,   305,   306,   307,   308,   309,
     310,   289,   288,   315,   318,   316,   319,   314,   317,   249,
     313,   320,   321,   292,   285,   287,   293,   286,   283,   284,
     281,   282,   323,   324,   325,   326,   327,   328,   329,   330,
     331,   339,   332,   333,   334,   338,   335,   336,   337,   340,
     341,   294,   342,   343,   344,   345,   346,   280,   291,     0,
     290,   245,   246,   295,   247,   248,   322,   569,   560,   570,
     567,   568,   571,   562,   565,   566,   564,   352,   497,   498,
     379,   575,   576,   503,   574,   577,   572,   573,   372,   505,
     506,   437,     0,     0,     0,     0,     0,     0,     0,   472,
     473,   474,   466,   468,   465,   467,   469,   470,   471,     0,
     458,   454,   455,     0,     0,   459,   460,   461,   462,   463,
       0,     0,   475,   476,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   398,
     399,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    89,    90,   348,     0,     0,     0,   354,
     356,   350,    14,    16,    19,    11,    92,     9,    94,   518,
     368,   520,   521,   370,     0,     0,   490,     0,   373,     0,
       0,     0,   492,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   457,   456,     0,     0,   451,   433,   440,     0,
       0,     0,     0,   444,   614,   611,   612,   615,   613,   609,
     610,   435,   391,   397,   418,   416,     0,   393,   395,   401,
     403,   405,   407,   409,   411,   413,   415,   420,   604,   608,
     382,   601,     0,   606,   607,   384,   388,   386,   389,    99,
       0,     0,   358,   360,   375,     0,   362,   364,   366,   377,
       0,    70,    29,    31,    32,    34,    33,    35,   561,   500,
     502,   563,   499,   582,   557,   579,   580,   583,   581,   578,
     508,   559,   504,   512,   510,   507,   438,     0,     0,   428,
     422,   424,   427,   425,   421,   432,     0,   430,   429,   450,
     442,   445,   441,   443,     0,   417,   602,   600,   603,   605,
       0,   229,   594,   591,   592,   525,   593,   590,   523,   519,
     529,   527,   522,     0,   374,     0,     0,     0,     0,    37,
       0,     0,     0,     0,     0,   423,   426,   431,     0,     0,
     101,     0,   228,   111,     0,     0,     0,   376,    39,    30,
       0,    42,    36,     0,   501,   558,   588,   586,   513,   589,
     587,   584,   585,   509,   517,   515,   511,     0,   436,     0,
      98,   104,   103,     0,     0,   547,   526,   551,   549,   599,
     597,   530,   532,   598,   595,   596,   524,   536,   534,   528,
      71,     0,     0,    41,     0,     0,   439,   100,   102,     0,
      95,     0,   113,   115,   117,     0,     0,     0,     0,   149,
     122,   125,   129,   132,   136,   139,   143,   110,     0,     0,
       0,     0,     0,   229,    46,     0,    51,     0,    43,    45,
      48,     0,    38,   514,   516,   151,   249,   251,   252,    28,
     112,     0,   231,   231,   231,   231,     0,   250,   253,   254,
     255,   233,     0,     0,   261,   264,   262,   265,   263,   259,
     260,   162,     0,   160,   161,   256,   257,   258,     0,     0,
     158,   147,     0,     0,     0,     0,     0,     0,     0,     0,
     552,   548,   556,   554,   550,   531,   537,   533,   541,   539,
     535,    74,   275,   273,   274,   269,   270,   276,   277,   271,
     272,   266,     0,   238,   239,   267,   268,    56,     0,     0,
       0,    40,     0,   230,    25,    26,    24,    22,    23,     0,
       0,   236,   118,     0,   159,     0,    76,   157,     0,     0,
     123,   126,   130,   133,   137,   140,   144,     0,     0,     0,
       0,     0,     0,   240,    54,     0,     0,    44,    47,    27,
      30,    96,    97,   114,   232,   235,   116,   234,   233,     0,
     174,   175,   179,   105,   165,   120,   164,     0,   177,   178,
     146,    75,     0,   152,     0,   150,     0,     0,     0,     0,
       0,     0,     0,   553,   555,   542,   538,   546,   544,   540,
       0,    72,     0,    79,     0,    73,    49,     0,   177,    52,
      50,     0,   119,     0,   173,   171,   184,   167,   107,   121,
     231,   251,   252,   153,   154,     0,   250,   253,   254,   255,
     148,   124,   127,   131,   134,   138,   141,   145,     0,     0,
      81,     0,     0,     0,   244,     0,     0,    59,   237,     0,
     182,     0,   231,   166,   169,   170,     0,   163,   176,   106,
       0,     0,     0,     0,     0,   543,   545,     0,    84,    87,
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
      -1,    38,    39,    40,   110,   108,   105,   106,   107,   570,
     391,   392,   393,   394,   395,   483,   396,   522,   482,   521,
     639,   558,   559,   560,   637,   767,   675,   728,   796,   839,
     864,   865,   899,   901,   900,    41,    50,   457,   553,   671,
     702,   725,   762,   832,   790,   834,   916,   835,   411,   369,
     109,    43,   112,   379,   683,   441,   469,   470,   503,   694,
     739,    44,   504,   547,   581,   582,   583,   689,   603,   706,
     604,   707,   782,   605,   708,   606,   709,   783,   607,   710,
     608,   711,   784,   609,   712,   658,   602,   704,   743,   744,
     599,   592,   593,   695,   776,   777,   696,   735,   697,   737,
     698,   733,   769,   772,   821,   822,   823,   854,   866,   876,
     867,   868,   869,   870,   878,   887,   874,   875,   473,   644,
     649,   650,   688,   699,   633,   727,   765,   837,   397,   575,
     594,   634,   576,   190,   191,   635,   193,   194,   195,   196,
      45,    92,   104,    51,   102,   103,   304,   305,   309,   310,
     311,   113,   114,    53,   385,   453,    52,   281,   282,   287,
     286,   263,   268,   271,   264,   272,   273,   274,   275,   276,
     277,   278,   279,   280,   262,   434,   320,   464,    49,   250,
     239,   254,    90,    91,    95,    46,    47,    48,   307,   313,
      68,   207,   317,   401,   460,   218,   318,   319,   412,   462,
     463,   493,   524,   525,   300,   380,   381,   449,   475,   474,
     476,   516,   550,   551,   552,   617,   669,   670,   716,   758,
     759,   506,   548,   549,   611,   667,   668,   413,   461,   208,
     315,   316,   209,   220,   414,   495,   451,   518,   370,   437,
     371,   372,   373,   374
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -745
static const yytype_int16 yypact[] =
{
     631,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,   267,   502,    30,  -745,     3,   182,   -10,
    -745,   -37,    -8,    14,    49,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,    85,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,   458,    65,
    1069,   413,   413,   618,   140,    47,   383,   288,   206,   221,
     221,   221,   -38,  -745,  -745,  -745,   157,   221,  -745,  -745,
    -745,   243,   -52,  -745,   114,   163,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,   -99,  -745,  -745,  -745,  -745,   131,  -745,  -745,
    -745,  -745,   413,   413,   -99,   300,   309,   306,   310,   204,
     339,  -745,  1069,  2069,   618,  -745,  -745,   268,   328,  -745,
    -745,  -745,   255,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,   234,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,   215,   221,   221,   221,   221,   221,   221,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,   221,
    -745,  -745,  -745,    20,   233,  -745,  -745,  -745,  -745,  -745,
     221,   221,  -745,  -745,   229,   230,   238,   221,   221,   221,
     221,   239,   156,   -99,   -99,   242,   253,   291,   -99,  -745,
    -745,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,   -99,
     -99,   297,   297,  -745,  -745,  -745,   297,   297,   260,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,   413,   618,  -745,   373,  -745,  2069,
    2069,   618,  -745,   269,   793,   -99,   -77,   -99,  2078,   -99,
     156,   221,   221,   272,   282,   283,   285,   290,   294,   312,
     221,   315,  -745,  -745,   320,   323,  -745,  -745,  -745,   325,
     326,   333,   334,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,   345,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  2087,  -745,  -745,  -745,  -745,  -745,  -745,   256,
    2096,   -99,  -745,  -745,  -745,   618,  -745,  -745,  -745,  -745,
     793,   261,   296,   298,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,   347,   349,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,   355,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,   221,  -745,  -745,  -745,  -745,  -745,
    1069,   338,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,   618,  -745,    37,   793,   303,   358,  -745,
     -99,   -99,  2142,   -99,   221,  -745,  -745,  -745,   357,    71,
    -745,   -82,  -745,  -745,  2151,  2160,   -99,  -745,  -745,  -745,
     793,  -745,  -745,  1069,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,   360,  -745,  1069,
    -745,  -745,  -745,   793,    46,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
     261,     4,    86,  -745,   413,   -99,  -745,  -745,   261,   361,
    -745,   862,  -745,  -745,  -745,  1138,  1138,  1345,  1069,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  2169,   -99,
     413,  2215,   -99,   338,  -745,  2027,  -745,  1069,  -745,   354,
    -745,  1069,  -745,  -745,  -745,  -745,   362,   -81,    29,   124,
    -745,   359,     8,   252,   265,   367,   370,   223,   235,   270,
     293,  -745,  2027,   375,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,   931,  -745,  -745,  -745,  -745,  -745,  1000,  1207,
    -745,  -745,  2027,   377,   378,   379,   380,   382,   385,   384,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,   376,   371,  -745,  -745,  -745,   388,   386,   387,
      31,  -745,   862,  -745,  -745,  -745,  -745,  -745,  -745,  1621,
    1552,  -745,  -745,  1414,  -745,  1414,   395,  -745,   389,  1552,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,   413,   -99,  2224,
     -99,    15,  2027,  -745,  -745,   390,  1069,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  1414,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  1690,  -745,  -745,
    -745,  -745,  1690,  1759,   391,  -745,   -99,   400,   -99,   401,
     -99,   403,  1414,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
     402,  -745,  2027,  -745,  1345,  -745,  -745,  2027,  -745,  -745,
    -745,  1893,  -745,  2027,  -745,  -745,   398,  1483,  -745,  -745,
     367,  -745,  -745,   397,  -745,   394,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,   413,   -99,
    -745,   396,   -69,  1276,   371,   108,  1826,   406,  -745,   167,
    -745,  2216,   367,  -745,  -745,  -745,    19,  -745,  -745,  -745,
    1759,  1069,   -99,   -99,   -99,  -745,  -745,   700,  -745,  -745,
    -745,   395,  -745,  -745,  1960,  -745,  -745,  -745,  2027,  -745,
    -745,  -745,  -745,  -745,   399,   404,   365,   407,   405,   412,
     414,  -745,   420,  -745,   423,   424,  -745,  -745,  -745,  -745,
     425,   620,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,   367,  -745,   -99,  1690,  2027,  -745,   194,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
     307,  -745,  -745,  -745,   415,  -745,  1826,  -745,  -745,   371,
     408,   426,   427,   443,   203,  -745,   410,   410,   410,   410,
     566,  1069,  1069,  1069,    81,  -745,   410,  -745,  -745,  1960,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,   -86,   -55,   -54,
     -50,   444,   447,   447,   447,  -745,  -745,   -44,   439,   410,
     579,   410,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,   -18,   457,     2,   -99,  -745,  -745,  -745,
    -745
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,   -53,
    -386,  -745,  -745,  -519,  -516,  -745,  -514,  -745,  -745,  -745,
    -745,  -745,  -745,   -47,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -277,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -203,  -745,  -745,  -745,  -745,  -745,  -745,  -745,   -91,     0,
    -745,  -745,  -745,  -745,  -573,  -745,  -745,    91,  -745,  -662,
    -242,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -185,
    -128,    61,  -457,  -621,  -745,  -745,  -176,  -745,  -744,  -700,
    -429,  -745,  -745,  -745,  -745,  -745,  -220,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -592,  -745,  -272,    50,  -537,
     -84,     5,  -745,  -345,  -704,  -745,  -745,  -745,   -40,  -684,
    -523,  -745,  -745,  -513,  -491,   -43,  -745,  -510,  -511,  -411,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,   -28,   587,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,   -49,  -745,  -745,  -745,  -103,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,     9,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,  -745,
    -745,  -745,  -745,  -745,  -745,  -745,  -745,  -372,  -745,   -51,
    -745,  -745,  -745,  -104,  -445,  -542,  -745,  -745,   -66,  -745,
     236,  -745,   237,  -231
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -490
static const yytype_int16 yytable[] =
{
      42,   285,   219,   210,   455,   554,   613,   192,   450,   302,
     189,   303,   572,   291,   600,   573,   720,   574,   577,   745,
     580,   579,   794,   764,   597,   597,   597,   257,   766,   508,
     283,   351,   255,   256,   700,   734,   645,   646,   647,   261,
     738,   266,   902,   501,   595,   595,   595,   529,   399,   252,
     253,   258,   283,   289,   290,   502,   788,  -292,  -292,  -292,
     222,    97,   301,   219,   330,    86,    87,   789,   732,   192,
     479,   284,   298,   904,   905,   775,   657,   686,   906,    93,
      94,   597,   903,   267,   911,   111,   705,   597,   597,   416,
     494,   757,    98,   284,   520,   223,   745,   224,   225,   226,
     227,   595,   507,   517,    88,    89,   619,   595,   595,   555,
     917,   556,   879,   903,   903,   228,   259,   528,   903,   721,
     722,    99,   723,   572,   903,   724,   573,   718,   574,   577,
     919,   580,   579,   859,   856,   654,   555,   643,   556,   557,
     690,   654,   691,   100,   636,   571,   -32,   -32,   530,   531,
     903,   532,   533,   534,   535,   536,   537,   538,   539,   540,
     541,   542,   543,   544,   545,   478,   557,  -293,  -293,  -293,
     903,   636,   352,   353,   738,   456,   612,   357,   101,   618,
     358,   359,   360,   361,   362,   363,   364,   365,   366,   367,
     746,   636,   749,   748,   123,   323,   324,   325,   326,   327,
     328,   600,   383,   779,   499,   302,   302,   546,   388,   895,
     632,   329,   500,   597,   221,   331,   375,    86,    87,   561,
     376,   377,   334,   335,   398,   400,   402,   562,   415,   339,
     340,   341,   342,   595,   251,   824,   260,   651,   636,   636,
     657,   792,   636,   269,   636,   871,   872,   873,   636,   793,
     252,   253,   597,  -279,   219,   382,   680,   651,   301,   301,
     219,   636,  -291,  -291,  -291,   321,   322,   746,   736,   749,
     748,   192,   595,   740,    54,   888,   889,   890,   636,   332,
     333,   344,   454,   265,   897,   288,   636,   345,   346,   347,
     452,   636,   270,   417,   418,    55,   855,   717,    56,   797,
     798,   636,   426,   292,   684,   687,    57,   913,   778,   915,
     294,   636,   293,   295,   687,    58,   636,    59,   386,   387,
     636,    60,   636,   348,   349,   350,   636,   726,   240,   241,
     242,   243,   296,   244,   219,   884,    61,   778,    62,    63,
      64,    65,   297,   860,   861,    66,   308,   192,   312,   306,
     477,   862,   860,   861,   314,   636,    67,   863,   336,   337,
     862,  -290,  -290,  -290,   245,   838,   863,   338,   343,   484,
     485,   354,   496,  -246,  -246,  -246,   636,   761,   246,   247,
     248,   643,   355,   636,   356,   519,   684,   636,   770,   378,
     -34,   -34,   -34,   384,   643,   229,   440,   192,   389,   456,
     471,   419,   219,   -33,   -33,   -33,   468,   740,  -247,  -247,
    -247,   420,   421,   192,   422,   230,   231,   232,   233,   423,
     234,   235,   344,   424,    36,   636,   636,   778,   345,   346,
     347,  -248,  -248,  -248,   564,   458,   497,   192,   459,   480,
     192,   425,   833,   523,   427,   636,   236,   237,   238,   428,
     898,   249,   429,   840,   430,   431,   192,   472,   614,   471,
     192,   620,   432,   433,   348,   349,   350,   368,   636,   115,
     116,   871,   872,   873,   435,   563,   465,   117,   466,   118,
     119,   120,   121,   122,   467,   481,   498,   640,   578,   526,
     565,  -278,   596,   596,   596,   192,   643,   642,   601,   648,
     652,   615,   660,   661,   664,   662,   663,   665,    69,   666,
      70,   673,   672,   674,   192,   677,   701,   638,   192,   781,
     750,   641,   676,   703,   752,   754,   729,   756,    71,   771,
     780,   760,   787,   843,   844,   591,   591,   795,   197,   846,
     198,   847,   199,   841,   200,   201,   202,   848,   842,   596,
     849,   850,   851,   881,   882,   596,   596,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
     883,   891,   907,   845,    36,   912,   880,   714,   886,   719,
     203,   204,   205,   206,   914,   918,   877,   885,   836,   679,
     527,    85,   591,   678,   858,   826,   763,   598,   591,   578,
     825,   853,   896,   621,   731,    96,     0,   659,   438,   439,
       0,     0,     0,     0,     0,   751,     0,   753,   713,   755,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    -3,     1,   192,     0,     0,   730,     2,     3,     4,
       5,     0,  -489,  -489,     6,     7,     8,     9,    10,    11,
    -489,     0,  -489,  -489,  -489,  -489,  -489,    12,    13,    14,
     747,     0,    15,     0,     0,     0,    16,     0,   786,     0,
      17,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   596,     0,     0,     0,    18,     0,     0,     0,     0,
       0,   828,   829,   830,     0,     0,     0,     0,     0,     0,
       0,    19,     0,     0,     0,     0,     0,     0,     0,   785,
       0,    20,     0,    21,    22,    23,    24,    25,    26,    27,
     596,     0,     0,     0,     0,    28,    29,    30,    31,    32,
      33,    34,    35,     0,     0,     0,     0,   747,   192,     0,
       0,   827,     0,   211,   857,   198,     0,   212,     0,   213,
     214,   215,   852,     0,     0,     0,     0,     0,    36,     0,
       0,     0,   799,   800,   801,   802,   803,   804,   805,   806,
     807,   808,   809,   810,   811,   812,   813,   814,   815,   816,
     817,   818,   819,   820,     0,   203,   216,   217,   206,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    37,     0,
       0,     0,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   622,
       0,     0,   623,     0,   624,   920,   625,     0,   192,   192,
     192,   892,   893,   894,   626,   627,   628,     0,   629,   630,
       0,     0,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   631,     0,
     831,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   908,   909,   910,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     390,     0,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,   187,     0,   188,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   390,
       0,   566,   150,   151,   152,   567,   154,   155,   568,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     187,     0,   569,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   584,   585,   586,   587,   653,   588,    36,     0,
     589,   590,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   187,
       0,   188,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   584,   585,   586,   587,   655,   588,    36,     0,   589,
     590,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   187,     0,
     188,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,     0,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   187,     0,   188,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   584,
     585,   586,   587,     0,   588,    36,     0,   589,   590,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,     0,   187,     0,   188,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   584,   585,
     586,   587,   656,   588,     0,     0,   589,   590,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,     0,   187,     0,   188,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   584,   585,   586,
     587,   791,   588,     0,     0,   589,   590,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   187,     0,   188,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   584,   585,   586,   587,
       0,   588,     0,     0,   589,   590,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,     0,   187,     0,   188,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   622,     0,   690,   623,   691,   624,     0,
     625,     0,     0,     0,     0,   692,     0,     0,   626,   627,
     628,     0,   629,   630,     0,     0,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,   183,   184,   185,
     186,     0,   631,     0,   693,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   622,     0,     0,   623,     0,   624,   773,   625,
       0,     0,   774,     0,   692,     0,     0,   626,   627,   628,
       0,   629,   630,     0,     0,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,   631,     0,   693,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   622,     0,     0,   623,     0,   624,     0,   625,     0,
       0,   681,   685,     0,     0,     0,   626,   627,   628,     0,
     629,   630,     0,     0,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,     0,
     631,     0,   682,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     622,     0,     0,   623,     0,   624,     0,   625,     0,     0,
     681,     0,     0,     0,     0,   626,   627,   628,     0,   629,
     630,     0,     0,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,   183,   184,   185,   186,     0,   631,
       0,   682,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   622,
       0,     0,   623,     0,   624,     0,   625,     0,     0,     0,
       0,   692,     0,     0,   626,   627,   628,     0,   629,   630,
       0,     0,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   631,     0,
     693,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     0,     0,     0,   150,
     151,   152,   741,   154,   155,   742,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,     0,   187,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   622,     0,   690,   623,   691,
     624,     0,   625,     0,     0,     0,     0,   692,     0,     0,
     626,   627,   628,     0,   629,   630,     0,     0,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186,     0,   631,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   622,     0,     0,   623,     0,   624,     0,   625,
       0,     0,     0,   768,     0,     0,     0,   626,   627,   628,
       0,   629,   630,     0,     0,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
       0,   631,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   622,
       0,     0,   623,     0,   624,     0,   625,     0,     0,     0,
       0,   692,     0,     0,   626,   627,   628,     0,   629,   630,
       0,     0,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,     0,   631,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   622,     0,     0,   623,
       0,   624,     0,   625,     0,     0,     0,     0,     0,     0,
       0,   626,   627,   628,     0,   629,   630,     0,     0,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   211,   631,   198,     0,   212,     0,
     299,   214,   215,   403,     0,   404,     0,   283,     0,   405,
     406,   407,   344,     0,    36,     0,   436,     0,   345,   346,
     347,   442,     0,   404,     0,   283,     0,   443,   444,   445,
       0,     0,     0,     0,     0,     0,   203,   216,   217,   206,
       0,     0,     0,     0,     0,   408,   409,   410,   284,     0,
       0,     0,     0,     0,   348,   349,   350,   368,     0,     0,
       0,     0,     0,   446,   447,   448,   284,   486,     0,   404,
       0,   283,     0,   487,   488,   489,   403,     0,   404,     0,
     283,     0,   405,   406,   407,   509,     0,   404,     0,   283,
       0,   510,   511,   512,   486,     0,   404,     0,   283,     0,
     487,   610,   489,     0,     0,     0,     0,     0,     0,   490,
     491,   492,   284,     0,     0,     0,     0,     0,   408,   409,
     505,   284,     0,     0,     0,     0,     0,   513,   514,   515,
     284,     0,     0,     0,     0,     0,   490,   491,   492,   284,
     403,     0,   404,     0,   283,     0,   405,   406,   407,   486,
       0,   404,     0,   283,     0,   487,   715,   489,   799,   800,
     801,   802,   803,   804,   805,   806,   807,   808,   809,   810,
     811,   812,   813,   814,   815,   816,   817,   818,   819,   820,
       0,     0,   408,   409,   616,   284,     0,     0,     0,     0,
       0,   490,   491,   492,   284
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-745))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
       0,    92,    53,    52,   390,     1,   548,    50,   380,   113,
      50,   114,   531,   104,   537,   531,     1,   531,   531,   703,
     531,   531,   766,   727,   535,   536,   537,    65,   728,   474,
     129,   262,    60,    61,   655,   697,   573,   574,   575,    67,
     702,    93,   128,   125,   535,   536,   537,     1,   125,    29,
      30,    89,   129,   102,   103,   137,   125,   138,   139,   140,
      13,    71,   113,   114,    44,    35,    36,   136,   689,   112,
     456,   170,   112,   128,   128,   737,   599,   650,   128,    76,
      77,   592,   168,   135,   128,     0,   659,   598,   599,   320,
     462,   712,   129,   170,   480,    48,   780,    50,    51,    52,
      53,   592,   474,   475,    74,    75,   551,   598,   599,   105,
     128,   107,   856,   168,   168,    68,   154,   503,   168,   104,
     105,   129,   107,   642,   168,   110,   642,   669,   642,   642,
     128,   642,   642,   837,   834,   592,   105,   129,   107,   135,
     121,   598,   123,   129,   555,   531,   138,   139,   102,   103,
     168,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   128,   135,   138,   139,   140,
     168,   582,   263,   264,   836,   138,   548,   268,   129,   551,
     271,   272,   273,   274,   275,   276,   277,   278,   279,   280,
     703,   602,   703,   703,   129,   223,   224,   225,   226,   227,
     228,   724,   305,   740,   133,   309,   310,   161,   311,   128,
     555,   239,   141,   724,    74,   243,   282,    35,    36,   133,
     286,   287,   250,   251,   315,   316,   317,   141,   319,   257,
     258,   259,   260,   724,    28,   772,    79,   582,   649,   650,
     763,   133,   653,   129,   655,   164,   165,   166,   659,   141,
      29,    30,   763,   129,   305,   304,   642,   602,   309,   310,
     311,   672,   138,   139,   140,    50,    51,   780,   697,   780,
     780,   314,   763,   702,     7,   867,   868,   869,   689,    46,
      47,   125,   385,    40,   876,   154,   697,   131,   132,   133,
     381,   702,   129,   321,   322,    28,   833,   669,    31,   132,
     133,   712,   330,     3,   649,   650,    39,   899,   737,   901,
       4,   722,     3,     3,   659,    48,   727,    50,   309,   310,
     731,    54,   733,   167,   168,   169,   737,   672,    40,    41,
      42,    43,   128,    45,   385,   132,    69,   766,    71,    72,
      73,    74,     3,   149,   150,    78,    18,   390,    93,    81,
     453,   157,   149,   150,   120,   766,    89,   163,   129,   129,
     157,   138,   139,   140,    76,   794,   163,   129,   129,   460,
     461,   129,   463,   138,   139,   140,   787,   722,    90,    91,
      92,   129,   129,   794,    93,   476,   731,   798,   733,   129,
     138,   139,   140,    20,   129,    12,   140,   440,   129,   138,
     440,   129,   453,   138,   139,   140,   434,   836,   138,   139,
     140,   129,   129,   456,   129,    32,    33,    34,    35,   129,
      37,    38,   125,   129,   127,   836,   837,   856,   131,   132,
     133,   138,   139,   140,   525,   139,   464,   480,   140,   136,
     483,   129,   787,   483,   129,   856,    63,    64,    65,   129,
     879,   163,   129,   798,   129,   129,   499,   119,   549,   499,
     503,   552,   129,   129,   167,   168,   169,   170,   879,    11,
      12,   164,   165,   166,   129,   524,   129,    19,   129,    21,
      22,    23,    24,    25,   129,   127,   129,   133,   531,   129,
     129,   129,   535,   536,   537,   538,   129,   138,   538,   129,
     125,   550,   125,   125,   122,   126,   126,   122,     6,   125,
       8,   140,   136,   125,   557,   128,   121,   557,   561,   125,
     129,   561,   136,   134,   124,   124,   136,   124,    26,   131,
     133,   129,   136,   168,   127,   535,   536,   131,   125,   127,
     127,   127,   129,   144,   131,   132,   133,   127,   144,   592,
     127,   127,   127,   127,   127,   598,   599,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
     127,     5,   128,   168,   127,   136,   168,   668,   168,   670,
     167,   168,   169,   170,     5,   128,   171,   864,   791,   642,
     499,    89,   592,   640,   836,   780,   724,   536,   598,   642,
     776,   821,   874,   553,   688,    18,    -1,   602,   372,   372,
      -1,    -1,    -1,    -1,    -1,   706,    -1,   708,   667,   710,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,     0,     1,   676,    -1,    -1,   676,     6,     7,     8,
       9,    -1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    -1,    21,    22,    23,    24,    25,    26,    27,    28,
     703,    -1,    31,    -1,    -1,    -1,    35,    -1,   759,    -1,
      39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   724,    -1,    -1,    -1,    54,    -1,    -1,    -1,    -1,
      -1,   782,   783,   784,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   758,
      -1,    80,    -1,    82,    83,    84,    85,    86,    87,    88,
     763,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,    -1,    -1,    -1,    -1,   780,   781,    -1,
      -1,   781,    -1,   125,   835,   127,    -1,   129,    -1,   131,
     132,   133,   132,    -1,    -1,    -1,    -1,    -1,   127,    -1,
      -1,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,    -1,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   167,    -1,
      -1,    -1,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,   122,    -1,   124,   916,   126,    -1,   871,   872,
     873,   871,   872,   873,   134,   135,   136,    -1,   138,   139,
      -1,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,    -1,
     170,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   892,   893,   894,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,    -1,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   168,    -1,   170,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
      -1,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
     168,    -1,   170,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,    -1,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
      -1,   170,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,    -1,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,    -1,
     170,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,   168,    -1,   170,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,    -1,   126,   127,    -1,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,    -1,   168,    -1,   170,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,    -1,    -1,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,    -1,   168,    -1,   170,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,    -1,    -1,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,   168,    -1,   170,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
      -1,   126,    -1,    -1,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,    -1,   168,    -1,   170,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,    -1,
     126,    -1,    -1,    -1,    -1,   131,    -1,    -1,   134,   135,
     136,    -1,   138,   139,    -1,    -1,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,    -1,   168,    -1,   170,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,   122,    -1,   124,   125,   126,
      -1,    -1,   129,    -1,   131,    -1,    -1,   134,   135,   136,
      -1,   138,   139,    -1,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   168,    -1,   170,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,    -1,   122,    -1,   124,    -1,   126,    -1,
      -1,   129,   130,    -1,    -1,    -1,   134,   135,   136,    -1,
     138,   139,    -1,    -1,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,    -1,
     168,    -1,   170,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    -1,    -1,   122,    -1,   124,    -1,   126,    -1,    -1,
     129,    -1,    -1,    -1,    -1,   134,   135,   136,    -1,   138,
     139,    -1,    -1,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,    -1,   168,
      -1,   170,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,   122,    -1,   124,    -1,   126,    -1,    -1,    -1,
      -1,   131,    -1,    -1,   134,   135,   136,    -1,   138,   139,
      -1,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,    -1,
     170,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,    -1,    -1,    -1,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,    -1,   168,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,    -1,   126,    -1,    -1,    -1,    -1,   131,    -1,    -1,
     134,   135,   136,    -1,   138,   139,    -1,    -1,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,    -1,   168,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,    -1,   122,    -1,   124,    -1,   126,
      -1,    -1,    -1,   130,    -1,    -1,    -1,   134,   135,   136,
      -1,   138,   139,    -1,    -1,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
      -1,   168,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,    -1,   122,    -1,   124,    -1,   126,    -1,    -1,    -1,
      -1,   131,    -1,    -1,   134,   135,   136,    -1,   138,   139,
      -1,    -1,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,    -1,   168,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,    -1,   122,
      -1,   124,    -1,   126,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   134,   135,   136,    -1,   138,   139,    -1,    -1,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   125,   168,   127,    -1,   129,    -1,
     131,   132,   133,   125,    -1,   127,    -1,   129,    -1,   131,
     132,   133,   125,    -1,   127,    -1,   129,    -1,   131,   132,
     133,   125,    -1,   127,    -1,   129,    -1,   131,   132,   133,
      -1,    -1,    -1,    -1,    -1,    -1,   167,   168,   169,   170,
      -1,    -1,    -1,    -1,    -1,   167,   168,   169,   170,    -1,
      -1,    -1,    -1,    -1,   167,   168,   169,   170,    -1,    -1,
      -1,    -1,    -1,   167,   168,   169,   170,   125,    -1,   127,
      -1,   129,    -1,   131,   132,   133,   125,    -1,   127,    -1,
     129,    -1,   131,   132,   133,   125,    -1,   127,    -1,   129,
      -1,   131,   132,   133,   125,    -1,   127,    -1,   129,    -1,
     131,   132,   133,    -1,    -1,    -1,    -1,    -1,    -1,   167,
     168,   169,   170,    -1,    -1,    -1,    -1,    -1,   167,   168,
     169,   170,    -1,    -1,    -1,    -1,    -1,   167,   168,   169,
     170,    -1,    -1,    -1,    -1,    -1,   167,   168,   169,   170,
     125,    -1,   127,    -1,   129,    -1,   131,   132,   133,   125,
      -1,   127,    -1,   129,    -1,   131,   132,   133,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
      -1,    -1,   167,   168,   169,   170,    -1,    -1,    -1,    -1,
      -1,   167,   168,   169,   170
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    14,    15,    16,
      17,    18,    26,    27,    28,    31,    35,    39,    54,    70,
      80,    82,    83,    84,    85,    86,    87,    88,    94,    95,
      96,    97,    98,    99,   100,   101,   127,   167,   173,   174,
     175,   207,   221,   223,   233,   312,   357,   358,   359,   350,
     208,   315,   328,   325,     7,    28,    31,    39,    48,    50,
      54,    69,    71,    72,    73,    74,    78,    89,   362,     6,
       8,    26,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    89,    35,    36,    74,    75,
     354,   355,   313,    76,    77,   356,   354,    71,   129,   129,
     129,   129,   316,   317,   314,   178,   179,   180,   177,   222,
     176,     0,   224,   323,   324,    11,    12,    19,    21,    22,
      23,    24,    25,   129,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   168,   170,   300,
     305,   306,   307,   308,   309,   310,   311,   125,   127,   129,
     131,   132,   133,   167,   168,   169,   170,   363,   401,   404,
     363,   125,   129,   131,   132,   133,   168,   169,   367,   401,
     405,    74,    13,    48,    50,    51,    52,    53,    68,    12,
      32,    33,    34,    35,    37,    38,    63,    64,    65,   352,
      40,    41,    42,    43,    45,    76,    90,    91,    92,   163,
     351,    28,    29,    30,   353,   353,   353,    65,    89,   154,
      79,   353,   346,   333,   336,    40,    93,   135,   334,   129,
     129,   335,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   329,   330,   129,   170,   220,   332,   331,   154,   363,
     363,   220,     3,     3,     4,     3,   128,     3,   300,   131,
     376,   401,   405,   367,   318,   319,    81,   360,    18,   320,
     321,   322,    93,   361,   120,   402,   403,   364,   368,   369,
     348,    50,    51,   353,   353,   353,   353,   353,   353,   353,
      44,   353,    46,    47,   353,   353,   129,   129,   129,   353,
     353,   353,   353,   129,   125,   131,   132,   133,   167,   168,
     169,   415,   220,   220,   129,   129,    93,   220,   220,   220,
     220,   220,   220,   220,   220,   220,   220,   220,   170,   221,
     410,   412,   413,   414,   415,   410,   410,   410,   129,   225,
     377,   378,   363,   367,    20,   326,   376,   376,   367,   129,
     127,   182,   183,   184,   185,   186,   188,   300,   220,   125,
     220,   365,   220,   125,   127,   131,   132,   133,   167,   168,
     169,   220,   370,   399,   406,   220,   415,   353,   353,   129,
     129,   129,   129,   129,   129,   129,   353,   129,   129,   129,
     129,   129,   129,   129,   347,   129,   129,   411,   412,   414,
     140,   227,   125,   131,   132,   133,   167,   168,   169,   379,
     399,   408,   220,   327,   367,   182,   138,   209,   139,   140,
     366,   400,   371,   372,   349,   129,   129,   129,   353,   228,
     229,   300,   119,   290,   381,   380,   382,   367,   128,   182,
     136,   127,   190,   187,   220,   220,   125,   131,   132,   133,
     167,   168,   169,   373,   399,   407,   220,   353,   129,   133,
     141,   125,   137,   230,   234,   169,   393,   399,   406,   125,
     131,   132,   133,   167,   168,   169,   383,   399,   409,   220,
     182,   191,   189,   300,   374,   375,   129,   229,   182,     1,
     102,   103,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   161,   235,   394,   395,
     384,   385,   386,   210,     1,   105,   107,   135,   193,   194,
     195,   133,   141,   363,   220,   129,   129,   133,   136,   170,
     181,   182,   185,   186,   188,   301,   304,   305,   307,   309,
     310,   236,   237,   238,   121,   122,   123,   124,   126,   129,
     130,   221,   263,   264,   302,   306,   307,   310,   263,   262,
     302,   300,   258,   240,   242,   245,   247,   250,   252,   255,
     132,   396,   399,   407,   220,   363,   169,   387,   399,   406,
     220,   290,   119,   122,   124,   126,   134,   135,   136,   138,
     139,   168,   295,   296,   303,   307,   311,   196,   300,   192,
     133,   300,   138,   129,   291,   291,   291,   291,   129,   292,
     293,   295,   125,   125,   264,   125,   125,   302,   257,   293,
     125,   125,   126,   126,   122,   122,   125,   397,   398,   388,
     389,   211,   136,   140,   125,   198,   136,   128,   195,   181,
     182,   129,   170,   226,   295,   130,   226,   295,   294,   239,
     121,   123,   131,   170,   231,   265,   268,   270,   272,   295,
     265,   121,   212,   134,   259,   226,   241,   243,   246,   248,
     251,   253,   256,   363,   220,   132,   390,   399,   407,   220,
       1,   104,   105,   107,   110,   213,   295,   297,   199,   136,
     300,   292,   265,   273,   231,   269,   272,   271,   231,   232,
     272,   133,   136,   260,   261,   301,   305,   307,   309,   310,
     129,   220,   124,   220,   124,   220,   124,   265,   391,   392,
     129,   295,   214,   262,   296,   298,   271,   197,   130,   274,
     295,   131,   275,   125,   129,   231,   266,   267,   272,   291,
     133,   125,   244,   249,   254,   363,   220,   136,   125,   136,
     216,   125,   133,   141,   270,   131,   200,   132,   133,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   276,   277,   278,   291,   268,   261,   300,   220,   220,
     220,   170,   215,   295,   217,   219,   212,   299,   272,   201,
     295,   144,   144,   168,   127,   168,   127,   127,   127,   127,
     127,   127,   132,   278,   279,   291,   271,   220,   232,   296,
     149,   150,   157,   163,   202,   203,   280,   282,   283,   284,
     285,   164,   165,   166,   288,   289,   281,   171,   286,   270,
     168,   127,   127,   127,   132,   203,   168,   287,   287,   287,
     287,     5,   300,   300,   300,   128,   289,   287,   272,   204,
     206,   205,   128,   168,   128,   128,   128,   128,   221,   221,
     221,   128,   136,   287,     5,   287,   218,   128,   128,   128,
     220
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
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
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


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
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
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
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

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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


/*----------.
| yyparse.  |
`----------*/

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
  if (yypact_value_is_default (yyn))
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

/* Line 1806 of yacc.c  */
#line 233 "surface.yy"
    { YYACCEPT; }
    break;

  case 3:

/* Line 1806 of yacc.c  */
#line 235 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			}
    break;

  case 8:

/* Line 1806 of yacc.c  */
#line 249 "surface.yy"
    { lexerFileNameMode(); }
    break;

  case 9:

/* Line 1806 of yacc.c  */
#line 251 "surface.yy"
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[(3) - (3)].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			}
    break;

  case 10:

/* Line 1806 of yacc.c  */
#line 259 "surface.yy"
    { lexerFileNameMode(); }
    break;

  case 11:

/* Line 1806 of yacc.c  */
#line 261 "surface.yy"
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[(3) - (3)].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			}
    break;

  case 12:

/* Line 1806 of yacc.c  */
#line 270 "surface.yy"
    {
			  cout << directoryManager.getCwd() << '\n';
			}
    break;

  case 13:

/* Line 1806 of yacc.c  */
#line 273 "surface.yy"
    { lexerFileNameMode(); }
    break;

  case 14:

/* Line 1806 of yacc.c  */
#line 275 "surface.yy"
    {
			  string directory;
			  directoryManager.realPath((yyvsp[(3) - (3)].yyString), directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			}
    break;

  case 15:

/* Line 1806 of yacc.c  */
#line 284 "surface.yy"
    { lexerFileNameMode(); }
    break;

  case 16:

/* Line 1806 of yacc.c  */
#line 286 "surface.yy"
    {
			  string directory;
			  directoryManager.realPath((yyvsp[(3) - (3)].yyString), directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			}
    break;

  case 17:

/* Line 1806 of yacc.c  */
#line 296 "surface.yy"
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
    break;

  case 18:

/* Line 1806 of yacc.c  */
#line 306 "surface.yy"
    { lexerStringMode(); }
    break;

  case 19:

/* Line 1806 of yacc.c  */
#line 308 "surface.yy"
    {
			  system((string("ls") + (yyvsp[(3) - (3)].yyString)).c_str());
			}
    break;

  case 20:

/* Line 1806 of yacc.c  */
#line 312 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			}
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 317 "surface.yy"
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			}
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 350 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[(1) - (2)].yyToken)));
                        }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 354 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[(1) - (2)].yyToken)));
                        }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 361 "surface.yy"
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			}
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 369 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  moduleExpressions.push(new ModuleExpression(t));
			}
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 379 "surface.yy"
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			}
    break;

  case 35:

/* Line 1806 of yacc.c  */
#line 395 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[(1) - (1)].yyToken)));
                        }
    break;

  case 36:

/* Line 1806 of yacc.c  */
#line 402 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			}
    break;

  case 37:

/* Line 1806 of yacc.c  */
#line 410 "surface.yy"
    { clear(); }
    break;

  case 38:

/* Line 1806 of yacc.c  */
#line 412 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, tokenSequence));
			}
    break;

  case 39:

/* Line 1806 of yacc.c  */
#line 419 "surface.yy"
    {}
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 422 "surface.yy"
    { store((yyvsp[(3) - (3)].yyToken)); }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 423 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 430 "surface.yy"
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			}
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 435 "surface.yy"
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			}
    break;

  case 49:

/* Line 1806 of yacc.c  */
#line 450 "surface.yy"
    {
			  currentRenaming->addSortMapping((yyvsp[(2) - (4)].yyToken), (yyvsp[(4) - (4)].yyToken));
			}
    break;

  case 50:

/* Line 1806 of yacc.c  */
#line 454 "surface.yy"
    {
			  currentRenaming->addLabelMapping((yyvsp[(2) - (4)].yyToken), (yyvsp[(4) - (4)].yyToken));
			}
    break;

  case 51:

/* Line 1806 of yacc.c  */
#line 457 "surface.yy"
    { lexBubble(BAR_COLON | BAR_TO, 1); }
    break;

  case 52:

/* Line 1806 of yacc.c  */
#line 458 "surface.yy"
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 459 "surface.yy"
    {}
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 464 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
    break;

  case 55:

/* Line 1806 of yacc.c  */
#line 465 "surface.yy"
    {}
    break;

  case 56:

/* Line 1806 of yacc.c  */
#line 466 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 472 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 473 "surface.yy"
    {}
    break;

  case 59:

/* Line 1806 of yacc.c  */
#line 474 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); }
    break;

  case 62:

/* Line 1806 of yacc.c  */
#line 481 "surface.yy"
    { currentRenaming->setPrec((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 482 "surface.yy"
    { clear(); }
    break;

  case 64:

/* Line 1806 of yacc.c  */
#line 483 "surface.yy"
    { currentRenaming->setGather(tokenSequence); }
    break;

  case 65:

/* Line 1806 of yacc.c  */
#line 484 "surface.yy"
    { clear(); }
    break;

  case 66:

/* Line 1806 of yacc.c  */
#line 485 "surface.yy"
    { currentRenaming->setFormat(tokenSequence); }
    break;

  case 67:

/* Line 1806 of yacc.c  */
#line 486 "surface.yy"
    { lexerLatexMode(); }
    break;

  case 68:

/* Line 1806 of yacc.c  */
#line 487 "surface.yy"
    { currentRenaming->setLatexMacro((yyvsp[(4) - (5)].yyString)); }
    break;

  case 69:

/* Line 1806 of yacc.c  */
#line 493 "surface.yy"
    { lexerIdMode(); }
    break;

  case 70:

/* Line 1806 of yacc.c  */
#line 495 "surface.yy"
    {
			  fileTable.beginModule((yyvsp[(1) - (5)].yyToken), (yyvsp[(3) - (5)].yyToken));
			  interpreter.setCurrentView(new View((yyvsp[(3) - (5)].yyToken)));
			  currentSyntaxContainer = CV;
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			}
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 503 "surface.yy"
    {
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			}
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 508 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[(3) - (12)].yyToken)).code(), CV);
			  CV->finishView();
			}
    break;

  case 75:

/* Line 1806 of yacc.c  */
#line 521 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
				       ": skipping " << QUOTE("->") << " in variable declaration.");
			}
    break;

  case 77:

/* Line 1806 of yacc.c  */
#line 529 "surface.yy"
    {
			  CV->addSortMapping((yyvsp[(2) - (4)].yyToken), (yyvsp[(4) - (4)].yyToken));
			}
    break;

  case 78:

/* Line 1806 of yacc.c  */
#line 532 "surface.yy"
    {}
    break;

  case 79:

/* Line 1806 of yacc.c  */
#line 533 "surface.yy"
    { lexBubble(BAR_COLON | BAR_TO, 1); }
    break;

  case 82:

/* Line 1806 of yacc.c  */
#line 538 "surface.yy"
    { (yyval.yyToken) = (yyvsp[(1) - (2)].yyToken); }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 540 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			}
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 549 "surface.yy"
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(lexerBubble);
			}
    break;

  case 85:

/* Line 1806 of yacc.c  */
#line 557 "surface.yy"
    {
			  lexBubble(END_STATEMENT, 1);
			}
    break;

  case 86:

/* Line 1806 of yacc.c  */
#line 561 "surface.yy"
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			}
    break;

  case 87:

/* Line 1806 of yacc.c  */
#line 566 "surface.yy"
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = lexerBubble;
			  lexBubble(END_STATEMENT, 1);
			}
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 576 "surface.yy"
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
    break;

  case 89:

/* Line 1806 of yacc.c  */
#line 598 "surface.yy"
    {}
    break;

  case 90:

/* Line 1806 of yacc.c  */
#line 600 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			}
    break;

  case 91:

/* Line 1806 of yacc.c  */
#line 608 "surface.yy"
    { lexBubble(BAR_RIGHT_PAREN, 1); }
    break;

  case 92:

/* Line 1806 of yacc.c  */
#line 609 "surface.yy"
    {}
    break;

  case 93:

/* Line 1806 of yacc.c  */
#line 615 "surface.yy"
    { lexerIdMode(); }
    break;

  case 94:

/* Line 1806 of yacc.c  */
#line 617 "surface.yy"
    {
			  interpreter.setCurrentModule(new SyntacticPreModule((yyvsp[(1) - (3)].yyToken), (yyvsp[(3) - (3)].yyToken)));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[(1) - (3)].yyToken), (yyvsp[(3) - (3)].yyToken));
			}
    break;

  case 95:

/* Line 1806 of yacc.c  */
#line 623 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[(8) - (8)].yyToken));
			}
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 630 "surface.yy"
    {}
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 632 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(t);
			  store(t);
			}
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 640 "surface.yy"
    {}
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 649 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter((yyvsp[(1) - (3)].yyToken), me);
			}
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 656 "surface.yy"
    {}
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 658 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			    ": saw " << QUOTE(':') << " instead of " <<
			    QUOTE("::") << " in parameter declaration.");
			}
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 666 "surface.yy"
    {
			  singleton[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[(1) - (1)].yyToken);  // needed for line number
			}
    break;

  case 107:

/* Line 1806 of yacc.c  */
#line 675 "surface.yy"
    {}
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 686 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport((yyvsp[(1) - (2)].yyToken), me);
			}
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 692 "surface.yy"
    { clear(); }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 693 "surface.yy"
    { CM->addSortDecl(tokenSequence); }
    break;

  case 115:

/* Line 1806 of yacc.c  */
#line 695 "surface.yy"
    { clear(); }
    break;

  case 116:

/* Line 1806 of yacc.c  */
#line 696 "surface.yy"
    { CM->addSubsortDecl(tokenSequence); }
    break;

  case 117:

/* Line 1806 of yacc.c  */
#line 698 "surface.yy"
    { lexBubble(BAR_COLON, 1); }
    break;

  case 118:

/* Line 1806 of yacc.c  */
#line 699 "surface.yy"
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
    break;

  case 119:

/* Line 1806 of yacc.c  */
#line 700 "surface.yy"
    {}
    break;

  case 120:

/* Line 1806 of yacc.c  */
#line 702 "surface.yy"
    {}
    break;

  case 121:

/* Line 1806 of yacc.c  */
#line 704 "surface.yy"
    {}
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 706 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON, 1); }
    break;

  case 123:

/* Line 1806 of yacc.c  */
#line 707 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); }
    break;

  case 124:

/* Line 1806 of yacc.c  */
#line 708 "surface.yy"
    { CM->addStatement(lexerBubble); }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 710 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON, 1);  }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 711 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 712 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); }
    break;

  case 128:

/* Line 1806 of yacc.c  */
#line 713 "surface.yy"
    { CM->addStatement(lexerBubble); }
    break;

  case 129:

/* Line 1806 of yacc.c  */
#line 715 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_EQUALS, 1); }
    break;

  case 130:

/* Line 1806 of yacc.c  */
#line 716 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 717 "surface.yy"
    { CM->addStatement(lexerBubble); }
    break;

  case 132:

/* Line 1806 of yacc.c  */
#line 719 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_EQUALS, 1); }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 720 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 721 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 722 "surface.yy"
    { CM->addStatement(lexerBubble); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 724 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_ARROW2, 1); }
    break;

  case 137:

/* Line 1806 of yacc.c  */
#line 725 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), END_STATEMENT, 1); }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 726 "surface.yy"
    { CM->addStatement(lexerBubble); }
    break;

  case 139:

/* Line 1806 of yacc.c  */
#line 728 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_ARROW2, 1); }
    break;

  case 140:

/* Line 1806 of yacc.c  */
#line 729 "surface.yy"
    { lexContinueBubble((yyvsp[(3) - (3)].yyToken), BAR_IF, 1); }
    break;

  case 141:

/* Line 1806 of yacc.c  */
#line 730 "surface.yy"
    { lexContinueBubble((yyvsp[(5) - (5)].yyToken), END_STATEMENT, 1); }
    break;

  case 142:

/* Line 1806 of yacc.c  */
#line 731 "surface.yy"
    { CM->addStatement(lexerBubble); }
    break;

  case 143:

/* Line 1806 of yacc.c  */
#line 733 "surface.yy"
    { lexBubble(BAR_COLON, 1); }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 734 "surface.yy"
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 735 "surface.yy"
    { CM->setFlag(SymbolType::MESSAGE); }
    break;

  case 146:

/* Line 1806 of yacc.c  */
#line 738 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 743 "surface.yy"
    {
			}
    break;

  case 148:

/* Line 1806 of yacc.c  */
#line 746 "surface.yy"
    {
			}
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 749 "surface.yy"
    { clear(); }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 750 "surface.yy"
    { CM->addSubsortDecl(tokenSequence); }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 753 "surface.yy"
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			}
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 763 "surface.yy"
    {}
    break;

  case 153:

/* Line 1806 of yacc.c  */
#line 764 "surface.yy"
    {}
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 772 "surface.yy"
    {
			}
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 776 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 777 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 784 "surface.yy"
    { singleton[0] = (yyvsp[(1) - (1)].yyToken); CM->addOpDecl(singleton); }
    break;

  case 162:

/* Line 1806 of yacc.c  */
#line 785 "surface.yy"
    { CM->addOpDecl(lexerBubble); }
    break;

  case 165:

/* Line 1806 of yacc.c  */
#line 791 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			}
    break;

  case 166:

/* Line 1806 of yacc.c  */
#line 798 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
				       ": skipping stray " << QUOTE(":") << " in operator declaration.");

			}
    break;

  case 169:

/* Line 1806 of yacc.c  */
#line 808 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
    break;

  case 170:

/* Line 1806 of yacc.c  */
#line 813 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[(1) - (1)].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			}
    break;

  case 171:

/* Line 1806 of yacc.c  */
#line 820 "surface.yy"
    {
			  if ((yyvsp[(1) - (2)].yyBool))
			    CM->convertSortsToKinds();
			}
    break;

  case 173:

/* Line 1806 of yacc.c  */
#line 827 "surface.yy"
    {}
    break;

  case 174:

/* Line 1806 of yacc.c  */
#line 830 "surface.yy"
    { (yyval.yyBool) = false; }
    break;

  case 175:

/* Line 1806 of yacc.c  */
#line 831 "surface.yy"
    { (yyval.yyBool) = true; }
    break;

  case 178:

/* Line 1806 of yacc.c  */
#line 839 "surface.yy"
    {
			  singleton[0] = (yyvsp[(1) - (1)].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			}
    break;

  case 179:

/* Line 1806 of yacc.c  */
#line 843 "surface.yy"
    { clear(); }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 845 "surface.yy"
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			}
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 850 "surface.yy"
    { store((yyvsp[(3) - (3)].yyToken)); }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 851 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 854 "surface.yy"
    {}
    break;

  case 187:

/* Line 1806 of yacc.c  */
#line 863 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			}
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 867 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			}
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 871 "surface.yy"
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			}
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 877 "surface.yy"
    {
			  CM->setFlag(SymbolType::ASSOC);
			}
    break;

  case 191:

/* Line 1806 of yacc.c  */
#line 881 "surface.yy"
    {
			  CM->setFlag(SymbolType::COMM);
			}
    break;

  case 192:

/* Line 1806 of yacc.c  */
#line 884 "surface.yy"
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); }
    break;

  case 193:

/* Line 1806 of yacc.c  */
#line 885 "surface.yy"
    { CM->setIdentity(lexerBubble); }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 887 "surface.yy"
    {
			  CM->setFlag(SymbolType::IDEM);
			}
    break;

  case 195:

/* Line 1806 of yacc.c  */
#line 891 "surface.yy"
    {
			  CM->setFlag(SymbolType::ITER);
			}
    break;

  case 196:

/* Line 1806 of yacc.c  */
#line 894 "surface.yy"
    { CM->setPrec((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 197:

/* Line 1806 of yacc.c  */
#line 895 "surface.yy"
    { clear(); }
    break;

  case 198:

/* Line 1806 of yacc.c  */
#line 896 "surface.yy"
    { CM->setGather(tokenSequence); }
    break;

  case 199:

/* Line 1806 of yacc.c  */
#line 897 "surface.yy"
    { clear(); }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 898 "surface.yy"
    { CM->setFormat(tokenSequence); }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 899 "surface.yy"
    { clear(); }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 900 "surface.yy"
    { CM->setStrat(tokenSequence); }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 901 "surface.yy"
    { clear(); }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 902 "surface.yy"
    { CM->setPoly(tokenSequence); }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 904 "surface.yy"
    {
			  CM->setFlag(SymbolType::MEMO);
			}
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 908 "surface.yy"
    {
			  CM->setFlag(SymbolType::CTOR);
			}
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 912 "surface.yy"
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			}
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 916 "surface.yy"
    { clear(); }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 917 "surface.yy"
    { CM->setFrozen(tokenSequence); }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 919 "surface.yy"
    {
			  CM->setFlag(SymbolType::CONFIG);
			}
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 923 "surface.yy"
    {
			  CM->setFlag(SymbolType::OBJECT);
			}
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 927 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			}
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 931 "surface.yy"
    {
			  CM->setMetadata((yyvsp[(2) - (2)].yyToken));
			}
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 934 "surface.yy"
    { lexerLatexMode(); }
    break;

  case 215:

/* Line 1806 of yacc.c  */
#line 935 "surface.yy"
    { CM->setLatexMacro((yyvsp[(4) - (5)].yyString)); }
    break;

  case 216:

/* Line 1806 of yacc.c  */
#line 936 "surface.yy"
    {}
    break;

  case 217:

/* Line 1806 of yacc.c  */
#line 938 "surface.yy"
    {
			  CM->setFlag(SymbolType::DITTO);
			}
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 951 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 952 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 959 "surface.yy"
    { clear(); CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[(2) - (2)].yyToken), tokenSequence); }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 960 "surface.yy"
    { CM->addHook(SyntacticPreModule::ID_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 961 "surface.yy"
    { CM->addHook(SyntacticPreModule::OP_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 962 "surface.yy"
    { CM->addHook(SyntacticPreModule::TERM_HOOK, (yyvsp[(2) - (3)].yyToken), lexerBubble); }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 968 "surface.yy"
    {}
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 970 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			}
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 976 "surface.yy"
    {}
    break;

  case 231:

/* Line 1806 of yacc.c  */
#line 978 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			}
    break;

  case 232:

/* Line 1806 of yacc.c  */
#line 986 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 990 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 991 "surface.yy"
    { store((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 236:

/* Line 1806 of yacc.c  */
#line 992 "surface.yy"
    { store((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 993 "surface.yy"
    { store((yyvsp[(4) - (4)].yyToken)); }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 1000 "surface.yy"
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  (yyval.yyToken) = t;
			}
    break;

  case 239:

/* Line 1806 of yacc.c  */
#line 1011 "surface.yy"
    { fragStore((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 1012 "surface.yy"
    { fragStore((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 241:

/* Line 1806 of yacc.c  */
#line 1013 "surface.yy"
    { fragStore((yyvsp[(5) - (5)].yyToken)); }
    break;

  case 242:

/* Line 1806 of yacc.c  */
#line 1016 "surface.yy"
    { fragStore((yyvsp[(2) - (2)].yyToken)); }
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 1017 "surface.yy"
    {}
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 1018 "surface.yy"
    {}
    break;

  case 347:

/* Line 1806 of yacc.c  */
#line 1098 "surface.yy"
    { lexBubble(END_COMMAND, 1); }
    break;

  case 348:

/* Line 1806 of yacc.c  */
#line 1100 "surface.yy"
    {
			  interpreter.setCurrentModule(lexerBubble);
			}
    break;

  case 349:

/* Line 1806 of yacc.c  */
#line 1103 "surface.yy"
    { lexBubble(END_COMMAND, 1); }
    break;

  case 350:

/* Line 1806 of yacc.c  */
#line 1105 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			}
    break;

  case 351:

/* Line 1806 of yacc.c  */
#line 1110 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
    break;

  case 352:

/* Line 1806 of yacc.c  */
#line 1115 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			}
    break;

  case 353:

/* Line 1806 of yacc.c  */
#line 1122 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
    break;

  case 354:

/* Line 1806 of yacc.c  */
#line 1127 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			}
    break;

  case 355:

/* Line 1806 of yacc.c  */
#line 1134 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
    break;

  case 356:

/* Line 1806 of yacc.c  */
#line 1139 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sreduce(lexerBubble);
			}
    break;

  case 357:

/* Line 1806 of yacc.c  */
#line 1146 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
    break;

  case 358:

/* Line 1806 of yacc.c  */
#line 1151 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[(1) - (4)].yyBool));
			}
    break;

  case 359:

/* Line 1806 of yacc.c  */
#line 1158 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
    break;

  case 360:

/* Line 1806 of yacc.c  */
#line 1164 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[(1) - (4)].yyBool));
			}
    break;

  case 361:

/* Line 1806 of yacc.c  */
#line 1170 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
    break;

  case 362:

/* Line 1806 of yacc.c  */
#line 1177 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[(1) - (4)].yyBool));
			}
    break;

  case 363:

/* Line 1806 of yacc.c  */
#line 1183 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
    break;

  case 364:

/* Line 1806 of yacc.c  */
#line 1190 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[(1) - (4)].yyBool));
			}
    break;

  case 365:

/* Line 1806 of yacc.c  */
#line 1196 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
    break;

  case 366:

/* Line 1806 of yacc.c  */
#line 1202 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[(1) - (4)].yyBool));
			}
    break;

  case 367:

/* Line 1806 of yacc.c  */
#line 1208 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			}
    break;

  case 368:

/* Line 1806 of yacc.c  */
#line 1215 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[(1) - (3)].yySearchKind));
			}
    break;

  case 369:

/* Line 1806 of yacc.c  */
#line 1221 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
    break;

  case 370:

/* Line 1806 of yacc.c  */
#line 1227 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[(1) - (3)].yyBool), number);
			}
    break;

  case 371:

/* Line 1806 of yacc.c  */
#line 1233 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
    break;

  case 372:

/* Line 1806 of yacc.c  */
#line 1239 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number);
			}
    break;

  case 373:

/* Line 1806 of yacc.c  */
#line 1245 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
    break;

  case 374:

/* Line 1806 of yacc.c  */
#line 1251 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.variantUnify(lexerBubble, number, (yyvsp[(1) - (5)].yyBool));
			}
    break;

  case 375:

/* Line 1806 of yacc.c  */
#line 1258 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			}
    break;

  case 376:

/* Line 1806 of yacc.c  */
#line 1264 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.getVariants(lexerBubble, number, (yyvsp[(3) - (6)].yyBool), (yyvsp[(1) - (6)].yyBool));
			}
    break;

  case 377:

/* Line 1806 of yacc.c  */
#line 1270 "surface.yy"
    {
			  interpreter.cont((yyvsp[(3) - (4)].yyInt64), (yyvsp[(1) - (4)].yyBool));
			}
    break;

  case 378:

/* Line 1806 of yacc.c  */
#line 1274 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			}
    break;

  case 379:

/* Line 1806 of yacc.c  */
#line 1279 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			}
    break;

  case 380:

/* Line 1806 of yacc.c  */
#line 1285 "surface.yy"
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			}
    break;

  case 381:

/* Line 1806 of yacc.c  */
#line 1290 "surface.yy"
    { lexerCmdMode(); }
    break;

  case 382:

/* Line 1806 of yacc.c  */
#line 1292 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[(2) - (4)].yyBool));
			}
    break;

  case 383:

/* Line 1806 of yacc.c  */
#line 1296 "surface.yy"
    { lexerCmdMode(); }
    break;

  case 384:

/* Line 1806 of yacc.c  */
#line 1298 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[(2) - (4)].yyBool));
			}
    break;

  case 385:

/* Line 1806 of yacc.c  */
#line 1302 "surface.yy"
    { lexerCmdMode(); }
    break;

  case 386:

/* Line 1806 of yacc.c  */
#line 1304 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[(2) - (4)].yyBool));
			}
    break;

  case 387:

/* Line 1806 of yacc.c  */
#line 1308 "surface.yy"
    { lexerCmdMode(); }
    break;

  case 388:

/* Line 1806 of yacc.c  */
#line 1310 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[(2) - (4)].yyBool));
			}
    break;

  case 389:

/* Line 1806 of yacc.c  */
#line 1315 "surface.yy"
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			}
    break;

  case 390:

/* Line 1806 of yacc.c  */
#line 1322 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 391:

/* Line 1806 of yacc.c  */
#line 1324 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
    break;

  case 392:

/* Line 1806 of yacc.c  */
#line 1328 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 393:

/* Line 1806 of yacc.c  */
#line 1330 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			}
    break;

  case 394:

/* Line 1806 of yacc.c  */
#line 1334 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 395:

/* Line 1806 of yacc.c  */
#line 1336 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			}
    break;

  case 396:

/* Line 1806 of yacc.c  */
#line 1340 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 397:

/* Line 1806 of yacc.c  */
#line 1342 "surface.yy"
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			}
    break;

  case 398:

/* Line 1806 of yacc.c  */
#line 1347 "surface.yy"
    {
			  interpreter.showModules(true);
			}
    break;

  case 399:

/* Line 1806 of yacc.c  */
#line 1351 "surface.yy"
    {
			  interpreter.showNamedViews();
			}
    break;

  case 400:

/* Line 1806 of yacc.c  */
#line 1354 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 401:

/* Line 1806 of yacc.c  */
#line 1356 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			}
    break;

  case 402:

/* Line 1806 of yacc.c  */
#line 1360 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 403:

/* Line 1806 of yacc.c  */
#line 1362 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			}
    break;

  case 404:

/* Line 1806 of yacc.c  */
#line 1366 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 405:

/* Line 1806 of yacc.c  */
#line 1368 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			}
    break;

  case 406:

/* Line 1806 of yacc.c  */
#line 1372 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 407:

/* Line 1806 of yacc.c  */
#line 1374 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			}
    break;

  case 408:

/* Line 1806 of yacc.c  */
#line 1378 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 409:

/* Line 1806 of yacc.c  */
#line 1380 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			}
    break;

  case 410:

/* Line 1806 of yacc.c  */
#line 1384 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 411:

/* Line 1806 of yacc.c  */
#line 1386 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			}
    break;

  case 412:

/* Line 1806 of yacc.c  */
#line 1390 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 413:

/* Line 1806 of yacc.c  */
#line 1392 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			}
    break;

  case 414:

/* Line 1806 of yacc.c  */
#line 1396 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 415:

/* Line 1806 of yacc.c  */
#line 1398 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			}
    break;

  case 416:

/* Line 1806 of yacc.c  */
#line 1403 "surface.yy"
    {
			  interpreter.showSearchPath((yyvsp[(3) - (4)].yyInt64));
			}
    break;

  case 417:

/* Line 1806 of yacc.c  */
#line 1407 "surface.yy"
    {
			  interpreter.showSearchPathLabels((yyvsp[(4) - (5)].yyInt64));
			}
    break;

  case 418:

/* Line 1806 of yacc.c  */
#line 1411 "surface.yy"
    {
			  interpreter.showSearchGraph();
			}
    break;

  case 419:

/* Line 1806 of yacc.c  */
#line 1414 "surface.yy"
    { lexBubble(END_COMMAND, 0); }
    break;

  case 420:

/* Line 1806 of yacc.c  */
#line 1416 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			}
    break;

  case 421:

/* Line 1806 of yacc.c  */
#line 1424 "surface.yy"
    {
			  globalAdvisoryFlag = alwaysAdviseFlag ? true : (yyvsp[(4) - (5)].yyBool);
			}
    break;

  case 422:

/* Line 1806 of yacc.c  */
#line 1428 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 423:

/* Line 1806 of yacc.c  */
#line 1432 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[(5) - (6)].yyBool));
			}
    break;

  case 424:

/* Line 1806 of yacc.c  */
#line 1436 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 425:

/* Line 1806 of yacc.c  */
#line 1440 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 426:

/* Line 1806 of yacc.c  */
#line 1444 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[(5) - (6)].yyBool));
			}
    break;

  case 427:

/* Line 1806 of yacc.c  */
#line 1448 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 428:

/* Line 1806 of yacc.c  */
#line 1452 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 429:

/* Line 1806 of yacc.c  */
#line 1456 "surface.yy"
    {
			  interpreter.setPrintFlag((yyvsp[(3) - (5)].yyPrintFlags), (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 430:

/* Line 1806 of yacc.c  */
#line 1460 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 431:

/* Line 1806 of yacc.c  */
#line 1464 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[(5) - (6)].yyBool));
			}
    break;

  case 432:

/* Line 1806 of yacc.c  */
#line 1468 "surface.yy"
    {
			  interpreter.setFlag((yyvsp[(3) - (5)].yyFlags), (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 433:

/* Line 1806 of yacc.c  */
#line 1472 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[(3) - (4)].yyBool));
			}
    break;

  case 434:

/* Line 1806 of yacc.c  */
#line 1475 "surface.yy"
    { lexerCmdMode(); }
    break;

  case 435:

/* Line 1806 of yacc.c  */
#line 1476 "surface.yy"
    { lexerInitialMode(); }
    break;

  case 436:

/* Line 1806 of yacc.c  */
#line 1478 "surface.yy"
    {
			  interpreter.setAutoImport((yyvsp[(2) - (7)].yyImportMode), (yyvsp[(4) - (7)].yyToken), (yyvsp[(6) - (7)].yyBool));
			}
    break;

  case 437:

/* Line 1806 of yacc.c  */
#line 1481 "surface.yy"
    { lexerCmdMode(); }
    break;

  case 438:

/* Line 1806 of yacc.c  */
#line 1482 "surface.yy"
    { lexerInitialMode(); }
    break;

  case 439:

/* Line 1806 of yacc.c  */
#line 1484 "surface.yy"
    {
			  interpreter.setOmodInclude((yyvsp[(5) - (8)].yyToken), (yyvsp[(7) - (8)].yyBool));
			}
    break;

  case 440:

/* Line 1806 of yacc.c  */
#line 1488 "surface.yy"
    {
			  globalVerboseFlag = (yyvsp[(3) - (4)].yyBool);
			}
    break;

  case 441:

/* Line 1806 of yacc.c  */
#line 1492 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 442:

/* Line 1806 of yacc.c  */
#line 1496 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 443:

/* Line 1806 of yacc.c  */
#line 1500 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 444:

/* Line 1806 of yacc.c  */
#line 1504 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[(3) - (4)].yyBool));
			}
    break;

  case 445:

/* Line 1806 of yacc.c  */
#line 1508 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 446:

/* Line 1806 of yacc.c  */
#line 1515 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			}
    break;

  case 447:

/* Line 1806 of yacc.c  */
#line 1519 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			}
    break;

  case 448:

/* Line 1806 of yacc.c  */
#line 1523 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			}
    break;

  case 449:

/* Line 1806 of yacc.c  */
#line 1527 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			}
    break;

  case 450:

/* Line 1806 of yacc.c  */
#line 1534 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[(4) - (5)].yyBool));
			}
    break;

  case 451:

/* Line 1806 of yacc.c  */
#line 1538 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[(3) - (4)].yyBool));
			}
    break;

  case 452:

/* Line 1806 of yacc.c  */
#line 1544 "surface.yy"
    { lexerInitialMode(); }
    break;

  case 454:

/* Line 1806 of yacc.c  */
#line 1551 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; }
    break;

  case 455:

/* Line 1806 of yacc.c  */
#line 1552 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; }
    break;

  case 456:

/* Line 1806 of yacc.c  */
#line 1553 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; }
    break;

  case 457:

/* Line 1806 of yacc.c  */
#line 1554 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; }
    break;

  case 458:

/* Line 1806 of yacc.c  */
#line 1555 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; }
    break;

  case 459:

/* Line 1806 of yacc.c  */
#line 1556 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; }
    break;

  case 460:

/* Line 1806 of yacc.c  */
#line 1557 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; }
    break;

  case 461:

/* Line 1806 of yacc.c  */
#line 1558 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; }
    break;

  case 462:

/* Line 1806 of yacc.c  */
#line 1559 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; }
    break;

  case 463:

/* Line 1806 of yacc.c  */
#line 1560 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; }
    break;

  case 464:

/* Line 1806 of yacc.c  */
#line 1563 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE; }
    break;

  case 465:

/* Line 1806 of yacc.c  */
#line 1564 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; }
    break;

  case 466:

/* Line 1806 of yacc.c  */
#line 1565 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; }
    break;

  case 467:

/* Line 1806 of yacc.c  */
#line 1566 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; }
    break;

  case 468:

/* Line 1806 of yacc.c  */
#line 1567 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; }
    break;

  case 469:

/* Line 1806 of yacc.c  */
#line 1568 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_MB; }
    break;

  case 470:

/* Line 1806 of yacc.c  */
#line 1569 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; }
    break;

  case 471:

/* Line 1806 of yacc.c  */
#line 1570 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_RL; }
    break;

  case 472:

/* Line 1806 of yacc.c  */
#line 1571 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; }
    break;

  case 473:

/* Line 1806 of yacc.c  */
#line 1572 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; }
    break;

  case 474:

/* Line 1806 of yacc.c  */
#line 1573 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; }
    break;

  case 475:

/* Line 1806 of yacc.c  */
#line 1576 "surface.yy"
    { (yyval.yyBool) = true; }
    break;

  case 476:

/* Line 1806 of yacc.c  */
#line 1577 "surface.yy"
    { (yyval.yyBool) = false; }
    break;

  case 477:

/* Line 1806 of yacc.c  */
#line 1580 "surface.yy"
    { (yyval.yyBool) = true; }
    break;

  case 478:

/* Line 1806 of yacc.c  */
#line 1581 "surface.yy"
    { (yyval.yyBool) = false; }
    break;

  case 479:

/* Line 1806 of yacc.c  */
#line 1584 "surface.yy"
    { (yyval.yyBool) = true; }
    break;

  case 480:

/* Line 1806 of yacc.c  */
#line 1585 "surface.yy"
    { (yyval.yyBool) = false; }
    break;

  case 481:

/* Line 1806 of yacc.c  */
#line 1588 "surface.yy"
    { (yyval.yyBool) = true; }
    break;

  case 482:

/* Line 1806 of yacc.c  */
#line 1589 "surface.yy"
    { (yyval.yyBool) = false; }
    break;

  case 483:

/* Line 1806 of yacc.c  */
#line 1594 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::NARROW; }
    break;

  case 484:

/* Line 1806 of yacc.c  */
#line 1595 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; }
    break;

  case 485:

/* Line 1806 of yacc.c  */
#line 1596 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::SEARCH; }
    break;

  case 486:

/* Line 1806 of yacc.c  */
#line 1599 "surface.yy"
    { (yyval.yyBool) = true; }
    break;

  case 487:

/* Line 1806 of yacc.c  */
#line 1600 "surface.yy"
    { (yyval.yyBool) = false; }
    break;

  case 488:

/* Line 1806 of yacc.c  */
#line 1603 "surface.yy"
    { (yyval.yyBool) = true; }
    break;

  case 489:

/* Line 1806 of yacc.c  */
#line 1604 "surface.yy"
    { (yyval.yyBool) = false; }
    break;

  case 490:

/* Line 1806 of yacc.c  */
#line 1607 "surface.yy"
    { (yyval.yyBool) = true; }
    break;

  case 491:

/* Line 1806 of yacc.c  */
#line 1608 "surface.yy"
    { (yyval.yyBool) = false; }
    break;

  case 492:

/* Line 1806 of yacc.c  */
#line 1611 "surface.yy"
    { (yyval.yyInt64) = (yyvsp[(1) - (1)].yyInt64); }
    break;

  case 493:

/* Line 1806 of yacc.c  */
#line 1612 "surface.yy"
    { (yyval.yyInt64) = NONE; }
    break;

  case 494:

/* Line 1806 of yacc.c  */
#line 1615 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::PROTECTING; }
    break;

  case 495:

/* Line 1806 of yacc.c  */
#line 1616 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::EXTENDING; }
    break;

  case 496:

/* Line 1806 of yacc.c  */
#line 1617 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::INCLUDING; }
    break;

  case 498:

/* Line 1806 of yacc.c  */
#line 1627 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 500:

/* Line 1806 of yacc.c  */
#line 1631 "surface.yy"
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); }
    break;

  case 503:

/* Line 1806 of yacc.c  */
#line 1643 "surface.yy"
    { lexSave((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 506:

/* Line 1806 of yacc.c  */
#line 1646 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0);  }
    break;

  case 508:

/* Line 1806 of yacc.c  */
#line 1651 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 510:

/* Line 1806 of yacc.c  */
#line 1653 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 513:

/* Line 1806 of yacc.c  */
#line 1659 "surface.yy"
    { number = Token::codeToInt64(lexerBubble[1].code()); }
    break;

  case 515:

/* Line 1806 of yacc.c  */
#line 1661 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 518:

/* Line 1806 of yacc.c  */
#line 1676 "surface.yy"
    { lexSave((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 521:

/* Line 1806 of yacc.c  */
#line 1679 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 523:

/* Line 1806 of yacc.c  */
#line 1688 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 525:

/* Line 1806 of yacc.c  */
#line 1690 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 527:

/* Line 1806 of yacc.c  */
#line 1692 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 530:

/* Line 1806 of yacc.c  */
#line 1702 "surface.yy"
    { number = Token::codeToInt64(lexerBubble[1].code()); }
    break;

  case 532:

/* Line 1806 of yacc.c  */
#line 1704 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 534:

/* Line 1806 of yacc.c  */
#line 1706 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 537:

/* Line 1806 of yacc.c  */
#line 1716 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 539:

/* Line 1806 of yacc.c  */
#line 1718 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 542:

/* Line 1806 of yacc.c  */
#line 1729 "surface.yy"
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			}
    break;

  case 544:

/* Line 1806 of yacc.c  */
#line 1735 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 547:

/* Line 1806 of yacc.c  */
#line 1745 "surface.yy"
    { lexContinueSave((yyvsp[(1) - (1)].yyToken)); }
    break;

  case 549:

/* Line 1806 of yacc.c  */
#line 1747 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 552:

/* Line 1806 of yacc.c  */
#line 1757 "surface.yy"
    { number2 = Token::codeToInt64(lexerBubble[2].code()); }
    break;

  case 554:

/* Line 1806 of yacc.c  */
#line 1759 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0); }
    break;

  case 557:

/* Line 1806 of yacc.c  */
#line 1764 "surface.yy"
    { lexContinueBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 0, 1); }
    break;

  case 560:

/* Line 1806 of yacc.c  */
#line 1770 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), END_COMMAND, 1, 1); }
    break;

  case 562:

/* Line 1806 of yacc.c  */
#line 1772 "surface.yy"
    { lexBubble((yyvsp[(1) - (1)].yyToken), BAR_COLON | END_COMMAND, 0); }
    break;

  case 564:

/* Line 1806 of yacc.c  */
#line 1775 "surface.yy"
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(lexerBubble[0]);
			}
    break;

  case 602:

/* Line 1806 of yacc.c  */
#line 1814 "surface.yy"
    {}
    break;

  case 604:

/* Line 1806 of yacc.c  */
#line 1819 "surface.yy"
    {
			  singleton[0].dropChar((yyvsp[(1) - (1)].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			}
    break;

  case 607:

/* Line 1806 of yacc.c  */
#line 1830 "surface.yy"
    {
			  singleton[0] = (yyvsp[(1) - (1)].yyToken);
			  interpreter.addSelected(singleton);
			}
    break;

  case 608:

/* Line 1806 of yacc.c  */
#line 1835 "surface.yy"
    {
			  interpreter.addSelected(lexerBubble);
			}
    break;



/* Line 1806 of yacc.c  */
#line 6020 "surface.c"
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
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
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



/* Line 2067 of yacc.c  */
#line 1865 "surface.yy"


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

