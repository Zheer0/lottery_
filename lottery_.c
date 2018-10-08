#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
 
#define MAX_USER_SIZE 100
#define PSD_LENGTH 20
#define NAME_LENGTH 10
#define INFO_LENGTH 100

struct student {
    int id;
	char name[NAME_LENGTH];
	char psd[PSD_LENGTH];
	int gender;
	int grade;
	char info[INFO_LENGTH];
    struct student *next;
};
typedef struct student NODE; //typedef为C语言的关键字，作用是为一种数据类型定义一个新名字。简化一些比较复杂的类型声明。
 
NODE * init(); //创建链表
void insert(NODE *head, NODE *pnew, int i); //插入
void delete(NODE *head, int i); //删除
void allClear(NODE *head); //清除链表的所有节点，包含头节点。
void display(NODE *head); //打印链表
NODE * login(NODE *head);
NODE * find(NODE *head,int id);
void regest(NODE *head);
void loop(NODE *head);
int powTen(int n);
int strToNumber(char []);
int strToNumber2(char []);
void save(NODE *);
int getSum(int *,int);
int getLotteryLevel(int *lottery, int);
char * cut(char*);
void saveconfs(int slen,int sgetrs,int sconf_scoll,int sglen,int sgstyle,int snlen,int snstyle,char *sprize_info[],char *sreword_info[],int *sprize_num,int *sa,char *sproinfo,char *sget_prize_info[],char *snot_get_prize_info[]);

int main(int argc, const char * argv[]) {
    //NODE *head, *pnew;
    NODE *head;
    head = init();
    //display(head);
	char sele[10];
	char name[20];
	system("clear");
	printf("***********************************************\n");
	printf("*              欢迎使用抽奖程序!               *\n");
	printf("***********************************************\n\n");
	while(1){
		printf("请选择\n登录(d),注册(z):\n");
		scanf("%s", sele);
		if(strcmp(sele, "d")==0){
		//登录
			loop(head);
			break;	
		}
		else if(strcmp(sele, "z")==0){
		//注册
			regest(head);
			loop(head);
			break;
		}
		else
			printf("输入错误!\n\n");
	} 
    return 0;
}
 
//创建链表
NODE * init() {
    NODE *head, *tail, *pnew;
    head = (NODE*)malloc(sizeof(NODE)); //创建头节点。
    if (head == NULL) {
        printf("创建失败!");
        return NULL;
    }
    head->next = NULL;
    tail = head;
    
    FILE *fpr = fopen("user.txt", "r");
    char buff[1024];
    int hitem=0;
    while(fgets(buff,1024,fpr)!=NULL){
    	//printf(">>%s\n", buff);
    	if(hitem%6==0){
    		//printf("0");
    		pnew = (NODE*)malloc(sizeof(NODE)); //创建新节点
			if (pnew == NULL) {
				printf("创建失败！");
				return  NULL;
			}
    		pnew->id = strToNumber2(buff);
    		//printf("\n%s-->%d\n", buff,strToNumber2(buff));
    	}
    	else if(hitem%6==1){
    		//printf("1");
    		strcpy(pnew->name , cut(buff));
    	}
    	else if(hitem%6==2){
    		//printf("2");
    		strcpy(pnew->psd , cut(buff));
    	}
    	else if(hitem%6==3){
    		//printf("3");
    		pnew->gender = strToNumber2(buff);
    	}
    	else if(hitem%6==4){
    		//printf("4");
    		pnew->grade = strToNumber2(buff);
    	}
    	else if(hitem%6==5){
    		//printf("5");
    		strcpy(pnew->info ,cut(buff));
    		pnew->next = NULL;
		    tail->next = pnew; //新节点插入到表尾
		    tail = pnew; //为指针指向当前的尾节点
		    //printf("[学号: %d, 姓名: %s, 性别: %d, 年级: %d, 个人简介: %s]", pnew->id, pnew->name, pnew->gender, pnew->grade, pnew->info);
    	}
    	hitem++;
    }
    
    //display(head);
    return head;
}

//插入 头结点不算，从有数据的开始算第一个
void insert(NODE *head, NODE *pnew, int i) {
	//printf("insert\n");
	//display(head);
    NODE *p = head;
    int j;
    for (j = 0; j < i && p != NULL; j++) {
        p = p->next;
        if (p == NULL) {
            printf("\n与插入的节点不存在！");
            return;
        }
    }
    pnew->next = p->next;
    p->next = pnew;
}
 
//删除 头结点不算，从有数据的开始算第一个
void delete(NODE *head, int i) {
    NODE *p = head, *pnew;
    int j;
    for (j = 0; j < i && p != NULL; j++) {
        p = p->next;
        if (p == NULL) {
            printf("\n删除的节点不存在！");
            return;
        }
    }
    pnew = p->next;
    p->next = pnew->next;
    free(pnew);
}

NODE * find(NODE *head,int id){
	NODE *p=head;
	while(p!=NULL){
		if(p->id == id)
			return p;
		else
			p=p->next;
	}
	return NULL;
}

//清除链表的所有节点，包含头节点。
void allClear(NODE *head) {
    NODE *p, *q;
    p = head;
    while (p->next != NULL) {
        q = p->next;
        p->next = q->next;
        free(q);
    }
    free(head); //最后删除头节点
}
 
//打印链表
void display(NODE *head) {
    printf("\n学生成绩分别是：");
    NODE *p;
    for (p = head->next; p != NULL; p = p->next) {
    	//printf("");
        printf("\n[学号: %d, 姓名: %s, 性别: %d, 年级: %d, 个人简介: %s]", p->id, p->name, p->gender, p->grade, p->info);
    }
}

NODE * login(NODE *head){
	int i;
	char id[10],psd[PSD_LENGTH];
	while(1){
		printf("请输入学号\n>>");
		scanf("%s", id);
		int nid = strToNumber(id);
		if(nid<=0)
			printf("输入有误!");
		else if(find(head,strToNumber(id))==NULL)
			printf("学号不存在!");
		else
			break;
	}
	NODE *temp = find(head,strToNumber(id));
	for(i=0;i<3;i++){
		//三次登录机会
		printf("请输入密码\n>>");
		scanf("%s", psd);
		//printf("psd->%s enter->%s", temp->psd,psd);
		if(strcmp(psd,temp->psd)==0)
			return temp;
		else
			printf("密码错误,重新输入!\n");
	}
	return NULL;
}

void regest(NODE *head){
	//printf("regest\n");
	//display(head);
	//int id, gender, grade;
	char id[10],gender[5],grade[5];
	char name[NAME_LENGTH], psd[PSD_LENGTH], rpsd[PSD_LENGTH], info[INFO_LENGTH];
	while(1){
		printf("请输入学号\n>>");
		scanf("%s", id);
		int nid = strToNumber(id);
		if(nid<=0)
			printf("输入有误!");
		else if(find(head,strToNumber(id))!=NULL)
			printf("学号已存在!");
		else
			break;
	}
	while(1){
		printf("请输入用户名\n>>");
		scanf("%s", name);
		break;
	}
	while(1){
		printf("请输入密码\n>>");
		scanf("%s", psd);
		printf("请再次输入密码\n>>");
		scanf("%s", rpsd);
		if(strcmp(psd,rpsd)==0)
			break;
		else
			printf("两次输出不同,重新输入!\n");
	}
	while(1){
		printf("请输入性别:男(1),女(2)\n>>");
		scanf("%s", gender);
		int temp=strToNumber(gender);
		printf("%d \n", temp);
		if(1<=temp&&temp<=2)
			break;
		else
			printf("输入有误!");
	}
	while(1){
		printf("请输入年级:大一(1),大二(2),大三(3),大四(4)\n>>");
		scanf("%s", grade);
		int temp=strToNumber(grade);
		printf("%d \n", temp);
		if(1<=temp&&temp<=4)
			break;
		else
			printf("输入有误!");
	}
	printf("请输入个人简介\n>>");
	scanf("%s", info);
	NODE *pnew = (NODE*)malloc(sizeof(NODE));
	pnew->id = strToNumber(id);
	strcpy(pnew->name,name);
	strcpy(pnew->psd,psd);
	//pnew->name = name;
	//pnew->psd = psd;
	pnew->gender = strToNumber(gender);
	pnew->grade = strToNumber(grade);
	//pnew->info = info;
	strcpy(pnew->info,info);
    insert(head, pnew, 1);
    printf("\n");
    save(head);
    printf("注册成功!请登录:\n");
}

void loop(NODE *head){
	//system("clear");
	//主循环
	
	
	
	NODE *me = login(head);
	
	
	int len=0;
	//int a[]={0};
	int getrs = 0;
	char proinfo[1024];//
	int conf_scoll=0;
	//char *get_prize_info[]={"a"};
	int glen=0;
	int gstyle=0;
	//char *not_get_prize_info[]={"a"};//
	int nlen=0;
	int nstyle=0;
	
	//--------------------
	
	
	
	//int len=sizeof(prize_num)/sizeof(prize_num[0]);
	//=========================
	
	//void loadconf(){
	FILE *fpr = fopen("conf.txt", "r");
	if(fpr==NULL)
		exit(1);
	char buff[1024];
	//get value
	fgets(buff,1024,fpr);
	int toint = strToNumber2(buff);
	len = toint;
	char *prize_info[len];
	char *reword_info[len];
	int *prize_num=(int*)malloc(len*sizeof(int));
	for(toint=0;toint<len;toint++){
		prize_info[toint]=(char*)malloc(sizeof(char)*20);
		reword_info[toint]=(char*)malloc(sizeof(char)*20);
	}
	//---
	fgets(buff,1024,fpr);
	toint = strToNumber2(buff);
	getrs = toint;
	int *a=(int*)malloc(getrs*2*sizeof(int));
	//---
	fgets(buff,1024,fpr);
	toint = strToNumber2(buff);
	conf_scoll = toint;
	//---
	fgets(buff,1024,fpr);
	toint = strToNumber2(buff);
	glen = toint;
	char *get_prize_info[glen];
	for(toint=0;toint<len;toint++){
		get_prize_info[toint]=(char*)malloc(sizeof(char)*20);
	}
	
	//---
	fgets(buff,1024,fpr);
	toint = strToNumber2(buff);
	gstyle = toint;
	//---
	fgets(buff,1024,fpr);
	toint = strToNumber2(buff);
	nlen = toint;
	char *not_get_prize_info[glen];
	for(toint=0;toint<len;toint++){
		not_get_prize_info[toint]=(char*)malloc(sizeof(char)*20);
	}
	
	//---
	fgets(buff,1024,fpr);
	toint = strToNumber2(buff);
	nstyle = toint;
	//---
	
	
	fgets(buff,1024,fpr);//奖品类别
	for(toint=0;toint<len;toint++){
		fgets(buff,1024,fpr);
		//printf("%s\n", buff);
		strcpy(prize_info[toint],cut(buff));
		//printf("err?");
	}
	fgets(buff,1024,fpr);//奖品信息
	for(toint=0;toint<len;toint++){
		fgets(buff,1024,fpr);
		//printf("%s\n", buff);
		strcpy(reword_info[toint],cut(buff));
	}	
	fgets(buff,1024,fpr);//奖品数量
	for(toint=0;toint<len;toint++){
		fgets(buff,1024,fpr);
		//printf("%s\n", buff);
		prize_num[toint]=strToNumber2(buff);
	}
	fgets(buff,1024,fpr);//中奖人信息
	for(toint=0;toint<getrs;toint++){
		fgets(buff,1024,fpr);
		//printf("%s\n", buff);
		a[2*toint]=strToNumber2(buff);
		fgets(buff,1024,fpr);
		//printf("%s\n", buff);
		a[2*toint+1]=strToNumber2(buff);
	}	
	fgets(buff,1024,fpr);//项目信息
	fgets(buff,1024,fpr);
	//printf("%s\n", buff);
	strcpy(proinfo,cut(buff));
	
	fgets(buff,1024,fpr);//成功信息
	for(toint=0;toint<glen;toint++){
		fgets(buff,1024,fpr);
		//printf("%s\n", buff);
		strcpy(get_prize_info[toint],cut(buff));
	}
	fgets(buff,1024,fpr);//失败信息
	for(toint=0;toint<nlen;toint++){
		fgets(buff,1024,fpr);
		//printf("%s\n", buff);
		strcpy(not_get_prize_info[toint],cut(buff));
	}


		
	//}
	//loadconf();
	
	void showt(){
		int toint;
		printf("len%d getrs%d conf_scoll%d glen%d gstyle%d nlen%d nstyle%d",len, getrs, conf_scoll, glen, gstyle, nlen,nstyle);
		
		printf("\n");
		for(toint=0;toint<len;toint++){
			printf("%s\n", prize_info[toint]);
		}
		
		printf("\n");
		for(toint=0;toint<len;toint++){
			printf("%s\n", reword_info[toint]);
		}	
		
		printf("\n");
		for(toint=0;toint<len;toint++){
			printf("%d\n", prize_num[toint]);
		}
		
		printf("\n");
		for(toint=0;toint<getrs;toint++){
			printf("%d %d\n", a[2*toint],a[2*toint+1]);
		}	
		
		printf("\n");
		printf("%s\n", proinfo);
		
		
		printf("\n");
		for(toint=0;toint<glen;toint++){
			printf("%s\n", get_prize_info[toint]);
		}
		
		printf("\n");
		for(toint=0;toint<nlen;toint++){
			printf("%s\n", not_get_prize_info[toint]);
		}
	}
	//showt();
	//exit(1);
	//==========================

	//=========================
	
	
	
	if(me==NULL){
		printf("登录失败!\n退出程序!");
	}
	else{
		/*char *prize_info[]={"特等奖", "一等奖","二等奖","三等奖","四等奖","参与奖","不得奖"};//
		char *reword_info[]={"5头牛", "4头牛","3头牛","2头牛","1头牛","1只鸡","谢谢参与"};//
		int prize_num[7]={1,2,3,4,5,30,100};//
		int len = 7;
		int a[]={10008,0,10021,2,10018,3,10007,5,10016,5};//
		int getrs = 5;//
		char proinfo[1024]="这是一款抽奖游戏!";//
		int conf_scoll=0;//
		char *get_prize_info[]={"抽奖成功!您抽到了**%s**,奖品为**%s**"};//
		int glen=1;//
		int gstyle=0;//
		char *not_get_prize_info[]={"真遗憾!您没有抽到奖品,下次再试试吧!"};//
		int nlen=1;
		int nstyle=0;*/
		//---------------------
		
		//char *prize_info[]={"a"};
		//char *reword_info[]={"a"};
		//int prize_num[]={0};

		
		
	
		srand((unsigned)time(0));	
		int id=me->id;
		char select_func[5];
		int nsf;
		if(id == 10001){
			//管理员账户,可以进行设置操作,不可参与抽奖
			
			system("clear");
			printf("\033[37m你是管理员,可以进行系统管理,不可抽奖!\n");
			while(1){
				printf("\n选择操作:\n 设置抽奖项目信息简介(1)\n 设置抽奖奖项(2)\n 设置是否滚动(3)\n 设置抽奖结果显示格式(4)\n 退出(ctrl+z)\n>>");
				scanf("%s", select_func);
				nsf = strToNumber(select_func);
				if(nsf<1||nsf>4){
					printf("输入错误!请重新");
				}
				/*else if(nsf==5){
					printf("\n退出程序!");
					//saveconfs();
					exit(1);
				}*/
				else{
					switch(nsf){
						case 1: {
							printf("当前抽奖项目信息简介:\n");
							printf("**%s**\n", proinfo);
							printf("输入新的抽奖项目信息\n>>\n");
							scanf("%s", proinfo);
							printf("新的抽奖项目信息:\n**%s**\n缓存中...\n缓存成功!\n", proinfo);
							//saveconfs();
							saveconfs(len,getrs,conf_scoll,glen,gstyle,nlen,nstyle,prize_info,reword_info,prize_num,a,proinfo,get_prize_info,not_get_prize_info);
							break;
						}
						case 2:{
							printf("当前抽奖奖项信息:\n");
							int pos;
							for(pos=0;pos<len;pos++){
								printf("**%s %s %d\n**", prize_info[pos], reword_info[pos], prize_num[pos]);
							}
							printf("请输入新的奖项类别数>>");
							int knum;
							scanf("%d", &knum);
							char *npname[knum];
							char *nrename[knum];
							int *npnum=(int*)malloc(knum*sizeof(int));
							for(pos=0;pos<knum;pos++){
								npname[pos]=(char*)malloc(sizeof(char)*20);
								printf("输入第%d个奖项名称>>",pos+1);
								scanf("%s",npname[pos]);
								nrename[pos]=(char*)malloc(sizeof(char)*20);
								printf("输入 %s 是什么>>",npname[pos]);
								scanf("%s",nrename[pos]);
								printf("输入 %s 的数量>>",npname[pos]);
								scanf("%d", &npnum[pos]);
							}
							printf("新抽奖奖项信息:\n");
						
							for(pos=0;pos<knum;pos++){
								printf("**%s %s %d\n**", npname[pos], nrename[pos],npnum[pos]);
							}
							saveconfs(knum,getrs,conf_scoll,glen,gstyle,nlen,nstyle,npname,nrename,npnum,a,proinfo,get_prize_info,not_get_prize_info);					
							break;
						}
						case 3:{
							//
							printf("设置用户抽奖时是否滚动显示中奖信息:滚动(0),不滚动(1)\n>>");
							while(1){
								char isscol[10];
								scanf("%s", isscol);
								int issid = strToNumber(isscol);
								if(issid<0||issid>1){
									printf("输入错误,请重新");
								}
								else{
									conf_scoll=issid;
									printf("成功设置!");
									break;
								}
							}
							saveconfs(len,getrs,conf_scoll,glen,gstyle,nlen,nstyle,prize_info,reword_info,prize_num,a,proinfo,get_prize_info,not_get_prize_info);		
							break;
						}
						case 4:{//抽奖格式显示和信息的设置
							int i;
							int te = 1;
							printf("当前已有获奖结果显示格式:\n");
							for(i=0;i<glen;i++){
								printf("NO.%d-->%s \n",te,get_prize_info[i]);
								te++;
							}
							printf("\n请选择展示给用户的获奖格式编号,或者直接输入新的格式:\n>>");
							char gtemp[100];
							scanf("%s", gtemp);
							int gti = strToNumber(gtemp);
							if(gti<0||gti>(te-1)){
								//新建
								char *ngpi[++glen];
								int gtipos;
								for(gtipos=0;gtipos<(glen-1);gtipos++){
									ngpi[gtipos]=(char*)malloc(sizeof(char)*20);
									ngpi[gtipos]=get_prize_info[gtipos];
									
								}
								strcpy(ngpi[glen-1],gtemp);
											
								saveconfs(len,getrs,conf_scoll,glen,gstyle,nlen,nstyle,prize_info,reword_info,prize_num,a,proinfo,ngpi,not_get_prize_info);
							}
							else
								gstyle = gti;
					
							te=1;
							printf("\n当前已有未获奖结果显示格式:\n");
							for(i=0;i<nlen;i++){
								printf("NO.%d-->%s \n",te,not_get_prize_info[i]);
								te++;
							}
							printf("\n请选择展示给用户的未获奖格式编号,或者直接输入新的格式:\n>>");
							char ntemp[100];
							scanf("%s", ntemp);
							gti = strToNumber(ntemp);
							if(gti<0||gti>(te-1)){
								//新建
								char *ngpid[++nlen];
								int gtipos;
								for(gtipos=0;gtipos<(nlen-1);gtipos++){
									ngpid[gtipos]=(char*)malloc(sizeof(char)*20);
									ngpid[gtipos]=get_prize_info[gtipos];
									
								}
								strcpy(ngpid[nlen-1],gtemp);
											
								saveconfs(len,getrs,conf_scoll,glen,gstyle,nlen,nstyle,prize_info,reword_info,prize_num,a,proinfo,get_prize_info,ngpid);
							}
							else
								nstyle = gti;
							
							
							break;
						}
						/*case 5:{//抽奖奖品信息设置
							printf("当前奖品信息:\n");
							int pos;
							for(pos=0;pos<len;pos++){
								printf(" %s 奖品为** %s **, 数量** %d **\n", prize_info[pos], reword_info[pos],prize_num[pos]);
							}
							char *npi[len];
							for(pos=0;pos<len;pos++){
								npi[pos]=(char*)malloc(sizeof(char)*20);
								printf("输入 %s 的信息>>", prize_info[pos]);
								scanf("%s",npi[pos]);
							}
							printf("新奖品信息:\n");
							for(pos=0;pos<len;pos++){
								printf(" %s 奖品为** %s **, 数量** %d **\n", prize_info[pos], npi[pos],prize_num[pos]);
							}						
							break;
						}*/
					}	
				}
			}

		}
		else{
			//普通用户,只可抽奖
		
			system("clear");
			printf("\033[37m您是普通用户!");
			int check;
			for(check=0;check<getrs;check++){
				if(a[2*check]==id){
					printf("您已参与过抽奖!\n退出!\n");
					exit(1);
				}
			}
		
			int xp=1,yp=1,pthid=0;
			void clear_scoll(){
				printf("\033[2;70H                                                     \n");
				printf("\033[3;70H                                            \n");
				printf("\033[4;70H                                            \n\033[%d;%dH\n",xp,yp);
			}
			void mythr(void){
				if(conf_scoll==1){
				
					printf("\033[2;70H --------------------------------------");
					printf("\033[4;70H --------------------------------------");
					while(1){
						sleep(1);
						printf("\033[37m\033[3;70H       用户%d获得%s                  \n\033[%d;%dH\n",a[2*pthid],reword_info[a[2*pthid+1]],xp,yp);
						pthid++;
						if(pthid==getrs)
							pthid=0;
					}
				}
			}
			void thr(void){
				char select[5];
				int nsf;
				while(1){
					printf("\033[37m\033[%d;%dH选择操作: 单组抽奖(1), 多组抽奖(2), 退出(0)\n", ++xp,yp);
					scanf("%s", select);
					nsf = strToNumber(select);
					if(nsf<0||nsf>2){
						printf("\033[%d;%dH输入错误!请重新",++xp,yp);
					}
					else{
						break;	
					}
				}
				xp++;
				switch(nsf){
					case 1:{
				
						//printf("\033[2J");
				
						printf("\033[%d;%dH------------------------------------\n", ++xp, yp);
						printf("\033[%d;%dH开始抽奖!\n", ++xp, yp);
						printf("\033[31m\033[%d;%dH特等奖?\n",++xp, yp);
						int cid=0,kk;
						for(kk=0;kk<30;kk++){
							printf("\033[31m\033[1A%s?\n", prize_info[cid++]);
							if(cid==len){
								cid=0;
								continue;
							}
							usleep(100000);
						}
						int prize = getLotteryLevel(prize_num, len);
						printf("\033[1A%s!\n", prize_info[prize]);
						if(prize==(len-1)){
							printf("\033[37m\033[%d;%dH%s\n", ++xp,yp,not_get_prize_info[nstyle]);
						}
						else{
							printf("\033[37m\033[%d;%dH%s您获得%s,奖品为%s!",++xp,yp,get_prize_info[gstyle], prize_info[prize],reword_info[prize]);
							a[getrs*2] = id;
							a[getrs*2+1] = prize;
							getrs++;
							clear_scoll();
							saveconfs(len,getrs,conf_scoll,glen,gstyle,nlen,nstyle,prize_info,reword_info,prize_num,a,proinfo,get_prize_info,not_get_prize_info);
						}
						printf("\033[%d;%dH------------------------------------\n",++xp,yp);
						
						
						exit(1);
						break;
					}
					case 2:{
						printf("\033[%d;%dH该功能尚未开放,敬请期待!\n",++xp,yp);
					
						clear_scoll();
					
						exit(1);
						break;
					}
					case 0:
						exit(1);
						break;
				}
			
			}
			pthread_t id1,id2;
			int ret1, ret2;
			ret1=pthread_create(&id1,NULL,(void *) mythr,NULL); // 成功返回0，错误返回错误编号
			ret2=pthread_create(&id2,NULL,(void *) thr,NULL); // 成功返回0，错误返回错误编号

			pthread_join(id1,NULL); 
			pthread_join(id2,NULL); 


		}
	}
}

int powTen(int n)//返回10的n次方
{
    int result=1;
    int i;
    for(i=1;i<=n;i++)
    {
        result*=10;
    }
    return result;
}

int strToNumber(char numbers[]){
    int length=strlen(numbers);//获取有效字符串长度
    int sum=0;
    int i;
    for(i=0;i<length;i++)
    {
        if(numbers[i]>='0'&&numbers[i]<='9')
            sum+=((int)numbers[i]-48)*powTen(length-i-1);
        else
        {
            return -1;
        }
    }
    return sum;
}
int strToNumber2(char numbers[]){
    int length=strlen(numbers)-1;//获取有效字符串长度
    int sum=0;
    int i;
    for(i=0;i<length;i++)
    {
        if(numbers[i]>='0'&&numbers[i]<='9')
            sum+=((int)numbers[i]-48)*powTen(length-i-1);
        else
        {
            return -1;
        }
    }
    return sum;
}

void save(NODE *head){
	FILE *fpw = fopen("user.txt", "w");
	if(fpw==NULL)
		exit(1);
    NODE *p;
    for (p = head->next; p != NULL; p = p->next) {
    	fprintf(fpw,"%d\n", p->id);
    	fprintf(fpw,"%s\n", p->name);
    	fprintf(fpw,"%s\n", p->psd);
    	fprintf(fpw,"%d\n", p->gender);
    	fprintf(fpw,"%d\n", p->grade);
    	fprintf(fpw,"%s\n", p->info);
    }
    fclose(fpw);
}

int getSum(int *lottery,int len) {
    int sum = 0;
    int v;
    for (v=0;v<len;v++) {
        sum += lottery[v];
    }
    return sum;
}

int getLotteryLevel(int *lottery, int len) {
	//抽奖算法
	int sum = getSum(lottery, len);
	int i;
	for(i=0;i<len;i++){
		int randNum = rand() % sum +1;
        	
	}
	return -1;
}
char * cut(char* src){
	int length=strlen(src);//获取有效字符串长度
	char ret[length];
    int i;
    for(i=0;i<length-1;i++)
    {
        ret[i]=src[i];
    }
    ret[length-1]='\0';
    return ret;
}

void saveconfs(int slen,int sgetrs,int sconf_scoll,int sglen,int sgstyle,int snlen,int snstyle,char *sprize_info[],char *sreword_info[],int *sprize_num,int *sa,char *sproinfo,char *sget_prize_info[],char *snot_get_prize_info[]){
	FILE *fpw = fopen("conf.txt", "w");
	if(fpw==NULL)
		exit(1);
		
	fprintf(fpw, "%d\n", slen);
	fprintf(fpw, "%d\n", sgetrs);//getrs conf_scoll, glen, gstyle, nlen,nstyle
	fprintf(fpw, "%d\n", sconf_scoll);
	fprintf(fpw, "%d\n", sglen);
	fprintf(fpw, "%d\n", sgstyle);
	fprintf(fpw, "%d\n", snlen);
	fprintf(fpw, "%d\n", snstyle);
	int spid;
	fprintf(fpw, "%s\n", "奖品类别");
	for(spid=0;spid<slen;spid++){
		fprintf(fpw, "%s\n", sprize_info[spid]);
	}
	fprintf(fpw, "%s\n", "奖品信息");
	for(spid=0;spid<slen;spid++){
		fprintf(fpw, "%s\n", sreword_info[spid]);
	}
			fprintf(fpw, "%s\n", "中奖人信息");
		fprintf(fpw, "%s\n", "项目信息");
	fprintf(fpw, "%s\n", sproinfo);

	fprintf(fpw, "%s\n", "成功信息");
	for(spid=0;spid<sglen;spid++){
		fprintf(fpw, "%s\n", sget_prize_info[spid]);
	}
	}
	

	//fprintf(fpw, "%d\n", );
	fclose(fpw);
}
