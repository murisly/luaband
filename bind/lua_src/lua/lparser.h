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
** ���ʽ����
*/

typedef enum {
  VVOID,	/* no value */
  VNIL,
  VTRUE,
  VFALSE,   /* ֵ���� */
  VK,		/* info = index of constant in `k' ������proto->������ */
  VKNUM,	/* nval = numerical value */
  VNONRELOC,/* info = result register ����Ҫ�ض��� */
  VLOCAL,	/* info = local register ����ֲ��������ʽ����info�б���ֲ�������Ӧ�ļĴ���id */
  VUPVAL,   /* info = index of upvalue in 'upvalues' ����upvalue�������ʽ����info�б���upvalue��id */
  VINDEXED,	/* t = table register/upvalue; idx = index R/K �����һ������������ı������ʽ������a.b����a[1]��ʹ��ind�ṹ�屣������ */
  VJMP,		/* info = instruction pc */
  VRELOCABLE,	/* info = instruction pc ��Ҫ�ض�λ*/
  VCALL,	/* info = instruction pc */
  VVARARG	/* info = instruction pc */
} expkind;


#define vkisvar(k)	(VLOCAL <= (k) && (k) <= VINDEXED)
#define vkisinreg(k)	((k) == VNONRELOC || (k) == VLOCAL)
/* variable (global, local, upvalue, or indexed) �����ֽ�����Ҫ*/
typedef struct expdesc {
  expkind k;  /*���ʽ���ͣ����Ͳ�ͬ���ڲ�ͬ�ı�����*/
  union {
    struct {  /* for indexed variables (VINDEXED) */
      short idx;  /* index (R/K) �о����ǼĴ�������*/
      lu_byte t;  /* table (register or upvalue) �����ڳ������е�����*/
      lu_byte vt;  /* whether 't' is register (VLOCAL) or upvalue (VUPVAL) ��������*/
    } ind;
    int info;  /* for generic use �����ڱ������е�λ��*/
    lua_Number nval;  /* for VKNUM �����ֵ�ֵ*/
  } u;
  int t;  /* patch list of `exit when true' ��Ϊtrueʱ������������� */
  int f;  /* patch list of `exit when false' ��Ϊfalseʱ������������� */
} expdesc;


/* description of active local variable */
typedef struct Vardesc {
  short idx;  /* variable index in stack ���ֵ�index��������proto->locvars�У� */
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
/* ֻ��ŵ�ǰ�����ľֲ�����,goto��goto��ǩ */
typedef struct Dyndata {
  struct {  /* list of active local variables �ֲ����� */
    Vardesc *arr;  /* ��ǰ�ֲ��������� */
    int n;		   /* �ֲ��������� */
    int size;      /* ��������Ĵ�С */
  } actvar;
  Labellist gt;    /* list of pending gotos goto���б�*/
  Labellist label; /* list of active labels ��ǩ�б�*/
} Dyndata;


/* control of blocks */
struct BlockCnt;  /* defined in lparser.c */


/* state needed to generate code for a given function ��ʾparseʱ����״̬��Ϣ*/
typedef struct FuncState {
  Proto *f;  /* current function header ���溯��ָ�������upvalue����Ϣ*/
  Table *h;  /* table to find (and reuse) elements in `k'  ������*/
  struct FuncState *prev;  /* enclosing function �ϲ㺯����*/
  struct LexState *ls;  /* lexical state ָ��ʷ������ṹ��ȫ�֣�*/
  struct BlockCnt *bl;  /* chain of current blocks */
  int pc;  /* next position to code (equivalent to `ncode')��һ������ִ�е� */
  int lasttarget;   /* 'label' of last 'jump label' */
  int jpc;  /* list of pending jumps to `pc' */
  int nk;  /* number of elements in `k' */
  int np;  /* number of elements in `p' */
  int firstlocal;  /* index of first local var (in Dyndata array) */
  short nlocvars;  /* number of elements in 'f->locvars' �ֲ���������*/
  lu_byte nactvar;  /* number of active local variables ��Ч�ֲ������ĸ���*/
  lu_byte nups;  /* number of upvalues upvalue�ĸ���*/
  lu_byte freereg;  /* first free register ��¼��ǰ���мĴ�������ʼid*/
} FuncState;


LUAI_FUNC Closure *luaY_parser (lua_State *L, ZIO *z, Mbuffer *buff,
                                Dyndata *dyd, const char *name, int firstchar);


#endif
