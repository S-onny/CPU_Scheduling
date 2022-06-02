
typedef struct{
	char name[40];
	int twt;
	int ttat;
	int trt;
}RANK;
int compare_w(const void* p1, const void* p2) {//twt순으로 정렬하기위한 compare 함수
	return ((RANK*)p1)->twt - ((RANK*)p2)->twt;
}
int compare_t(const void* p1, const void* p2) {//ttat순으로 정렬하기위한 compare 함수
	return ((RANK*)p1)->ttat - ((RANK*)p2)->ttat;
}
int compare_r(const void* p1, const void* p2) {//trt순으로 정렬하기위한 compare 함수
	return ((RANK*)p1)->trt - ((RANK*)p2)->trt;
}
void al_rank(int * check,DATA* datas){
	int np=datas[0].num_proc;
	for(int i=0;i<7;i++){//check all algorithms are executed
		if(check[i]==0) {
			printf("Please run all algorithms!\n");
			return;
		}
	}
	RANK* ranks=(RANK *)malloc(sizeof(RANK)*7);
	char str[30];
	int s=0;
	for(int i = 0 ; i < 7 ; i++){
		switch(i){
			case(1):
				strcpy((ranks[i].name),"SJF");
				break;
			case(2):
				strcpy((ranks[i].name),"SRTF");
				break;
			case(3):
				strcpy((ranks[i].name),"RR");
				break;
			case(4):
				strcpy((ranks[i].name),"Non-Preemptive Priority");
				break;
			case(5):
				strcpy((ranks[i].name),"Preemptive Priority");
				break;
			case(6):
				strcpy((ranks[i].name),"Non-Preemptive Priority with RR");
				break;
			default:
				strcpy((ranks[i].name),"FCFS");
		}
			ranks[i].twt=(datas[i].twt);
			ranks[i].ttat=(datas[i].ttat);
			ranks[i].trt=(datas[i].trt);
	}
	while(1){
		printf("Which metric? \n[1]Average Wait time\n[2]Average Turnaround time\n[3]Average Response time");
		while(1){
			printf("Enter:");
			scanf("%d",&s);
			if(s<=3 && 1<=s) break;
			else{
				while(getchar()!='\n');
				printf("Retry with valid integer\n");
			}
		}
		int N=0;
		switch(s){
			case(3):
				qsort(ranks,7,sizeof(RANK),compare_r);
				strcpy(str,"Avrg Response time");
				printf("Ranking of Algorithms by %s\n",str);
				printf("Rank#\tAlgorithm\t\t\t\t Value\n");
				printf("---------------------------------------------------------------\n");
				printf("#%d\t%s\t\t\t\t %.2f",1,ranks[0].name,(float)(ranks[0].trt)/np); 
				N=1;
				for(int i=1;i<7;i++){
					if(ranks[i-1].trt==ranks[i].trt){
						printf("#%d\t%s\t\t\t\t %.2f\n",N,ranks[i].name,(float)(ranks[i].trt)/np);
					}
					else{
						N=i+1;
						printf("#%d\t%s\t\t\t\t %.2f\n",N,ranks[i].name,(float)(ranks[i].trt)/np);
					}
				}
				break;
			case(2):
				qsort(ranks,7,sizeof(RANK),compare_t);
				strcpy(str,"Avrg Turnaround time");
				printf("Ranking of Algorithms by %s\n",str);
				printf("Rank#\tAlgorithm\t\t\t\t Value\n");
				printf("---------------------------------------------------------------\n");
				printf("#%d\t%s\t\t\t\t %.2f",1,ranks[0].name,(float)(ranks[0].ttat)/np); 
				N=1;
				for(int i=1;i<7;i++){
					if(ranks[i-1].ttat==ranks[i].ttat){
						printf("#%d\t%s\t\t\t\t %.2f\n",N,ranks[i].name,(float)(ranks[i].ttat)/np);
					}
					else{
						N=i+1;
						printf("#%d\t%s\t\t\t\t %.2f\n",N,ranks[i].name,(float)(ranks[i].ttat)/np);
					}
				}
				break;
			default:
				qsort(ranks,7,sizeof(RANK),compare_w);
				strcpy(str,"Avrg Wait time");
				printf("Ranking of Algorithms by %s\n",str);
				printf("Rank#\tAlgorithm\t\t\t\t Value\n");
				printf("---------------------------------------------------------------\n");
				printf("#%d\t%s\t\t\t\t %.2f",1,ranks[0].name,(float)(ranks[0].twt)/np); 
				N=1;
				for(int i=1;i<7;i++){
					if(ranks[i-1].twt==ranks[i].twt){
						printf("#%d\t%s\t\t\t\t %.2f\n",N,ranks[i].name,(float)(ranks[i].twt)/np);
					}
					else{
						N=i+1;
						printf("#%d\t%s\t\t\t\t %.2f\n",N,ranks[i].name,(float)(ranks[i].twt)/np);
					}
				}
		}
		int s2=2;
			
		printf("Redo? [1]yes [0] no:");
		while(1){
			scanf("%d",&s2);
			if(s2<=1 && 0<=s2) break;
			else{
				while(getchar()!='\n');
				printf("Retry with valid integer\n");
			}	
			
		}
		if(s2==0) break;
	}
	free(ranks);
}
