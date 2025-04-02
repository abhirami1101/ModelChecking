/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

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

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     TOP = 258,
     BOTTOM = 259,
     NOT = 260,
     AND = 261,
     OR = 262,
     IMPLIES = 263,
     EX = 264,
     EG = 265,
     EF = 266,
     AX = 267,
     AG = 268,
     AF = 269,
     BEGCTL = 270,
     ENDCTL = 271,
     BEGK = 272,
     ENDK = 273,
     STATES = 274,
     TRANS = 275,
     LS = 276,
     A = 277,
     E = 278,
     U = 279,
     LPAREN = 280,
     RPAREN = 281,
     PROP_VAR = 282,
     STATE_VAR = 283
   };
#endif
/* Tokens.  */
#define TOP 258
#define BOTTOM 259
#define NOT 260
#define AND 261
#define OR 262
#define IMPLIES 263
#define EX 264
#define EG 265
#define EF 266
#define AX 267
#define AG 268
#define AF 269
#define BEGCTL 270
#define ENDCTL 271
#define BEGK 272
#define ENDK 273
#define STATES 274
#define TRANS 275
#define LS 276
#define A 277
#define E 278
#define U 279
#define LPAREN 280
#define RPAREN 281
#define PROP_VAR 282
#define STATE_VAR 283




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 18 "ModelChecker.y"
{
	char prop_var;
	struct treeNode* node;
	char* state_var;
	struct state* State;
	struct StateNode* States;
	struct transition* tran;
	struct KripkeStructure* Kripke;
	struct label* label;
	struct labelling* Labelling;
	char* Prop_list;
}
/* Line 1529 of yacc.c.  */
#line 118 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

