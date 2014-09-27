/*On my honor, I have neither given nor received unauthorized aid on this assignment*/

#include "iostream"
#include "fstream"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int last_address = 128;
int PC = 128;
int flag = 0;
int pre_issue_count = 0;
int pre_alu_count = 0;
int current_cycle = 0;
//int flag1 = 0;
int R[32];
bool ready[32] = {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true};
bool ready1[32] = {true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true,true};

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

typedef struct queue
{
	char op[10];
	int field1, field2, field3;
	int cycle;
	struct queue *next;
}queue, *queueptr;

queueptr pre_issue_queue = NULL;
queueptr waiting_inst = NULL;
queueptr executed_inst = NULL;
queueptr pre_alu = NULL;

typedef struct dummy
{
	
	char op[10];
	int field1, field2, field3;
	int cycle;
}dummy, *dummyptr;

dummyptr dummy_head = NULL;
dummyptr temp_head = NULL;
dummyptr temp_head1 = NULL;

typedef struct result_queue
{
	char op[10];
	int field1, field2, field3;
	int result;
	int reg;
	int cycle;
}result_queue, *resqueueptr;

resqueueptr prev_post_alu = NULL;
resqueueptr next_post_alu = NULL;
resqueueptr prev_post_mem = NULL;
resqueueptr next_post_mem = NULL;
resqueueptr prev_pre_mem = NULL;
resqueueptr next_pre_mem = NULL;

void convertDecimalToBinary2(char *temp,int num);
int convertDecimalToBinary(int num);
int convertBinaryToDecimal(int num);
int convertBinaryToDecimal2(char* num);
void display_List(instptr list_begin);
int find_value_at(char *read_inst);
memptr insert(memptr mem_list,int num, int add, char *line);
void display_mem(memptr mem_list);
bool fetch();
void issue();
void alu();
bool scoreboard(queueptr x,int temp,int num);
void memory();
void write_back();

queueptr insert_into_queue(queueptr q, instptr p)
{
	queueptr y;
	queueptr x = q;

	if(q == NULL)
	{
		y = (queueptr)malloc(sizeof(queue));
		strcpy(y->op,p->op);
		y->field1 = p->field1;
		y->field2 = p->field2;
		y->field3 = p->field3;
		y->cycle = current_cycle;
		y->next = NULL;
		q = y;
		return q;
	}

	while(x->next != NULL)
		x = x->next;

	y = (queueptr)malloc(sizeof(queue));
	strcpy(y->op,p->op);
	y->field1 = p->field1;
	y->field2 = p->field2;
	y->field3 = p->field3;
	y->cycle = current_cycle;
	y->next = NULL;
	x->next = y;
	return q;
}

queueptr insert_into_queue(queueptr q, queueptr p)
{
	queueptr y;
	queueptr x = q;

	if(q == NULL)
	{
		y = (queueptr)malloc(sizeof(queue));
		strcpy(y->op,p->op);
		y->field1 = p->field1;
		y->field2 = p->field2;
		y->field3 = p->field3;
		y->cycle = current_cycle;
		y->next = NULL;
		q = y;
		return q;
	}

	while(x->next != NULL)
		x = x->next;

	y = (queueptr)malloc(sizeof(queue));
	strcpy(y->op,p->op);
	y->field1 = p->field1;
	y->field2 = p->field2;
	y->field3 = p->field3;
	y->cycle = current_cycle;
	y->next = NULL;
	x->next = y;
	return q;
}

queueptr remove_from_queue(queueptr q, int index)
{
	int i = 0;
	queueptr y;
	queueptr x = q;

	if(q == NULL)
		return NULL;

	if(index == 0)
	{
		q = q->next;
		return q;
	}
	
	y = x;
	x = x->next;
	i++;
	while(i != index)
	{
		y = x;
		x = x->next;
		i++;
	}

	y->next = x->next;
	return q;
}


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

bool fetch()
{
	instptr p = list_begin;
	int num = 0;
	while(num < 2 && pre_issue_count < 4)
	{
		while(p->address != PC)
			p = p->next;

		if(waiting_inst != NULL && strcmp(waiting_inst->op,"BGTZ") == 0)
		{
			if(ready[waiting_inst->field1] == true && ready1[waiting_inst->field1] == true)
			{
				if(R[waiting_inst->field1] > 0)
					PC = PC + waiting_inst->field3 + 4;
				else
					PC = PC + 4;
				executed_inst = insert_into_queue(executed_inst,waiting_inst);
				waiting_inst = remove_from_queue(waiting_inst, 0);
			}
			return true;
		}
		else if(waiting_inst != NULL && strcmp(waiting_inst->op,"BEQ") == 0)
		{
			if(ready[waiting_inst->field1] == true && ready[waiting_inst->field2] == true && ready1[waiting_inst->field1] == true && ready1[waiting_inst->field2] == true)
			{
				if(R[waiting_inst->field1] == R[waiting_inst->field2])
					PC = PC + waiting_inst->field3 + 4;
				else
					PC = PC + 4;
				executed_inst = insert_into_queue(executed_inst,waiting_inst);
				waiting_inst = remove_from_queue(waiting_inst, 0);
			}
			return true;
		}
		else if(p->op == "BREAK")
		{
			if(executed_inst != NULL)
				executed_inst = NULL;
			executed_inst = insert_into_queue(executed_inst,p);
			return false;
		}
		else
		{
			if(executed_inst!=NULL)
			{
				executed_inst = remove_from_queue(executed_inst,0);
			}

			if(p->op == "BGTZ")
			{
				if(ready[p->field1] == true && ready1[p->field1] == true)
				{
					if(R[p->field1] > 0)
						PC = PC + p->field3 + 4;
					else
						PC = PC + 4;
					executed_inst = insert_into_queue(executed_inst,p);
				}
				else
					waiting_inst = insert_into_queue(waiting_inst,p);

				num++;
			}
			else if(p->op == "BEQ")
			{
				if(ready[p->field1] == true && ready[p->field2] == true && ready1[p->field1] == true && ready1[p->field2] == true)
				{
					if(R[p->field1] == R[p->field2])
						PC = PC + p->field3 + 4;
					else
						PC = PC + 4;
					executed_inst = insert_into_queue(executed_inst,p);
				}
				else
					waiting_inst = insert_into_queue(waiting_inst,p);

				num++;
			}
			else if(p->op == "J")
			{
				PC = p->field3;
				executed_inst = insert_into_queue(executed_inst,p);
				num++;
			}
			else
			{
				pre_issue_queue = insert_into_queue(pre_issue_queue,p);
				pre_issue_count++;
				PC = PC + 4;
			}

			if(p->op == "LW" || p->op == "ADDI" || p->op == "ANDI" || p->op == "ORI" || p->op == "XORI")
			{
				if(p->field1 == p->field2 && ready[p->field1] == true)
				{
					ready1[p->field2] = false;
				//	flag1 = 1;
				}
				else
					ready[p->field2] = false;
			}
			else if(p->op == "ADD" || p->op == "SUB" || p->op == "MUL" || p->op == "AND" || p->op == "OR" || p->op == "XOR" || p->op == "NOR")
			{
				if((p->field1 == p->field3 && ready[p->field1] == true) || (p->field2 == p->field3 && ready[p->field2] == true))
				{
					ready1[p->field3] = false;
				//	flag1 = 1;
				}
				else
					ready[p->field3] = false;
			}

			num++;
			//cout<<"NUM = "<<num<<" "<<"Count = "<<pre_issue_count<<endl;
		}
	}
	return true;
}

void issue()
{
	int rdy, temp = 0, num = 0;
	queueptr x = pre_issue_queue;

	if(temp_head != NULL)
		temp_head = NULL;

	if(temp_head1 != NULL)
		temp_head1 = NULL;

	if(dummy_head == NULL)
		;
	else if(dummy_head->cycle == 2)
		dummy_head = NULL;
	else
		dummy_head->cycle = dummy_head->cycle + 1;

	while(num < 2 && pre_alu_count < 2)
	{
		if(x != NULL && x->cycle < current_cycle)
		{
			rdy = scoreboard(x,temp,num);
			if(rdy)
			{
				if(ready1[x->field2] == false || ready1[x->field3] == false)
				{
					if(!strcmp(x->op,"LW") || !strcmp(x->op,"ADDI") || !strcmp(x->op,"ANDI") || !strcmp(x->op,"ORI") || !strcmp(x->op,"XORI"))
					{
						ready1[x->field2] = true;
						ready[x->field2] = false;
					}
					else if(!strcmp(x->op,"ADD") || !strcmp(x->op,"SUB") || !strcmp(x->op,"MUL") || !strcmp(x->op,"AND") || !strcmp(x->op,"OR") || !strcmp(x->op,"XOR") || !strcmp(x->op,"NOR"))
					{
						ready1[x->field3] = true;
						ready[x->field3] = false;
					}
					//flag1 = 0;
				}

				if(strcmp(x->op, "LW") == 0)
				{
					dummyptr t = (dummyptr)malloc(sizeof(dummy));
					strcpy(t->op,x->op);
					t->field1 = x->field1;
					t->field2 = x->field2;
					t->field3 = x->field3;
					t->cycle = 0;
					dummy_head = t;
				}

				pre_alu = insert_into_queue(pre_alu,x);
				pre_issue_queue = remove_from_queue(pre_issue_queue,temp);
				temp--;
				pre_alu_count++;
				pre_issue_count--;
				num++;
				if(num == 1 && x->op == "LW")
				{
					dummyptr s = (dummyptr)malloc(sizeof(dummy));
					strcpy(s->op,x->op);
					s->field1 = x->field1;
					s->field2 = x->field2;
					s->field3 = x->field3;
					temp_head = s;	
				}
				if(num == 1)
				{
					dummyptr u = (dummyptr)malloc(sizeof(dummy));
					strcpy(u->op,x->op);
					u->field1 = x->field1;
					u->field2 = x->field2;
					u->field3 = x->field3;
					temp_head1 = u;
				}
			}
			x = x->next;
			temp++;
		}
		else
			break;
	}
}

bool scoreboard(queueptr x, int temp, int num)
{
	//operands ready or not
	if(!strcmp(x->op,"SW") || !strcmp(x->op,"ADD") || !strcmp(x->op,"SUB") || !strcmp(x->op,"MUL") || !strcmp(x->op,"AND") || !strcmp(x->op,"OR") || !strcmp(x->op,"XOR") || !strcmp(x->op,"NOR"))
	{
		if(!(ready[x->field1] == true && ready[x->field2] == true))
			return false;
	}
	else if(!strcmp(x->op,"LW") || !strcmp(x->op,"ADDI") || !strcmp(x->op,"ANDI") || !strcmp(x->op,"ORI") || !strcmp(x->op,"XORI"))
	{
		if(ready[x->field1] == false)
			return false;
	}

	//WAW with active instructions
	int destination = -1;
	if(!strcmp(x->op,"ADDI") || !strcmp(x->op,"ANDI") || !strcmp(x->op,"ORI") || !strcmp(x->op,"XORI"))
		destination = x->field2;
	else if(!strcmp(x->op,"ADD") || !strcmp(x->op,"SUB") || !strcmp(x->op,"MUL") || !strcmp(x->op,"AND") || !strcmp(x->op,"OR") || !strcmp(x->op,"XOR") || !strcmp(x->op,"NOR"))
		destination = x->field3;

	if(dummy_head != NULL)
	{
		if(dummy_head->field2 == destination)
			return false;
	}
	
	int i = 0;
	queueptr q = pre_issue_queue;
	while(i != temp)
	{
		if(!strcmp(q->op,"LW") || !strcmp(q->op,"ADDI") || !strcmp(q->op,"ANDI") || !strcmp(q->op,"ORI") || !strcmp(q->op,"XORI"))
		{
			if(q->field2 == destination)
				return false;
			else
			{
				q = q->next;
				i++;
			}
		}
		else if(!strcmp(q->op,"ADD") || !strcmp(q->op,"SUB") || !strcmp(q->op,"MUL") || !strcmp(q->op,"AND") || !strcmp(q->op,"OR") || !strcmp(q->op,"XOR") || !strcmp(q->op,"NOR"))
		{
			if(q->field3 == destination)
				return false;
			else
			{
				q = q->next;
				i++;
			}
		}
		else
		{
			q = q->next;
			i++;
		}
	}

	//WAR
	i = 0;
	q = pre_issue_queue;
	while(i != temp)
	{
		if(!strcmp(q->op,"SW"))
		{
			if(q->field1 == destination || q->field2 == destination)
				return false;
			else
			{
				q = q->next;
				i++;
			}
		}
		else if(!strcmp(q->op,"LW"))
		{
			if(q->field1 == destination)
				return false;
			else
			{
				q = q->next;
				i++;
			}
		}
		else if(!strcmp(q->op,"ADDI") || !strcmp(q->op,"ANDI") || !strcmp(q->op,"ORI") || !strcmp(q->op,"XORI"))
		{
			if(q->field1 == destination)
				return false;
			else
			{
				q = q->next;
				i++;
			}
		}
		else if(!strcmp(q->op,"ADD") || !strcmp(q->op,"SUB") || !strcmp(q->op,"MUL") || !strcmp(q->op,"AND") || !strcmp(q->op,"OR") || !strcmp(q->op,"XOR") || !strcmp(q->op,"NOR"))
		{
			if(q->field1 == destination || q->field2 == destination)
				return false;
			else
			{
				q = q->next;
				i++;
			}
		}
		else
		{
			q = q->next;
			i++;
		}
	}

	if(temp_head != NULL)
	{
		if(!strcmp(x->op,"ADDI") || !strcmp(x->op,"ANDI") || !strcmp(x->op,"ORI") || !strcmp(x->op,"XORI"))
			destination = x->field2;
		else if(!strcmp(x->op,"ADD") || !strcmp(x->op,"SUB") || !strcmp(x->op,"MUL") || !strcmp(x->op,"AND") || !strcmp(x->op,"OR") || !strcmp(x->op,"XOR") || !strcmp(x->op,"NOR"))
			destination = x->field3;
	
		if(temp_head->field2 == destination)
			return false;
	}

	if(num == 1 && temp_head1 != NULL)
	{
		if(!strcmp(x->op,"LW") || !strcmp(x->op,"ADDI") || !strcmp(x->op,"ANDI") || !strcmp(x->op,"ORI") || !strcmp(x->op,"XORI"))
		{
			if(x->field1 == x->field2 && ready[x->field1] == true)
			{
				if(!strcmp(temp_head1->op,"SW") || !strcmp(temp_head1->op,"ADD") || !strcmp(temp_head1->op,"SUB") || !strcmp(temp_head1->op,"MUL") || !strcmp(temp_head1->op,"AND") || !strcmp(temp_head1->op,"OR") || !strcmp(temp_head1->op,"XOR") || !strcmp(temp_head1->op,"NOR"))
				{
					if(temp_head1->field1 == x->field1 || temp_head1->field2 == x->field1)
						return false;
				}
				else if(!strcmp(temp_head1->op,"LW") || !strcmp(temp_head1->op,"ADDI") || !strcmp(temp_head1->op,"ANDI") || !strcmp(temp_head1->op,"ORI") || !strcmp(temp_head1->op,"XORI"))
				{
					if(temp_head1->field1 == x->field1)
						return false;
				}
			}
		}
		else if(!strcmp(x->op,"ADD") || !strcmp(x->op,"SUB") || !strcmp(x->op,"MUL") || !strcmp(x->op,"AND") || !strcmp(x->op,"OR") || !strcmp(x->op,"XOR") || !strcmp(x->op,"NOR"))
		{
			if((x->field1 == x->field3 && ready[x->field1] == true) || (x->field2 == x->field3 && ready[x->field2] == true))
			{
				if(!strcmp(temp_head1->op,"SW") || !strcmp(temp_head1->op,"ADD") || !strcmp(temp_head1->op,"SUB") || !strcmp(temp_head1->op,"MUL") || !strcmp(temp_head1->op,"AND") || !strcmp(temp_head1->op,"OR") || !strcmp(temp_head1->op,"XOR") || !strcmp(temp_head1->op,"NOR"))
				{
					if(temp_head1->field1 == x->field1 || temp_head1->field2 == x->field1)
						return false;
				}
				else if(!strcmp(temp_head1->op,"LW") || !strcmp(temp_head1->op,"ADDI") || !strcmp(temp_head1->op,"ANDI") || !strcmp(temp_head1->op,"ORI") || !strcmp(temp_head1->op,"XORI"))
				{
					if(temp_head1->field1 == x->field1)
						return false;
				}
			}
		}
	}

	i = 0;
	q = pre_issue_queue;
	while(i != temp)
	{
		if(!strcmp(q->op,"SW") && !strcmp(x->op,"SW"))
			return false;
		else if(!strcmp(q->op,"SW") && !strcmp(x->op,"LW"))
			return false;
		else
		{
			q = q->next;
			i++;
		}
	}

	return true;
}

void alu()
{
	queueptr x = pre_alu;
	if(x != NULL && x->cycle < current_cycle)
	{
		pre_alu = remove_from_queue(pre_alu,0);
		pre_alu_count--;
		
		if(!strcmp(x->op,"SW"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->result = x->field3 + R[x->field1];
			q->reg = x->field2;
			q->cycle = current_cycle;
			strcpy(q->op,x->op);
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			prev_pre_mem = q;
		}
		else if(!strcmp(x->op,"LW"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->result = x->field3 + R[x->field1];
			q->reg = x->field2;
			q->cycle = current_cycle;
			strcpy(q->op,x->op);
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			prev_pre_mem = q;
		}
		else if(!strcmp(x->op,"ADD"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field3;
			q->result = R[x->field1] + R[x->field2];
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"SUB"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field3;
			q->result = R[x->field1] - R[x->field2];
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"MUL"))		
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field3;
			q->result = R[x->field1] * R[x->field2];
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"AND"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field3;
			q->result = R[x->field1] & R[x->field2];
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"OR"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field3;
			q->result = R[x->field1] | R[x->field2];
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"XOR"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field3;
			q->result = R[x->field1] ^ R[x->field2];
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"NOR"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field3;
			q->result = ~(R[x->field1] | R[x->field2]);
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"ADDI"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field2;
			q->result =R[x->field1] + x->field3;
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"ANDI"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field2;
			q->result =R[x->field1] & x->field3;
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"ORI"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field2;
			q->result =R[x->field1] | x->field3;
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}
		else if(!strcmp(x->op,"XORI"))
		{
			resqueueptr q = (resqueueptr)malloc(sizeof(result_queue));
			q->reg = x->field2;
			q->result =R[x->field1] ^ x->field3;
			q->cycle = current_cycle;
			q->field1 = x->field1;
			q->field2 = x->field2;
			q->field3 = x->field3;
			strcpy(q->op,x->op);
			prev_post_alu = q;
		}	
	}
}

void memory()
{
	if(next_pre_mem != NULL && next_pre_mem->cycle < current_cycle)
	{
		resqueueptr x = next_pre_mem;
		if(!strcmp(x->op,"SW"))
		{
			int addr = x->result;
			memptr q = mem_list;
			while(q->next!=NULL && q->address != addr)
				q = q->next;
			if(q==NULL)
				;
			else
				q->num = R[x->reg];
		}

		else if (!strcmp(x->op,"LW"))
		{
			int addr = x->result;
			memptr q = mem_list;
			while(q->address != addr)
				q = q->next;
			if(q==NULL)
				;
			else
			{
				resqueueptr r = (resqueueptr)malloc(sizeof(result_queue));
				r->reg = x->reg;
				r->result = q->num;
				r->cycle = current_cycle;
				r->field1 = x->field1;
				r->field2 = x->field2;
				r->field3 = x->field3;
				strcpy(r->op,x->op);
				prev_post_mem = r;
			}
		}
		next_pre_mem = NULL;
	}
}

void write_back()
{
	int num = 0;
	while(num<2)
	{
		if(next_post_mem != NULL && next_post_mem->cycle < current_cycle)
		{
			R[next_post_mem->reg] = next_post_mem->result;
			ready[next_post_mem->reg] = true;
			next_post_mem = NULL;
			num++;
		}
		if(next_post_alu != NULL && next_post_alu->cycle < current_cycle)
		{
			R[next_post_alu->reg] = next_post_alu->result;
			ready[next_post_alu->reg] = true;
			next_post_alu = NULL;
			num++;
		}
		else
			return;
	}
}

int main(int argc, char *argv[])
{
	char buffer[33], temp[4], read_inst[33];
	char a[33];
	int i = 0,j = 0,m,add,val,cycle = 1;
	FILE *fp, *fp2;
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

	
	for(int k=0; k<32; k++)
	{
		R[k] = 0;
	}
	
	bool not_break = true;
	fp2 = fopen("simulation.txt" ,"w");

	while(not_break)
	{
		current_cycle++;
		not_break = fetch();		
		issue();
		alu();	
		memory();
		write_back();

		fputs("--------------------",fp2);
		fputs("\nCycle:",fp2);
		memset(a,'\0',33);
		sprintf(a,"%d",current_cycle);
		fputs(a,fp2);
		fputs("\n\nIF Unit:\n\tWaiting Instruction: ",fp2);
		
		queueptr z = waiting_inst;
		if(z!=NULL)
		{
			fputs("[",fp2);
			fputs(z->op,fp2);

			if(!strcmp(z->op,"BEQ"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}

			else if(!strcmp(z->op,"BGTZ"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}
		}
			
		fputs("\n\tExecuted Instruction: ",fp2);
		z = executed_inst;
		if(z!=NULL)
		{
			fputs("[",fp2);
			fputs(z->op,fp2);

			if(!strcmp(z->op,"J"))
			{
				fputs(" #",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs(a,fp2);
				fputs("]",fp2);
			}

			else if(!strcmp(z->op,"BEQ"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}

			else if(!strcmp(z->op,"BGTZ"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}

			else if(!strcmp(z->op,"BREAK"))
			{
				fputs("]",fp2);
			}
		}

		fputs("\nPre-Issue Queue:\n",fp2);
		z = pre_issue_queue;
		int m = 0;
		while(z!=NULL)
		{
			fputs("\tEntry ",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",m);
			fputs(a,fp2);
			fputs(": ",fp2);
			fputs("[",fp2);
			fputs(z->op,fp2);

			if(!strcmp(z->op,"ADD") || !strcmp(z->op,"SUB") || !strcmp(z->op,"MUL") || !strcmp(z->op,"AND") || !strcmp(z->op,"OR") || !strcmp(z->op,"XOR") || !strcmp(z->op,"NOR"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}

			else if(!strcmp(z->op,"ADDI") || !strcmp(z->op,"ANDI") || !strcmp(z->op,"ORI") || !strcmp(z->op,"XORI"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}
			
			else if(!strcmp(z->op,"SW") || !strcmp(z->op,"LW"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs(a,fp2);
				fputs("(R",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs(a,fp2);
				fputs(")",fp2);
				fputs("]",fp2);
			}

			fputs("\n",fp2);
			z = z->next;
			m++;
		}

		while(m!=4)
		{
			fputs("\tEntry ",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",m);
			fputs(a,fp2);
			fputs(":\n",fp2);
			m++;
		}

		fputs("Pre-ALU Queue:\n",fp2);
		z = pre_alu;
		m = 0;
		while(z!=NULL)
		{
			fputs("\tEntry ",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",m);
			fputs(a,fp2);
			fputs(": ",fp2);
			fputs("[",fp2);
			fputs(z->op,fp2);

			if(!strcmp(z->op,"ADD") || !strcmp(z->op,"SUB") || !strcmp(z->op,"MUL") || !strcmp(z->op,"AND") || !strcmp(z->op,"OR") || !strcmp(z->op,"XOR") || !strcmp(z->op,"NOR"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}

			else if(!strcmp(z->op,"ADDI") || !strcmp(z->op,"ANDI") || !strcmp(z->op,"ORI") || !strcmp(z->op,"XORI"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}
			
			else if(!strcmp(z->op,"SW") || !strcmp(z->op,"LW"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field3);
				fputs(a,fp2);
				fputs("(R",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z->field1);
				fputs(a,fp2);
				fputs(")",fp2);
				fputs("]",fp2);
			}

			fputs("\n",fp2);
			z = z->next;
			m++;
		}

		while(m!=2)
		{
			fputs("\tEntry ",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",m);
			fputs(a,fp2);
			fputs(":\n",fp2);
			m++;
		}

		fputs("Pre-MEM Queue: ",fp2);
		resqueueptr z1 = prev_pre_mem;
		if(z1!=NULL)
		{
			fputs("[",fp2);
			fputs(z1->op,fp2);

			if(!strcmp(z1->op,"SW") || !strcmp(z1->op,"LW"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field3);
				fputs(a,fp2);
				fputs("(R",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field1);
				fputs(a,fp2);
				fputs(")",fp2);
				fputs("]",fp2);
			}
		}

		fputs("\nPost-MEM Queue: ",fp2);
		z1 = prev_post_mem;
		if(z1!=NULL)
		{
			fputs("[",fp2);
			fputs(z1->op,fp2);
			fputs(" ",fp2);
			memset(a,'\0',33); 
			sprintf(a, "%d", z1->field2);
			fputs("R",fp2);
			fputs(a,fp2);
			fputs(", ",fp2);
			memset(a,'\0',33); 
			sprintf(a, "%d", z1->field3);
			fputs(a,fp2);
			fputs("(R",fp2);
			memset(a,'\0',33); 
			sprintf(a, "%d", z1->field1);
			fputs(a,fp2);
			fputs(")",fp2);
			fputs("]",fp2);
		}

		fputs("\nPost-ALU Queue: ",fp2);
		z1 = prev_post_alu;
		if(z1!=NULL)
		{
			fputs("[",fp2);
			fputs(z1->op,fp2);

			if(!strcmp(z1->op ,"ADD") || !strcmp(z1->op,"SUB") || !strcmp(z1->op,"MUL") || !strcmp(z1->op,"AND") || !strcmp(z1->op,"OR") || !strcmp(z1->op,"XOR") || !strcmp(z1->op,"NOR"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field3);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}

			else if(!strcmp(z1->op,"ADDI") || !strcmp(z1->op,"ANDI") || !strcmp(z1->op,"ORI") || !strcmp(z1->op,"XORI"))
			{
				fputs(" ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field2);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field1);
				fputs("R",fp2);
				fputs(a,fp2);
				fputs(", ",fp2);
				memset(a,'\0',33); 
				sprintf(a, "%d", z1->field3);
				fputs("#",fp2);
				fputs(a,fp2);
				fputs("]",fp2);
			}
		}
		
		fputs("\n\nRegisters\n",fp2);
		fputs("R00:",fp2);
		for(int b = 0; b<8 ; b++)
		{
			fputs("\t",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",R[b]);
			fputs(a,fp2);
		}

		fputs("\nR08:",fp2);
		for(int b = 8; b<16 ; b++)
		{
			fputs("\t",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",R[b]);
			fputs(a,fp2);
		}

		fputs("\nR16:",fp2);
		for(int b = 16; b<24 ; b++)
		{
			fputs("\t",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",R[b]);
			fputs(a,fp2);
		}

		fputs("\nR24:",fp2);
		for(int b = 24; b<32 ; b++)
		{
			fputs("\t",fp2);
			memset(a,'\0',33);
			sprintf(a,"%d",R[b]);
			fputs(a,fp2);
		}

		fputs("\nData\n",fp2);
		
		memptr q = mem_list;
		int addr = q->address;
		for(int b=addr; q!=NULL ;b = b+32)
		{
			memset(a,'\0',33);
			sprintf(a,"%d",b);
			fputs(a,fp2);
			fputs(":",fp2);
			for(int w=0 ; w<8 && q!=NULL ; w++)
			{
				fputs("\t",fp2);
				memset(a,'\0',33);
				sprintf(a,"%d",q->num);
				fputs(a,fp2);
				q = q->next;
			}
			fputs("\n",fp2);
		}

		next_post_alu = prev_post_alu;
		prev_post_alu = NULL;
		next_post_mem = prev_post_mem;
		prev_post_mem = NULL;
		next_pre_mem = prev_pre_mem;
		prev_pre_mem = NULL;
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