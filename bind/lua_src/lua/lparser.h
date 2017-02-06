/*
** $Id: lparser.h,v 1.70.1.1 2013/04/12 18:48:47 roberto Exp $
** Lua Parser
** See Copyright Notice in lua.h
*/

#ifndef lparser_h
#define lparser_h

#include "llimits.h"
#include "lobject.h"
#include "lzio.h"


/*
** Expression descriptor
** 表达式描述
*/

typedef enum {
  VVOID,	/* no value */
  VNIL,
  VTRUE,
  VFALSE,   /* 值类型 */
  VK,		/* info = index of constant in `k' 常量在proto->中索引 */
  VKNUM,	/* nval = numerical value */
  VNONRELOC,/* info = result register 不需要重定向 */
  VLOCAL,	/* info = local register 代表局部变量表达式，在info中保存局部变量对应的寄存器id */
  VUPVAL,   /* info = index of upvalue in 'upvalues' 代表upvalue变量表达式，在info中保存upvalue的id */
  VINDEXED,	/* t = table register/upvalue; idx = index R/K 代表对一个表进行索引的变量表达式，比如a.b或者a[1]，使用ind结构体保存数据 */
  VJMP,		/* info = instruction pc */
  VRELOCABLE,	/* info = instruction pc 需要重定位*/
  VCALL,	/* info = instruction pc */
  VVARARG	/* info = instruction pc */
} expkind;


#define vkisvar(k)	(VLOCAL <= (k) && (k) <= VINDEXED)
#define vkisinreg(k)	((k) == VNONRELOC || (k) == VLOCAL)
/* variable (global, local, upvalue, or indexed) 生成字节码需要*/
typedef struct expdesc {
  expkind k;  /*表达式类型，类型不同就在不同的表中找*/
  union {
    struct {  /* for indexed variables (VINDEXED) */
      short idx;  /* index (R/K) 感觉像是寄存器索引*/
      lu_byte t;  /* table (register or upvalue) 名称在常量表中的索引*/
      lu_byte vt;  /* whether 't' is register (VLOCAL) or upvalue (VUPVAL) 变量类型*/
    } ind;
    int info;  /* for generic use 变量在变量表中的位置*/
    lua_Number nval;  /* for VKNUM 存数字的值*/
  } u;
  int t;  /* patch list of `exit when true' 当为true时，待回填的坐标 */
  int f;  /* patch list of `exit when false' 当为false时，待回填的坐标 */
} expdesc;


/* description of active local variable */
typedef struct Vardesc {
  short idx;  /* variable index in stack 名字的index，名字在proto->locvars中， */
} Vardesc;


/* description of pending goto statements and label statements */
typedef struct Labeldesc {
  TString *name;  /* label identifier */
  int pc;  /* position in code */
  int line;  /* line where it appeared */
  lu_byte nactvar;  /* local level where it appears in current block */
} Labeldesc;


/* list of labels or gotos */
typedef struct Labellist {
  Labeldesc *arr;  /* array */
  int n;  /* number of entries in use */
  int size;  /* array size */
} Labellist;


/* dynamic structures used by the parser */
/* 只存放当前代码块的局部变量,goto表，goto标签 */
typedef struct Dyndata {
  struct {  /* list of active local variables 局部变量 */
    Vardesc *arr;  /* 当前局部变量数组 */
    int n;		   /* 局部变量个数 */
    int size;      /* 数组申请的大小 */
  } actvar;
  Labellist gt;    /* list of pending gotos goto的列表*/
  Labellist label; /* list of active labels 标签列表*/
} Dyndata;


/* control of blocks */
struct BlockCnt;  /* defined in lparser.c */


/* state needed to generate code for a given function 表示parse时函数状态信息*/
typedef struct FuncState {
  Proto *f;  /* current function header 保存函数指令，变量，upvalue等信息*/
  Table *h;  /* table to find (and reuse) elements in `k'  常量表？*/
  struct FuncState *prev;  /* enclosing function 上层函数块*/
  struct LexState *ls;  /* lexical state 指向词法分析结构，全局？*/
  struct BlockCnt *bl;  /* chain of current blocks */
  int pc;  /* next position to code (equivalent to `ncode')下一个程序执行点 */
  int lasttarget;   /* 'label' of last 'jump label' */
  int jpc;  /* list of pending jumps to `pc' */
  int nk;  /* number of elements in `k' */
  int np;  /* number of elements in `p' */
  int firstlocal;  /* index of first local var (in Dyndata array) */
  short nlocvars;  /* number of elements in 'f->locvars' 局部变量个数*/
  lu_byte nactvar;  /* number of active local variables 有效局部变量的个数*/
  lu_byte nups;  /* number of upvalues upvalue的个数*/
  lu_byte freereg;  /* first free register 记录当前空闲寄存器的起始id*/
} FuncState;


LUAI_FUNC Closure *luaY_parser (lua_State *L, ZIO *z, Mbuffer *buff,
                                Dyndata *dyd, const char *name, int firstchar);


#endif
