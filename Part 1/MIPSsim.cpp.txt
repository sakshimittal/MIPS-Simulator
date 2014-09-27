/*On my honor, I have neither given nor received unauthorized aid on this assignment*/

#include "iostream"
#include "fstream"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
int last_address = 128;
int flag = 0;
int R[32];

typedef struct inst
{
	int address;
	char *op;
	char inp[33];
	int field1, field2, field3;
	struct inst *next;
}inst, *instptr;

instptr list_begin = NULL;

typedef struct mem
{
	int address;
	int num;
	char inp[33];
	struct mem *next;
}mem, *memptr;

memptr mem_list = NULL;


void convertDecimalToBinary2(char *temp,int num);
int convertDecimalToBinary(int num);
int convertBinaryToDecimal(int num);
int convertBinaryToDecimal2(char* num);
void display_List(instptr list_begin);
int find_value_at(char *read_inst);
memptr insert(memptr mem_list,int num, int add, char *line);
void display_mem(memptr mem_list);


class Category1
{
public:
	char* line;
	int rs;
	int rt;
	int offset;
	char* instruction;

	Category1(char *read_inst,int add)
	{
		line = read_inst;
		fetch_opcode();
		fetch_rest(add);
	}

	void fetch_opcode()
	{
		char temp[4];
		int j=0;
		for(int i=3; i<=5; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';
		
		if(strcmp(temp, "000") == 0)
		{
			instruction = "J";
		}
		else if(strcmp(temp, "010") == 0)
		{
			instruction = "BEQ";
		}
		else if(strcmp(temp, "100") == 0)
		{
			instruction = "BGTZ";
		}
		else if(strcmp(temp, "101") == 0)
		{
			instruction = "BREAK";
			flag = 1;
		}
		else if(strcmp(temp, "110") == 0)
		{
			instruction = "SW";
		}
		else if(strcmp(temp, "111") == 0)
		{
			instruction = "LW";
		}
	}

	void fetch_rest(int add)
	{
		if(strcmp(instruction,"J") == 0)
		{
			char temp[33], temp1[33], temp2[33], str[33], str1[33];
			int j=0, k=0, l=0;
			for(int i=6; i<=31; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j] = '\0';
			
			k = convertBinaryToDecimal2(temp);
			k = k<<2;
			memset(temp1,'\0',33);
			convertDecimalToBinary2(temp1,k);
			
			memset(temp, '\0', 33);
			k=0;
			k = 28 - strlen(temp1);
			for(int n=0; n<k; n++)
				strcat(temp,"0");
			
			strcat(temp, temp1); 
			
			memset(str1, '\0', 33);
			convertDecimalToBinary2(str1,add);

			memset(str, '\0', 33);
			memset(temp2, '\0', 33);
			k=0;
			k = 32 - strlen(str1);
			for(int n=0; n<k; n++)
				strcat(str,"0");
			strcat(str, str1);

			strncpy(temp2,str,4);
			
			memset(str, '\0', 33);
			strcat(str,temp2);
			strcat(str,temp);
			
			offset = 0;
			offset = convertBinaryToDecimal2(str);
		}

		else if(strcmp(instruction,"BEQ") == 0)
		{
			char temp[33], temp1[33], temp2[33], str[33], str1[33];
			int j=0, k=0, l=0;

			/* rs */

			for(int i=6; i<=10; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j]='\0';
			j=atoi(temp);
			rs = 0;
			rs = convertBinaryToDecimal(j);

			/* rt */

			j=0;
			for(int i=11; i<=15; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j]='\0';
			j=atoi(temp);
			rt = 0;
			rt = convertBinaryToDecimal(j);

			/*offset */

			j = 0;
			for(int i=16; i<=31; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j] = '\0';

			k = convertBinaryToDecimal2(temp);
			k = k<<2;
			memset(temp1,'\0',33);
			if(line[16] == '1')
			{
				k = find_value_at(temp);
				k = k*(-1);
				convertDecimalToBinary2(temp1,k);
			}
			else
			{
				convertDecimalToBinary2(temp1,k);
			}
			
			memset(temp, '\0', 33);
			k=0;
			k = 18 - strlen(temp1);
			for(int n=0; n<k; n++)
				strcat(temp,"0");
			
			strcat(temp, temp1); 
			
			memset(str1, '\0', 33);
			convertDecimalToBinary2(str1,add);

			memset(str, '\0', 33);
			memset(temp2, '\0', 33);
			k=0;
			k = 32 - strlen(str1);
			for(int n=0; n<k; n++)
				strcat(str,"0");
			strcat(str, str1);

			strncpy(temp2,str,14);
			
			memset(str, '\0', 33);
			strcat(str,temp2);
			strcat(str,temp);
			
			offset = 0;
			offset = convertBinaryToDecimal2(str);

			if(line[16] == '1')
			{
				offset = offset * (-1);
			}
		}

		else if(strcmp(instruction,"BGTZ") == 0)
		{
			char temp[33], temp1[33], temp2[33], str[33], str1[33];
			int j=0, k=0, l=0;

			/* rs */

			for(int i=6; i<=10; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j]='\0';
			j=atoi(temp);
			rs = 0;
			rs = convertBinaryToDecimal(j);

			/*offset */

			j = 0;
			for(int i=16; i<=31; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j] = '\0';

			k = convertBinaryToDecimal2(temp);
			k = k<<2;
			memset(temp1,'\0',33);
			
			if(line[16] == '1')
			{
				k = find_value_at(temp);
				k = k*(-1);
				convertDecimalToBinary2(temp1,k);
			}
			else
			{
				convertDecimalToBinary2(temp1,k);
			}
			
			memset(temp, '\0', 33);
			k=0;
			k = 18 - strlen(temp1);
			for(int n=0; n<k; n++)
				strcat(temp,"0");
			
			strcat(temp, temp1); 
			
			memset(str1, '\0', 33);
			convertDecimalToBinary2(str1,add);

			memset(str, '\0', 33);
			memset(temp2, '\0', 33);
			k=0;
			k = 32 - strlen(str1);
			for(int n=0; n<k; n++)
				strcat(str,"0");
			strcat(str, str1);

			strncpy(temp2,str,14);
			
			memset(str, '\0', 33);
			strcat(str,temp2);
			strcat(str,temp);
			
			offset = 0;
			offset = convertBinaryToDecimal2(str);

			if(line[16] == '1')
			{
				offset = offset * (-1);
			}
		}
		
		else if(strcmp(instruction,"BREAK") == 0)
		{
			//do nothing
		}

		else if(strcmp(instruction,"SW") == 0 || strcmp(instruction,"LW") == 0)
		{
			char temp[33];
			int j=0, k=0;

			/* base */

			for(int i=6; i<=10; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j]='\0';
			j=atoi(temp);
			rs = 0;
			rs = convertBinaryToDecimal(j);

			/* rt */

			j=0;
			for(int i=11; i<=15; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j]='\0';
			j=atoi(temp);
			rt = 0;
			rt = convertBinaryToDecimal(j);

			/*offset */

			j = 0;
			for(int i=16; i<=31; i++)
			{
				temp[j] = line[i];
				j++;
			}
			temp[j] = '\0';

			offset = convertBinaryToDecimal2(temp);	
		}
	}

	instptr addTo_list(instptr list_begin)
	{
		int add;
		instptr p, temp;
		p = (instptr)malloc(sizeof(inst));
		p->op = instruction;
		strcpy(p->inp,line);
		p->next = NULL;

		if(list_begin == NULL)
		{
			p->address = 128;
			
			if(strcmp(instruction, "J") == 0)
			{
				p->field1 = -1;
				p->field2 = -1;
				p->field3 = offset;
			}

			else if(strcmp(instruction, "BGTZ") == 0)
			{
				p->field1 = rs;
				p->field2 = -1;
				p->field3 = offset;
			}

			else if(strcmp(instruction, "BEQ") == 0 || strcmp(instruction, "SW") == 0 || strcmp(instruction, "LW") == 0)
			{
				p->field1 = rs;
				p->field2 = rt;
				p->field3 = offset;
			}

			else if(strcmp(instruction, "BREAK") == 0)
			{
				p->field1 = -1;
				p->field2 = -1;
				p->field3 = -1;
			}
			
			list_begin = p;
		}

		else
		{
			temp = list_begin;
			while(temp->next != NULL)
				temp = temp->next;
		
			add = temp->address;
			p->address = add + 4;

			if(strcmp(instruction, "J") == 0)
			{
				p->field1 = -1;
				p->field2 = -1;
				p->field3 = offset;
			}

			else if(strcmp(instruction, "BGTZ") == 0)
			{
				p->field1 = rs;
				p->field2 = -1;
				p->field3 = offset;
			}

			else if(strcmp(instruction, "BEQ") == 0 || strcmp(instruction, "SW") == 0 || strcmp(instruction, "LW") == 0)
			{
				p->field1 = rs;
				p->field2 = rt;
				p->field3 = offset;
			}

			else if(strcmp(instruction, "BREAK") == 0)
			{
				p->field1 = -1;
				p->field2 = -1;
				p->field3 = -1;
			}

			temp->next = p;
			last_address = p->address;
		}

		return list_begin;
	}
};

class Category2
{
public:
	char* line;
	int rs;
	int rt;
	int rd;
	char* instruction;
	
	Category2(char *read_inst)
	{
		line = read_inst;
		fetch_rs();
		fetch_rt();
		fetch_opcode();
		fetch_rd();
	}

	void fetch_rs()
	{
		char temp[6];
		int j=0,pw=1;

		for(int i=3; i<=7; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';

		j=atoi(temp);
		
		rs = 0;
		rs = convertBinaryToDecimal(j);
		
		//sprintf(rs,"%d",k);
	}

	void fetch_rt()
	{
		char temp[6];
		int j=0,pw=1;

		for(int i=8; i<=12; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';

		j=atoi(temp);
		
		rt = 0;
		rt = convertBinaryToDecimal(j);
	}

	void fetch_opcode()
	{
		char temp[4];
		int j=0;
		for(int i=13; i<=15; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';
		
		if(strcmp(temp, "000") == 0)
		{
			instruction = "ADD";
		}
		else if(strcmp(temp, "001") == 0)
		{
			instruction = "SUB";
		}
		else if(strcmp(temp, "010") == 0)
		{
			instruction = "MUL";
		}
		else if(strcmp(temp, "011") == 0)
		{
			instruction = "AND";
		}
		else if(strcmp(temp, "100") == 0)
		{
			instruction = "OR";
		}
		else if(strcmp(temp, "101") == 0)
		{
			instruction = "XOR";
		}
		else if(strcmp(temp, "110") == 0)
		{
			instruction = "NOR";
		}
	}

	void fetch_rd()
	{
		char temp[6];
		int j=0,pw=1;

		for(int i=16; i<=20; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';

		j=atoi(temp);
		
		rd = 0;
		rd = convertBinaryToDecimal(j);
	}

	instptr addTo_list(instptr list_begin)
	{
		int add;
		instptr p, temp;
		p = (instptr)malloc(sizeof(inst));
		p->op = instruction;
		strcpy(p->inp,line);
		p->field1 = rs;
		p->field2 = rt;
		p->field3 = rd;
		p->next = NULL;

		if(list_begin == NULL)
		{
			p->address = 128; 
			list_begin = p;
		}
		else
		{
			temp = list_begin;
			while(temp->next != NULL)
				temp = temp->next;
		
			add = temp->address;
			p->address = add + 4;
			temp->next = p;
			last_address = p->address;
		}

		return list_begin;
	}
};

class Category3
{
public:
	char* line;
	int rs;
	int rt;
	int imm_val;
	char* instruction;
	
	Category3(char *read_inst)
	{
		line = read_inst;
		fetch_rs();
		fetch_rt();
		fetch_opcode();
		fetch_imm();
	}

	void fetch_rs()
	{
		char temp[6];
		int j=0,pw=1;

		for(int i=3; i<=7; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';

		j=atoi(temp);
		
		rs = 0;
		rs = convertBinaryToDecimal(j);
 	}

	void fetch_rt()
	{
		char temp[6];
		int j=0,pw=1;

		for(int i=8; i<=12; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';

		j=atoi(temp);
		
		rt = 0;
		rt = convertBinaryToDecimal(j);
	}

	void fetch_opcode()
	{
		char temp[4];
		int j=0;
		for(int i=13; i<=15; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';
		
		if(strcmp(temp, "000") == 0)
		{
			instruction = "ADDI";
		}
		else if(strcmp(temp, "001") == 0)
		{
			instruction = "ANDI";
		}
		else if(strcmp(temp, "010") == 0)
		{
			instruction = "ORI";
		}
		else if(strcmp(temp, "011") == 0)
		{
			instruction = "XORI";
		}
	}

	void fetch_imm()
	{
		char temp[33];
		int j=0,pw=1;

		for(int i=16; i<=31; i++)
		{
			temp[j]=line[i];
			j++;
		}
		temp[j]='\0';

		imm_val = 0;

		imm_val = find_value_at(temp);
	}

	instptr addTo_list(instptr list_begin)
	{
		int add;
		instptr p, temp;
		
		p = (instptr)malloc(sizeof(inst));
		p->op = instruction;
		strcpy(p->inp,line);
		p->field1 = rs;
		p->field2 = rt;
		p->field3 = imm_val;
		p->next = NULL;

		if(list_begin == NULL)
		{
			p->address = 128; 
			list_begin = p;
		}
		else
		{
			temp = list_begin;
			while(temp->next != NULL)
				temp = temp->next;
		
			add = temp->address;
			p->address = add + 4;
			temp->next = p;
			last_address = p->address;
		}
		return list_begin;
	}
};

int main(int argc, char *argv[])
{
	char buffer[33], temp[4], read_inst[33];
	char a[33];
	int i = 0,j = 0,m,add,val,cycle = 1;
	FILE *fp, *fp1, *fp2;
	instptr p, p1, jump;
	memptr q;

	if(argc == 2)
	{
		fp = fopen(argv[1],"r");
	}
	else
	{
		cout<<"\nInput File name not entered!";
	}
	

	while((buffer[i] = fgetc(fp)) != EOF)
	{
		read_inst[j]=buffer[i];
		j++;
		if(buffer[i] == '\n' || buffer[i] == '\0')
		{
			read_inst[32]='\0';

			for(m=0; m<=2; m++)
				temp[m] = read_inst[m];

			temp[m]='\0';
			
			if(flag == 1)
			{
				val = find_value_at(read_inst);
				mem_list = insert(mem_list, val, last_address, read_inst);
			}

			else
			{
				if(strcmp(temp,"000") == 0)
				{
					if(list_begin == NULL)
					{
						add = 128;
					}
					else
					{
						add = last_address + 8;
					}
				
					Category1 *c1 = new Category1(read_inst,add);
					list_begin = c1->addTo_list(list_begin);
				}
		
				else if(strcmp(temp,"110") == 0)
				{
					Category2 *c2 = new Category2(read_inst);
					list_begin = c2->addTo_list(list_begin);
				}

				else if(strcmp(temp,"111") == 0)
				{
					Category3 *c3 = new Category3(read_inst);
					list_begin = c3->addTo_list(list_begin);
				}
			}

			j=0;
			i=0;
		}
		else
			i++;
	}

	read_inst[32]='\0';
		
	if(flag == 1)
	{
		val = find_value_at(read_inst);
		mem_list = insert(mem_list, val, last_address, read_inst);
	}

	fclose(fp);

	fp1 = fopen("disassembly.txt", "w");
	if(fp1 != NULL)
	{
		p = list_begin;
		while(p != NULL)
		{
			fputs(p->inp,fp1);
			fputs("\t",fp1);
			memset(a,'\0',33);
			sprintf(a, "%d", p->address);
			fputs(a,fp1);
			fputs("\t",fp1);
			fputs(p->op,fp1);

			if(p->op == "ADD" || p->op == "SUB" || p->op == "MUL" || p->op == "AND" || p->op == "OR" || p->op == "XOR" || p->op == "NOR")
			{
				fputs(" ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field3);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs(", ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field1);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs(", ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field2);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs("\n",fp1);
			}

			else if(p->op == "ADDI" || p->op == "ANDI" || p->op == "ORI" || p->op == "XORI")
			{
				fputs(" ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field2);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs(", ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field1);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs(", ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field3);
				fputs("#",fp1);
				fputs(a,fp1);
				fputs("\n",fp1);
			}

			else if(p->op == "J")
			{
				fputs(" #",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field3);
				fputs(a,fp1);
				fputs("\n",fp1);
			}

			else if(p->op == "BEQ")
			{
				fputs(" ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field1);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs(", ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field2);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs(", ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field3);
				fputs("#",fp1);
				fputs(a,fp1);
				fputs("\n",fp1);
			}

			else if(p->op == "BGTZ")
			{
				fputs(" ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field1);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs(", ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field3);
				fputs("#",fp1);
				fputs(a,fp1);
				fputs("\n",fp1);
			}

			else if(p->op == "BREAK")
			{
				fputs("\n",fp1);
			}

			else if(p->op == "SW" || p->op == "LW")
			{
				fputs(" ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field2);
				fputs("R",fp1);
				fputs(a,fp1);
				fputs(", ",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field3);
				fputs(a,fp1);
				fputs("(R",fp1);
				memset(a,'\0',33); 
				sprintf(a, "%d", p->field1);
				fputs(a,fp1);
				fputs(")",fp1);
				fputs("\n",fp1);
			}

			p = p->next;
		}

		q = mem_list;
		while(q != NULL)
		{
			if(q->next == NULL)
			{
				fputs(q->inp,fp1);
				fputs("\t",fp1);
				memset(a,'\0',33);
				sprintf(a, "%d", q->address);
				fputs(a,fp1);
				fputs("\t",fp1);
				memset(a,'\0',33);
				sprintf(a, "%d", q->num);
				fputs(a,fp1);
			}

			else
			{
				fputs(q->inp,fp1);
				fputs("\t",fp1);
				memset(a,'\0',33);
				sprintf(a, "%d", q->address);
				fputs(a,fp1);
				fputs("\t",fp1);
				memset(a,'\0',33);
				sprintf(a, "%d", q->num);
				fputs(a,fp1);
				fputs("\n",fp1);
			}

			q = q->next;
		}
		
		fclose(fp1);
	}

	for(int k=0; k<32; k++)
	{
		R[k] = 0;
	}

	add = 128;
	p = list_begin;
	flag = 0;

	fp2 = fopen("simulation.txt" ,"w");
	if(fp2 != NULL)
	{
		while(p != NULL)
		{
			p1 = p;
			fputs("--------------------",fp2);
			fputs("\nCycle:",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",cycle);
			fputs(a,fp2);
			fputs("\t",fp2);
			memset(a,'\0',33);
			sprintf(a, "%d", p->address);
			fputs(a,fp2);
			fputs("\t",fp2);
			fputs(p->op,fp2);

			if(p->op == "J")
			{
				add = p->field3;
				jump = list_begin;
				while(jump->address != add)
					jump = jump->next;

				p = jump;
			}
			else if(p->op == "BEQ")
			{
				if(R[p->field1] == R[p->field2])
				{
					add = p->address + p->field3 + 4;
					jump = list_begin;
					while(jump->address != add)
						jump = jump->next;

					p = jump;
				}
				else
					p = p->next;
			}
			else if(p->op == "BGTZ")
			{
				if(R[p->field1] > 0)
				{
					add = p->address + p->field3 + 4;
					jump = list_begin;
					while(jump->address != add)
						jump = jump->next;

					p = jump;
				}
				else
					p = p->next;
			}
			else if(p->op == "BREAK")
			{
				p = p->next;
			}
			else if(p->op == "SW")
			{
				add = 0;
				add = p->field3 + R[p->field1];
				q = mem_list;
				while(q->next!=NULL && q->address != add)
					q = q->next;
				if(q==NULL)
					;
				else
				{
					q->num = R[p->field2]; 
					p = p->next;
				}
			}
			else if(p->op == "LW")
			{
				add = p->field3 + R[p->field1];
				q = mem_list;
				while(q->address != add)
					q = q->next;
				R[p->field2] = q->num;
				p = p->next;
			}
			else if(p->op == "ADD")
			{
				R[p->field3] = R[p->field1] + R[p->field2];
				p = p->next;
			}
			else if(p->op == "SUB")
			{
				R[p->field3] = R[p->field1] - R[p->field2];
				p = p->next;
			}
			else if(p->op == "MUL")
			{
				R[p->field3] = R[p->field1] * R[p->field2];
				p = p->next;
			}
			else if(p->op == "AND")
			{
				R[p->field3] = R[p->field1] & R[p->field2];
				p = p->next;
			}
			else if(p->op == "OR")
			{
				R[p->field3] = R[p->field1] | R[p->field2];
				p = p->next;
			}
			else if(p->op == "XOR")
			{
				R[p->field3] = R[p->field1] ^ R[p->field2];
				p = p->next;
			}
			else if(p->op == "NOR")
			{
				R[p->field3] = ~(R[p->field1] | R[p->field2]);
				p = p->next;
			}
			else if(p->op == "ADDI")
			{
				R[p->field2] = R[p->field1] + p->field3;
				p = p->next;
			}
			else if(p->op == "ANDI")
			{
				R[p->field2] = R[p->field1] & p->field3;
				p = p->next;
			}
			else if(p->op == "ORI")
			{
				R[p->field2] = R[p->field1] | p->field3;
				p = p->next;
			}
			else if(p->op == "XORI")
			{
				R[p->field2] = R[p->field1] ^ p->field3;
				p = p->next;
			}

			if(p1->op == "ADD" || p1->op == "SUB" || p1->op == "MUL" || p1->op == "AND" || p1->op == "OR" || p1->op == "XOR" || p1->op == "NOR")
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field3);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs("\n",fp2);
			}

			else if(p1->op == "ADDI" || p1->op == "ANDI" || p1->op == "ORI" || p1->op == "XORI")
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("\n",fp2);
			}

			else if(p1->op == "J")
			{
				fputs(" #",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field3);
				fputs(a,fp2);
				fputs("\n",fp2);
			}

			else if(p1->op == "BEQ")
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("\n",fp2);
			}

			else if(p1->op == "BGTZ")
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("\n",fp2);
			}

			else if(p1->op == "BREAK")
			{
				fputs("\n",fp2);
				flag = 1;
			}

			else if(p1->op == "SW" || p1->op == "LW")
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field3);
				fputs(a,fp2);
				fputs("(R",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", p1->field1);
				fputs(a,fp2);
				fputs(")",fp2);
				fputs("\n",fp2);
			}

			fputs("\n",fp2);
			fputs("Registers",fp2);
			fputs("\n",fp2);
			fputs("R00:",fp2);
		
			for(int n=0;n<=7;n++)
			{
				fputs("\t",fp2);
				memset(a,'\0',33);
				sprintf(a,"%d",R[n]);
				fputs(a,fp2);
			}

			fputs("\n",fp2);
			fputs("R08:",fp2);
			for(int n=8;n<=15;n++)
			{
				fputs("\t",fp2);
				memset(a,'\0',33);
				sprintf(a,"%d",R[n]);
				fputs(a,fp2);
			}

			fputs("\n",fp2);
			fputs("R16:",fp2);
			for(int n=16;n<=23;n++)
			{
				fputs("\t",fp2);
				memset(a,'\0',33);
				sprintf(a,"%d",R[n]);
				fputs(a,fp2);
			}

			fputs("\n",fp2);
			fputs("R24:",fp2);
			for(int n=24;n<=31;n++)
			{
				fputs("\t",fp2);
				memset(a,'\0',33);
				sprintf(a,"%d",R[n]);
				fputs(a,fp2);
			}

			fputs("\n\n",fp2);
			fputs("Data",fp2);

			q = mem_list;
			add = q->address;
			i = 0;
			
			while(q != NULL)
			{
				if(i == 0)
				{
					fputs("\n",fp2);
					memset(a,'\0',33);
					sprintf(a,"%d",q->address);
					fputs(a,fp2);
					fputs(":",fp2);
					fputs("\t",fp2);
					memset(a,'\0',33);
					sprintf(a,"%d",q->num);
					fputs(a,fp2);
					i++;
				}
				else
				{
					fputs("\t",fp2);
					memset(a,'\0',33);
					sprintf(a,"%d",q->num);
					fputs(a,fp2);
					if(i!=7)
						i++;
					else
						i=0;
				}
				q = q->next;
			}

			if(flag == 0)
				fputs("\n\n",fp2);

	
			cycle++;
		}
	}

	fclose(fp2);
	
	//display_List(list_begin);
	//display_mem(mem_list);
	return 0;
}


void convertDecimalToBinary2(char *temp1, int num)
{
	char temp[33];
	int j=0;
	memset(temp,'\0',33);
	while(num>0)
	{
		if(num%2 == 1)
			strcat(temp,"1");
		else
			strcat(temp,"0");
		
		num = num / 2;
	}	

	strcat(temp,"\0");
	for(int i=strlen(temp)-1;i>=0;i--)
	{
		temp1[j] = temp[i];
		j++;
	}
}

int convertDecimalToBinary(int num)
{
	int k=0, m=1;
	while(num>0)
	{
		k = k + (num%2) * m;
		num = num / 2;
		m = m * 10;
	}	

	return k;
}

int convertBinaryToDecimal(int num)
{
	int k=0, pw=1;
	while(num>0)
	{
		if((num%10) == 1)
			k = k + pw;
				
		num = num/10;
		pw = pw*2;
	}

	return k;
}

int convertBinaryToDecimal2(char* num)
{
	int temp=0, pw=1;

	for(int i=strlen(num)-1;i>=0;i--)
		{
			if(num[i] == '1')
				temp = temp + pw;

			pw = pw * 2;
		}

	return temp;
}

void display_List(instptr list_begin)
{
	if(list_begin == NULL)
		return;

	instptr p = list_begin;
	cout<<p->inp<<"\t"<<p->address<<"\t"<<p->op<<"\t"<<p->field1<<"\t"<<p->field2<<"\t"<<p->field3<<endl;
	p = p->next;
	while(p!= NULL)
	{
		cout<<p->inp<<"\t"<<p->address<<"\t"<<p->op<<"\t"<<p->field1<<"\t"<<p->field2<<"\t"<<p->field3<<endl;
		p = p->next;
	}
}

int find_value_at(char *read_inst)
{
	if(read_inst[0] == '1')
	{
		char temp[33];
		int add[31], temp1[31], temp2[31];
		int i=0, carry, k, pw, num,l=0;;
		for(i=1; i<strlen(read_inst); i++)
		{
			temp[l] = read_inst[i];
			l++;
		}
		temp[l] = '\0';
		//cout<<endl<<"temp\t"<<temp;
		i=0;
		for(i=0; i<strlen(read_inst)-1; i++)
		{
			if(temp[i] == '1')
			{
				temp1[i] = 0;
			}
			else
			{
				temp1[i] = 1;
			}
		}

		i=0;
		for(i=0; i<strlen(read_inst)-2; i++)
		{
			temp2[i] = 0;
		}
		temp2[i] = 1;

		carry = 0;
		for(i=strlen(read_inst)-2; i>=0; i--)
		{
			add[i] = (temp1[i] ^ temp2[i]) ^ carry;
			carry = ((temp1[i] & temp2[i]) | (temp1[i] & carry) | (temp2[i] & carry));
		}

		pw=1;
		num=0;
		for(int k=strlen(read_inst)-2;k>=0;k--)
		{
			if(add[k] == 1)
				num = num + pw;

			pw = pw * 2;
		}
		//cout<<endl<<num<<endl;

		return (-1*num);
	}

	else
	{
		int k = convertBinaryToDecimal2(read_inst);
		return k;
	}
}

memptr insert(memptr mem_list, int num, int add, char *read_inst)
{
	memptr p, temp;
	int add1;
	p = (memptr)malloc(sizeof(mem));
	p->num = num;
	strcpy(p->inp,read_inst);
	p->next = NULL;

	if(mem_list == NULL)
	{
		p->address = add + 4; 
		mem_list = p;
	}
	else
	{
		temp = mem_list;
		while(temp->next != NULL)
			temp = temp->next;
		
		add1 = temp->address;
		p->address = add1 + 4;
		temp->next = p;
	}

	return mem_list;
}

void display_mem(memptr mem_list)
{
	if(mem_list == NULL)
		return;

	memptr p = mem_list;
	cout<<p->inp<<"\t"<<p->address<<"\t"<<p->num<<endl;
	p = p->next;
	while(p!= NULL)
	{
		cout<<p->inp<<"\t"<<p->address<<"\t"<<p->num<<endl;
		p = p->next;
	}
}