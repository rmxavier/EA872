/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
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
    NL = 258,
    CR = 259,
    COMMA = 260,
    END = 261,
    SLASH = 262,
    BLANK = 263,
    AND = 264,
    GET = 265,
    TRACE = 266,
    OPTIONS = 267,
    HEAD = 268,
    PATCH = 269,
    POST = 270,
    PUT = 271,
    DELETE = 272,
    HOST = 273,
    USER_AGENT = 274,
    ACCEPT = 275,
    ACCEPT_LANGUAGE = 276,
    ACCEPT_ENCODING = 277,
    CONTENT_LENGTH = 278,
    CONTENT_TYPE = 279,
    CONNECTION = 280,
    UPGRADE_INSECURE_REQUESTS = 281,
    PARAMETER = 282
  };
#endif
/* Tokens.  */
#define NL 258
#define CR 259
#define COMMA 260
#define END 261
#define SLASH 262
#define BLANK 263
#define AND 264
#define GET 265
#define TRACE 266
#define OPTIONS 267
#define HEAD 268
#define PATCH 269
#define POST 270
#define PUT 271
#define DELETE 272
#define HOST 273
#define USER_AGENT 274
#define ACCEPT 275
#define ACCEPT_LANGUAGE 276
#define ACCEPT_ENCODING 277
#define CONTENT_LENGTH 278
#define CONTENT_TYPE 279
#define CONNECTION 280
#define UPGRADE_INSECURE_REQUESTS 281
#define PARAMETER 282

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 15 "parser.y" /* yacc.c:1909  */

	char * token;

#line 112 "y.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
