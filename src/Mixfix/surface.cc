/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0



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
     KW_MATCH = 270,
     KW_XMATCH = 271,
     KW_UNIFY = 272,
     KW_XUNIFY = 273,
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
     KW_WITH = 295,
     KW_PARENS = 296,
     KW_ALIASES = 297,
     KW_GC = 298,
     KW_TIME = 299,
     KW_STATS = 300,
     KW_TIMING = 301,
     KW_CMD = 302,
     KW_BREAKDOWN = 303,
     KW_BREAK = 304,
     KW_PATH = 305,
     KW_MODULE = 306,
     KW_MODULES = 307,
     KW_VIEWS = 308,
     KW_ALL = 309,
     KW_SORTS = 310,
     KW_OPS = 311,
     KW_VARS = 312,
     KW_MBS = 313,
     KW_EQS = 314,
     KW_RLS = 315,
     KW_SUMMARY = 316,
     KW_KINDS = 317,
     KW_ADVISE = 318,
     KW_VERBOSE = 319,
     KW_DO = 320,
     KW_CLEAR = 321,
     KW_PROTECT = 322,
     KW_EXTEND = 323,
     KW_INCLUDE = 324,
     KW_EXCLUDE = 325,
     KW_CONCEAL = 326,
     KW_REVEAL = 327,
     KW_COMPILE = 328,
     KW_COUNT = 329,
     KW_DEBUG = 330,
     KW_RESUME = 331,
     KW_ABORT = 332,
     KW_STEP = 333,
     KW_WHERE = 334,
     KW_CREDUCE = 335,
     KW_DUMP = 336,
     KW_PROFILE = 337,
     KW_NUMBER = 338,
     KW_RAT = 339,
     KW_COLOR = 340,
     SIMPLE_NUMBER = 341,
     KW_PWD = 342,
     KW_CD = 343,
     KW_PUSHD = 344,
     KW_POPD = 345,
     KW_LS = 346,
     KW_LOAD = 347,
     KW_QUIT = 348,
     KW_EOF = 349,
     KW_ENDM = 350,
     KW_IMPORT = 351,
     KW_ENDV = 352,
     KW_SORT = 353,
     KW_SUBSORT = 354,
     KW_OP = 355,
     KW_MSGS = 356,
     KW_VAR = 357,
     KW_CLASS = 358,
     KW_SUBCLASS = 359,
     KW_MB = 360,
     KW_CMB = 361,
     KW_EQ = 362,
     KW_CEQ = 363,
     KW_RL = 364,
     KW_CRL = 365,
     KW_IS = 366,
     KW_FROM = 367,
     KW_ARROW = 368,
     KW_ARROW2 = 369,
     KW_PARTIAL = 370,
     KW_IF = 371,
     KW_LABEL = 372,
     KW_TO = 373,
     KW_COLON2 = 374,
     KW_ASSOC = 375,
     KW_COMM = 376,
     KW_ID = 377,
     KW_IDEM = 378,
     KW_ITER = 379,
     KW_LEFT = 380,
     KW_RIGHT = 381,
     KW_PREC = 382,
     KW_GATHER = 383,
     KW_METADATA = 384,
     KW_STRAT = 385,
     KW_POLY = 386,
     KW_MEMO = 387,
     KW_FROZEN = 388,
     KW_CTOR = 389,
     KW_LATEX = 390,
     KW_SPECIAL = 391,
     KW_CONFIG = 392,
     KW_OBJ = 393,
     KW_MSG = 394,
     KW_DITTO = 395,
     KW_FORMAT = 396,
     KW_ID_HOOK = 397,
     KW_OP_HOOK = 398,
     KW_TERM_HOOK = 399,
     KW_IN = 400,
     IDENTIFIER = 401,
     NUMERIC_ID = 402,
     ENDS_IN_DOT = 403
   };
#endif
/* Tokens.  */
#define FILE_NAME_STRING 258
#define UNINTERPRETED_STRING 259
#define LATEX_STRING 260
#define KW_MOD 261
#define KW_OMOD 262
#define KW_VIEW 263
#define KW_PARSE 264
#define KW_NORMALIZE 265
#define KW_REDUCE 266
#define KW_REWRITE 267
#define KW_LOOP 268
#define KW_NARROW 269
#define KW_MATCH 270
#define KW_XMATCH 271
#define KW_UNIFY 272
#define KW_XUNIFY 273
#define KW_EREWRITE 274
#define KW_FREWRITE 275
#define KW_SREWRITE 276
#define KW_CONTINUE 277
#define KW_SEARCH 278
#define KW_SET 279
#define KW_SHOW 280
#define KW_ON 281
#define KW_OFF 282
#define KW_TRACE 283
#define KW_BODY 284
#define KW_BUILTIN 285
#define KW_WHOLE 286
#define KW_SELECT 287
#define KW_DESELECT 288
#define KW_CONDITION 289
#define KW_SUBSTITUTION 290
#define KW_PRINT 291
#define KW_GRAPH 292
#define KW_MIXFIX 293
#define KW_FLAT 294
#define KW_WITH 295
#define KW_PARENS 296
#define KW_ALIASES 297
#define KW_GC 298
#define KW_TIME 299
#define KW_STATS 300
#define KW_TIMING 301
#define KW_CMD 302
#define KW_BREAKDOWN 303
#define KW_BREAK 304
#define KW_PATH 305
#define KW_MODULE 306
#define KW_MODULES 307
#define KW_VIEWS 308
#define KW_ALL 309
#define KW_SORTS 310
#define KW_OPS 311
#define KW_VARS 312
#define KW_MBS 313
#define KW_EQS 314
#define KW_RLS 315
#define KW_SUMMARY 316
#define KW_KINDS 317
#define KW_ADVISE 318
#define KW_VERBOSE 319
#define KW_DO 320
#define KW_CLEAR 321
#define KW_PROTECT 322
#define KW_EXTEND 323
#define KW_INCLUDE 324
#define KW_EXCLUDE 325
#define KW_CONCEAL 326
#define KW_REVEAL 327
#define KW_COMPILE 328
#define KW_COUNT 329
#define KW_DEBUG 330
#define KW_RESUME 331
#define KW_ABORT 332
#define KW_STEP 333
#define KW_WHERE 334
#define KW_CREDUCE 335
#define KW_DUMP 336
#define KW_PROFILE 337
#define KW_NUMBER 338
#define KW_RAT 339
#define KW_COLOR 340
#define SIMPLE_NUMBER 341
#define KW_PWD 342
#define KW_CD 343
#define KW_PUSHD 344
#define KW_POPD 345
#define KW_LS 346
#define KW_LOAD 347
#define KW_QUIT 348
#define KW_EOF 349
#define KW_ENDM 350
#define KW_IMPORT 351
#define KW_ENDV 352
#define KW_SORT 353
#define KW_SUBSORT 354
#define KW_OP 355
#define KW_MSGS 356
#define KW_VAR 357
#define KW_CLASS 358
#define KW_SUBCLASS 359
#define KW_MB 360
#define KW_CMB 361
#define KW_EQ 362
#define KW_CEQ 363
#define KW_RL 364
#define KW_CRL 365
#define KW_IS 366
#define KW_FROM 367
#define KW_ARROW 368
#define KW_ARROW2 369
#define KW_PARTIAL 370
#define KW_IF 371
#define KW_LABEL 372
#define KW_TO 373
#define KW_COLON2 374
#define KW_ASSOC 375
#define KW_COMM 376
#define KW_ID 377
#define KW_IDEM 378
#define KW_ITER 379
#define KW_LEFT 380
#define KW_RIGHT 381
#define KW_PREC 382
#define KW_GATHER 383
#define KW_METADATA 384
#define KW_STRAT 385
#define KW_POLY 386
#define KW_MEMO 387
#define KW_FROZEN 388
#define KW_CTOR 389
#define KW_LATEX 390
#define KW_SPECIAL 391
#define KW_CONFIG 392
#define KW_OBJ 393
#define KW_MSG 394
#define KW_DITTO 395
#define KW_FORMAT 396
#define KW_ID_HOOK 397
#define KW_OP_HOOK 398
#define KW_TERM_HOOK 399
#define KW_IN 400
#define IDENTIFIER 401
#define NUMERIC_ID 402
#define ENDS_IN_DOT 403




/* Copy the first part of user declarations.  */
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
#include "preModule.hh"
#include "visibleModule.hh"  // HACK
#include "userLevelRewritingContext.hh"
#include "interpreter.hh"

#include "global.hh"
#define clear()			bubble.contractTo(0);
#define store(token)		bubble.append(token)
#define fragClear()		fragments.contractTo(0);
#define fragStore(token)	fragments.append(token)
#define YYPARSE_PARAM	parseResult
#define PARSE_RESULT	(*((UserLevelRewritingContext::ParseResult*) parseResult))

#define CM		interpreter.getCurrentModule()
#define CV		interpreter.getCurrentView()

void lexerInitialMode();
void lexerIdMode();
void lexerCmdMode();
void lexerFileNameMode();
void lexerStringMode();
void lexerLatexMode();
bool handleEof();
bool includeFile(const string& directory, const string& fileName, bool silent, int lineNr);
void eatComment(bool firstNonWhite);

Vector<Token> bubble;
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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 98 "surface.yy"
typedef union YYSTYPE {
  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 460 "surface.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 108 "surface.yy"

int yylex(YYSTYPE* lvalp);


/* Line 219 of yacc.c.  */
#line 475 "surface.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

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

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
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
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  107
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   7105

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  163
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  281
/* YYNRULES -- Number of rules. */
#define YYNRULES  743
/* YYNRULES -- Number of states. */
#define YYNSTATES  1142

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   403

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     119,   120,   131,   130,   125,     2,   121,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   117,     2,
     122,   118,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   123,     2,   124,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   132,   126,   133,     2,     2,     2,     2,
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
     115,   116,   127,   128,   129,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    10,    12,    14,    15,
      19,    20,    24,    26,    27,    31,    32,    36,    38,    39,
      43,    45,    47,    50,    53,    56,    59,    62,    66,    68,
      70,    74,    76,    78,    80,    82,    84,    88,    89,    95,
      99,   103,   105,   106,   107,   113,   115,   117,   121,   123,
     128,   133,   140,   141,   145,   146,   151,   156,   157,   158,
     162,   163,   168,   172,   173,   176,   178,   181,   182,   188,
     189,   195,   196,   202,   203,   204,   205,   218,   221,   222,
     227,   232,   233,   238,   241,   244,   246,   247,   248,   257,
     258,   262,   263,   264,   273,   275,   277,   281,   282,   286,
     288,   292,   294,   297,   299,   302,   304,   306,   308,   311,
     312,   315,   316,   321,   322,   327,   331,   335,   340,   341,
     342,   349,   350,   351,   352,   362,   363,   364,   371,   372,
     373,   374,   384,   385,   386,   393,   394,   395,   396,   406,
     410,   414,   415,   421,   422,   427,   430,   432,   435,   437,
     441,   445,   448,   450,   451,   455,   456,   461,   464,   466,
     468,   469,   474,   477,   480,   483,   485,   490,   493,   495,
     499,   501,   503,   505,   508,   509,   511,   512,   517,   521,
     523,   527,   528,   531,   533,   535,   538,   541,   543,   545,
     546,   550,   552,   554,   557,   558,   564,   565,   571,   572,
     578,   579,   585,   587,   589,   591,   592,   598,   600,   602,
     604,   607,   608,   614,   619,   621,   624,   626,   629,   631,
     634,   635,   642,   643,   650,   651,   658,   660,   661,   663,
     664,   667,   668,   671,   674,   675,   680,   681,   687,   690,
     691,   692,   698,   701,   702,   703,   709,   712,   713,   714,
     720,   723,   724,   725,   731,   734,   735,   736,   742,   745,
     746,   747,   753,   756,   757,   759,   762,   764,   765,   770,
     771,   777,   780,   783,   785,   787,   789,   790,   796,   797,
     802,   804,   806,   809,   811,   813,   815,   817,   818,   823,
     825,   827,   829,   831,   833,   835,   837,   839,   841,   843,
     845,   847,   849,   851,   853,   855,   857,   859,   861,   863,
     865,   867,   869,   871,   873,   875,   877,   879,   881,   883,
     885,   887,   889,   891,   893,   895,   897,   899,   901,   903,
     905,   907,   909,   911,   913,   915,   917,   919,   921,   923,
     925,   927,   929,   931,   933,   935,   937,   939,   941,   943,
     945,   947,   949,   951,   953,   955,   957,   959,   961,   963,
     965,   967,   969,   971,   973,   975,   977,   979,   981,   983,
     985,   987,   989,   991,   993,   995,   997,   999,  1001,  1003,
    1005,  1007,  1009,  1011,  1013,  1015,  1017,  1019,  1021,  1023,
    1025,  1027,  1029,  1031,  1033,  1035,  1037,  1039,  1041,  1043,
    1045,  1047,  1049,  1051,  1053,  1055,  1057,  1059,  1061,  1063,
    1065,  1067,  1069,  1071,  1073,  1075,  1077,  1079,  1081,  1083,
    1085,  1087,  1089,  1091,  1093,  1095,  1097,  1099,  1101,  1103,
    1105,  1107,  1109,  1111,  1113,  1115,  1117,  1119,  1121,  1123,
    1125,  1127,  1129,  1131,  1133,  1135,  1137,  1138,  1139,  1146,
    1147,  1148,  1155,  1156,  1160,  1161,  1165,  1166,  1171,  1172,
    1177,  1178,  1183,  1184,  1189,  1190,  1195,  1196,  1200,  1201,
    1205,  1206,  1210,  1215,  1216,  1220,  1221,  1226,  1227,  1233,
    1234,  1240,  1241,  1247,  1248,  1254,  1259,  1260,  1266,  1267,
    1273,  1274,  1280,  1281,  1287,  1291,  1295,  1296,  1302,  1303,
    1309,  1310,  1316,  1317,  1323,  1324,  1330,  1331,  1337,  1338,
    1344,  1345,  1351,  1356,  1362,  1367,  1368,  1374,  1380,  1386,
    1393,  1399,  1405,  1412,  1418,  1424,  1430,  1436,  1441,  1442,
    1443,  1451,  1452,  1453,  1462,  1467,  1473,  1479,  1485,  1490,
    1496,  1499,  1502,  1505,  1508,  1514,  1519,  1520,  1524,  1526,
    1528,  1531,  1534,  1536,  1538,  1540,  1542,  1544,  1546,  1547,
    1549,  1551,  1553,  1555,  1557,  1559,  1561,  1563,  1565,  1567,
    1569,  1571,  1573,  1575,  1577,  1579,  1581,  1583,  1585,  1587,
    1589,  1591,  1593,  1594,  1596,  1597,  1599,  1601,  1603,  1604,
    1609,  1610,  1615,  1616,  1617,  1624,  1626,  1627,  1631,  1632,
    1637,  1638,  1643,  1644,  1648,  1649,  1654,  1656,  1657,  1661,
    1662,  1667,  1669,  1670,  1674,  1675,  1680,  1681,  1686,  1687,
    1691,  1692,  1696,  1697,  1702,  1704,  1705,  1709,  1710,  1714,
    1715,  1720,  1722,  1723,  1727,  1728,  1733,  1735,  1736,  1740,
    1741,  1746,  1748,  1749,  1753,  1754,  1759,  1761,  1762,  1766,
    1767,  1772,  1774,  1777,  1778,  1781,  1782,  1785,  1786,  1788,
    1790,  1792,  1794,  1796,  1798,  1800,  1802,  1803,  1808,  1810,
    1812,  1814,  1816,  1818,  1820,  1822,  1823,  1828,  1830,  1832,
    1834,  1836,  1838,  1840,  1841,  1846,  1848,  1850,  1852,  1854,
    1856,  1858,  1860,  1861,  1866,  1868,  1870,  1872,  1874,  1876,
    1878,  1879,  1884,  1886,  1888,  1890,  1892,  1894,  1896,  1897,
    1902,  1904,  1906,  1908,  1910,  1912,  1914,  1915,  1920,  1922,
    1924,  1926,  1928,  1930,  1931,  1936,  1938,  1940,  1942,  1944,
    1946,  1947,  1952,  1955,  1957,  1959,  1960,  1965,  1967,  1969,
    1971,  1973,  1975,  1977
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     164,     0,    -1,   165,    -1,    -1,   212,    -1,   200,    -1,
     166,    -1,   329,    -1,    -1,   159,   167,     3,    -1,    -1,
      92,   168,     3,    -1,    87,    -1,    -1,    88,   169,     3,
      -1,    -1,    89,   170,     3,    -1,    90,    -1,    -1,    91,
     171,     4,    -1,    93,    -1,    94,    -1,   312,   281,    -1,
     321,   121,    -1,   179,   281,    -1,   176,   281,    -1,   177,
     281,    -1,   173,   130,   172,    -1,   162,    -1,   174,    -1,
     173,   130,   173,    -1,   175,    -1,   176,    -1,   179,    -1,
     177,    -1,   311,    -1,   174,   131,   181,    -1,    -1,   175,
     132,   178,   180,   133,    -1,   119,   173,   120,    -1,   180,
     125,   311,    -1,   311,    -1,    -1,    -1,   119,   182,   184,
     183,   120,    -1,   185,    -1,     1,    -1,   185,   125,   186,
      -1,   186,    -1,    98,   303,   128,   303,    -1,   127,   311,
     128,   311,    -1,   100,   187,   190,   128,   191,   194,    -1,
      -1,   311,   188,   289,    -1,    -1,   119,   189,   285,   120,
      -1,   117,   259,   258,   260,    -1,    -1,    -1,   311,   192,
     291,    -1,    -1,   119,   193,   285,   120,    -1,   123,   195,
     124,    -1,    -1,   195,   196,    -1,   196,    -1,   141,   160,
      -1,    -1,   142,   119,   197,   274,   120,    -1,    -1,   155,
     119,   198,   274,   120,    -1,    -1,   149,   119,   199,     5,
     120,    -1,    -1,    -1,    -1,     8,   201,   311,   112,   173,
     202,   128,   173,   203,   280,   204,    97,    -1,   204,   205,
      -1,    -1,    98,   303,   128,   207,    -1,   102,   247,   117,
     220,    -1,    -1,   100,   206,   289,   208,    -1,     1,   121,
      -1,   303,   281,    -1,   162,    -1,    -1,    -1,   117,   209,
     259,   258,   260,   128,   210,   221,    -1,    -1,   128,   211,
     221,    -1,    -1,    -1,   222,   213,   311,   214,   216,   280,
     223,    95,    -1,   121,    -1,   162,    -1,   132,   217,   133,
      -1,    -1,   217,   125,   218,    -1,   218,    -1,   311,   129,
     173,    -1,   162,    -1,   260,   281,    -1,   219,    -1,   299,
     215,    -1,   162,    -1,     6,    -1,     7,    -1,   223,   224,
      -1,    -1,    96,   172,    -1,    -1,    98,   225,   282,   215,
      -1,    -1,    99,   226,   283,   215,    -1,   100,   248,   254,
      -1,    56,   251,   254,    -1,   102,   247,   117,   220,    -1,
      -1,    -1,   105,   227,   287,   117,   228,   221,    -1,    -1,
      -1,    -1,   106,   229,   287,   117,   230,   297,   116,   231,
     221,    -1,    -1,    -1,   107,   232,   293,   118,   233,   221,
      -1,    -1,    -1,    -1,   108,   234,   293,   118,   235,   297,
     116,   236,   221,    -1,    -1,    -1,   109,   237,   295,   114,
     238,   221,    -1,    -1,    -1,    -1,   110,   239,   295,   114,
     240,   297,   116,   241,   221,    -1,   153,   248,   254,    -1,
     101,   251,   254,    -1,    -1,   103,   311,   242,   244,   121,
      -1,    -1,   104,   243,   283,   215,    -1,     1,   121,    -1,
     126,    -1,   126,   245,    -1,   246,    -1,   245,   125,   246,
      -1,   312,   117,   311,    -1,   247,   317,    -1,   317,    -1,
      -1,   311,   249,   287,    -1,    -1,   119,   250,   285,   120,
      -1,   251,   252,    -1,   252,    -1,   317,    -1,    -1,   119,
     253,   285,   120,    -1,   117,   255,    -1,   260,   256,    -1,
     258,   257,    -1,   219,    -1,   260,   259,   258,   257,    -1,
     258,   257,    -1,   121,    -1,   260,   263,   281,    -1,   219,
      -1,   113,    -1,   115,    -1,   259,   260,    -1,    -1,   303,
      -1,    -1,   123,   261,   262,   124,    -1,   262,   125,   303,
      -1,   303,    -1,   123,   264,   124,    -1,    -1,   264,   266,
      -1,   266,    -1,   136,    -1,   139,   136,    -1,   140,   136,
      -1,   134,    -1,   135,    -1,    -1,   265,   267,   308,    -1,
     137,    -1,   138,    -1,   141,   160,    -1,    -1,   142,   119,
     268,   274,   120,    -1,    -1,   155,   119,   269,   274,   120,
      -1,    -1,   144,   119,   270,   274,   120,    -1,    -1,   145,
     119,   271,   274,   120,    -1,   146,    -1,   148,    -1,   147,
      -1,    -1,   147,   119,   272,   274,   120,    -1,   151,    -1,
     152,    -1,   153,    -1,   143,   160,    -1,    -1,   149,   119,
     273,     5,   120,    -1,   150,   119,   275,   120,    -1,   154,
      -1,   274,   160,    -1,   160,    -1,   275,   276,    -1,   276,
      -1,   156,   311,    -1,    -1,   156,   311,   119,   277,   285,
     120,    -1,    -1,   157,   311,   119,   278,   285,   120,    -1,
      -1,   158,   311,   119,   279,   285,   120,    -1,   111,    -1,
      -1,   121,    -1,    -1,   282,   303,    -1,    -1,   283,   303,
      -1,   283,   122,    -1,    -1,   303,   284,   282,   122,    -1,
      -1,   285,   119,   286,   285,   120,    -1,   285,   311,    -1,
      -1,    -1,   287,   119,   288,   285,   120,    -1,   287,   317,
      -1,    -1,    -1,   289,   119,   290,   285,   120,    -1,   289,
     318,    -1,    -1,    -1,   291,   119,   292,   285,   120,    -1,
     291,   319,    -1,    -1,    -1,   293,   119,   294,   285,   120,
      -1,   293,   315,    -1,    -1,    -1,   295,   119,   296,   285,
     120,    -1,   295,   314,    -1,    -1,    -1,   297,   119,   298,
     285,   120,    -1,   297,   316,    -1,    -1,   300,    -1,   299,
     321,    -1,   321,    -1,    -1,   119,   301,   285,   120,    -1,
      -1,   299,   119,   302,   285,   120,    -1,   300,   324,    -1,
     299,   313,    -1,   312,    -1,   304,    -1,   320,    -1,    -1,
     304,   132,   305,   306,   133,    -1,    -1,   306,   125,   307,
     304,    -1,   304,    -1,   309,    -1,   308,   309,    -1,   323,
      -1,   325,    -1,   326,    -1,   121,    -1,    -1,   119,   310,
     285,   120,    -1,   323,    -1,   324,    -1,   326,    -1,   327,
      -1,   121,    -1,   322,    -1,   125,    -1,   128,    -1,   324,
      -1,   326,    -1,   327,    -1,   322,    -1,   125,    -1,   128,
      -1,   326,    -1,   327,    -1,   323,    -1,   324,    -1,   327,
      -1,   121,    -1,   122,    -1,   117,    -1,   113,    -1,   115,
      -1,   118,    -1,   116,    -1,   323,    -1,   324,    -1,   327,
      -1,   121,    -1,   122,    -1,   117,    -1,   113,    -1,   115,
      -1,   114,    -1,   116,    -1,   323,    -1,   324,    -1,   327,
      -1,   121,    -1,   122,    -1,   117,    -1,   113,    -1,   115,
      -1,   118,    -1,   114,    -1,   323,    -1,   324,    -1,   327,
      -1,   121,    -1,   122,    -1,   113,    -1,   115,    -1,   118,
      -1,   114,    -1,   116,    -1,   322,    -1,   162,    -1,   125,
      -1,   324,    -1,   327,    -1,   121,    -1,   122,    -1,   113,
      -1,   115,    -1,   118,    -1,   114,    -1,   116,    -1,   322,
      -1,   162,    -1,   128,    -1,   324,    -1,   328,    -1,   121,
      -1,   124,    -1,   326,    -1,   160,    -1,   324,    -1,   328,
      -1,   118,    -1,   126,    -1,   130,    -1,   131,    -1,   114,
      -1,   116,    -1,   111,    -1,   127,    -1,   128,    -1,   121,
      -1,   162,    -1,   160,    -1,   132,    -1,   133,    -1,   130,
      -1,   131,    -1,   126,    -1,   129,    -1,   127,    -1,   112,
      -1,   111,    -1,   322,    -1,   162,    -1,   125,    -1,   128,
      -1,   153,    -1,   325,    -1,    96,    -1,    98,    -1,    99,
      -1,   100,    -1,    56,    -1,   102,    -1,   101,    -1,   103,
      -1,   104,    -1,   105,    -1,   106,    -1,   107,    -1,   108,
      -1,   109,    -1,   110,    -1,    95,    -1,    97,    -1,   122,
      -1,   117,    -1,   113,    -1,   115,    -1,   118,    -1,   114,
      -1,   116,    -1,   123,    -1,   124,    -1,   328,    -1,   134,
      -1,   135,    -1,   136,    -1,   137,    -1,   138,    -1,   139,
      -1,   140,    -1,   141,    -1,   142,    -1,   144,    -1,   145,
      -1,   146,    -1,   148,    -1,   149,    -1,   150,    -1,   147,
      -1,   143,    -1,   151,    -1,   152,    -1,   154,    -1,   155,
      -1,   156,    -1,   157,    -1,   158,    -1,    -1,    -1,    32,
     330,   422,   331,   420,   121,    -1,    -1,    -1,    81,   332,
     422,   333,   420,   121,    -1,    -1,     9,   334,   379,    -1,
      -1,    80,   335,   379,    -1,    -1,   376,    11,   336,   379,
      -1,    -1,   376,    12,   337,   385,    -1,    -1,   376,    19,
     338,   395,    -1,    -1,   376,    20,   339,   395,    -1,    -1,
     376,    21,   340,   385,    -1,    -1,    23,   341,   395,    -1,
      -1,   374,   342,   385,    -1,    -1,   375,   343,   385,    -1,
     376,    22,   377,   121,    -1,    -1,    13,   344,   379,    -1,
      -1,   119,   345,   419,   120,    -1,    -1,    28,   371,   346,
     440,   121,    -1,    -1,    28,   372,   347,   440,   121,    -1,
      -1,    49,   371,   348,   440,   121,    -1,    -1,    36,   373,
     349,   440,   121,    -1,    65,    66,   146,   121,    -1,    -1,
      25,     6,   350,   420,   121,    -1,    -1,    25,    51,   351,
     420,   121,    -1,    -1,    25,    54,   352,   420,   121,    -1,
      -1,    25,     8,   353,   420,   121,    -1,    25,    52,   121,
      -1,    25,    53,   121,    -1,    -1,    25,    55,   354,   420,
     121,    -1,    -1,    25,    56,   355,   420,   121,    -1,    -1,
      25,    57,   356,   420,   121,    -1,    -1,    25,    58,   357,
     420,   121,    -1,    -1,    25,    59,   358,   420,   121,    -1,
      -1,    25,    60,   359,   420,   121,    -1,    -1,    25,    61,
     360,   420,   121,    -1,    -1,    25,    62,   361,   420,   121,
      -1,    25,    50,    86,   121,    -1,    25,    50,   127,    86,
     121,    -1,    25,    23,    37,   121,    -1,    -1,    25,    82,
     362,   420,   121,    -1,    24,    25,    63,   370,   121,    -1,
      24,    25,    45,   370,   121,    -1,    24,    25,    13,    45,
     370,   121,    -1,    24,    25,    46,   370,   121,    -1,    24,
      25,    48,   370,   121,    -1,    24,    25,    13,    46,   370,
     121,    -1,    24,    25,    47,   370,   121,    -1,    24,    25,
      43,   370,   121,    -1,    24,    36,   368,   370,   121,    -1,
      24,    28,   369,   370,   121,    -1,    24,    49,   370,   121,
      -1,    -1,    -1,    24,   378,   363,   443,   364,   370,   121,
      -1,    -1,    -1,    24,     7,    69,   365,   443,   366,   370,
     121,    -1,    24,    64,   370,   121,    -1,    24,    66,   146,
     370,   121,    -1,    24,    66,    60,   370,   121,    -1,    24,
      73,    74,   370,   121,    -1,    24,    82,   370,   121,    -1,
      24,    66,    82,   370,   121,    -1,    76,   121,    -1,    77,
     121,    -1,    78,   121,    -1,    79,   121,    -1,    24,    43,
      25,   370,   121,    -1,    24,    45,   370,   121,    -1,    -1,
       1,   367,   121,    -1,    38,    -1,    39,    -1,    40,    42,
      -1,    40,    41,    -1,    37,    -1,    71,    -1,    83,    -1,
      84,    -1,    85,    -1,   155,    -1,    -1,    34,    -1,    31,
      -1,    35,    -1,    32,    -1,    58,    -1,    59,    -1,    60,
      -1,    12,    -1,    29,    -1,    30,    -1,    26,    -1,    27,
      -1,    32,    -1,    33,    -1,    70,    -1,    69,    -1,    71,
      -1,    72,    -1,    16,    -1,    15,    -1,    18,    -1,    17,
      -1,    75,    -1,    -1,    86,    -1,    -1,    67,    -1,    68,
      -1,    69,    -1,    -1,   159,   380,   421,   382,    -1,    -1,
     428,   381,   420,   121,    -1,    -1,    -1,   117,   383,   422,
     384,   420,   121,    -1,   121,    -1,    -1,   123,   386,   389,
      -1,    -1,   159,   387,   421,   382,    -1,    -1,   430,   388,
     420,   121,    -1,    -1,   161,   390,   392,    -1,    -1,   432,
     391,   420,   121,    -1,   121,    -1,    -1,   124,   393,   379,
      -1,    -1,   434,   394,   420,   121,    -1,   121,    -1,    -1,
     123,   396,   399,    -1,    -1,   159,   397,   421,   382,    -1,
      -1,   430,   398,   420,   121,    -1,    -1,   161,   400,   403,
      -1,    -1,   125,   401,   413,    -1,    -1,   438,   402,   420,
     121,    -1,   121,    -1,    -1,   124,   404,   379,    -1,    -1,
     125,   405,   407,    -1,    -1,   436,   406,   420,   121,    -1,
     121,    -1,    -1,   161,   408,   410,    -1,    -1,   432,   409,
     420,   121,    -1,   121,    -1,    -1,   124,   411,   379,    -1,
      -1,   434,   412,   420,   121,    -1,   121,    -1,    -1,   161,
     414,   416,    -1,    -1,   432,   415,   420,   121,    -1,   121,
      -1,    -1,   124,   417,   379,    -1,    -1,   434,   418,   420,
     121,    -1,   121,    -1,   419,   422,    -1,    -1,   420,   424,
      -1,    -1,   421,   426,    -1,    -1,   160,    -1,   161,    -1,
     123,    -1,   124,    -1,   159,    -1,   117,    -1,   121,    -1,
     125,    -1,    -1,   119,   423,   419,   120,    -1,   160,    -1,
     161,    -1,   123,    -1,   124,    -1,   159,    -1,   117,    -1,
     125,    -1,    -1,   119,   425,   419,   120,    -1,   160,    -1,
     161,    -1,   123,    -1,   124,    -1,   159,    -1,   125,    -1,
      -1,   119,   427,   419,   120,    -1,   160,    -1,   161,    -1,
     123,    -1,   124,    -1,   117,    -1,   121,    -1,   125,    -1,
      -1,   119,   429,   419,   120,    -1,   160,    -1,   161,    -1,
     124,    -1,   117,    -1,   121,    -1,   125,    -1,    -1,   119,
     431,   419,   120,    -1,   160,    -1,   123,    -1,   124,    -1,
     159,    -1,   117,    -1,   125,    -1,    -1,   119,   433,   419,
     120,    -1,   160,    -1,   161,    -1,   123,    -1,   159,    -1,
     117,    -1,   125,    -1,    -1,   119,   435,   419,   120,    -1,
     160,    -1,   161,    -1,   123,    -1,   159,    -1,   117,    -1,
      -1,   119,   437,   419,   120,    -1,   160,    -1,   123,    -1,
     124,    -1,   159,    -1,   117,    -1,    -1,   119,   439,   419,
     120,    -1,   440,   441,    -1,   441,    -1,   443,    -1,    -1,
     119,   442,   419,   120,    -1,   160,    -1,   161,    -1,   123,
      -1,   124,    -1,   159,    -1,   117,    -1,   125,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,   216,   216,   218,   223,   224,   225,   226,   232,   232,
     242,   242,   252,   256,   256,   267,   267,   278,   289,   289,
     294,   299,   332,   336,   340,   341,   342,   343,   351,   361,
     362,   372,   373,   376,   377,   378,   385,   394,   394,   403,
     406,   407,   414,   419,   413,   425,   426,   429,   430,   433,
     437,   441,   444,   444,   449,   449,   456,   457,   460,   460,
     462,   462,   466,   467,   470,   471,   474,   475,   475,   477,
     477,   479,   479,   486,   488,   496,   486,   509,   510,   513,
     517,   518,   518,   520,   523,   524,   535,   543,   534,   552,
     551,   586,   588,   586,   601,   602,   612,   613,   616,   617,
     620,   628,   640,   641,   644,   645,   655,   655,   658,   659,
     662,   669,   669,   672,   672,   675,   677,   679,   681,   682,
     681,   685,   686,   687,   685,   690,   691,   690,   694,   695,
     696,   694,   699,   700,   699,   703,   704,   705,   703,   708,
     713,   719,   718,   725,   725,   728,   739,   740,   743,   744,
     747,   752,   753,   756,   756,   758,   758,   762,   763,   766,
     772,   772,   776,   780,   781,   786,   794,   799,   804,   811,
     812,   815,   816,   819,   820,   823,   829,   829,   836,   837,
     840,   841,   844,   845,   848,   852,   856,   862,   866,   870,
     870,   872,   876,   880,   881,   881,   883,   883,   885,   885,
     887,   887,   889,   893,   897,   902,   902,   904,   908,   912,
     916,   920,   920,   922,   923,   929,   930,   933,   934,   937,
     942,   942,   947,   947,   952,   952,   962,   964,   970,   972,
     980,   981,   984,   985,   986,   986,   993,   993,   995,   996,
     999,   999,  1001,  1002,  1005,  1005,  1007,  1008,  1011,  1011,
    1013,  1014,  1017,  1017,  1019,  1020,  1023,  1023,  1025,  1026,
    1029,  1029,  1031,  1032,  1035,  1036,  1037,  1040,  1040,  1042,
    1042,  1044,  1045,  1046,  1052,  1064,  1065,  1065,  1069,  1069,
    1071,  1077,  1078,  1081,  1081,  1081,  1081,  1082,  1082,  1089,
    1089,  1089,  1089,  1089,  1092,  1092,  1092,  1093,  1093,  1093,
    1096,  1096,  1096,  1097,  1097,  1100,  1100,  1100,  1100,  1101,
    1101,  1101,  1101,  1101,  1101,  1104,  1104,  1104,  1104,  1105,
    1105,  1105,  1105,  1105,  1105,  1108,  1108,  1108,  1108,  1109,
    1109,  1109,  1109,  1109,  1109,  1112,  1112,  1112,  1112,  1113,
    1113,  1113,  1113,  1113,  1113,  1116,  1116,  1116,  1117,  1117,
    1117,  1118,  1118,  1118,  1118,  1118,  1118,  1121,  1121,  1121,
    1122,  1122,  1122,  1122,  1122,  1125,  1125,  1125,  1126,  1126,
    1126,  1126,  1127,  1127,  1127,  1127,  1127,  1130,  1130,  1136,
    1136,  1136,  1136,  1136,  1136,  1136,  1136,  1137,  1137,  1140,
    1140,  1140,  1140,  1143,  1143,  1146,  1146,  1146,  1146,  1146,
    1146,  1147,  1147,  1147,  1148,  1148,  1148,  1148,  1148,  1148,
    1148,  1148,  1151,  1151,  1151,  1151,  1151,  1151,  1151,  1154,
    1154,  1154,  1157,  1157,  1157,  1157,  1157,  1157,  1157,  1158,
    1158,  1158,  1158,  1158,  1158,  1159,  1159,  1159,  1159,  1160,
    1160,  1160,  1160,  1161,  1161,  1161,  1188,  1189,  1188,  1195,
    1196,  1195,  1204,  1203,  1217,  1216,  1230,  1229,  1243,  1242,
    1256,  1255,  1270,  1269,  1284,  1283,  1297,  1296,  1311,  1310,
    1324,  1323,  1336,  1341,  1340,  1352,  1352,  1361,  1361,  1367,
    1367,  1373,  1373,  1379,  1379,  1385,  1393,  1393,  1400,  1400,
    1407,  1407,  1414,  1414,  1421,  1425,  1429,  1429,  1436,  1436,
    1443,  1443,  1450,  1450,  1457,  1457,  1464,  1464,  1471,  1471,
    1478,  1478,  1485,  1489,  1493,  1497,  1497,  1507,  1511,  1515,
    1519,  1523,  1527,  1531,  1535,  1539,  1543,  1547,  1551,  1552,
    1551,  1557,  1558,  1557,  1563,  1567,  1571,  1575,  1579,  1583,
    1590,  1594,  1598,  1602,  1609,  1613,  1620,  1620,  1627,  1628,
    1629,  1630,  1631,  1632,  1633,  1634,  1635,  1636,  1639,  1640,
    1641,  1642,  1643,  1644,  1645,  1646,  1647,  1648,  1649,  1652,
    1653,  1656,  1657,  1660,  1661,  1664,  1665,  1668,  1669,  1672,
    1673,  1676,  1677,  1680,  1681,  1684,  1685,  1686,  1694,  1694,
    1696,  1696,  1700,  1701,  1700,  1703,  1713,  1713,  1715,  1715,
    1717,  1717,  1722,  1722,  1724,  1724,  1726,  1731,  1730,  1736,
    1736,  1738,  1751,  1751,  1753,  1753,  1755,  1755,  1764,  1764,
    1766,  1766,  1768,  1768,  1770,  1779,  1778,  1784,  1784,  1786,
    1786,  1788,  1796,  1796,  1798,  1798,  1800,  1809,  1808,  1815,
    1815,  1817,  1825,  1825,  1827,  1827,  1829,  1838,  1837,  1843,
    1843,  1845,  1851,  1852,  1855,  1856,  1860,  1861,  1867,  1867,
    1867,  1867,  1867,  1867,  1867,  1867,  1868,  1868,  1872,  1872,
    1872,  1872,  1872,  1872,  1872,  1873,  1873,  1878,  1878,  1878,
    1878,  1878,  1878,  1879,  1879,  1883,  1883,  1883,  1883,  1883,
    1883,  1883,  1884,  1884,  1888,  1888,  1888,  1888,  1888,  1888,
    1889,  1889,  1894,  1894,  1894,  1894,  1894,  1894,  1895,  1895,
    1900,  1900,  1900,  1900,  1900,  1900,  1901,  1901,  1906,  1906,
    1906,  1906,  1906,  1907,  1907,  1912,  1912,  1912,  1912,  1912,
    1913,  1913,  1919,  1920,  1923,  1929,  1929,  1937,  1937,  1937,
    1937,  1937,  1937,  1937
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FILE_NAME_STRING",
  "UNINTERPRETED_STRING", "LATEX_STRING", "KW_MOD", "KW_OMOD", "KW_VIEW",
  "KW_PARSE", "KW_NORMALIZE", "KW_REDUCE", "KW_REWRITE", "KW_LOOP",
  "KW_NARROW", "KW_MATCH", "KW_XMATCH", "KW_UNIFY", "KW_XUNIFY",
  "KW_EREWRITE", "KW_FREWRITE", "KW_SREWRITE", "KW_CONTINUE", "KW_SEARCH",
  "KW_SET", "KW_SHOW", "KW_ON", "KW_OFF", "KW_TRACE", "KW_BODY",
  "KW_BUILTIN", "KW_WHOLE", "KW_SELECT", "KW_DESELECT", "KW_CONDITION",
  "KW_SUBSTITUTION", "KW_PRINT", "KW_GRAPH", "KW_MIXFIX", "KW_FLAT",
  "KW_WITH", "KW_PARENS", "KW_ALIASES", "KW_GC", "KW_TIME", "KW_STATS",
  "KW_TIMING", "KW_CMD", "KW_BREAKDOWN", "KW_BREAK", "KW_PATH",
  "KW_MODULE", "KW_MODULES", "KW_VIEWS", "KW_ALL", "KW_SORTS", "KW_OPS",
  "KW_VARS", "KW_MBS", "KW_EQS", "KW_RLS", "KW_SUMMARY", "KW_KINDS",
  "KW_ADVISE", "KW_VERBOSE", "KW_DO", "KW_CLEAR", "KW_PROTECT",
  "KW_EXTEND", "KW_INCLUDE", "KW_EXCLUDE", "KW_CONCEAL", "KW_REVEAL",
  "KW_COMPILE", "KW_COUNT", "KW_DEBUG", "KW_RESUME", "KW_ABORT", "KW_STEP",
  "KW_WHERE", "KW_CREDUCE", "KW_DUMP", "KW_PROFILE", "KW_NUMBER", "KW_RAT",
  "KW_COLOR", "SIMPLE_NUMBER", "KW_PWD", "KW_CD", "KW_PUSHD", "KW_POPD",
  "KW_LS", "KW_LOAD", "KW_QUIT", "KW_EOF", "KW_ENDM", "KW_IMPORT",
  "KW_ENDV", "KW_SORT", "KW_SUBSORT", "KW_OP", "KW_MSGS", "KW_VAR",
  "KW_CLASS", "KW_SUBCLASS", "KW_MB", "KW_CMB", "KW_EQ", "KW_CEQ", "KW_RL",
  "KW_CRL", "KW_IS", "KW_FROM", "KW_ARROW", "KW_ARROW2", "KW_PARTIAL",
  "KW_IF", "':'", "'='", "'('", "')'", "'.'", "'<'", "'['", "']'", "','",
  "'|'", "KW_LABEL", "KW_TO", "KW_COLON2", "'+'", "'*'", "'{'", "'}'",
  "KW_ASSOC", "KW_COMM", "KW_ID", "KW_IDEM", "KW_ITER", "KW_LEFT",
  "KW_RIGHT", "KW_PREC", "KW_GATHER", "KW_METADATA", "KW_STRAT", "KW_POLY",
  "KW_MEMO", "KW_FROZEN", "KW_CTOR", "KW_LATEX", "KW_SPECIAL", "KW_CONFIG",
  "KW_OBJ", "KW_MSG", "KW_DITTO", "KW_FORMAT", "KW_ID_HOOK", "KW_OP_HOOK",
  "KW_TERM_HOOK", "KW_IN", "IDENTIFIER", "NUMERIC_ID", "ENDS_IN_DOT",
  "$accept", "top", "item", "directive", "@1", "@2", "@3", "@4", "@5",
  "moduleExprDot", "moduleExpr", "moduleExpr2", "moduleExpr3",
  "renameExpr", "instantExpr", "@6", "parenExpr", "argList", "renaming",
  "@7", "@8", "renaming2", "mappingList", "mapping", "fromOpName", "@9",
  "@10", "fromSpec", "toOpName", "@11", "@12", "toAttributes",
  "toAttributeList", "toAttribute", "@13", "@14", "@15", "view", "@16",
  "@17", "@18", "viewDecList", "viewDeclaration", "@19", "sortDot",
  "viewEndOpMap", "@20", "@21", "@22", "module", "@23", "@24", "dot",
  "parameters", "parameterList", "parameter", "badType", "typeDot",
  "endStatement", "startModule", "decList", "declaration", "@25", "@26",
  "@27", "@28", "@29", "@30", "@31", "@32", "@33", "@34", "@35", "@36",
  "@37", "@38", "@39", "@40", "@41", "@42", "@43", "classDef", "cPairList",
  "cPair", "varNameList", "opName", "@44", "@45", "opNameList",
  "simpleOpName", "@46", "domainRangeAttr", "dra2", "dra3", "typeAttr",
  "arrow", "typeList", "typeName", "@47", "sortNames", "attributes",
  "attributeList", "idKeyword", "attribute", "@48", "@49", "@50", "@51",
  "@52", "@53", "@54", "idList", "hookList", "hook", "@55", "@56", "@57",
  "expectedIs", "expectedDot", "sortNameList", "subsortList", "@58",
  "tokens", "@59", "tokensBarColon", "@60", "tokensBarColonTo", "@61",
  "tokensBarCommaLeft", "@62", "tokensBarEqual", "@63", "tokensBarArrow2",
  "@64", "tokensBarIf", "@65", "endTokens", "noTrailingDot", "@66", "@67",
  "sortName", "sortNameFrag", "@68", "sortNameFrags", "@69", "identity",
  "identityChunk", "@70", "token", "tokenBarDot", "endToken",
  "tokenBarArrow2", "tokenBarEqual", "tokenBarIf", "tokenBarColon",
  "tokenBarColonTo", "tokenBarCommaLeft", "sortToken", "endsInDot",
  "inert", "identifier", "startKeyword", "startKeyword2", "midKeyword",
  "attrKeyword", "attrKeyword2", "command", "@71", "@72", "@73", "@74",
  "@75", "@76", "@77", "@78", "@79", "@80", "@81", "@82", "@83", "@84",
  "@85", "@86", "@87", "@88", "@89", "@90", "@91", "@92", "@93", "@94",
  "@95", "@96", "@97", "@98", "@99", "@100", "@101", "@102", "@103",
  "@104", "@105", "@106", "@107", "@108", "printOption", "traceOption",
  "polarity", "select", "exclude", "conceal", "match", "unify", "optDebug",
  "optNumber", "importMode", "moduleAndTerm", "@109", "@110", "inEnd",
  "@111", "@112", "numberModuleTerm", "@113", "@114", "@115",
  "numberModuleTerm1", "@116", "@117", "numberModuleTerm2", "@118", "@119",
  "numbersModuleTerm", "@120", "@121", "@122", "numbersModuleTerm1",
  "@123", "@124", "@125", "numbersModuleTerm2", "@126", "@127", "@128",
  "numbersModuleTerm3", "@129", "@130", "numbersModuleTerm4", "@131",
  "@132", "numbersModuleTerm5", "@133", "@134", "numbersModuleTerm6",
  "@135", "@136", "cTokens", "cTokensBarDot", "cTokensBarDotColon",
  "cToken", "@137", "cTokenBarDot", "@138", "cTokenBarDotColon", "@139",
  "cTokenBarIn", "@140", "cTokenBarLeftIn", "@141", "cTokenBarDotNumber",
  "@142", "cTokenBarDotRight", "@143", "cTokenBarDotCommaRight", "@144",
  "cTokenBarDotCommaNumber", "@145", "cOpNameList", "cSimpleOpName",
  "@146", "cSimpleTokenBarDot", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
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
     365,   366,   367,   368,   369,   370,   371,    58,    61,    40,
      41,    46,    60,    91,    93,    44,   124,   372,   373,   374,
      43,    42,   123,   125,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
{
       0,   163,   164,   164,   165,   165,   165,   165,   167,   166,
     168,   166,   166,   169,   166,   170,   166,   166,   171,   166,
     166,   166,   172,   172,   172,   172,   172,   172,   172,   173,
     173,   174,   174,   175,   175,   175,   176,   178,   177,   179,
     180,   180,   182,   183,   181,   184,   184,   185,   185,   186,
     186,   186,   188,   187,   189,   187,   190,   190,   192,   191,
     193,   191,   194,   194,   195,   195,   196,   197,   196,   198,
     196,   199,   196,   201,   202,   203,   200,   204,   204,   205,
     205,   206,   205,   205,   207,   207,   209,   210,   208,   211,
     208,   213,   214,   212,   215,   215,   216,   216,   217,   217,
     218,   219,   220,   220,   221,   221,   222,   222,   223,   223,
     224,   225,   224,   226,   224,   224,   224,   224,   227,   228,
     224,   229,   230,   231,   224,   232,   233,   224,   234,   235,
     236,   224,   237,   238,   224,   239,   240,   241,   224,   224,
     224,   242,   224,   243,   224,   224,   244,   244,   245,   245,
     246,   247,   247,   249,   248,   250,   248,   251,   251,   252,
     253,   252,   254,   255,   255,   255,   256,   256,   256,   257,
     257,   258,   258,   259,   259,   260,   261,   260,   262,   262,
     263,   263,   264,   264,   265,   265,   265,   266,   266,   267,
     266,   266,   266,   266,   268,   266,   269,   266,   270,   266,
     271,   266,   266,   266,   266,   272,   266,   266,   266,   266,
     266,   273,   266,   266,   266,   274,   274,   275,   275,   276,
     277,   276,   278,   276,   279,   276,   280,   280,   281,   281,
     282,   282,   283,   283,   284,   283,   286,   285,   285,   285,
     288,   287,   287,   287,   290,   289,   289,   289,   292,   291,
     291,   291,   294,   293,   293,   293,   296,   295,   295,   295,
     298,   297,   297,   297,   299,   299,   299,   301,   300,   302,
     300,   300,   300,   300,   303,   304,   305,   304,   307,   306,
     306,   308,   308,   309,   309,   309,   309,   310,   309,   311,
     311,   311,   311,   311,   312,   312,   312,   312,   312,   312,
     313,   313,   313,   313,   313,   314,   314,   314,   314,   314,
     314,   314,   314,   314,   314,   315,   315,   315,   315,   315,
     315,   315,   315,   315,   315,   316,   316,   316,   316,   316,
     316,   316,   316,   316,   316,   317,   317,   317,   317,   317,
     317,   317,   317,   317,   317,   318,   318,   318,   318,   318,
     318,   318,   318,   318,   318,   318,   318,   319,   319,   319,
     319,   319,   319,   319,   319,   320,   320,   320,   320,   320,
     320,   320,   320,   320,   320,   320,   320,   321,   321,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   323,
     323,   323,   323,   324,   324,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   326,   326,   326,   326,   326,   326,   326,   327,
     327,   327,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   328,   328,   328,   328,
     328,   328,   328,   328,   328,   328,   330,   331,   329,   332,
     333,   329,   334,   329,   335,   329,   336,   329,   337,   329,
     338,   329,   339,   329,   340,   329,   341,   329,   342,   329,
     343,   329,   329,   344,   329,   345,   329,   346,   329,   347,
     329,   348,   329,   349,   329,   329,   350,   329,   351,   329,
     352,   329,   353,   329,   329,   329,   354,   329,   355,   329,
     356,   329,   357,   329,   358,   329,   359,   329,   360,   329,
     361,   329,   329,   329,   329,   362,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   329,   329,   363,   364,
     329,   365,   366,   329,   329,   329,   329,   329,   329,   329,
     329,   329,   329,   329,   329,   329,   367,   329,   368,   368,
     368,   368,   368,   368,   368,   368,   368,   368,   369,   369,
     369,   369,   369,   369,   369,   369,   369,   369,   369,   370,
     370,   371,   371,   372,   372,   373,   373,   374,   374,   375,
     375,   376,   376,   377,   377,   378,   378,   378,   380,   379,
     381,   379,   383,   384,   382,   382,   386,   385,   387,   385,
     388,   385,   390,   389,   391,   389,   389,   393,   392,   394,
     392,   392,   396,   395,   397,   395,   398,   395,   400,   399,
     401,   399,   402,   399,   399,   404,   403,   405,   403,   406,
     403,   403,   408,   407,   409,   407,   407,   411,   410,   412,
     410,   410,   414,   413,   415,   413,   413,   417,   416,   418,
     416,   416,   419,   419,   420,   420,   421,   421,   422,   422,
     422,   422,   422,   422,   422,   422,   423,   422,   424,   424,
     424,   424,   424,   424,   424,   425,   424,   426,   426,   426,
     426,   426,   426,   427,   426,   428,   428,   428,   428,   428,
     428,   428,   429,   428,   430,   430,   430,   430,   430,   430,
     431,   430,   432,   432,   432,   432,   432,   432,   433,   432,
     434,   434,   434,   434,   434,   434,   435,   434,   436,   436,
     436,   436,   436,   437,   436,   438,   438,   438,   438,   438,
     439,   438,   440,   440,   441,   442,   441,   443,   443,   443,
     443,   443,   443,   443
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     0,     1,     1,     1,     1,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       1,     1,     2,     2,     2,     2,     2,     3,     1,     1,
       3,     1,     1,     1,     1,     1,     3,     0,     5,     3,
       3,     1,     0,     0,     5,     1,     1,     3,     1,     4,
       4,     6,     0,     3,     0,     4,     4,     0,     0,     3,
       0,     4,     3,     0,     2,     1,     2,     0,     5,     0,
       5,     0,     5,     0,     0,     0,    12,     2,     0,     4,
       4,     0,     4,     2,     2,     1,     0,     0,     8,     0,
       3,     0,     0,     8,     1,     1,     3,     0,     3,     1,
       3,     1,     2,     1,     2,     1,     1,     1,     2,     0,
       2,     0,     4,     0,     4,     3,     3,     4,     0,     0,
       6,     0,     0,     0,     9,     0,     0,     6,     0,     0,
       0,     9,     0,     0,     6,     0,     0,     0,     9,     3,
       3,     0,     5,     0,     4,     2,     1,     2,     1,     3,
       3,     2,     1,     0,     3,     0,     4,     2,     1,     1,
       0,     4,     2,     2,     2,     1,     4,     2,     1,     3,
       1,     1,     1,     2,     0,     1,     0,     4,     3,     1,
       3,     0,     2,     1,     1,     2,     2,     1,     1,     0,
       3,     1,     1,     2,     0,     5,     0,     5,     0,     5,
       0,     5,     1,     1,     1,     0,     5,     1,     1,     1,
       2,     0,     5,     4,     1,     2,     1,     2,     1,     2,
       0,     6,     0,     6,     0,     6,     1,     0,     1,     0,
       2,     0,     2,     2,     0,     4,     0,     5,     2,     0,
       0,     5,     2,     0,     0,     5,     2,     0,     0,     5,
       2,     0,     0,     5,     2,     0,     0,     5,     2,     0,
       0,     5,     2,     0,     1,     2,     1,     0,     4,     0,
       5,     2,     2,     1,     1,     1,     0,     5,     0,     4,
       1,     1,     2,     1,     1,     1,     1,     0,     4,     1,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     0,     6,     0,
       0,     6,     0,     3,     0,     3,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     3,     0,     3,
       0,     3,     4,     0,     3,     0,     4,     0,     5,     0,
       5,     0,     5,     0,     5,     4,     0,     5,     0,     5,
       0,     5,     0,     5,     3,     3,     0,     5,     0,     5,
       0,     5,     0,     5,     0,     5,     0,     5,     0,     5,
       0,     5,     4,     5,     4,     0,     5,     5,     5,     6,
       5,     5,     6,     5,     5,     5,     5,     4,     0,     0,
       7,     0,     0,     8,     4,     5,     5,     5,     4,     5,
       2,     2,     2,     2,     5,     4,     0,     3,     1,     1,
       2,     2,     1,     1,     1,     1,     1,     1,     0,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     0,     1,     0,     1,     1,     1,     0,     4,
       0,     4,     0,     0,     6,     1,     0,     3,     0,     4,
       0,     4,     0,     3,     0,     4,     1,     0,     3,     0,
       4,     1,     0,     3,     0,     4,     0,     4,     0,     3,
       0,     3,     0,     4,     1,     0,     3,     0,     3,     0,
       4,     1,     0,     3,     0,     4,     1,     0,     3,     0,
       4,     1,     0,     3,     0,     4,     1,     0,     3,     0,
       4,     1,     2,     0,     2,     0,     2,     0,     1,     1,
       1,     1,     1,     1,     1,     1,     0,     4,     1,     1,
       1,     1,     1,     1,     1,     0,     4,     1,     1,     1,
       1,     1,     1,     0,     4,     1,     1,     1,     1,     1,
       1,     1,     0,     4,     1,     1,     1,     1,     1,     1,
       0,     4,     1,     1,     1,     1,     1,     1,     0,     4,
       1,     1,     1,     1,     1,     1,     0,     4,     1,     1,
       1,     1,     1,     0,     4,     1,     1,     1,     1,     1,
       0,     4,     2,     1,     1,     0,     4,     1,     1,     1,
       1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       0,   546,   106,   107,    73,   452,   473,   578,   577,   580,
     579,   466,     0,     0,     0,   446,     0,     0,     0,   581,
       0,     0,     0,     0,   454,   449,    12,    13,    15,    17,
      18,    10,    20,    21,   475,     8,     0,     2,     6,     5,
       4,    91,     7,   468,   470,     0,     0,     0,     0,     0,
       0,     0,     0,   558,     0,     0,     0,     0,     0,     0,
     585,   586,   587,     0,     0,   528,   486,   492,     0,     0,
     488,     0,     0,   490,   496,   498,   500,   502,   504,   506,
     508,   510,   515,   571,   572,   574,   573,   477,   479,     0,
     575,   576,   483,   481,     0,   540,   541,   542,   543,     0,
       0,     0,     0,     0,     0,   653,     0,     1,     0,     0,
       0,   456,   458,   460,   462,   464,   584,   547,   399,   410,
     395,   411,   396,   397,   398,   401,   400,   402,   403,   404,
     405,   406,   407,   408,   409,   388,   387,   414,   417,   415,
     418,   413,   416,   293,   412,   419,   420,   391,   384,   386,
     392,   385,   382,   383,   380,   381,   422,   423,   424,   425,
     426,   427,   428,   429,   430,   438,   431,   432,   433,   437,
     434,   435,   436,   439,   440,   393,   441,   442,   443,   444,
     445,   379,   390,     0,   389,   289,   290,   394,   291,   292,
     421,   689,   692,   690,   687,   688,   691,   588,   685,   686,
     453,   590,   474,   697,   700,   698,   612,   696,   699,   614,
     694,   695,   467,   616,   531,     0,     0,     0,     0,     0,
       0,     0,   566,   567,   568,   560,   562,   559,   561,   563,
     564,   565,     0,   552,   548,   549,     0,   553,   554,   555,
     556,   557,     0,     0,   569,   570,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   655,   655,     0,     0,     0,
     655,   494,   495,   655,   655,   655,   655,   655,   655,   655,
     655,   655,   655,     0,     0,   663,   666,   664,   660,   661,
     665,   662,   658,   659,   447,     0,     0,     0,   455,   450,
      14,    16,    19,    11,     0,     9,    92,   596,   598,   469,
     600,   471,     0,     0,     0,     0,     0,   583,     0,     0,
     653,   657,   655,   653,     0,   657,   655,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   551,   550,     0,
       0,   545,   527,   534,     0,     0,     0,     0,   538,   742,
     739,   740,   743,   741,   737,   738,   529,     0,     0,   514,
     512,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   735,     0,   733,   734,     0,   653,   655,
       0,     0,   485,   655,   476,   652,    97,     0,   657,   655,
     457,   459,   461,   463,   465,   472,     0,    74,    29,    31,
      32,    34,    33,    35,     0,     0,     0,     0,   729,   730,
     624,   726,   727,   620,   728,   725,   618,   613,   622,     0,
       0,   532,     0,     0,   524,   518,   520,   523,   521,   517,
     526,   525,   544,   536,   539,   535,   537,     0,   673,   675,
     487,   670,   671,   674,   672,   668,   669,   654,   493,   513,
     489,   491,   497,   499,   501,   503,   505,   507,   509,   511,
     516,   653,   478,   732,   480,     0,     0,   484,   482,     0,
       0,   227,   706,   708,   606,   703,   704,   707,   705,   702,
     602,   597,   604,     0,     0,     0,     0,     0,     0,    37,
     693,   592,   683,   595,   679,   680,   682,   681,   677,   678,
     589,   656,   591,   701,   653,     0,     0,   655,   615,   617,
       0,   519,   522,     0,   653,     0,   667,   448,   451,     0,
      99,     0,   226,   109,   653,     0,   655,   599,   601,    39,
      30,     0,    42,    36,     0,     0,   653,     0,   646,   642,
     621,   644,   722,   723,   631,   720,   625,   627,   721,   718,
     719,   619,   629,     0,     0,   530,     0,   736,     0,    96,
       0,     0,     0,   714,   716,   611,   712,   607,   715,   713,
     710,   711,   603,   609,     0,    75,     0,     0,    41,   593,
       0,   731,     0,   655,   653,     0,     0,   655,   623,   533,
     676,    98,   100,     0,     0,    93,     0,   111,   113,     0,
       0,     0,     0,   143,   118,   121,   125,   128,   132,   135,
       0,   108,   709,   653,     0,   655,   605,   227,    46,     0,
       0,     0,    43,    45,    48,     0,    38,   655,   684,   651,
     647,   643,   649,     0,     0,   626,   636,   632,   628,   634,
       0,   145,   340,   343,   341,   344,   342,   160,   338,   339,
       0,   158,   159,   335,   336,   337,   293,   295,   296,    28,
     110,     0,   229,   229,   229,   229,     0,   294,   297,   298,
     299,   231,     0,   155,     0,   153,     0,     0,   152,   141,
       0,   243,   243,   255,   255,   259,   259,     0,     0,   608,
       0,    78,   374,   372,   373,   368,   369,   375,   376,   370,
     371,   365,     0,   274,   275,   366,   367,    54,    57,    52,
       0,     0,     0,    40,     0,     0,   655,   645,   724,     0,
     655,   630,   239,     0,   157,   116,     0,   228,    25,    26,
      24,    22,    23,     0,     0,   234,   239,   115,   243,   140,
       0,   151,     0,     0,     0,     0,     0,     0,     0,     0,
     139,   717,   610,     0,     0,   276,   239,   174,     0,   247,
       0,    44,    47,   594,   648,     0,   641,   637,   633,   639,
       0,     0,   171,   172,   176,   101,   165,   162,     0,     0,
     175,    27,    30,    94,    95,   112,   230,   233,   114,   232,
     231,     0,   154,   103,   117,   229,   146,     0,   144,   119,
     240,   242,   122,   321,   323,   322,   324,   320,   126,   252,
     318,   319,   254,   315,   316,   317,   129,   311,   133,   312,
     314,   310,   313,   256,   308,   309,   258,   305,   306,   307,
     136,     0,    76,     0,    81,     0,    77,    49,     0,     0,
       0,     0,    53,    50,   650,     0,   655,   635,   236,   161,
     238,     0,   170,   164,   181,   168,   163,     0,   174,     0,
     156,   102,   295,   296,   147,   148,     0,   294,   297,   298,
     299,   142,     0,   239,   263,     0,   239,   263,     0,   239,
     263,    83,     0,   247,     0,   280,     0,    55,     0,   173,
      60,    63,    58,   352,   355,   353,   356,   354,   244,   350,
     351,   347,   346,   246,   345,   348,   349,   638,     0,   239,
       0,   179,     0,   229,   167,     0,   235,     0,     0,   267,
     377,   378,   120,     0,   264,   273,   266,     0,     0,   127,
       0,     0,   134,     0,     0,     0,     0,     0,   278,   277,
      56,   239,     0,    51,   251,   239,   640,     0,   177,     0,
     187,   188,   184,   191,   192,     0,     0,     0,     0,     0,
       0,     0,   202,   204,   203,     0,     0,   207,   208,   209,
     214,     0,     0,   189,   183,   169,     0,   149,   150,   239,
     269,   377,   301,   302,   378,   104,   272,   265,   300,   303,
     304,   271,   241,   331,   334,   332,   123,   330,   333,   260,
     328,   329,   262,   325,   326,   327,   253,   130,   257,   137,
      85,    79,   229,    86,    89,    82,    80,     0,     0,     0,
       0,     0,     0,     0,    65,    59,     0,   237,   178,   185,
     186,   193,   194,   210,   198,   200,   205,   211,     0,   196,
     180,   182,     0,   166,     0,   239,     0,   239,     0,     0,
      84,   174,     0,   279,    61,    66,    67,    71,    69,    62,
      64,   248,   362,   363,   359,   358,   250,   357,   360,   364,
     361,   245,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   218,     0,   287,   286,   190,   281,   283,   284,   285,
     268,     0,   124,     0,   131,   138,     0,    90,     0,     0,
       0,   239,   216,     0,     0,     0,     0,     0,   219,     0,
       0,   213,   217,     0,   239,   282,   270,   261,     0,     0,
       0,     0,     0,   195,   215,   199,   201,   206,   212,   220,
     222,   224,   197,     0,     0,    68,    72,    70,   249,   239,
     239,   239,   288,    87,     0,     0,     0,     0,   221,   223,
     225,    88
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    36,    37,    38,   106,   104,   101,   102,   103,   650,
     387,   388,   389,   390,   391,   524,   392,   567,   523,   566,
     701,   612,   613,   614,   698,   749,   746,   748,   881,   934,
     931,   933,  1013,  1014,  1088,  1090,  1089,    39,    47,   477,
     607,   743,   826,   873,  1001,  1005,  1041,  1137,  1042,    40,
     108,   376,   775,   461,   509,   510,   842,   784,   912,    41,
     551,   601,   661,   662,   671,   862,   672,   864,  1036,   673,
     865,   674,   867,  1038,   675,   868,   676,   870,  1039,   732,
     670,   787,   854,   855,   667,   664,   728,   726,   640,   641,
     712,   715,   767,   846,   843,   768,   830,   844,   841,   900,
     903,   962,   963,   964,  1032,  1062,  1072,  1063,  1064,  1065,
    1066,  1093,  1070,  1071,  1129,  1130,  1131,   513,   718,   723,
     724,   780,   761,   899,   734,   863,   832,   935,  1015,  1091,
     736,   866,   738,   869,   918,  1037,   913,   914,   969,  1035,
     770,   693,   828,   876,  1007,  1075,  1076,  1104,   840,   915,
     976,   816,   802,   992,   642,   893,  1056,   694,   916,   184,
     185,   186,   187,   188,   189,   190,    42,    89,   369,   100,
     373,    48,    99,   302,   303,   304,   305,   306,    50,   109,
     110,    49,   105,   273,   274,   286,   285,   255,   260,   263,
     256,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     254,   427,   317,   500,    46,   242,   232,   246,    87,    88,
      92,    43,    44,    45,   308,    65,   200,   311,   312,   490,
     525,   617,   299,   377,   378,   379,   471,   515,   516,   562,
     604,   605,   212,   314,   315,   316,   407,   496,   495,   497,
     541,   575,   576,   577,   628,   709,   710,   758,   835,   836,
     530,   572,   573,   621,   705,   706,   294,   347,   395,   375,
     368,   437,   504,   491,   526,   201,   310,   300,   313,   472,
     514,   563,   603,   542,   574,   408,   494,   364,   365,   451,
     366
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -973
static const short int yypact[] =
{
     816,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  6106,  6068,    58,  -973,   115,   201,     4,  -973,
     -57,   -44,   -32,   -22,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,   105,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,   229,    18,  4430,   348,   348,
     412,    56,   727,   646,   628,   109,   253,   253,   253,    -2,
    -973,  -973,  -973,    88,   253,  -973,  -973,  -973,   116,   -29,
    -973,    80,    93,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  6203,
    -973,  -973,  -973,  -973,    78,  -973,  -973,  -973,  -973,   348,
    6203,   224,   235,   239,   244,  -973,   249,  -973,  4430,  6212,
    6212,  -973,  -973,  -973,  -973,  -973,   175,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,   153,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,   243,   253,   253,   253,   253,
     253,   253,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,   253,  -973,  -973,  -973,   255,  -973,  -973,  -973,
    -973,  -973,   253,   253,  -973,  -973,   149,   162,   165,   253,
     253,   253,   253,   179,  6944,  -973,  -973,   190,   202,   242,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  6899,  6899,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  6899,  6899,   204,  -973,  -973,
    -973,  -973,  -973,  -973,   726,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,   348,  6212,   412,   412,  6212,  -973,   209,  2679,
    -973,  -973,  -973,  -973,  6221,  -973,  -973,  6944,   253,   253,
     213,   219,   225,   227,   236,   246,   256,  -973,  -973,   258,
     259,  -973,  -973,  -973,   261,   267,   269,   271,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  6266,  6275,  -973,
    -973,   273,  6284,  6293,  6338,  6347,  6356,  6365,  6410,  6419,
    6428,  6437,  6482,  -973,  6491,  -973,  -973,  6500,  -973,  -973,
    6509,  6554,  -973,  -973,  -973,  -973,   264,  6563,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  2679,   215,   266,   270,
    -973,  -973,  -973,  -973,   739,  6572,  6581,   819,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  6572,
    6626,  -973,   278,   280,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,   253,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  5489,  6635,  -973,  -973,  6644,
    4430,   263,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  6572,  6653,   -34,  2679,   276,   286,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  6698,  6707,  -973,  -973,  -973,
     253,  -973,  -973,   288,  -973,  5976,  -973,  -973,  -973,    21,
    -973,   282,  -973,  -973,  -973,  6716,  -973,  -973,  -973,  -973,
    -973,  2679,  -973,  -973,  4430,  6203,  -973,  5985,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  6725,   293,  -973,  6122,  -973,  4430,  -973,
    2679,   817,  6131,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  6770,   215,    32,    40,  -973,  -973,
    6140,  -973,  6779,  -973,  -973,   348,  6788,  -973,  -973,  -973,
    -973,  -973,   215,   294,  4533,  -973,  2782,  -973,  -973,  2885,
    4533,  4945,  4430,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    2885,  -973,  -973,  -973,   348,  -973,  -973,   263,  -973,  5897,
    2988,  4430,  -973,   297,  -973,  4430,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  6797,  6149,  -973,  -973,  -973,  -973,  -973,
    6842,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    3091,  -973,  -973,  -973,  -973,  -973,   298,    20,   128,   125,
    -973,   277,    79,   171,   206,   307,   312,   223,   254,   295,
     300,  -973,  5897,  -973,   301,  -973,  3091,  4636,  -973,  -973,
    5897,  -973,  -973,  -973,  -973,  -973,  -973,   301,  6194,  -973,
    6851,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,   308,   303,  -973,  -973,  -973,  -973,   321,  -973,
     311,   322,    31,  -973,  6860,   348,  -973,  -973,  -973,  6869,
    -973,  -973,  -973,  5189,  -973,  -973,  2782,  -973,  -973,  -973,
    -973,  -973,  -973,  5257,   855,  -973,  -973,  -973,  -973,  -973,
    5325,  -973,   317,   855,  3194,  3297,  3400,  3503,  3606,  3709,
    -973,  -973,  -973,    45,  5897,  -973,  -973,  -973,   318,  -973,
    4430,  -973,  -973,  -973,  -973,  6914,  -973,  -973,  -973,  -973,
    6923,   928,  -973,  -973,  -973,  -973,  -973,  -973,  5325,  5632,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  1031,  4739,  -973,  -973,   307,  5529,   327,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,   328,  -973,  5897,  -973,  4945,  -973,  -973,  5897,  1134,
    5698,  3812,  5048,  -973,  -973,   348,  -973,  -973,  -973,  -973,
    -973,  5897,  -973,  -973,   332,  -973,  -973,  5325,  -973,  5764,
    -973,  -973,  -973,  -973,   325,  -973,   339,  -973,  -973,  -973,
    -973,  -973,  3915,  -973,  -973,  3915,  -973,  -973,  3915,  -973,
    -973,  -973,   335,  -973,  4842,   303,    74,  -973,  5831,  -973,
    -973,   343,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  6932,  -973,
     180,  -973,  1060,   307,  -973,  5698,  -973,  5529,  4430,  -973,
    -973,   542,  -973,  6078,  5963,  -973,  -973,  1237,  4018,  -973,
    1340,  4121,  -973,  1443,  4224,  5393,  4327,  5325,  -973,  -973,
    -973,  -973,    48,  -973,  -973,  -973,  -973,  1546,  -973,  5897,
    -973,  -973,  -973,  -973,  -973,   334,   340,   315,   359,   319,
     365,   366,  -973,   368,  -973,   369,   371,  -973,  -973,  -973,
    -973,   372,   965,  -973,  -973,  -973,  5325,  -973,  -973,  -973,
    -973,   627,  -973,  -973,   658,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,   307,  -973,  -973,  -973,  -973,  5897,  1649,   333,
     373,   375,   377,    87,  -973,  5120,  1752,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,   203,  -973,
    -973,  -973,  5461,  -973,  1855,  -973,  3915,  -973,  3915,  3915,
    -973,  -973,  3915,   303,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,   338,   338,   338,   338,   494,  4430,  4430,  4430,
      13,  -973,   338,  -973,  -973,  5461,  -973,  -973,  -973,  -973,
    -973,  1958,  -973,  2061,  -973,  -973,  5698,  -973,   338,   497,
     338,  -973,  -973,   -66,   -58,   -51,   -45,   391,   398,   400,
     403,  -973,  -973,   -42,  -973,  -973,  -973,  -973,  5831,   -41,
     405,   -39,  2164,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  2267,   395,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  2370,  2473,  2576,  3915,  -973,  -973,
    -973,  -973
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -188,
    -384,  -973,  -973,  -555,  -544,  -973,  -531,  -973,  -973,  -973,
    -973,  -973,  -973,  -168,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -475,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -681,  -973,  -973,    -8,  -692,  -386,  -761,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -363,  -277,   -46,  -973,  -973,   -35,  -573,
    -973,  -590,  -973,  -973,  -802,  -765,  -826,  -674,  -973,  -973,
    -973,  -973,  -973,  -413,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -503,  -973,  -512,  -973,  -973,  -973,   -33,  -630,  -215,
    -104,  -973,  -353,  -973,  -628,  -973,  -298,  -973,  -973,  -973,
     -98,  -973,   -99,  -973,  -782,  -973,  -973,  -973,  -973,  -973,
    -425,  -801,  -973,  -973,  -973,  -973,  -495,  -973,   -47,  -581,
    -973,  -973,  -973,  -973,  -559,  -973,  -973,  -973,  -580,  -445,
    -543,  -556,  -972,  -578,  -518,  -218,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,   383,   567,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,   -48,  -973,  -973,  -372,
    -973,  -973,   -81,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,    38,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,  -973,
    -973,  -973,  -973,  -973,  -973,  -973,    53,  -253,  -266,   -80,
    -973,  -973,  -973,  -973,  -973,  -973,  -973,   -10,  -973,  -459,
    -973,  -546,  -973,  -973,  -973,  -973,  -973,  -203,   -54,  -973,
    -204
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -583
static const short int yytable[] =
{
     183,   202,   475,   348,   847,   655,   656,   352,   659,   284,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     289,   766,   905,   719,   720,   721,   622,   875,   644,   301,
     658,   652,   668,   608,   644,   644,   531,   498,   783,   769,
     213,   643,   653,   778,   735,   904,   821,   643,   643,   409,
     346,   288,   788,   695,  1113,   654,   785,   258,   249,   396,
    1078,   296,  1115,   410,    95,   878,   645,   714,   660,  1116,
      94,   367,   645,   645,   727,  1117,   729,    96,  1122,  1125,
     250,  1127,   370,   371,   644,   921,   519,   740,   924,    97,
      83,    84,   520,   714,  1114,   848,   476,   643,   259,    98,
     782,   517,  1114,  1078,   919,   107,   695,   922,   731,  1114,
     644,   644,   473,   411,   695,  1114,   456,   629,  1114,  1114,
     459,  1114,   645,   643,   643,   214,   474,    85,    86,   609,
     609,   610,   610,  1101,   243,   655,   656,   565,   659,   117,
     966,   657,   822,   823,   251,   824,   548,   825,   645,   645,
    -391,  -391,  -391,   257,   549,   851,   879,   695,   611,   611,
     658,   652,   252,   759,  1033,   615,   582,   695,   695,  1067,
    1068,  1069,   653,   616,   695,   791,   791,   695,   644,   644,
     804,   804,   818,   818,   692,   654,    90,    91,   695,  1009,
    1010,   643,   643,   803,   803,   817,   817,  1011,   660,   928,
     717,   261,   651,  1012,   930,   856,  1043,   929,   859,   -32,
     -32,  1049,   695,   695,   262,  1086,   645,   645,   805,   805,
     819,   819,   381,   791,   287,   384,   644,   290,  1009,  1010,
     858,   879,   975,    83,    84,   783,  1011,   725,   291,   643,
     111,   112,  1012,   292,   543,   725,  -378,   293,   113,   114,
     115,   116,   295,   785,   380,  -390,  -390,  -390,  -392,  -392,
    -392,   307,   393,   564,   645,   309,   668,   695,   860,   644,
     331,   657,   695,   965,   695,  1082,   895,  1084,  1085,   244,
     245,  1087,   643,   332,   859,   695,   333,   859,   318,   319,
     859,   695,   717,   695,   213,   213,   327,   328,   776,   779,
     338,   -34,   -34,   -34,   938,   939,   858,   645,   779,   858,
     453,   349,   858,   453,   896,   731,   453,   453,   644,   827,
     623,  1108,   695,   350,   630,   372,   856,   717,   351,   859,
     385,   643,   772,   977,   414,   979,   -33,   -33,   -33,   393,
     415,   857,   382,   383,   860,   476,   416,   860,   417,   695,
     860,   858,   680,  -389,  -389,  -389,   645,   418,   981,  1067,
    1068,  1069,   994,   394,   704,   994,   397,   419,   994,   695,
     895,   695,  1040,   781,   512,   993,  1141,   420,   993,   421,
     422,   993,   423,   695,  -290,  -290,  -290,   894,   424,   860,
     425,   696,   426,   829,   439,   980,   460,   478,   872,   501,
     995,   502,   479,   995,   521,   522,   995,   716,   896,   545,
     695,   550,   879,   511,   579,   631,   901,   857,   713,  -377,
     857,   455,   702,   857,   776,  -291,  -291,  -291,   717,   393,
    -292,  -292,  -292,   722,  1124,   745,   744,  1059,   747,   750,
     247,   248,   751,   786,   696,   569,   831,   253,   861,   871,
     907,   695,   696,   755,  1079,   902,   908,   760,   859,  1058,
     859,   859,   857,   925,   859,   191,   932,   192,   978,   193,
    1019,   194,   195,   196,   393,  1021,  1020,   568,  1022,  1023,
     858,   894,   858,   858,  1024,  1025,   858,  1026,  1027,  1077,
    1028,  1029,  1046,  1045,  1047,   696,  1048,  1079,  1092,  1097,
    1002,   511,  1110,   393,   505,   696,   696,   197,   198,   199,
     917,  1118,   696,   920,  1018,   696,   923,  1119,   860,  1120,
     860,   860,  1121,  1133,   860,  1126,   696,   625,   771,   203,
     695,   204,  1077,   205,   752,   206,   207,   208,  1050,   393,
     581,  1006,   665,  -105,   967,   669,   937,   527,   874,  1031,
     696,   696,   695,   665,   677,   666,   679,   546,  1102,   859,
    1094,  1095,  1096,   699,   700,   849,   733,   552,   703,  1103,
    1057,   209,   210,   211,   681,   926,   737,   739,  1008,   570,
    1105,   858,  1016,   898,    93,  1109,     0,  1111,     0,     0,
       0,   857,     0,   857,   857,     0,     0,   857,  -105,   320,
     321,   322,   323,   324,   325,   696,     0,     0,     0,     0,
     696,     0,   696,     0,     0,   326,  1034,     0,     0,   860,
       0,     0,     0,   696,     0,   329,   330,   624,   -94,   696,
       0,   696,   334,   335,   336,   337,     0,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,     0,     0,     0,   678,   754,   222,   -95,
     696,     0,     0,     0,     0,   233,   234,   235,   236,   393,
       0,     0,     0,     0,     0,   223,   224,   225,   226,     0,
     227,   228,  1081,   -94,  1083,     0,     0,   696,     0,     0,
       0,     0,   857,     0,     0,  -105,     0,     0,     0,   237,
       0,   412,   413,   833,   229,   230,   231,   696,     0,   696,
       0,   238,   239,   240,   -95,     0,     0,     0,     0,     0,
       0,   696,   -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,
     -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,  1112,     0,
     215,     0,     0,     0,     0,     0,     0,     0,   696,     0,
       0,  1123,     0,   -95,   -95,   -95,   -95,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,     0,
     216,     0,   217,   218,   219,   220,  1134,  1135,  1136,     0,
     -94,     0,     0,   241,   882,     0,     0,   897,     0,   696,
     221,     0,     0,     0,     0,     0,     0,  1060,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     503,   -95,     0,     0,     0,     0,    -3,     1,   583,     0,
       0,     0,     2,     3,     4,     5,     0,  -582,  -582,     6,
       0,     7,     8,     9,    10,  -582,  -582,  -582,  -582,    11,
      12,    13,     0,   275,    14,   276,   374,   277,    15,   278,
     279,   280,    16,     0,     0,     0,   275,     0,   276,   480,
     277,   968,   278,   279,   280,    17,     0,     0,   696,     0,
       0,     0,     0,   584,     0,     0,     0,     0,     0,     0,
       0,    18,     0,   544,     0,   281,   282,   283,     0,     0,
     696,    19,    20,    21,    22,    23,    24,    25,   281,   282,
     283,     0,     0,    26,    27,    28,    29,    30,    31,    32,
      33,   118,   585,   586,     0,   587,   588,   589,   590,   591,
     592,   593,   594,   595,   596,   597,   598,   599,     0,     0,
       0,     0,     0,     0,     0,    34,   275,     0,   276,   493,
     277,     0,   278,   279,   280,     0,     0,     0,     0,     0,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   682,     0,     0,   683,
     600,   684,     0,   685,     0,    35,   773,   777,   281,   282,
     283,   686,   687,   688,   118,   689,   690,     0,     0,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,     0,   691,     0,   774,     0,     0,
    1098,  1099,  1100,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   838,   839,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   118,   181,  1030,
     182,     0,     0,     0,     0,     0,     0,     0,     0,   940,
     941,   942,   943,   944,   945,   946,   947,   948,   949,   950,
     951,   952,   953,   954,   955,   956,   957,   958,   959,   960,
     961,     0,     0,     0,     0,     0,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     838,   850,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     118,   181,     0,   182,   940,   941,   942,   943,   944,   945,
     946,   947,   948,   949,   950,   951,   952,   953,   954,   955,
     956,   957,   958,   959,   960,   961,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   838,   877,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   118,   181,     0,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   838,   982,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   118,   181,     0,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   838,
     996,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   118,
     181,     0,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   838,   998,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   118,   181,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   838,  1017,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   118,   181,     0,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   838,  1044,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   118,   181,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   838,  1061,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   118,   181,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   838,  1080,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   118,   181,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   838,  1106,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   118,   181,     0,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     838,  1107,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     118,   181,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   838,  1128,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   118,   181,     0,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   838,  1132,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   118,   181,     0,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   838,
    1138,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   118,
     181,     0,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   838,  1139,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   118,   181,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   838,  1140,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   118,   181,     0,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   386,     0,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   118,   181,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   386,     0,   646,   144,   145,   146,   647,   148,   149,
     648,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   118,   181,     0,   649,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   663,     0,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   118,   181,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   697,     0,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   118,   181,     0,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   632,   633,   634,   635,   713,   636,
     637,     0,   638,   639,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     118,   181,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   632,   633,   634,
     635,   789,   636,   790,     0,   638,   639,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   118,   181,     0,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     632,   633,   634,   635,   792,   636,   790,     0,   638,   639,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   118,   181,     0,   182,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   793,   794,   795,   796,   797,   798,   799,
       0,   800,   801,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   118,
     181,     0,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   793,   794,   795,   796,
     797,   806,   799,     0,   800,   801,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   118,   181,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   807,
     808,   809,   810,   811,   812,   813,     0,   814,   815,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   118,   181,     0,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   807,   820,   809,   810,   811,   812,   813,     0,
     814,   815,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   118,   181,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   880,     0,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   118,   181,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   909,     0,   910,   144,   145,   146,
     852,   148,   149,   853,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   118,   181,     0,   911,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   983,   984,   985,   986,   987,   988,   989,     0,   990,
     991,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   118,   181,     0,
     182,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   983,   984,   985,   997,   987,   988,
     989,     0,   990,   991,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     118,   181,     0,   182,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   983,   984,   985,
     999,   987,   988,   989,     0,   990,   991,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   118,   181,     0,   182,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     883,   884,   885,   886,  1003,   887,   888,     0,   889,   890,
     145,   146,   891,   148,   149,  1004,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   118,   181,     0,   892,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,     0,
       0,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   118,
     181,     0,   182,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   632,   633,   634,   635,
       0,   636,   637,     0,   638,   639,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   118,   181,     0,   182,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   632,
     633,   634,   635,   730,   636,     0,     0,   638,   639,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   118,   181,     0,   182,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   632,   633,   634,   635,     0,   636,   790,     0,
     638,   639,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   118,   181,
       0,   182,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   632,   633,   634,   635,   927,
     636,     0,     0,   638,   639,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   118,   181,     0,   182,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   632,   633,
     634,   635,     0,   636,     0,     0,   638,   639,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   118,   181,     0,   182,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   883,   884,   885,   886,     0,   887,   888,     0,   889,
     890,   145,   146,   891,   148,   149,   118,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,   181,     0,
     892,     0,     0,     0,     0,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,  1051,
       0,  1052,   144,     0,  1053,   118,   148,   149,  1054,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,     0,
     181,     0,  1055,     0,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     682,     0,   762,   683,   763,   684,     0,   685,     0,     0,
       0,     0,   764,   118,     0,   686,   687,   688,     0,   689,
     690,     0,     0,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,     0,   691,
       0,   765,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   682,     0,
       0,   683,     0,   684,     0,   685,     0,     0,   773,     0,
       0,   118,     0,   686,   687,   688,     0,   689,   690,     0,
       0,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,     0,   691,     0,   774,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   682,     0,     0,   683,
       0,   684,     0,   685,     0,     0,     0,     0,   764,   118,
       0,   686,   687,   688,     0,   689,   690,     0,     0,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,     0,   691,     0,   765,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   682,     0,     0,   683,     0,   684,
       0,   685,     0,     0,     0,     0,     0,   118,     0,   686,
     687,   688,     0,   689,   690,     0,     0,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,     0,   691,     0,  1000,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
    1073,     0,  1074,   144,     0,   118,   147,   148,   149,   150,
     151,   152,   153,   154,   155,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   275,     0,   276,   506,
     277,     0,   278,   279,   280,     0,     0,     0,     0,     0,
       0,   181,     0,   182,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   281,   282,
     283,   144,   145,   146,   852,   148,   149,   853,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   118,   181,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   682,     0,   762,   683,   763,   684,     0,
     685,     0,     0,   845,   118,   764,     0,     0,   686,   687,
     688,     0,   689,   690,     0,     0,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,     0,   691,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   682,
       0,   762,   683,   763,   684,     0,   685,     0,     0,     0,
     118,   764,     0,     0,   686,   687,   688,     0,   689,   690,
       0,     0,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,     0,   691,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   682,     0,     0,   683,     0,
     684,     0,   685,     0,     0,     0,   906,   118,     0,     0,
     686,   687,   688,     0,   689,   690,     0,     0,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,     0,   691,     0,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   682,     0,     0,   683,     0,   684,     0,   685,
       0,     0,     0,   118,   764,     0,     0,   686,   687,   688,
       0,   689,   690,     0,     0,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
       0,   691,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   682,     0,
       0,   683,     0,   684,     0,   685,     0,     0,     0,   118,
       0,     0,     0,   686,   687,   688,     0,   689,   690,     0,
       0,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,     0,   691,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,    66,     0,    67,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    68,     0,   275,     0,   276,   547,   277,     0,   278,
     279,   280,   275,     0,   276,   571,   277,     0,   278,   279,
     280,     0,     0,    51,     0,     0,   175,     0,    69,    70,
      71,    72,    73,    74,    75,    76,    77,    78,    79,    80,
      81,    52,     0,     0,    53,   281,   282,   283,     0,     0,
       0,     0,    54,     0,   281,   282,   283,     0,     0,    55,
      82,    56,     0,     0,     0,    57,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      58,     0,    59,    60,    61,    62,     0,     0,     0,    63,
       0,     0,     0,     0,     0,     0,     0,     0,    64,   135,
     136,   137,   138,   139,   140,   141,   142,   970,     0,   971,
     144,   145,   146,   972,   148,   149,   973,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,     0,   176,   177,   178,   179,   180,     0,   181,   275,
     974,   276,   580,   277,     0,   278,   279,   280,   275,     0,
     276,   602,   277,     0,   278,   279,   280,   275,     0,   276,
     618,   277,     0,   278,   279,   280,   275,     0,   276,   708,
     277,     0,   278,   279,   280,     0,     0,     0,     0,     0,
       0,   281,   282,   283,     0,     0,     0,     0,     0,     0,
     281,   282,   283,     0,     0,     0,     0,     0,     0,   281,
     282,   283,     0,     0,     0,     0,     0,     0,   281,   282,
     283,   275,     0,   276,   741,   277,     0,   278,   279,   280,
     275,     0,   276,     0,   277,     0,   278,   279,   280,   203,
       0,   204,     0,   205,     0,   297,   207,   208,   398,     0,
     399,     0,   400,     0,   401,   402,   403,     0,     0,     0,
       0,     0,     0,   281,   282,   283,     0,     0,     0,     0,
       0,     0,   281,   282,   283,     0,     0,     0,     0,     0,
       0,   298,   210,   211,     0,     0,     0,     0,     0,     0,
     404,   405,   406,   428,     0,   429,     0,   430,     0,   431,
     432,   433,   428,     0,   429,     0,   438,     0,   431,   432,
     433,   428,     0,   429,     0,   440,     0,   431,   432,   433,
     428,     0,   429,     0,   441,     0,   431,   432,   433,     0,
       0,     0,     0,     0,     0,   434,   435,   436,     0,     0,
       0,     0,     0,     0,   434,   435,   436,     0,     0,     0,
       0,     0,     0,   434,   435,   436,     0,     0,     0,     0,
       0,     0,   434,   435,   436,   428,     0,   429,     0,   442,
       0,   431,   432,   433,   428,     0,   429,     0,   443,     0,
     431,   432,   433,   428,     0,   429,     0,   444,     0,   431,
     432,   433,   428,     0,   429,     0,   445,     0,   431,   432,
     433,     0,     0,     0,     0,     0,     0,   434,   435,   436,
       0,     0,     0,     0,     0,     0,   434,   435,   436,     0,
       0,     0,     0,     0,     0,   434,   435,   436,     0,     0,
       0,     0,     0,     0,   434,   435,   436,   428,     0,   429,
       0,   446,     0,   431,   432,   433,   428,     0,   429,     0,
     447,     0,   431,   432,   433,   428,     0,   429,     0,   448,
       0,   431,   432,   433,   428,     0,   429,     0,   449,     0,
     431,   432,   433,     0,     0,     0,     0,     0,     0,   434,
     435,   436,     0,     0,     0,     0,     0,     0,   434,   435,
     436,     0,     0,     0,     0,     0,     0,   434,   435,   436,
       0,     0,     0,     0,     0,     0,   434,   435,   436,   428,
       0,   429,     0,   450,     0,   431,   432,   433,   339,     0,
     363,     0,   452,     0,   340,   341,   342,   339,     0,   363,
       0,   454,     0,   340,   341,   342,   339,     0,   363,     0,
     457,     0,   340,   341,   342,     0,     0,     0,     0,     0,
       0,   434,   435,   436,     0,     0,     0,     0,     0,     0,
     343,   344,   345,     0,     0,     0,     0,     0,     0,   343,
     344,   345,     0,     0,     0,     0,     0,     0,   343,   344,
     345,   339,     0,   363,     0,   458,     0,   340,   341,   342,
     462,     0,   463,     0,   464,     0,   465,   466,   467,   481,
       0,   482,     0,   483,     0,   484,   485,   486,   428,     0,
     429,     0,   492,     0,   431,   432,   433,     0,     0,     0,
       0,     0,     0,   343,   344,   345,     0,     0,     0,     0,
       0,     0,   468,   469,   470,     0,     0,     0,     0,     0,
       0,   487,   488,   489,     0,     0,     0,     0,     0,     0,
     434,   435,   436,   428,     0,   429,     0,   499,     0,   431,
     432,   433,   428,     0,   429,     0,   507,     0,   431,   432,
     433,   428,     0,   429,     0,   508,     0,   431,   432,   433,
     428,     0,   429,     0,   518,     0,   431,   432,   433,     0,
       0,     0,     0,     0,     0,   434,   435,   436,     0,     0,
       0,     0,     0,     0,   434,   435,   436,     0,     0,     0,
       0,     0,     0,   434,   435,   436,     0,     0,     0,     0,
       0,     0,   434,   435,   436,   462,     0,   463,     0,   528,
       0,   465,   466,   467,   532,     0,   533,     0,   534,     0,
     535,   536,   537,   553,     0,   554,     0,   555,     0,   556,
     557,   558,   428,     0,   429,     0,   578,     0,   431,   432,
     433,     0,     0,     0,     0,     0,     0,   468,   469,   529,
       0,     0,     0,     0,     0,     0,   538,   539,   540,     0,
       0,     0,     0,     0,     0,   559,   560,   561,     0,     0,
       0,     0,     0,     0,   434,   435,   436,   428,     0,   429,
       0,   606,     0,   431,   432,   433,   553,     0,   554,     0,
     619,     0,   556,   620,   558,   462,     0,   463,     0,   626,
       0,   465,   466,   467,   428,     0,   429,     0,   707,     0,
     431,   432,   433,     0,     0,     0,     0,     0,     0,   434,
     435,   436,     0,     0,     0,     0,     0,     0,   559,   560,
     561,     0,     0,     0,     0,     0,     0,   468,   469,   627,
       0,     0,     0,     0,     0,     0,   434,   435,   436,   428,
       0,   429,     0,   711,     0,   431,   432,   433,   428,     0,
     429,     0,   742,     0,   431,   432,   433,   428,     0,   429,
       0,   753,     0,   431,   432,   433,   553,     0,   554,     0,
     756,     0,   556,   757,   558,     0,     0,     0,     0,     0,
       0,   434,   435,   436,     0,     0,     0,     0,     0,     0,
     434,   435,   436,     0,     0,     0,   339,     0,   363,   434,
     435,   436,   340,   341,   342,     0,     0,     0,   559,   560,
     561,   428,     0,   429,     0,   834,     0,   431,   432,   433,
     428,     0,   429,     0,   837,     0,   431,   432,   433,   428,
       0,   429,     0,   936,     0,   431,   432,   433,   343,   344,
     345,   339,     0,     0,     0,     0,     0,   340,   341,   342,
       0,     0,     0,   434,   435,   436,     0,     0,     0,     0,
       0,     0,   434,   435,   436,     0,     0,     0,     0,     0,
       0,   434,   435,   436,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   343,   344,   345
};

static const short int yycheck[] =
{
      47,    49,   386,   256,   769,   586,   586,   260,   586,    89,
     263,   264,   265,   266,   267,   268,   269,   270,   271,   272,
     100,   713,   848,   653,   654,   655,   572,   828,   584,   110,
     586,   586,   591,     1,   590,   591,   495,   409,   730,   713,
      50,   584,   586,   724,   672,   847,     1,   590,   591,   315,
     254,    99,   733,   609,   120,   586,   730,    86,    60,   312,
    1032,   108,   120,   316,   121,   830,   584,   640,   586,   120,
      66,   274,   590,   591,   664,   120,   666,   121,   120,   120,
      82,   120,   285,   286,   640,   867,   120,   677,   870,   121,
      32,    33,   476,   666,   160,   769,   130,   640,   127,   121,
     728,   473,   160,  1075,   865,     0,   662,   868,   667,   160,
     666,   667,   378,   317,   670,   160,   369,   576,   160,   160,
     373,   160,   640,   666,   667,    69,   379,    69,    70,    98,
      98,   100,   100,   120,    25,   716,   716,   521,   716,   121,
     905,   586,    97,    98,   146,   100,   125,   102,   666,   667,
     130,   131,   132,    37,   133,   785,   830,   713,   127,   127,
     716,   716,    74,   709,   966,   125,   550,   723,   724,   156,
     157,   158,   716,   133,   730,   734,   735,   733,   734,   735,
     736,   737,   738,   739,   609,   716,    71,    72,   744,   141,
     142,   734,   735,   736,   737,   738,   739,   149,   716,   125,
     121,   121,   586,   155,   878,   786,  1007,   133,   786,   130,
     131,   124,   768,   769,   121,  1041,   734,   735,   736,   737,
     738,   739,   303,   782,   146,   306,   782,     3,   141,   142,
     786,   905,   913,    32,    33,   927,   149,   662,     3,   782,
      11,    12,   155,     4,   497,   670,   121,     3,    19,    20,
      21,    22,     3,   927,   302,   130,   131,   132,   130,   131,
     132,    86,   309,   516,   782,   112,   825,   823,   786,   825,
     121,   716,   828,   903,   830,  1036,   832,  1038,  1039,    26,
      27,  1042,   825,   121,   862,   841,   121,   865,    45,    46,
     868,   847,   121,   849,   304,   305,    41,    42,   723,   724,
     121,   130,   131,   132,   124,   125,   862,   825,   733,   865,
     364,   121,   868,   367,   832,   874,   370,   371,   874,   744,
     573,  1086,   878,   121,   577,   121,   907,   121,    86,   907,
     121,   874,   716,   913,   121,   913,   130,   131,   132,   386,
     121,   786,   304,   305,   862,   130,   121,   865,   121,   905,
     868,   907,   605,   130,   131,   132,   874,   121,   914,   156,
     157,   158,   918,   310,   617,   921,   313,   121,   924,   925,
     926,   927,  1002,   726,   111,   918,  1137,   121,   921,   121,
     121,   924,   121,   939,   130,   131,   132,   832,   121,   907,
     121,   609,   121,   746,   121,   913,   132,   131,   823,   121,
     918,   121,   132,   921,   128,   119,   924,   130,   926,   121,
     966,   129,  1086,   460,   121,   121,   841,   862,   117,   121,
     865,   368,   125,   868,   849,   130,   131,   132,   121,   476,
     130,   131,   132,   121,  1108,   132,   128,  1015,   117,   128,
      57,    58,   120,   126,   662,   525,   128,    64,   121,   121,
     125,  1007,   670,   706,  1032,   123,   117,   710,  1036,  1015,
    1038,  1039,   907,   128,  1042,   117,   123,   119,   913,   121,
     136,   123,   124,   125,   521,   160,   136,   524,   119,   160,
    1036,   926,  1038,  1039,   119,   119,  1042,   119,   119,  1032,
     119,   119,   119,   160,   119,   713,   119,  1075,   160,     5,
     925,   548,     5,   550,   451,   723,   724,   159,   160,   161,
     863,   120,   730,   866,   939,   733,   869,   119,  1036,   119,
    1038,  1039,   119,   128,  1042,   120,   744,   575,   716,   117,
    1086,   119,  1075,   121,   702,   123,   124,   125,  1013,   586,
     548,   927,   589,     1,   907,   592,   899,   494,   825,   962,
     768,   769,  1108,   600,   600,   590,   604,   504,  1070,  1137,
    1063,  1064,  1065,   610,   611,   780,   670,   514,   615,  1072,
    1015,   159,   160,   161,   607,   873,   674,   676,   931,   526,
    1075,  1137,   935,   836,    17,  1088,    -1,  1090,    -1,    -1,
      -1,  1036,    -1,  1038,  1039,    -1,    -1,  1042,    56,   216,
     217,   218,   219,   220,   221,   823,    -1,    -1,    -1,    -1,
     828,    -1,   830,    -1,    -1,   232,   969,    -1,    -1,  1137,
      -1,    -1,    -1,   841,    -1,   242,   243,   574,     1,   847,
      -1,   849,   249,   250,   251,   252,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,    -1,   603,   705,    12,     1,
     878,    -1,    -1,    -1,    -1,    37,    38,    39,    40,   716,
      -1,    -1,    -1,    -1,    -1,    29,    30,    31,    32,    -1,
      34,    35,  1035,    56,  1037,    -1,    -1,   905,    -1,    -1,
      -1,    -1,  1137,    -1,    -1,   153,    -1,    -1,    -1,    71,
      -1,   318,   319,   750,    58,    59,    60,   925,    -1,   927,
      -1,    83,    84,    85,    56,    -1,    -1,    -1,    -1,    -1,
      -1,   939,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,  1091,    -1,
      13,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   966,    -1,
      -1,  1104,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,    -1,
      43,    -1,    45,    46,    47,    48,  1129,  1130,  1131,    -1,
     153,    -1,    -1,   155,   831,    -1,    -1,   835,    -1,  1007,
      63,    -1,    -1,    -1,    -1,    -1,    -1,  1015,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     427,   153,    -1,    -1,    -1,    -1,     0,     1,     1,    -1,
      -1,    -1,     6,     7,     8,     9,    -1,    11,    12,    13,
      -1,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    -1,   117,    28,   119,   120,   121,    32,   123,
     124,   125,    36,    -1,    -1,    -1,   117,    -1,   119,   120,
     121,   908,   123,   124,   125,    49,    -1,    -1,  1086,    -1,
      -1,    -1,    -1,    56,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    65,    -1,   500,    -1,   159,   160,   161,    -1,    -1,
    1108,    75,    76,    77,    78,    79,    80,    81,   159,   160,
     161,    -1,    -1,    87,    88,    89,    90,    91,    92,    93,
      94,    56,    95,    96,    -1,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   119,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,    -1,   114,
     153,   116,    -1,   118,    -1,   159,   121,   122,   159,   160,
     161,   126,   127,   128,    56,   130,   131,    -1,    -1,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,   160,    -1,   162,    -1,    -1,
    1067,  1068,  1069,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    56,   160,   124,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      56,   160,    -1,   162,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    56,   160,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    56,   160,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    56,
     160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    56,   160,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    56,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    56,   160,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    56,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    56,   160,    -1,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    56,   160,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      56,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    56,   160,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    56,   160,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    56,
     160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    56,   160,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    56,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    56,   160,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    56,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    56,   160,    -1,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    56,   160,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      56,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    56,   160,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    56,   160,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    56,
     160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    56,   160,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    56,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    56,   160,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    56,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    56,   160,    -1,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    56,   160,    -1,
     162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    -1,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      56,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,    -1,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    56,   160,    -1,   162,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,    -1,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    56,   160,    -1,   162,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
      -1,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    56,
     160,    -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
      -1,   118,   119,    -1,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    56,   160,    -1,   162,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,    -1,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,    56,   160,    -1,   162,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,    -1,   118,   119,    -1,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    56,   160,
      -1,   162,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,    -1,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    56,   160,    -1,   162,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,    -1,   118,    -1,    -1,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    56,   160,    -1,   162,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,    -1,   118,   119,    -1,   121,
     122,   123,   124,   125,   126,   127,    56,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,   160,    -1,
     162,    -1,    -1,    -1,    -1,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,   121,   122,    -1,   124,    56,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,    -1,
     160,    -1,   162,    -1,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,    -1,   113,   114,   115,   116,    -1,   118,    -1,    -1,
      -1,    -1,   123,    56,    -1,   126,   127,   128,    -1,   130,
     131,    -1,    -1,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    -1,   160,
      -1,   162,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
      -1,   114,    -1,   116,    -1,   118,    -1,    -1,   121,    -1,
      -1,    56,    -1,   126,   127,   128,    -1,   130,   131,    -1,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,   160,    -1,   162,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,    -1,    -1,   114,
      -1,   116,    -1,   118,    -1,    -1,    -1,    -1,   123,    56,
      -1,   126,   127,   128,    -1,   130,   131,    -1,    -1,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,    -1,   160,    -1,   162,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,    -1,    -1,   114,    -1,   116,
      -1,   118,    -1,    -1,    -1,    -1,    -1,    56,    -1,   126,
     127,   128,    -1,   130,   131,    -1,    -1,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,    -1,   160,    -1,   162,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,    -1,   121,   122,    -1,    56,   125,   126,   127,   128,
     129,   130,   131,   132,   133,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,   160,    -1,   162,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   159,   160,
     161,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,    56,   160,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,    -1,   113,   114,   115,   116,    -1,
     118,    -1,    -1,   121,    56,   123,    -1,    -1,   126,   127,
     128,    -1,   130,   131,    -1,    -1,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,    -1,   160,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
      -1,   113,   114,   115,   116,    -1,   118,    -1,    -1,    -1,
      56,   123,    -1,    -1,   126,   127,   128,    -1,   130,   131,
      -1,    -1,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,    -1,   160,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,    -1,    -1,   114,    -1,
     116,    -1,   118,    -1,    -1,    -1,   122,    56,    -1,    -1,
     126,   127,   128,    -1,   130,   131,    -1,    -1,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,    -1,   160,    -1,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,    -1,    -1,   114,    -1,   116,    -1,   118,
      -1,    -1,    -1,    56,   123,    -1,    -1,   126,   127,   128,
      -1,   130,   131,    -1,    -1,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
      -1,   160,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,    -1,
      -1,   114,    -1,   116,    -1,   118,    -1,    -1,    -1,    56,
      -1,    -1,    -1,   126,   127,   128,    -1,   130,   131,    -1,
      -1,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,    -1,   160,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,     6,    -1,     8,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    23,    -1,   117,    -1,   119,   120,   121,    -1,   123,
     124,   125,   117,    -1,   119,   120,   121,    -1,   123,   124,
     125,    -1,    -1,     7,    -1,    -1,   153,    -1,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    59,    60,    61,
      62,    25,    -1,    -1,    28,   159,   160,   161,    -1,    -1,
      -1,    -1,    36,    -1,   159,   160,   161,    -1,    -1,    43,
      82,    45,    -1,    -1,    -1,    49,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      64,    -1,    66,    67,    68,    69,    -1,    -1,    -1,    73,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    82,   111,
     112,   113,   114,   115,   116,   117,   118,   119,    -1,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,    -1,   154,   155,   156,   157,   158,    -1,   160,   117,
     162,   119,   120,   121,    -1,   123,   124,   125,   117,    -1,
     119,   120,   121,    -1,   123,   124,   125,   117,    -1,   119,
     120,   121,    -1,   123,   124,   125,   117,    -1,   119,   120,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,
     161,   117,    -1,   119,   120,   121,    -1,   123,   124,   125,
     117,    -1,   119,    -1,   121,    -1,   123,   124,   125,   117,
      -1,   119,    -1,   121,    -1,   123,   124,   125,   117,    -1,
     119,    -1,   121,    -1,   123,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,   161,   117,    -1,   119,    -1,   121,    -1,   123,
     124,   125,   117,    -1,   119,    -1,   121,    -1,   123,   124,
     125,   117,    -1,   119,    -1,   121,    -1,   123,   124,   125,
     117,    -1,   119,    -1,   121,    -1,   123,   124,   125,    -1,
      -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   159,   160,   161,   117,    -1,   119,    -1,   121,
      -1,   123,   124,   125,   117,    -1,   119,    -1,   121,    -1,
     123,   124,   125,   117,    -1,   119,    -1,   121,    -1,   123,
     124,   125,   117,    -1,   119,    -1,   121,    -1,   123,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,   159,   160,   161,   117,    -1,   119,
      -1,   121,    -1,   123,   124,   125,   117,    -1,   119,    -1,
     121,    -1,   123,   124,   125,   117,    -1,   119,    -1,   121,
      -1,   123,   124,   125,   117,    -1,   119,    -1,   121,    -1,
     123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,   117,
      -1,   119,    -1,   121,    -1,   123,   124,   125,   117,    -1,
     119,    -1,   121,    -1,   123,   124,   125,   117,    -1,   119,
      -1,   121,    -1,   123,   124,   125,   117,    -1,   119,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,
     161,   117,    -1,   119,    -1,   121,    -1,   123,   124,   125,
     117,    -1,   119,    -1,   121,    -1,   123,   124,   125,   117,
      -1,   119,    -1,   121,    -1,   123,   124,   125,   117,    -1,
     119,    -1,   121,    -1,   123,   124,   125,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,   161,   117,    -1,   119,    -1,   121,    -1,   123,
     124,   125,   117,    -1,   119,    -1,   121,    -1,   123,   124,
     125,   117,    -1,   119,    -1,   121,    -1,   123,   124,   125,
     117,    -1,   119,    -1,   121,    -1,   123,   124,   125,    -1,
      -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   159,   160,   161,   117,    -1,   119,    -1,   121,
      -1,   123,   124,   125,   117,    -1,   119,    -1,   121,    -1,
     123,   124,   125,   117,    -1,   119,    -1,   121,    -1,   123,
     124,   125,   117,    -1,   119,    -1,   121,    -1,   123,   124,
     125,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,
      -1,    -1,    -1,    -1,    -1,   159,   160,   161,    -1,    -1,
      -1,    -1,    -1,    -1,   159,   160,   161,   117,    -1,   119,
      -1,   121,    -1,   123,   124,   125,   117,    -1,   119,    -1,
     121,    -1,   123,   124,   125,   117,    -1,   119,    -1,   121,
      -1,   123,   124,   125,   117,    -1,   119,    -1,   121,    -1,
     123,   124,   125,    -1,    -1,    -1,    -1,    -1,    -1,   159,
     160,   161,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,
     161,    -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,
      -1,    -1,    -1,    -1,    -1,    -1,   159,   160,   161,   117,
      -1,   119,    -1,   121,    -1,   123,   124,   125,   117,    -1,
     119,    -1,   121,    -1,   123,   124,   125,   117,    -1,   119,
      -1,   121,    -1,   123,   124,   125,   117,    -1,   119,    -1,
     121,    -1,   123,   124,   125,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
     159,   160,   161,    -1,    -1,    -1,   117,    -1,   119,   159,
     160,   161,   123,   124,   125,    -1,    -1,    -1,   159,   160,
     161,   117,    -1,   119,    -1,   121,    -1,   123,   124,   125,
     117,    -1,   119,    -1,   121,    -1,   123,   124,   125,   117,
      -1,   119,    -1,   121,    -1,   123,   124,   125,   159,   160,
     161,   117,    -1,    -1,    -1,    -1,    -1,   123,   124,   125,
      -1,    -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,
      -1,    -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,
      -1,   159,   160,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   159,   160,   161
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    15,    16,    17,
      18,    23,    24,    25,    28,    32,    36,    49,    65,    75,
      76,    77,    78,    79,    80,    81,    87,    88,    89,    90,
      91,    92,    93,    94,   119,   159,   164,   165,   166,   200,
     212,   222,   329,   374,   375,   376,   367,   201,   334,   344,
     341,     7,    25,    28,    36,    43,    45,    49,    64,    66,
      67,    68,    69,    73,    82,   378,     6,     8,    23,    50,
      51,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    82,    32,    33,    69,    70,   371,   372,   330,
      71,    72,   373,   371,    66,   121,   121,   121,   121,   335,
     332,   169,   170,   171,   168,   345,   167,     0,   213,   342,
     343,    11,    12,    19,    20,    21,    22,   121,    56,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   160,   162,   311,   322,   323,   324,   325,   326,   327,
     328,   117,   119,   121,   123,   124,   125,   159,   160,   161,
     379,   428,   379,   117,   119,   121,   123,   124,   125,   159,
     160,   161,   395,   430,    69,    13,    43,    45,    46,    47,
      48,    63,    12,    29,    30,    31,    32,    34,    35,    58,
      59,    60,   369,    37,    38,    39,    40,    71,    83,    84,
      85,   155,   368,    25,    26,    27,   370,   370,   370,    60,
      82,   146,    74,   370,   363,   350,   353,    37,    86,   127,
     351,   121,   121,   352,   354,   355,   356,   357,   358,   359,
     360,   361,   362,   346,   347,   117,   119,   121,   123,   124,
     125,   159,   160,   161,   422,   349,   348,   146,   379,   422,
       3,     3,     4,     3,   419,     3,   311,   123,   159,   385,
     430,   385,   336,   337,   338,   339,   340,    86,   377,   112,
     429,   380,   381,   431,   396,   397,   398,   365,    45,    46,
     370,   370,   370,   370,   370,   370,   370,    41,    42,   370,
     370,   121,   121,   121,   370,   370,   370,   370,   121,   117,
     123,   124,   125,   159,   160,   161,   443,   420,   420,   121,
     121,    86,   420,   420,   420,   420,   420,   420,   420,   420,
     420,   420,   420,   119,   440,   441,   443,   440,   423,   331,
     440,   440,   121,   333,   120,   422,   214,   386,   387,   388,
     379,   385,   395,   395,   385,   121,   119,   173,   174,   175,
     176,   177,   179,   311,   419,   421,   420,   419,   117,   119,
     121,   123,   124,   125,   159,   160,   161,   399,   438,   421,
     420,   443,   370,   370,   121,   121,   121,   121,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   364,   117,   119,
     121,   123,   124,   125,   159,   160,   161,   424,   121,   121,
     121,   121,   121,   121,   121,   121,   121,   121,   121,   121,
     121,   442,   121,   441,   121,   419,   420,   121,   121,   420,
     132,   216,   117,   119,   121,   123,   124,   125,   159,   160,
     161,   389,   432,   421,   420,   173,   130,   202,   131,   132,
     120,   117,   119,   121,   123,   124,   125,   159,   160,   161,
     382,   426,   121,   120,   439,   401,   400,   402,   382,   121,
     366,   121,   121,   370,   425,   419,   120,   121,   121,   217,
     218,   311,   111,   280,   433,   390,   391,   382,   121,   120,
     173,   128,   119,   181,   178,   383,   427,   419,   121,   161,
     413,   432,   117,   119,   121,   123,   124,   125,   159,   160,
     161,   403,   436,   420,   370,   121,   419,   120,   125,   133,
     129,   223,   419,   117,   119,   121,   123,   124,   125,   159,
     160,   161,   392,   434,   420,   173,   182,   180,   311,   422,
     419,   120,   414,   415,   437,   404,   405,   406,   121,   121,
     120,   218,   173,     1,    56,    95,    96,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     153,   224,   120,   435,   393,   394,   121,   203,     1,    98,
     100,   127,   184,   185,   186,   125,   133,   384,   120,   121,
     124,   416,   434,   420,   419,   379,   121,   161,   407,   432,
     420,   121,   113,   114,   115,   116,   118,   119,   121,   122,
     251,   252,   317,   323,   324,   327,   121,   125,   128,   162,
     172,   173,   176,   177,   179,   312,   321,   322,   324,   326,
     327,   225,   226,   119,   248,   311,   251,   247,   317,   311,
     243,   227,   229,   232,   234,   237,   239,   248,   419,   379,
     420,   280,   111,   114,   116,   118,   126,   127,   128,   130,
     131,   160,   303,   304,   320,   324,   328,   119,   187,   311,
     311,   183,   125,   311,   420,   417,   418,   121,   120,   408,
     409,   121,   253,   117,   252,   254,   130,   121,   281,   281,
     281,   281,   121,   282,   283,   303,   250,   254,   249,   254,
     117,   317,   242,   283,   287,   287,   293,   293,   295,   295,
     254,   120,   121,   204,   128,   132,   189,   117,   190,   188,
     128,   120,   186,   121,   379,   420,   121,   124,   410,   434,
     420,   285,   113,   115,   123,   162,   219,   255,   258,   260,
     303,   172,   173,   121,   162,   215,   303,   122,   215,   303,
     284,   285,   287,   219,   220,   260,   126,   244,   215,   117,
     119,   317,   117,   113,   114,   115,   116,   117,   118,   119,
     121,   122,   315,   323,   324,   327,   118,   113,   114,   115,
     116,   117,   118,   119,   121,   122,   314,   323,   324,   327,
     114,     1,    97,    98,   100,   102,   205,   303,   305,   285,
     259,   128,   289,   311,   121,   411,   412,   121,   119,   120,
     311,   261,   219,   257,   260,   121,   256,   258,   260,   282,
     120,   281,   125,   128,   245,   246,   312,   322,   324,   326,
     327,   121,   228,   288,   230,   233,   294,   235,   238,   296,
     240,   121,   303,   206,   247,   304,   306,   120,   258,   260,
     119,   191,   311,   113,   114,   115,   116,   118,   119,   121,
     122,   125,   162,   318,   322,   324,   327,   379,   420,   286,
     262,   303,   123,   263,   257,   259,   122,   125,   117,   119,
     121,   162,   221,   299,   300,   312,   321,   285,   297,   221,
     285,   297,   221,   285,   297,   128,   289,   117,   125,   133,
     260,   193,   123,   194,   192,   290,   121,   285,   124,   125,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   264,   265,   266,   281,   258,   246,   311,   301,
     119,   121,   125,   128,   162,   215,   313,   321,   322,   326,
     327,   324,   120,   113,   114,   115,   116,   117,   118,   119,
     121,   122,   316,   323,   324,   327,   120,   116,   120,   116,
     162,   207,   303,   117,   128,   208,   220,   307,   285,   141,
     142,   149,   155,   195,   196,   291,   285,   120,   303,   136,
     136,   160,   119,   160,   119,   119,   119,   119,   119,   119,
     124,   266,   267,   257,   285,   302,   231,   298,   236,   241,
     281,   209,   211,   304,   120,   160,   119,   119,   119,   124,
     196,   119,   121,   124,   128,   162,   319,   322,   324,   326,
     328,   120,   268,   270,   271,   272,   273,   156,   157,   158,
     275,   276,   269,   119,   121,   308,   309,   323,   325,   326,
     120,   285,   221,   285,   221,   221,   259,   221,   197,   199,
     198,   292,   160,   274,   274,   274,   274,     5,   311,   311,
     311,   120,   276,   274,   310,   309,   120,   120,   258,   274,
       5,   274,   285,   120,   160,   120,   120,   120,   120,   119,
     119,   119,   120,   285,   260,   120,   120,   120,   120,   277,
     278,   279,   120,   128,   285,   285,   285,   210,   120,   120,
     120,   221
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
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
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
    while (0)
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
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
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
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
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
      size_t yyn = 0;
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

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
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
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

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
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

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
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


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
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

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

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
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

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
#line 216 "surface.yy"
    { YYACCEPT; ;}
    break;

  case 3:
#line 218 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			;}
    break;

  case 8:
#line 232 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 9:
#line 234 "surface.yy"
    {
			  int lineNr = lineNumber;
			  eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			;}
    break;

  case 10:
#line 242 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 11:
#line 244 "surface.yy"
    {
			  int lineNr = lineNumber;
			  eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			;}
    break;

  case 12:
#line 253 "surface.yy"
    {
			  cout << directoryManager.getCwd() << '\n';
			;}
    break;

  case 13:
#line 256 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 14:
#line 258 "surface.yy"
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			;}
    break;

  case 15:
#line 267 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 16:
#line 269 "surface.yy"
    {
			  string directory;
			  directoryManager.realPath((yyvsp[0].yyString), directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			;}
    break;

  case 17:
#line 279 "surface.yy"
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
#line 289 "surface.yy"
    { lexerStringMode(); ;}
    break;

  case 19:
#line 291 "surface.yy"
    {
			  system((string("ls") + (yyvsp[0].yyString)).c_str());
			;}
    break;

  case 20:
#line 295 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			;}
    break;

  case 21:
#line 300 "surface.yy"
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			;}
    break;

  case 22:
#line 333 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        ;}
    break;

  case 23:
#line 337 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        ;}
    break;

  case 27:
#line 344 "surface.yy"
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			;}
    break;

  case 28:
#line 352 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  moduleExpressions.push(new ModuleExpression(t));
			;}
    break;

  case 30:
#line 363 "surface.yy"
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			;}
    break;

  case 35:
#line 379 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[0].yyToken)));
                        ;}
    break;

  case 36:
#line 386 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			;}
    break;

  case 37:
#line 394 "surface.yy"
    { clear(); ;}
    break;

  case 38:
#line 396 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, bubble));
			;}
    break;

  case 39:
#line 403 "surface.yy"
    {;}
    break;

  case 40:
#line 406 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 41:
#line 407 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 42:
#line 414 "surface.yy"
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			;}
    break;

  case 43:
#line 419 "surface.yy"
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			;}
    break;

  case 49:
#line 434 "surface.yy"
    {
			  currentRenaming->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			;}
    break;

  case 50:
#line 438 "surface.yy"
    {
			  currentRenaming->addLabelMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			;}
    break;

  case 51:
#line 441 "surface.yy"
    {;}
    break;

  case 52:
#line 444 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 53:
#line 446 "surface.yy"
    {
			  currentRenaming->addOpMapping(bubble);
			;}
    break;

  case 54:
#line 449 "surface.yy"
    { clear(); ;}
    break;

  case 55:
#line 451 "surface.yy"
    {
			  currentRenaming->addOpMapping(bubble);
			;}
    break;

  case 56:
#line 456 "surface.yy"
    {;}
    break;

  case 58:
#line 460 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 59:
#line 461 "surface.yy"
    { currentRenaming->addOpTarget(bubble); ;}
    break;

  case 60:
#line 462 "surface.yy"
    { clear(); ;}
    break;

  case 61:
#line 463 "surface.yy"
    { currentRenaming->addOpTarget(bubble); ;}
    break;

  case 62:
#line 466 "surface.yy"
    {;}
    break;

  case 66:
#line 474 "surface.yy"
    { currentRenaming->setPrec((yyvsp[0].yyToken)); ;}
    break;

  case 67:
#line 475 "surface.yy"
    { clear(); ;}
    break;

  case 68:
#line 476 "surface.yy"
    { currentRenaming->setGather(bubble); ;}
    break;

  case 69:
#line 477 "surface.yy"
    { clear(); ;}
    break;

  case 70:
#line 478 "surface.yy"
    { currentRenaming->setFormat(bubble); ;}
    break;

  case 71:
#line 479 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 72:
#line 480 "surface.yy"
    { currentRenaming->setLatexMacro((yyvsp[-1].yyString)); ;}
    break;

  case 73:
#line 486 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 74:
#line 488 "surface.yy"
    {
			  fileTable.beginModule((yyvsp[-4].yyToken), (yyvsp[-2].yyToken));
			  interpreter.setCurrentView(new View((yyvsp[-2].yyToken)));
			  currentSyntaxContainer = CV;
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			;}
    break;

  case 75:
#line 496 "surface.yy"
    {
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			;}
    break;

  case 76:
#line 501 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[-9].yyToken)).code(), CV);
			  CV->finishView();
			;}
    break;

  case 79:
#line 514 "surface.yy"
    {
			  CV->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			;}
    break;

  case 80:
#line 517 "surface.yy"
    {;}
    break;

  case 81:
#line 518 "surface.yy"
    { clear(); ;}
    break;

  case 84:
#line 523 "surface.yy"
    { (yyval.yyToken) = (yyvsp[-1].yyToken); ;}
    break;

  case 85:
#line 525 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  (yyval.yyToken) = t;
			;}
    break;

  case 86:
#line 535 "surface.yy"
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(bubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(bubble);
			;}
    break;

  case 87:
#line 543 "surface.yy"
    {
			  clear();
			;}
    break;

  case 88:
#line 547 "surface.yy"
    {
			  Token::peelParens(bubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(bubble);
			;}
    break;

  case 89:
#line 552 "surface.yy"
    {
			  //
			  //	At this point we don't know if we have an op->term mapping
			  //	or a generic op->op mapping so we save the from description and
			  //	press on.
			  //
			  opDescription = bubble;
			  clear();
			;}
    break;

  case 90:
#line 562 "surface.yy"
    {
			  if (bubble[0].code() == Token::encode("term"))
			    {
			      //
			      //	Op->term mapping.
			      //
			      CV->addOpTermMapping(opDescription, bubble);
			    }
			  else
			    {
			      //
			      //	Generic op->op mapping.
			      //
			      Token::peelParens(opDescription);  // remove any enclosing parens from op name
			      CV->addOpMapping(opDescription);
			      Token::peelParens(bubble);  // remove any enclosing parens from op name
			      CV->addOpTarget(bubble);
			    }
			;}
    break;

  case 91:
#line 586 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 92:
#line 588 "surface.yy"
    {
			  interpreter.setCurrentModule(new PreModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken)));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			;}
    break;

  case 93:
#line 594 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[0].yyToken));
			;}
    break;

  case 94:
#line 601 "surface.yy"
    {;}
    break;

  case 95:
#line 603 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  store(t);
			;}
    break;

  case 96:
#line 612 "surface.yy"
    {;}
    break;

  case 100:
#line 621 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter((yyvsp[-2].yyToken), me);
			;}
    break;

  case 101:
#line 629 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  clear();
			  store(t);
			  currentSyntaxContainer->addType(false, bubble);
			;}
    break;

  case 105:
#line 646 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  store(t);
			;}
    break;

  case 110:
#line 663 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport((yyvsp[-1].yyToken), me);
			;}
    break;

  case 111:
#line 669 "surface.yy"
    { clear(); ;}
    break;

  case 112:
#line 670 "surface.yy"
    { CM->addSortDecl(bubble); ;}
    break;

  case 113:
#line 672 "surface.yy"
    { clear(); ;}
    break;

  case 114:
#line 673 "surface.yy"
    { CM->addSubsortDecl(bubble); ;}
    break;

  case 115:
#line 675 "surface.yy"
    {;}
    break;

  case 116:
#line 677 "surface.yy"
    {;}
    break;

  case 117:
#line 679 "surface.yy"
    {;}
    break;

  case 118:
#line 681 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 119:
#line 682 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 120:
#line 683 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 121:
#line 685 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 122:
#line 686 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 123:
#line 687 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 124:
#line 688 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 125:
#line 690 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 126:
#line 691 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 127:
#line 692 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 128:
#line 694 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 129:
#line 695 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 130:
#line 696 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 131:
#line 697 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 132:
#line 699 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 133:
#line 700 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 134:
#line 701 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 135:
#line 703 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 136:
#line 704 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 137:
#line 705 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 138:
#line 706 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 139:
#line 709 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 140:
#line 714 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 141:
#line 719 "surface.yy"
    {
			;}
    break;

  case 142:
#line 722 "surface.yy"
    {
			;}
    break;

  case 143:
#line 725 "surface.yy"
    { clear(); ;}
    break;

  case 144:
#line 726 "surface.yy"
    { CM->addSubsortDecl(bubble); ;}
    break;

  case 145:
#line 729 "surface.yy"
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			;}
    break;

  case 146:
#line 739 "surface.yy"
    {;}
    break;

  case 147:
#line 740 "surface.yy"
    {;}
    break;

  case 150:
#line 748 "surface.yy"
    {
			;}
    break;

  case 151:
#line 752 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); ;}
    break;

  case 152:
#line 753 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); ;}
    break;

  case 153:
#line 756 "surface.yy"
    { clear(); store((yyvsp[0].yyToken)); ;}
    break;

  case 154:
#line 757 "surface.yy"
    { CM->addOpDecl(bubble); ;}
    break;

  case 155:
#line 758 "surface.yy"
    { clear(); ;}
    break;

  case 156:
#line 759 "surface.yy"
    { CM->addOpDecl(bubble); ;}
    break;

  case 159:
#line 767 "surface.yy"
    {
			  clear();
			  store((yyvsp[0].yyToken));
			  CM->addOpDecl(bubble);
			;}
    break;

  case 160:
#line 772 "surface.yy"
    { clear(); ;}
    break;

  case 161:
#line 773 "surface.yy"
    { CM->addOpDecl(bubble); ;}
    break;

  case 162:
#line 776 "surface.yy"
    {;}
    break;

  case 164:
#line 782 "surface.yy"
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			;}
    break;

  case 165:
#line 787 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			;}
    break;

  case 166:
#line 795 "surface.yy"
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			;}
    break;

  case 167:
#line 800 "surface.yy"
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			;}
    break;

  case 168:
#line 805 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in constant declaration.");
			;}
    break;

  case 171:
#line 815 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 172:
#line 816 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 175:
#line 824 "surface.yy"
    {
			  clear();
			  store((yyvsp[0].yyToken));
			  currentSyntaxContainer->addType(false, bubble);
			;}
    break;

  case 176:
#line 829 "surface.yy"
    { clear(); ;}
    break;

  case 177:
#line 831 "surface.yy"
    {
			  currentSyntaxContainer->addType(true, bubble);
			;}
    break;

  case 178:
#line 836 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 179:
#line 837 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 180:
#line 840 "surface.yy"
    {;}
    break;

  case 184:
#line 849 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			;}
    break;

  case 185:
#line 853 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			;}
    break;

  case 186:
#line 857 "surface.yy"
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			;}
    break;

  case 187:
#line 863 "surface.yy"
    {
			  CM->setFlag(SymbolType::ASSOC);
			;}
    break;

  case 188:
#line 867 "surface.yy"
    {
			  CM->setFlag(SymbolType::COMM);
			;}
    break;

  case 189:
#line 870 "surface.yy"
    { clear(); ;}
    break;

  case 190:
#line 871 "surface.yy"
    { CM->setIdentity(bubble); ;}
    break;

  case 191:
#line 873 "surface.yy"
    {
			  CM->setFlag(SymbolType::IDEM);
			;}
    break;

  case 192:
#line 877 "surface.yy"
    {
			  CM->setFlag(SymbolType::ITER);
			;}
    break;

  case 193:
#line 880 "surface.yy"
    { CM->setPrec((yyvsp[0].yyToken)); ;}
    break;

  case 194:
#line 881 "surface.yy"
    { clear(); ;}
    break;

  case 195:
#line 882 "surface.yy"
    { CM->setGather(bubble); ;}
    break;

  case 196:
#line 883 "surface.yy"
    { clear(); ;}
    break;

  case 197:
#line 884 "surface.yy"
    { CM->setFormat(bubble); ;}
    break;

  case 198:
#line 885 "surface.yy"
    { clear(); ;}
    break;

  case 199:
#line 886 "surface.yy"
    { CM->setStrat(bubble); ;}
    break;

  case 200:
#line 887 "surface.yy"
    { clear(); ;}
    break;

  case 201:
#line 888 "surface.yy"
    { CM->setPoly(bubble); ;}
    break;

  case 202:
#line 890 "surface.yy"
    {
			  CM->setFlag(SymbolType::MEMO);
			;}
    break;

  case 203:
#line 894 "surface.yy"
    {
			  CM->setFlag(SymbolType::CTOR);
			;}
    break;

  case 204:
#line 898 "surface.yy"
    {
			  clear();
			  CM->setFrozen(bubble);
			;}
    break;

  case 205:
#line 902 "surface.yy"
    { clear(); ;}
    break;

  case 206:
#line 903 "surface.yy"
    { CM->setFrozen(bubble); ;}
    break;

  case 207:
#line 905 "surface.yy"
    {
			  CM->setFlag(SymbolType::CONFIG);
			;}
    break;

  case 208:
#line 909 "surface.yy"
    {
			  CM->setFlag(SymbolType::OBJECT);
			;}
    break;

  case 209:
#line 913 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 210:
#line 917 "surface.yy"
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			;}
    break;

  case 211:
#line 920 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 212:
#line 921 "surface.yy"
    { CM->setLatexMacro((yyvsp[-1].yyString)); ;}
    break;

  case 213:
#line 922 "surface.yy"
    {;}
    break;

  case 214:
#line 924 "surface.yy"
    {
			  CM->setFlag(SymbolType::DITTO);
			;}
    break;

  case 215:
#line 929 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 216:
#line 930 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 219:
#line 938 "surface.yy"
    {
			  clear();
			  CM->addHook(PreModule::ID_HOOK, (yyvsp[0].yyToken), bubble);
			;}
    break;

  case 220:
#line 942 "surface.yy"
    { clear(); ;}
    break;

  case 221:
#line 944 "surface.yy"
    {
			  CM->addHook(PreModule::ID_HOOK, (yyvsp[-4].yyToken), bubble);
			;}
    break;

  case 222:
#line 947 "surface.yy"
    { clear(); ;}
    break;

  case 223:
#line 949 "surface.yy"
    {
			  CM->addHook(PreModule::OP_HOOK, (yyvsp[-4].yyToken), bubble);
			;}
    break;

  case 224:
#line 952 "surface.yy"
    { clear(); ;}
    break;

  case 225:
#line 954 "surface.yy"
    {
			  CM->addHook(PreModule::TERM_HOOK, (yyvsp[-4].yyToken), bubble);
			;}
    break;

  case 226:
#line 962 "surface.yy"
    {;}
    break;

  case 227:
#line 964 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			;}
    break;

  case 228:
#line 970 "surface.yy"
    {;}
    break;

  case 229:
#line 972 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			;}
    break;

  case 230:
#line 980 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 232:
#line 984 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 233:
#line 985 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 234:
#line 986 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 235:
#line 987 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 236:
#line 993 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 237:
#line 994 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 238:
#line 995 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 240:
#line 999 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 241:
#line 1000 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 242:
#line 1001 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 244:
#line 1005 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 245:
#line 1006 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 246:
#line 1007 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 248:
#line 1011 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 249:
#line 1012 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 250:
#line 1013 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 252:
#line 1017 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 253:
#line 1018 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 254:
#line 1019 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 256:
#line 1023 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 257:
#line 1024 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 258:
#line 1025 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 260:
#line 1029 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 261:
#line 1030 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 262:
#line 1031 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 265:
#line 1036 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 266:
#line 1037 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 267:
#line 1040 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 268:
#line 1041 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 269:
#line 1042 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 270:
#line 1043 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 271:
#line 1044 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 272:
#line 1045 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 273:
#line 1046 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 274:
#line 1053 "surface.yy"
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

  case 275:
#line 1064 "surface.yy"
    { fragStore((yyvsp[0].yyToken)); ;}
    break;

  case 276:
#line 1065 "surface.yy"
    { fragStore((yyvsp[0].yyToken)); ;}
    break;

  case 277:
#line 1066 "surface.yy"
    { fragStore((yyvsp[0].yyToken)); ;}
    break;

  case 278:
#line 1069 "surface.yy"
    { fragStore((yyvsp[0].yyToken)); ;}
    break;

  case 279:
#line 1070 "surface.yy"
    {;}
    break;

  case 280:
#line 1071 "surface.yy"
    {;}
    break;

  case 281:
#line 1077 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 282:
#line 1078 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 287:
#line 1082 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 288:
#line 1083 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 446:
#line 1188 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 447:
#line 1189 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 448:
#line 1191 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.setCurrentModule(bubble);
			;}
    break;

  case 449:
#line 1195 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 450:
#line 1196 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 451:
#line 1198 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    CM->dump();
			;}
    break;

  case 452:
#line 1204 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 453:
#line 1210 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(bubble);
			;}
    break;

  case 454:
#line 1217 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 455:
#line 1223 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(bubble);
			;}
    break;

  case 456:
#line 1230 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 457:
#line 1236 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(bubble, (yyvsp[-3].yyBool));
			;}
    break;

  case 458:
#line 1243 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 459:
#line 1250 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(bubble, number, (yyvsp[-3].yyBool));
			;}
    break;

  case 460:
#line 1256 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 461:
#line 1264 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(bubble, number, number2, (yyvsp[-3].yyBool));
			;}
    break;

  case 462:
#line 1270 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 463:
#line 1278 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(bubble, number, number2, (yyvsp[-3].yyBool));
			;}
    break;

  case 464:
#line 1284 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 465:
#line 1291 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(bubble, number, (yyvsp[-3].yyBool));
			;}
    break;

  case 466:
#line 1297 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 467:
#line 1305 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(bubble, number, number2);
			;}
    break;

  case 468:
#line 1311 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 469:
#line 1318 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(bubble, (yyvsp[-2].yyBool), number);
			;}
    break;

  case 470:
#line 1324 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 471:
#line 1331 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(bubble, (yyvsp[-2].yyBool), number);
			;}
    break;

  case 472:
#line 1337 "surface.yy"
    {
			  interpreter.cont((yyvsp[-1].yyInt64), (yyvsp[-3].yyBool));
			;}
    break;

  case 473:
#line 1341 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 474:
#line 1347 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(bubble);
			;}
    break;

  case 475:
#line 1352 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 476:
#line 1354 "surface.yy"
    {
			  lexerInitialMode();
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(bubble);
			;}
    break;

  case 477:
#line 1361 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 478:
#line 1363 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[-3].yyBool));
			;}
    break;

  case 479:
#line 1367 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 480:
#line 1369 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[-3].yyBool));
			;}
    break;

  case 481:
#line 1373 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 482:
#line 1375 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[-3].yyBool));
			;}
    break;

  case 483:
#line 1379 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 484:
#line 1381 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[-3].yyBool));
			;}
    break;

  case 485:
#line 1386 "surface.yy"
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			;}
    break;

  case 486:
#line 1393 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 487:
#line 1395 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    CM->showModule();
			;}
    break;

  case 488:
#line 1400 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 489:
#line 1402 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    CM->showModule();
			;}
    break;

  case 490:
#line 1407 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 491:
#line 1409 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showModule(true);
			;}
    break;

  case 492:
#line 1414 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 493:
#line 1416 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentView(bubble))
			    interpreter.showView();
			;}
    break;

  case 494:
#line 1422 "surface.yy"
    {
			  interpreter.showModules(true);
			;}
    break;

  case 495:
#line 1426 "surface.yy"
    {
			  interpreter.showNamedViews();
			;}
    break;

  case 496:
#line 1429 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 497:
#line 1431 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showSortsAndSubsorts();
			;}
    break;

  case 498:
#line 1436 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 499:
#line 1438 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showOps();
			;}
    break;

  case 500:
#line 1443 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 501:
#line 1445 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showVars();
			;}
    break;

  case 502:
#line 1450 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 503:
#line 1452 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showMbs();
			;}
    break;

  case 504:
#line 1457 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 505:
#line 1459 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showEqs();
			;}
    break;

  case 506:
#line 1464 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 507:
#line 1466 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showRls();
			;}
    break;

  case 508:
#line 1471 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 509:
#line 1473 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showSummary();
			;}
    break;

  case 510:
#line 1478 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 511:
#line 1480 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showKinds();
			;}
    break;

  case 512:
#line 1486 "surface.yy"
    {
			  interpreter.showSearchPath((yyvsp[-1].yyInt64));
			;}
    break;

  case 513:
#line 1490 "surface.yy"
    {
			  interpreter.showSearchPathLabels((yyvsp[-1].yyInt64));
			;}
    break;

  case 514:
#line 1494 "surface.yy"
    {
			  interpreter.showSearchGraph();
			;}
    break;

  case 515:
#line 1497 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 516:
#line 1499 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showProfile();
			;}
    break;

  case 517:
#line 1508 "surface.yy"
    {
			  globalAdvisoryFlag = (yyvsp[-1].yyBool);
			;}
    break;

  case 518:
#line 1512 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			;}
    break;

  case 519:
#line 1516 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[-1].yyBool));
			;}
    break;

  case 520:
#line 1520 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[-1].yyBool));
			;}
    break;

  case 521:
#line 1524 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[-1].yyBool));
			;}
    break;

  case 522:
#line 1528 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[-1].yyBool));
			;}
    break;

  case 523:
#line 1532 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[-1].yyBool));
			;}
    break;

  case 524:
#line 1536 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			;}
    break;

  case 525:
#line 1540 "surface.yy"
    {
			  interpreter.setPrintFlag((yyvsp[-2].yyPrintFlags), (yyvsp[-1].yyBool));
			;}
    break;

  case 526:
#line 1544 "surface.yy"
    {
			  interpreter.setFlag((yyvsp[-2].yyFlags), (yyvsp[-1].yyBool));
			;}
    break;

  case 527:
#line 1548 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[-1].yyBool));
			;}
    break;

  case 528:
#line 1551 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 529:
#line 1552 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 530:
#line 1554 "surface.yy"
    {
			  interpreter.setAutoImport((yyvsp[-5].yyImportMode), (yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			;}
    break;

  case 531:
#line 1557 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 532:
#line 1558 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 533:
#line 1560 "surface.yy"
    {
			  interpreter.setOmodInclude((yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			;}
    break;

  case 534:
#line 1564 "surface.yy"
    {
			  globalVerboseFlag = (yyvsp[-1].yyBool);
			;}
    break;

  case 535:
#line 1568 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[-1].yyBool));
			;}
    break;

  case 536:
#line 1572 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[-1].yyBool));
			;}
    break;

  case 537:
#line 1576 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[-1].yyBool));
			;}
    break;

  case 538:
#line 1580 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[-1].yyBool));
			;}
    break;

  case 539:
#line 1584 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[-1].yyBool));
			;}
    break;

  case 540:
#line 1591 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			;}
    break;

  case 541:
#line 1595 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			;}
    break;

  case 542:
#line 1599 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			;}
    break;

  case 543:
#line 1603 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			;}
    break;

  case 544:
#line 1610 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			;}
    break;

  case 545:
#line 1614 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			;}
    break;

  case 546:
#line 1620 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 548:
#line 1627 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; ;}
    break;

  case 549:
#line 1628 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; ;}
    break;

  case 550:
#line 1629 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; ;}
    break;

  case 551:
#line 1630 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; ;}
    break;

  case 552:
#line 1631 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; ;}
    break;

  case 553:
#line 1632 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; ;}
    break;

  case 554:
#line 1633 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; ;}
    break;

  case 555:
#line 1634 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; ;}
    break;

  case 556:
#line 1635 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; ;}
    break;

  case 557:
#line 1636 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; ;}
    break;

  case 558:
#line 1639 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE; ;}
    break;

  case 559:
#line 1640 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; ;}
    break;

  case 560:
#line 1641 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; ;}
    break;

  case 561:
#line 1642 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; ;}
    break;

  case 562:
#line 1643 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; ;}
    break;

  case 563:
#line 1644 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_MB; ;}
    break;

  case 564:
#line 1645 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; ;}
    break;

  case 565:
#line 1646 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_RL; ;}
    break;

  case 566:
#line 1647 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; ;}
    break;

  case 567:
#line 1648 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; ;}
    break;

  case 568:
#line 1649 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; ;}
    break;

  case 569:
#line 1652 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 570:
#line 1653 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 571:
#line 1656 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 572:
#line 1657 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 573:
#line 1660 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 574:
#line 1661 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 575:
#line 1664 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 576:
#line 1665 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 577:
#line 1668 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 578:
#line 1669 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 579:
#line 1672 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 580:
#line 1673 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 581:
#line 1676 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 582:
#line 1677 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 583:
#line 1680 "surface.yy"
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); ;}
    break;

  case 584:
#line 1681 "surface.yy"
    { (yyval.yyInt64) = NONE; ;}
    break;

  case 585:
#line 1684 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::PROTECTING; ;}
    break;

  case 586:
#line 1685 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::EXTENDING; ;}
    break;

  case 587:
#line 1686 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::INCLUDING; ;}
    break;

  case 588:
#line 1694 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 590:
#line 1696 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 592:
#line 1700 "surface.yy"
    { moduleExpr = bubble; clear(); ;}
    break;

  case 593:
#line 1701 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 594:
#line 1702 "surface.yy"
    {;}
    break;

  case 595:
#line 1703 "surface.yy"
    {;}
    break;

  case 596:
#line 1713 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 598:
#line 1715 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 600:
#line 1717 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 602:
#line 1722 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 604:
#line 1724 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 606:
#line 1726 "surface.yy"
    {;}
    break;

  case 607:
#line 1731 "surface.yy"
    {
			  number = Token::codeToInt64(bubble[1].code());
			  clear();
			;}
    break;

  case 609:
#line 1736 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 611:
#line 1738 "surface.yy"
    {;}
    break;

  case 612:
#line 1751 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 614:
#line 1753 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 616:
#line 1755 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 618:
#line 1764 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 620:
#line 1766 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 622:
#line 1768 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 624:
#line 1770 "surface.yy"
    {;}
    break;

  case 625:
#line 1779 "surface.yy"
    {
			  number = Token::codeToInt64(bubble[1].code());
			  clear();
			;}
    break;

  case 627:
#line 1784 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 629:
#line 1786 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 631:
#line 1788 "surface.yy"
    {;}
    break;

  case 632:
#line 1796 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 634:
#line 1798 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 636:
#line 1800 "surface.yy"
    {;}
    break;

  case 637:
#line 1809 "surface.yy"
    {
			  number = Token::codeToInt64(bubble[1].code());
			  number2 = Token::codeToInt64(bubble[3].code());
			  clear();
			;}
    break;

  case 639:
#line 1815 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 641:
#line 1817 "surface.yy"
    {;}
    break;

  case 642:
#line 1825 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 644:
#line 1827 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 646:
#line 1829 "surface.yy"
    {;}
    break;

  case 647:
#line 1838 "surface.yy"
    {
			  number2 = Token::codeToInt64(bubble[2].code());
			  clear();
			;}
    break;

  case 649:
#line 1843 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 651:
#line 1845 "surface.yy"
    {;}
    break;

  case 652:
#line 1851 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 654:
#line 1855 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 656:
#line 1860 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 666:
#line 1868 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 667:
#line 1869 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 675:
#line 1873 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 676:
#line 1874 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 683:
#line 1879 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 684:
#line 1880 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 692:
#line 1884 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 693:
#line 1885 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 700:
#line 1889 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 701:
#line 1890 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 708:
#line 1895 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 709:
#line 1896 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 716:
#line 1901 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 717:
#line 1902 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 723:
#line 1907 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 724:
#line 1908 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 730:
#line 1913 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 731:
#line 1914 "surface.yy"
    { (yyval.yyToken) = (yyvsp[0].yyToken); ;}
    break;

  case 734:
#line 1924 "surface.yy"
    {
			  clear();
			  store((yyvsp[0].yyToken));
			  interpreter.addSelected(bubble);
			;}
    break;

  case 735:
#line 1929 "surface.yy"
    { clear(); ;}
    break;

  case 736:
#line 1931 "surface.yy"
    {
			  interpreter.addSelected(bubble);
			;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 6545 "surface.c"

  yyvsp -= yylen;
  yyssp -= yylen;


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
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
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
	  int yychecklim = YYLAST - yyn;
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
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
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
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
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


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
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

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1961 "surface.yy"


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
  //bubble.contractTo(0);
  //moduleExpr.contractTo(0);
  //pattern.contractTo(0);
  interpreter.makeClean(lineNumber);
  /*
  if (currentModule != 0 && !(currentModule->isComplete()))
    {
      IssueAdvisory(cerr << LineNumber(lineNumber) << ": discarding incomplete module.");
      delete currentModule;
      currentModule = 0;
    }
  */
}

