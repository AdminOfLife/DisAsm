/*
* Filename: StrAsm.c
* Author:   DisAsmCompany
* Date:     21/09/2013
*
* Description: StrAsm API implementation
*              
*              
*
*/

#include <stdio.h>
#include <stdlib.h>

#include "../DisAsm/DisAsm"
#define _STR_ENUM
#undef  __MNEMONIC_H__
#undef  __REGISTER_H__
#include "../DisAsm/DisAsm"
#include "StrAsm"

void PrintByte(uint8_t value)
{
	printf("%X%X", value >> 4, value & 0xF);
}

uint8_t IsNegative(uint32_t value, uint8_t size)
{
	return !!(value & (0x80 << 8 * (size - 1)));
}

uint32_t Inverse(uint32_t value, uint8_t size)
{
	if (size == 4)
	{
		value = 0xFFFFFFFFUL - value;
	}
	if (size == 2)
	{
		value = 0xFFFFUL - value;
	}
	if (size == 1)
	{
		value = 0xFFUL - value;
	}
	return value + 1;
}

void PrintValue(uint32_t value)
{
	if (value > 0xFFFFUL)
	{
		PrintByte((value >> 24) & 0xFF);
		PrintByte((value >> 16) & 0xFF);
	}
	if (value > 0xFF)
	{
		PrintByte((value >> 8) & 0xFF);
	}
	PrintByte(value & 0xFF);
	printf("h");
}

void PrintOperand(InstructionInfo * pInfo, Operand * pOperand)
{
	if (pOperand->type == Reg)
	{
		char * reg = RegisterToString(pOperand->value.reg);
		if (pOperand->memory)
		{
			printf("[");

			if (pOperand->scale > 1)
			{
				printf("%d * ", pOperand->scale);
			}
		}
		printf("%s", reg);
		if (pOperand->memory)
		{
			if (pOperand->hasBase)
			{
				char * base = RegisterToString(pOperand->base);
				printf(" + %s", base);
			}
			if (pInfo->hasDisp)
			{
				if (IsNegative(pInfo->disp, pInfo->sizeDisp))
				{
					printf(" - ");
					PrintValue(Inverse(pInfo->disp, pInfo->sizeDisp));
				}
				else
				{
					printf(" + ");
					PrintValue(pInfo->disp);
				}
			}
			printf("]");
		}
	}
	if (HITYPE(pOperand->type) == J)
	{
		PrintValue(pInfo->imm);
	}
	if (HITYPE(pOperand->type) == I)
	{
		if (IsNegative(pInfo->imm, pInfo->sizeImm))
		{
			printf("-");
			PrintValue(Inverse(pInfo->imm, pInfo->sizeImm));
		}
		else
		{
			PrintValue(pInfo->imm);
		}
	}
}

void StrAsmPrintInstruction(InstructionInfo * pInfo)
{
	 char * mnemonic = MnemonicToString(pInfo->mnemonic);
	 printf("%s", mnemonic);

	 if (pInfo->nOperands > 0)
	 {
		 printf(" ");
		 PrintOperand(pInfo, &pInfo->operands[0]);
		 
		 if (pInfo->nOperands > 1)
		 {
			 printf(", ");
			 PrintOperand(pInfo, &pInfo->operands[1]);
			 if (pInfo->nOperands > 2)
			 {
				 printf(", ");
				 PrintOperand(pInfo, &pInfo->operands[2]);
			 }
		 }
	 }
	 printf("\n");
}
