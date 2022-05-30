
typedef struct {
	int at;//arrival time
	int bt;//burst time
	int p;//pid
	int et;//first execute time
	int ct;//complete time
	int tat;//turnaround time
	int wt;//wait time
	int pri;//priority
	int rem;//remain burst time
	int c;//완료했으면 0 아니면 -1
}PROC;
//열린 파일로부터 데이터를 읽어 프로세스 배열을 채우는 함수
int Process_load(FILE* fp,int* num_proc,int* tq, PROC* procs);

int compare_a(const void* p1, const void* p2);//arrival 순서 퀵 정렬을 위한 compare 함수
int compare_p(const void* p1, const void* p2);//pid순으로 정렬하기위한 compare 함수