#pragma warning(disable:4996)	// scanf, fopen 함수의 불안정성으로 인한 비활성화 방지 코드
#include "subject.h"
#define MAX_LENGTH 200

subject* load_subject_information();	// .txt 파일을 불러와서 객체를 생성하는 함수
subject* connect_information(char* info);	// 데이터를 입력받아서 객체를 연결하는 함수
subject* search_subject(char sub[], subject* root);	// 1차원 링크드 리스트에서 입력한 교과목을 찾는 함수

void print_start_Window(int* user_school_year, int* user_school_semester);		// 시작 화면을 출력하고 학년•학기 정보를 입력받는 함수
void connect_prev_next_subject(subject* sub);	// 모든 교과목의 선후수 교과목을 연결하는 함수
void print_connecting_subject(subject* sub, int school_year, int semester);	// 연결된 선후수 교과목을 전부 출력하는 함수
void free_allocation(subject* sub);	// 동적할당을 해제하는 함수

int main(void) {
	// 학년•학기와 교과목 변수
	int user_school_year = 0;
	int user_school_semester = 0;
	char find_subject[MAX_LENGTH] = "";
	subject* sub_ptr = NULL;
	subject* find_sub = NULL;

	// 객체 생성 및 연결
	sub_ptr = load_subject_information();

	// 시작 화면 출력
	system("mode con: lines=50 cols=85");
	print_start_Window(&user_school_year, &user_school_semester);

	// 선후수 교과목 연결
	connect_prev_next_subject(sub_ptr);

	// 화면 초기화
	system("cls");

	// 해당 학년•학기의 전공필수 교과목과 그 외의 것을 출력
	print_connecting_subject(sub_ptr, user_school_year, user_school_semester);

	// 입력한 교과목의 필수 선후수 교과목을 출력
	while (1) {
		printf("              > 과목명을 입력하세요: ");
		scanf("%s", find_subject);
		if (!strcmp(find_subject, "exit")) {
			printf("              > 프로그램이 종료되었습니다!\n\n");
			break;
		}
		else if (!strcmp(find_subject, "clear")) {
			system("cls");
			print_connecting_subject(sub_ptr, user_school_year, user_school_semester);
			continue;
		}
		else if (!strcmp(find_subject, "back")) {
			system("cls");
			print_start_Window(&user_school_year, &user_school_semester);
			system("cls");
			print_connecting_subject(sub_ptr, user_school_year, user_school_semester);
			continue;
		}

		find_sub = search_subject(find_subject, sub_ptr);
		if (!find_sub) {
			printf("              > 과목명을 바르게 입력하세요!\n\n");
			continue;
		}
		if (!find_sub->pPrevious && !find_sub->pNext) {
			printf("              > 해당 과목은 선후수 과목이 없습니다!\n\n");
			continue;
		}
		printf("              > ");

		while (find_sub->pPrevious) find_sub = find_sub->pPrevious;
		while (find_sub) {
			printf("%s", find_sub->subject_name);
			if (find_sub->pNext) printf(" -> ");
			else printf("\n\n");
			find_sub = find_sub->pNext;
		}
	}

	// 동적할당 해제
	free_allocation(sub_ptr);

	// 종료
	return 0;
}

subject* load_subject_information() {
	// 노드
	subject* rootNode = NULL;
	subject* newNode = NULL;
	subject* prevNode = NULL;

	// .txt 파일을 불러와서 객체를 생성
	FILE* fp;
	fp = fopen("subjectInfo.txt", "r");
	if (fp == NULL) {
		printf("              * error!\n");
		return NULL;
	}
	char str[MAX_LENGTH] = { 0 };
	fgets(str, MAX_LENGTH, fp);
	str[strlen(str) - 1] = '\0';
	if (strcmp(str, "") != 0) rootNode = connect_information(str);

	prevNode = rootNode;
	while (!feof(fp)) {
		fgets(str, MAX_LENGTH, fp);
		if (str[strlen(str) - 1] == '\n') str[strlen(str) - 1] = '\0';
		newNode = connect_information(str);
		prevNode->next = newNode;
		prevNode = newNode;
	}
	fclose(fp);

	return rootNode;
}

subject* connect_information(char* info) {
	// 데이터를 입력받아서 객체를 연결
	subject* newNode = (subject*)malloc(sizeof(subject));
	newNode->next = NULL;
	newNode->pNext = NULL;
	newNode->pPrevious = NULL;

	char* temp = strtok(info, ",");
	int i = 1;
	while (temp != NULL) {
		switch (i++) {
		case 1: newNode->school_year = atoi(temp);
		case 2: newNode->semester = atoi(temp);
		case 3: strcpy(newNode->subject_name, temp);
		case 4: strcpy(newNode->subject_type, temp);
		case 5:
			if (!strcmp(temp, "mr")) newNode->essential_major_in = true;
			else newNode->essential_major_in = false;
		case 6: strcpy(newNode->previous_subject, temp);
		case 7: strcpy(newNode->next_subject, temp);
		}
		temp = strtok(NULL, ",");
	}

	return newNode;
}

subject* search_subject(char sub[], subject* root) {
	// 1차원 링크드 리스트에서 입력한 교과목을 찾기
	while (root) {
		if (!strcmp(root->subject_name, sub)) return root;
		else root = root->next;
	}

	return NULL;
}

void print_start_Window(int* user_school_year, int* user_school_semester) {
	// 학년•학기 초기화
	*user_school_year = 0;
	*user_school_semester = 0;

	// 학년 입력(영문, 한글을 입력받으면 예외처리)
	printf("  ==============================================================================\n");
	printf("||                                                                              ||\n");
	printf("||            ## 컴퓨터정보공학부 학년별 이수체계도 안내 프로그램 ##            ||\n");
	printf("||                                                                              ||\n");
	printf("||            * 2022학번을 기준으로 한 프로그램입니다                           ||\n");
	printf("||                                                                              ||\n");
	printf("||            * programmed by. 2019202040 전민구 & 2019202057 서현준            ||\n");
	printf("||                                                                              ||\n");
	printf("  ==============================================================================\n\n");
	while (1) {
		printf("              > '학년'을 입력하세요(1~4): ");
		scanf("%d", user_school_year);
		rewind(stdin);
		if (*user_school_year < 5 && *user_school_year > 0) break;
		else printf("              > 1~4 중 하나를 입력하세요!\n\n");
	}
	printf("\n\n");

	// 학기 입력
	while (1) {
		printf("              > '학기'를 입력하세요(1~2): ");
		scanf("%d", user_school_semester);
		rewind(stdin);
		if (*user_school_semester == 1 || *user_school_semester == 2) break;
		else printf("              > 1 또는 2를 입력하세요!\n\n");
	}
}

void connect_prev_next_subject(subject* sub) {
	// 모든 교과목의 선후수 교과목을 연결
	subject* pSub = sub;
	char pre_sub[MAX_LENGTH] = { 0 };
	char next_sub[MAX_LENGTH] = { 0 };
	int i = 1;

	while (pSub) {
		if (strcmp(pSub->next_subject, "none") != 0) {
			pSub->pNext = search_subject(pSub->next_subject, sub);
			if (pSub->pNext == NULL) printf("              * error!\n");
		}
		if (strcmp(pSub->previous_subject, "none") != 0) {
			pSub->pPrevious = search_subject(pSub->previous_subject, sub);
			if (pSub->pPrevious == NULL) printf("              * error!\n");
		}
		pSub = pSub->next;
	}
}

void print_connecting_subject(subject* sub, int school_year, int semester) {
	// 연결된 선후수 교과목을 전부 출력
	int i = 1;
	bool mr_lar = false, mr_lab = false, mr_eb = false, mr_eg = false, mr_ed = false;
	bool nmr_lar = false, nmr_lab = false, nmr_eb = false, nmr_eg = false, nmr_ed = false;
	subject* pSub = sub;

	// 필수 수강 교과목인지 체크
	while (pSub) {
		if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in && !strcmp(pSub->subject_type, "lar")) mr_lar = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in && !strcmp(pSub->subject_type, "lab")) mr_lab = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in && !strcmp(pSub->subject_type, "eb")) mr_eb = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in && !strcmp(pSub->subject_type, "eg")) mr_eg = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in && !strcmp(pSub->subject_type, "ed")) mr_ed = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && !pSub->essential_major_in && !strcmp(pSub->subject_type, "lar")) nmr_lar = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && !pSub->essential_major_in && !strcmp(pSub->subject_type, "lab")) nmr_lab = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && !pSub->essential_major_in && !strcmp(pSub->subject_type, "eb")) nmr_eb = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && !pSub->essential_major_in && !strcmp(pSub->subject_type, "eg")) nmr_eg = true;
		else if (pSub->school_year == school_year && pSub->semester == semester && !pSub->essential_major_in && !strcmp(pSub->subject_type, "ed")) nmr_ed = true;
		pSub = pSub->next;
	}

	printf("  ==============================================================================  \n");
	printf("||                                 %d학년 %d학기                                  ||\n", school_year, semester);
	printf("||==============================================================================||\n");
	printf("||    * 필수 선후수 교과목이 존재하는 경우, 교과목명 옆에 (o)를 표시했습니다    ||\n");
	if (school_year == 1 && (semester == 1 || semester == 2))
		printf("||    * 대학물리학1, 대학물리학2, 대학화학및실험1 중 하나를 선택하세요          ||\n");
	if (school_year == 4 && (semester == 1 || semester == 2))
		printf("||    * '산학협력캡스톤설계' 교과목은 1과 2를 구분하지 않았습니다               ||\n");
	printf("||==============================================================================||\n");

	// 필수 수강 교과목 출력
	printf("||                            >  필수 수강 교과목  <                            ||\n");
	printf("||------------------------------------------------------------------------------||\n");
	if (mr_lar || nmr_lar) {
		printf("||                                                                              ||\n");
		printf("||                                  [필수교양]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1;  pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "lar")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "lar")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	if (mr_lab) {
		printf("||                                                                              ||\n");
		printf("||                                  [기초교양]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1; pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "lab")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	if (mr_eb) {
		printf("||                                                                              ||\n");
		printf("||                                  [공학기초]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1; pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "eb")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	if (mr_eg) {
		printf("||                                                                              ||\n");
		printf("||                                  [공학일반]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1; pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "eg")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	if (mr_ed) {
		printf("||                                                                              ||\n");
		printf("||                                  [공학심화]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1; pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "ed")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	if (!mr_lar && !mr_lab && !mr_eb && !mr_eg && !mr_ed && !nmr_lar) {
		printf("||                                                                              ||\n");
		printf("||                          필수 수강 교과목이 없습니다                         ||\n");
	}

	// 그 외의 교과목 출력
	printf("||                                                                              ||\n");
	printf("||==============================================================================||\n");
	printf("||                            >  추천 수강 교과목  <                            ||\n");
	printf("||------------------------------------------------------------------------------||\n");
	if (nmr_lab) {
		printf("||                                                                              ||\n");
		printf("||                                  [기초교양]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1; pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "lab")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	if (nmr_eb) {
		printf("||                                                                              ||\n");
		printf("||                                  [공학기초]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1; pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "eb")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	if (nmr_eg) {
		printf("||                                                                              ||\n");
		printf("||                                  [공학일반]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1; pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "eg")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	if (nmr_ed) {
		printf("||                                                                              ||\n");
		printf("||                                  [공학심화]                                  ||\n");
		printf("||                                                                              ||\n");
		i = 1; pSub = sub;
		while (pSub) {
			if (pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "ed")) {
				if (!pSub->pNext && !pSub->pPrevious)
					printf("||                         %d. %-50s||\n", i++, pSub->subject_name);
				else printf("||                         %d. %-30s (o)                ||\n", i++, pSub->subject_name);
			}
			pSub = pSub->next;
		}
	}
	printf("||                                                                              ||\n");
	printf("||==============================================================================||\n");
	printf("||               화면 초기화: clear / 뒤로 가기: back / 종료: exit              ||\n");
	printf("  ==============================================================================\n\n");
	printf("                          << 필수 선후수 교과목 확인 >>                          \n\n");
}

void free_allocation(subject* sub) {
	// 동적할당 해제
	subject* Psub = sub->next;

	while (sub) {
		free(sub);
		sub = Psub;
		if (sub) Psub = sub->next;
	}
}