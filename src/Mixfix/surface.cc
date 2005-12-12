/* A Bison parser, made by GNU Bison 1.875.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002 Free Software Foundation, Inc.

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
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

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
     KW_OREWRITE = 276,
     KW_CONTINUE = 277,
     KW_SEARCH = 278,
     KW_SET = 279,
     KW_SHOW = 280,
     KW_ON = 281,
     KW_OFF = 282,
     KW_TRACE = 283,
     KW_BODY = 284,
     KW_WHOLE = 285,
     KW_SELECT = 286,
     KW_DESELECT = 287,
     KW_CONDITION = 288,
     KW_SUBSTITUTION = 289,
     KW_PRINT = 290,
     KW_GRAPH = 291,
     KW_MIXFIX = 292,
     KW_FLAT = 293,
     KW_WITH = 294,
     KW_PARENS = 295,
     KW_ALIASES = 296,
     KW_GC = 297,
     KW_TIME = 298,
     KW_STATS = 299,
     KW_TIMING = 300,
     KW_CMD = 301,
     KW_BREAKDOWN = 302,
     KW_BREAK = 303,
     KW_PATH = 304,
     KW_MODULE = 305,
     KW_MODULES = 306,
     KW_VIEWS = 307,
     KW_ALL = 308,
     KW_SORTS = 309,
     KW_OPS = 310,
     KW_VARS = 311,
     KW_MBS = 312,
     KW_EQS = 313,
     KW_RLS = 314,
     KW_SUMMARY = 315,
     KW_KINDS = 316,
     KW_ADVISE = 317,
     KW_VERBOSE = 318,
     KW_DO = 319,
     KW_CLEAR = 320,
     KW_PROTECT = 321,
     KW_EXTEND = 322,
     KW_INCLUDE = 323,
     KW_EXCLUDE = 324,
     KW_CONCEAL = 325,
     KW_REVEAL = 326,
     KW_COMPILE = 327,
     KW_COUNT = 328,
     KW_DEBUG = 329,
     KW_RESUME = 330,
     KW_ABORT = 331,
     KW_STEP = 332,
     KW_WHERE = 333,
     KW_CREDUCE = 334,
     KW_DUMP = 335,
     KW_PROFILE = 336,
     KW_NUMBER = 337,
     KW_RAT = 338,
     KW_COLOR = 339,
     SIMPLE_NUMBER = 340,
     KW_PWD = 341,
     KW_CD = 342,
     KW_PUSHD = 343,
     KW_POPD = 344,
     KW_LS = 345,
     KW_LOAD = 346,
     KW_QUIT = 347,
     KW_EOF = 348,
     KW_ENDM = 349,
     KW_IMPORT = 350,
     KW_ENDV = 351,
     KW_SORT = 352,
     KW_SUBSORT = 353,
     KW_OP = 354,
     KW_MSGS = 355,
     KW_VAR = 356,
     KW_CLASS = 357,
     KW_SUBCLASS = 358,
     KW_MB = 359,
     KW_CMB = 360,
     KW_EQ = 361,
     KW_CEQ = 362,
     KW_RL = 363,
     KW_CRL = 364,
     KW_IS = 365,
     KW_FROM = 366,
     KW_ARROW = 367,
     KW_ARROW2 = 368,
     KW_PARTIAL = 369,
     KW_IF = 370,
     KW_LABEL = 371,
     KW_TO = 372,
     KW_COLON2 = 373,
     KW_ASSOC = 374,
     KW_COMM = 375,
     KW_ID = 376,
     KW_IDEM = 377,
     KW_ITER = 378,
     KW_LEFT = 379,
     KW_RIGHT = 380,
     KW_PREC = 381,
     KW_GATHER = 382,
     KW_METADATA = 383,
     KW_STRAT = 384,
     KW_POLY = 385,
     KW_MEMO = 386,
     KW_FROZEN = 387,
     KW_CTOR = 388,
     KW_LATEX = 389,
     KW_SPECIAL = 390,
     KW_CONFIG = 391,
     KW_OBJ = 392,
     KW_MSG = 393,
     KW_DITTO = 394,
     KW_FORMAT = 395,
     KW_ID_HOOK = 396,
     KW_OP_HOOK = 397,
     KW_TERM_HOOK = 398,
     KW_IN = 399,
     IDENTIFIER = 400,
     NUMERIC_ID = 401,
     ENDS_IN_DOT = 402
   };
#endif
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
#define KW_OREWRITE 276
#define KW_CONTINUE 277
#define KW_SEARCH 278
#define KW_SET 279
#define KW_SHOW 280
#define KW_ON 281
#define KW_OFF 282
#define KW_TRACE 283
#define KW_BODY 284
#define KW_WHOLE 285
#define KW_SELECT 286
#define KW_DESELECT 287
#define KW_CONDITION 288
#define KW_SUBSTITUTION 289
#define KW_PRINT 290
#define KW_GRAPH 291
#define KW_MIXFIX 292
#define KW_FLAT 293
#define KW_WITH 294
#define KW_PARENS 295
#define KW_ALIASES 296
#define KW_GC 297
#define KW_TIME 298
#define KW_STATS 299
#define KW_TIMING 300
#define KW_CMD 301
#define KW_BREAKDOWN 302
#define KW_BREAK 303
#define KW_PATH 304
#define KW_MODULE 305
#define KW_MODULES 306
#define KW_VIEWS 307
#define KW_ALL 308
#define KW_SORTS 309
#define KW_OPS 310
#define KW_VARS 311
#define KW_MBS 312
#define KW_EQS 313
#define KW_RLS 314
#define KW_SUMMARY 315
#define KW_KINDS 316
#define KW_ADVISE 317
#define KW_VERBOSE 318
#define KW_DO 319
#define KW_CLEAR 320
#define KW_PROTECT 321
#define KW_EXTEND 322
#define KW_INCLUDE 323
#define KW_EXCLUDE 324
#define KW_CONCEAL 325
#define KW_REVEAL 326
#define KW_COMPILE 327
#define KW_COUNT 328
#define KW_DEBUG 329
#define KW_RESUME 330
#define KW_ABORT 331
#define KW_STEP 332
#define KW_WHERE 333
#define KW_CREDUCE 334
#define KW_DUMP 335
#define KW_PROFILE 336
#define KW_NUMBER 337
#define KW_RAT 338
#define KW_COLOR 339
#define SIMPLE_NUMBER 340
#define KW_PWD 341
#define KW_CD 342
#define KW_PUSHD 343
#define KW_POPD 344
#define KW_LS 345
#define KW_LOAD 346
#define KW_QUIT 347
#define KW_EOF 348
#define KW_ENDM 349
#define KW_IMPORT 350
#define KW_ENDV 351
#define KW_SORT 352
#define KW_SUBSORT 353
#define KW_OP 354
#define KW_MSGS 355
#define KW_VAR 356
#define KW_CLASS 357
#define KW_SUBCLASS 358
#define KW_MB 359
#define KW_CMB 360
#define KW_EQ 361
#define KW_CEQ 362
#define KW_RL 363
#define KW_CRL 364
#define KW_IS 365
#define KW_FROM 366
#define KW_ARROW 367
#define KW_ARROW2 368
#define KW_PARTIAL 369
#define KW_IF 370
#define KW_LABEL 371
#define KW_TO 372
#define KW_COLON2 373
#define KW_ASSOC 374
#define KW_COMM 375
#define KW_ID 376
#define KW_IDEM 377
#define KW_ITER 378
#define KW_LEFT 379
#define KW_RIGHT 380
#define KW_PREC 381
#define KW_GATHER 382
#define KW_METADATA 383
#define KW_STRAT 384
#define KW_POLY 385
#define KW_MEMO 386
#define KW_FROZEN 387
#define KW_CTOR 388
#define KW_LATEX 389
#define KW_SPECIAL 390
#define KW_CONFIG 391
#define KW_OBJ 392
#define KW_MSG 393
#define KW_DITTO 394
#define KW_FORMAT 395
#define KW_ID_HOOK 396
#define KW_OP_HOOK 397
#define KW_TERM_HOOK 398
#define KW_IN 399
#define IDENTIFIER 400
#define NUMERIC_ID 401
#define ENDS_IN_DOT 402




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

#include "main.hh"
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

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
#line 97 "surface.yy"
typedef union YYSTYPE {
  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
} YYSTYPE;
/* Line 191 of yacc.c.  */
#line 447 "surface.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 107 "surface.yy"

int yylex(YYSTYPE* lvalp);


/* Line 214 of yacc.c.  */
#line 462 "surface.c"

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# if YYSTACK_USE_ALLOCA
#  define YYSTACK_ALLOC alloca
# else
#  ifndef YYSTACK_USE_ALLOCA
#   if defined (alloca) || defined (_ALLOCA_H)
#    define YYSTACK_ALLOC alloca
#   else
#    ifdef __GNUC__
#     define YYSTACK_ALLOC __builtin_alloca
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
# else
#  if defined (__STDC__) || defined (__cplusplus)
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   define YYSIZE_T size_t
#  endif
#  define YYSTACK_ALLOC malloc
#  define YYSTACK_FREE free
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short) + sizeof (YYSTYPE))				\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  register YYSIZE_T yyi;		\
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
   typedef short yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  104
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   6839

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  162
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  267
/* YYNRULES -- Number of rules. */
#define YYNRULES  711
/* YYNRULES -- Number of states. */
#define YYNSTATES  1095

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   402

#define YYTRANSLATE(YYX) 						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     118,   119,   130,   129,   124,     2,   120,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   116,     2,
     121,   117,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   122,     2,   123,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   131,   125,   132,     2,     2,     2,     2,
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
     115,   126,   127,   128,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short yyprhs[] =
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
     468,   469,   474,   479,   483,   485,   487,   489,   492,   493,
     495,   496,   501,   505,   507,   511,   512,   515,   517,   519,
     522,   525,   527,   529,   530,   534,   536,   538,   541,   542,
     548,   549,   555,   556,   562,   563,   569,   571,   573,   575,
     576,   582,   584,   586,   588,   591,   592,   598,   603,   605,
     608,   610,   613,   615,   618,   619,   626,   627,   634,   635,
     642,   644,   645,   647,   648,   651,   652,   655,   658,   659,
     664,   665,   671,   674,   675,   676,   682,   685,   686,   687,
     693,   696,   697,   698,   704,   707,   708,   709,   715,   718,
     719,   720,   726,   729,   730,   731,   737,   740,   741,   743,
     746,   748,   749,   754,   755,   761,   764,   767,   769,   771,
     773,   774,   780,   781,   786,   788,   790,   793,   795,   797,
     799,   801,   802,   807,   809,   811,   813,   815,   817,   819,
     821,   823,   825,   827,   829,   831,   833,   835,   837,   839,
     841,   843,   845,   847,   849,   851,   853,   855,   857,   859,
     861,   863,   865,   867,   869,   871,   873,   875,   877,   879,
     881,   883,   885,   887,   889,   891,   893,   895,   897,   899,
     901,   903,   905,   907,   909,   911,   913,   915,   917,   919,
     921,   923,   925,   927,   929,   931,   933,   935,   937,   939,
     941,   943,   945,   947,   949,   951,   953,   955,   957,   959,
     961,   963,   965,   967,   969,   971,   973,   975,   977,   979,
     981,   983,   985,   987,   989,   991,   993,   995,   997,   999,
    1001,  1003,  1005,  1007,  1009,  1011,  1013,  1015,  1017,  1019,
    1021,  1023,  1025,  1027,  1029,  1031,  1033,  1035,  1037,  1039,
    1041,  1043,  1045,  1047,  1049,  1051,  1053,  1055,  1057,  1059,
    1061,  1063,  1065,  1067,  1069,  1071,  1073,  1075,  1077,  1079,
    1081,  1083,  1085,  1087,  1089,  1091,  1093,  1095,  1097,  1099,
    1101,  1103,  1105,  1107,  1109,  1111,  1113,  1115,  1117,  1119,
    1121,  1122,  1123,  1130,  1131,  1132,  1139,  1140,  1144,  1145,
    1149,  1150,  1155,  1156,  1161,  1162,  1167,  1168,  1173,  1174,
    1178,  1179,  1183,  1188,  1189,  1193,  1194,  1199,  1200,  1206,
    1207,  1213,  1214,  1220,  1221,  1227,  1232,  1233,  1239,  1240,
    1246,  1247,  1253,  1254,  1260,  1264,  1268,  1269,  1275,  1276,
    1282,  1283,  1289,  1290,  1296,  1297,  1303,  1304,  1310,  1311,
    1317,  1318,  1324,  1329,  1335,  1340,  1341,  1347,  1353,  1359,
    1366,  1372,  1378,  1385,  1391,  1397,  1403,  1409,  1414,  1415,
    1416,  1424,  1425,  1426,  1435,  1440,  1446,  1452,  1458,  1463,
    1469,  1472,  1475,  1478,  1481,  1487,  1492,  1493,  1497,  1499,
    1501,  1504,  1507,  1509,  1511,  1513,  1515,  1517,  1519,  1520,
    1522,  1524,  1526,  1528,  1530,  1532,  1534,  1536,  1538,  1540,
    1542,  1544,  1546,  1548,  1550,  1552,  1554,  1556,  1558,  1560,
    1561,  1563,  1564,  1566,  1568,  1570,  1571,  1576,  1577,  1582,
    1583,  1584,  1591,  1593,  1594,  1598,  1599,  1604,  1605,  1610,
    1611,  1615,  1616,  1621,  1623,  1624,  1628,  1629,  1634,  1636,
    1637,  1641,  1642,  1647,  1648,  1653,  1654,  1658,  1659,  1664,
    1666,  1667,  1671,  1672,  1676,  1677,  1682,  1684,  1685,  1689,
    1690,  1695,  1697,  1698,  1702,  1703,  1708,  1710,  1713,  1714,
    1717,  1718,  1721,  1722,  1724,  1726,  1728,  1730,  1732,  1734,
    1736,  1738,  1739,  1744,  1746,  1748,  1750,  1752,  1754,  1756,
    1758,  1759,  1764,  1766,  1768,  1770,  1772,  1774,  1776,  1777,
    1782,  1784,  1786,  1788,  1790,  1792,  1794,  1796,  1797,  1802,
    1804,  1806,  1808,  1810,  1812,  1814,  1815,  1820,  1822,  1824,
    1826,  1828,  1830,  1832,  1833,  1838,  1840,  1842,  1844,  1846,
    1848,  1850,  1851,  1856,  1858,  1860,  1862,  1864,  1866,  1867,
    1872,  1875,  1877,  1879,  1880,  1885,  1887,  1889,  1891,  1893,
    1895,  1897
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short yyrhs[] =
{
     163,     0,    -1,   164,    -1,    -1,   211,    -1,   199,    -1,
     165,    -1,   326,    -1,    -1,   158,   166,     3,    -1,    -1,
      91,   167,     3,    -1,    86,    -1,    -1,    87,   168,     3,
      -1,    -1,    88,   169,     3,    -1,    89,    -1,    -1,    90,
     170,     4,    -1,    92,    -1,    93,    -1,   309,   278,    -1,
     318,   120,    -1,   178,   278,    -1,   175,   278,    -1,   176,
     278,    -1,   172,   129,   171,    -1,   161,    -1,   173,    -1,
     172,   129,   172,    -1,   174,    -1,   175,    -1,   178,    -1,
     176,    -1,   308,    -1,   173,   130,   180,    -1,    -1,   174,
     131,   177,   179,   132,    -1,   118,   172,   119,    -1,   179,
     124,   308,    -1,   308,    -1,    -1,    -1,   118,   181,   183,
     182,   119,    -1,   184,    -1,     1,    -1,   184,   124,   185,
      -1,   185,    -1,    97,   300,   127,   300,    -1,   126,   308,
     127,   308,    -1,    99,   186,   189,   127,   190,   193,    -1,
      -1,   308,   187,   286,    -1,    -1,   118,   188,   282,   119,
      -1,   116,   256,   255,   257,    -1,    -1,    -1,   308,   191,
     288,    -1,    -1,   118,   192,   282,   119,    -1,   122,   194,
     123,    -1,    -1,   194,   195,    -1,   195,    -1,   140,   159,
      -1,    -1,   141,   118,   196,   271,   119,    -1,    -1,   154,
     118,   197,   271,   119,    -1,    -1,   148,   118,   198,     5,
     119,    -1,    -1,    -1,    -1,     8,   200,   308,   111,   172,
     201,   127,   172,   202,   277,   203,    96,    -1,   203,   204,
      -1,    -1,    97,   300,   127,   206,    -1,   101,   246,   116,
     219,    -1,    -1,    99,   205,   286,   207,    -1,     1,   120,
      -1,   300,   278,    -1,   161,    -1,    -1,    -1,   116,   208,
     256,   255,   257,   127,   209,   220,    -1,    -1,   127,   210,
     220,    -1,    -1,    -1,   221,   212,   308,   213,   215,   277,
     222,    94,    -1,   120,    -1,   161,    -1,   131,   216,   132,
      -1,    -1,   216,   124,   217,    -1,   217,    -1,   308,   128,
     172,    -1,   161,    -1,   257,   278,    -1,   218,    -1,   296,
     214,    -1,   161,    -1,     6,    -1,     7,    -1,   222,   223,
      -1,    -1,    95,   171,    -1,    -1,    97,   224,   279,   214,
      -1,    -1,    98,   225,   280,   214,    -1,    99,   247,   253,
      -1,    55,   250,   253,    -1,   101,   246,   116,   219,    -1,
      -1,    -1,   104,   226,   284,   116,   227,   220,    -1,    -1,
      -1,    -1,   105,   228,   284,   116,   229,   294,   115,   230,
     220,    -1,    -1,    -1,   106,   231,   290,   117,   232,   220,
      -1,    -1,    -1,    -1,   107,   233,   290,   117,   234,   294,
     115,   235,   220,    -1,    -1,    -1,   108,   236,   292,   113,
     237,   220,    -1,    -1,    -1,    -1,   109,   238,   292,   113,
     239,   294,   115,   240,   220,    -1,   152,   247,   253,    -1,
     100,   250,   253,    -1,    -1,   102,   308,   241,   243,   120,
      -1,    -1,   103,   242,   280,   214,    -1,     1,   120,    -1,
     125,    -1,   125,   244,    -1,   245,    -1,   244,   124,   245,
      -1,   309,   116,   308,    -1,   246,   314,    -1,   314,    -1,
      -1,   308,   248,   284,    -1,    -1,   118,   249,   282,   119,
      -1,   250,   251,    -1,   251,    -1,   314,    -1,    -1,   118,
     252,   282,   119,    -1,   116,   256,   255,   254,    -1,   257,
     260,   278,    -1,   218,    -1,   112,    -1,   114,    -1,   256,
     257,    -1,    -1,   300,    -1,    -1,   122,   258,   259,   123,
      -1,   259,   124,   300,    -1,   300,    -1,   122,   261,   123,
      -1,    -1,   261,   263,    -1,   263,    -1,   135,    -1,   138,
     135,    -1,   139,   135,    -1,   133,    -1,   134,    -1,    -1,
     262,   264,   305,    -1,   136,    -1,   137,    -1,   140,   159,
      -1,    -1,   141,   118,   265,   271,   119,    -1,    -1,   154,
     118,   266,   271,   119,    -1,    -1,   143,   118,   267,   271,
     119,    -1,    -1,   144,   118,   268,   271,   119,    -1,   145,
      -1,   147,    -1,   146,    -1,    -1,   146,   118,   269,   271,
     119,    -1,   150,    -1,   151,    -1,   152,    -1,   142,   159,
      -1,    -1,   148,   118,   270,     5,   119,    -1,   149,   118,
     272,   119,    -1,   153,    -1,   271,   159,    -1,   159,    -1,
     272,   273,    -1,   273,    -1,   155,   308,    -1,    -1,   155,
     308,   118,   274,   282,   119,    -1,    -1,   156,   308,   118,
     275,   282,   119,    -1,    -1,   157,   308,   118,   276,   282,
     119,    -1,   110,    -1,    -1,   120,    -1,    -1,   279,   300,
      -1,    -1,   280,   300,    -1,   280,   121,    -1,    -1,   300,
     281,   279,   121,    -1,    -1,   282,   118,   283,   282,   119,
      -1,   282,   308,    -1,    -1,    -1,   284,   118,   285,   282,
     119,    -1,   284,   314,    -1,    -1,    -1,   286,   118,   287,
     282,   119,    -1,   286,   315,    -1,    -1,    -1,   288,   118,
     289,   282,   119,    -1,   288,   316,    -1,    -1,    -1,   290,
     118,   291,   282,   119,    -1,   290,   312,    -1,    -1,    -1,
     292,   118,   293,   282,   119,    -1,   292,   311,    -1,    -1,
      -1,   294,   118,   295,   282,   119,    -1,   294,   313,    -1,
      -1,   297,    -1,   296,   318,    -1,   318,    -1,    -1,   118,
     298,   282,   119,    -1,    -1,   296,   118,   299,   282,   119,
      -1,   297,   321,    -1,   296,   310,    -1,   309,    -1,   301,
      -1,   317,    -1,    -1,   301,   131,   302,   303,   132,    -1,
      -1,   303,   124,   304,   301,    -1,   301,    -1,   306,    -1,
     305,   306,    -1,   320,    -1,   322,    -1,   323,    -1,   120,
      -1,    -1,   118,   307,   282,   119,    -1,   320,    -1,   321,
      -1,   323,    -1,   324,    -1,   120,    -1,   319,    -1,   124,
      -1,   127,    -1,   321,    -1,   323,    -1,   324,    -1,   319,
      -1,   124,    -1,   127,    -1,   323,    -1,   324,    -1,   320,
      -1,   321,    -1,   324,    -1,   120,    -1,   121,    -1,   116,
      -1,   112,    -1,   114,    -1,   117,    -1,   115,    -1,   320,
      -1,   321,    -1,   324,    -1,   120,    -1,   121,    -1,   116,
      -1,   112,    -1,   114,    -1,   113,    -1,   115,    -1,   320,
      -1,   321,    -1,   324,    -1,   120,    -1,   121,    -1,   116,
      -1,   112,    -1,   114,    -1,   117,    -1,   113,    -1,   320,
      -1,   321,    -1,   324,    -1,   120,    -1,   121,    -1,   112,
      -1,   114,    -1,   117,    -1,   113,    -1,   115,    -1,   319,
      -1,   161,    -1,   124,    -1,   321,    -1,   324,    -1,   120,
      -1,   121,    -1,   112,    -1,   114,    -1,   117,    -1,   113,
      -1,   115,    -1,   319,    -1,   161,    -1,   127,    -1,   321,
      -1,   325,    -1,   120,    -1,   123,    -1,   323,    -1,   159,
      -1,   321,    -1,   325,    -1,   117,    -1,   125,    -1,   129,
      -1,   130,    -1,   113,    -1,   115,    -1,   110,    -1,   126,
      -1,   127,    -1,   120,    -1,   161,    -1,   159,    -1,   131,
      -1,   132,    -1,   129,    -1,   130,    -1,   125,    -1,   128,
      -1,   126,    -1,   111,    -1,   110,    -1,   319,    -1,   161,
      -1,   124,    -1,   127,    -1,   152,    -1,   322,    -1,    95,
      -1,    97,    -1,    98,    -1,    99,    -1,    55,    -1,   101,
      -1,   100,    -1,   102,    -1,   103,    -1,   104,    -1,   105,
      -1,   106,    -1,   107,    -1,   108,    -1,   109,    -1,    94,
      -1,    96,    -1,   121,    -1,   116,    -1,   112,    -1,   114,
      -1,   117,    -1,   113,    -1,   115,    -1,   122,    -1,   123,
      -1,   325,    -1,   133,    -1,   134,    -1,   135,    -1,   136,
      -1,   137,    -1,   138,    -1,   139,    -1,   140,    -1,   141,
      -1,   143,    -1,   144,    -1,   145,    -1,   147,    -1,   148,
      -1,   149,    -1,   146,    -1,   142,    -1,   150,    -1,   151,
      -1,   153,    -1,   154,    -1,   155,    -1,   156,    -1,   157,
      -1,    -1,    -1,    31,   327,   409,   328,   407,   120,    -1,
      -1,    -1,    80,   329,   409,   330,   407,   120,    -1,    -1,
       9,   331,   373,    -1,    -1,    79,   332,   373,    -1,    -1,
     370,    11,   333,   373,    -1,    -1,   370,    12,   334,   379,
      -1,    -1,   370,    19,   335,   379,    -1,    -1,   370,    20,
     336,   389,    -1,    -1,    23,   337,   379,    -1,    -1,   369,
     338,   379,    -1,   370,    22,   371,   120,    -1,    -1,    13,
     339,   373,    -1,    -1,   118,   340,   406,   119,    -1,    -1,
      28,   366,   341,   425,   120,    -1,    -1,    28,   367,   342,
     425,   120,    -1,    -1,    48,   366,   343,   425,   120,    -1,
      -1,    35,   368,   344,   425,   120,    -1,    64,    65,   145,
     120,    -1,    -1,    25,     6,   345,   407,   120,    -1,    -1,
      25,    50,   346,   407,   120,    -1,    -1,    25,    53,   347,
     407,   120,    -1,    -1,    25,     8,   348,   407,   120,    -1,
      25,    51,   120,    -1,    25,    52,   120,    -1,    -1,    25,
      54,   349,   407,   120,    -1,    -1,    25,    55,   350,   407,
     120,    -1,    -1,    25,    56,   351,   407,   120,    -1,    -1,
      25,    57,   352,   407,   120,    -1,    -1,    25,    58,   353,
     407,   120,    -1,    -1,    25,    59,   354,   407,   120,    -1,
      -1,    25,    60,   355,   407,   120,    -1,    -1,    25,    61,
     356,   407,   120,    -1,    25,    49,    85,   120,    -1,    25,
      49,   126,    85,   120,    -1,    25,    23,    36,   120,    -1,
      -1,    25,    81,   357,   407,   120,    -1,    24,    25,    62,
     365,   120,    -1,    24,    25,    44,   365,   120,    -1,    24,
      25,    13,    44,   365,   120,    -1,    24,    25,    45,   365,
     120,    -1,    24,    25,    47,   365,   120,    -1,    24,    25,
      13,    45,   365,   120,    -1,    24,    25,    46,   365,   120,
      -1,    24,    25,    42,   365,   120,    -1,    24,    35,   363,
     365,   120,    -1,    24,    28,   364,   365,   120,    -1,    24,
      48,   365,   120,    -1,    -1,    -1,    24,   372,   358,   428,
     359,   365,   120,    -1,    -1,    -1,    24,     7,    68,   360,
     428,   361,   365,   120,    -1,    24,    63,   365,   120,    -1,
      24,    65,   145,   365,   120,    -1,    24,    65,    59,   365,
     120,    -1,    24,    72,    73,   365,   120,    -1,    24,    81,
     365,   120,    -1,    24,    65,    81,   365,   120,    -1,    75,
     120,    -1,    76,   120,    -1,    77,   120,    -1,    78,   120,
      -1,    24,    42,    25,   365,   120,    -1,    24,    44,   365,
     120,    -1,    -1,     1,   362,   120,    -1,    37,    -1,    38,
      -1,    39,    41,    -1,    39,    40,    -1,    36,    -1,    70,
      -1,    82,    -1,    83,    -1,    84,    -1,   154,    -1,    -1,
      33,    -1,    30,    -1,    34,    -1,    31,    -1,    57,    -1,
      58,    -1,    59,    -1,    12,    -1,    29,    -1,    26,    -1,
      27,    -1,    31,    -1,    32,    -1,    69,    -1,    68,    -1,
      70,    -1,    71,    -1,    16,    -1,    15,    -1,    74,    -1,
      -1,    85,    -1,    -1,    66,    -1,    67,    -1,    68,    -1,
      -1,   158,   374,   408,   376,    -1,    -1,   415,   375,   407,
     120,    -1,    -1,    -1,   116,   377,   409,   378,   407,   120,
      -1,   120,    -1,    -1,   122,   380,   383,    -1,    -1,   158,
     381,   408,   376,    -1,    -1,   417,   382,   407,   120,    -1,
      -1,   160,   384,   386,    -1,    -1,   419,   385,   407,   120,
      -1,   120,    -1,    -1,   123,   387,   373,    -1,    -1,   421,
     388,   407,   120,    -1,   120,    -1,    -1,   122,   390,   393,
      -1,    -1,   158,   391,   408,   376,    -1,    -1,   417,   392,
     407,   120,    -1,    -1,   160,   394,   396,    -1,    -1,   419,
     395,   407,   120,    -1,   120,    -1,    -1,   123,   397,   373,
      -1,    -1,   124,   398,   400,    -1,    -1,   423,   399,   407,
     120,    -1,   120,    -1,    -1,   160,   401,   403,    -1,    -1,
     419,   402,   407,   120,    -1,   120,    -1,    -1,   123,   404,
     373,    -1,    -1,   421,   405,   407,   120,    -1,   120,    -1,
     406,   409,    -1,    -1,   407,   411,    -1,    -1,   408,   413,
      -1,    -1,   159,    -1,   160,    -1,   122,    -1,   123,    -1,
     158,    -1,   116,    -1,   120,    -1,   124,    -1,    -1,   118,
     410,   406,   119,    -1,   159,    -1,   160,    -1,   122,    -1,
     123,    -1,   158,    -1,   116,    -1,   124,    -1,    -1,   118,
     412,   406,   119,    -1,   159,    -1,   160,    -1,   122,    -1,
     123,    -1,   158,    -1,   124,    -1,    -1,   118,   414,   406,
     119,    -1,   159,    -1,   160,    -1,   122,    -1,   123,    -1,
     116,    -1,   120,    -1,   124,    -1,    -1,   118,   416,   406,
     119,    -1,   159,    -1,   160,    -1,   123,    -1,   116,    -1,
     120,    -1,   124,    -1,    -1,   118,   418,   406,   119,    -1,
     159,    -1,   122,    -1,   123,    -1,   158,    -1,   116,    -1,
     124,    -1,    -1,   118,   420,   406,   119,    -1,   159,    -1,
     160,    -1,   122,    -1,   158,    -1,   116,    -1,   124,    -1,
      -1,   118,   422,   406,   119,    -1,   159,    -1,   160,    -1,
     122,    -1,   158,    -1,   116,    -1,    -1,   118,   424,   406,
     119,    -1,   425,   426,    -1,   426,    -1,   428,    -1,    -1,
     118,   427,   406,   119,    -1,   159,    -1,   160,    -1,   122,
      -1,   123,    -1,   158,    -1,   116,    -1,   124,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short yyrline[] =
{
       0,   215,   215,   217,   222,   223,   224,   225,   231,   231,
     241,   241,   251,   255,   255,   266,   266,   277,   288,   288,
     293,   298,   331,   335,   339,   340,   341,   342,   350,   360,
     361,   371,   372,   375,   376,   377,   384,   393,   393,   402,
     405,   406,   413,   418,   412,   424,   425,   428,   429,   432,
     436,   440,   443,   443,   448,   448,   455,   456,   459,   459,
     461,   461,   465,   466,   469,   470,   473,   474,   474,   476,
     476,   478,   478,   485,   487,   495,   485,   508,   509,   512,
     516,   517,   517,   519,   522,   523,   534,   542,   533,   551,
     550,   585,   587,   585,   600,   601,   611,   612,   615,   616,
     619,   627,   639,   640,   643,   644,   654,   654,   657,   658,
     661,   668,   668,   671,   671,   674,   676,   678,   680,   681,
     680,   684,   685,   686,   684,   689,   690,   689,   693,   694,
     695,   693,   698,   699,   698,   702,   703,   704,   702,   707,
     712,   718,   717,   724,   724,   727,   738,   739,   742,   743,
     746,   751,   752,   755,   755,   757,   757,   761,   762,   765,
     771,   771,   775,   782,   783,   786,   787,   790,   791,   794,
     800,   800,   807,   808,   811,   812,   815,   816,   819,   823,
     827,   833,   837,   841,   841,   843,   847,   851,   852,   852,
     854,   854,   856,   856,   858,   858,   860,   864,   868,   873,
     873,   875,   879,   883,   887,   891,   891,   893,   894,   900,
     901,   904,   905,   908,   913,   913,   918,   918,   923,   923,
     933,   935,   941,   943,   951,   952,   955,   956,   957,   957,
     964,   964,   966,   967,   970,   970,   972,   973,   976,   976,
     978,   979,   982,   982,   984,   985,   988,   988,   990,   991,
     994,   994,   996,   997,  1000,  1000,  1002,  1003,  1006,  1007,
    1008,  1011,  1011,  1013,  1013,  1015,  1016,  1017,  1023,  1035,
    1036,  1036,  1040,  1040,  1042,  1048,  1049,  1052,  1052,  1052,
    1052,  1053,  1053,  1060,  1060,  1060,  1060,  1060,  1063,  1063,
    1063,  1064,  1064,  1064,  1067,  1067,  1067,  1068,  1068,  1071,
    1071,  1071,  1071,  1072,  1072,  1072,  1072,  1072,  1072,  1075,
    1075,  1075,  1075,  1076,  1076,  1076,  1076,  1076,  1076,  1079,
    1079,  1079,  1079,  1080,  1080,  1080,  1080,  1080,  1080,  1083,
    1083,  1083,  1083,  1084,  1084,  1084,  1084,  1084,  1084,  1087,
    1087,  1087,  1088,  1088,  1088,  1089,  1089,  1089,  1089,  1089,
    1089,  1092,  1092,  1092,  1093,  1093,  1093,  1093,  1093,  1096,
    1096,  1096,  1097,  1097,  1097,  1097,  1098,  1098,  1098,  1098,
    1098,  1101,  1101,  1107,  1107,  1107,  1107,  1107,  1107,  1107,
    1107,  1108,  1108,  1111,  1111,  1111,  1111,  1114,  1114,  1117,
    1117,  1117,  1117,  1117,  1117,  1118,  1118,  1118,  1119,  1119,
    1119,  1119,  1119,  1119,  1119,  1119,  1122,  1122,  1122,  1122,
    1122,  1122,  1122,  1125,  1125,  1125,  1128,  1128,  1128,  1128,
    1128,  1128,  1128,  1129,  1129,  1129,  1129,  1129,  1129,  1130,
    1130,  1130,  1130,  1131,  1131,  1131,  1131,  1132,  1132,  1132,
    1159,  1160,  1159,  1166,  1167,  1166,  1175,  1174,  1188,  1187,
    1201,  1200,  1214,  1213,  1227,  1226,  1241,  1240,  1255,  1254,
    1268,  1267,  1280,  1285,  1284,  1296,  1296,  1305,  1305,  1311,
    1311,  1317,  1317,  1323,  1323,  1329,  1337,  1337,  1344,  1344,
    1351,  1351,  1358,  1358,  1365,  1369,  1373,  1373,  1380,  1380,
    1387,  1387,  1394,  1394,  1401,  1401,  1408,  1408,  1415,  1415,
    1422,  1422,  1429,  1433,  1437,  1441,  1441,  1451,  1455,  1459,
    1463,  1467,  1471,  1475,  1479,  1483,  1487,  1491,  1495,  1496,
    1495,  1501,  1502,  1501,  1507,  1511,  1515,  1519,  1523,  1527,
    1534,  1538,  1542,  1546,  1553,  1557,  1564,  1564,  1571,  1572,
    1573,  1574,  1575,  1576,  1577,  1578,  1579,  1580,  1583,  1584,
    1585,  1586,  1587,  1588,  1589,  1590,  1591,  1592,  1595,  1596,
    1599,  1600,  1603,  1604,  1607,  1608,  1611,  1612,  1615,  1616,
    1619,  1620,  1623,  1624,  1625,  1633,  1633,  1635,  1635,  1639,
    1640,  1639,  1642,  1651,  1651,  1653,  1653,  1655,  1655,  1660,
    1660,  1662,  1662,  1664,  1669,  1668,  1674,  1674,  1676,  1685,
    1685,  1687,  1687,  1689,  1689,  1694,  1694,  1696,  1696,  1698,
    1703,  1702,  1708,  1708,  1710,  1710,  1712,  1716,  1716,  1718,
    1718,  1720,  1725,  1724,  1731,  1731,  1733,  1739,  1740,  1743,
    1744,  1748,  1749,  1755,  1755,  1755,  1755,  1755,  1755,  1755,
    1755,  1756,  1756,  1760,  1760,  1760,  1760,  1760,  1760,  1760,
    1761,  1761,  1766,  1766,  1766,  1766,  1766,  1766,  1767,  1767,
    1771,  1771,  1771,  1771,  1771,  1771,  1771,  1772,  1772,  1776,
    1776,  1776,  1776,  1776,  1776,  1777,  1777,  1782,  1782,  1782,
    1782,  1782,  1782,  1783,  1783,  1788,  1788,  1788,  1788,  1788,
    1788,  1789,  1789,  1794,  1794,  1794,  1794,  1794,  1795,  1795,
    1802,  1803,  1806,  1812,  1812,  1820,  1820,  1820,  1820,  1820,
    1820,  1820
};
#endif

#if YYDEBUG || YYERROR_VERBOSE
/* YYTNME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "FILE_NAME_STRING", "UNINTERPRETED_STRING", 
  "LATEX_STRING", "KW_MOD", "KW_OMOD", "KW_VIEW", "KW_PARSE", 
  "KW_NORMALIZE", "KW_REDUCE", "KW_REWRITE", "KW_LOOP", "KW_NARROW", 
  "KW_MATCH", "KW_XMATCH", "KW_UNIFY", "KW_XUNIFY", "KW_EREWRITE", 
  "KW_FREWRITE", "KW_OREWRITE", "KW_CONTINUE", "KW_SEARCH", "KW_SET", 
  "KW_SHOW", "KW_ON", "KW_OFF", "KW_TRACE", "KW_BODY", "KW_WHOLE", 
  "KW_SELECT", "KW_DESELECT", "KW_CONDITION", "KW_SUBSTITUTION", 
  "KW_PRINT", "KW_GRAPH", "KW_MIXFIX", "KW_FLAT", "KW_WITH", "KW_PARENS", 
  "KW_ALIASES", "KW_GC", "KW_TIME", "KW_STATS", "KW_TIMING", "KW_CMD", 
  "KW_BREAKDOWN", "KW_BREAK", "KW_PATH", "KW_MODULE", "KW_MODULES", 
  "KW_VIEWS", "KW_ALL", "KW_SORTS", "KW_OPS", "KW_VARS", "KW_MBS", 
  "KW_EQS", "KW_RLS", "KW_SUMMARY", "KW_KINDS", "KW_ADVISE", "KW_VERBOSE", 
  "KW_DO", "KW_CLEAR", "KW_PROTECT", "KW_EXTEND", "KW_INCLUDE", 
  "KW_EXCLUDE", "KW_CONCEAL", "KW_REVEAL", "KW_COMPILE", "KW_COUNT", 
  "KW_DEBUG", "KW_RESUME", "KW_ABORT", "KW_STEP", "KW_WHERE", 
  "KW_CREDUCE", "KW_DUMP", "KW_PROFILE", "KW_NUMBER", "KW_RAT", 
  "KW_COLOR", "SIMPLE_NUMBER", "KW_PWD", "KW_CD", "KW_PUSHD", "KW_POPD", 
  "KW_LS", "KW_LOAD", "KW_QUIT", "KW_EOF", "KW_ENDM", "KW_IMPORT", 
  "KW_ENDV", "KW_SORT", "KW_SUBSORT", "KW_OP", "KW_MSGS", "KW_VAR", 
  "KW_CLASS", "KW_SUBCLASS", "KW_MB", "KW_CMB", "KW_EQ", "KW_CEQ", 
  "KW_RL", "KW_CRL", "KW_IS", "KW_FROM", "KW_ARROW", "KW_ARROW2", 
  "KW_PARTIAL", "KW_IF", "':'", "'='", "'('", "')'", "'.'", "'<'", "'['", 
  "']'", "','", "'|'", "KW_LABEL", "KW_TO", "KW_COLON2", "'+'", "'*'", 
  "'{'", "'}'", "KW_ASSOC", "KW_COMM", "KW_ID", "KW_IDEM", "KW_ITER", 
  "KW_LEFT", "KW_RIGHT", "KW_PREC", "KW_GATHER", "KW_METADATA", 
  "KW_STRAT", "KW_POLY", "KW_MEMO", "KW_FROZEN", "KW_CTOR", "KW_LATEX", 
  "KW_SPECIAL", "KW_CONFIG", "KW_OBJ", "KW_MSG", "KW_DITTO", "KW_FORMAT", 
  "KW_ID_HOOK", "KW_OP_HOOK", "KW_TERM_HOOK", "KW_IN", "IDENTIFIER", 
  "NUMERIC_ID", "ENDS_IN_DOT", "$accept", "top", "item", "directive", 
  "@1", "@2", "@3", "@4", "@5", "moduleExprDot", "moduleExpr", 
  "moduleExpr2", "moduleExpr3", "renameExpr", "instantExpr", "@6", 
  "parenExpr", "argList", "renaming", "@7", "@8", "renaming2", 
  "mappingList", "mapping", "fromOpName", "@9", "@10", "fromSpec", 
  "toOpName", "@11", "@12", "toAttributes", "toAttributeList", 
  "toAttribute", "@13", "@14", "@15", "view", "@16", "@17", "@18", 
  "viewDecList", "viewDeclaration", "@19", "sortDot", "viewEndOpMap", 
  "@20", "@21", "@22", "module", "@23", "@24", "dot", "parameters", 
  "parameterList", "parameter", "badType", "typeDot", "endStatement", 
  "startModule", "decList", "declaration", "@25", "@26", "@27", "@28", 
  "@29", "@30", "@31", "@32", "@33", "@34", "@35", "@36", "@37", "@38", 
  "@39", "@40", "@41", "@42", "@43", "classDef", "cPairList", "cPair", 
  "varNameList", "opName", "@44", "@45", "opNameList", "simpleOpName", 
  "@46", "domainRangeAttr", "typeAttr", "arrow", "typeList", "typeName", 
  "@47", "sortNames", "attributes", "attributeList", "idKeyword", 
  "attribute", "@48", "@49", "@50", "@51", "@52", "@53", "@54", "idList", 
  "hookList", "hook", "@55", "@56", "@57", "expectedIs", "expectedDot", 
  "sortNameList", "subsortList", "@58", "tokens", "@59", "tokensBarColon", 
  "@60", "tokensBarColonTo", "@61", "tokensBarCommaLeft", "@62", 
  "tokensBarEqual", "@63", "tokensBarArrow2", "@64", "tokensBarIf", "@65", 
  "endTokens", "noTrailingDot", "@66", "@67", "sortName", "sortNameFrag", 
  "@68", "sortNameFrags", "@69", "identity", "identityChunk", "@70", 
  "token", "tokenBarDot", "endToken", "tokenBarArrow2", "tokenBarEqual", 
  "tokenBarIf", "tokenBarColon", "tokenBarColonTo", "tokenBarCommaLeft", 
  "sortToken", "endsInDot", "inert", "identifier", "startKeyword", 
  "startKeyword2", "midKeyword", "attrKeyword", "attrKeyword2", "command", 
  "@71", "@72", "@73", "@74", "@75", "@76", "@77", "@78", "@79", "@80", 
  "@81", "@82", "@83", "@84", "@85", "@86", "@87", "@88", "@89", "@90", 
  "@91", "@92", "@93", "@94", "@95", "@96", "@97", "@98", "@99", "@100", 
  "@101", "@102", "@103", "@104", "@105", "@106", "printOption", 
  "traceOption", "polarity", "select", "exclude", "conceal", "match", 
  "optDebug", "optNumber", "importMode", "moduleAndTerm", "@107", "@108", 
  "inEnd", "@109", "@110", "numberModuleTerm", "@111", "@112", "@113", 
  "numberModuleTerm1", "@114", "@115", "numberModuleTerm2", "@116", 
  "@117", "numbersModuleTerm", "@118", "@119", "@120", 
  "numbersModuleTerm1", "@121", "@122", "numbersModuleTerm2", "@123", 
  "@124", "@125", "numbersModuleTerm3", "@126", "@127", 
  "numbersModuleTerm4", "@128", "@129", "cTokens", "cTokensBarDot", 
  "cTokensBarDotColon", "cToken", "@130", "cTokenBarDot", "@131", 
  "cTokenBarDotColon", "@132", "cTokenBarIn", "@133", "cTokenBarLeftIn", 
  "@134", "cTokenBarDotNumber", "@135", "cTokenBarDotRight", "@136", 
  "cTokenBarDotCommaRight", "@137", "cOpNameList", "cSimpleOpName", 
  "@138", "cSimpleTokenBarDot", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short yytoknum[] =
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
     365,   366,   367,   368,   369,   370,    58,    61,    40,    41,
      46,    60,    91,    93,    44,   124,   371,   372,   373,    43,
      42,   123,   125,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short yyr1[] =
{
       0,   162,   163,   163,   164,   164,   164,   164,   166,   165,
     167,   165,   165,   168,   165,   169,   165,   165,   170,   165,
     165,   165,   171,   171,   171,   171,   171,   171,   171,   172,
     172,   173,   173,   174,   174,   174,   175,   177,   176,   178,
     179,   179,   181,   182,   180,   183,   183,   184,   184,   185,
     185,   185,   187,   186,   188,   186,   189,   189,   191,   190,
     192,   190,   193,   193,   194,   194,   195,   196,   195,   197,
     195,   198,   195,   200,   201,   202,   199,   203,   203,   204,
     204,   205,   204,   204,   206,   206,   208,   209,   207,   210,
     207,   212,   213,   211,   214,   214,   215,   215,   216,   216,
     217,   218,   219,   219,   220,   220,   221,   221,   222,   222,
     223,   224,   223,   225,   223,   223,   223,   223,   226,   227,
     223,   228,   229,   230,   223,   231,   232,   223,   233,   234,
     235,   223,   236,   237,   223,   238,   239,   240,   223,   223,
     223,   241,   223,   242,   223,   223,   243,   243,   244,   244,
     245,   246,   246,   248,   247,   249,   247,   250,   250,   251,
     252,   251,   253,   254,   254,   255,   255,   256,   256,   257,
     258,   257,   259,   259,   260,   260,   261,   261,   262,   262,
     262,   263,   263,   264,   263,   263,   263,   263,   265,   263,
     266,   263,   267,   263,   268,   263,   263,   263,   263,   269,
     263,   263,   263,   263,   263,   270,   263,   263,   263,   271,
     271,   272,   272,   273,   274,   273,   275,   273,   276,   273,
     277,   277,   278,   278,   279,   279,   280,   280,   281,   280,
     283,   282,   282,   282,   285,   284,   284,   284,   287,   286,
     286,   286,   289,   288,   288,   288,   291,   290,   290,   290,
     293,   292,   292,   292,   295,   294,   294,   294,   296,   296,
     296,   298,   297,   299,   297,   297,   297,   297,   300,   301,
     302,   301,   304,   303,   303,   305,   305,   306,   306,   306,
     306,   307,   306,   308,   308,   308,   308,   308,   309,   309,
     309,   309,   309,   309,   310,   310,   310,   310,   310,   311,
     311,   311,   311,   311,   311,   311,   311,   311,   311,   312,
     312,   312,   312,   312,   312,   312,   312,   312,   312,   313,
     313,   313,   313,   313,   313,   313,   313,   313,   313,   314,
     314,   314,   314,   314,   314,   314,   314,   314,   314,   315,
     315,   315,   315,   315,   315,   315,   315,   315,   315,   315,
     315,   316,   316,   316,   316,   316,   316,   316,   316,   317,
     317,   317,   317,   317,   317,   317,   317,   317,   317,   317,
     317,   318,   318,   319,   319,   319,   319,   319,   319,   319,
     319,   319,   319,   320,   320,   320,   320,   321,   321,   322,
     322,   322,   322,   322,   322,   322,   322,   322,   322,   322,
     322,   322,   322,   322,   322,   322,   323,   323,   323,   323,
     323,   323,   323,   324,   324,   324,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     325,   325,   325,   325,   325,   325,   325,   325,   325,   325,
     327,   328,   326,   329,   330,   326,   331,   326,   332,   326,
     333,   326,   334,   326,   335,   326,   336,   326,   337,   326,
     338,   326,   326,   339,   326,   340,   326,   341,   326,   342,
     326,   343,   326,   344,   326,   326,   345,   326,   346,   326,
     347,   326,   348,   326,   326,   326,   349,   326,   350,   326,
     351,   326,   352,   326,   353,   326,   354,   326,   355,   326,
     356,   326,   326,   326,   326,   357,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   326,   326,   358,   359,
     326,   360,   361,   326,   326,   326,   326,   326,   326,   326,
     326,   326,   326,   326,   326,   326,   362,   326,   363,   363,
     363,   363,   363,   363,   363,   363,   363,   363,   364,   364,
     364,   364,   364,   364,   364,   364,   364,   364,   365,   365,
     366,   366,   367,   367,   368,   368,   369,   369,   370,   370,
     371,   371,   372,   372,   372,   374,   373,   375,   373,   377,
     378,   376,   376,   380,   379,   381,   379,   382,   379,   384,
     383,   385,   383,   383,   387,   386,   388,   386,   386,   390,
     389,   391,   389,   392,   389,   394,   393,   395,   393,   393,
     397,   396,   398,   396,   399,   396,   396,   401,   400,   402,
     400,   400,   404,   403,   405,   403,   403,   406,   406,   407,
     407,   408,   408,   409,   409,   409,   409,   409,   409,   409,
     409,   410,   409,   411,   411,   411,   411,   411,   411,   411,
     412,   411,   413,   413,   413,   413,   413,   413,   414,   413,
     415,   415,   415,   415,   415,   415,   415,   416,   415,   417,
     417,   417,   417,   417,   417,   418,   417,   419,   419,   419,
     419,   419,   419,   420,   419,   421,   421,   421,   421,   421,
     421,   422,   421,   423,   423,   423,   423,   423,   424,   423,
     425,   425,   426,   427,   426,   428,   428,   428,   428,   428,
     428,   428
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
       0,     4,     4,     3,     1,     1,     1,     2,     0,     1,
       0,     4,     3,     1,     3,     0,     2,     1,     1,     2,
       2,     1,     1,     0,     3,     1,     1,     2,     0,     5,
       0,     5,     0,     5,     0,     5,     1,     1,     1,     0,
       5,     1,     1,     1,     2,     0,     5,     4,     1,     2,
       1,     2,     1,     2,     0,     6,     0,     6,     0,     6,
       1,     0,     1,     0,     2,     0,     2,     2,     0,     4,
       0,     5,     2,     0,     0,     5,     2,     0,     0,     5,
       2,     0,     0,     5,     2,     0,     0,     5,     2,     0,
       0,     5,     2,     0,     0,     5,     2,     0,     1,     2,
       1,     0,     4,     0,     5,     2,     2,     1,     1,     1,
       0,     5,     0,     4,     1,     1,     2,     1,     1,     1,
       1,     0,     4,     1,     1,     1,     1,     1,     1,     1,
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
       0,     0,     6,     0,     0,     6,     0,     3,     0,     3,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     3,
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
       1,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       1,     0,     1,     1,     1,     0,     4,     0,     4,     0,
       0,     6,     1,     0,     3,     0,     4,     0,     4,     0,
       3,     0,     4,     1,     0,     3,     0,     4,     1,     0,
       3,     0,     4,     0,     4,     0,     3,     0,     4,     1,
       0,     3,     0,     3,     0,     4,     1,     0,     3,     0,
       4,     1,     0,     3,     0,     4,     1,     2,     0,     2,
       0,     2,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     0,     4,     1,     1,     1,     1,     1,     1,     1,
       0,     4,     1,     1,     1,     1,     1,     1,     0,     4,
       1,     1,     1,     1,     1,     1,     1,     0,     4,     1,
       1,     1,     1,     1,     1,     0,     4,     1,     1,     1,
       1,     1,     1,     0,     4,     1,     1,     1,     1,     1,
       1,     0,     4,     1,     1,     1,     1,     1,     0,     4,
       2,     1,     1,     0,     4,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short yydefact[] =
{
       0,   536,   106,   107,    73,   446,   463,   567,   566,   458,
       0,     0,     0,   440,     0,     0,     0,   568,     0,     0,
       0,     0,   448,   443,    12,    13,    15,    17,    18,    10,
      20,    21,   465,     8,     0,     2,     6,     5,     4,    91,
       7,   460,     0,     0,     0,     0,     0,     0,     0,     0,
     548,     0,     0,     0,     0,     0,     0,   572,   573,   574,
       0,     0,   518,   476,   482,     0,     0,   478,     0,     0,
     480,   486,   488,   490,   492,   494,   496,   498,   500,   505,
     560,   561,   563,   562,   467,   469,     0,   564,   565,   473,
     471,     0,   530,   531,   532,   533,     0,     0,     0,     0,
       0,     0,   628,     0,     1,     0,     0,   450,   452,   454,
     456,   571,   537,   393,   404,   389,   405,   390,   391,   392,
     395,   394,   396,   397,   398,   399,   400,   401,   402,   403,
     382,   381,   408,   411,   409,   412,   407,   410,   287,   406,
     413,   414,   385,   378,   380,   386,   379,   376,   377,   374,
     375,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     432,   425,   426,   427,   431,   428,   429,   430,   433,   434,
     387,   435,   436,   437,   438,   439,   373,   384,     0,   383,
     283,   284,   388,   285,   286,   415,   664,   667,   665,   662,
     663,   666,   575,   660,   661,   447,   577,   464,   672,   675,
     673,   583,   671,   674,   585,   669,   670,   459,   587,   521,
       0,     0,     0,     0,     0,     0,     0,   556,   557,   550,
     552,   549,   551,   553,   554,   555,     0,   542,   538,   539,
       0,   543,   544,   545,   546,   547,     0,     0,   558,   559,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   630,
     630,     0,     0,     0,   630,   484,   485,   630,   630,   630,
     630,   630,   630,   630,   630,   630,   630,     0,     0,   638,
     641,   639,   635,   636,   640,   637,   633,   634,   441,     0,
       0,     0,   449,   444,    14,    16,    19,    11,     0,     9,
      92,   461,     0,     0,     0,     0,   570,     0,     0,   628,
     632,   630,   628,     0,   632,   630,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   541,   540,     0,     0,
     535,   517,   524,     0,     0,     0,     0,   528,   710,   707,
     708,   711,   709,   705,   706,   519,     0,     0,   504,   502,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   703,     0,   701,   702,     0,   628,   630,     0,
       0,   475,   630,   466,   627,    97,   451,   453,   455,   599,
     601,   457,   603,   462,     0,    74,    29,    31,    32,    34,
      33,    35,     0,     0,     0,     0,   681,   683,   593,   678,
     679,   682,   680,   677,   589,   584,   591,     0,     0,   522,
       0,     0,   514,   508,   510,   513,   511,   507,   516,   515,
     534,   526,   529,   525,   527,     0,   648,   650,   477,   645,
     646,   649,   647,   643,   644,   629,   483,   503,   479,   481,
     487,   489,   491,   493,   495,   497,   499,   501,   506,   628,
     468,   700,   470,     0,     0,   474,   472,     0,     0,   221,
       0,   632,   630,     0,     0,     0,     0,    37,   668,   579,
     658,   582,   654,   655,   657,   656,   652,   653,   576,   631,
     578,   676,   628,     0,   630,   586,   588,     0,   509,   512,
       0,   628,     0,   642,   442,   445,     0,    99,     0,   220,
     109,   609,   605,   600,   607,     0,     0,    39,    30,     0,
      42,    36,     0,     0,   628,     0,   689,   691,   598,   687,
     594,   690,   688,   685,   686,   590,   596,     0,     0,   520,
       0,   704,     0,    96,     0,     0,     0,   630,   602,   604,
      75,     0,     0,    41,   580,     0,   684,   628,     0,   630,
     592,   523,   651,    98,   100,     0,     0,    93,     0,   111,
     113,     0,     0,     0,     0,   143,   118,   121,   125,   128,
     132,   135,     0,   108,   697,   698,   616,   695,   610,   612,
     696,   693,   694,   606,   614,     0,   221,    46,     0,     0,
       0,    43,    45,    48,     0,    38,   630,   659,     0,   595,
       0,   145,   334,   337,   335,   338,   336,   160,   332,   333,
       0,   158,   159,   329,   330,   331,   287,   289,   290,    28,
     110,     0,   223,   223,   223,   223,     0,   288,   291,   292,
     293,   225,     0,   155,     0,   153,     0,     0,   152,   141,
       0,   237,   237,   249,   249,   253,   253,     0,   628,     0,
       0,   630,   608,    78,   368,   366,   367,   362,   363,   369,
     370,   364,   365,   359,     0,   268,   269,   360,   361,    54,
      57,    52,     0,     0,     0,    40,     0,   692,   597,   233,
     168,   157,   116,     0,   222,    25,    26,    24,    22,    23,
       0,     0,   228,   233,   115,   237,   140,     0,   151,     0,
       0,     0,     0,     0,     0,     0,     0,   139,     0,   611,
     621,   617,   613,   619,     0,     0,     0,   270,   233,   168,
       0,   241,     0,    44,    47,   581,     0,     0,    27,    30,
      94,    95,   112,   224,   227,   114,   226,   225,     0,   154,
     170,   101,   103,   117,   223,   169,   146,     0,   144,   119,
     234,   236,   122,   315,   317,   316,   318,   314,   126,   246,
     312,   313,   248,   309,   310,   311,   129,   305,   133,   306,
     308,   304,   307,   250,   302,   303,   252,   299,   300,   301,
     136,   699,     0,   630,   615,     0,    76,     0,    81,     0,
      77,    49,     0,     0,     0,     0,    53,    50,   230,   161,
     232,   165,   166,     0,   167,     0,   156,     0,   102,   289,
     290,   147,   148,     0,   288,   291,   292,   293,   142,     0,
     233,   257,     0,   233,   257,     0,   233,   257,   626,   622,
     618,   624,     0,    83,     0,   241,     0,   274,     0,    55,
       0,    60,    63,    58,   346,   349,   347,   350,   348,   238,
     344,   345,   341,   340,   240,   339,   342,   343,   233,   164,
     162,   175,   229,     0,   173,     0,     0,   261,   371,   372,
     120,     0,   258,   267,   260,     0,     0,   127,     0,     0,
     134,     0,     0,     0,   630,   620,     0,     0,     0,   272,
     271,    56,   233,     0,    51,   245,   233,     0,     0,   223,
     171,     0,   149,   150,   233,   263,   371,   295,   296,   372,
     104,   266,   259,   294,   297,   298,   265,   235,   325,   328,
     326,   123,   324,   327,   254,   322,   323,   256,   319,   320,
     321,   247,   130,   251,   137,   623,     0,    85,    79,   223,
      86,    89,    82,    80,     0,     0,     0,     0,     0,     0,
       0,    65,    59,     0,   231,   181,   182,   178,   185,   186,
       0,     0,     0,     0,     0,     0,     0,   196,   198,   197,
       0,     0,   201,   202,   203,   208,     0,     0,   183,   177,
     163,   172,     0,   233,     0,   233,     0,     0,   625,    84,
     168,     0,   273,    61,    66,    67,    71,    69,    62,    64,
     242,   356,   357,   353,   352,   244,   351,   354,   358,   355,
     239,   179,   180,   187,   188,   204,   192,   194,   199,   205,
       0,   190,   174,   176,     0,   262,     0,   124,     0,   131,
     138,     0,    90,     0,     0,     0,   233,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   212,     0,   281,   280,
     184,   275,   277,   278,   279,   264,   255,     0,   210,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   213,     0,
       0,   207,   211,     0,   233,   276,     0,    68,   209,    72,
      70,   243,   189,   193,   195,   200,   206,   214,   216,   218,
     191,     0,    87,   233,   233,   233,   282,     0,     0,     0,
       0,    88,   215,   217,   219
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short yydefgoto[] =
{
      -1,    34,    35,    36,   103,   101,    98,    99,   100,   610,
     375,   376,   377,   378,   379,   502,   380,   532,   501,   531,
     663,   581,   582,   583,   660,   711,   708,   710,   832,   885,
     882,   884,   940,   941,  1023,  1025,  1024,    37,    44,   455,
     576,   705,   780,   825,   928,   932,   980,  1087,   981,    38,
     105,   365,   722,   449,   486,   487,   732,   733,   860,    39,
     525,   563,   621,   622,   631,   809,   632,   811,   974,   633,
     812,   634,   814,   976,   635,   815,   636,   817,   977,   689,
     630,   737,   801,   802,   627,   624,   685,   683,   600,   601,
     669,   672,   850,   793,   717,   794,   797,   853,   889,   967,
     968,   969,  1014,  1027,  1037,  1028,  1029,  1030,  1031,  1049,
    1035,  1036,  1083,  1084,  1085,   490,   675,   680,   681,   727,
     716,   848,   691,   810,   786,   886,   942,  1026,   693,   813,
     695,   816,   866,   975,   861,   862,   894,   973,   735,   655,
     782,   828,   934,  1040,  1041,  1064,   790,   863,   901,   766,
     752,   917,   602,   844,   995,   656,   864,   179,   180,   181,
     182,   183,   184,   185,    40,    86,   358,    97,   362,    45,
      96,   292,   293,   294,   295,    47,   106,    46,   102,   267,
     268,   280,   279,   249,   254,   257,   250,   258,   259,   260,
     261,   262,   263,   264,   265,   266,   248,   415,   306,   477,
      43,   236,   226,   240,    84,    85,    89,    41,    42,   297,
      62,   195,   300,   301,   468,   503,   586,   207,   303,   304,
     305,   395,   473,   474,   515,   538,   539,   371,   450,   451,
     452,   493,   526,   527,   573,   639,   640,   641,   702,   772,
     773,   820,   873,   874,   288,   336,   383,   364,   357,   425,
     481,   469,   504,   196,   299,   208,   302,   396,   472,   516,
     537,   574,   638,   353,   354,   439,   355
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -881
static const short yypact[] =
{
     711,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    5990,  5952,   175,  -881,   195,   217,     5,  -881,    -4,     7,
       9,    23,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,   118,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,   333,    32,  4399,  6004,  6004,  6013,    65,   574,
      47,   483,   143,   246,   246,   246,   -13,  -881,  -881,  -881,
     101,   246,  -881,  -881,  -881,   148,    -6,  -881,    67,    74,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  6022,  -881,  -881,  -881,
    -881,    51,  -881,  -881,  -881,  -881,  6004,  6022,   214,   228,
     231,   242,  -881,   260,  -881,  4399,  6013,  -881,  -881,  -881,
    -881,   211,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,   174,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
     248,   246,   246,   246,   246,   246,   246,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,   246,  -881,  -881,  -881,
     291,  -881,  -881,  -881,  -881,  -881,   246,   246,  -881,  -881,
     198,   206,   209,   246,   246,   246,   246,   223,   531,  -881,
    -881,   226,   236,   235,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,   283,   283,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,   283,
     283,   238,  -881,  -881,  -881,  -881,  -881,  -881,   522,  -881,
    -881,  -881,  6004,  6013,  6013,  6031,  -881,   240,  2648,  -881,
    -881,  -881,  -881,  6076,  -881,  -881,   531,   246,   246,   250,
     254,   255,   263,   266,   268,   278,  -881,  -881,   282,   288,
    -881,  -881,  -881,   289,   292,   293,   300,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  6085,  6094,  -881,  -881,
     301,  6103,  6148,  6157,  6166,  6175,  6220,  6229,  6238,  6247,
    6292,  6301,  -881,  6310,  -881,  -881,  6319,  -881,  -881,  6364,
    6373,  -881,  -881,  -881,  -881,   237,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  2648,   208,   294,   295,  -881,  -881,
    -881,  -881,   658,  6382,  6391,   691,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  6382,  6436,  -881,
     303,   312,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,   246,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,   772,  6445,  -881,  -881,  6454,  4399,   323,
    6463,  -881,  -881,   -47,  2648,   307,   319,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  6508,  -881,  -881,  -881,   246,  -881,  -881,
     318,  -881,   818,  -881,  -881,  -881,    73,  -881,   316,  -881,
    -881,  -881,  -881,  -881,  -881,  6382,  6517,  -881,  -881,  2648,
    -881,  -881,  4399,  6022,  -881,   827,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  6526,   320,  -881,
    5458,  -881,  4399,  -881,  2648,   476,  6535,  -881,  -881,  -881,
     208,    43,    79,  -881,  -881,  5561,  -881,  -881,  6004,  -881,
    -881,  -881,  -881,  -881,   208,   325,  4502,  -881,  2751,  -881,
    -881,  2854,  4502,  4914,  4399,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  2854,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  6580,   323,  -881,  5800,  2957,
    4399,  -881,   326,  -881,  4399,  -881,  -881,  -881,  5861,  -881,
    6589,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    3060,  -881,  -881,  -881,  -881,  -881,   327,    18,    99,    70,
    -881,   322,   -65,   178,   265,   334,   336,   126,   159,   261,
     287,  -881,  5800,  -881,   341,  -881,  3060,  4605,  -881,  -881,
    5800,  -881,  -881,  -881,  -881,  -881,  -881,   341,  -881,  6004,
    6598,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,   337,   329,  -881,  -881,  -881,  -881,
     347,  -881,   340,   349,   -43,  -881,  6607,  -881,  -881,  -881,
    -881,  -881,  -881,  2751,  -881,  -881,  -881,  -881,  -881,  -881,
    5226,  5158,  -881,  -881,  -881,  -881,  -881,  5294,  -881,   345,
    5158,  3163,  3266,  3369,  3472,  3575,  3678,  -881,  5870,  -881,
    -881,  -881,  -881,  -881,  6652,    40,  5800,  -881,  -881,  -881,
     344,  -881,  4399,  -881,  -881,  -881,   897,  5601,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  1000,  4708,
    -881,  -881,  -881,  -881,   334,  -881,  5498,   354,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  6661,  -881,  -881,   355,  -881,  5800,  -881,  4914,
    -881,  -881,  5800,  1103,  5601,  3781,  5017,  -881,  -881,  -881,
    -881,  -881,  -881,  5294,  -881,  5667,  -881,  5800,  -881,  -881,
    -881,   359,  -881,   369,  -881,  -881,  -881,  -881,  -881,  3884,
    -881,  -881,  3884,  -881,  -881,  3884,  -881,  -881,  -881,  -881,
    -881,  -881,  6670,  -881,   361,  -881,  4811,   329,   109,  -881,
    5734,  -881,   370,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,   372,  -881,   200,  -881,  5498,  4399,  -881,  -881,   737,
    -881,  5962,    -8,  -881,  -881,  1206,  3987,  -881,  1309,  4090,
    -881,  1412,  4193,  6004,  -881,  -881,  5362,  4296,  5294,  -881,
    -881,  -881,  -881,   120,  -881,  -881,  -881,  1515,  1029,   334,
    -881,  5800,  -881,  -881,  -881,  -881,   804,  -881,  -881,   820,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  6679,  -881,  -881,   334,
    -881,  -881,  -881,  -881,  5800,  1618,   338,   382,   384,   385,
     194,  -881,  5089,  1721,  -881,  -881,  -881,  -881,  -881,  -881,
     373,   374,   346,   393,   368,   395,   396,  -881,   410,  -881,
     411,   412,  -881,  -881,  -881,  -881,   415,   934,  -881,  -881,
    -881,  -881,  1824,  -881,  3884,  -881,  3884,  3884,  -881,  -881,
    -881,  3884,   329,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
     273,  -881,  -881,  -881,  5430,  -881,  1927,  -881,  2030,  -881,
    -881,  5601,  -881,   375,   532,   375,  -881,   375,   375,   375,
     375,   533,  4399,  4399,  4399,    96,  -881,   375,  -881,  -881,
    5430,  -881,  -881,  -881,  -881,  -881,  -881,  5734,  -881,   -50,
     420,   -35,  2133,    -2,     0,     2,    11,   423,   425,   426,
     427,  -881,  -881,    12,  -881,  -881,   419,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  2236,  -881,  -881,  -881,  -881,  -881,  3884,  2339,  2442,
    2545,  -881,  -881,  -881,  -881
};

/* YYPGOTO[NTERM-NUM].  */
static const short yypgoto[] =
{
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -125,
    -332,  -881,  -881,  -520,  -519,  -881,  -509,  -881,  -881,  -881,
    -881,  -881,  -881,  -115,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -389,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -638,  -881,  -881,    34,  -239,  -321,  -788,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -294,  -215,     6,  -881,  -881,    17,  -560,
    -881,  -564,  -881,  -762,  -705,  -680,  -881,  -881,  -881,  -881,
    -881,  -379,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -429,
    -881,  -446,  -881,  -881,  -881,    14,  -579,  -135,   -27,  -881,
    -413,  -881,  -583,  -881,  -230,  -881,  -881,  -881,   -29,  -881,
     -26,  -881,  -743,  -881,  -881,  -881,  -881,  -881,  -555,  -752,
    -881,  -881,  -881,  -881,  -433,  -881,   -44,  -527,  -881,  -881,
    -881,  -881,  -415,  -881,  -881,  -881,  -545,  -528,  -390,  -515,
    -880,  -490,  -314,  -191,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,   462,   594,  -881,  -881,  -881,  -881,  -881,
    -881,   -45,  -881,  -881,  -372,  -881,  -881,   -74,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,  -881,
    -881,  -881,  -881,  -881,  -254,  -248,  -278,   -78,  -881,  -881,
    -881,  -881,  -881,  -881,  -881,   317,  -881,  -445,  -881,  -161,
    -881,  -881,  -881,   -55,    13,  -881,  -198
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -570
static const short yytable[] =
{
     178,   197,   337,   616,   784,   494,   341,   734,   278,   342,
     343,   344,   345,   346,   347,   348,   349,   350,   351,   283,
     617,   615,   830,   654,   867,   475,   397,   870,   612,   613,
     827,   604,   291,   618,   676,   677,   678,   604,   604,   614,
     671,   775,   453,   725,   577,   382,   243,   113,   385,   692,
     335,   282,   738,   384,   578,   674,   579,   398,   619,   217,
     684,   290,   686,   657,   -32,   -32,   671,   682,   244,  1067,
      91,   869,   497,   697,   872,   682,   218,   219,   220,   252,
     221,   222,   454,   580,  1070,   604,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   729,   443,   223,   224,   225,   657,   399,  1068,
     444,   604,   604,   851,   447,   657,    92,  1072,   104,  1073,
     253,  1074,   498,   528,  1068,   723,   726,    93,   616,    94,
    1075,  1080,   245,   209,  1043,   726,   776,   777,   628,   778,
     578,   779,   579,    95,   170,   617,   615,  -385,  -385,  -385,
     881,   781,   112,   612,   613,   798,   603,  1068,   618,  1068,
    1043,  1068,   603,   603,   614,   657,   657,   530,   237,   580,
    1068,  1068,   657,   495,   246,   657,   604,   604,   754,   754,
     768,   768,   982,   619,   251,   482,  1017,   255,  1019,  1020,
    -372,   657,   544,  1022,   256,   703,   281,   522,   734,  -384,
    -384,  -384,   657,   584,   496,   523,    80,    81,   804,   803,
     603,   585,   688,   356,   604,  1061,   611,   284,   505,   367,
     368,   805,   824,   900,   359,   360,   517,   520,  -386,  -386,
    -386,   285,   605,   879,   620,   286,   603,   603,   605,   605,
     723,   880,   854,    82,    83,   287,   806,   366,    80,    81,
     535,  1032,  1033,  1034,   381,  -383,  -383,  -383,   845,  1047,
     936,   937,   657,   289,   604,    87,    88,   657,   938,   657,
     728,   846,   238,   239,   939,  1021,   741,   741,   657,   575,
     657,   804,   657,   588,   804,   298,   605,   804,  -284,  -284,
    -284,   590,   307,   308,   805,   783,   296,   805,   674,  1091,
     805,   603,   603,   753,   753,   767,   767,   -34,   -34,   -34,
     970,   604,   605,   605,   741,   657,   902,   988,   320,   806,
     340,   929,   806,   890,   891,   806,   321,   804,   803,   322,
     381,   316,   317,   903,   936,   937,   971,   454,   666,   603,
     805,   719,   938,   327,   107,   108,   338,   906,   939,   845,
     979,   919,   109,   110,   919,   111,   339,   919,   361,   620,
     373,   657,   846,   657,   628,   806,   441,  1066,   448,   441,
     402,   904,   441,   441,   403,   404,   657,   605,   605,   755,
     755,   769,   769,   405,   698,   674,   406,   658,   407,   603,
    -285,  -285,  -285,   704,   -33,   -33,   -33,   865,   408,   328,
     868,   352,   409,   871,   488,   329,   330,   331,   410,   411,
     381,   688,   412,   413,   996,   605,  -286,  -286,  -286,   657,
     414,   427,   807,   478,   456,   534,   457,   997,  1032,  1033,
    1034,   658,   479,   489,   499,   887,   603,   500,   519,   658,
     541,   332,   333,   334,   524,   591,   804,  -371,   804,   804,
     664,   673,   998,   804,   674,   381,   679,   670,   533,   805,
     707,   805,   805,   709,   706,   605,   805,   712,   713,   935,
     736,   785,   847,   943,   808,   823,   918,   545,   488,   918,
     381,   972,   918,   855,   806,   856,   806,   806,   876,   658,
     658,   806,   883,   589,   888,   807,   658,   984,   807,   658,
     985,   807,   986,   987,   381,  1003,   657,   625,  1001,  1002,
     629,  1004,   605,  1006,  1007,   658,   241,   242,   625,   227,
     228,   229,   230,   247,  1044,   822,   658,  1005,  1008,  1009,
    1010,   546,   657,  1011,  1048,   661,   662,  1050,  1057,  1069,
     665,   807,  1076,  1077,  1078,  1079,  1082,   905,   718,   714,
    1044,   989,   920,   231,   849,   920,   543,   933,   920,   804,
    1016,   892,  1018,   847,   826,   232,   233,   234,   637,   626,
     547,   548,   805,   549,   550,   551,   552,   553,   554,   555,
     556,   557,   558,   559,   560,   561,   658,   210,  1013,  1062,
     643,   658,   795,   658,   699,   877,  1051,   806,  1053,  1054,
    1055,  1056,   658,   690,   658,   694,   658,  1065,  1063,    90,
     696,   821,   372,  1052,     0,     0,   211,     0,   212,   213,
     214,   215,     0,     0,  1042,     0,   926,     0,   562,   381,
       0,     0,     0,     0,     0,     0,   216,   235,   269,   658,
     270,   363,   271,     0,   272,   273,   274,   328,     0,     0,
    1042,  1081,     0,   329,   330,   331,     0,     0,     0,     0,
     807,     0,   807,   807,     0,     0,     0,   807,   787,     0,
    1088,  1089,  1090,   309,   310,   311,   312,   313,   314,     0,
     275,   276,   277,     0,     0,   658,     0,   658,   315,   332,
     333,   334,     0,     0,     0,     0,     0,     0,   318,   319,
     658,     0,     0,     0,     0,   323,   324,   325,   326,     0,
       0,    -3,     1,     0,     0,     0,     0,     2,     3,     4,
       5,     0,  -569,  -569,     6,     0,     7,     8,     0,     0,
    -569,  -569,     0,  -569,     9,    10,    11,     0,  -105,    12,
       0,   833,    13,   658,     0,     0,    14,     0,     0,     0,
       0,   999,     0,     0,     0,     0,     0,     0,     0,    15,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   400,
     401,     0,     0,   807,   269,    16,   270,   458,   271,     0,
     272,   273,   274,     0,     0,    17,    18,    19,    20,    21,
      22,    23,  -105,     0,     0,     0,     0,    24,    25,    26,
      27,    28,    29,    30,    31,   -94,     0,   269,     0,   270,
     471,   271,   893,   272,   273,   274,   275,   276,   277,     0,
       0,   -95,     0,     0,     0,     0,     0,     0,   925,    32,
     658,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,  -105,
    -105,  -105,  -105,  -105,  -105,  -105,  -105,     0,     0,   275,
     276,   277,     0,     0,     0,     0,   658,     0,     0,   -94,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    33,
       0,     0,     0,     0,     0,   -95,     0,   480,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   269,  -105,
     270,   483,   271,     0,   272,   273,   274,     0,   -94,   -94,
     -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,   -94,
     -94,   -94,   -94,   -94,   -95,   -95,   -95,   -95,   -95,   -95,
     -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,   -95,
     275,   276,   277,     0,   269,     0,   270,   521,   271,   518,
     272,   273,   274,   269,     0,   270,   536,   271,     0,   272,
     273,   274,   113,     0,     0,     0,   -94,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   -95,     0,     0,     0,   275,   276,   277,     0,
       0,     0,     0,     0,     0,   275,   276,   277,  1058,  1059,
    1060,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   788,   789,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   113,   176,  1012,   177,     0,
       0,     0,     0,     0,     0,     0,     0,   945,   946,   947,
     948,   949,   950,   951,   952,   953,   954,   955,   956,   957,
     958,   959,   960,   961,   962,   963,   964,   965,   966,     0,
       0,     0,     0,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   788,   796,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   113,   176,
       0,   177,   945,   946,   947,   948,   949,   950,   951,   952,
     953,   954,   955,   956,   957,   958,   959,   960,   961,   962,
     963,   964,   965,   966,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   788,   829,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   113,   176,     0,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   788,   907,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   113,   176,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   788,   921,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   113,   176,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     788,   923,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     113,   176,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   788,   944,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   113,   176,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   788,   983,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   113,   176,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   788,
    1000,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   113,
     176,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   788,  1015,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   113,   176,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   788,  1045,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   113,   176,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   788,  1046,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   113,   176,
       0,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   788,  1071,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   113,   176,     0,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   788,  1086,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   113,   176,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   788,  1092,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   113,   176,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     788,  1093,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     113,   176,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   788,  1094,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   113,   176,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   374,     0,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   113,   176,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   374,
       0,   606,   139,   140,   141,   607,   143,   144,   608,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   113,
     176,     0,   609,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   623,     0,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   113,   176,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   659,     0,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   113,   176,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   592,   593,   594,   595,   670,   596,   597,     0,
     598,   599,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   113,   176,
       0,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   592,   593,   594,   595,   739,
     596,   740,     0,   598,   599,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   113,   176,     0,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   592,   593,
     594,   595,   742,   596,   740,     0,   598,   599,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   113,   176,     0,   177,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   743,   744,   745,   746,   747,   748,   749,     0,   750,
     751,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   113,   176,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   743,   744,   745,   746,   747,   756,
     749,     0,   750,   751,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     113,   176,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   757,   758,   759,
     760,   761,   762,   763,     0,   764,   765,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   113,   176,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     757,   770,   759,   760,   761,   762,   763,     0,   764,   765,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   113,   176,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   831,
       0,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   113,
     176,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   857,     0,   858,   139,   140,   141,   799,   143,
     144,   800,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   113,   176,     0,   859,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   908,
     909,   910,   911,   912,   913,   914,     0,   915,   916,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   113,   176,     0,   177,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   908,   909,   910,   922,   912,   913,   914,     0,
     915,   916,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   113,   176,
       0,   177,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   908,   909,   910,   924,   912,
     913,   914,     0,   915,   916,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   113,   176,     0,   177,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   834,   835,
     836,   837,   930,   838,   839,     0,   840,   841,   140,   141,
     842,   143,   144,   931,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   113,   176,     0,   843,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,     0,     0,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   113,   176,     0,
     177,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   592,   593,   594,   595,     0,   596,
     597,     0,   598,   599,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     113,   176,     0,   177,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   592,   593,   594,
     595,   687,   596,     0,     0,   598,   599,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   113,   176,     0,   177,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     592,   593,   594,   595,     0,   596,   740,     0,   598,   599,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   113,   176,     0,   177,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   592,   593,   594,   595,   878,   596,     0,
       0,   598,   599,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   113,
     176,     0,   177,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   592,   593,   594,   595,
       0,   596,     0,     0,   598,   599,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   113,   176,     0,   177,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   834,
     835,   836,   837,     0,   838,   839,     0,   840,   841,   140,
     141,   842,   143,   144,   113,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     0,   176,     0,   843,     0,
       0,     0,     0,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   990,     0,   991,
     139,     0,   992,   113,   143,   144,   993,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,     0,   176,     0,
     994,     0,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   644,     0,
       0,   645,     0,   646,     0,   647,     0,     0,   720,   724,
       0,   113,     0,   648,   649,   650,     0,   651,   652,     0,
       0,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,     0,   653,     0,   721,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   644,     0,     0,   645,
       0,   646,     0,   647,     0,     0,   720,     0,     0,   113,
       0,   648,   649,   650,     0,   651,   652,     0,     0,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,     0,   653,     0,   721,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   644,     0,     0,   645,     0,   646,
       0,   647,     0,     0,     0,     0,   730,   113,     0,   648,
     649,   650,     0,   651,   652,     0,     0,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   653,     0,   731,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   644,     0,     0,   645,     0,   646,     0,   647,
       0,     0,     0,     0,     0,   113,     0,   648,   649,   650,
       0,   651,   652,     0,     0,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
       0,   653,     0,   927,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,  1038,     0,
    1039,   139,     0,   113,   142,   143,   144,   145,   146,   147,
     148,   149,   150,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   269,     0,   270,   542,   271,     0,
     272,   273,   274,     0,     0,     0,     0,     0,     0,   176,
       0,   177,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   275,   276,   277,   139,
     140,   141,   799,   143,   144,   800,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   113,   176,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   269,     0,   270,
     587,   271,     0,   272,   273,   274,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   644,     0,   791,   645,   792,   646,     0,   647,   275,
     276,   277,   113,   730,     0,     0,   648,   649,   650,     0,
     651,   652,     0,     0,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,     0,
     653,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   644,     0,     0,
     645,     0,   646,     0,   647,     0,     0,     0,   852,   113,
       0,     0,   648,   649,   650,     0,   651,   652,     0,     0,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,     0,   653,     0,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   644,     0,     0,   645,     0,   646,
       0,   647,     0,     0,     0,   113,   730,     0,     0,   648,
     649,   650,     0,   651,   652,     0,     0,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,     0,   653,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     644,     0,     0,   645,     0,   646,     0,   647,     0,     0,
       0,     0,     0,     0,     0,   648,   649,   650,     0,   651,
     652,     0,     0,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,    63,   653,
      64,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    65,     0,   269,     0,   270,
     667,   271,     0,   272,   273,   274,   269,     0,   270,   771,
     271,     0,   272,   273,   274,     0,     0,    48,     0,     0,
       0,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,     0,    49,     0,     0,    50,   275,
     276,   277,     0,     0,     0,    51,     0,     0,   275,   276,
     277,     0,    52,    79,    53,     0,     0,     0,    54,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,    56,    57,    58,    59,     0,
       0,     0,    60,     0,     0,     0,     0,     0,     0,     0,
       0,    61,   130,   131,   132,   133,   134,   135,   136,   137,
     895,     0,   896,   139,   140,   141,   897,   143,   144,   898,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,     0,   171,   172,   173,   174,   175,
     186,   176,   187,   899,   188,     0,   189,   190,   191,   198,
       0,   199,     0,   200,     0,   201,   202,   203,   269,     0,
     270,     0,   271,     0,   272,   273,   274,   198,     0,   199,
       0,   200,     0,   369,   202,   203,     0,     0,     0,     0,
       0,     0,   192,   193,   194,     0,     0,     0,     0,     0,
       0,   204,   205,   206,     0,     0,     0,     0,     0,     0,
     275,   276,   277,     0,     0,     0,     0,     0,     0,   370,
     205,   206,   386,     0,   387,     0,   388,     0,   389,   390,
     391,   416,     0,   417,     0,   418,     0,   419,   420,   421,
     416,     0,   417,     0,   426,     0,   419,   420,   421,   416,
       0,   417,     0,   428,     0,   419,   420,   421,     0,     0,
       0,     0,     0,     0,   392,   393,   394,     0,     0,     0,
       0,     0,     0,   422,   423,   424,     0,     0,     0,     0,
       0,     0,   422,   423,   424,     0,     0,     0,     0,     0,
       0,   422,   423,   424,   416,     0,   417,     0,   429,     0,
     419,   420,   421,   416,     0,   417,     0,   430,     0,   419,
     420,   421,   416,     0,   417,     0,   431,     0,   419,   420,
     421,   416,     0,   417,     0,   432,     0,   419,   420,   421,
       0,     0,     0,     0,     0,     0,   422,   423,   424,     0,
       0,     0,     0,     0,     0,   422,   423,   424,     0,     0,
       0,     0,     0,     0,   422,   423,   424,     0,     0,     0,
       0,     0,     0,   422,   423,   424,   416,     0,   417,     0,
     433,     0,   419,   420,   421,   416,     0,   417,     0,   434,
       0,   419,   420,   421,   416,     0,   417,     0,   435,     0,
     419,   420,   421,   416,     0,   417,     0,   436,     0,   419,
     420,   421,     0,     0,     0,     0,     0,     0,   422,   423,
     424,     0,     0,     0,     0,     0,     0,   422,   423,   424,
       0,     0,     0,     0,     0,     0,   422,   423,   424,     0,
       0,     0,     0,     0,     0,   422,   423,   424,   416,     0,
     417,     0,   437,     0,   419,   420,   421,   416,     0,   417,
       0,   438,     0,   419,   420,   421,   328,     0,   352,     0,
     440,     0,   329,   330,   331,   328,     0,   352,     0,   442,
       0,   329,   330,   331,     0,     0,     0,     0,     0,     0,
     422,   423,   424,     0,     0,     0,     0,     0,     0,   422,
     423,   424,     0,     0,     0,     0,     0,     0,   332,   333,
     334,     0,     0,     0,     0,     0,     0,   332,   333,   334,
     328,     0,   352,     0,   445,     0,   329,   330,   331,   328,
       0,   352,     0,   446,     0,   329,   330,   331,   459,     0,
     460,     0,   461,     0,   462,   463,   464,   416,     0,   417,
       0,   470,     0,   419,   420,   421,     0,     0,     0,     0,
       0,     0,   332,   333,   334,     0,     0,     0,     0,     0,
       0,   332,   333,   334,     0,     0,     0,     0,     0,     0,
     465,   466,   467,     0,     0,     0,     0,     0,     0,   422,
     423,   424,   416,     0,   417,     0,   476,     0,   419,   420,
     421,   416,     0,   417,     0,   484,     0,   419,   420,   421,
     416,     0,   417,     0,   485,     0,   419,   420,   421,   386,
       0,   387,     0,   491,     0,   389,   390,   391,     0,     0,
       0,     0,     0,     0,   422,   423,   424,     0,     0,     0,
       0,     0,     0,   422,   423,   424,     0,     0,     0,     0,
       0,     0,   422,   423,   424,     0,     0,     0,     0,     0,
       0,   392,   393,   492,   506,     0,   507,     0,   508,     0,
     509,   510,   511,   416,     0,   417,     0,   529,     0,   419,
     420,   421,   416,     0,   417,     0,   540,     0,   419,   420,
     421,   564,     0,   565,     0,   566,     0,   567,   568,   569,
       0,     0,     0,     0,     0,     0,   512,   513,   514,     0,
       0,     0,     0,     0,     0,   422,   423,   424,     0,     0,
       0,     0,     0,     0,   422,   423,   424,     0,     0,     0,
       0,     0,     0,   570,   571,   572,   416,     0,   417,     0,
     642,     0,   419,   420,   421,   416,     0,   417,     0,   668,
       0,   419,   420,   421,   386,     0,   387,     0,   700,     0,
     389,   390,   391,   416,     0,   417,     0,   715,     0,   419,
     420,   421,     0,     0,     0,     0,     0,     0,   422,   423,
     424,     0,     0,     0,     0,     0,     0,   422,   423,   424,
       0,     0,     0,     0,     0,     0,   392,   393,   701,     0,
       0,     0,     0,     0,     0,   422,   423,   424,   416,     0,
     417,     0,   774,     0,   419,   420,   421,   506,     0,   507,
       0,   818,     0,   509,   819,   511,   416,     0,   417,     0,
     875,     0,   419,   420,   421,   416,     0,   417,     0,   978,
       0,   419,   420,   421,     0,     0,     0,     0,     0,     0,
     422,   423,   424,     0,     0,     0,     0,     0,     0,   512,
     513,   514,     0,     0,     0,     0,     0,     0,   422,   423,
     424,     0,     0,     0,     0,     0,     0,   422,   423,   424
};

static const short yycheck[] =
{
      44,    46,   250,   548,   709,   450,   254,   687,    86,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,    97,
     548,   548,   784,   578,   812,   397,   304,   815,   548,   548,
     782,   546,   106,   548,   613,   614,   615,   552,   553,   548,
     600,     1,   374,   681,     1,   299,    59,    55,   302,   632,
     248,    96,   690,   301,    97,   120,    99,   305,   548,    12,
     624,   105,   626,   578,   129,   130,   626,   622,    81,   119,
      65,   814,   119,   637,   817,   630,    29,    30,    31,    85,
      33,    34,   129,   126,   119,   600,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   685,   357,    57,    58,    59,   622,   306,   159,
     358,   626,   627,   793,   362,   630,   120,   119,     0,   119,
     126,   119,   454,   495,   159,   680,   681,   120,   673,   120,
     119,   119,   145,    68,  1014,   690,    96,    97,   553,    99,
      97,   101,    99,   120,   152,   673,   673,   129,   130,   131,
     830,   706,   120,   673,   673,   734,   546,   159,   673,   159,
    1040,   159,   552,   553,   673,   680,   681,   499,    25,   126,
     159,   159,   687,   451,    73,   690,   691,   692,   693,   694,
     695,   696,   934,   673,    36,   439,   974,   120,   976,   977,
     120,   706,   524,   981,   120,   640,   145,   124,   878,   129,
     130,   131,   717,   124,   452,   132,    31,    32,   736,   736,
     600,   132,   627,   268,   729,   119,   548,     3,   472,   293,
     294,   736,   777,   861,   279,   280,   474,   481,   129,   130,
     131,     3,   546,   124,   548,     4,   626,   627,   552,   553,
     795,   132,   797,    68,    69,     3,   736,   292,    31,    32,
     504,   155,   156,   157,   298,   129,   130,   131,   786,  1021,
     140,   141,   777,     3,   779,    70,    71,   782,   148,   784,
     683,   786,    26,    27,   154,   980,   691,   692,   793,   527,
     795,   809,   797,   537,   812,   111,   600,   815,   129,   130,
     131,   539,    44,    45,   809,   708,    85,   812,   120,  1087,
     815,   691,   692,   693,   694,   695,   696,   129,   130,   131,
     889,   826,   626,   627,   729,   830,   861,   123,   120,   809,
      85,   876,   812,   123,   124,   815,   120,   855,   855,   120,
     374,    40,    41,   861,   140,   141,   891,   129,   586,   729,
     855,   673,   148,   120,    11,    12,   120,   862,   154,   877,
     929,   866,    19,    20,   869,    22,   120,   872,   120,   673,
     120,   876,   877,   878,   779,   855,   353,  1047,   131,   356,
     120,   861,   359,   360,   120,   120,   891,   691,   692,   693,
     694,   695,   696,   120,   638,   120,   120,   578,   120,   779,
     129,   130,   131,   641,   129,   130,   131,   810,   120,   116,
     813,   118,   120,   816,   448,   122,   123,   124,   120,   120,
     454,   826,   120,   120,   942,   729,   129,   130,   131,   934,
     120,   120,   736,   120,   130,   503,   131,   942,   155,   156,
     157,   622,   120,   110,   127,   848,   826,   118,   120,   630,
     120,   158,   159,   160,   128,   120,   974,   120,   976,   977,
     124,   129,   942,   981,   120,   499,   120,   116,   502,   974,
     131,   976,   977,   116,   127,   779,   981,   127,   119,   882,
     125,   127,   786,   886,   120,   120,   866,     1,   522,   869,
     524,   894,   872,   124,   974,   116,   976,   977,   127,   680,
     681,   981,   122,   538,   122,   809,   687,   159,   812,   690,
     118,   815,   118,   118,   548,   159,  1021,   551,   135,   135,
     554,   118,   826,   118,   118,   706,    54,    55,   562,    36,
      37,    38,    39,    61,  1014,   773,   717,   159,   118,   118,
     118,    55,  1047,   118,   159,   579,   580,     5,     5,   119,
     584,   855,   119,   118,   118,   118,   127,   861,   673,   664,
    1040,   940,   866,    70,   793,   869,   522,   878,   872,  1087,
     973,   855,   975,   877,   779,    82,    83,    84,   562,   552,
      94,    95,  1087,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   777,    13,   967,  1035,
     576,   782,   727,   784,   639,   825,  1025,  1087,  1027,  1028,
    1029,  1030,   793,   630,   795,   634,   797,  1040,  1037,    15,
     636,   772,   295,  1026,    -1,    -1,    42,    -1,    44,    45,
      46,    47,    -1,    -1,  1014,    -1,   874,    -1,   152,   673,
      -1,    -1,    -1,    -1,    -1,    -1,    62,   154,   116,   830,
     118,   119,   120,    -1,   122,   123,   124,   116,    -1,    -1,
    1040,  1064,    -1,   122,   123,   124,    -1,    -1,    -1,    -1,
     974,    -1,   976,   977,    -1,    -1,    -1,   981,   712,    -1,
    1083,  1084,  1085,   211,   212,   213,   214,   215,   216,    -1,
     158,   159,   160,    -1,    -1,   876,    -1,   878,   226,   158,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,   236,   237,
     891,    -1,    -1,    -1,    -1,   243,   244,   245,   246,    -1,
      -1,     0,     1,    -1,    -1,    -1,    -1,     6,     7,     8,
       9,    -1,    11,    12,    13,    -1,    15,    16,    -1,    -1,
      19,    20,    -1,    22,    23,    24,    25,    -1,     1,    28,
      -1,   785,    31,   934,    -1,    -1,    35,    -1,    -1,    -1,
      -1,   942,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    48,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   307,
     308,    -1,    -1,  1087,   116,    64,   118,   119,   120,    -1,
     122,   123,   124,    -1,    -1,    74,    75,    76,    77,    78,
      79,    80,    55,    -1,    -1,    -1,    -1,    86,    87,    88,
      89,    90,    91,    92,    93,     1,    -1,   116,    -1,   118,
     119,   120,   856,   122,   123,   124,   158,   159,   160,    -1,
      -1,     1,    -1,    -1,    -1,    -1,    -1,    -1,   873,   118,
    1021,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,    -1,  1047,    -1,    -1,    55,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   158,
      -1,    -1,    -1,    -1,    -1,    55,    -1,   415,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,   152,
     118,   119,   120,    -1,   122,   123,   124,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     158,   159,   160,    -1,   116,    -1,   118,   119,   120,   477,
     122,   123,   124,   116,    -1,   118,   119,   120,    -1,   122,
     123,   124,    55,    -1,    -1,    -1,   152,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   152,    -1,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,  1032,  1033,
    1034,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    55,   159,   123,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    55,   159,
      -1,   161,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    55,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    55,   159,    -1,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    55,   159,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      55,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    55,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    55,   159,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    55,
     159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,   119,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    55,   159,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    55,   159,    -1,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    55,   159,
      -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    55,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,   119,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    55,   159,    -1,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,   119,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    55,   159,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      55,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    55,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    55,   159,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    55,
     159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    55,   159,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    55,   159,    -1,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    55,   159,
      -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    55,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    55,   159,    -1,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    55,   159,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      55,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    55,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    55,   159,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    55,
     159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
     116,   117,   118,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    55,   159,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    55,   159,    -1,   161,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,    55,   159,
      -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,    -1,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,    55,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,   115,   116,   117,   118,    -1,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    55,   159,    -1,   161,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,    -1,    -1,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    55,   159,    -1,
     161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,    -1,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      55,   159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,    -1,    -1,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,    55,   159,    -1,   161,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,    -1,   117,   118,    -1,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    55,   159,    -1,   161,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,    -1,
      -1,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    55,
     159,    -1,   161,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,   115,
      -1,   117,    -1,    -1,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    55,   159,    -1,   161,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,    -1,   117,   118,    -1,   120,   121,   122,
     123,   124,   125,   126,    55,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,   159,    -1,   161,    -1,
      -1,    -1,    -1,    94,    95,    96,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,   115,   116,   117,   118,    -1,   120,
     121,    -1,   123,    55,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,    -1,   159,    -1,
     161,    -1,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,    -1,
      -1,   113,    -1,   115,    -1,   117,    -1,    -1,   120,   121,
      -1,    55,    -1,   125,   126,   127,    -1,   129,   130,    -1,
      -1,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    -1,   159,    -1,   161,
      94,    95,    96,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,    -1,    -1,   113,
      -1,   115,    -1,   117,    -1,    -1,   120,    -1,    -1,    55,
      -1,   125,   126,   127,    -1,   129,   130,    -1,    -1,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,    -1,   159,    -1,   161,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,   113,    -1,   115,
      -1,   117,    -1,    -1,    -1,    -1,   122,    55,    -1,   125,
     126,   127,    -1,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    -1,   159,    -1,   161,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,    -1,    -1,   113,    -1,   115,    -1,   117,
      -1,    -1,    -1,    -1,    -1,    55,    -1,   125,   126,   127,
      -1,   129,   130,    -1,    -1,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
      -1,   159,    -1,   161,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,    -1,
     120,   121,    -1,    55,   124,   125,   126,   127,   128,   129,
     130,   131,   132,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   116,    -1,   118,   119,   120,    -1,
     122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,   159,
      -1,   161,    94,    95,    96,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   158,   159,   160,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,    55,   159,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    94,    95,    96,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,    -1,   112,   113,   114,   115,    -1,   117,   158,
     159,   160,    55,   122,    -1,    -1,   125,   126,   127,    -1,
     129,   130,    -1,    -1,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,    -1,
     159,    94,    95,    96,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    -1,    -1,
     113,    -1,   115,    -1,   117,    -1,    -1,    -1,   121,    55,
      -1,    -1,   125,   126,   127,    -1,   129,   130,    -1,    -1,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,    -1,   159,    -1,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,    -1,    -1,   113,    -1,   115,
      -1,   117,    -1,    -1,    -1,    55,   122,    -1,    -1,   125,
     126,   127,    -1,   129,   130,    -1,    -1,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,    -1,   159,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,    -1,    -1,   113,    -1,   115,    -1,   117,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   125,   126,   127,    -1,   129,
     130,    -1,    -1,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,     6,   159,
       8,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    23,    -1,   116,    -1,   118,
     119,   120,    -1,   122,   123,   124,   116,    -1,   118,   119,
     120,    -1,   122,   123,   124,    -1,    -1,     7,    -1,    -1,
      -1,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    -1,    25,    -1,    -1,    28,   158,
     159,   160,    -1,    -1,    -1,    35,    -1,    -1,   158,   159,
     160,    -1,    42,    81,    44,    -1,    -1,    -1,    48,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    63,    -1,    65,    66,    67,    68,    -1,
      -1,    -1,    72,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    81,   110,   111,   112,   113,   114,   115,   116,   117,
     118,    -1,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,    -1,   153,   154,   155,   156,   157,
     116,   159,   118,   161,   120,    -1,   122,   123,   124,   116,
      -1,   118,    -1,   120,    -1,   122,   123,   124,   116,    -1,
     118,    -1,   120,    -1,   122,   123,   124,   116,    -1,   118,
      -1,   120,    -1,   122,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,   160,   116,    -1,   118,    -1,   120,    -1,   122,   123,
     124,   116,    -1,   118,    -1,   120,    -1,   122,   123,   124,
     116,    -1,   118,    -1,   120,    -1,   122,   123,   124,   116,
      -1,   118,    -1,   120,    -1,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   116,    -1,   118,    -1,   120,    -1,
     122,   123,   124,   116,    -1,   118,    -1,   120,    -1,   122,
     123,   124,   116,    -1,   118,    -1,   120,    -1,   122,   123,
     124,   116,    -1,   118,    -1,   120,    -1,   122,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,   116,    -1,   118,    -1,
     120,    -1,   122,   123,   124,   116,    -1,   118,    -1,   120,
      -1,   122,   123,   124,   116,    -1,   118,    -1,   120,    -1,
     122,   123,   124,   116,    -1,   118,    -1,   120,    -1,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,   116,    -1,
     118,    -1,   120,    -1,   122,   123,   124,   116,    -1,   118,
      -1,   120,    -1,   122,   123,   124,   116,    -1,   118,    -1,
     120,    -1,   122,   123,   124,   116,    -1,   118,    -1,   120,
      -1,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
     116,    -1,   118,    -1,   120,    -1,   122,   123,   124,   116,
      -1,   118,    -1,   120,    -1,   122,   123,   124,   116,    -1,
     118,    -1,   120,    -1,   122,   123,   124,   116,    -1,   118,
      -1,   120,    -1,   122,   123,   124,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,   160,   116,    -1,   118,    -1,   120,    -1,   122,   123,
     124,   116,    -1,   118,    -1,   120,    -1,   122,   123,   124,
     116,    -1,   118,    -1,   120,    -1,   122,   123,   124,   116,
      -1,   118,    -1,   120,    -1,   122,   123,   124,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,    -1,
      -1,    -1,   158,   159,   160,    -1,    -1,    -1,    -1,    -1,
      -1,   158,   159,   160,   116,    -1,   118,    -1,   120,    -1,
     122,   123,   124,   116,    -1,   118,    -1,   120,    -1,   122,
     123,   124,   116,    -1,   118,    -1,   120,    -1,   122,   123,
     124,   116,    -1,   118,    -1,   120,    -1,   122,   123,   124,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,
      -1,    -1,    -1,    -1,   158,   159,   160,    -1,    -1,    -1,
      -1,    -1,    -1,   158,   159,   160,   116,    -1,   118,    -1,
     120,    -1,   122,   123,   124,   116,    -1,   118,    -1,   120,
      -1,   122,   123,   124,   116,    -1,   118,    -1,   120,    -1,
     122,   123,   124,   116,    -1,   118,    -1,   120,    -1,   122,
     123,   124,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,
      -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160,    -1,
      -1,    -1,    -1,    -1,    -1,   158,   159,   160,   116,    -1,
     118,    -1,   120,    -1,   122,   123,   124,   116,    -1,   118,
      -1,   120,    -1,   122,   123,   124,   116,    -1,   118,    -1,
     120,    -1,   122,   123,   124,   116,    -1,   118,    -1,   120,
      -1,   122,   123,   124,    -1,    -1,    -1,    -1,    -1,    -1,
     158,   159,   160,    -1,    -1,    -1,    -1,    -1,    -1,   158,
     159,   160,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,
     160,    -1,    -1,    -1,    -1,    -1,    -1,   158,   159,   160
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    15,    16,    23,
      24,    25,    28,    31,    35,    48,    64,    74,    75,    76,
      77,    78,    79,    80,    86,    87,    88,    89,    90,    91,
      92,    93,   118,   158,   163,   164,   165,   199,   211,   221,
     326,   369,   370,   362,   200,   331,   339,   337,     7,    25,
      28,    35,    42,    44,    48,    63,    65,    66,    67,    68,
      72,    81,   372,     6,     8,    23,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    81,
      31,    32,    68,    69,   366,   367,   327,    70,    71,   368,
     366,    65,   120,   120,   120,   120,   332,   329,   168,   169,
     170,   167,   340,   166,     0,   212,   338,    11,    12,    19,
      20,    22,   120,    55,    94,    95,    96,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   159,   161,   308,   319,
     320,   321,   322,   323,   324,   325,   116,   118,   120,   122,
     123,   124,   158,   159,   160,   373,   415,   373,   116,   118,
     120,   122,   123,   124,   158,   159,   160,   379,   417,    68,
      13,    42,    44,    45,    46,    47,    62,    12,    29,    30,
      31,    33,    34,    57,    58,    59,   364,    36,    37,    38,
      39,    70,    82,    83,    84,   154,   363,    25,    26,    27,
     365,   365,   365,    59,    81,   145,    73,   365,   358,   345,
     348,    36,    85,   126,   346,   120,   120,   347,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   341,   342,   116,
     118,   120,   122,   123,   124,   158,   159,   160,   409,   344,
     343,   145,   373,   409,     3,     3,     4,     3,   406,     3,
     308,   379,   333,   334,   335,   336,    85,   371,   111,   416,
     374,   375,   418,   380,   381,   382,   360,    44,    45,   365,
     365,   365,   365,   365,   365,   365,    40,    41,   365,   365,
     120,   120,   120,   365,   365,   365,   365,   120,   116,   122,
     123,   124,   158,   159,   160,   428,   407,   407,   120,   120,
      85,   407,   407,   407,   407,   407,   407,   407,   407,   407,
     407,   407,   118,   425,   426,   428,   425,   410,   328,   425,
     425,   120,   330,   119,   409,   213,   373,   379,   379,   122,
     158,   389,   417,   120,   118,   172,   173,   174,   175,   176,
     178,   308,   406,   408,   407,   406,   116,   118,   120,   122,
     123,   124,   158,   159,   160,   383,   419,   408,   407,   428,
     365,   365,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   359,   116,   118,   120,   122,
     123,   124,   158,   159,   160,   411,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   427,
     120,   426,   120,   406,   407,   120,   120,   407,   131,   215,
     390,   391,   392,   172,   129,   201,   130,   131,   119,   116,
     118,   120,   122,   123,   124,   158,   159,   160,   376,   413,
     120,   119,   420,   384,   385,   376,   120,   361,   120,   120,
     365,   412,   406,   119,   120,   120,   216,   217,   308,   110,
     277,   120,   160,   393,   419,   408,   407,   119,   172,   127,
     118,   180,   177,   377,   414,   406,   116,   118,   120,   122,
     123,   124,   158,   159,   160,   386,   421,   407,   365,   120,
     406,   119,   124,   132,   128,   222,   394,   395,   376,   120,
     172,   181,   179,   308,   409,   406,   119,   422,   387,   388,
     120,   120,   119,   217,   172,     1,    55,    94,    95,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   152,   223,   116,   118,   120,   122,   123,   124,
     158,   159,   160,   396,   423,   407,   202,     1,    97,    99,
     126,   183,   184,   185,   124,   132,   378,   119,   406,   373,
     407,   120,   112,   113,   114,   115,   117,   118,   120,   121,
     250,   251,   314,   320,   321,   324,   120,   124,   127,   161,
     171,   172,   175,   176,   178,   309,   318,   319,   321,   323,
     324,   224,   225,   118,   247,   308,   250,   246,   314,   308,
     242,   226,   228,   231,   233,   236,   238,   247,   424,   397,
     398,   399,   120,   277,   110,   113,   115,   117,   125,   126,
     127,   129,   130,   159,   300,   301,   317,   321,   325,   118,
     186,   308,   308,   182,   124,   308,   407,   119,   120,   252,
     116,   251,   253,   129,   120,   278,   278,   278,   278,   120,
     279,   280,   300,   249,   253,   248,   253,   116,   314,   241,
     280,   284,   284,   290,   290,   292,   292,   253,   406,   373,
     120,   160,   400,   419,   407,   203,   127,   131,   188,   116,
     189,   187,   127,   119,   185,   120,   282,   256,   171,   172,
     120,   161,   214,   300,   121,   214,   300,   281,   282,   284,
     122,   161,   218,   219,   257,   300,   125,   243,   214,   116,
     118,   314,   116,   112,   113,   114,   115,   116,   117,   118,
     120,   121,   312,   320,   321,   324,   117,   112,   113,   114,
     115,   116,   117,   118,   120,   121,   311,   320,   321,   324,
     113,   119,   401,   402,   120,     1,    96,    97,    99,   101,
     204,   300,   302,   282,   256,   127,   286,   308,   118,   119,
     308,   112,   114,   255,   257,   279,   119,   258,   278,   124,
     127,   244,   245,   309,   319,   321,   323,   324,   120,   227,
     285,   229,   232,   291,   234,   237,   293,   239,   120,   123,
     403,   421,   407,   120,   300,   205,   246,   301,   303,   119,
     255,   118,   190,   308,   112,   113,   114,   115,   117,   118,
     120,   121,   124,   161,   315,   319,   321,   324,   283,   218,
     254,   257,   121,   259,   300,   124,   116,   118,   120,   161,
     220,   296,   297,   309,   318,   282,   294,   220,   282,   294,
     220,   282,   294,   404,   405,   120,   127,   286,   116,   124,
     132,   257,   192,   122,   193,   191,   287,   282,   122,   260,
     123,   124,   245,   308,   298,   118,   120,   124,   127,   161,
     214,   310,   318,   319,   323,   324,   321,   119,   112,   113,
     114,   115,   116,   117,   118,   120,   121,   313,   320,   321,
     324,   119,   115,   119,   115,   373,   407,   161,   206,   300,
     116,   127,   207,   219,   304,   282,   140,   141,   148,   154,
     194,   195,   288,   282,   119,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   261,   262,   263,
     278,   300,   282,   299,   230,   295,   235,   240,   120,   278,
     208,   210,   301,   119,   159,   118,   118,   118,   123,   195,
     118,   120,   123,   127,   161,   316,   319,   321,   323,   325,
     119,   135,   135,   159,   118,   159,   118,   118,   118,   118,
     118,   118,   123,   263,   264,   119,   282,   220,   282,   220,
     220,   256,   220,   196,   198,   197,   289,   265,   267,   268,
     269,   270,   155,   156,   157,   272,   273,   266,   118,   120,
     305,   306,   320,   322,   323,   119,   119,   255,   159,   271,
       5,   271,   282,   271,   271,   271,   271,     5,   308,   308,
     308,   119,   273,   271,   307,   306,   257,   119,   159,   119,
     119,   119,   119,   119,   119,   119,   119,   118,   118,   118,
     119,   282,   127,   274,   275,   276,   119,   209,   282,   282,
     282,   220,   119,   119,   119
};

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# if defined (__STDC__) || defined (__cplusplus)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# endif
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrlab1


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
    { 								\
      yyerror ("syntax error: cannot back up");\
      YYERROR;							\
    }								\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

/* YYLLOC_DEFAULT -- Compute the default location (before the actions
   are run).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)         \
  Current.first_line   = Rhs[1].first_line;      \
  Current.first_column = Rhs[1].first_column;    \
  Current.last_line    = Rhs[N].last_line;       \
  Current.last_column  = Rhs[N].last_column;
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

# define YYDSYMPRINT(Args)			\
do {						\
  if (yydebug)					\
    yysymprint Args;				\
} while (0)

# define YYDSYMPRINTF(Title, Token, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr, 					\
                  Token, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (cinluded).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short *bottom, short *top)
#else
static void
yy_stack_print (bottom, top)
    short *bottom;
    short *top;
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
  unsigned int yylineno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %u), ",
             yyrule - 1, yylineno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname [yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname [yyr1[yyrule]]);
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
# define YYDSYMPRINT(Args)
# define YYDSYMPRINTF(Title, Token, Value, Location)
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
   SIZE_MAX < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#if YYMAXDEPTH == 0
# undef YYMAXDEPTH
#endif

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
  register const char *yys = yystr;

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
  register char *yyd = yydest;
  register const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

#endif /* !YYERROR_VERBOSE */



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
    {
      YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
# ifdef YYPRINT
      YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
    }
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

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
yydestruct (int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yytype, yyvaluep)
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

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
  /* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  register int yystate;
  register int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short	yyssa[YYINITDEPTH];
  short *yyss = yyssa;
  register short *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  register YYSTYPE *yyvsp;



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
	short *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow ("parser stack overflow",
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyoverflowlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyoverflowlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyoverflowlab;
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
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

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
      YYDSYMPRINTF ("Next token is", yytoken, &yylval, &yylloc);
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

  /* Shift the lookahead token.  */
  YYDPRINTF ((stderr, "Shifting token %s, ", yytname[yytoken]));

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
#line 215 "surface.yy"
    { YYACCEPT; ;}
    break;

  case 3:
#line 217 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			;}
    break;

  case 8:
#line 231 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 9:
#line 233 "surface.yy"
    {
			  int lineNr = lineNumber;
			  eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile(yyvsp[0].yyString, directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			;}
    break;

  case 10:
#line 241 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 11:
#line 243 "surface.yy"
    {
			  int lineNr = lineNumber;
			  eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile(yyvsp[0].yyString, directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			;}
    break;

  case 12:
#line 252 "surface.yy"
    {
			  cout << directoryManager.getCwd() << '\n';
			;}
    break;

  case 13:
#line 255 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 14:
#line 257 "surface.yy"
    {
			  string directory;
			  directoryManager.realPath(yyvsp[0].yyString, directory);
			  if (!directoryManager.cd(directory))
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": cd failed");
			    }
			;}
    break;

  case 15:
#line 266 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 16:
#line 268 "surface.yy"
    {
			  string directory;
			  directoryManager.realPath(yyvsp[0].yyString, directory);
			  if (directoryManager.pushd(directory) == UNDEFINED)
			    {
			      IssueWarning(LineNumber(lineNumber) <<
					   ": pushd failed");
			    }
			;}
    break;

  case 17:
#line 278 "surface.yy"
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
#line 288 "surface.yy"
    { lexerStringMode(); ;}
    break;

  case 19:
#line 290 "surface.yy"
    {
			  system((string("ls") + yyvsp[0].yyString).c_str());
			;}
    break;

  case 20:
#line 294 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			;}
    break;

  case 21:
#line 299 "surface.yy"
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			;}
    break;

  case 22:
#line 332 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression(yyvsp[-1].yyToken));
                        ;}
    break;

  case 23:
#line 336 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression(yyvsp[-1].yyToken));
                        ;}
    break;

  case 27:
#line 343 "surface.yy"
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			;}
    break;

  case 28:
#line 351 "surface.yy"
    {
			  IssueWarning(LineNumber(yyvsp[0].yyToken.lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar(yyvsp[0].yyToken);
			  moduleExpressions.push(new ModuleExpression(t));
			;}
    break;

  case 30:
#line 362 "surface.yy"
    {
			  ModuleExpression* m1 = moduleExpressions.top();
			  moduleExpressions.pop();
			  ModuleExpression* m2 = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m1, m2));
			;}
    break;

  case 35:
#line 378 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression(yyvsp[0].yyToken));
                        ;}
    break;

  case 36:
#line 385 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			;}
    break;

  case 37:
#line 393 "surface.yy"
    { clear(); ;}
    break;

  case 38:
#line 395 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, bubble));
			;}
    break;

  case 39:
#line 402 "surface.yy"
    {;}
    break;

  case 40:
#line 405 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 41:
#line 406 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 42:
#line 413 "surface.yy"
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			;}
    break;

  case 43:
#line 418 "surface.yy"
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			;}
    break;

  case 49:
#line 433 "surface.yy"
    {
			  currentRenaming->addSortMapping(yyvsp[-2].yyToken, yyvsp[0].yyToken);
			;}
    break;

  case 50:
#line 437 "surface.yy"
    {
			  currentRenaming->addLabelMapping(yyvsp[-2].yyToken, yyvsp[0].yyToken);
			;}
    break;

  case 51:
#line 440 "surface.yy"
    {;}
    break;

  case 52:
#line 443 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 53:
#line 445 "surface.yy"
    {
			  currentRenaming->addOpMapping(bubble);
			;}
    break;

  case 54:
#line 448 "surface.yy"
    { clear(); ;}
    break;

  case 55:
#line 450 "surface.yy"
    {
			  currentRenaming->addOpMapping(bubble);
			;}
    break;

  case 56:
#line 455 "surface.yy"
    {;}
    break;

  case 58:
#line 459 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 59:
#line 460 "surface.yy"
    { currentRenaming->addOpTarget(bubble); ;}
    break;

  case 60:
#line 461 "surface.yy"
    { clear(); ;}
    break;

  case 61:
#line 462 "surface.yy"
    { currentRenaming->addOpTarget(bubble); ;}
    break;

  case 62:
#line 465 "surface.yy"
    {;}
    break;

  case 66:
#line 473 "surface.yy"
    { currentRenaming->setPrec(yyvsp[0].yyToken); ;}
    break;

  case 67:
#line 474 "surface.yy"
    { clear(); ;}
    break;

  case 68:
#line 475 "surface.yy"
    { currentRenaming->setGather(bubble); ;}
    break;

  case 69:
#line 476 "surface.yy"
    { clear(); ;}
    break;

  case 70:
#line 477 "surface.yy"
    { currentRenaming->setFormat(bubble); ;}
    break;

  case 71:
#line 478 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 72:
#line 479 "surface.yy"
    { currentRenaming->setLatexMacro(yyvsp[-1].yyString); ;}
    break;

  case 73:
#line 485 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 74:
#line 487 "surface.yy"
    {
			  fileTable.beginModule(yyvsp[-4].yyToken, yyvsp[-2].yyToken);
			  interpreter.setCurrentView(new View(yyvsp[-2].yyToken));
			  currentSyntaxContainer = CV;
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			;}
    break;

  case 75:
#line 495 "surface.yy"
    {
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			;}
    break;

  case 76:
#line 500 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView((yyvsp[-9].yyToken).code(), CV);
			  CV->finishView();
			;}
    break;

  case 79:
#line 513 "surface.yy"
    {
			  CV->addSortMapping(yyvsp[-2].yyToken, yyvsp[0].yyToken);
			;}
    break;

  case 80:
#line 516 "surface.yy"
    {;}
    break;

  case 81:
#line 517 "surface.yy"
    { clear(); ;}
    break;

  case 84:
#line 522 "surface.yy"
    { yyval.yyToken = yyvsp[-1].yyToken; ;}
    break;

  case 85:
#line 524 "surface.yy"
    {
			  IssueWarning(LineNumber(yyvsp[0].yyToken.lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar(yyvsp[0].yyToken);
			  yyval.yyToken = t;
			;}
    break;

  case 86:
#line 534 "surface.yy"
    {
			  //
			  //	Specific op->op mapping.
			  //
			  Token::peelParens(bubble);  // remove any enclosing parens from op name
			  CV->addOpMapping(bubble);
			;}
    break;

  case 87:
#line 542 "surface.yy"
    {
			  clear();
			;}
    break;

  case 88:
#line 546 "surface.yy"
    {
			  Token::peelParens(bubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(bubble);
			;}
    break;

  case 89:
#line 551 "surface.yy"
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
#line 561 "surface.yy"
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
#line 585 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 92:
#line 587 "surface.yy"
    {
			  interpreter.setCurrentModule(new PreModule(yyvsp[-2].yyToken, yyvsp[0].yyToken));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule(yyvsp[-2].yyToken, yyvsp[0].yyToken);
			;}
    break;

  case 93:
#line 593 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule(yyvsp[0].yyToken);
			;}
    break;

  case 94:
#line 600 "surface.yy"
    {;}
    break;

  case 95:
#line 602 "surface.yy"
    {
			  IssueWarning(LineNumber(yyvsp[0].yyToken.lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar(yyvsp[0].yyToken);
			  store(t);
			;}
    break;

  case 96:
#line 611 "surface.yy"
    {;}
    break;

  case 100:
#line 620 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter(yyvsp[-2].yyToken, me);
			;}
    break;

  case 101:
#line 628 "surface.yy"
    {
			  IssueWarning(LineNumber(yyvsp[0].yyToken.lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar(yyvsp[0].yyToken);
			  clear();
			  store(t);
			  currentSyntaxContainer->addType(false, bubble);
			;}
    break;

  case 105:
#line 645 "surface.yy"
    {
			  IssueWarning(LineNumber(yyvsp[0].yyToken.lineNumber()) <<
                                       ": missing space before period.");
			  Token t;
			  t.dropChar(yyvsp[0].yyToken);
			  store(t);
			;}
    break;

  case 110:
#line 662 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport(yyvsp[-1].yyToken, me);
			;}
    break;

  case 111:
#line 668 "surface.yy"
    { clear(); ;}
    break;

  case 112:
#line 669 "surface.yy"
    { CM->addSortDecl(bubble); ;}
    break;

  case 113:
#line 671 "surface.yy"
    { clear(); ;}
    break;

  case 114:
#line 672 "surface.yy"
    { CM->addSubsortDecl(bubble); ;}
    break;

  case 115:
#line 674 "surface.yy"
    {;}
    break;

  case 116:
#line 676 "surface.yy"
    {;}
    break;

  case 117:
#line 678 "surface.yy"
    {;}
    break;

  case 118:
#line 680 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 119:
#line 681 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 120:
#line 682 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 121:
#line 684 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 122:
#line 685 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 123:
#line 686 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 124:
#line 687 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 125:
#line 689 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 126:
#line 690 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 127:
#line 691 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 128:
#line 693 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 129:
#line 694 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 130:
#line 695 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 131:
#line 696 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 132:
#line 698 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 133:
#line 699 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 134:
#line 700 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 135:
#line 702 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 136:
#line 703 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 137:
#line 704 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 138:
#line 705 "surface.yy"
    { CM->addStatement(bubble); ;}
    break;

  case 139:
#line 708 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 140:
#line 713 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 141:
#line 718 "surface.yy"
    {
			;}
    break;

  case 142:
#line 721 "surface.yy"
    {
			;}
    break;

  case 143:
#line 724 "surface.yy"
    { clear(); ;}
    break;

  case 144:
#line 725 "surface.yy"
    { CM->addSubsortDecl(bubble); ;}
    break;

  case 145:
#line 728 "surface.yy"
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
#line 738 "surface.yy"
    {;}
    break;

  case 147:
#line 739 "surface.yy"
    {;}
    break;

  case 150:
#line 747 "surface.yy"
    {
			;}
    break;

  case 151:
#line 751 "surface.yy"
    { currentSyntaxContainer->addVarDecl(yyvsp[0].yyToken); ;}
    break;

  case 152:
#line 752 "surface.yy"
    { currentSyntaxContainer->addVarDecl(yyvsp[0].yyToken); ;}
    break;

  case 153:
#line 755 "surface.yy"
    { clear(); store(yyvsp[0].yyToken); ;}
    break;

  case 154:
#line 756 "surface.yy"
    { CM->addOpDecl(bubble); ;}
    break;

  case 155:
#line 757 "surface.yy"
    { clear(); ;}
    break;

  case 156:
#line 758 "surface.yy"
    { CM->addOpDecl(bubble); ;}
    break;

  case 159:
#line 766 "surface.yy"
    {
			  clear();
			  store(yyvsp[0].yyToken);
			  CM->addOpDecl(bubble);
			;}
    break;

  case 160:
#line 771 "surface.yy"
    { clear(); ;}
    break;

  case 161:
#line 772 "surface.yy"
    { CM->addOpDecl(bubble); ;}
    break;

  case 162:
#line 776 "surface.yy"
    {
			  if (yyvsp[-1].yyBool)
			    CM->convertSortsToKinds();
			;}
    break;

  case 165:
#line 786 "surface.yy"
    { yyval.yyBool = false; ;}
    break;

  case 166:
#line 787 "surface.yy"
    { yyval.yyBool = true; ;}
    break;

  case 169:
#line 795 "surface.yy"
    {
			  clear();
			  store(yyvsp[0].yyToken);
			  currentSyntaxContainer->addType(false, bubble);
			;}
    break;

  case 170:
#line 800 "surface.yy"
    { clear(); ;}
    break;

  case 171:
#line 802 "surface.yy"
    {
			  currentSyntaxContainer->addType(true, bubble);
			;}
    break;

  case 172:
#line 807 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 173:
#line 808 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 174:
#line 811 "surface.yy"
    {;}
    break;

  case 178:
#line 820 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			;}
    break;

  case 179:
#line 824 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			;}
    break;

  case 180:
#line 828 "surface.yy"
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			;}
    break;

  case 181:
#line 834 "surface.yy"
    {
			  CM->setFlag(SymbolType::ASSOC);
			;}
    break;

  case 182:
#line 838 "surface.yy"
    {
			  CM->setFlag(SymbolType::COMM);
			;}
    break;

  case 183:
#line 841 "surface.yy"
    { clear(); ;}
    break;

  case 184:
#line 842 "surface.yy"
    { CM->setIdentity(bubble); ;}
    break;

  case 185:
#line 844 "surface.yy"
    {
			  CM->setFlag(SymbolType::IDEM);
			;}
    break;

  case 186:
#line 848 "surface.yy"
    {
			  CM->setFlag(SymbolType::ITER);
			;}
    break;

  case 187:
#line 851 "surface.yy"
    { CM->setPrec(yyvsp[0].yyToken); ;}
    break;

  case 188:
#line 852 "surface.yy"
    { clear(); ;}
    break;

  case 189:
#line 853 "surface.yy"
    { CM->setGather(bubble); ;}
    break;

  case 190:
#line 854 "surface.yy"
    { clear(); ;}
    break;

  case 191:
#line 855 "surface.yy"
    { CM->setFormat(bubble); ;}
    break;

  case 192:
#line 856 "surface.yy"
    { clear(); ;}
    break;

  case 193:
#line 857 "surface.yy"
    { CM->setStrat(bubble); ;}
    break;

  case 194:
#line 858 "surface.yy"
    { clear(); ;}
    break;

  case 195:
#line 859 "surface.yy"
    { CM->setPoly(bubble); ;}
    break;

  case 196:
#line 861 "surface.yy"
    {
			  CM->setFlag(SymbolType::MEMO);
			;}
    break;

  case 197:
#line 865 "surface.yy"
    {
			  CM->setFlag(SymbolType::CTOR);
			;}
    break;

  case 198:
#line 869 "surface.yy"
    {
			  clear();
			  CM->setFrozen(bubble);
			;}
    break;

  case 199:
#line 873 "surface.yy"
    { clear(); ;}
    break;

  case 200:
#line 874 "surface.yy"
    { CM->setFrozen(bubble); ;}
    break;

  case 201:
#line 876 "surface.yy"
    {
			  CM->setFlag(SymbolType::CONFIG);
			;}
    break;

  case 202:
#line 880 "surface.yy"
    {
			  CM->setFlag(SymbolType::OBJECT);
			;}
    break;

  case 203:
#line 884 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 204:
#line 888 "surface.yy"
    {
			  CM->setMetadata(yyvsp[0].yyToken);
			;}
    break;

  case 205:
#line 891 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 206:
#line 892 "surface.yy"
    { CM->setLatexMacro(yyvsp[-1].yyString); ;}
    break;

  case 207:
#line 893 "surface.yy"
    {;}
    break;

  case 208:
#line 895 "surface.yy"
    {
			  CM->setFlag(SymbolType::DITTO);
			;}
    break;

  case 209:
#line 900 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 210:
#line 901 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 213:
#line 909 "surface.yy"
    {
			  clear();
			  CM->addHook(PreModule::ID_HOOK, yyvsp[0].yyToken, bubble);
			;}
    break;

  case 214:
#line 913 "surface.yy"
    { clear(); ;}
    break;

  case 215:
#line 915 "surface.yy"
    {
			  CM->addHook(PreModule::ID_HOOK, yyvsp[-4].yyToken, bubble);
			;}
    break;

  case 216:
#line 918 "surface.yy"
    { clear(); ;}
    break;

  case 217:
#line 920 "surface.yy"
    {
			  CM->addHook(PreModule::OP_HOOK, yyvsp[-4].yyToken, bubble);
			;}
    break;

  case 218:
#line 923 "surface.yy"
    { clear(); ;}
    break;

  case 219:
#line 925 "surface.yy"
    {
			  CM->addHook(PreModule::TERM_HOOK, yyvsp[-4].yyToken, bubble);
			;}
    break;

  case 220:
#line 933 "surface.yy"
    {;}
    break;

  case 221:
#line 935 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			;}
    break;

  case 222:
#line 941 "surface.yy"
    {;}
    break;

  case 223:
#line 943 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			;}
    break;

  case 224:
#line 951 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 226:
#line 955 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 227:
#line 956 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 228:
#line 957 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 229:
#line 958 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 230:
#line 964 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 231:
#line 965 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 232:
#line 966 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 234:
#line 970 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 235:
#line 971 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 236:
#line 972 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 238:
#line 976 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 239:
#line 977 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 240:
#line 978 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 242:
#line 982 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 243:
#line 983 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 244:
#line 984 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 246:
#line 988 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 247:
#line 989 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 248:
#line 990 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 250:
#line 994 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 251:
#line 995 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 252:
#line 996 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 254:
#line 1000 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 255:
#line 1001 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 256:
#line 1002 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 259:
#line 1007 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 260:
#line 1008 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 261:
#line 1011 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 262:
#line 1012 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 263:
#line 1013 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 264:
#line 1014 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 265:
#line 1015 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 266:
#line 1016 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 267:
#line 1017 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 268:
#line 1024 "surface.yy"
    {
			  Token t;
			  if (fragments.size() == 1)
			    t = fragments[0];
			  else
			    t.tokenize(Token::bubbleToPrefixNameCode(fragments), fragments[0].lineNumber());
			  fragClear();
			  yyval.yyToken = t;
			;}
    break;

  case 269:
#line 1035 "surface.yy"
    { fragStore(yyvsp[0].yyToken); ;}
    break;

  case 270:
#line 1036 "surface.yy"
    { fragStore(yyvsp[0].yyToken); ;}
    break;

  case 271:
#line 1037 "surface.yy"
    { fragStore(yyvsp[0].yyToken); ;}
    break;

  case 272:
#line 1040 "surface.yy"
    { fragStore(yyvsp[0].yyToken); ;}
    break;

  case 273:
#line 1041 "surface.yy"
    {;}
    break;

  case 274:
#line 1042 "surface.yy"
    {;}
    break;

  case 275:
#line 1048 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 276:
#line 1049 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 281:
#line 1053 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 282:
#line 1054 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 440:
#line 1159 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 441:
#line 1160 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 442:
#line 1162 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.setCurrentModule(bubble);
			;}
    break;

  case 443:
#line 1166 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 444:
#line 1167 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 445:
#line 1169 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    CM->dump();
			;}
    break;

  case 446:
#line 1175 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 447:
#line 1181 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(bubble);
			;}
    break;

  case 448:
#line 1188 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 449:
#line 1194 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(bubble);
			;}
    break;

  case 450:
#line 1201 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 451:
#line 1207 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(bubble, yyvsp[-3].yyBool);
			;}
    break;

  case 452:
#line 1214 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 453:
#line 1221 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(bubble, number, yyvsp[-3].yyBool);
			;}
    break;

  case 454:
#line 1227 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 455:
#line 1235 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(bubble, number, number2, yyvsp[-3].yyBool);
			;}
    break;

  case 456:
#line 1241 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 457:
#line 1249 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(bubble, number, number2, yyvsp[-3].yyBool);
			;}
    break;

  case 458:
#line 1255 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 459:
#line 1262 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(bubble, number);
			;}
    break;

  case 460:
#line 1268 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 461:
#line 1275 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(bubble, yyvsp[-2].yyBool, number);
			;}
    break;

  case 462:
#line 1281 "surface.yy"
    {
			  interpreter.cont(yyvsp[-1].yyInt64, yyvsp[-3].yyBool);
			;}
    break;

  case 463:
#line 1285 "surface.yy"
    {
			  lexerCmdMode();
			  clear();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 464:
#line 1291 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(bubble);
			;}
    break;

  case 465:
#line 1296 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 466:
#line 1298 "surface.yy"
    {
			  lexerInitialMode();
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(bubble);
			;}
    break;

  case 467:
#line 1305 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 468:
#line 1307 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceSelect(yyvsp[-3].yyBool);
			;}
    break;

  case 469:
#line 1311 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 470:
#line 1313 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceExclude(yyvsp[-3].yyBool);
			;}
    break;

  case 471:
#line 1317 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 472:
#line 1319 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.breakSelect(yyvsp[-3].yyBool);
			;}
    break;

  case 473:
#line 1323 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 474:
#line 1325 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.printConceal(yyvsp[-3].yyBool);
			;}
    break;

  case 475:
#line 1330 "surface.yy"
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			;}
    break;

  case 476:
#line 1337 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 477:
#line 1339 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    CM->showModule();
			;}
    break;

  case 478:
#line 1344 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 479:
#line 1346 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    CM->showModule();
			;}
    break;

  case 480:
#line 1351 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 481:
#line 1353 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showModule(true);
			;}
    break;

  case 482:
#line 1358 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 483:
#line 1360 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentView(bubble))
			    interpreter.showView();
			;}
    break;

  case 484:
#line 1366 "surface.yy"
    {
			  interpreter.showModules(true);
			;}
    break;

  case 485:
#line 1370 "surface.yy"
    {
			  interpreter.showNamedViews();
			;}
    break;

  case 486:
#line 1373 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 487:
#line 1375 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showSortsAndSubsorts();
			;}
    break;

  case 488:
#line 1380 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 489:
#line 1382 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showOps();
			;}
    break;

  case 490:
#line 1387 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 491:
#line 1389 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showVars();
			;}
    break;

  case 492:
#line 1394 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 493:
#line 1396 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showMbs();
			;}
    break;

  case 494:
#line 1401 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 495:
#line 1403 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showEqs();
			;}
    break;

  case 496:
#line 1408 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 497:
#line 1410 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showRls();
			;}
    break;

  case 498:
#line 1415 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 499:
#line 1417 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showSummary();
			;}
    break;

  case 500:
#line 1422 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 501:
#line 1424 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showKinds();
			;}
    break;

  case 502:
#line 1430 "surface.yy"
    {
			  interpreter.showSearchPath(yyvsp[-1].yyInt64);
			;}
    break;

  case 503:
#line 1434 "surface.yy"
    {
			  interpreter.showSearchPathLabels(yyvsp[-1].yyInt64);
			;}
    break;

  case 504:
#line 1438 "surface.yy"
    {
			  interpreter.showSearchGraph();
			;}
    break;

  case 505:
#line 1441 "surface.yy"
    { lexerCmdMode(); clear(); ;}
    break;

  case 506:
#line 1443 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(bubble))
			    interpreter.showProfile();
			;}
    break;

  case 507:
#line 1452 "surface.yy"
    {
			  globalAdvisoryFlag = yyvsp[-1].yyBool;
			;}
    break;

  case 508:
#line 1456 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, yyvsp[-1].yyBool);
			;}
    break;

  case 509:
#line 1460 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, yyvsp[-1].yyBool);
			;}
    break;

  case 510:
#line 1464 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, yyvsp[-1].yyBool);
			;}
    break;

  case 511:
#line 1468 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, yyvsp[-1].yyBool);
			;}
    break;

  case 512:
#line 1472 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, yyvsp[-1].yyBool);
			;}
    break;

  case 513:
#line 1476 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, yyvsp[-1].yyBool);
			;}
    break;

  case 514:
#line 1480 "surface.yy"
    {
			  MemoryCell::setShowGC(yyvsp[-1].yyBool);
			;}
    break;

  case 515:
#line 1484 "surface.yy"
    {
			  interpreter.setPrintFlag(yyvsp[-2].yyPrintFlags, yyvsp[-1].yyBool);
			;}
    break;

  case 516:
#line 1488 "surface.yy"
    {
			  interpreter.setFlag(yyvsp[-2].yyFlags, yyvsp[-1].yyBool);
			;}
    break;

  case 517:
#line 1492 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::BREAK, yyvsp[-1].yyBool);
			;}
    break;

  case 518:
#line 1495 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 519:
#line 1496 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 520:
#line 1498 "surface.yy"
    {
			  interpreter.setAutoImport(yyvsp[-5].yyImportMode, yyvsp[-3].yyToken, yyvsp[-1].yyBool);
			;}
    break;

  case 521:
#line 1501 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 522:
#line 1502 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 523:
#line 1504 "surface.yy"
    {
			  interpreter.setOmodInclude(yyvsp[-3].yyToken, yyvsp[-1].yyBool);
			;}
    break;

  case 524:
#line 1508 "surface.yy"
    {
			  globalVerboseFlag = yyvsp[-1].yyBool;
			;}
    break;

  case 525:
#line 1512 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, yyvsp[-1].yyBool);
			;}
    break;

  case 526:
#line 1516 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, yyvsp[-1].yyBool);
			;}
    break;

  case 527:
#line 1520 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, yyvsp[-1].yyBool);
			;}
    break;

  case 528:
#line 1524 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PROFILE, yyvsp[-1].yyBool);
			;}
    break;

  case 529:
#line 1528 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, yyvsp[-1].yyBool);
			;}
    break;

  case 530:
#line 1535 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			;}
    break;

  case 531:
#line 1539 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			;}
    break;

  case 532:
#line 1543 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			;}
    break;

  case 533:
#line 1547 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			;}
    break;

  case 534:
#line 1554 "surface.yy"
    {
			  MemoryCell::setShowGC(yyvsp[-1].yyBool);
			;}
    break;

  case 535:
#line 1558 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, yyvsp[-1].yyBool);
			;}
    break;

  case 536:
#line 1564 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 538:
#line 1571 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_MIXFIX; ;}
    break;

  case 539:
#line 1572 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_FLAT; ;}
    break;

  case 540:
#line 1573 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_WITH_ALIASES; ;}
    break;

  case 541:
#line 1574 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_WITH_PARENS; ;}
    break;

  case 542:
#line 1575 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_GRAPH; ;}
    break;

  case 543:
#line 1576 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_CONCEAL; ;}
    break;

  case 544:
#line 1577 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_NUMBER; ;}
    break;

  case 545:
#line 1578 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_RAT; ;}
    break;

  case 546:
#line 1579 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_COLOR; ;}
    break;

  case 547:
#line 1580 "surface.yy"
    { yyval.yyPrintFlags = Interpreter::PRINT_FORMAT; ;}
    break;

  case 548:
#line 1583 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE; ;}
    break;

  case 549:
#line 1584 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_CONDITION; ;}
    break;

  case 550:
#line 1585 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_WHOLE; ;}
    break;

  case 551:
#line 1586 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_SUBSTITUTION; ;}
    break;

  case 552:
#line 1587 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_SELECT; ;}
    break;

  case 553:
#line 1588 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_MB; ;}
    break;

  case 554:
#line 1589 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_EQ; ;}
    break;

  case 555:
#line 1590 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_RL; ;}
    break;

  case 556:
#line 1591 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_REWRITE; ;}
    break;

  case 557:
#line 1592 "surface.yy"
    { yyval.yyFlags = Interpreter::TRACE_BODY; ;}
    break;

  case 558:
#line 1595 "surface.yy"
    { yyval.yyBool = true; ;}
    break;

  case 559:
#line 1596 "surface.yy"
    { yyval.yyBool = false; ;}
    break;

  case 560:
#line 1599 "surface.yy"
    { yyval.yyBool = true; ;}
    break;

  case 561:
#line 1600 "surface.yy"
    { yyval.yyBool = false; ;}
    break;

  case 562:
#line 1603 "surface.yy"
    { yyval.yyBool = true; ;}
    break;

  case 563:
#line 1604 "surface.yy"
    { yyval.yyBool = false; ;}
    break;

  case 564:
#line 1607 "surface.yy"
    { yyval.yyBool = true; ;}
    break;

  case 565:
#line 1608 "surface.yy"
    { yyval.yyBool = false; ;}
    break;

  case 566:
#line 1611 "surface.yy"
    { yyval.yyBool = true; ;}
    break;

  case 567:
#line 1612 "surface.yy"
    { yyval.yyBool = false; ;}
    break;

  case 568:
#line 1615 "surface.yy"
    { yyval.yyBool = true; ;}
    break;

  case 569:
#line 1616 "surface.yy"
    { yyval.yyBool = false; ;}
    break;

  case 570:
#line 1619 "surface.yy"
    { yyval.yyInt64 = yyvsp[0].yyInt64; ;}
    break;

  case 571:
#line 1620 "surface.yy"
    { yyval.yyInt64 = NONE; ;}
    break;

  case 572:
#line 1623 "surface.yy"
    { yyval.yyImportMode = ImportModule::PROTECTING; ;}
    break;

  case 573:
#line 1624 "surface.yy"
    { yyval.yyImportMode = ImportModule::EXTENDING; ;}
    break;

  case 574:
#line 1625 "surface.yy"
    { yyval.yyImportMode = ImportModule::INCLUDING; ;}
    break;

  case 575:
#line 1633 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 577:
#line 1635 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 579:
#line 1639 "surface.yy"
    { moduleExpr = bubble; clear(); ;}
    break;

  case 580:
#line 1640 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 581:
#line 1641 "surface.yy"
    {;}
    break;

  case 582:
#line 1642 "surface.yy"
    {;}
    break;

  case 583:
#line 1651 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 585:
#line 1653 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 587:
#line 1655 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 589:
#line 1660 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 591:
#line 1662 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 593:
#line 1664 "surface.yy"
    {;}
    break;

  case 594:
#line 1669 "surface.yy"
    {
			  number = Token::codeToInt64(bubble[1].code());
			  clear();
			;}
    break;

  case 596:
#line 1674 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 598:
#line 1676 "surface.yy"
    {;}
    break;

  case 599:
#line 1685 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 601:
#line 1687 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 603:
#line 1689 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 605:
#line 1694 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 607:
#line 1696 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 609:
#line 1698 "surface.yy"
    {;}
    break;

  case 610:
#line 1703 "surface.yy"
    {
			  number = Token::codeToInt64(bubble[1].code());
			  clear();
			;}
    break;

  case 612:
#line 1708 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 614:
#line 1710 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 616:
#line 1712 "surface.yy"
    {;}
    break;

  case 617:
#line 1716 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 619:
#line 1718 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 621:
#line 1720 "surface.yy"
    {;}
    break;

  case 622:
#line 1725 "surface.yy"
    {
			  number = Token::codeToInt64(bubble[1].code());
			  number2 = Token::codeToInt64(bubble[3].code());
			  clear();
			;}
    break;

  case 624:
#line 1731 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 626:
#line 1733 "surface.yy"
    {;}
    break;

  case 627:
#line 1739 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 629:
#line 1743 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 631:
#line 1748 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 641:
#line 1756 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 642:
#line 1757 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 650:
#line 1761 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 651:
#line 1762 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 658:
#line 1767 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 659:
#line 1768 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 667:
#line 1772 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 668:
#line 1773 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 675:
#line 1777 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 676:
#line 1778 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 683:
#line 1783 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 684:
#line 1784 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 691:
#line 1789 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 692:
#line 1790 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 698:
#line 1795 "surface.yy"
    { store(yyvsp[0].yyToken); ;}
    break;

  case 699:
#line 1796 "surface.yy"
    { yyval.yyToken = yyvsp[0].yyToken; ;}
    break;

  case 702:
#line 1807 "surface.yy"
    {
			  clear();
			  store(yyvsp[0].yyToken);
			  interpreter.addSelected(bubble);
			;}
    break;

  case 703:
#line 1812 "surface.yy"
    { clear(); ;}
    break;

  case 704:
#line 1814 "surface.yy"
    {
			  interpreter.addSelected(bubble);
			;}
    break;


    }

/* Line 999 of yacc.c.  */
#line 6187 "surface.c"

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
	  YYSIZE_T yysize = 0;
	  int yytype = YYTRANSLATE (yychar);
	  char *yymsg;
	  int yyx, yycount;

	  yycount = 0;
	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  for (yyx = yyn < 0 ? -yyn : 0;
	       yyx < (int) (sizeof (yytname) / sizeof (char *)); yyx++)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      yysize += yystrlen (yytname[yyx]) + 15, yycount++;
	  yysize += yystrlen ("syntax error, unexpected ") + 1;
	  yysize += yystrlen (yytname[yytype]);
	  yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg != 0)
	    {
	      char *yyp = yystpcpy (yymsg, "syntax error, unexpected ");
	      yyp = yystpcpy (yyp, yytname[yytype]);

	      if (yycount < 5)
		{
		  yycount = 0;
		  for (yyx = yyn < 0 ? -yyn : 0;
		       yyx < (int) (sizeof (yytname) / sizeof (char *));
		       yyx++)
		    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
		      {
			const char *yyq = ! yycount ? ", expecting " : " or ";
			yyp = yystpcpy (yyp, yyq);
			yyp = yystpcpy (yyp, yytname[yyx]);
			yycount++;
		      }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    yyerror ("syntax error; also virtual memory exhausted");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror ("syntax error");
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      /* Return failure if at end of input.  */
      if (yychar == YYEOF)
        {
	  /* Pop the error token.  */
          YYPOPSTACK;
	  /* Pop the rest of the stack.  */
	  while (yyss < yyssp)
	    {
	      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
	      yydestruct (yystos[*yyssp], yyvsp);
	      YYPOPSTACK;
	    }
	  YYABORT;
        }

      YYDSYMPRINTF ("Error: discarding", yytoken, &yylval, &yylloc);
      yydestruct (yytoken, &yylval);
      yychar = YYEMPTY;

    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*----------------------------------------------------.
| yyerrlab1 -- error raised explicitly by an action.  |
`----------------------------------------------------*/
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

      YYDSYMPRINTF ("Error: popping", yystos[*yyssp], yyvsp, yylsp);
      yydestruct (yystos[yystate], yyvsp);
      yyvsp--;
      yystate = *--yyssp;

      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  YYDPRINTF ((stderr, "Shifting error token, "));

  *++yyvsp = yylval;


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
/*----------------------------------------------.
| yyoverflowlab -- parser overflow comes here.  |
`----------------------------------------------*/
yyoverflowlab:
  yyerror ("parser stack overflow");
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 1844 "surface.yy"


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

