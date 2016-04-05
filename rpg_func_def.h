

/* how to add an obj:
 * 	add obj enum here.
 * 	add enum check/type str to obj_do_itype())
 *  add obj to def
 */


/* function id's 

#define F_SET		0
#define F_IF_JMP	1
*/


enum
{
	F_SET,
	F_PRINTLN,
	F_OP_ADD,
	F_OP_SUB,
	F_OP_MUL,
	F_OP_DIV,
	F_OP_MOD,
	F_CMP_EQUAL,
	F_CMP_NEQUAL,
	F_CMP_GREATER,
	F_CMP_LESSER,
	F_CMP_GREATEQUAL,
	F_CMP_LESSEQUAL,
	F_IF_JMP,
	F_JMP,
	F_TERM,
	F_TERM_VM,
	F_LOOP,
	F_SLEEP,
	F_KEY,
	F_KEY_DOWN,
	F_KEY_UP,
	F_KEY_MOVE,
	F_FADE_VAR,
	F_START,
	F_STOP,
	F_ACTIVATE,
	F_DEACTIVATE,
	F_RANDINT,
	F_CAM_FOCUS,
	F_CAM_UNFOCUS,
	F_CLIP,
	F_AACTORS_CLEAR,
	F_ATMAPS_CLEAR,
	F_KEYS_DISABLE,
	F_KEYS_ENABLE,
	F_SPRT_FILL_LAYER,
	F_TMAP_FILL_LAYER,
	F_MSG_SEND,
	F_MSG_CLEAR,
	F_MSG_CHECK,
	F_OPEN,
	F_CLOSE,
	F_SETMODE,
	F_PLAYSND,
	F_STOPSND,
	F_STOPALL,
	F_ADDITEM,
	F_DROPITEM,
	F_PRINT,
	F_TYPE,
	NFUNCNAMES /* this must be kept as last item */
};


enum
{
	O_NONE,
	O_GAME,
	O_SPRT,
	O_ACTOR,
	O_TMAP,
	O_SND,
	O_WIN,
	O_INV,
	O_ITEM
};
