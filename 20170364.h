/* Headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <math.h>

/* Macros */
#define MAX_COMMAND 20
#define MAX_MNEMONIC 10
#define MEMORY_ROW 65536
#define MEMORY_COL 16
#define LINE_WIDTH 38
#define MAX_MEMORY 1048575
#define HASH_LENTH 20
#define MAX_LINE 100

/* Global Variables */
char *code = NULL;
int memory[MEMORY_ROW][MEMORY_COL];
int lastAddr = 0;			// address accessed most recently
//int lastCalledAddr = -1;	// last called by dump()
const char *FILENAME = "opcode.txt";
int startaddr;
int endaddr;

typedef struct _command {
	char comm[MAX_COMMAND];
	char *par1;
	char *par2;
	char *par3;
	struct _command *next;
	int indexNum;
}command;
int commandNum = 0;
command *commandRoot = NULL;

typedef struct _mnemonic {
	char mnem[MAX_MNEMONIC];
	int opcode;
	int format;
	int formatXe;
	struct _mnemonic *next;
}mnemonic;
mnemonic *hash_table[20];

typedef struct _inst {
	int lineNum;
	int loc;
	char *symbol;
	char *mnem;
	char *var1;
	char *var2;
	int objcode;
	char *tmpobj;
	struct _inst *next;
	struct _inst *prev;
}inst;
inst *instRoot = NULL;

typedef struct _sym {
	char *symbol;
	int loc;
	int visit;
	struct _sym *next;
	struct _sym *prev;
} sym;
sym *symbol_hash[26];


/* Functions */
char *changemode(char *code);
int mode();
command *findLastCommand();
int delLastCommand();
int addCommand(char *code);
int freeLinkedList();

int power(int x, int expo);
char *dtoh(int val, int base);
int htod(char* hex);
char *changeAddr5(char* addr);
char *changeAddr2(char* addr);
int checkMemErr(int start, int end);
int checkValErr(int value);
int hashFunc(char *mnemonic);
int readFile();
int makeHashTable(mnemonic *newM, int hashkey);

int initInst(inst *newInst);
int makeInstList();
int checkMnem(char *mnem);
int checkConstSize(char *mnem, char *con);
int checkVarSize(char *mnem, char *var);
int locSet(); // pass1
int symHashFunc(char* symbol);
int makeSymHash();
int findSymLoc(inst *tmpinst);
int regnum(char *r);
int getObjCode(inst *tmpinst, int format, int n, int i, int x, int b, int p, int e, int dist, int compflag);
int objcodeSet(); //pass2
int getFormat(char *mnem);
char *itoa(int value, char* result, int base);


void help();
int dir();
int quit(int flag);
void history();
int dump();
int edit();
int reset();
int fill();
int opcode_mnemonic();
int opcodelist();
int assemble();
int type();
int symbol();

