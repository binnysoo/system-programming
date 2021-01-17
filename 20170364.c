#include "20170364.h"

/**********
[function]	delete blank, change upper case to lower case 
[parameter]	(1) input code
[return]	changed valid code
**********/
char* changemode(char *code) {
	int i, index = 0;

	while (code[index] == ' ') index++;		// is blank from 0 to index - 1 		
	if (index != 0) {
		for (i=0;i<strlen(code)-index;i++) 
			code[i] = code[i + index];		// delete blank
		code[i] = '\0';
	}

	if (code[strlen(code)-1] == '\n') code[strlen(code)-1] = '\0';

	return code;
}

/**********
[function]	get input from the user
[parameter]	none
[return]	code for each operation (0: error, 1: help, 2: dir, 3: quit, 4: history ...)
**********/
int  mode(){
	char tmp_code[MAX_COMMAND];
	char *code_ptr;
	int i, UptoLow;

	code = (char*)malloc(sizeof(char)*MAX_COMMAND);
	printf("sicsim> ");
	fgets(code, sizeof(char)*MAX_COMMAND, stdin);
	code = changemode(code);

	if (!strcmp(code,"") || !strcmp(code, "\n")) return 0;    // exeption (no input)

	strcpy(tmp_code, code);
	code_ptr = strtok(tmp_code, " ");
	
	UptoLow = (int)('a' - 'A');
	for (i=0;i<strlen(code_ptr);i++) {	// change upper case to lower case
		if ((int)('A') <= code_ptr[i] && code_ptr[i] <= (int)('Z')) {
			code_ptr[i] = code_ptr[i] + UptoLow;
			code[i] = code_ptr[i];
		}
	}
	/*
	for (i=strlen(code_ptr);i<strlen(code);i++) {		// change address value from lowercase to uppercase
		if ('a' <= code[i] && code[i] <= 'f') {
			code[i] = code[i] - UptoLow;
		}
	}
	*/

	if (!(strcmp(code_ptr, "h")) || !(strcmp(code_ptr, "help"))) return 1;
	else if (!(strcmp(code_ptr, "d")) || !(strcmp(code_ptr, "dir"))) return 2;
	else if (!(strcmp(code_ptr, "q")) || !(strcmp(code_ptr, "quit"))) return 3;
	else if (!(strcmp(code_ptr, "hi")) || !(strcmp(code_ptr, "history"))) return 4;
	else if (!(strcmp(code_ptr, "du")) || !(strcmp(code_ptr, "dump"))) return 5;
	else if (!(strcmp(code_ptr, "e")) || !(strcmp(code_ptr, "edit"))) return 6;
	else if (!(strcmp(code_ptr, "f")) || !(strcmp(code_ptr, "fill"))) return 7;
	else if (!(strcmp(code_ptr, "reset"))) return 8;
	else if (!(strcmp(code_ptr, "opcode"))) return 9;
	else if (!(strcmp(code_ptr, "opcodelist"))) return 10;
	else if (!(strcmp(code_ptr, "assemble"))) return 11;
	else if (!(strcmp(code_ptr, "type"))) return 12;
	else if (!(strcmp(code_ptr, "symbol"))) return 13;
	else return 0;
}

/**********
[function]	find the last command structure in the command linked list 
[parameter]	none
[return]	last command structure
**********/
command* findLastCommand() {
	command* lastC;
	lastC = commandRoot;
	
	while(lastC->next != NULL) {
		lastC = lastC->next;
	}

	return lastC;
}

/**********
[function]	find the last command structure in the command linked list and delete it 
[parameter]	none
[return]	none
**********/
int delLastCommand() {
	command* lastC;
	command* newlastC;

	newlastC = commandRoot;
	lastC = newlastC->next;
	if (lastC == NULL) {
		free(newlastC);
		commandRoot->next = NULL;
		commandRoot = NULL;
	}
	else {
		while(lastC->next != NULL) {
			newlastC = newlastC->next;
			lastC = newlastC->next;
		}
		free(lastC->par1);
		free(lastC->par2);
		free(lastC->par3);
		free(lastC);
		newlastC->next = NULL;
	}
	commandNum--;
	return 0;
}


/**********
[function]	initialize command structure and put new command in the linked list
[parameter]	(1) input command code
[return]	none
**********/
int addCommand(char *code) {
	char *ptr;
	int count = 0;
	command *newC, *lastC;
	newC = (command*)malloc(sizeof(command));
	commandNum++;
	
	/* initialize */
	newC->par1 = NULL;
	newC->par2 = NULL;
	newC->par3 = NULL;
	
	/* slice string as the required format */
	ptr = strtok(code, " ");
	while (ptr != NULL) {
		if (count == 0) strcpy(newC->comm, code);
		else if (count == 1) { 
			newC->par1 = (char*)malloc(sizeof(char)*(strlen(ptr)+1));
			strcpy(newC->par1, ptr);
		}
		else if (count == 2) {
			newC->par2 = (char*)malloc(sizeof(char)*(strlen(ptr)+1));
			strcpy(newC->par2, ptr);
		}
		else if (count == 3) {
			newC->par3 = (char*)malloc(sizeof(char)*(strlen(ptr)+1));
			strcpy(newC->par3, ptr);
		}

		count++;
		ptr = strtok(NULL, ", ");
	}
	
	/* make linked list from the commandRoot(global variable) */
	newC->indexNum = commandNum;
	newC->next = NULL;
	if (commandRoot == NULL) commandRoot = newC;
	else {
		lastC = findLastCommand();
		lastC->next = newC; 
	}
	
	return 0;
}

/**********
[function]	free allocated linked list memory when the user quits the program
[parameter]	none
[return]	none
**********/
int freeLinkedList() {
	command *ptr1, *ptr2;
	
	ptr1 = commandRoot;
	ptr2 = commandRoot;
	while(!ptr2) {
		ptr2 = ptr1->next;
		free(ptr1->par1);
		free(ptr1->par2);
		free(ptr1->par3);
		free(ptr1);	
	}
	if (!commandRoot) free(commandRoot);

	return 0;
}

/**********
[function]	show executable commands
[parameter]	none
[return]	none
**********/
void help(){ 
	printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\nassemble filename\ntype filename\nsymbol\n");
	
	return;
}


/**********
[function]	print out fils and folders in current directory
[parameter] none	
[return]	none
**********/
int dir() {
	DIR *dir_info;
	struct dirent *dir_entry;
	struct stat dir_status;
	
	dir_info = opendir("."); // open curent directory

	if (!dir_info) {
		printf("failed to open directory!!!\n");
		return -1;
	}
	else {
		while((dir_entry = readdir(dir_info))) {
			lstat(dir_entry->d_name, &dir_status);
			/* if directory, put '/' at the end of the directory name */
			if (dir_entry->d_type == DT_DIR) {
				printf("%s/\n", dir_entry->d_name);
			}
			/* if executable file (X.out), put '*' at the end of the file name */
			else if ((dir_entry->d_type == DT_REG) && (S_IEXEC & dir_status.st_mode)) {
				printf("%s*\n", dir_entry->d_name);
			}
			/* else, put nothing but the file name */
			else {
				printf("%s\n", dir_entry->d_name);
			}
		}
	}
	
	closedir(dir_info);

	return 0;
}

/**********
[function]	quit sicsim	
[parameter]	flag(=0)
[return]	flag(=1)
**********/
int quit(int flag) {
	flag = 1;
	return flag;
}

/**********
[function]	print out history
[parameter]	none
[return]	none
**********/
void history() {
	int i;
	command *tmpC = commandRoot;
	for (i=1;i<=commandNum;i++) {
		/* print out command */
		printf("%d\t%s", tmpC->indexNum, tmpC->comm);
		/* if par1, par2, par3 != NULL, print out parameters */
		if (tmpC->par1) {
			printf(" %s", tmpC->par1);
			if (tmpC->par2) {
				printf(", %s", tmpC->par2);
			}
			if (tmpC->par3) {
				printf(", %s", tmpC->par3);
			}
		}
		printf("\n");
		/* search next linked list */
		tmpC = tmpC->next;
	}
}

/**********
[function]	implementation of pow function 
[parameter]	(1) integer (2) exponential
[return]	calculated value (x^expo)
**********/
int power(int x, int expo) {
	int result = 1;
	int i;
	for (i=0;i<expo;i++) result *= x;
	return result;
}

/**********
[function]	change decimal number to hexadecimal number
[parameter]	(1) decimal number (2) 16
[return]	hexadecimal number saved in character array
**********/
char* dtoh(int val, int base) {
	static char buf[32] = {0};
	int i = 30;
	if (val == 0) buf[i] = '0';
	for(; val && i ; --i, val /= base)
		buf[i] = "0123456789ABCDEF"[val % base];

	return &buf[i+1];	
}

/**********
[function]	change hexadecimal number to decimal number
[parameter]	(1) hexadecimal number 
[return]	decimal number
**********/
int htod(char* hex) {
	int decimal = 0;
	int len = strlen(hex);
	int i;
	char tmpC;
	int tmpd = 0;
	
	/* when hexadecimal address is negative number */
	if (hex[0] == '-') return MAX_MEMORY + 1;
	
	/* when address contains non hexadecimal number */
	for (i=0;i<len;i++) {
		if (('0' <= hex[i] && hex[i] <= '9') 
				|| ('A' <= hex[i] && hex[i] <= 'F') 
				|| ('a' <= hex[i] && hex[i] <= 'f')
				|| hex[i] == ' ') 
			continue;
		else return -1;
	}

	for (i=len-1;i>=0;i--) {
		tmpC = hex[i];
		switch(tmpC) {
			case '0': 
			case '1':	
			case '2':	
			case '3':	
			case '4':	
			case '5':	
			case '6': 
			case '7':
			case '8': 
			case '9':	
				tmpd = atoi(&tmpC); break;
			case 'a': 
			case 'A':
				tmpd = 10; break;
			case 'b': 
			case 'B':
				tmpd = 11; break;
			case 'c':
			case 'C':
				tmpd = 12; break;
			case 'd':
			case 'D':
				tmpd = 13; break;
			case 'e':
			case 'E': 
				tmpd = 14; break;
			case 'f':
			case 'F':
				tmpd = 15; break;
			break;
		}
		decimal += tmpd * power(16, len-i-1);
	}

	return decimal;
}

/**********
[function]	change address number to length 5
[parameter]	(1) address 
[return]	changed address (ex. changeAddr5("10") 10 -> 00010)
**********/
char* changeAddr5(char* addr) {
	static char buf[5] = {0};
	int i, len;
	len = strlen(addr);
	for (i=len;i>=0;--i) {
		buf[i+5-len] = addr[i];
	}
	for (i=0;i<5-len;i++) buf[i] = '0';
	return buf;
}

/**********
[function]	change address number to length 2
[parameter]	(1) address 
[return]	changed address (ex. changeAddr2("1") 1 -> 01)
**********/
char* changeAddr2(char* addr) {
	static char buf[2] = {0};
	int i, len;
	len = strlen(addr);
	for (i=len;i>=0;--i) {
		buf[i+2-len] = addr[i];
	}
	for (i=0;i<2-len;i++) buf[i] = '0';
	return buf;
}

/**********
[function]	check if start address and end address are valid
[parameter]	(1) start address (2) end address
[return]	if error -1, if normal 0
**********/
int checkMemErr(int start, int end) {
	
	/* when start address or end address contains non hexadecimal number */
	if (start == -1 || end == -1) {
		printf("start address and end address must be hexadecimal number!!!\n");
		return -1;
	}

	/* when start address and end address are both not valid */
	if ((start > MAX_MEMORY || start < 0) && (end > MAX_MEMORY || end < 0)) {
		printf("start address and end address both not in memory range!!!\n");
		return -1;
	}

	/* when start address is bigger than memory capacity  */
	if (start > MAX_MEMORY || start < 0) {
		printf("start address not in memory range!!!\n");
		return -1;
	}

	/* when end address is not valid */
	if (end > MAX_MEMORY || end < 0) {
		printf("end address not in memory range!!!\n");
		return -1;
	}

	/* when end address is smaller than start address */
	if (end < start) {
		printf("end address is smaller than start address!!!\n");
		return -1;
	}

	return 0;

}

/**********
[function]	check if given value is valid
[parameter]	(1) value
[return]	if error -1, if normal 0
**********/
int checkValErr(int value) {
	
	if (value < 0 || value > 255) {
		printf("value out of range!!!\n");
		return -1;
	}
	return 0;
}

/**********
[function]	print out memory 	
[parameter]	none
[return]	none
**********/
int dump() {
	int i, j;
	int start, end;
	int errflag;
	
	if (findLastCommand()->par1) start = htod(findLastCommand()->par1);
	else start = lastAddr;

	if (findLastCommand()->par2) end = htod(findLastCommand()->par2);
	else {
		end = start + 160 - 1;
		if (end > MAX_MEMORY) end = MAX_MEMORY;
	}

	// MAKE ERROR CASES!!!!!!!
	errflag = checkMemErr(start, end);
	if (errflag == -1) return -1;

	for (i=start;i<16*((end/16)+1);i+=MEMORY_COL) {
		/* print memory address */
		printf("%s ", changeAddr5(dtoh(i-i%16, 16)));
		
		/* print memory value */
		if (i==start) {
			/* case when searching first row */
			if (i/16 == end/16) {
			/* case when searching the first row is also the last row*/
				for (j=0;j<start%16;j++) printf("%s ", "  ");
				for (j=start%16;j<=end%16;j++) printf("%s ", changeAddr2(dtoh(memory[(i-i%16)/16][j], 16)));
				for (j=end%16+1;j<MEMORY_COL;j++) printf("%s ", "  ");
			}
			else {
				for (j=0;j<start%16;j++) printf("%s ", "  ");
				for (j=start%16;j<MEMORY_COL;j++) printf("%s ", changeAddr2(dtoh(memory[(i-i%16)/16][j], 16)));
			}
		}
		else if (i/16 == end/16) {
			/* case when searching last row */
			for (j=0;j<=end%16;j++) printf("%s ", changeAddr2(dtoh(memory[(i-i%16)/16][j], 16)));
			for (j=end%16+1;j<MEMORY_COL;j++) printf("%s ", "  ");
		}
		else {
			for (j=0;j<MEMORY_COL;j++)
				printf("%s ", changeAddr2(dtoh(memory[(i-i%16)/16][j], 16)));
		}
		
		/* divide section by printing semi-colon(;) */
		printf("%s", "; ");
		
		/* print ASCII code value */
		for (j=0;j<MEMORY_COL;j++) {
			if (32 <= memory[(i-i%16)/16][j] && memory[(i-i%16)/16][j] <= 126) 
					printf("%c", memory[(i-i%16)/16][j]);
			else printf("%c", '.');
		}
		
		/* change line */
		printf("\n");
	}
	lastAddr = end + 1;
	if (lastAddr > MAX_MEMORY) lastAddr = 0;

	return 0;
}

/**********
[function]	change value in memory
[parameter]	none
[return]	none
**********/
int edit(){
	int address, value, row = 0, col = 0;
	int errflag;
	command *commandptr;

	commandptr = findLastCommand();
	
	// MAKE ERROR CASE WHEN THERE'S NO ADDRESS OR VALUE GIVEN FROM THE USER (ex. edit, edit 2)
	/* when not enough parameters are given */
	if (!commandptr->par1 || !commandptr->par2) {
		printf("edit function needs address and value!!!\n");
		return -1;
	}

	address = htod(commandptr->par1);
	value = htod(commandptr->par2);
	
	// MAKE ERROR CASES!!!!!!!
	errflag = checkMemErr(address, MAX_MEMORY);
	if (errflag == -1) return -1;
	errflag = checkValErr(value);
	if (errflag == -1) return -1;

	col = address % 16;
	address -= col;
	while(address > 1) {
		row++;
		address -= 16;
	}

	memory[row][col] = value;

	return 0;	
}

/**********
[function]	set memory to 0
[parameter]	none	
[return]	none
**********/
int reset() {
	int i; 
	if (lastAddr == -1) {
		printf("No memory stored!!!");
		return -1;
	}
	for (i=0;i<=lastAddr;i++) {
		memory[i/16][i%16] = 0;
	}
	return 0;
}
/**********
[function]	fill memory with same value
[parameter] none
[return]	none
**********/
int fill() {
	int i, j;
	int Srow = 0, Scol = 0, Erow = 0, Ecol = 0;
	int start, end, value;
	int errflag;
	command *commandptr = findLastCommand();

	/* when not enough parameters are given */
	if (!commandptr->par1 || !commandptr->par2 || !commandptr->par3) {
		printf("fill function needs start, end address and value!!!\n");
		return -1;
	}
	start = htod(commandptr->par1);
	end = htod(commandptr->par2);
	value = htod(commandptr->par3);

	// MAKE ERROR CASES!!!!!!!
	errflag = checkMemErr(start, end);
	if (errflag == -1) return -1;
	errflag = checkValErr(value);
	if (errflag == -1) return -1;
	
	Scol = start % 16;
	start -= Scol;
	while(start > 1) {
		Srow++;
		start -= 16;
	}

	Ecol = end % 16;
	end -= Ecol;
	while(end > 1) {
		Erow++;
		end -= 16;
	}

	/* first row == last row */
	if (Srow == Erow) {
		for (j=Scol;j<=Ecol;j++) memory[Srow][j] = value;
	}
	else {
		for (j=Scol;j<MEMORY_COL;j++) memory[Srow][j] = value;
		for (i=Srow + 1;i<Erow;i++) {
			for (j=0;j<MEMORY_COL;j++) 
				memory[i][j] = value;
		}
		for (j=0;j<=Ecol;j++) memory[Erow][j] = value;
	}

	return 0;
}

/**********
[function]	generate key value of given data (hash function)
[parameter]	(1) mnemonic.mnem 	
[return]	hash key
**********/
int hashFunc(char* mnemonic) {
	int hashkey;
	// make hash key 
	// it is best to use all 20 storage in hash_table to get higher search speed
	mnemonic = strtok(mnemonic, "+");
	hashkey = (mnemonic[0]+(mnemonic[1]+1)*2)%20;	
	return hashkey;
}

/**********
[function]	read opcode.txt and save data
[parameter]	none	
[return]	none
**********/
int readFile() {
	FILE *fp;
	mnemonic *newM;
	int i, index, hashkey;
	char c;
	fp = fopen(FILENAME, "r");
	
	if (fp == NULL) {
		printf("failed to open opcode.txt!!!\n");
		return -1;
	}
	
	/* initialize hash_table */
	for (i=0;i<HASH_LENTH;i++) {
		hash_table[i] = NULL;
	}

	while(1) {
	
		/* memory allocation */
		newM = (mnemonic*)malloc(sizeof(mnemonic));
	
		/* initialize */
		newM->mnem[0] = '\0';
		newM->opcode = 0xFFFFFFFF; // -1
		newM->format = 0x00;	   // 0
		newM->formatXe = 0x00;	   // 0
		newM->next = NULL;
		
		/* read opcode */
		fscanf(fp, "%x", &newM->opcode);

		/* read mnemonic */
		do {							// delete spaces and tabs
			c = fgetc(fp);
		} while(c == ' ' || c == '\t');
		
		if (c == EOF) break;
		
		index = 0;
		for(;(c != ' ') && (c != '\t');c=fgetc(fp)) {
			newM->mnem[index++] = c;
		}
		newM->mnem[index] = '\0';

		/* read format(s) */
		fscanf(fp, "%d", &newM->format);
		c = fgetc(fp);
		if (c == '/') {
			fscanf(fp, "%d", &newM->formatXe); 
			c = fgetc(fp);	// read '\n'
		}
		
		/* generate hashkey and put it in the table */
		hashkey = hashFunc(newM->mnem);
		if (hashkey == -1) 
			printf("wrong mnemonic in opcode.txt!!!\n");
		else makeHashTable(newM, hashkey);

		/* free memory */
		//free(newM);
	}

	fclose(fp);
	return 0;
}


/**********
[function]	add data to hash table
[parameter]	(1) mnemonic (struct _mnemonic) (2) hashkey	
[return]	none
**********/
int makeHashTable(mnemonic *newM, int hashkey) {
	mnemonic *tmpM;
	
	if (hashkey >= HASH_LENTH) {
		printf("wrong hash key generated!!!\n");
		return -1;
	}
		
	tmpM = hash_table[hashkey];
	
	/* when hash table is empty */
	if (tmpM == NULL) 
		hash_table[hashkey] = newM;
	else {
		while(tmpM->next != NULL)
			tmpM = tmpM->next;
		tmpM->next = newM;
		newM->next = NULL;	// already initialized but init again to make sure it points to NULL
	}


	return 0;
}

/**********
[function]	find opcode for given mnemonic 
[parameter]	none
[return]	none
**********/
int opcode_mnemonic() {
	int index;
	mnemonic *tmpM;
	command *commandptr = findLastCommand();
	
	if (commandptr->par1 == NULL) {
		printf("opcode mnemonic function needs specific mnemonic code!!!\n");
		return -1;
	}
	index = hashFunc(commandptr->par1);
	tmpM = hash_table[index];
	
	if (tmpM != NULL) {
		while (strcmp(tmpM->mnem, commandptr->par1)) {
			tmpM = tmpM->next;
			if (tmpM == NULL) break;
		}
	}

	if (tmpM == NULL) {
		printf("mnemonic does not exist!!!\n");
		return -1;
	}
	else printf("opcode is %X\n", tmpM->opcode);

	return 0;
}

/**********
[function]	print out mnemonics and matching opcodes based on hash table 
[parameter]	none
[return]	none
**********/
int opcodelist() {
	mnemonic *tmpM;
	int i;

	for (i=0;i<HASH_LENTH;i++) {
		/* print index number of hash table */
		printf("%d : ", i);
		/* print all of the mnemonics stored in current index */
		tmpM = hash_table[i];
		if (tmpM != NULL) {
			do {
				printf("[%s,%X]", tmpM->mnem, tmpM->opcode);
				if (tmpM->next != NULL) printf(" -> ");
				tmpM = tmpM->next;
			} while(tmpM != NULL);
		}
		/* change line */
		printf("\n");
	}
	return 0;
}

int initInst(inst *newInst) {
	newInst->lineNum = 0;
	newInst->loc = -1;
	newInst->symbol= " ";
	newInst->mnem = " ";
	newInst->var1 = " ";
	newInst->var2 = " ";
	newInst->objcode = -1;
	newInst->prev = NULL;
	newInst->next = NULL;
	
	return 0;
}

int makeInstList() {
	command *commandptr = findLastCommand();
	FILE *fp = fopen(commandptr->par1, "r");
	char line[MAX_LINE];
	char *ptr;
	inst *newInst, *prevInst;
	int start_line = 5;

	if (fp == NULL) {
		printf("failed to open file!!!\n");
		return -1;
	}
	/* when previous instruction file exists in the linked list */
	if (instRoot != NULL) {
		//inst *delLL = instRoot;
		/*
		while(delLL != NULL) {
			free(delLL);
			delLL = delLL->next;
		}*/
		instRoot = NULL; 
	}

	while(1) {
		if (fgets(line, sizeof(line), fp) == NULL) break;
		else {
			if (line[0] == '\n') continue;

			newInst = (inst*)malloc(sizeof(inst));
			initInst(newInst);
			/* case for comments */
			if (line[0] == '.') { 
				ptr = strtok(line, "\n");
				newInst->symbol = (char*)malloc(sizeof(char)*strlen(ptr)+1);
				strcpy(newInst->symbol, ptr);
			}
			/* case for instructions */
			else {
				/* case for instruction lines with symbol*/
				if (line[0] != ' ' && line[0] != '\t') {
					ptr = strtok(line, "\t ");
					if (ptr!=NULL) {
						newInst->symbol = (char*)malloc(sizeof(char)*strlen(ptr)+1);
						strcpy(newInst->symbol, ptr);
					}

					ptr = strtok(NULL, "\t \n");	
					newInst->mnem = (char*)malloc(sizeof(char)*strlen(ptr)+1);
					strcpy(newInst->mnem, ptr);
				}
				/* case for instruction lines without symbol */
				else {
					ptr = strtok(line, "\t \n");
					newInst->mnem = (char*)malloc(sizeof(char)*strlen(ptr)+1);
					strcpy(newInst->mnem, ptr);
				}
				ptr = strtok(NULL, "\t ,\n");
				if (ptr!=NULL) {
					newInst->var1 = (char*)malloc(sizeof(char)*strlen(ptr)+1);
					strcpy(newInst->var1, ptr);
				}
					
				ptr = strtok(NULL, "\t \n");
				if (ptr!=NULL) {
					newInst->var2 = (char*)malloc(sizeof(char)*strlen(ptr)+1);
					strcpy(newInst->var2, ptr);
				}					
			
			}
			
			/* calculate line number */
			newInst->lineNum = start_line;
			start_line += 5;

			/* save instructions as linked list structure */
			if (instRoot == NULL) instRoot = newInst;
			else {
				prevInst->next = newInst;
				newInst->prev = prevInst;
			}
			prevInst = newInst;
		}
	}
	
	fclose(fp);

	return 0;
}


int checkMnem(char *mnem) {
	if (!strcmp(mnem, "WORD") || !strcmp(mnem, "BYTE"))
		return 1;
	else if (!strcmp(mnem, "RESW") || !strcmp(mnem, "RESB"))
		return 2;
	else {
		/* error check */
		mnemonic *tmpm = hash_table[hashFunc(mnem)];
		char *newm = strtok(mnem, "+");
		if (tmpm == NULL) return -1;
		while(strcmp(newm, tmpm->mnem)) {
			if (tmpm->next == NULL) return -1;
			tmpm = tmpm->next;
		}
		return 3;
	}
}

int checkConstSize(char *mnem, char *con) {
	int onesize, count;
	if (!strcmp(mnem, "BYTE")) onesize = 1;
	else if (!strcmp(mnem, "WORD")) onesize = 3;
	
	if (con[0] == 'c' || con[0] == 'C') 
		count = strlen(con) - 3;
	else if (con[0] == 'x' || con[0] == 'X') {
		count = strlen(con) - 3;
		count = (count/2) + (count%2);
	}
	else count = 1;

	return onesize*count;
}

int checkVarSize(char *mnem, char *var) {
	int onesize, count;
	if (!strcmp(mnem, "RESB")) onesize = 1;
	else if (!strcmp(mnem, "RESW")) onesize = 3;

	count = atoi(var);
	
	return onesize*count;
}

// or 'pass1'
int locSet() {
	inst *tmpinst = instRoot;
	int startloc, disp, errflag = 0, finalerror = 0;
	mnemonic *tmpM;
	
	if (tmpinst == NULL) {
		printf("no assembly instructions!!!\n");
		return -1;
	}

	if (!strcmp(tmpinst->mnem, "START")) {
		startloc = htod(tmpinst->var1); 
		startaddr = startloc;
	}
	else startloc = 0;

	do {
		errflag = 0;
		/* case for comments */
		if (tmpinst->symbol[0] == '.')
				tmpinst->loc = -1;
		/* case for assembler directive */
		else if (!strcmp(tmpinst->mnem, "BASE")) 
			tmpinst->loc = -1;
		else if (!strcmp(tmpinst->mnem, "END")) {
			tmpinst->loc = startloc;
			endaddr = startloc;
		}
		/* case for non-assember directive */
		else if (!strcmp(tmpinst->mnem, "START")) {
			tmpinst->loc = startloc;
		}
		else {
			switch(checkMnem(tmpinst->mnem)) {
				/* case for constant */
				case 1:
					disp = checkConstSize(tmpinst->mnem, tmpinst->var1);
					break;
				/* case for variable */
				case 2:
					disp = checkVarSize(tmpinst->mnem, tmpinst->var1);
					break;
				/* case for mnemonic */
				case 3:
					/* format 4 */
					if (tmpinst->mnem[0] == '+')
						disp = 4;
					/* format 1,2,3 */
					else {
						tmpM = hash_table[hashFunc(tmpinst->mnem)];
						/* error check */
						if (tmpM == NULL) {
							printf("Error in line %d :: mnemonic does not exist\n", tmpinst->lineNum);
							errflag = finalerror = -1;
						}
						else {
							while(strcmp(tmpM->mnem, tmpinst->mnem)) {
								if (tmpM->next == NULL) {
									printf("Error in line %d :: mnemonic does not exist\n", tmpinst->lineNum);
									errflag = finalerror = -1;
									break;
								}
								tmpM = tmpM->next;
							}
						}
						if (errflag == -1) disp = 0;
						else disp = tmpM->format;
					}
					break;
				case -1:
					printf("Error in line %d :: mnemonic does not exist\n", tmpinst->lineNum);
					errflag = finalerror = -1;
					disp = 0;
					break;
			} 
			tmpinst->loc = startloc;
			startloc += disp;
		}
		tmpinst = tmpinst->next;
	} while (tmpinst != NULL);
	
	if (finalerror == -1) return -1;

	return 0;
}

int symHashFunc(char* symbol) {
	int hashkey;
	
	symbol = strtok(symbol, "#@");
	hashkey = ((int)symbol[0] - ((int)'A')) % 26;
	return hashkey;
}


int makeSymHash() {
	inst *tmpinst = instRoot;
	sym *newsym, *tmps, *x;
	int hashkey, i, errflag = 0, finalerror = 0;
	
	/* initialize symbol_hash */
	for (i=0;i<26;i++) symbol_hash[i] = NULL;

	while(tmpinst != NULL) {
		errflag = 0;
		if (tmpinst->symbol[0] != ' ' && tmpinst->symbol[0] != '.' && strcmp(tmpinst->symbol, "COPY")) {  
			hashkey = symHashFunc(tmpinst->symbol);
			tmps = symbol_hash[hashkey];
			
			/* error check - symbol already exists */
			for (x=symbol_hash[hashkey];x!=NULL;x=x->next) {
				if (!strcmp(x->symbol, tmpinst->symbol)) {
					printf("Error in line %d :: same symbol already exists\n", tmpinst->lineNum);
					errflag = finalerror = -1;
				}
			}
			if (errflag == -1) {
				tmpinst = tmpinst->next;
				continue;
			}

			newsym = (sym*)malloc(sizeof(sym));
			newsym->loc = tmpinst->loc; 
			newsym->symbol = tmpinst->symbol;
			newsym->visit = 0;
			newsym->prev = newsym->next = NULL;
			
			/* when hash table is empty */
			if (tmps == NULL) 
				symbol_hash[hashkey] = newsym;
			else {
				while(tmps->next != NULL) 
					tmps = tmps->next;
				tmps->next = newsym;
				newsym->next = NULL;	
			}

		}

		tmpinst = tmpinst->next;
	}
		
	if (errflag == -1) return -1;

	return 0;
}

int findSymLoc(inst* tmpinst) {
	int errflag = 0, hashkey;
	char symbol[20];
	strcpy(symbol, strtok(tmpinst->var1, "#@"));
	hashkey = symHashFunc(symbol);
	sym *tmps = symbol_hash[hashkey];
	
	if (tmps == NULL) {
		printf("Error in line %d :: symbol does not exist\n", tmpinst->lineNum);
		errflag = -1;
	}
	else {
		while(strcmp(tmps->symbol, symbol)) {
			if (tmps->next == NULL) {
				printf("Error in line %d :: symbol does not exist\n", tmpinst->lineNum);
				errflag = -1;
				tmps = tmps->next;
				break;
			}
			tmps = tmps->next;
		} 
	}
	if (errflag == -1) return -1;

	return tmps->loc;
}


int regnum(char *r) {
	if (!strcmp(r, "A") || r[0] == ' ') return 0;
	else if (!strcmp(r, "X")) return 1;
	else if (!strcmp(r, "L")) return 2;
	else if (!strcmp(r, "B")) return 3;
	else if (!strcmp(r, "S")) return 4;
	else if (!strcmp(r, "T")) return 5;
	else if (!strcmp(r, "F")) return 6;
	else if (!strcmp(r, "PC")) return 8;
	else if (!strcmp(r, "SW")) return 9;
	
	return -1;
}
	

int getObjCode(inst *tmpinst, int format, int n, int i, int x, int b, int p, int e, int disp, int compflag) {
	int objcode = 0;
	int r1, r2;
	char* normalizemnem = strtok(tmpinst->mnem, "+");
	mnemonic *tmpM = hash_table[hashFunc(tmpinst->mnem)];
	
	while(strcmp(tmpM->mnem, normalizemnem)) tmpM = tmpM->next;

	if (compflag) {
		/* 2's complement */
		disp = power(16, format) - disp;
	}

	switch(format) {
	case 1:
		objcode = tmpM->opcode;
		break;
	case 2:
		r1 = regnum(tmpinst->var1);
		if (tmpinst->var2 != NULL) r2 = regnum(tmpinst->var2);
		else r2 = 0; 
		objcode = tmpM->opcode*power(2, 8) + r1*power(16, 1) + r2;
		break;
	case 3:
		objcode = (tmpM->opcode/4)*power(2, 18) + n*power(2, 17) + i*power(2,16) + x*power(2,15) + b*power(2,14) + p*power(2,13) + e*power(2,12) + disp;
		break;
	case 4:
		objcode = (tmpM->opcode/4)*power(2, 26) + n*power(2, 25) + i*power(2,24) + x*power(2,23) + b*power(2,22) + p*power(2,21) + e*power(2,20) + disp;
		break;
	}


	return objcode;
}

// or pass2
int objcodeSet() {
	inst *tmpinst = instRoot;
	inst *tmppc;
	mnemonic *tmpmnem;
	char *normalizemnem = (char*)malloc(sizeof(char)*30);
	int n, i, x, b, p, e, disp, format, base, pc, ta, compflag, index;
	int finalerror = 0;
	
	if (tmpinst == NULL) {
		printf("no assembly instructions!!!\n");
		return -1;
	}
		
	while (tmpinst != NULL) {
		compflag = 0;
		if (!strcmp(tmpinst->mnem, "BASE")) {
			// don't need to create object code
			base = findSymLoc(tmpinst);
			if (base == -1) {
				finalerror =  -1; 
				tmpinst = tmpinst->next;
				continue;
			}
			tmpinst->objcode = -1;
		}
		else if (!strcmp(tmpinst->mnem, "START") || !strcmp(tmpinst->mnem, "END") || tmpinst->symbol[0] == '.') {
			// don't need to create object code
			tmpinst->objcode = -1;
		}
		else if (!strcmp(tmpinst->mnem, "RESW") ||
				!strcmp(tmpinst->mnem, "RESB")) {
			// don't need to create object code
			tmpinst->objcode = -1;
		}
		else if (!strcmp(tmpinst->mnem, "WORD") || !strcmp(tmpinst->mnem, "BYTE")) {
			disp = 0;
			if (tmpinst->var1[0] == 'C') {
				strcpy(normalizemnem, tmpinst->var1);
				normalizemnem = strtok(normalizemnem, "C'");
				// case when length is longer than 4
				if (checkConstSize(tmpinst->mnem, tmpinst->var1) > 4) {
					tmpinst->tmpobj = (char*)malloc(sizeof(char)*(checkConstSize(tmpinst->mnem, tmpinst->var1+1)));
					strcpy(tmpinst->tmpobj,normalizemnem);
					tmpinst->objcode = 0;
				}
				else {
					for (index = 0;index<strlen(normalizemnem);index++) {
						disp += (int)(normalizemnem[index])*power(16, 2*( strlen(normalizemnem)-1-index));
					}
					tmpinst->objcode = disp;
				}
			}
			else if (tmpinst->var1[0] == 'X') {
				strcpy(normalizemnem, tmpinst->var1);
				normalizemnem = strtok(normalizemnem, "X'");
				tmpinst->objcode = htod(normalizemnem);
			}
		}
		else {
			tmpmnem = hash_table[hashFunc(tmpinst->mnem)];
			strcpy(normalizemnem, tmpinst->mnem);
			normalizemnem = strtok(normalizemnem, "+");
			while (strcmp(tmpmnem->mnem, normalizemnem)) 
				tmpmnem = tmpmnem->next;

			/* checking format */
			if (tmpinst->mnem[0] == '+') {
				e = 1; format = 4;
			}
			else {
				e = 0; format = tmpmnem->format;
			}

			/* checking addressing method */
			if (tmpinst->var1[0] == '#') {
				n = 0; i = 1;
			}
			else if (tmpinst->var1[0] == '@') {
				n = 1; i = 0;
			}
			else {
				n = 1; i = 1;
			}

			/* cheking usage of resgister X */
			if (tmpinst->var1[0] == 'X' || tmpinst->var2[0] == 'X')
				x = 1;
			else x = 0;

			/* make Target Address */
			if (tmpinst->var1[0] == '#' && '0' <= tmpinst->var1[1] && tmpinst->var1[1] <= '9') {
				b = 0; p = 0;
				disp = atoi(strtok(tmpinst->var1, "#"));
			}
			else if (format == 4) {
				b = 0; p = 0;
				disp = findSymLoc(tmpinst);
				if (disp == -1) {
					finalerror = -1;
					tmpinst = tmpinst->next;
					continue;
				}
			}
			else if (format == 2) {
				b = 0; p = 0; disp = 0;
			}
			else if (tmpinst->var1[0] == ' ') {
				b = 0; p = 0; disp = 0;
			}
			else {
				tmppc = tmpinst->next;
				while (tmppc->loc == -1) tmppc = tmppc->next;
				pc = tmppc->loc;
				ta = findSymLoc(tmpinst);
				if (ta == -1) {
					finalerror = -1; 
					tmpinst = tmpinst->next;
					continue;
				}
				if (pc > ta) {
					disp = pc - ta;
					compflag = 1; // 2's complement
				}
				else {
					disp = ta - pc;
					compflag = 0;
				}
				
				if (disp > 0x1000) {
					b = 1; p = 0;
					disp = ta - base;
					compflag = 0;
				}
				else {
					b = 0; p = 1;
				}
			}
			tmpinst->objcode = getObjCode(tmpinst, format, n, i, x, b, p, e, disp, compflag);
		}
		tmpinst = tmpinst->next;
	}
	//free(normalizemnem);
	if (finalerror == -1) return -1;
	return 0;
}

int getFormat(char *mnem) {
	char* newm = strtok(mnem, "+");
	mnemonic* tmpm = hash_table[hashFunc(newm)];

	if (tmpm == NULL) return -1;
	while(strcmp(newm, tmpm->mnem)) {
		if (tmpm->next==NULL) return -1;
		tmpm = tmpm->next;
	}

	return tmpm->format;
}

char* itoa(int value, char* result, int base) {
	char* ptr = result, *ptr1 = result, tmp_char;
	int tmp_value;

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"[35+(tmp_value - value*base)];
	} while(value);

	if (tmp_value < 0) *ptr++ = '-';
	*ptr-- = '\0';
	while(ptr1<ptr) {
		tmp_char = *ptr;
		*ptr--=*ptr1;
		*ptr1++ = tmp_char;
	}
	return result;
}

/**********
[function]  read given file and show 	
[parameter]	none
[return]	none
**********/
int assemble() {
	FILE *fp;
	inst *tmpinst;
	command *tmpC = findLastCommand();
	mnemonic *tmpm;
	char *newfile1 = (char*)malloc(sizeof(char)*30);
	char *newfile2 = (char*)malloc(sizeof(char)*30);
	char *vars = (char*)malloc(sizeof(char)*30);
	int size, i, errflag = 0, finalerror = 0;

	/* fill location and object code for each instruction */
	errflag = makeInstList();
	if (errflag == -1) return -1;

	errflag = locSet();
	if (errflag == -1) finalerror = -1;

	errflag = makeSymHash();
	if (errflag == -1 || finalerror == -1) return -1;
	
	errflag = objcodeSet();
	if (errflag == -1) return -1;
	
	/* make lst file */
	strcpy(newfile1, tmpC->par1);
	newfile1 = strtok(newfile1, ".");
	strcat(newfile1, ".lst");

	fp = fopen(newfile1, "w");
	if (fp == NULL) {
		printf("failed to open file!!!\n");
		return -1;
	}

	tmpinst = instRoot;
	while(tmpinst != NULL) {
		fprintf(fp, "%-10d", tmpinst->lineNum);
		if (tmpinst->loc != -1 && strcmp(tmpinst->mnem, "END")) fprintf(fp, "%04X\t\t", tmpinst->loc);
		else fprintf(fp, "%4s\t\t", " ");
		if (tmpinst->symbol[0] != ' ') fprintf(fp, "%-30s", tmpinst->symbol);
		else fprintf(fp, "%-30s", " ");
		if (tmpinst->mnem[0] != ' ') fprintf(fp, "%-30s", tmpinst->mnem);
		else fprintf(fp, "%-30s", " ");
		if (tmpinst->var1[0] != ' ') {
			strcpy(vars, tmpinst->var1);
			if (tmpinst->var2[0] != ' ') {
				strcat(vars, ", ");
				strcat(vars, tmpinst->var2);
			}
			fprintf(fp, "%-30s", vars);
		}
		else fprintf(fp, "%-30s", " ");
		if (tmpinst->objcode != -1) {
			if (!strcmp(tmpinst->mnem, "BYTE") || !strcmp(tmpinst->mnem, "WORD")) {
				if (tmpinst->var1[0] == 'C') {
					// case when the string length is longer than 4
					if (checkConstSize(tmpinst->mnem, tmpinst->var1) > 4) {
						for (i=0;i<strlen(tmpinst->tmpobj);i++)
							fprintf(fp,"%X",(int)(tmpinst->tmpobj[i]));
					}
					// case when the string length is equal to or shorter than 4
					else fprintf(fp, "%X", tmpinst->objcode);		
				}
				else if (tmpinst->var1[0] == 'X') {
					size =  checkConstSize(tmpinst->mnem, tmpinst->var1); 
					for (i=0;i<2*size-strlen(dtoh(tmpinst->objcode, 16));i++)	
						fprintf(fp, "%c", '0');
					fprintf(fp, "%X", tmpinst->objcode);
				}
			}
			else if (tmpinst->mnem[0] == '+')
				fprintf(fp, "%08X", tmpinst->objcode);
			else {
				tmpm = hash_table[hashFunc(tmpinst->mnem)];
				while(strcmp(tmpm->mnem, strtok(tmpinst->mnem, "+"))) tmpm = tmpm->next;
				if (tmpm->format == 1)
					fprintf(fp, "%02X", tmpinst->objcode);
				else if (tmpm->format == 2)
					fprintf(fp, "%04X", tmpinst->objcode);
				else if (tmpm->format == 3)
					fprintf(fp, "%06X", tmpinst->objcode);
			}
		}
		fprintf(fp, "\n");

		tmpinst = tmpinst->next;
	}
	fclose(fp);
	free(vars);

	/* make obj file */
	strcpy(newfile2, tmpC->par1);
	newfile2 = strtok(newfile2, ".");
	strcat(newfile2, ".obj");

	fp = fopen(newfile2, "w");
	if (fp == NULL) {
		printf("failed to open file!!!\n");
		return -1;
	}
	
	int len = 0, startflag = 1, enterflag = 0, increase = 0;
	char objs[300];
	char buff[30];
	inst* tmpindex;

	objs[0] = '\0';
	buff[0] = '\0';

	tmpinst = instRoot;
	while (strcmp(tmpinst->mnem, "START")) tmpinst = tmpinst->next;
	fprintf(fp, "H%-6s%06X%06X\n", tmpinst->symbol, startaddr, endaddr-startaddr);	
	
	tmpinst = tmpinst->next;
	while(tmpinst != NULL) {
		if (!strcmp(tmpinst->mnem, "END")) {
			fprintf(fp, "%02X%s\n", len/2, objs);
			for (tmpindex = instRoot; tmpindex!=NULL;tmpindex=tmpindex->next) {
				itoa(tmpindex->objcode, buff, 16);
				if (!strncmp(buff, "4B1", 3))
					fprintf(fp, "M%06X%02X\n", tmpindex->loc + 1, 5);
			}
			fprintf(fp, "E%06X\n", startaddr);
			objs[0] = '\0';
			break;
		}

		if (startflag) {
			fprintf(fp, "T%06X", tmpinst->loc);
			startflag = 0; 
		}

		if (!strcmp(tmpinst->mnem, "RESW") || !strcmp(tmpinst->mnem, "RESB")) {
			enterflag = 1; 
		}
		else if (tmpinst->symbol[0] != '.' && strcmp(tmpinst->mnem, "BASE")) {
			if (!strcmp(tmpinst->mnem, "WORD") || !strcmp(tmpinst->mnem, "BYTE")) {
				if (tmpinst->var1[0] == 'C') {
					itoa(tmpinst->objcode, buff, 16);		
					increase = strlen(buff);
				}
				else if (tmpinst->var1[0] == 'X') {
					increase = 2*checkConstSize(tmpinst->mnem, tmpinst->var1); 
					itoa(tmpinst->objcode, buff, 16);		
				}
			}
			else {
				switch(getFormat(tmpinst->mnem)){
					case 1: break;
					case 2:
						increase = 4; break;
					case 3:
						increase = 6; break;
					case 4:
						increase = 8; break;
					case -1: break;
				}
				if (tmpinst->mnem[0] == '+') increase = 8;
				itoa(tmpinst->objcode, buff, 16);
			}
			if (len + increase <= 30*2 && !enterflag) {
				len += increase;
				for(i=0;i<increase-strlen(buff);i++) strcat(objs, "0");
				strcat(objs, buff); 
				buff[0] = '\0';
			}
			else {
				// print out objs and len!
				fprintf(fp, "%02X%s", len/2, objs);
				fprintf(fp, "\n"); startflag = 1; len = 0; enterflag = 0;
				objs[0] = '\0'; buff[0] = '\0';
				tmpinst = tmpinst->prev;	
			}
		}
		tmpinst = tmpinst->next;
	}


	printf("Successfully assemble %s\n", tmpC->par1);


	fclose(fp);
	free(newfile1);
	free(newfile2);
	return 0;
}

/**********
[function]  read given file and show 	
[parameter]	none
[return]	none
**********/
int type() {
	FILE *fp;
	char readC;
	command *commandptr = findLastCommand();
	
	fp = fopen(commandptr->par1, "r");
	/* error when filename does not exist */	
	if (fp == NULL) {
		printf("file does not exeist!!!\n");
		return -1;
	}
	while(1) {
		readC = fgetc(fp);
		if (readC == EOF) break;
		else printf("%c", readC);
	} 
	

	fclose(fp);
	return 0;	
}


/**********
[function]  read given file and show 	
[parameter]	none
[return]	none
**********/
int symbol() {
	sym *tmps, *index1, *min;
	int i, nullcount = 0, count, n;

	for (i=0;i<26;i++) {
		tmps = symbol_hash[i];
		if (tmps == NULL) {
			nullcount++; 
			if (nullcount >= 26) {
				printf("symbol hash table not made yet!!!\n");
				return -1;
			}
			continue;
		}
		count = 0; n = 0;
		while (tmps!=NULL) {
			n++;
			tmps = tmps->next;
		}
		while (count<n) {
			min = NULL; tmps = symbol_hash[i];
			for(index1=tmps;index1!=NULL;index1=index1->next) {
				if (min == NULL) {
					while(tmps->visit == 1) tmps = tmps->next;
					min = tmps;
				}
				if (strcmp(min->symbol, index1->symbol) > 0 && index1->visit == 0) min = index1;
			}
			printf("%s\t%04X\n", min->symbol, min->loc);
			min->visit = 1; count++;
		}
	}

	for (i=0;i<26;i++) {
		tmps = symbol_hash[i];
		while(tmps!=NULL) {
			tmps->visit = 0;
			tmps = tmps->next;
		}
	}

	return 0;
}


/* MAIN FUNCTION */
int main() {
	int flag = 0, errflag;
	/* read opcode.txt */
	readFile();

	while(flag == 0) {
		errflag = 0;
		switch(mode()) {
			case 1:
				addCommand(code);
				help();
				break;
			case 2:
				addCommand(code);
				dir();
				break;
			case 3:
				addCommand(code);
				flag = quit(flag);
				freeLinkedList();
				break;
			case 4:
				addCommand(code);
				history();
				break;
			case 5:
				addCommand(code);
				errflag = dump();
				if (errflag == -1) delLastCommand();
				break;
			case 6:
				addCommand(code);
				errflag = edit();
				if (errflag == -1) delLastCommand();
				break;
			case 7: 
				addCommand(code);
				errflag = fill();
				if (errflag == -1) delLastCommand();
				break;
			case 8:
				addCommand(code);
				reset();
				break;
			case 9:
				addCommand(code);
				errflag = opcode_mnemonic();
				if (errflag == -1) delLastCommand();
				break;
			case 10:
				addCommand(code);
				opcodelist();
				break;
			case 11:
				addCommand(code);
				errflag = assemble();
				if (errflag == -1) delLastCommand();
				break;
			case 12:
				addCommand(code);
				errflag = type();
				if (errflag == -1) delLastCommand();
				break;
			case 13:
				addCommand(code);
				symbol();
				break;
			case 0:
				// error
				break;
		}
	}
	free(code);
	return 0;
}

