#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

typedef struct subject
{
	// Basic information
	int school_year;	// 학년
	int semester;	// 학기
	char subject_name[100];	// 과목명

	// Subject type
	char subject_type[100];

	// 필수 수강 교과목 체크
	bool essential_major_in;

	// 선후수 교과목
	char previous_subject[100];
	char next_subject[100];

	// 선후수 교과목 포인터
	struct subject* pPrevious;
	struct subject* pNext;

	// 다음 노드를 가리키는 포인터
	struct subject* next;

} subject;