#pragma warning(disable:4996) // scanf, fopen 함수의 불안정성으로 인한 비활성화 issue 해결
#include "subject.h"
#define MAX_LENGTH 200

subject* load_subject_information();                // 데이터를 불러와 과목 객체들을 만들고 연결하는 함수
subject* connect_information(char* info);           // 데이터를 입력받아 객체를 만들고 연결하는 함수
void print_connecting_subject(subject* sub);        // 연결 된 선후수 과목들을 전부 출력하는 함수

int main(void){
    // 학년, 학기에 대한 변수
    int user_school_year = 0;
    int user_school_semester = 0;
    subject* sub_ptr = NULL;

    // 선후수 데이터 불러오기 & 과목 객체 연결
    sub_ptr = load_subject_information();

    // 학년 정보 입력받기 (영문, 한글 입력받아진 경우 예외처리)
    while(1){
        printf("학년을 입력하세요(1 ~ 4): ");
        scanf("%d", &user_school_year);
        if(user_school_year < 5 && user_school_year > 0) break;
        else printf("(1학년 ~ 4학년 중 입력해주시길 바랍니다.)\n");
    }

    // 학기 정보 입력받기
    while(1){
        printf("학기를 입력하세요(1 or 2): ");
        scanf("%d", &user_school_semester);
        if(user_school_semester == 1  || user_school_semester == 2) break;
        else printf("(1학기, 2학기 중 하나를 입력해주시길 바랍니다.)\n");
    }

    // 연결 내용 확인
    print_connecting_subject(sub_ptr);

    // 해당 학년, 학기에 반드시 들어야하는 과목과 추천 과목 list-up

    // list-up 된 과목 중 알아보고 싶은 과목을 검색 ⇒ 필수, 추천 선/후수를 구분하여 출력
    
    // Done
    return 0;
}

subject* load_subject_information(){
    // Node declare
    subject* rootNode = NULL;
    subject* newNode = NULL;
    subject* prevNode = NULL;

    // 파일을 읽어와 데이터를 객체로 만들어 저장
    FILE* fp;
    fp = fopen("subjectInfo.txt", "r");
    if(fp == NULL){
        printf("File don't exist.\n");
        return NULL;
    }
    char str[MAX_LENGTH] = {0};
    fgets(str, MAX_LENGTH, fp);

    if(strcmp(str, "") != 0){
        rootNode = connect_information(str);
    }
    // printf("%d학년, %d학기, 과목명:%s, 과목타입:%s, 선수과목:%s, 후수과목:%s", rootNode->school_year, rootNode->semester, rootNode->subject_name, rootNode->subject_type, rootNode->previous_subject,rootNode->next_subject);
    
    prevNode = rootNode;
    while(!feof(fp)){
        fgets(str, MAX_LENGTH, fp);
        newNode = connect_information(str);
        prevNode->next = newNode;
        prevNode = newNode;
    }
    fclose(fp);

    return rootNode;
}

subject* connect_information(char* info){
    // 새로운 정보 노드를 동적 할당
    subject *newNode = malloc(sizeof(subject));

    // info string을 잘라서 정보를 가져옴
    // 학년,학기,과목명,subject_type(mr-전필, eg-공학일반, ef-공학기초, ed-공학심화, bg-기초교양, rg-필수교양),필수 선수과목명,필수 후수과목명
    char *temp = strtok(info, ","); 
    int i = 1;
    while (temp != NULL) {
        switch(i++){
            case 1: newNode->school_year = atoi(temp);
            case 2: newNode->semester = atoi(temp);
            case 3: strcpy(newNode->subject_name, temp);
            case 4: strcpy(newNode->subject_type, temp);
            case 5: strcpy(newNode->previous_subject, temp);
            case 6: strcpy(newNode->next_subject, temp);
        }
        temp = strtok(NULL, ",");
    }
    // 정보를 담은 node를 반환
    return newNode;
}

void print_connecting_subject(subject* sub){
    while(sub){
        printf("%d학년, %d학기, 과목명:%s, 과목타입:%s, 선수과목:%s, 후수과목:%s", sub->school_year, sub->semester, sub->subject_name, sub->subject_type, sub->previous_subject,sub->next_subject);
        sub = sub->next;
    }
}

