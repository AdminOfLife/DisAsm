/*
* Filename: DisAsm.h
* Author:   DisAsmCompany
* Date:     21/09/2013
*
* Description: DisAsm API entry points :
*              public functions to be
*              called by users.
*
*/

#pragma once
#ifndef __DISASM_H__
#define __DISASM_H__

typedef void * HDISASM;

HDISASM DisAsmCreate();
void DisAsmDestroy(HDISASM hDisAsm);

uint8_t DisAsmInstructionDecode(HDISASM hDisAsm, uint8_t * buffer, InstructionInfo * pInfo);

/* for tests */

OpCode _ChooseOpCode(uint8_t * buffer);

#endif /* __DISASM_H__ */