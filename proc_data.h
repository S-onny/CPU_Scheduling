
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
	int c;//�Ϸ������� 0 �ƴϸ� -1
}PROC;
typedef struct{
	PROC* procs;
	int* g_p;
	int* g_et;
	int* g_bt;
	int gantt_index;
}DATA;
//���� ���Ϸκ��� �����͸� �о� ���μ��� �迭�� ä��� �Լ�
int Process_load(FILE* fp,int* num_proc,int* tq, PROC* procs);

//�Էµ� ���μ��� �迭�� ����
PROC* Copy_processes(PROC* procs);

//�Էµ� ���μ��� �迭�κ��� �����층 �Լ��� ���� ������ ���� �迭 ����
DATA* Make_dataIn(PROC* procArr,int num_proc);
//������ �����迭 ��ü ����(free)
void Destroy_data(DATA* datas);

int compare_a(const void* p1, const void* p2);//arrival ���� �� ������ ���� compare �Լ�
int compare_p(const void* p1, const void* p2);//pid������ �����ϱ����� compare �Լ�