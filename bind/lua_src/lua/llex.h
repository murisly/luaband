/*
** $Id: llex.h,v 1.72.1.1 2013/04/12 18:48:47 roberto Exp $
** Lexical Analyzer
** See Copyright Notice in lua.h
*/

#ifndef llex_h
#define llex_h

#include "lobject.h"
#include "lzio.h"

/*ǰ������Ѿ������ascii*/
#define FIRST_RESERVED	257



/*
* WARNING: if you change the order of this enumeration,
* grep "ORDER RESERVED" �����ؼ���
*/
enum RESERVED {
  /* terminal symbols denoted by reserved words */
  TK_AND = FIRST_RESERVED, TK_BREAK,
  TK_DO, TK_ELSE, TK_ELSEIF, TK_END, TK_FALSE, TK_FOR, TK_FUNCTION,
  TK_GOTO, TK_IF, TK_IN, TK_LOCAL, TK_NIL, TK_NOT, TK_OR, TK_REPEAT,
  TK_RETURN, TK_THEN, TK_TRUE, TK_UNTIL, TK_WHILE,
  /* other terminal symbols */
  TK_CONCAT, TK_DOTS, TK_EQ, TK_GE, TK_LE, TK_NE, TK_DBCOLON, TK_EOS,
  TK_NUMBER, TK_NAME, TK_STRING
};

/* number of reserved words */
#define NUM_RESERVED	(cast(int, TK_WHILE-FIRST_RESERVED+1))


typedef union {
  lua_Number r;
  TString *ts;
} SemInfo;  /* semantics information */


typedef struct Token {
  int token;  /*token����*/
  SemInfo seminfo;  /*��ʾ������Ϣ������token�����Ϳ�������ֵ�����ַ���*/
} Token;


/* state of the lexer plus state of the parser when shared by all
   functions �ʷ�������������*/
typedef struct LexState {
  int current;  /* current character (charint) ��ǰ�������ַ�*/
  int linenumber;  /* input line counter ��ǰ������*/
  int lastline;  /* line of last token `consumed' ǰһ�е�����*/
  Token t;  /* current token ��ǰtoken(���)*/
  Token lookahead;  /* look ahead token */
  struct FuncState *fs;  /* current function (parser) ��ǰ��������״̬*/
  struct lua_State *L;  /*ָ���߳�ջ*/
  ZIO *z;  /* input stream */
  Mbuffer *buff;  /* buffer for tokens �洢��ǰ��ȡtoken��buffer*/
  struct Dyndata *dyd;  /* dynamic structures used by the parser �����оֲ�������*/
  TString *source;/* current source name ��ǰ���ļ���*/
  TString *envn;  /* environment variable name ��ǰ��������(_ENV)*/
  char decpoint;  /* locale decimal point ��ǰ��С����*/
} LexState;


LUAI_FUNC void luaX_init (lua_State *L);
LUAI_FUNC void luaX_setinput (lua_State *L, LexState *ls, ZIO *z,
                              TString *source, int firstchar);
LUAI_FUNC TString *luaX_newstring (LexState *ls, const char *str, size_t l);
LUAI_FUNC void luaX_next (LexState *ls);
LUAI_FUNC int luaX_lookahead (LexState *ls);
LUAI_FUNC l_noret luaX_syntaxerror (LexState *ls, const char *s);
LUAI_FUNC const char *luaX_token2str (LexState *ls, int token);


#endif
