/*
________________________________________________________________________________________
				Standard Types for cortex M3 core
	standard_types.h

Ensure standard types definition compatible with 
  trampoline OSEK kernel        tpl_os_std_types.h
  stm32f10x lib                 stm32f10x_type.h

 _______________________________________________________________
// Définition de types standards
// u32 u16 u8   		unsigned 32,16,8 bits
// s32 s16 s8   		signed 32,16,8 bits
// vu32 vu16 vu8   		volatile unsigned 32,16,8 bits
// vs32 vs16 vs8   		volatile signed 32,16,8 bits
// uc32 uc16 uc8   		unsigned constant 32,16,8 bits
// sc32 sc16 sc8   		signed constant 32,16,8 bits
// vuc32 vuc16 vuc8  	volatile unsigned constant 32,16,8 bits
// vsc32 vsc16 vsc8	 	volatile signed constant 32,16,8 bits
 _______________________________________________________________________________________
USAGE
	RELPATH is the relative path from your projetc directory to lib_users directory	
   	add include "RELPATH/lib_users/standard_types.h"
	BEFORE any include about trampoline OS and stm32f10x includes
________________________________________________________________________________________
REVS
	[Acco 06/01/2010] finalisation et commentaires de la première version
_______________________________________________________________________________________
TODO
________________________________________________________________________________________
*/


// On vérifie si trampoline à déjà défini 
//  les standards dans
//   tpl_os_std_types.h
#ifndef TPL_OS_STD_TYPES_H

#ifndef __STM32F10x_TYPE_H

#define TPL_OS_STD_TYPES_H

/**
 * @typedef u8
 *
 * 8 bits unsigned number
 */
typedef unsigned char   u8;

/**
 * @typedef s8
 *
 * 8 bits signed number
 */
typedef signed char     s8;

/**
 * @typedef u16
 *
 * 16 bits unsigned number
 */
typedef unsigned short  u16;

/**
 * @typedef s16
 *
 * 16 bits signed number
 */
typedef signed short    s16;

/**
 * @typedef u32
 *
 * 32 bits unsigned number
 */
typedef unsigned int   u32;

/**
 * @typedef s32
 *
 * 32 bits signed number
 */
typedef signed int     s32;

/**
 * @typedef u64
 *
 * 64 bits unsigned number
 */
typedef unsigned long   u64;

/**
 * @typedef s64
 *
 * 64 bits signed number
 */
typedef signed long     s64;

#endif /* trampoline */
#endif //STM32lib

// On vérifie si stl32f10x lib  à déjà défini 
//  les standards dans
//   stm32f10x_type.h
#ifndef	__STM32F10x_TYPE_H
#define	__STM32F10x_TYPE_H
/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

typedef signed long  const sc32;  /* Read Only */
typedef signed short const sc16;  /* Read Only */
typedef signed char  const sc8;   /* Read Only */

typedef volatile signed long  vs32;
typedef volatile signed short vs16;
typedef volatile signed char  vs8;

typedef volatile signed long  const vsc32;  /* Read Only */
typedef volatile signed short const vsc16;  /* Read Only */
typedef volatile signed char  const vsc8;   /* Read Only */

typedef unsigned long  const uc32;  /* Read Only */
typedef unsigned short const uc16;  /* Read Only */
typedef unsigned char  const uc8;   /* Read Only */

typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;
typedef volatile unsigned char  vu8;

typedef volatile unsigned long  const vuc32;  /* Read Only */
typedef volatile unsigned short const vuc16;  /* Read Only */
typedef volatile unsigned char  const vuc8;   /* Read Only */

typedef enum {FALSE = 0, TRUE = !FALSE} bool;

typedef enum {RESET = 0, SET = !RESET} FlagStatus, ITStatus;

typedef enum {DISABLE = 0, ENABLE = !DISABLE} FunctionalState;
#define IS_FUNCTIONAL_STATE(STATE) (((STATE) == DISABLE) || ((STATE) == ENABLE))

typedef enum {ERROR = 0, SUCCESS = !ERROR} ErrorStatus;

#define U8_MAX     ((u8)255)
#define S8_MAX     ((s8)127)
#define S8_MIN     ((s8)-128)
#define U16_MAX    ((u16)65535u)
#define S16_MAX    ((s16)32767)
#define S16_MIN    ((s16)-32768)
#define U32_MAX    ((u32)4294967295uL)
#define S32_MAX    ((s32)2147483647)
#define S32_MIN    ((s32)-2147483648)

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* stm */


