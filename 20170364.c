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
	for (i;i<strlen(code);i++) {
		if ('a' <= code[i] && code[i] <= 'f') {
			code[i] = code[i] - UptoLow;
		}
	}

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
	printf("h[elp]\nd[ir]\nq[uit]\nhi[story]\ndu[mp] [start, end]\ne[dit] address, value\nf[ill] start, end, value\nreset\nopcode mnemonic\nopcodelist\n");
	
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
			case 0:
				// error
				break;
		}
	}
	free(code);
	return 0;
}

