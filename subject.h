#pragma once
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <windows.h>

typedef struct subject
{
	// Basic information
	int school_year;	// �г�
	int semester;	// �б�
	char subject_name[100];	// �����

	// Subject type
	char subject_type[100];

	// �ʼ� ���� ������ üũ
	bool essential_major_in;

	// ���ļ� ������
	char previous_subject[100];
	char next_subject[100];

	// ���ļ� ������ ������
	struct subject* pPrevious;
	struct subject* pNext;

	// ���� ��带 ����Ű�� ������
	struct subject* next;

} subject;