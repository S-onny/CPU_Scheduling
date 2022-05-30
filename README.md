# CPU_Scheduling
Coding CPU Scheduling

* Things to do
  * FCFS  - 지원
  * SJF  -  지원
  * SRTF - 창수
  * RR  - 우석
  * Non-Preemptive Priority  - 우석
  * Preemptive Priority  - 창수
  * Non-Preemptive Priority with RR - 찬욱

* CLI

#### Merge 하실때 그냥 master branch 하셔도 괜찮습니다!

####5월 30저녁~31일 02:30 최창수-변경사항 요약: 
커밋이 좀 뒤죽박죽이라 간단히 변경 사항 요약을 하겠습니다.
#####1.선점 우선순위 알고리즘과 srtf가 약간 수정되었습니다
1.1.도착시간순 정렬을 빼먹거나 헤더 빼먹은 것 등 수정하였습니다.
1.2.wt계산할때 while문 매 반복마다 for반복문 쓰던걸 마지막에 tat 이용해서 구하는 걸로 개선했습니다.
1.3.한 프로세스가 종료될때마다 종료된 프로세스를 뒤로 밀어내고 qsort돌렸었는데 구조체에 추가된 c 변수 이용하는걸로 대체하고 제거했습니다.

#####2.main, 헤더 일부 수정
과제 조건에 cli로 할거면 파일입력하라고 하셔서
혹시몰라서 별도로 그조건에 맞게 메인 함수(main_alt.c)따로 만들었습니다.
이건 아직 미완입니다...

compare.c에 있던 비교함수들 proc_data.h로 복사했습니다.
proc_data.h에 파일포인터 받고 데이터 읽어오는 함수(Process_load)추가했습니다.

#####3.추후 예정:31일 저녁~
gantt chart 그리는 부분, table 그리는 부분 함수화.
main_alt 마저 작성.
헤더 구조 정리
table은 response time 도 같이 출력해야할것같아요
+RR처럼 readyqueue를 따로 이용하도록 알고리즘 변경?
	readyqueue를 따로 작성하면 time을 기준으로 readyqueue내용 변경사항을 표시해주는 추가 기능을 넣으면 어떨까요