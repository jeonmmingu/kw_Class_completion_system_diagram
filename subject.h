#pragma once
#include <stdio.h>
#include <stdbool.h>

typedef struct
{
    // Basic information
    int school_year;                    // 학년
    int semester;                       // 학기
    char subject_name[30];            // 과목명
    
    // Subject type
    bool major_required;            // 전공필수
    bool engineering_general;       // 공학일반
    bool engineering_fundamentals;  // 공학기초
    bool engineering_deepening;     // 공학심화
    
    // 선후수 과목 포인터
    struct subject* previous;
    struct subject* next;

} subject;
