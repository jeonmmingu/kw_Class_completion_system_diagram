#pragma warning(disable:4996) // scanf, fopen 함수의 불안정성으로 인한 비활성화 issue 해결
#include "subject.h"
#define MAX_LENGTH 200

subject* load_subject_information();                                                // 데이터를 불러와 과목 객체들을 만들고 연결하는 함수
subject* connect_information(char* info);                                           // 데이터를 입력받아 객체를 만들고 연결하는 함수
void connect_prev_next_subject(subject* sub);                                       // 모든 과목 노드들의 선/후수 과목들을 연결하는 함수
subject* search_subject(char sub[], subject* root);                                 // 1차원 링크드 리스트에서 해당 과목명을 가지고 있는 노드를 찾는 함수
void print_connecting_subject(subject* sub, int school_year, int semester);         // 연결 된 선후수 과목들을 전부 출력하는 함수
void free_allocation(subject* sub);                                                 // 동적할당 해제하는 함수

int main(void){
    // 학년, 학기에 대한 변수
    int user_school_year = 0;
    int user_school_semester = 0;
    char find_subject[100] = "";
    subject* sub_ptr = NULL;
    subject* find_sub = NULL;

    // 과목 객체 연결
    sub_ptr = load_subject_information();

    // 학년 정보 입력받기 (영문, 한글 입력받아진 경우 예외처리)
    while(1){
        printf("학년을 입력하세요(1 ~ 4): ");
        scanf("%d", &user_school_year);
        rewind(stdin);
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

    // 선/후수 과목 연결
    connect_prev_next_subject(sub_ptr);

    // 화면 정리
    system("clear");

    // 해당 학년, 학기에 반드시 들어야하는 과목과 추천 과목 list-up
    print_connecting_subject(sub_ptr, user_school_year, user_school_semester);
    
    // list-up 된 과목 중 알아보고 싶은 과목을 검색 ⇒ 필수, 추천 선/후수를 구분하여 출력
    while(1){
        printf("필수 선/후수 과목 확인 (화면 초기화:clear / 종료하기:exit)\n");
        printf("과목명을 입력하세요: ");
        scanf("%s", find_subject);
        if(!strcmp(find_subject, "exit")){
            printf("\n프로그램이 종료 되었습니다.\n");
            break;
        }
        else if(!strcmp(find_subject, "clear")){
            // 해당 학년, 학기에 반드시 들어야하는 과목과 추천 과목 list-up
            system("clear");
            print_connecting_subject(sub_ptr, user_school_year, user_school_semester);
            continue;
        }
        find_sub = search_subject(find_subject, sub_ptr);
        if(!find_sub){
            printf("과목명을 바르게 입력해주십시오.\n\n");
            continue;
        }
        if(!find_sub->pNext && !find_sub->pPrevious){
            printf("해당 과목은 필수 선/후수 과목이 없습니다.\n\n");
            continue;
        }
        printf("\n[%s]과목 필수 선/후수 과목\n", find_sub->subject_name);
        while(find_sub->pPrevious)
            find_sub = find_sub->pPrevious;
        while(find_sub){
            printf("%s", find_sub->subject_name);
            if(find_sub->pNext)
                printf(" -> ");
            else
                printf("\n\n");
            find_sub= find_sub->pNext;
        }
    }

    // 동적할당 해제
    free_allocation(sub_ptr);

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
    str[strlen(str) - 2] = '\0';

    if(strcmp(str, "") != 0){
        rootNode = connect_information(str);
    }
    // printf("%d학년, %d학기, 과목명:%s, 과목타입:%s, 선수과목:%s, 후수과목:%s", rootNode->school_year, rootNode->semester, rootNode->subject_name, rootNode->subject_type, rootNode->previous_subject,rootNode->next_subject);

    prevNode = rootNode;
    while(!feof(fp)){
        fgets(str, MAX_LENGTH, fp);
        if(str[strlen(str) - 1] == '\n') 
            str[strlen(str) - 2] = '\0';
        newNode = connect_information(str);
        prevNode->next = newNode;
        prevNode = newNode;
    }
    fclose(fp);

    return rootNode;
}

subject* connect_information(char* info){
    // 새로운 정보 노드를 동적 할당
    subject *newNode = (subject*)malloc(sizeof(subject));

    // info string을 잘라서 정보를 가져옴
    char *temp = strtok(info, ","); 
    int i = 1;
    while (temp != NULL) {
        switch(i++){
            case 1: newNode->school_year = atoi(temp);
            case 2: newNode->semester = atoi(temp);
            case 3: strcpy(newNode->subject_name, temp);
            case 4: strcpy(newNode->subject_type, temp);
            case 5: 
                if(!strcmp(temp, "mr")) 
                    newNode->essential_major_in = true;
                else
                    newNode->essential_major_in = false;
            case 6: strcpy(newNode->previous_subject, temp);
            case 7: strcpy(newNode->next_subject, temp);
        }
        temp = strtok(NULL, ",");
    }
    // 정보를 담은 node를 반환
    return newNode;
}

void connect_prev_next_subject(subject* sub){
    subject* mSub = sub;
    subject* pSub = sub;
    char pre_sub[100] = {0};
    char next_sub[100] = {0};
    int i = 1;

    while(pSub){
        if(strcmp(pSub->next_subject, "none") != 0){
            pSub->pNext = search_subject(pSub->next_subject, sub);
            if(pSub->pNext == NULL) printf("Error next connect!\n");
            // printf("pSub:%s => next: %s\n", pSub->subject_name, pSub->pNext->subject_name);
        }
        if(strcmp(pSub->previous_subject, "none") != 0){
            pSub->pPrevious = search_subject(pSub->previous_subject, sub);
            if(pSub->pPrevious == NULL) printf("Error Previous connect!\n");
            // printf("pSub:%s => prev: %s\n", pSub->subject_name, pSub->pPrevious->subject_name);
        }
        pSub = pSub->next;
    }
}

subject* search_subject(char sub[], subject* root){
    int i = 1;
    while(root){
        if(!strcmp(root->subject_name, sub))
            return root;
        else
            root = root->next;
    }
    return NULL;
}

void print_connecting_subject(subject* sub, int school_year, int semester){
    int i = 1;
    bool mr_lar = false, mr_lab = false, mr_eb = false, mr_eg = false, mr_ed = false;
    bool nmr_lar = false, nmr_lab = false, nmr_eb = false, nmr_eg = false, nmr_ed = false;
    subject* pSub = sub;
    
    // 각 영역의 과목들이 있는지 체크 => 과목이 존재하는 영역들만 출력
    while(pSub){
        if(pSub->school_year==school_year&&pSub->semester == semester&&pSub->essential_major_in && !strcmp(pSub->subject_type, "lar")) mr_lar = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&pSub->essential_major_in && !strcmp(pSub->subject_type, "lab")) mr_lab = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&pSub->essential_major_in && !strcmp(pSub->subject_type, "eb")) mr_eb = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&pSub->essential_major_in && !strcmp(pSub->subject_type, "eg")) mr_eg = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&pSub->essential_major_in && !strcmp(pSub->subject_type, "ed")) mr_ed = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&!pSub->essential_major_in && !strcmp(pSub->subject_type, "lar")) nmr_lar = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&!pSub->essential_major_in && !strcmp(pSub->subject_type, "lab")) nmr_lab = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&!pSub->essential_major_in && !strcmp(pSub->subject_type, "eb")) nmr_eb = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&!pSub->essential_major_in && !strcmp(pSub->subject_type, "eg")) nmr_eg = true;
        else if(pSub->school_year==school_year&&pSub->semester == semester&&!pSub->essential_major_in && !strcmp(pSub->subject_type, "ed")) nmr_ed = true;
        pSub = pSub->next;
    }

    printf("----------------------------------------------------------------");
    printf("\n* 필수 선/후수 과목이 있는 과목의 경우 (o) 표기되어 있습니다. *\n");
    if(school_year == 1 && semester == 1){
        printf("* 대학물리학1, 대학물리학2, 대학화학및실험1 중 택 2 *\n");
    }
    // 필수 수강 과목 출력
    printf("----------------------------------------------------------------\n");
    printf("<< %d학년 %d학기 필수 수강 과목 >>\n", school_year, semester);

    if(mr_lar){
        printf("\n[필수교양]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "lar")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
                
            pSub = pSub->next;
        }
    }
    if(mr_lab){
        printf("\n[기초교양]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "lab")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }
    }
    if(mr_eb){
        printf("\n[공학기초]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "eb")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }
    }
    if(mr_eg){
        printf("\n[공학일반]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "eg")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }     
    }
    if(mr_ed){
        printf("\n[공학심화]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == true && !strcmp(pSub->subject_type, "ed")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }
    }
    if(!mr_lar&&!mr_lab&&!mr_eb&&!mr_eg&&!mr_ed)
        printf("\n필수 수강 과목이 없습니다.\n");

    // 추천 수강 과목 출력
    pSub = sub; 
    printf("\n<< %d학년 %d학기 추천 수강 과목 >>\n", school_year, semester);
    if(nmr_lar){
        printf("\n[필수교양]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "lar")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }
    }
    if(nmr_lab){
        printf("\n[기초교양]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "lab")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }
    }
    if(nmr_eb){
        printf("\n[공학기초]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "eb")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }
    }
    if(nmr_eg){
        printf("\n[공학일반]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "eg")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }
    }
    if(nmr_ed){
        printf("\n[공학심화]\n"); i = 1; pSub = sub;
        while(pSub){
            if(pSub->school_year == school_year && pSub->semester == semester && pSub->essential_major_in == false && !strcmp(pSub->subject_type, "ed")){
                if(!pSub->pNext && !pSub->pPrevious){
                    printf("%d. %s\n", i++, pSub->subject_name);
                }
                else{
                    printf("%d. %s (o)\n", i++, pSub->subject_name);
                }
            }
            pSub = pSub->next;
        }
    }
    printf("\n----------------------------------------------------------------\n");
}

void free_allocation(subject* sub){
    subject* Psub = sub->next;
    while(sub){
        free(sub);
        sub = Psub;
        if(sub)
            Psub = sub->next;
    }
}
