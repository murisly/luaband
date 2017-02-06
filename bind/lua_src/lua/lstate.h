/*
** $Id: lstate.h,v 2.82.1.1 2013/04/12 18:48:47 roberto Exp $
** Global State
** See Copyright Notice in lua.h
*/

#ifndef lstate_h
#define lstate_h

#include "lua.h"

#include "lobject.h"
#include "ltm.h"
#include "lzio.h"


/*

** Some notes about garbage-collected objects:  All objects in Lua must
** be kept somehow accessible until being freed.
**
** Lua keeps most objects linked in list g->allgc. The link uses field
** 'next' of the CommonHeader.
**
** Strings are kept in several lists headed by the array g->strt.hash.
**
** Open upvalues are not subject to independent garbage collection. They
** are collected together with their respective threads. Lua keeps a
** double-linked list with all open upvalues (g->uvhead) so that it can
** mark objects referred by them. (They are always gray, so they must
** be remarked in the atomic step. Usually their contents would be marked
** when traversing the respective threads, but the thread may already be
** dead, while the upvalue is still accessible through closures.)
**
** Objects with finalizers are kept in the list g->finobj.
**
** The list g->tobefnz links all objects being finalized.

*/


struct lua_longjmp;  /* defined in ldo.c */



/* extra stack space to handle TM calls and some other extras */
#define EXTRA_STACK   5

/*基本栈的大小40*/
#define BASIC_STACK_SIZE        (2*LUA_MINSTACK)


/* kinds of Garbage Collection */
#define KGC_NORMAL	0
#define KGC_EMERGENCY	1	/* gc was forced by an allocation failure */
#define KGC_GEN		2	/* generational collection */


typedef struct stringtable {
  GCObject **hash;  //保存所有的字符串
  lu_int32 nuse;  /* number of elements 已装元素的个数*/
  int size;  //当前hash桶的大小
} stringtable;


/*
** information about a call 调用栈帧
*/
typedef struct CallInfo {
  StkId func;  /* function index in the stack 调用闭包的位置，正在执行函数的数据栈位置*/
  StkId	top;  /* top for this function 栈顶*/
  struct CallInfo *previous, *next;  /* dynamic call link 调用栈，双向链表*/
  short nresults;  /* expected number of results from this function 返回值个数*/
  lu_byte callstatus;  /*调用状态*/
  ptrdiff_t extra;
  union {
    struct {  /* only for Lua functions */
      StkId base;  /* base for this function 字节码的基址 */
      const Instruction *savedpc;  /*字节码的指针，一直在++*/
    } l;
    struct {  /* only for C functions */
      int ctx;  /* context info. in case of yields */
      lua_CFunction k;  /* continuation in case of yields */
      ptrdiff_t old_errfunc;
      lu_byte old_allowhook;
      lu_byte status;
    } c;
  } u;
} CallInfo;


/*
** Bits in CallInfo status
*/
#define CIST_LUA	(1<<0)	/* call is running a Lua function */
#define CIST_HOOKED	(1<<1)	/* call is running a debug hook */
#define CIST_REENTRY	(1<<2)	/* call is running on same invocation of
                                   luaV_execute of previous call */
#define CIST_YIELDED	(1<<3)	/* call reentered after suspension */
#define CIST_YPCALL	(1<<4)	/* call is a yieldable protected call */
#define CIST_STAT	(1<<5)	/* call has an error status (pcall) */
#define CIST_TAIL	(1<<6)	/* call was tail called */
#define CIST_HOOKYIELD	(1<<7)	/* last hook called yielded */


#define isLua(ci)	((ci)->callstatus & CIST_LUA)


/*
** `global state', shared by all threads of this state全局state所有线程共享
*/
typedef struct global_State {
  lua_Alloc frealloc;  /* function to reallocate memory 内存分配函数*/
  void *ud;         /* auxiliary data to `frealloc' */
  lu_mem totalbytes;  /* number of bytes currently allocated - GCdebt */
  l_mem GCdebt;  /* bytes allocated not yet compensated by the collector 被分配还未回收的内存*/
  lu_mem GCmemtrav;  /* memory traversed by the GC */
  lu_mem GCestimate;  /* an estimate of the non-garbage memory in use */
  stringtable strt;  /* hash table for strings  保存短字符串*/
  TValue l_registry;  /* lua 注册表项 */
  unsigned int seed;  /* randomized seed for hashes 计算hash的随机种子*/
  lu_byte currentwhite;
  lu_byte gcstate;  /* state of garbage collector 垃圾回收的状态*/
  lu_byte gckind;  /* kind of GC running */
  lu_byte gcrunning;  /* true if GC is running */
  int sweepstrgc;  /* position of sweep in `strt' */
  GCObject *allgc;  /* list of all collectable objects 被回收对象的链表*/
  GCObject *finobj;  /* list of collectable objects with finalizers */
  GCObject **sweepgc;  /* current position of sweep in list 'allgc' */
  GCObject **sweepfin;  /* current position of sweep in list 'finobj' */
  GCObject *gray;  /* list of gray objects */
  GCObject *grayagain;  /* list of objects to be traversed atomically */
  GCObject *weak;  /* list of tables with weak values */
  GCObject *ephemeron;  /* list of ephemeron tables (weak keys) */
  GCObject *allweak;  /* list of all-weak tables */
  GCObject *tobefnz;  /* list of userdata to be GC */
  UpVal uvhead;  /* head of double-linked list of all open upvalues */
  Mbuffer buff;  /* temporary buffer for string concatenation */
  int gcpause;  /* size of pause between successive GCs */
  int gcmajorinc;  /* pause between major collections (only in gen. mode) */
  int gcstepmul;  /* GC `granularity' */
  lua_CFunction panic;  /* to be called in unprotected errors 非保护模式下出错了调用 */
  struct lua_State *mainthread;  /*主线程*/
  const lua_Number *version;  /* pointer to version number */
  TString *memerrmsg;  /* memory-error message */
  TString *tmname[TM_N];  /* array with tag-method names 标记方法名的数组*/
  struct Table *mt[LUA_NUMTAGS];  /* metatables for basic types 元表基本类型*/
} global_State;


/*
** `per thread' state 每个线程的state
*/
struct lua_State {
  CommonHeader;
  lu_byte status;  //线程脚本的状态，线程可选状态如下：
  StkId top;  /* first free slot in the stack 指向栈顶元素*/
  global_State *l_G;  //全局栈
  CallInfo *ci;  /* call info for current function 当前函数信息*/
  const Instruction *oldpc;  /* last pc traced */
  StkId stack_last;  /* last free slot in the stack 栈底元素*/
  StkId stack;  /* stack base 当前指向的元素(也可以说成当前函数的栈底)*/
  int stacksize;  /*栈的大小*/
  unsigned short nny;  /* number of non-yieldable calls in stack 栈中不能被yiled的个数*/
  unsigned short nCcalls;  /* number of nested C calls 调用深度*/
  lu_byte hookmask;  /*支持的hook能力*/
  lu_byte allowhook;  /*是否允许hook*/
  int basehookcount;  /*用户设置的执行指令数(LUA_MASKCOUNT下有效)*/
  int hookcount;   /*运行时，跑了多少条指令（LUA_MASKCOUNT下有效）*/
  lua_Hook hook;  /*用户注册的hook函数*/
  GCObject *openupval;  /* list of open upvalues in this stack 用于GC*/
  GCObject *gclist; /*用于GC*/
  struct lua_longjmp *errorJmp;  /* current error recover point 发生错误的长跳转位置，用于记录当函数发生错误时跳转出去的位置*/
  ptrdiff_t errfunc;  /* current error handling function (stack index) */
  CallInfo base_ci;  /* CallInfo for first level (C calling Lua)函数调用栈的栈底 */
};


#define G(L)	(L->l_G)


/*
** Union of all collectable objects 可以GC的数据结构
*/
union GCObject {
  GCheader gch;      /* common header */
  union TString ts;  //string类型
  union Udata u;	 //用户数据
  union Closure cl;	 //闭包
  struct Table h;	 //表
  struct Proto p;	 //函数字节码结构
  struct UpVal uv;	 //闭包数据
  struct lua_State th;  /* thread 线程(协同)*/
};


#define gch(o)		(&(o)->gch)

/* macros to convert a GCObject into a specific value */
#define rawgco2ts(o)  \
	check_exp(novariant((o)->gch.tt) == LUA_TSTRING, &((o)->ts))  //计算得到GCObject中的tstring
#define gco2ts(o)	(&rawgco2ts(o)->tsv)
#define rawgco2u(o)	check_exp((o)->gch.tt == LUA_TUSERDATA, &((o)->u))
#define gco2u(o)	(&rawgco2u(o)->uv)
#define gco2lcl(o)	check_exp((o)->gch.tt == LUA_TLCL, &((o)->cl.l))
#define gco2ccl(o)	check_exp((o)->gch.tt == LUA_TCCL, &((o)->cl.c))
#define gco2cl(o)  \
	check_exp(novariant((o)->gch.tt) == LUA_TFUNCTION, &((o)->cl))
#define gco2t(o)	check_exp((o)->gch.tt == LUA_TTABLE, &((o)->h))
#define gco2p(o)	check_exp((o)->gch.tt == LUA_TPROTO, &((o)->p))
#define gco2uv(o)	check_exp((o)->gch.tt == LUA_TUPVAL, &((o)->uv))
#define gco2th(o)	check_exp((o)->gch.tt == LUA_TTHREAD, &((o)->th))

/* macro to convert any Lua object into a GCObject */
#define obj2gco(v)	(cast(GCObject *, (v)))


/* actual number of total bytes allocated */
#define gettotalbytes(g)	((g)->totalbytes + (g)->GCdebt)

LUAI_FUNC void luaE_setdebt (global_State *g, l_mem debt);
LUAI_FUNC void luaE_freethread (lua_State *L, lua_State *L1);
LUAI_FUNC CallInfo *luaE_extendCI (lua_State *L);
LUAI_FUNC void luaE_freeCI (lua_State *L);


#endif

