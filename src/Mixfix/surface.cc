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
#define KW_XG_NARROW 270
#define KW_MATCH 271
#define KW_XMATCH 272
#define KW_UNIFY 273
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
#define KW_ATTRIBUTE 295
#define KW_NEWLINE 296
#define KW_WITH 297
#define KW_PARENS 298
#define KW_ALIASES 299
#define KW_GC 300
#define KW_TIME 301
#define KW_STATS 302
#define KW_TIMING 303
#define KW_CMD 304
#define KW_BREAKDOWN 305
#define KW_BREAK 306
#define KW_PATH 307
#define KW_MODULE 308
#define KW_MODULES 309
#define KW_VIEWS 310
#define KW_ALL 311
#define KW_SORTS 312
#define KW_OPS2 313
#define KW_VARS 314
#define KW_MBS 315
#define KW_EQS 316
#define KW_RLS 317
#define KW_SUMMARY 318
#define KW_KINDS 319
#define KW_ADVISE 320
#define KW_VERBOSE 321
#define KW_DO 322
#define KW_CLEAR 323
#define KW_PROTECT 324
#define KW_EXTEND 325
#define KW_INCLUDE 326
#define KW_EXCLUDE 327
#define KW_CONCEAL 328
#define KW_REVEAL 329
#define KW_COMPILE 330
#define KW_COUNT 331
#define KW_DEBUG 332
#define KW_RESUME 333
#define KW_ABORT 334
#define KW_STEP 335
#define KW_WHERE 336
#define KW_CREDUCE 337
#define KW_DUMP 338
#define KW_PROFILE 339
#define KW_NUMBER 340
#define KW_RAT 341
#define KW_COLOR 342
#define SIMPLE_NUMBER 343
#define KW_PWD 344
#define KW_CD 345
#define KW_PUSHD 346
#define KW_POPD 347
#define KW_LS 348
#define KW_LOAD 349
#define KW_QUIT 350
#define KW_EOF 351
#define KW_ENDM 352
#define KW_IMPORT 353
#define KW_ENDV 354
#define KW_SORT 355
#define KW_SUBSORT 356
#define KW_OP 357
#define KW_OPS 358
#define KW_MSGS 359
#define KW_VAR 360
#define KW_CLASS 361
#define KW_SUBCLASS 362
#define KW_MB 363
#define KW_CMB 364
#define KW_EQ 365
#define KW_CEQ 366
#define KW_RL 367
#define KW_CRL 368
#define KW_IS 369
#define KW_FROM 370
#define KW_ARROW 371
#define KW_ARROW2 372
#define KW_PARTIAL 373
#define KW_IF 374
#define KW_LABEL 375
#define KW_TO 376
#define KW_COLON2 377
#define KW_ASSOC 378
#define KW_COMM 379
#define KW_ID 380
#define KW_IDEM 381
#define KW_ITER 382
#define KW_LEFT 383
#define KW_RIGHT 384
#define KW_PREC 385
#define KW_GATHER 386
#define KW_METADATA 387
#define KW_STRAT 388
#define KW_POLY 389
#define KW_MEMO 390
#define KW_FROZEN 391
#define KW_CTOR 392
#define KW_LATEX 393
#define KW_SPECIAL 394
#define KW_CONFIG 395
#define KW_OBJ 396
#define KW_MSG 397
#define KW_DITTO 398
#define KW_FORMAT 399
#define KW_ID_HOOK 400
#define KW_OP_HOOK 401
#define KW_TERM_HOOK 402
#define KW_IN 403
#define IDENTIFIER 404
#define NUMERIC_ID 405
#define ENDS_IN_DOT 406
#define FORCE_LOOKAHEAD 407




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
#line 103 "surface.yy"
typedef union YYSTYPE {
  bool yyBool;
  Int64 yyInt64;
  const char* yyString;
  Token yyToken;
  ImportModule::ImportMode yyImportMode;
  Interpreter::Flags yyFlags;
  Interpreter::PrintFlags yyPrintFlags;
  Interpreter::SearchKind yySearchKind;
} YYSTYPE;
/* Line 196 of yacc.c.  */
#line 474 "surface.c"
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */
#line 114 "surface.yy"

int yylex(YYSTYPE* lvalp);


/* Line 219 of yacc.c.  */
#line 489 "surface.c"

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
#define YYFINAL  109
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2360

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  167
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  237
/* YYNRULES -- Number of rules. */
#define YYNRULES  601
/* YYNRULES -- Number of states. */
#define YYNSTATES  901

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   407

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
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
static const unsigned short int yyprhs[] =
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
     280,   284,   286,   289,   291,   293,   295,   298,   299,   302,
     303,   308,   309,   314,   315,   316,   322,   327,   332,   333,
     334,   340,   341,   342,   343,   351,   352,   353,   359,   360,
     361,   362,   370,   371,   372,   378,   379,   380,   381,   389,
     390,   391,   397,   402,   403,   409,   410,   415,   418,   420,
     423,   425,   429,   433,   436,   438,   441,   443,   445,   447,
     451,   453,   455,   457,   459,   461,   464,   468,   470,   472,
     474,   477,   478,   480,   481,   486,   490,   492,   496,   497,
     500,   502,   504,   507,   510,   512,   514,   515,   519,   521,
     523,   526,   527,   533,   534,   540,   541,   547,   548,   554,
     556,   558,   560,   561,   567,   569,   571,   573,   576,   577,
     583,   588,   590,   592,   593,   596,   598,   601,   603,   606,
     610,   614,   618,   620,   621,   623,   624,   627,   628,   631,
     634,   635,   640,   642,   644,   645,   651,   652,   657,   659,
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
     861,   863,   864,   868,   869,   873,   874,   878,   879,   883,
     884,   889,   890,   895,   896,   901,   902,   907,   908,   913,
     914,   918,   919,   923,   924,   928,   933,   934,   938,   940,
     941,   946,   947,   952,   953,   958,   959,   964,   969,   970,
     975,   976,   981,   982,   987,   988,   993,   997,  1001,  1002,
    1007,  1008,  1013,  1014,  1019,  1020,  1025,  1026,  1031,  1032,
    1037,  1038,  1043,  1044,  1049,  1054,  1060,  1065,  1066,  1071,
    1077,  1083,  1090,  1096,  1102,  1109,  1115,  1121,  1127,  1133,
    1140,  1146,  1151,  1152,  1153,  1161,  1162,  1163,  1172,  1177,
    1183,  1189,  1195,  1200,  1206,  1209,  1212,  1215,  1218,  1224,
    1229,  1230,  1234,  1236,  1238,  1241,  1244,  1246,  1248,  1250,
    1252,  1254,  1256,  1257,  1259,  1261,  1263,  1265,  1267,  1269,
    1271,  1273,  1275,  1277,  1279,  1281,  1283,  1285,  1287,  1289,
    1291,  1293,  1295,  1297,  1299,  1301,  1303,  1305,  1306,  1308,
    1309,  1311,  1313,  1315,  1317,  1318,  1322,  1323,  1327,  1329,
    1330,  1334,  1336,  1337,  1341,  1342,  1346,  1347,  1351,  1353,
    1354,  1358,  1359,  1363,  1365,  1366,  1370,  1372,  1373,  1377,
    1378,  1382,  1383,  1387,  1388,  1392,  1394,  1395,  1399,  1400,
    1404,  1405,  1409,  1411,  1412,  1416,  1417,  1421,  1423,  1424,
    1428,  1429,  1433,  1435,  1436,  1440,  1441,  1445,  1447,  1448,
    1452,  1453,  1457,  1459,  1460,  1464,  1466,  1467,  1471,  1472,
    1476,  1478,  1480,  1482,  1484,  1486,  1488,  1490,  1492,  1494,
    1496,  1498,  1500,  1502,  1504,  1506,  1508,  1510,  1512,  1514,
    1516,  1518,  1520,  1522,  1524,  1526,  1528,  1530,  1532,  1534,
    1536,  1538,  1540,  1542,  1544,  1546,  1548,  1551,  1553,  1555,
    1557,  1559,  1562,  1564,  1566,  1568,  1570,  1572,  1574,  1576,
    1578,  1580
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const short int yyrhs[] =
{
     168,     0,    -1,   169,    -1,    -1,   217,    -1,   202,    -1,
     170,    -1,   304,    -1,    -1,   162,   171,     3,    -1,    -1,
      94,   172,     3,    -1,    89,    -1,    -1,    90,   173,     3,
      -1,    -1,    91,   174,     3,    -1,    92,    -1,    -1,    93,
     175,     4,    -1,    95,    -1,    96,    -1,   293,   283,    -1,
     296,   124,    -1,   183,   283,    -1,   180,   283,    -1,   181,
     283,    -1,   177,   133,   176,    -1,   165,    -1,   178,    -1,
     177,   133,   177,    -1,   179,    -1,   180,    -1,   183,    -1,
     181,    -1,   292,    -1,   178,   134,   185,    -1,    -1,   179,
     135,   182,   184,   136,    -1,   122,   177,   123,    -1,   184,
     128,   292,    -1,   292,    -1,    -1,    -1,   122,   186,   188,
     187,   123,    -1,   189,    -1,     1,    -1,   189,   128,   190,
      -1,   190,    -1,   100,   287,   131,   287,    -1,   130,   292,
     131,   292,    -1,    -1,    -1,   102,   191,   193,   131,   192,
     195,    -1,    -1,   120,   194,   263,   262,   264,    -1,    -1,
      -1,   126,   196,   197,   127,    -1,    -1,   197,   198,    -1,
     198,    -1,   144,   163,    -1,    -1,   145,   122,   199,   279,
     123,    -1,    -1,   158,   122,   200,   279,   123,    -1,    -1,
     152,   122,   201,     5,   123,    -1,    -1,    -1,    -1,     8,
     203,   292,   115,   177,   204,   131,   177,   205,   282,   206,
      99,    -1,   206,   207,    -1,    -1,   100,   287,   131,   209,
      -1,   105,   255,   120,   225,    -1,    -1,   102,   208,   210,
      -1,     1,   124,    -1,   287,   283,    -1,   165,    -1,    -1,
      -1,   120,   211,   263,   262,   264,   131,   212,   214,    -1,
      -1,   131,   213,   214,    -1,   124,    -1,   165,    -1,    -1,
     122,   216,   123,    -1,    -1,    -1,   226,   218,   292,   219,
     221,   282,   227,    97,    -1,   124,    -1,   165,    -1,   135,
     222,   136,    -1,    -1,   222,   128,   223,    -1,   223,    -1,
     292,   132,   177,    -1,   165,    -1,   264,   283,    -1,   224,
      -1,     6,    -1,     7,    -1,   227,   228,    -1,    -1,    98,
     176,    -1,    -1,   100,   229,   284,   220,    -1,    -1,   101,
     230,   285,   220,    -1,    -1,    -1,   102,   231,   120,   232,
     258,    -1,   103,   256,   120,   258,    -1,   105,   255,   120,
     225,    -1,    -1,    -1,   108,   233,   120,   234,   214,    -1,
      -1,    -1,    -1,   109,   235,   120,   236,   119,   237,   214,
      -1,    -1,    -1,   110,   238,   121,   239,   214,    -1,    -1,
      -1,    -1,   111,   240,   121,   241,   119,   242,   214,    -1,
      -1,    -1,   112,   243,   117,   244,   214,    -1,    -1,    -1,
      -1,   113,   245,   117,   246,   119,   247,   214,    -1,    -1,
      -1,   156,   248,   120,   249,   258,    -1,   104,   256,   120,
     258,    -1,    -1,   106,   292,   250,   252,   124,    -1,    -1,
     107,   251,   285,   220,    -1,     1,   124,    -1,   129,    -1,
     129,   253,    -1,   254,    -1,   253,   128,   254,    -1,   293,
     120,   292,    -1,   255,   294,    -1,   294,    -1,   256,   257,
      -1,   257,    -1,   294,    -1,   215,    -1,   264,   263,   259,
      -1,   260,    -1,   224,    -1,   260,    -1,   124,    -1,   224,
      -1,   262,   261,    -1,   264,   267,   283,    -1,   224,    -1,
     116,    -1,   118,    -1,   263,   264,    -1,    -1,   287,    -1,
      -1,   126,   265,   266,   127,    -1,   266,   128,   287,    -1,
     287,    -1,   126,   268,   127,    -1,    -1,   268,   270,    -1,
     270,    -1,   139,    -1,   142,   139,    -1,   143,   139,    -1,
     137,    -1,   138,    -1,    -1,   269,   271,   278,    -1,   140,
      -1,   141,    -1,   144,   163,    -1,    -1,   145,   122,   272,
     279,   123,    -1,    -1,   158,   122,   273,   279,   123,    -1,
      -1,   147,   122,   274,   279,   123,    -1,    -1,   148,   122,
     275,   279,   123,    -1,   149,    -1,   151,    -1,   150,    -1,
      -1,   150,   122,   276,   279,   123,    -1,   154,    -1,   155,
      -1,   156,    -1,   146,   163,    -1,    -1,   152,   122,   277,
       5,   123,    -1,   153,   122,   280,   123,    -1,   157,    -1,
     166,    -1,    -1,   279,   163,    -1,   163,    -1,   280,   281,
      -1,   281,    -1,   159,   292,    -1,   159,   292,   215,    -1,
     160,   292,   215,    -1,   161,   292,   215,    -1,   114,    -1,
      -1,   124,    -1,    -1,   284,   287,    -1,    -1,   285,   287,
      -1,   285,   125,    -1,    -1,   287,   286,   284,   125,    -1,
     288,    -1,   295,    -1,    -1,   288,   135,   289,   290,   136,
      -1,    -1,   290,   128,   291,   288,    -1,   288,    -1,   298,
      -1,   299,    -1,   301,    -1,   302,    -1,   124,    -1,   297,
      -1,   128,    -1,   131,    -1,   299,    -1,   301,    -1,   302,
      -1,   298,    -1,   299,    -1,   302,    -1,   124,    -1,   125,
      -1,   116,    -1,   118,    -1,   121,    -1,   117,    -1,   119,
      -1,   163,    -1,   299,    -1,   303,    -1,   121,    -1,   129,
      -1,   133,    -1,   134,    -1,   117,    -1,   119,    -1,   114,
      -1,   130,    -1,   131,    -1,   124,    -1,   165,    -1,   163,
      -1,   135,    -1,   136,    -1,   133,    -1,   134,    -1,   129,
      -1,   132,    -1,   130,    -1,   115,    -1,   114,    -1,   297,
      -1,   165,    -1,   128,    -1,   131,    -1,   156,    -1,   300,
      -1,    98,    -1,   100,    -1,   101,    -1,   102,    -1,   103,
      -1,   105,    -1,   104,    -1,   106,    -1,   107,    -1,   108,
      -1,   109,    -1,   110,    -1,   111,    -1,   112,    -1,   113,
      -1,    97,    -1,    99,    -1,   125,    -1,   120,    -1,   116,
      -1,   118,    -1,   121,    -1,   117,    -1,   119,    -1,   126,
      -1,   127,    -1,   303,    -1,   137,    -1,   138,    -1,   139,
      -1,   140,    -1,   141,    -1,   142,    -1,   143,    -1,   144,
      -1,   145,    -1,   147,    -1,   148,    -1,   149,    -1,   151,
      -1,   152,    -1,   153,    -1,   150,    -1,   146,    -1,   154,
      -1,   155,    -1,   157,    -1,   158,    -1,   159,    -1,   160,
      -1,   161,    -1,    -1,    32,   305,   214,    -1,    -1,    83,
     306,   214,    -1,    -1,     9,   307,   351,    -1,    -1,    82,
     308,   351,    -1,    -1,   348,    11,   309,   351,    -1,    -1,
     348,    12,   310,   355,    -1,    -1,   348,    19,   311,   364,
      -1,    -1,   348,    20,   312,   364,    -1,    -1,   348,    21,
     313,   355,    -1,    -1,   346,   314,   364,    -1,    -1,   347,
     315,   355,    -1,    -1,    18,   316,   355,    -1,   348,    22,
     349,   124,    -1,    -1,    13,   317,   351,    -1,   215,    -1,
      -1,    28,   343,   318,   398,    -1,    -1,    28,   344,   319,
     398,    -1,    -1,    51,   343,   320,   398,    -1,    -1,    36,
     345,   321,   398,    -1,    67,    68,   149,   124,    -1,    -1,
      25,     6,   322,   214,    -1,    -1,    25,    53,   323,   214,
      -1,    -1,    25,    56,   324,   214,    -1,    -1,    25,     8,
     325,   214,    -1,    25,    54,   124,    -1,    25,    55,   124,
      -1,    -1,    25,    57,   326,   214,    -1,    -1,    25,    58,
     327,   214,    -1,    -1,    25,    59,   328,   214,    -1,    -1,
      25,    60,   329,   214,    -1,    -1,    25,    61,   330,   214,
      -1,    -1,    25,    62,   331,   214,    -1,    -1,    25,    63,
     332,   214,    -1,    -1,    25,    64,   333,   214,    -1,    25,
      52,    88,   124,    -1,    25,    52,   130,    88,   124,    -1,
      25,    23,    37,   124,    -1,    -1,    25,    84,   334,   214,
      -1,    24,    25,    65,   342,   124,    -1,    24,    25,    47,
     342,   124,    -1,    24,    25,    13,    47,   342,   124,    -1,
      24,    25,    48,   342,   124,    -1,    24,    25,    50,   342,
     124,    -1,    24,    25,    13,    48,   342,   124,    -1,    24,
      25,    49,   342,   124,    -1,    24,    25,    45,   342,   124,
      -1,    24,    36,   340,   342,   124,    -1,    24,    36,    40,
     342,   124,    -1,    24,    36,    40,    41,   342,   124,    -1,
      24,    28,   341,   342,   124,    -1,    24,    51,   342,   124,
      -1,    -1,    -1,    24,   350,   335,   403,   336,   342,   124,
      -1,    -1,    -1,    24,     7,    71,   337,   403,   338,   342,
     124,    -1,    24,    66,   342,   124,    -1,    24,    68,   149,
     342,   124,    -1,    24,    68,    62,   342,   124,    -1,    24,
      75,    76,   342,   124,    -1,    24,    84,   342,   124,    -1,
      24,    68,    84,   342,   124,    -1,    78,   124,    -1,    79,
     124,    -1,    80,   124,    -1,    81,   124,    -1,    24,    45,
      25,   342,   124,    -1,    24,    47,   342,   124,    -1,    -1,
       1,   339,   124,    -1,    38,    -1,    39,    -1,    42,    44,
      -1,    42,    43,    -1,    37,    -1,    73,    -1,    85,    -1,
      86,    -1,    87,    -1,   158,    -1,    -1,    34,    -1,    31,
      -1,    35,    -1,    32,    -1,    60,    -1,    61,    -1,    62,
      -1,    12,    -1,    29,    -1,    30,    -1,    26,    -1,    27,
      -1,    32,    -1,    33,    -1,    72,    -1,    71,    -1,    73,
      -1,    74,    -1,    14,    -1,    15,    -1,    23,    -1,    17,
      -1,    16,    -1,    77,    -1,    -1,    88,    -1,    -1,    69,
      -1,    70,    -1,    71,    -1,   389,    -1,    -1,   392,   352,
     214,    -1,    -1,   120,   354,   214,    -1,   214,    -1,    -1,
     126,   356,   358,    -1,   389,    -1,    -1,   393,   357,   214,
      -1,    -1,   164,   359,   361,    -1,    -1,   394,   360,   214,
      -1,   387,    -1,    -1,   127,   362,   351,    -1,    -1,   395,
     363,   214,    -1,   387,    -1,    -1,   126,   365,   367,    -1,
     389,    -1,    -1,   393,   366,   214,    -1,    -1,   164,   368,
     371,    -1,    -1,   128,   369,   381,    -1,    -1,   396,   370,
     214,    -1,   387,    -1,    -1,   127,   372,   351,    -1,    -1,
     128,   373,   375,    -1,    -1,   397,   374,   214,    -1,   387,
      -1,    -1,   164,   376,   378,    -1,    -1,   394,   377,   214,
      -1,   387,    -1,    -1,   127,   379,   351,    -1,    -1,   395,
     380,   214,    -1,   387,    -1,    -1,   164,   382,   384,    -1,
      -1,   394,   383,   214,    -1,   387,    -1,    -1,   127,   385,
     351,    -1,    -1,   395,   386,   214,    -1,   387,    -1,    -1,
     122,   388,   214,    -1,   214,    -1,    -1,   122,   390,   214,
      -1,    -1,   162,   391,   353,    -1,   165,    -1,   163,    -1,
     164,    -1,   126,    -1,   127,    -1,   120,    -1,   124,    -1,
     128,    -1,   163,    -1,   164,    -1,   127,    -1,   120,    -1,
     124,    -1,   128,    -1,   163,    -1,   126,    -1,   127,    -1,
     162,    -1,   120,    -1,   128,    -1,   163,    -1,   164,    -1,
     126,    -1,   162,    -1,   120,    -1,   128,    -1,   163,    -1,
     126,    -1,   127,    -1,   162,    -1,   120,    -1,   163,    -1,
     164,    -1,   126,    -1,   162,    -1,   120,    -1,   401,   399,
      -1,   400,    -1,   124,    -1,   400,    -1,   165,    -1,   401,
     402,    -1,   402,    -1,   403,    -1,   215,    -1,   163,    -1,
     164,    -1,   126,    -1,   127,    -1,   162,    -1,   120,    -1,
     128,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
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
     639,   647,   656,   657,   660,   660,   663,   664,   667,   674,
     674,   677,   677,   680,   681,   680,   684,   686,   688,   689,
     688,   692,   693,   694,   692,   697,   698,   697,   701,   702,
     703,   701,   706,   707,   706,   710,   711,   712,   710,   715,
     716,   715,   719,   725,   724,   731,   731,   734,   745,   746,
     749,   750,   753,   758,   759,   762,   763,   766,   767,   770,
     771,   772,   779,   780,   785,   792,   799,   800,   803,   804,
     807,   808,   811,   816,   816,   823,   824,   827,   828,   831,
     832,   835,   839,   843,   849,   853,   857,   857,   859,   863,
     867,   868,   868,   870,   870,   872,   872,   874,   874,   876,
     880,   884,   889,   889,   891,   895,   899,   903,   907,   907,
     909,   910,   920,   921,   924,   925,   928,   929,   932,   933,
     934,   935,   941,   943,   949,   951,   959,   960,   963,   964,
     965,   965,   972,   984,   985,   985,   989,   989,   991,   997,
     997,   997,   997,   997,  1000,  1000,  1000,  1001,  1001,  1001,
    1004,  1004,  1004,  1004,  1005,  1005,  1005,  1005,  1005,  1005,
    1008,  1008,  1008,  1009,  1009,  1009,  1009,  1010,  1010,  1010,
    1010,  1010,  1013,  1013,  1019,  1019,  1019,  1019,  1019,  1019,
    1019,  1019,  1020,  1020,  1023,  1023,  1023,  1023,  1026,  1026,
    1029,  1029,  1029,  1029,  1029,  1029,  1030,  1030,  1030,  1031,
    1031,  1031,  1031,  1031,  1031,  1031,  1031,  1034,  1034,  1034,
    1034,  1034,  1034,  1034,  1037,  1037,  1037,  1040,  1040,  1040,
    1040,  1040,  1040,  1040,  1041,  1041,  1041,  1041,  1041,  1041,
    1042,  1042,  1042,  1042,  1043,  1043,  1043,  1043,  1044,  1044,
    1044,  1071,  1071,  1076,  1076,  1083,  1082,  1095,  1094,  1107,
    1106,  1119,  1118,  1131,  1130,  1144,  1143,  1157,  1156,  1169,
    1168,  1182,  1181,  1194,  1193,  1205,  1210,  1209,  1220,  1226,
    1226,  1232,  1232,  1238,  1238,  1244,  1244,  1250,  1258,  1258,
    1264,  1264,  1270,  1270,  1276,  1276,  1282,  1286,  1290,  1290,
    1296,  1296,  1302,  1302,  1308,  1308,  1314,  1314,  1320,  1320,
    1326,  1326,  1332,  1332,  1338,  1342,  1346,  1350,  1350,  1359,
    1363,  1367,  1371,  1375,  1379,  1383,  1387,  1391,  1395,  1399,
    1403,  1407,  1411,  1412,  1411,  1417,  1418,  1417,  1423,  1427,
    1431,  1435,  1439,  1443,  1450,  1454,  1458,  1462,  1469,  1473,
    1480,  1480,  1487,  1488,  1489,  1490,  1491,  1492,  1493,  1494,
    1495,  1496,  1499,  1500,  1501,  1502,  1503,  1504,  1505,  1506,
    1507,  1508,  1509,  1512,  1513,  1516,  1517,  1520,  1521,  1524,
    1525,  1530,  1531,  1532,  1535,  1536,  1539,  1540,  1543,  1544,
    1547,  1548,  1549,  1558,  1559,  1559,  1563,  1563,  1565,  1575,
    1575,  1577,  1578,  1578,  1583,  1583,  1585,  1585,  1587,  1591,
    1591,  1593,  1593,  1595,  1608,  1608,  1610,  1611,  1611,  1620,
    1620,  1622,  1622,  1624,  1624,  1626,  1634,  1634,  1636,  1636,
    1638,  1638,  1640,  1648,  1648,  1650,  1650,  1652,  1661,  1660,
    1667,  1667,  1669,  1677,  1677,  1679,  1679,  1681,  1689,  1689,
    1691,  1691,  1693,  1696,  1696,  1698,  1702,  1702,  1704,  1704,
    1706,  1717,  1717,  1717,  1717,  1717,  1717,  1717,  1720,  1720,
    1720,  1720,  1720,  1720,  1724,  1724,  1724,  1724,  1724,  1724,
    1728,  1728,  1728,  1728,  1728,  1728,  1732,  1732,  1732,  1732,
    1732,  1736,  1736,  1736,  1736,  1736,  1742,  1743,  1746,  1747,
    1750,  1757,  1758,  1761,  1766,  1773,  1773,  1773,  1773,  1773,
    1773,  1773
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
  "top", "item", "directive", "@1", "@2", "@3", "@4", "@5",
  "moduleExprDot", "moduleExpr", "moduleExpr2", "moduleExpr3",
  "renameExpr", "instantExpr", "@6", "parenExpr", "argList", "renaming",
  "@7", "@8", "renaming2", "mappingList", "mapping", "@9", "@10",
  "fromSpec", "@11", "toAttributes", "@12", "toAttributeList",
  "toAttribute", "@13", "@14", "@15", "view", "@16", "@17", "@18",
  "viewDecList", "viewDeclaration", "@19", "sortDot", "viewEndOpMap",
  "@20", "@21", "@22", "endBubble", "parenBubble", "@23", "module", "@24",
  "@25", "dot", "parameters", "parameterList", "parameter", "badType",
  "typeDot", "startModule", "decList", "declaration", "@26", "@27", "@28",
  "@29", "@30", "@31", "@32", "@33", "@34", "@35", "@36", "@37", "@38",
  "@39", "@40", "@41", "@42", "@43", "@44", "@45", "@46", "@47", "@48",
  "classDef", "cPairList", "cPair", "varNameList", "opNameList",
  "simpleOpName", "domainRangeAttr", "dra2", "rangeAttr", "typeAttr",
  "arrow", "typeList", "typeName", "@49", "sortNames", "attributes",
  "attributeList", "idKeyword", "attribute", "@50", "@51", "@52", "@53",
  "@54", "@55", "@56", "identity", "idList", "hookList", "hook",
  "expectedIs", "expectedDot", "sortNameList", "subsortList", "@57",
  "sortName", "sortNameFrag", "@58", "sortNameFrags", "@59", "token",
  "tokenBarDot", "tokenBarColon", "sortToken", "endsInDot", "inert",
  "identifier", "startKeyword", "startKeyword2", "midKeyword",
  "attrKeyword", "attrKeyword2", "command", "@60", "@61", "@62", "@63",
  "@64", "@65", "@66", "@67", "@68", "@69", "@70", "@71", "@72", "@73",
  "@74", "@75", "@76", "@77", "@78", "@79", "@80", "@81", "@82", "@83",
  "@84", "@85", "@86", "@87", "@88", "@89", "@90", "@91", "@92", "@93",
  "@94", "printOption", "traceOption", "polarity", "select", "exclude",
  "conceal", "search", "match", "optDebug", "optNumber", "importMode",
  "moduleAndTerm", "@95", "inEnd", "@96", "numberModuleTerm", "@97", "@98",
  "numberModuleTerm1", "@99", "@100", "numberModuleTerm2", "@101", "@102",
  "numbersModuleTerm", "@103", "@104", "numbersModuleTerm1", "@105",
  "@106", "@107", "numbersModuleTerm2", "@108", "@109", "@110",
  "numbersModuleTerm3", "@111", "@112", "numbersModuleTerm4", "@113",
  "@114", "numbersModuleTerm5", "@115", "@116", "numbersModuleTerm6",
  "@117", "@118", "miscEndBubble", "@119", "initialEndBubble", "@120",
  "@121", "cTokenBarIn", "cTokenBarLeftIn", "cTokenBarDotNumber",
  "cTokenBarDotRight", "cTokenBarDotCommaNumber", "cTokenBarDotCommaRight",
  "opSelect", "endSelect", "badSelect", "cOpNameList", "cSimpleOpName",
  "cSimpleTokenBarDot", 0
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
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
      58,    61,    40,    41,    46,    60,    91,    93,    44,   124,
     375,   376,   377,    43,    42,   123,   125,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned short int yyr1[] =
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
     223,   224,   225,   225,   226,   226,   227,   227,   228,   229,
     228,   230,   228,   231,   232,   228,   228,   228,   233,   234,
     228,   235,   236,   237,   228,   238,   239,   228,   240,   241,
     242,   228,   243,   244,   228,   245,   246,   247,   228,   248,
     249,   228,   228,   250,   228,   251,   228,   228,   252,   252,
     253,   253,   254,   255,   255,   256,   256,   257,   257,   258,
     258,   258,   259,   259,   259,   260,   261,   261,   262,   262,
     263,   263,   264,   265,   264,   266,   266,   267,   267,   268,
     268,   269,   269,   269,   270,   270,   271,   270,   270,   270,
     270,   272,   270,   273,   270,   274,   270,   275,   270,   270,
     270,   270,   276,   270,   270,   270,   270,   270,   277,   270,
     270,   270,   278,   278,   279,   279,   280,   280,   281,   281,
     281,   281,   282,   282,   283,   283,   284,   284,   285,   285,
     286,   285,   287,   288,   289,   288,   291,   290,   290,   292,
     292,   292,   292,   292,   293,   293,   293,   293,   293,   293,
     294,   294,   294,   294,   294,   294,   294,   294,   294,   294,
     295,   295,   295,   295,   295,   295,   295,   295,   295,   295,
     295,   295,   296,   296,   297,   297,   297,   297,   297,   297,
     297,   297,   297,   297,   298,   298,   298,   298,   299,   299,
     300,   300,   300,   300,   300,   300,   300,   300,   300,   300,
     300,   300,   300,   300,   300,   300,   300,   301,   301,   301,
     301,   301,   301,   301,   302,   302,   302,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   303,   303,   303,   303,   303,   303,   303,   303,   303,
     303,   305,   304,   306,   304,   307,   304,   308,   304,   309,
     304,   310,   304,   311,   304,   312,   304,   313,   304,   314,
     304,   315,   304,   316,   304,   304,   317,   304,   304,   318,
     304,   319,   304,   320,   304,   321,   304,   304,   322,   304,
     323,   304,   324,   304,   325,   304,   304,   304,   326,   304,
     327,   304,   328,   304,   329,   304,   330,   304,   331,   304,
     332,   304,   333,   304,   304,   304,   304,   334,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     304,   304,   335,   336,   304,   337,   338,   304,   304,   304,
     304,   304,   304,   304,   304,   304,   304,   304,   304,   304,
     339,   304,   340,   340,   340,   340,   340,   340,   340,   340,
     340,   340,   341,   341,   341,   341,   341,   341,   341,   341,
     341,   341,   341,   342,   342,   343,   343,   344,   344,   345,
     345,   346,   346,   346,   347,   347,   348,   348,   349,   349,
     350,   350,   350,   351,   352,   351,   354,   353,   353,   356,
     355,   355,   357,   355,   359,   358,   360,   358,   358,   362,
     361,   363,   361,   361,   365,   364,   364,   366,   364,   368,
     367,   369,   367,   370,   367,   367,   372,   371,   373,   371,
     374,   371,   371,   376,   375,   377,   375,   375,   379,   378,
     380,   378,   378,   382,   381,   383,   381,   381,   385,   384,
     386,   384,   384,   388,   387,   387,   390,   389,   391,   389,
     389,   392,   392,   392,   392,   392,   392,   392,   393,   393,
     393,   393,   393,   393,   394,   394,   394,   394,   394,   394,
     395,   395,   395,   395,   395,   395,   396,   396,   396,   396,
     396,   397,   397,   397,   397,   397,   398,   398,   399,   399,
     400,   401,   401,   402,   402,   403,   403,   403,   403,   403,
     403,   403
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
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
       3,     1,     2,     1,     1,     1,     2,     0,     2,     0,
       4,     0,     4,     0,     0,     5,     4,     4,     0,     0,
       5,     0,     0,     0,     7,     0,     0,     5,     0,     0,
       0,     7,     0,     0,     5,     0,     0,     0,     7,     0,
       0,     5,     4,     0,     5,     0,     4,     2,     1,     2,
       1,     3,     3,     2,     1,     2,     1,     1,     1,     3,
       1,     1,     1,     1,     1,     2,     3,     1,     1,     1,
       2,     0,     1,     0,     4,     3,     1,     3,     0,     2,
       1,     1,     2,     2,     1,     1,     0,     3,     1,     1,
       2,     0,     5,     0,     5,     0,     5,     0,     5,     1,
       1,     1,     0,     5,     1,     1,     1,     2,     0,     5,
       4,     1,     1,     0,     2,     1,     2,     1,     2,     3,
       3,     3,     1,     0,     1,     0,     2,     0,     2,     2,
       0,     4,     1,     1,     0,     5,     0,     4,     1,     1,
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
       1,     0,     3,     0,     3,     0,     3,     0,     3,     0,
       4,     0,     4,     0,     4,     0,     4,     0,     4,     0,
       3,     0,     3,     0,     3,     4,     0,     3,     1,     0,
       4,     0,     4,     0,     4,     0,     4,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     3,     3,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     4,     5,     4,     0,     4,     5,
       5,     6,     5,     5,     6,     5,     5,     5,     5,     6,
       5,     4,     0,     0,     7,     0,     0,     8,     4,     5,
       5,     5,     4,     5,     2,     2,     2,     2,     5,     4,
       0,     3,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     0,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     0,     1,     0,
       1,     1,     1,     1,     0,     3,     0,     3,     1,     0,
       3,     1,     0,     3,     0,     3,     0,     3,     1,     0,
       3,     0,     3,     1,     0,     3,     1,     0,     3,     0,
       3,     0,     3,     0,     3,     1,     0,     3,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       0,     3,     1,     0,     3,     0,     3,     1,     0,     3,
       0,     3,     1,     0,     3,     1,     0,     3,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     2,     1,     1,     1,
       1,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       0,   440,   104,   105,    69,   345,   366,   471,   472,   475,
     474,   363,   473,     0,     0,     0,   341,     0,     0,     0,
     476,     0,     0,     0,     0,   347,   343,    12,    13,    15,
      17,    18,    10,    20,    21,    89,     8,     0,     2,     6,
       5,   368,     4,    91,     7,   359,   361,     0,     0,     0,
       0,     0,     0,     0,     0,   452,     0,     0,     0,     0,
       0,     0,   480,   481,   482,     0,     0,   422,   378,   384,
       0,     0,   380,     0,     0,   382,   388,   390,   392,   394,
     396,   398,   400,   402,   407,   465,   466,   468,   467,   369,
     371,     0,   469,   470,   375,   373,     0,   434,   435,   436,
     437,     0,     0,     0,     0,     0,     0,     0,     0,     1,
       0,     0,     0,   349,   351,   353,   355,   357,   479,   441,
     305,   290,   306,   291,   292,   293,   294,   296,   295,   297,
     298,   299,   300,   301,   302,   303,   304,   283,   282,   309,
     312,   310,   313,   308,   311,   243,   307,   314,   315,   286,
     279,   281,   287,   280,   277,   278,   275,   276,   317,   318,
     319,   320,   321,   322,   323,   324,   325,   333,   326,   327,
     328,   332,   329,   330,   331,   334,   335,   288,   336,   337,
     338,   339,   340,   274,   285,     0,   284,   239,   240,   289,
     241,   242,   316,   555,   546,   556,   553,   554,   557,   548,
     551,   552,   550,   346,   483,   484,   367,   561,   562,   489,
     560,   563,   558,   559,   364,   491,   492,   425,     0,     0,
       0,     0,     0,     0,     0,   460,   461,   462,   454,   456,
     453,   455,   457,   458,   459,     0,   446,   442,   443,     0,
       0,   447,   448,   449,   450,   451,     0,     0,   463,   464,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   386,   387,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    87,
      88,   342,     0,     0,     0,   348,   344,    14,    16,    19,
      11,    90,     9,    92,   504,   360,   506,   507,   362,     0,
       0,     0,     0,     0,   478,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   445,   444,     0,     0,   439,   421,
     428,     0,     0,     0,     0,   432,   600,   597,   598,   601,
     599,   595,   596,   423,   379,   385,   406,   404,     0,   381,
     383,   389,   391,   393,   395,   397,   399,   401,   403,   408,
     590,   594,   370,   587,     0,   592,   593,   372,   376,   374,
     377,    97,     0,     0,   350,   352,   354,   356,   358,   365,
       0,    70,    29,    31,    32,    34,    33,    35,   547,   486,
     488,   549,   485,   568,   543,   565,   566,   569,   567,   564,
     494,   545,   490,   498,   496,   493,   426,     0,     0,   416,
     410,   412,   415,   413,   409,   420,     0,   418,   417,   438,
     430,   433,   429,   431,     0,   405,   588,   586,   589,   591,
       0,   223,   580,   577,   578,   511,   579,   576,   509,   505,
     515,   513,   508,     0,     0,     0,     0,    37,     0,     0,
       0,     0,     0,   411,   414,   419,     0,     0,    99,     0,
     222,   107,     0,     0,     0,    39,    30,     0,    42,    36,
       0,   487,   544,   574,   572,   499,   575,   573,   570,   571,
     495,   503,   501,   497,     0,   424,     0,    96,     0,     0,
     533,   512,   537,   535,   585,   583,   516,   518,   584,   581,
     582,   510,   522,   520,   514,    71,     0,     0,    41,     0,
       0,   427,    98,   100,     0,    93,     0,   109,   111,   113,
       0,     0,     0,     0,   145,   118,   121,   125,   128,   132,
     135,   139,   106,     0,     0,     0,     0,     0,   223,    46,
       0,    51,     0,    43,    45,    48,     0,    38,   500,   502,
     147,   243,   245,   246,    28,   108,     0,   225,   225,   225,
     225,     0,   244,   247,   248,   249,   227,     0,     0,   255,
     258,   256,   259,   257,   253,   254,   158,     0,   156,   157,
     250,   251,   252,     0,     0,   154,   143,     0,     0,     0,
       0,     0,     0,     0,     0,   538,   534,   542,   540,   536,
     517,   523,   519,   527,   525,   521,    74,   269,   267,   268,
     263,   264,   270,   271,   265,   266,   260,     0,   232,   233,
     261,   262,    56,     0,     0,     0,    40,     0,   224,    25,
      26,    24,    22,    23,     0,     0,   230,   114,     0,   155,
       0,     0,   153,     0,     0,   119,   122,   126,   129,   133,
     136,   140,     0,     0,     0,     0,     0,     0,   234,    54,
       0,     0,    44,    47,    27,    30,    94,    95,   110,   226,
     229,   112,   228,   227,     0,   168,   169,   173,   101,   161,
     116,   160,     0,   171,   172,   142,   103,   117,   225,   148,
       0,   146,     0,     0,     0,     0,     0,     0,     0,   539,
     541,   528,   524,   532,   530,   526,     0,    72,     0,    77,
       0,    73,    49,     0,   171,    52,    50,     0,   115,     0,
     167,   165,   178,     0,   102,   245,   246,   149,   150,     0,
     244,   247,   248,   249,   144,   120,   123,   127,   130,   134,
     137,   141,     0,     0,    79,     0,     0,     0,   238,     0,
       0,    59,   231,     0,   176,     0,   225,   163,   164,   159,
     162,   170,     0,     0,     0,     0,     0,   529,   531,     0,
      82,    85,    78,     0,   236,   235,     0,    57,    53,   174,
       0,   184,   185,   181,   188,   189,     0,     0,     0,     0,
       0,     0,     0,   199,   201,   200,     0,     0,   204,   205,
     206,   211,     0,     0,   186,   180,   166,   151,   152,   124,
     131,   138,    81,    75,   225,   171,     0,    76,     0,    55,
       0,   175,   182,   183,   190,   191,   207,   195,   197,   202,
     208,     0,   193,   177,   179,   213,    80,     0,    86,   237,
       0,     0,     0,     0,     0,    61,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   217,     0,   212,   187,     0,
      62,    63,    67,    65,    58,    60,   215,     0,     0,     0,
       0,     0,   218,     0,     0,   210,   216,     0,     0,     0,
       0,     0,   192,   214,   196,   198,   203,   209,   219,   220,
     221,   194,    83,     0,     0,     0,     0,    64,    68,    66,
      84
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    37,    38,    39,   108,   106,   103,   104,   105,   555,
     381,   382,   383,   384,   385,   470,   386,   507,   469,   506,
     624,   543,   544,   545,   622,   751,   660,   714,   778,   820,
     844,   845,   879,   881,   880,    40,    49,   445,   538,   656,
     711,   746,   813,   772,   815,   896,   816,   401,   361,   107,
      42,   110,   371,   668,   431,   457,   458,   679,   687,    43,
     489,   532,   566,   567,   568,   674,   588,   692,   589,   693,
     764,   590,   694,   591,   695,   765,   592,   696,   593,   697,
     766,   594,   698,   643,   587,   690,   727,   728,   584,   577,
     578,   680,   759,   681,   721,   682,   723,   683,   719,   753,
     756,   803,   804,   805,   835,   846,   856,   847,   848,   849,
     850,   858,   867,   854,   855,   461,   629,   634,   635,   673,
     684,   618,   713,   749,   818,   387,   560,   579,   619,   561,
     186,   187,   620,   189,   190,   191,   192,    44,    91,   102,
      50,   101,   299,   300,   301,   302,   303,   111,   112,    52,
      51,   277,   278,   283,   282,   259,   264,   267,   260,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   258,   424,
     312,   452,    48,   246,   235,   250,    89,    90,    94,    45,
      46,    47,   305,    67,   203,   309,   391,   448,   214,   310,
     311,   402,   450,   451,   480,   509,   510,   295,   372,   373,
     439,   463,   462,   464,   501,   535,   536,   537,   602,   654,
     655,   702,   742,   743,   491,   533,   534,   596,   652,   653,
     403,   449,   204,   307,   308,   205,   216,   404,   482,   441,
     503,   362,   427,   363,   364,   365,   366
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -731
static const short int yypact[] =
{
     619,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,   297,   699,    83,  -731,   135,    24,   -28,
    -731,   -45,    12,    32,    49,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   110,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   358,    70,  1054,
     329,   329,   404,   149,   267,   374,   -14,   180,   163,   163,
     163,    -8,  -731,  -731,  -731,   148,   163,  -731,  -731,  -731,
     211,   -44,  -731,   140,   146,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,   -69,  -731,  -731,  -731,  -731,   130,  -731,  -731,  -731,
    -731,   329,   -69,   279,   285,   287,   299,   186,   300,  -731,
    1054,   452,   404,  -731,  -731,  -731,  -731,  -731,   238,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,   208,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,   179,   163,
     163,   163,   163,   163,   163,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,   163,  -731,  -731,  -731,   116,
     196,  -731,  -731,  -731,  -731,  -731,   163,   163,  -731,  -731,
     207,   210,   226,   163,   163,   163,   163,   229,   193,   -69,
     -69,   230,   236,   274,   -69,  -731,  -731,   -69,   -69,   -69,
     -69,   -69,   -69,   -69,   -69,   -69,   -69,  2150,  2150,  -731,
    -731,  -731,  2150,  2150,   240,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,   329,
     404,   452,   452,   404,  -731,   247,   778,   -69,   -81,   -69,
     605,   -69,   193,   163,   163,   251,   252,   261,   265,   273,
     275,   283,   163,   288,  -731,  -731,   290,   291,  -731,  -731,
    -731,   292,   293,   294,   295,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,   298,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,   622,  -731,  -731,  -731,  -731,  -731,
    -731,   263,   652,   -69,  -731,  -731,  -731,  -731,  -731,  -731,
     778,   296,   239,   286,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   301,   302,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,   307,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,   163,  -731,  -731,  -731,  -731,  -731,
    1054,   309,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,    26,   778,   306,   310,  -731,   -69,   -69,
     668,   -69,   163,  -731,  -731,  -731,   314,    30,  -731,   308,
    -731,  -731,   677,   698,   -69,  -731,  -731,   778,  -731,  -731,
    1054,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,   315,  -731,  1054,  -731,   778,   449,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,   296,    17,    35,  -731,   329,
     -69,  -731,  -731,   296,   317,  -731,   847,  -731,  -731,  -731,
    1123,  1123,  1330,  1054,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  2123,   -69,   329,  2132,   -69,   309,  -731,
    2081,  -731,  1054,  -731,   305,  -731,  1054,  -731,  -731,  -731,
    -731,   318,   160,   172,   -46,  -731,   319,   134,    27,   112,
     324,   330,   206,   212,   249,   258,  -731,  2081,   338,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   916,  -731,  -731,
    -731,  -731,  -731,   985,  1192,  -731,  -731,  2081,   339,   341,
     343,   344,   345,   349,   347,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   337,   334,  -731,
    -731,  -731,   350,   340,   352,   -17,  -731,   847,  -731,  -731,
    -731,  -731,  -731,  -731,  1606,  1537,  -731,  -731,  1468,  -731,
    1468,  1675,  -731,   353,  1537,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,   329,   -69,  2141,   -69,    21,  2081,  -731,  -731,
     354,  1054,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  1468,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  1675,  -731,  -731,  -731,  -731,  -731,   324,  1813,
     348,  -731,   -69,   357,   -69,   362,   -69,   365,  1468,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,   363,  -731,  2081,  -731,
    1330,  -731,  -731,  2081,  -731,  -731,  -731,  1947,  -731,  2081,
    -731,  -731,   364,  1399,  -731,  -731,  -731,   360,  -731,   369,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,   329,   -69,  -731,   366,   -54,  1261,   334,   156,
    1880,   370,  -731,   169,  -731,  2202,   324,  -731,  -731,  -731,
    -731,  -731,  1813,  1054,   -69,   -69,   -69,  -731,  -731,  1744,
    -731,  -731,  -731,  1675,  -731,  -731,  2014,  -731,  -731,  -731,
    2081,  -731,  -731,  -731,  -731,  -731,   356,   359,   342,   378,
     346,   379,   382,  -731,   385,  -731,   386,   388,  -731,  -731,
    -731,  -731,   389,  2180,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,   324,  -731,   -69,  -731,  2081,  -731,
     107,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,   235,  -731,  -731,  -731,   351,  -731,  1880,  -731,   334,
     355,   390,   391,   392,   191,  -731,   372,   372,   372,   372,
     510,  1054,  1054,  1054,   -60,  -731,   372,  -731,  -731,  2014,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   -72,   -70,   -65,
     -33,   393,   397,   397,   397,  -731,  -731,     1,   394,   372,
     517,   372,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,     9,   400,    25,   -69,  -731,  -731,  -731,
    -731
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -100,
    -370,  -731,  -731,  -505,  -489,  -731,  -487,  -731,  -731,  -731,
    -731,  -731,  -731,   -96,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -311,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,   -90,     0,  -731,
    -731,  -731,  -731,  -442,  -731,  -731,    50,  -620,  -236,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -224,  -171,    22,
    -508,  -606,  -731,  -181,  -731,  -730,  -676,  -449,  -731,  -731,
    -731,  -731,  -731,  -259,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -571,  -731,  -309,    10,  -543,  -109,   -16,  -731,
    -306,  -683,  -731,  -731,  -731,   -40,  -658,  -503,  -731,  -731,
    -513,  -472,   -43,  -731,  -502,  -475,  -507,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,   -24,   552,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,   -49,  -731,  -731,  -731,   -75,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,    -2,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,  -731,
    -368,  -731,   -47,  -731,  -731,  -731,   -98,  -430,  -525,  -731,
    -731,   -41,  -731,   209,  -731,   217,  -251
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -478
static const short int yytable[] =
{
      41,   281,   206,   562,   440,   215,   188,   343,   598,   185,
     443,   557,   286,   297,   564,   630,   631,   632,   539,   585,
     776,   686,   706,   236,   237,   238,   239,   558,   240,   559,
     748,   729,   493,   621,   685,   251,   252,   298,   750,   389,
      96,   565,   257,   279,   262,   582,   582,   582,   580,   580,
     580,   882,   285,   884,   253,   279,    85,    86,   885,   241,
     621,   406,   720,   875,   296,   215,   770,   188,   718,   639,
     293,   242,   243,   244,   466,   639,   254,   771,  -273,    97,
     621,   642,   481,   540,   280,   541,   263,  -285,  -285,  -285,
     886,   883,   741,   883,   492,   502,   280,   505,   883,   851,
     852,   853,   582,   758,   729,   580,   604,   859,   582,   582,
     109,   580,   580,   542,   562,    85,    86,   540,   513,   541,
     707,   708,   557,   709,   891,   564,   710,   621,   621,   704,
     883,   621,   897,   621,   621,   839,    98,   621,   558,   837,
     559,   255,   248,   249,   245,   724,   556,   542,   899,   465,
     621,   628,   565,   686,    87,    88,    99,   322,   486,   444,
     -34,   -34,   -34,   546,   883,   597,   487,   621,   603,   344,
     345,   547,   883,   100,   349,   621,   730,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   732,   883,   248,
     249,   621,   688,   671,   119,   315,   316,   317,   318,   319,
     320,   621,   691,   297,   297,   247,   621,   585,    92,    93,
     621,   321,   621,   806,   733,   323,   621,   388,   390,   392,
     217,   405,   326,   327,   256,   375,   313,   314,   378,   331,
     332,   333,   334,   722,   617,   582,   628,   367,   580,   324,
     325,   368,   369,   621,   642,   -33,   -33,   -33,   261,   730,
     374,   840,   841,   215,   296,   296,   215,   665,   628,   842,
     732,   636,   621,   188,   265,   843,   621,   -32,   -32,   621,
     266,   836,   582,   621,   761,   580,   868,   869,   870,   284,
     218,   636,   287,   442,   774,   877,   703,   733,   288,   407,
     408,   289,   775,  -286,  -286,  -286,   779,   780,   416,   376,
     377,   761,   290,   292,    53,  -287,  -287,  -287,   893,   291,
     895,   621,   219,   336,   220,   221,   222,   223,   864,   337,
     338,   339,    54,   306,   688,    55,   304,   819,   669,   672,
     621,   328,   224,    56,   329,   840,   841,   188,   672,  -284,
    -284,  -284,    57,   842,    58,  -240,  -240,  -240,    59,   843,
     330,   712,   621,   335,   346,   340,   341,   342,   471,   472,
     347,   483,   348,    60,   370,    61,    62,    63,    64,   113,
     114,   379,    65,   446,   504,   409,   410,   115,   116,   117,
     118,    66,  -241,  -241,  -241,   411,   225,   188,   761,   412,
     459,  -242,  -242,  -242,   851,   852,   853,   413,   430,   414,
     456,   188,   745,   226,   227,   228,   229,   415,   230,   231,
     878,   669,   417,   754,   418,   419,   420,   421,   422,   423,
     549,   447,   425,   460,   188,   453,   454,   188,   484,   444,
     508,   455,   468,   625,   232,   233,   234,   467,   485,   511,
     488,   550,  -272,   188,   599,   188,   459,   605,   628,   193,
     514,   194,   627,   195,   633,   196,   197,   198,   637,   645,
     548,   646,   649,   814,   647,   648,   650,   651,   657,   658,
     659,   661,   734,   563,   821,   662,   736,   581,   581,   581,
     188,   738,   689,   586,   740,   715,   600,   744,   762,   763,
     755,   199,   200,   201,   202,   822,   777,   769,   823,   188,
     825,   827,   623,   188,   828,   824,   626,   829,   830,   826,
     831,   832,   861,   862,   863,   871,   887,   857,   860,    35,
     576,   576,   894,   898,   207,   892,   194,   664,   208,   663,
     209,   210,   211,   865,   581,   866,   512,   817,   807,   747,
     581,   581,   760,   583,   834,   876,   515,   516,   606,   517,
     518,   519,   520,   521,   522,   523,   524,   525,   526,   527,
     528,   529,   530,   700,   717,   705,   199,   212,   213,   202,
      95,   644,   207,   428,   194,     0,   208,   576,   294,   210,
     211,   429,     0,   576,   563,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   735,   699,   737,   531,   739,     0,     0,     0,
       0,     0,     0,     0,   199,   212,   213,   202,   188,    -3,
       1,   716,     0,     0,     0,     2,     3,     4,     5,     0,
    -477,  -477,     6,     7,     8,     9,    10,    11,  -477,  -477,
    -477,  -477,    12,    13,    14,     0,   731,    15,     0,     0,
       0,    16,     0,   768,     0,    17,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   581,     0,     0,
      18,     0,     0,     0,   809,   810,   811,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    19,     0,     0,     0,
       0,     0,     0,   767,     0,     0,    20,    21,    22,    23,
      24,    25,    26,     0,   581,    68,     0,    69,    27,    28,
      29,    30,    31,    32,    33,    34,     0,     0,     0,   731,
     188,     0,    70,   808,     0,   393,   838,   394,     0,   279,
       0,   395,   396,   397,     0,     0,     0,     0,     0,     0,
       0,    35,   336,     0,    35,     0,   426,     0,   337,   338,
     339,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,     0,     0,     0,   398,   399,   400,
     280,     0,   432,     0,   394,     0,   279,     0,   433,   434,
     435,    36,     0,    84,   340,   341,   342,   360,   473,     0,
     394,     0,   279,     0,   474,   475,   476,   393,     0,   394,
       0,   279,     0,   395,   396,   397,   900,     0,   188,   188,
     188,   872,   873,   874,   436,   437,   438,   280,   494,     0,
     394,     0,   279,     0,   495,   496,   497,     0,     0,     0,
     477,   478,   479,   280,     0,     0,     0,     0,     0,   398,
     399,   490,   280,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     498,   499,   500,   280,     0,     0,     0,     0,     0,     0,
       0,     0,   888,   889,   890,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     380,     0,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
       0,   183,     0,   184,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   380,
       0,   551,   146,   147,   148,   552,   150,   151,   553,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
     183,     0,   554,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   569,   570,   571,   572,   638,   573,    35,     0,
     574,   575,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,     0,   183,
       0,   184,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   569,   570,   571,   572,   640,   573,    35,     0,   574,
     575,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,   183,     0,
     184,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,     0,     0,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,     0,   183,     0,   184,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   569,
     570,   571,   572,     0,   573,    35,     0,   574,   575,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,   183,     0,   184,   120,
     121,   122,   123,   124,   125,   126,   127,   128,   129,   130,
     131,   132,   133,   134,   135,   136,   137,   138,   569,   570,
     571,   572,   641,   573,     0,     0,   574,   575,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   162,   163,   164,   165,   166,   167,   168,
     169,   170,   171,   172,   173,   174,   175,   176,   177,   178,
     179,   180,   181,   182,     0,   183,     0,   184,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   569,   570,   571,
     572,   773,   573,     0,     0,   574,   575,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,     0,   183,     0,   184,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   569,   570,   571,   572,
       0,   573,     0,     0,   574,   575,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,     0,   183,     0,   184,   120,   121,   122,   123,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   607,     0,   675,   608,   676,   609,     0,
     610,     0,     0,   757,     0,   677,     0,     0,   611,   612,
     613,     0,   614,   615,     0,     0,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,     0,   616,     0,   678,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   607,     0,   675,   608,   676,   609,     0,   610,
       0,     0,     0,     0,   677,     0,     0,   611,   612,   613,
       0,   614,   615,     0,     0,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
       0,   616,     0,   678,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   607,     0,     0,   608,     0,   609,     0,   610,     0,
       0,   666,   670,     0,     0,     0,   611,   612,   613,     0,
     614,   615,     0,     0,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
     616,     0,   667,   120,   121,   122,   123,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     607,     0,     0,   608,     0,   609,     0,   610,     0,     0,
     666,     0,     0,     0,     0,   611,   612,   613,     0,   614,
     615,     0,     0,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,     0,   616,
       0,   667,   120,   121,   122,   123,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   607,
       0,     0,   608,     0,   609,     0,   610,     0,     0,     0,
       0,   677,     0,     0,   611,   612,   613,     0,   614,   615,
       0,     0,   158,   159,   160,   161,   162,   163,   164,   165,
     166,   167,   168,   169,   170,   171,   172,   173,   174,   175,
     176,   177,   178,   179,   180,   181,   182,     0,   616,     0,
     678,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   607,     0,
       0,   608,     0,   609,     0,   610,     0,     0,     0,     0,
       0,     0,     0,   611,   612,   613,     0,   614,   615,     0,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,     0,   616,     0,   812,
     120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,     0,     0,     0,   146,   147,
     148,   725,   150,   151,   726,   153,   154,   155,   156,   157,
     158,   159,   160,   161,   162,   163,   164,   165,   166,   167,
     168,   169,   170,   171,   172,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,     0,   183,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   607,     0,   675,   608,   676,   609,
       0,   610,     0,     0,     0,     0,   677,     0,     0,   611,
     612,   613,     0,   614,   615,     0,     0,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,     0,   616,   120,   121,   122,   123,   124,   125,
     126,   127,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   607,     0,     0,   608,     0,   609,     0,   610,     0,
       0,     0,   752,     0,     0,     0,   611,   612,   613,     0,
     614,   615,     0,     0,   158,   159,   160,   161,   162,   163,
     164,   165,   166,   167,   168,   169,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,   180,   181,   182,     0,
     616,   120,   121,   122,   123,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   607,     0,
       0,   608,     0,   609,     0,   610,     0,     0,     0,     0,
     677,     0,     0,   611,   612,   613,     0,   614,   615,     0,
       0,   158,   159,   160,   161,   162,   163,   164,   165,   166,
     167,   168,   169,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,     0,   616,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   607,     0,     0,   608,     0,
     609,     0,   610,     0,     0,     0,     0,     0,     0,     0,
     611,   612,   613,     0,   614,   615,     0,     0,   158,   159,
     160,   161,   162,   163,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   473,   616,   394,     0,   279,     0,   474,
     595,   476,   393,     0,   394,     0,   279,     0,   395,   396,
     397,   473,     0,   394,     0,   279,     0,   474,   701,   476,
     336,     0,    35,     0,     0,     0,   337,   338,   339,     0,
       0,     0,     0,     0,     0,   477,   478,   479,   280,     0,
       0,     0,     0,     0,   398,   399,   601,   280,     0,     0,
       0,     0,     0,   477,   478,   479,   280,   833,     0,     0,
       0,     0,   340,   341,   342,   360,     0,   781,   782,   783,
     784,   785,   786,   787,   788,   789,   790,   791,   792,   793,
     794,   795,   796,   797,   798,   799,   800,   801,   802,   781,
     782,   783,   784,   785,   786,   787,   788,   789,   790,   791,
     792,   793,   794,   795,   796,   797,   798,   799,   800,   801,
     802
};

static const short int yycheck[] =
{
       0,    91,    51,   516,   372,    52,    49,   258,   533,    49,
     380,   516,   102,   111,   516,   558,   559,   560,     1,   522,
     750,   641,     1,    37,    38,    39,    40,   516,    42,   516,
     713,   689,   462,   540,   640,    59,    60,   112,   714,   120,
      68,   516,    66,   124,    88,   520,   521,   522,   520,   521,
     522,   123,   101,   123,    62,   124,    32,    33,   123,    73,
     567,   312,   682,   123,   111,   112,   120,   110,   674,   577,
     110,    85,    86,    87,   444,   583,    84,   131,   124,   124,
     587,   584,   450,   100,   165,   102,   130,   133,   134,   135,
     123,   163,   698,   163,   462,   463,   165,   467,   163,   159,
     160,   161,   577,   723,   762,   577,   536,   837,   583,   584,
       0,   583,   584,   130,   627,    32,    33,   100,   488,   102,
      99,   100,   627,   102,   123,   627,   105,   634,   635,   654,
     163,   638,   123,   640,   641,   818,   124,   644,   627,   815,
     627,   149,    26,    27,   158,   688,   516,   130,   123,   123,
     657,   124,   627,   773,    71,    72,   124,    41,   128,   133,
     133,   134,   135,   128,   163,   533,   136,   674,   536,   259,
     260,   136,   163,   124,   264,   682,   689,   267,   268,   269,
     270,   271,   272,   273,   274,   275,   276,   689,   163,    26,
      27,   698,   641,   635,   124,   219,   220,   221,   222,   223,
     224,   708,   644,   301,   302,    25,   713,   710,    73,    74,
     717,   235,   719,   756,   689,   239,   723,   307,   308,   309,
      71,   311,   246,   247,    76,   300,    47,    48,   303,   253,
     254,   255,   256,   682,   540,   710,   124,   278,   710,    43,
      44,   282,   283,   750,   747,   133,   134,   135,    37,   762,
     299,   144,   145,   300,   301,   302,   303,   627,   124,   152,
     762,   567,   769,   306,   124,   158,   773,   133,   134,   776,
     124,   814,   747,   780,   723,   747,   847,   848,   849,   149,
      13,   587,     3,   373,   128,   856,   654,   762,     3,   313,
     314,     4,   136,   133,   134,   135,   127,   128,   322,   301,
     302,   750,     3,     3,     7,   133,   134,   135,   879,   123,
     881,   818,    45,   120,    47,    48,    49,    50,   127,   126,
     127,   128,    25,   115,   773,    28,    88,   776,   634,   635,
     837,   124,    65,    36,   124,   144,   145,   380,   644,   133,
     134,   135,    45,   152,    47,   133,   134,   135,    51,   158,
     124,   657,   859,   124,   124,   162,   163,   164,   448,   449,
     124,   451,    88,    66,   124,    68,    69,    70,    71,    11,
      12,   124,    75,   134,   464,   124,   124,    19,    20,    21,
      22,    84,   133,   134,   135,   124,    12,   430,   837,   124,
     430,   133,   134,   135,   159,   160,   161,   124,   135,   124,
     424,   444,   708,    29,    30,    31,    32,   124,    34,    35,
     859,   717,   124,   719,   124,   124,   124,   124,   124,   124,
     510,   135,   124,   114,   467,   124,   124,   470,   452,   133,
     470,   124,   122,   128,    60,    61,    62,   131,   124,   124,
     132,   124,   124,   486,   534,   488,   486,   537,   124,   120,
       1,   122,   133,   124,   124,   126,   127,   128,   120,   120,
     509,   120,   117,   769,   121,   121,   117,   120,   131,   135,
     120,   131,   124,   516,   780,   123,   119,   520,   521,   522,
     523,   119,   129,   523,   119,   131,   535,   124,   128,   120,
     126,   162,   163,   164,   165,   139,   126,   131,   139,   542,
     122,   122,   542,   546,   122,   163,   546,   122,   122,   163,
     122,   122,   122,   122,   122,     5,   123,   166,   163,   122,
     520,   521,     5,   123,   120,   131,   122,   627,   124,   625,
     126,   127,   128,   844,   577,   163,   486,   773,   762,   710,
     583,   584,   723,   521,   803,   854,    97,    98,   538,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   653,   673,   655,   162,   163,   164,   165,
      18,   587,   120,   364,   122,    -1,   124,   577,   126,   127,
     128,   364,    -1,   583,   627,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   692,   652,   694,   156,   696,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,   661,     0,
       1,   661,    -1,    -1,    -1,     6,     7,     8,     9,    -1,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,   689,    28,    -1,    -1,
      -1,    32,    -1,   743,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   710,    -1,    -1,
      51,    -1,    -1,    -1,   764,   765,   766,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    67,    -1,    -1,    -1,
      -1,    -1,    -1,   742,    -1,    -1,    77,    78,    79,    80,
      81,    82,    83,    -1,   747,     6,    -1,     8,    89,    90,
      91,    92,    93,    94,    95,    96,    -1,    -1,    -1,   762,
     763,    -1,    23,   763,    -1,   120,   816,   122,    -1,   124,
      -1,   126,   127,   128,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,   122,   120,    -1,   122,    -1,   124,    -1,   126,   127,
     128,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    -1,    -1,    -1,   162,   163,   164,
     165,    -1,   120,    -1,   122,    -1,   124,    -1,   126,   127,
     128,   162,    -1,    84,   162,   163,   164,   165,   120,    -1,
     122,    -1,   124,    -1,   126,   127,   128,   120,    -1,   122,
      -1,   124,    -1,   126,   127,   128,   896,    -1,   851,   852,
     853,   851,   852,   853,   162,   163,   164,   165,   120,    -1,
     122,    -1,   124,    -1,   126,   127,   128,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,   162,
     163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     162,   163,   164,   165,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   872,   873,   874,    97,    98,    99,   100,   101,
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
     114,   115,   116,   117,   118,   119,   120,   121,   122,    -1,
     124,   125,   126,   127,   128,   129,   130,   131,   132,   133,
     134,   135,   136,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,   163,
      -1,   165,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,    -1,   124,
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
     117,   118,   119,    -1,   121,   122,    -1,   124,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,    -1,   165,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   111,   112,   113,   114,   115,   116,   117,
     118,   119,   120,   121,    -1,    -1,   124,   125,   126,   127,
     128,   129,   130,   131,   132,   133,   134,   135,   136,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158,   159,   160,   161,    -1,   163,    -1,   165,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,   115,   116,   117,   118,
     119,   120,   121,    -1,    -1,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,    -1,   163,    -1,   165,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,   115,   116,   117,   118,   119,
      -1,   121,    -1,    -1,   124,   125,   126,   127,   128,   129,
     130,   131,   132,   133,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    -1,   165,    97,    98,    99,   100,
     101,   102,   103,   104,   105,   106,   107,   108,   109,   110,
     111,   112,   113,   114,    -1,   116,   117,   118,   119,    -1,
     121,    -1,    -1,   124,    -1,   126,    -1,    -1,   129,   130,
     131,    -1,   133,   134,    -1,    -1,   137,   138,   139,   140,
     141,   142,   143,   144,   145,   146,   147,   148,   149,   150,
     151,   152,   153,   154,   155,   156,   157,   158,   159,   160,
     161,    -1,   163,    -1,   165,    97,    98,    99,   100,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   111,
     112,   113,   114,    -1,   116,   117,   118,   119,    -1,   121,
      -1,    -1,    -1,    -1,   126,    -1,    -1,   129,   130,   131,
      -1,   133,   134,    -1,    -1,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
      -1,   163,    -1,   165,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,    -1,   121,    -1,
      -1,   124,   125,    -1,    -1,    -1,   129,   130,   131,    -1,
     133,   134,    -1,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,    -1,   165,    97,    98,    99,   100,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,   113,
     114,    -1,    -1,   117,    -1,   119,    -1,   121,    -1,    -1,
     124,    -1,    -1,    -1,    -1,   129,   130,   131,    -1,   133,
     134,    -1,    -1,   137,   138,   139,   140,   141,   142,   143,
     144,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,    -1,   163,
      -1,   165,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
      -1,    -1,   117,    -1,   119,    -1,   121,    -1,    -1,    -1,
      -1,   126,    -1,    -1,   129,   130,   131,    -1,   133,   134,
      -1,    -1,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,    -1,   163,    -1,
     165,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   129,   130,   131,    -1,   133,   134,    -1,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    -1,   165,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,    -1,    -1,    -1,   125,   126,
     127,   128,   129,   130,   131,   132,   133,   134,   135,   136,
     137,   138,   139,   140,   141,   142,   143,   144,   145,   146,
     147,   148,   149,   150,   151,   152,   153,   154,   155,   156,
     157,   158,   159,   160,   161,    -1,   163,    97,    98,    99,
     100,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,   111,   112,   113,   114,    -1,   116,   117,   118,   119,
      -1,   121,    -1,    -1,    -1,    -1,   126,    -1,    -1,   129,
     130,   131,    -1,   133,   134,    -1,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   159,
     160,   161,    -1,   163,    97,    98,    99,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,    -1,    -1,   117,    -1,   119,    -1,   121,    -1,
      -1,    -1,   125,    -1,    -1,    -1,   129,   130,   131,    -1,
     133,   134,    -1,    -1,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,    -1,
     163,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,   107,   108,   109,   110,   111,   112,   113,   114,    -1,
      -1,   117,    -1,   119,    -1,   121,    -1,    -1,    -1,    -1,
     126,    -1,    -1,   129,   130,   131,    -1,   133,   134,    -1,
      -1,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   159,   160,   161,    -1,   163,    97,    98,
      99,   100,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   111,   112,   113,   114,    -1,    -1,   117,    -1,
     119,    -1,   121,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     129,   130,   131,    -1,   133,   134,    -1,    -1,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   120,   163,   122,    -1,   124,    -1,   126,
     127,   128,   120,    -1,   122,    -1,   124,    -1,   126,   127,
     128,   120,    -1,   122,    -1,   124,    -1,   126,   127,   128,
     120,    -1,   122,    -1,    -1,    -1,   126,   127,   128,    -1,
      -1,    -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,
      -1,    -1,    -1,    -1,   162,   163,   164,   165,    -1,    -1,
      -1,    -1,    -1,   162,   163,   164,   165,   127,    -1,    -1,
      -1,    -1,   162,   163,   164,   165,    -1,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   149,
     150,   151,   152,   153,   154,   155,   156,   157,   158,   137,
     138,   139,   140,   141,   142,   143,   144,   145,   146,   147,
     148,   149,   150,   151,   152,   153,   154,   155,   156,   157,
     158
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned short int yystos[] =
{
       0,     1,     6,     7,     8,     9,    13,    14,    15,    16,
      17,    18,    23,    24,    25,    28,    32,    36,    51,    67,
      77,    78,    79,    80,    81,    82,    83,    89,    90,    91,
      92,    93,    94,    95,    96,   122,   162,   168,   169,   170,
     202,   215,   217,   226,   304,   346,   347,   348,   339,   203,
     307,   317,   316,     7,    25,    28,    36,    45,    47,    51,
      66,    68,    69,    70,    71,    75,    84,   350,     6,     8,
      23,    52,    53,    54,    55,    56,    57,    58,    59,    60,
      61,    62,    63,    64,    84,    32,    33,    71,    72,   343,
     344,   305,    73,    74,   345,   343,    68,   124,   124,   124,
     124,   308,   306,   173,   174,   175,   172,   216,   171,     0,
     218,   314,   315,    11,    12,    19,    20,    21,    22,   124,
      97,    98,    99,   100,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,   111,   112,   113,   114,   115,   116,
     117,   118,   119,   120,   121,   124,   125,   126,   127,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   155,   156,   157,   158,
     159,   160,   161,   163,   165,   292,   297,   298,   299,   300,
     301,   302,   303,   120,   122,   124,   126,   127,   128,   162,
     163,   164,   165,   351,   389,   392,   351,   120,   124,   126,
     127,   128,   163,   164,   355,   389,   393,    71,    13,    45,
      47,    48,    49,    50,    65,    12,    29,    30,    31,    32,
      34,    35,    60,    61,    62,   341,    37,    38,    39,    40,
      42,    73,    85,    86,    87,   158,   340,    25,    26,    27,
     342,   342,   342,    62,    84,   149,    76,   342,   335,   322,
     325,    37,    88,   130,   323,   124,   124,   324,   326,   327,
     328,   329,   330,   331,   332,   333,   334,   318,   319,   124,
     165,   214,   321,   320,   149,   351,   214,     3,     3,     4,
       3,   123,     3,   292,   126,   364,   389,   393,   355,   309,
     310,   311,   312,   313,    88,   349,   115,   390,   391,   352,
     356,   357,   337,    47,    48,   342,   342,   342,   342,   342,
     342,   342,    41,   342,    43,    44,   342,   342,   124,   124,
     124,   342,   342,   342,   342,   124,   120,   126,   127,   128,
     162,   163,   164,   403,   214,   214,   124,   124,    88,   214,
     214,   214,   214,   214,   214,   214,   214,   214,   214,   214,
     165,   215,   398,   400,   401,   402,   403,   398,   398,   398,
     124,   219,   365,   366,   351,   355,   364,   364,   355,   124,
     122,   177,   178,   179,   180,   181,   183,   292,   214,   120,
     214,   353,   214,   120,   122,   126,   127,   128,   162,   163,
     164,   214,   358,   387,   394,   214,   403,   342,   342,   124,
     124,   124,   124,   124,   124,   124,   342,   124,   124,   124,
     124,   124,   124,   124,   336,   124,   124,   399,   400,   402,
     135,   221,   120,   126,   127,   128,   162,   163,   164,   367,
     387,   396,   214,   177,   133,   204,   134,   135,   354,   388,
     359,   360,   338,   124,   124,   124,   342,   222,   223,   292,
     114,   282,   369,   368,   370,   123,   177,   131,   122,   185,
     182,   214,   214,   120,   126,   127,   128,   162,   163,   164,
     361,   387,   395,   214,   342,   124,   128,   136,   132,   227,
     164,   381,   387,   394,   120,   126,   127,   128,   162,   163,
     164,   371,   387,   397,   214,   177,   186,   184,   292,   362,
     363,   124,   223,   177,     1,    97,    98,   100,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   156,   228,   382,   383,   372,   373,   374,   205,     1,
     100,   102,   130,   188,   189,   190,   128,   136,   351,   214,
     124,   124,   128,   131,   165,   176,   177,   180,   181,   183,
     293,   296,   297,   299,   301,   302,   229,   230,   231,   116,
     117,   118,   119,   121,   124,   125,   215,   256,   257,   294,
     298,   299,   302,   256,   255,   294,   292,   251,   233,   235,
     238,   240,   243,   245,   248,   127,   384,   387,   395,   214,
     351,   164,   375,   387,   394,   214,   282,   114,   117,   119,
     121,   129,   130,   131,   133,   134,   163,   287,   288,   295,
     299,   303,   191,   292,   187,   128,   292,   133,   124,   283,
     283,   283,   283,   124,   284,   285,   287,   120,   120,   257,
     120,   120,   294,   250,   285,   120,   120,   121,   121,   117,
     117,   120,   385,   386,   376,   377,   206,   131,   135,   120,
     193,   131,   123,   190,   176,   177,   124,   165,   220,   287,
     125,   220,   287,   286,   232,   116,   118,   126,   165,   224,
     258,   260,   262,   264,   287,   258,   224,   225,   264,   129,
     252,   220,   234,   236,   239,   241,   244,   246,   249,   351,
     214,   127,   378,   387,   395,   214,     1,    99,   100,   102,
     105,   207,   287,   289,   194,   131,   292,   284,   258,   265,
     224,   261,   264,   263,   283,   128,   131,   253,   254,   293,
     297,   299,   301,   302,   124,   214,   119,   214,   119,   214,
     119,   258,   379,   380,   124,   287,   208,   255,   288,   290,
     263,   192,   125,   266,   287,   126,   267,   124,   224,   259,
     260,   264,   128,   120,   237,   242,   247,   351,   214,   131,
     120,   131,   210,   120,   128,   136,   262,   126,   195,   127,
     128,   137,   138,   139,   140,   141,   142,   143,   144,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   155,
     156,   157,   158,   268,   269,   270,   283,   254,   292,   214,
     214,   214,   165,   209,   287,   211,   213,   225,   291,   264,
     196,   287,   139,   139,   163,   122,   163,   122,   122,   122,
     122,   122,   122,   127,   270,   271,   283,   263,   214,   288,
     144,   145,   152,   158,   197,   198,   272,   274,   275,   276,
     277,   159,   160,   161,   280,   281,   273,   166,   278,   262,
     163,   122,   122,   122,   127,   198,   163,   279,   279,   279,
     279,     5,   292,   292,   292,   123,   281,   279,   264,   199,
     201,   200,   123,   163,   123,   123,   123,   123,   215,   215,
     215,   123,   131,   279,     5,   279,   212,   123,   123,   123,
     214
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
#line 232 "surface.yy"
    { YYACCEPT; ;}
    break;

  case 3:
#line 234 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			;}
    break;

  case 8:
#line 248 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 9:
#line 250 "surface.yy"
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, false, lineNr);
			;}
    break;

  case 10:
#line 258 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 11:
#line 260 "surface.yy"
    {
			  int lineNr = lineNumber;
			  //eatComment(false); // eat \n so that line number is correct
			  string directory;
			  string fileName;
			  if (findFile((yyvsp[0].yyString), directory, fileName, lineNr))
			    includeFile(directory, fileName, true, lineNr);
			;}
    break;

  case 12:
#line 269 "surface.yy"
    {
			  cout << directoryManager.getCwd() << '\n';
			;}
    break;

  case 13:
#line 272 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 14:
#line 274 "surface.yy"
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
#line 283 "surface.yy"
    { lexerFileNameMode(); ;}
    break;

  case 16:
#line 285 "surface.yy"
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
#line 305 "surface.yy"
    { lexerStringMode(); ;}
    break;

  case 19:
#line 307 "surface.yy"
    {
			  system((string("ls") + (yyvsp[0].yyString)).c_str());
			;}
    break;

  case 20:
#line 311 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::QUIT;
			  YYACCEPT;
			;}
    break;

  case 21:
#line 316 "surface.yy"
    {
			  if(!handleEof())
			    {
			      PARSE_RESULT = UserLevelRewritingContext::QUIT;
			    }
			;}
    break;

  case 22:
#line 349 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        ;}
    break;

  case 23:
#line 353 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[-1].yyToken)));
                        ;}
    break;

  case 27:
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
#line 368 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  moduleExpressions.push(new ModuleExpression(t));
			;}
    break;

  case 30:
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
#line 394 "surface.yy"
    {
                          moduleExpressions.push(new ModuleExpression((yyvsp[0].yyToken)));
                        ;}
    break;

  case 36:
#line 401 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, currentRenaming));
			  currentRenaming = 0;
			;}
    break;

  case 37:
#line 409 "surface.yy"
    { clear(); ;}
    break;

  case 38:
#line 411 "surface.yy"
    {
			  ModuleExpression* m = moduleExpressions.top();
			  moduleExpressions.pop();
			  moduleExpressions.push(new ModuleExpression(m, tokenSequence));
			;}
    break;

  case 39:
#line 418 "surface.yy"
    {;}
    break;

  case 40:
#line 421 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 41:
#line 422 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 42:
#line 429 "surface.yy"
    {
			  oldSyntaxContainer = currentSyntaxContainer;
			  currentSyntaxContainer = currentRenaming = new Renaming;
			;}
    break;

  case 43:
#line 434 "surface.yy"
    {
			  currentSyntaxContainer = oldSyntaxContainer;
			;}
    break;

  case 49:
#line 449 "surface.yy"
    {
			  currentRenaming->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			;}
    break;

  case 50:
#line 453 "surface.yy"
    {
			  currentRenaming->addLabelMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			;}
    break;

  case 51:
#line 456 "surface.yy"
    { lexBubble(BAR_COLON | BAR_TO, 1); ;}
    break;

  case 52:
#line 457 "surface.yy"
    { lexBubble(BAR_COMMA | BAR_LEFT_BRACKET | BAR_RIGHT_PAREN, 1); ;}
    break;

  case 53:
#line 458 "surface.yy"
    {;}
    break;

  case 54:
#line 463 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); ;}
    break;

  case 55:
#line 464 "surface.yy"
    {;}
    break;

  case 56:
#line 465 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpMapping(lexerBubble); ;}
    break;

  case 57:
#line 471 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); ;}
    break;

  case 58:
#line 472 "surface.yy"
    {;}
    break;

  case 59:
#line 473 "surface.yy"
    { Token::peelParens(lexerBubble); currentRenaming->addOpTarget(lexerBubble); ;}
    break;

  case 62:
#line 480 "surface.yy"
    { currentRenaming->setPrec((yyvsp[0].yyToken)); ;}
    break;

  case 63:
#line 481 "surface.yy"
    { clear(); ;}
    break;

  case 64:
#line 482 "surface.yy"
    { currentRenaming->setGather(tokenSequence); ;}
    break;

  case 65:
#line 483 "surface.yy"
    { clear(); ;}
    break;

  case 66:
#line 484 "surface.yy"
    { currentRenaming->setFormat(tokenSequence); ;}
    break;

  case 67:
#line 485 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 68:
#line 486 "surface.yy"
    { currentRenaming->setLatexMacro((yyvsp[-1].yyString)); ;}
    break;

  case 69:
#line 492 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 70:
#line 494 "surface.yy"
    {
			  fileTable.beginModule((yyvsp[-4].yyToken), (yyvsp[-2].yyToken));
			  interpreter.setCurrentView(new View((yyvsp[-2].yyToken)));
			  currentSyntaxContainer = CV;
			  CV->addFrom(moduleExpressions.top());
			  moduleExpressions.pop();
			;}
    break;

  case 71:
#line 502 "surface.yy"
    {
			  CV->addTo(moduleExpressions.top());
			  moduleExpressions.pop();
			;}
    break;

  case 72:
#line 507 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  interpreter.insertView(((yyvsp[-9].yyToken)).code(), CV);
			  CV->finishView();
			;}
    break;

  case 75:
#line 520 "surface.yy"
    {
			  CV->addSortMapping((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			;}
    break;

  case 76:
#line 523 "surface.yy"
    {;}
    break;

  case 77:
#line 524 "surface.yy"
    { lexBubble(BAR_COLON | BAR_TO, 1); ;}
    break;

  case 80:
#line 529 "surface.yy"
    { (yyval.yyToken) = (yyvsp[-1].yyToken); ;}
    break;

  case 81:
#line 531 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  (yyval.yyToken) = t;
			;}
    break;

  case 82:
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
#line 548 "surface.yy"
    {
			  lexBubble(END_STATEMENT, 1);
			;}
    break;

  case 84:
#line 552 "surface.yy"
    {
			  Token::peelParens(lexerBubble);  // remove any enclosing parens from op name
			  CV->addOpTarget(lexerBubble);
			;}
    break;

  case 85:
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
#line 589 "surface.yy"
    {;}
    break;

  case 88:
#line 591 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  lexerBubble.append(t);
			;}
    break;

  case 89:
#line 599 "surface.yy"
    { lexBubble(BAR_RIGHT_PAREN, 1); ;}
    break;

  case 90:
#line 600 "surface.yy"
    {;}
    break;

  case 91:
#line 606 "surface.yy"
    { lexerIdMode(); ;}
    break;

  case 92:
#line 608 "surface.yy"
    {
			  interpreter.setCurrentModule(new PreModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken)));
			  currentSyntaxContainer = CM;
			  fileTable.beginModule((yyvsp[-2].yyToken), (yyvsp[0].yyToken));
			;}
    break;

  case 93:
#line 614 "surface.yy"
    {
			  lexerInitialMode();
			  fileTable.endModule(lineNumber);
			  CM->finishModule((yyvsp[0].yyToken));
			;}
    break;

  case 94:
#line 621 "surface.yy"
    {;}
    break;

  case 95:
#line 623 "surface.yy"
    {
			  Token t;
			  t.dropChar((yyvsp[0].yyToken));
			  missingSpace(t);
			  store(t);
			;}
    break;

  case 96:
#line 631 "surface.yy"
    {;}
    break;

  case 100:
#line 640 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addParameter((yyvsp[-2].yyToken), me);
			;}
    break;

  case 101:
#line 648 "surface.yy"
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  currentSyntaxContainer->addType(false, singleton);
			  (yyval.yyToken) = (yyvsp[0].yyToken);  // needed for line number
			;}
    break;

  case 103:
#line 657 "surface.yy"
    {;}
    break;

  case 108:
#line 668 "surface.yy"
    {
			  ModuleExpression* me = moduleExpressions.top();
			  moduleExpressions.pop();
			  CM->addImport((yyvsp[-1].yyToken), me);
			;}
    break;

  case 109:
#line 674 "surface.yy"
    { clear(); ;}
    break;

  case 110:
#line 675 "surface.yy"
    { CM->addSortDecl(tokenSequence); ;}
    break;

  case 111:
#line 677 "surface.yy"
    { clear(); ;}
    break;

  case 112:
#line 678 "surface.yy"
    { CM->addSubsortDecl(tokenSequence); ;}
    break;

  case 113:
#line 680 "surface.yy"
    { lexBubble(BAR_COLON, 1); ;}
    break;

  case 114:
#line 681 "surface.yy"
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); ;}
    break;

  case 115:
#line 682 "surface.yy"
    {;}
    break;

  case 116:
#line 684 "surface.yy"
    {;}
    break;

  case 117:
#line 686 "surface.yy"
    {;}
    break;

  case 118:
#line 688 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1); ;}
    break;

  case 119:
#line 689 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); ;}
    break;

  case 120:
#line 690 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 121:
#line 692 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), BAR_COLON, 1);  ;}
    break;

  case 122:
#line 693 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); ;}
    break;

  case 123:
#line 694 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); ;}
    break;

  case 124:
#line 695 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 125:
#line 697 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); ;}
    break;

  case 126:
#line 698 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); ;}
    break;

  case 127:
#line 699 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 128:
#line 701 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), BAR_EQUALS, 1); ;}
    break;

  case 129:
#line 702 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); ;}
    break;

  case 130:
#line 703 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); ;}
    break;

  case 131:
#line 704 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 132:
#line 706 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); ;}
    break;

  case 133:
#line 707 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); ;}
    break;

  case 134:
#line 708 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 135:
#line 710 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), BAR_ARROW2, 1); ;}
    break;

  case 136:
#line 711 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), BAR_IF, 1); ;}
    break;

  case 137:
#line 712 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_STATEMENT, 1); ;}
    break;

  case 138:
#line 713 "surface.yy"
    { CM->addStatement(lexerBubble); ;}
    break;

  case 139:
#line 715 "surface.yy"
    { lexBubble(BAR_COLON, 1); ;}
    break;

  case 140:
#line 716 "surface.yy"
    { Token::peelParens(lexerBubble); CM->addOpDecl(lexerBubble); ;}
    break;

  case 141:
#line 717 "surface.yy"
    { CM->setFlag(SymbolType::MESSAGE); ;}
    break;

  case 142:
#line 720 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 143:
#line 725 "surface.yy"
    {
			;}
    break;

  case 144:
#line 728 "surface.yy"
    {
			;}
    break;

  case 145:
#line 731 "surface.yy"
    { clear(); ;}
    break;

  case 146:
#line 732 "surface.yy"
    { CM->addSubsortDecl(tokenSequence); ;}
    break;

  case 147:
#line 735 "surface.yy"
    {
			  //
			  //	Fix things that might be in a bad state due
			  //	to a partially processed declaration.
			  //
			  cleanUpModuleExpression();
			  CM->makeOpDeclsConsistent();
			;}
    break;

  case 148:
#line 745 "surface.yy"
    {;}
    break;

  case 149:
#line 746 "surface.yy"
    {;}
    break;

  case 152:
#line 754 "surface.yy"
    {
			;}
    break;

  case 153:
#line 758 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); ;}
    break;

  case 154:
#line 759 "surface.yy"
    { currentSyntaxContainer->addVarDecl((yyvsp[0].yyToken)); ;}
    break;

  case 157:
#line 766 "surface.yy"
    { singleton[0] = (yyvsp[0].yyToken); CM->addOpDecl(singleton); ;}
    break;

  case 158:
#line 767 "surface.yy"
    { CM->addOpDecl(lexerBubble); ;}
    break;

  case 161:
#line 773 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) <<
				       ": missing " << QUOTE("->") << " in constant declaration.");
			;}
    break;

  case 163:
#line 781 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			;}
    break;

  case 164:
#line 786 "surface.yy"
    {
			  IssueWarning(LineNumber((yyvsp[0].yyToken).lineNumber()) <<
			  ": missing " << QUOTE("->") << " in operator declaration.");
			;}
    break;

  case 165:
#line 793 "surface.yy"
    {
			  if ((yyvsp[-1].yyBool))
			    CM->convertSortsToKinds();
			;}
    break;

  case 167:
#line 800 "surface.yy"
    {;}
    break;

  case 168:
#line 803 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 169:
#line 804 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 172:
#line 812 "surface.yy"
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  currentSyntaxContainer->addType(false, singleton);
			;}
    break;

  case 173:
#line 816 "surface.yy"
    { clear(); ;}
    break;

  case 174:
#line 818 "surface.yy"
    {
			  currentSyntaxContainer->addType(true, tokenSequence);
			;}
    break;

  case 175:
#line 823 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 176:
#line 824 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 177:
#line 827 "surface.yy"
    {;}
    break;

  case 181:
#line 836 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
			;}
    break;

  case 182:
#line 840 "surface.yy"
    {
			  CM->setFlag(SymbolType::LEFT_ID);
			;}
    break;

  case 183:
#line 844 "surface.yy"
    {
			  CM->setFlag(SymbolType::RIGHT_ID);
			;}
    break;

  case 184:
#line 850 "surface.yy"
    {
			  CM->setFlag(SymbolType::ASSOC);
			;}
    break;

  case 185:
#line 854 "surface.yy"
    {
			  CM->setFlag(SymbolType::COMM);
			;}
    break;

  case 186:
#line 857 "surface.yy"
    { lexBubble(BAR_RIGHT_BRACKET | BAR_OP_ATTRIBUTE, 1); ;}
    break;

  case 187:
#line 858 "surface.yy"
    { CM->setIdentity(lexerBubble); ;}
    break;

  case 188:
#line 860 "surface.yy"
    {
			  CM->setFlag(SymbolType::IDEM);
			;}
    break;

  case 189:
#line 864 "surface.yy"
    {
			  CM->setFlag(SymbolType::ITER);
			;}
    break;

  case 190:
#line 867 "surface.yy"
    { CM->setPrec((yyvsp[0].yyToken)); ;}
    break;

  case 191:
#line 868 "surface.yy"
    { clear(); ;}
    break;

  case 192:
#line 869 "surface.yy"
    { CM->setGather(tokenSequence); ;}
    break;

  case 193:
#line 870 "surface.yy"
    { clear(); ;}
    break;

  case 194:
#line 871 "surface.yy"
    { CM->setFormat(tokenSequence); ;}
    break;

  case 195:
#line 872 "surface.yy"
    { clear(); ;}
    break;

  case 196:
#line 873 "surface.yy"
    { CM->setStrat(tokenSequence); ;}
    break;

  case 197:
#line 874 "surface.yy"
    { clear(); ;}
    break;

  case 198:
#line 875 "surface.yy"
    { CM->setPoly(tokenSequence); ;}
    break;

  case 199:
#line 877 "surface.yy"
    {
			  CM->setFlag(SymbolType::MEMO);
			;}
    break;

  case 200:
#line 881 "surface.yy"
    {
			  CM->setFlag(SymbolType::CTOR);
			;}
    break;

  case 201:
#line 885 "surface.yy"
    {
			  clear();
			  CM->setFrozen(tokenSequence);
			;}
    break;

  case 202:
#line 889 "surface.yy"
    { clear(); ;}
    break;

  case 203:
#line 890 "surface.yy"
    { CM->setFrozen(tokenSequence); ;}
    break;

  case 204:
#line 892 "surface.yy"
    {
			  CM->setFlag(SymbolType::CONFIG);
			;}
    break;

  case 205:
#line 896 "surface.yy"
    {
			  CM->setFlag(SymbolType::OBJECT);
			;}
    break;

  case 206:
#line 900 "surface.yy"
    {
			  CM->setFlag(SymbolType::MESSAGE);
			;}
    break;

  case 207:
#line 904 "surface.yy"
    {
			  CM->setMetadata((yyvsp[0].yyToken));
			;}
    break;

  case 208:
#line 907 "surface.yy"
    { lexerLatexMode(); ;}
    break;

  case 209:
#line 908 "surface.yy"
    { CM->setLatexMacro((yyvsp[-1].yyString)); ;}
    break;

  case 210:
#line 909 "surface.yy"
    {;}
    break;

  case 211:
#line 911 "surface.yy"
    {
			  CM->setFlag(SymbolType::DITTO);
			;}
    break;

  case 214:
#line 924 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 215:
#line 925 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 218:
#line 932 "surface.yy"
    { clear(); CM->addHook(PreModule::ID_HOOK, (yyvsp[0].yyToken), tokenSequence); ;}
    break;

  case 219:
#line 933 "surface.yy"
    { CM->addHook(PreModule::ID_HOOK, (yyvsp[-1].yyToken), lexerBubble); ;}
    break;

  case 220:
#line 934 "surface.yy"
    { CM->addHook(PreModule::OP_HOOK, (yyvsp[-1].yyToken), lexerBubble); ;}
    break;

  case 221:
#line 935 "surface.yy"
    { CM->addHook(PreModule::TERM_HOOK, (yyvsp[-1].yyToken), lexerBubble); ;}
    break;

  case 222:
#line 941 "surface.yy"
    {;}
    break;

  case 223:
#line 943 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing " <<
				       QUOTE("is") << " keyword.");
			;}
    break;

  case 224:
#line 949 "surface.yy"
    {;}
    break;

  case 225:
#line 951 "surface.yy"
    {
			  IssueWarning(LineNumber(lineNumber) << ": missing period.");
			;}
    break;

  case 226:
#line 959 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 228:
#line 963 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 229:
#line 964 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 230:
#line 965 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 231:
#line 966 "surface.yy"
    { store((yyvsp[0].yyToken)); ;}
    break;

  case 232:
#line 973 "surface.yy"
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

  case 233:
#line 984 "surface.yy"
    { fragStore((yyvsp[0].yyToken)); ;}
    break;

  case 234:
#line 985 "surface.yy"
    { fragStore((yyvsp[0].yyToken)); ;}
    break;

  case 235:
#line 986 "surface.yy"
    { fragStore((yyvsp[0].yyToken)); ;}
    break;

  case 236:
#line 989 "surface.yy"
    { fragStore((yyvsp[0].yyToken)); ;}
    break;

  case 237:
#line 990 "surface.yy"
    {;}
    break;

  case 238:
#line 991 "surface.yy"
    {;}
    break;

  case 341:
#line 1071 "surface.yy"
    { lexBubble(END_COMMAND, 1) ;}
    break;

  case 342:
#line 1073 "surface.yy"
    {
			  interpreter.setCurrentModule(lexerBubble);
			;}
    break;

  case 343:
#line 1076 "surface.yy"
    { lexBubble(END_COMMAND, 1) ;}
    break;

  case 344:
#line 1078 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->dump();
			;}
    break;

  case 345:
#line 1083 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 346:
#line 1088 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.parse(lexerBubble);
			;}
    break;

  case 347:
#line 1095 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 348:
#line 1100 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.creduce(lexerBubble);
			;}
    break;

  case 349:
#line 1107 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 350:
#line 1112 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.reduce(lexerBubble, (yyvsp[-3].yyBool));
			;}
    break;

  case 351:
#line 1119 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 352:
#line 1125 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.rewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			;}
    break;

  case 353:
#line 1131 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 354:
#line 1138 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.eRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			;}
    break;

  case 355:
#line 1144 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 356:
#line 1151 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.fRewrite(lexerBubble, number, number2, (yyvsp[-3].yyBool));
			;}
    break;

  case 357:
#line 1157 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 358:
#line 1163 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.sRewrite(lexerBubble, number, (yyvsp[-3].yyBool));
			;}
    break;

  case 359:
#line 1169 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			  number2 = NONE;
			;}
    break;

  case 360:
#line 1176 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.search(lexerBubble, number, number2, (yyvsp[-2].yySearchKind));
			;}
    break;

  case 361:
#line 1182 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 362:
#line 1188 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.match(lexerBubble, (yyvsp[-2].yyBool), number);
			;}
    break;

  case 363:
#line 1194 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			  number = NONE;
			;}
    break;

  case 364:
#line 1200 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.unify(lexerBubble, number);
			;}
    break;

  case 365:
#line 1206 "surface.yy"
    {
			  interpreter.cont((yyvsp[-1].yyInt64), (yyvsp[-3].yyBool));
			;}
    break;

  case 366:
#line 1210 "surface.yy"
    {
			  lexerCmdMode();
			  moduleExpr.contractTo(0);
			;}
    break;

  case 367:
#line 1215 "surface.yy"
    {
			  lexerInitialMode();
			  if (interpreter.setCurrentModule(moduleExpr, 1))
			    interpreter.loop(lexerBubble);
			;}
    break;

  case 368:
#line 1221 "surface.yy"
    {
			  moduleExpr.contractTo(0);
			  if (interpreter.setCurrentModule(moduleExpr))  // HACK
			    interpreter.contLoop(lexerBubble);
			;}
    break;

  case 369:
#line 1226 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 370:
#line 1228 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceSelect((yyvsp[-2].yyBool));
			;}
    break;

  case 371:
#line 1232 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 372:
#line 1234 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.traceExclude((yyvsp[-2].yyBool));
			;}
    break;

  case 373:
#line 1238 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 374:
#line 1240 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.breakSelect((yyvsp[-2].yyBool));
			;}
    break;

  case 375:
#line 1244 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 376:
#line 1246 "surface.yy"
    {
			  lexerInitialMode();
			  interpreter.printConceal((yyvsp[-2].yyBool));
			;}
    break;

  case 377:
#line 1251 "surface.yy"
    {
			  if (CM != 0)  // HACK
			    CM->getFlatSignature()->clearMemo();
			;}
    break;

  case 378:
#line 1258 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 379:
#line 1260 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			;}
    break;

  case 380:
#line 1264 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 381:
#line 1266 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    CM->showModule();
			;}
    break;

  case 382:
#line 1270 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 383:
#line 1272 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showModule(true);
			;}
    break;

  case 384:
#line 1276 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 385:
#line 1278 "surface.yy"
    {
			  if (interpreter.setCurrentView(lexerBubble))
			    interpreter.showView();
			;}
    break;

  case 386:
#line 1283 "surface.yy"
    {
			  interpreter.showModules(true);
			;}
    break;

  case 387:
#line 1287 "surface.yy"
    {
			  interpreter.showNamedViews();
			;}
    break;

  case 388:
#line 1290 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 389:
#line 1292 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSortsAndSubsorts();
			;}
    break;

  case 390:
#line 1296 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 391:
#line 1298 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showOps();
			;}
    break;

  case 392:
#line 1302 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 393:
#line 1304 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showVars();
			;}
    break;

  case 394:
#line 1308 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 395:
#line 1310 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showMbs();
			;}
    break;

  case 396:
#line 1314 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 397:
#line 1316 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showEqs();
			;}
    break;

  case 398:
#line 1320 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 399:
#line 1322 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showRls();
			;}
    break;

  case 400:
#line 1326 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 401:
#line 1328 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showSummary();
			;}
    break;

  case 402:
#line 1332 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 403:
#line 1334 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showKinds();
			;}
    break;

  case 404:
#line 1339 "surface.yy"
    {
			  interpreter.showSearchPath((yyvsp[-1].yyInt64));
			;}
    break;

  case 405:
#line 1343 "surface.yy"
    {
			  interpreter.showSearchPathLabels((yyvsp[-1].yyInt64));
			;}
    break;

  case 406:
#line 1347 "surface.yy"
    {
			  interpreter.showSearchGraph();
			;}
    break;

  case 407:
#line 1350 "surface.yy"
    { lexBubble(END_COMMAND, 0); ;}
    break;

  case 408:
#line 1352 "surface.yy"
    {
			  if (interpreter.setCurrentModule(lexerBubble))
			    interpreter.showProfile();
			;}
    break;

  case 409:
#line 1360 "surface.yy"
    {
			  globalAdvisoryFlag = (yyvsp[-1].yyBool);
			;}
    break;

  case 410:
#line 1364 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			;}
    break;

  case 411:
#line 1368 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_STATS, (yyvsp[-1].yyBool));
			;}
    break;

  case 412:
#line 1372 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_TIMING, (yyvsp[-1].yyBool));
			;}
    break;

  case 413:
#line 1376 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_BREAKDOWN, (yyvsp[-1].yyBool));
			;}
    break;

  case 414:
#line 1380 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_LOOP_TIMING, (yyvsp[-1].yyBool));
			;}
    break;

  case 415:
#line 1384 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_COMMAND, (yyvsp[-1].yyBool));
			;}
    break;

  case 416:
#line 1388 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			;}
    break;

  case 417:
#line 1392 "surface.yy"
    {
			  interpreter.setPrintFlag((yyvsp[-2].yyPrintFlags), (yyvsp[-1].yyBool));
			;}
    break;

  case 418:
#line 1396 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE, (yyvsp[-1].yyBool));
			;}
    break;

  case 419:
#line 1400 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PRINT_ATTRIBUTE_NEWLINE, (yyvsp[-1].yyBool));
			;}
    break;

  case 420:
#line 1404 "surface.yy"
    {
			  interpreter.setFlag((yyvsp[-2].yyFlags), (yyvsp[-1].yyBool));
			;}
    break;

  case 421:
#line 1408 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::BREAK, (yyvsp[-1].yyBool));
			;}
    break;

  case 422:
#line 1411 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 423:
#line 1412 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 424:
#line 1414 "surface.yy"
    {
			  interpreter.setAutoImport((yyvsp[-5].yyImportMode), (yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			;}
    break;

  case 425:
#line 1417 "surface.yy"
    { lexerCmdMode(); ;}
    break;

  case 426:
#line 1418 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 427:
#line 1420 "surface.yy"
    {
			  interpreter.setOmodInclude((yyvsp[-3].yyToken), (yyvsp[-1].yyBool));
			;}
    break;

  case 428:
#line 1424 "surface.yy"
    {
			  globalVerboseFlag = (yyvsp[-1].yyBool);
			;}
    break;

  case 429:
#line 1428 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_MEMO, (yyvsp[-1].yyBool));
			;}
    break;

  case 430:
#line 1432 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_RULES, (yyvsp[-1].yyBool));
			;}
    break;

  case 431:
#line 1436 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::COMPILE_COUNT, (yyvsp[-1].yyBool));
			;}
    break;

  case 432:
#line 1440 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::PROFILE, (yyvsp[-1].yyBool));
			;}
    break;

  case 433:
#line 1444 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::AUTO_CLEAR_PROFILE, (yyvsp[-1].yyBool));
			;}
    break;

  case 434:
#line 1451 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::RESUME;
			;}
    break;

  case 435:
#line 1455 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::ABORT;
			;}
    break;

  case 436:
#line 1459 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::STEP;
			;}
    break;

  case 437:
#line 1463 "surface.yy"
    {
			  PARSE_RESULT = UserLevelRewritingContext::WHERE;
			;}
    break;

  case 438:
#line 1470 "surface.yy"
    {
			  MemoryCell::setShowGC((yyvsp[-1].yyBool));
			;}
    break;

  case 439:
#line 1474 "surface.yy"
    {
			  interpreter.setFlag(Interpreter::SHOW_STATS, (yyvsp[-1].yyBool));
			;}
    break;

  case 440:
#line 1480 "surface.yy"
    { lexerInitialMode(); ;}
    break;

  case 442:
#line 1487 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_MIXFIX; ;}
    break;

  case 443:
#line 1488 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FLAT; ;}
    break;

  case 444:
#line 1489 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_ALIASES; ;}
    break;

  case 445:
#line 1490 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_WITH_PARENS; ;}
    break;

  case 446:
#line 1491 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_GRAPH; ;}
    break;

  case 447:
#line 1492 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_CONCEAL; ;}
    break;

  case 448:
#line 1493 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_NUMBER; ;}
    break;

  case 449:
#line 1494 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_RAT; ;}
    break;

  case 450:
#line 1495 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_COLOR; ;}
    break;

  case 451:
#line 1496 "surface.yy"
    { (yyval.yyPrintFlags) = Interpreter::PRINT_FORMAT; ;}
    break;

  case 452:
#line 1499 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE; ;}
    break;

  case 453:
#line 1500 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_CONDITION; ;}
    break;

  case 454:
#line 1501 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_WHOLE; ;}
    break;

  case 455:
#line 1502 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SUBSTITUTION; ;}
    break;

  case 456:
#line 1503 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_SELECT; ;}
    break;

  case 457:
#line 1504 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_MB; ;}
    break;

  case 458:
#line 1505 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_EQ; ;}
    break;

  case 459:
#line 1506 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_RL; ;}
    break;

  case 460:
#line 1507 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_REWRITE; ;}
    break;

  case 461:
#line 1508 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BODY; ;}
    break;

  case 462:
#line 1509 "surface.yy"
    { (yyval.yyFlags) = Interpreter::TRACE_BUILTIN; ;}
    break;

  case 463:
#line 1512 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 464:
#line 1513 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 465:
#line 1516 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 466:
#line 1517 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 467:
#line 1520 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 468:
#line 1521 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 469:
#line 1524 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 470:
#line 1525 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 471:
#line 1530 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::NARROW; ;}
    break;

  case 472:
#line 1531 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::XG_NARROW; ;}
    break;

  case 473:
#line 1532 "surface.yy"
    { (yyval.yySearchKind) = Interpreter::SEARCH; ;}
    break;

  case 474:
#line 1535 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 475:
#line 1536 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 476:
#line 1539 "surface.yy"
    { (yyval.yyBool) = true; ;}
    break;

  case 477:
#line 1540 "surface.yy"
    { (yyval.yyBool) = false; ;}
    break;

  case 478:
#line 1543 "surface.yy"
    { (yyval.yyInt64) = (yyvsp[0].yyInt64); ;}
    break;

  case 479:
#line 1544 "surface.yy"
    { (yyval.yyInt64) = NONE; ;}
    break;

  case 480:
#line 1547 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::PROTECTING; ;}
    break;

  case 481:
#line 1548 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::EXTENDING; ;}
    break;

  case 482:
#line 1549 "surface.yy"
    { (yyval.yyImportMode) = ImportModule::INCLUDING; ;}
    break;

  case 484:
#line 1559 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 486:
#line 1563 "surface.yy"
    { moduleExpr = lexerBubble; lexBubble(END_COMMAND, 1); ;}
    break;

  case 489:
#line 1575 "surface.yy"
    { lexSave((yyvsp[0].yyToken)); ;}
    break;

  case 492:
#line 1578 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0);  ;}
    break;

  case 494:
#line 1583 "surface.yy"
    { lexContinueSave((yyvsp[0].yyToken)); ;}
    break;

  case 496:
#line 1585 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 499:
#line 1591 "surface.yy"
    { number = Token::codeToInt64(lexerBubble[1].code()); ;}
    break;

  case 501:
#line 1593 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 504:
#line 1608 "surface.yy"
    { lexSave((yyvsp[0].yyToken)); ;}
    break;

  case 507:
#line 1611 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 509:
#line 1620 "surface.yy"
    { lexContinueSave((yyvsp[0].yyToken)); ;}
    break;

  case 511:
#line 1622 "surface.yy"
    { lexContinueSave((yyvsp[0].yyToken)); ;}
    break;

  case 513:
#line 1624 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 516:
#line 1634 "surface.yy"
    { number = Token::codeToInt64(lexerBubble[1].code()); ;}
    break;

  case 518:
#line 1636 "surface.yy"
    { lexContinueSave((yyvsp[0].yyToken)); ;}
    break;

  case 520:
#line 1638 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 523:
#line 1648 "surface.yy"
    { lexContinueSave((yyvsp[0].yyToken)); ;}
    break;

  case 525:
#line 1650 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 528:
#line 1661 "surface.yy"
    {
			  number = Token::codeToInt64(lexerBubble[1].code());
			  number2 = Token::codeToInt64(lexerBubble[3].code());
			  clear();
			;}
    break;

  case 530:
#line 1667 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 533:
#line 1677 "surface.yy"
    { lexContinueSave((yyvsp[0].yyToken)); ;}
    break;

  case 535:
#line 1679 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 538:
#line 1689 "surface.yy"
    { number2 = Token::codeToInt64(lexerBubble[2].code()); ;}
    break;

  case 540:
#line 1691 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0); ;}
    break;

  case 543:
#line 1696 "surface.yy"
    { lexContinueBubble((yyvsp[0].yyToken), END_COMMAND, 0, 1); ;}
    break;

  case 546:
#line 1702 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), END_COMMAND, 1, 1); ;}
    break;

  case 548:
#line 1704 "surface.yy"
    { lexBubble((yyvsp[0].yyToken), BAR_COLON | END_COMMAND, 0); ;}
    break;

  case 550:
#line 1707 "surface.yy"
    {
			  lexerBubble.resize(1);
			  lexerBubble[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(lexerBubble[0]);
			;}
    break;

  case 588:
#line 1746 "surface.yy"
    {;}
    break;

  case 590:
#line 1751 "surface.yy"
    {
			  singleton[0].dropChar((yyvsp[0].yyToken));
			  missingSpace(singleton[0]);
			  interpreter.addSelected(singleton);
			;}
    break;

  case 593:
#line 1762 "surface.yy"
    {
			  singleton[0] = (yyvsp[0].yyToken);
			  interpreter.addSelected(singleton);
			;}
    break;

  case 594:
#line 1767 "surface.yy"
    {
			  interpreter.addSelected(lexerBubble);
			;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 5038 "surface.c"

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


#line 1797 "surface.yy"


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

void
missingSpace(const Token& token)
{
  IssueWarning(LineNumber(token.lineNumber()) << ": missing space between " <<
	       QUOTE(token) << " and period.");
}

