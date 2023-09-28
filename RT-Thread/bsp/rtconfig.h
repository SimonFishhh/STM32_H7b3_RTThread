
/* RT-Thread config file */

#ifndef __RTTHREAD_CFG_H__
#define __RTTHREAD_CFG_H__

/* RT-Thread Kernel */
#define RT_VER_NUM 0x30000
#define RT_NAME_MAX 10
#define RT_ALIGN_SIZE 4
#define RT_THREAD_PRIORITY_MAX 32
#define RT_TICK_PER_SECOND 1000
#define RT_USING_OVERFLOW_CHECK
//#define RT_USING_TIMER_SOFT
#define RT_USING_HOOK
#define RT_USING_IDLE_HOOK

/* kservice optimization */
#define RT_DEBUG
#define RT_DEBUG_COLOR

/* Inter-Thread communication */
#define RT_USING_SEMAPHORE
//#define RT_USING_MUTEX
//#define RT_USING_EVENT
//#define RT_USING_MAILBOX
//#define RT_USING_MESSAGEQUEUE

/* Memory Management */
#define RT_USING_HEAP
#define RT_USING_SMALL_MEM
//#define RT_USING_MEMHEAP
//#define RT_USING_MEMPOOL
//#define RT_USING_MEMHEAP_AS_HEAP
//#define RT_USING_MEMPOOL_AS_HEAP

/* Kernel Device Object */
#define RT_USING_DEVICE
#define RT_USING_CONSOLE
#define RT_CONSOLEBUF_SIZE 128
#define RT_CONSOLE_DEVICE_NAME "uart1"
#define RT_USING_FULL_PRINTF

/* RT-Thread Components */
#define RT_USING_COMPONENTS_INIT
#define RT_USING_USER_MAIN
#define RT_MAIN_THREAD_STACK_SIZE 1024
#define RT_USING_FINSH
#define FINSH_THREAD_NAME "tshell"
#define FINSH_THREAD_STACK_SIZE 2048
#define FINSH_THREAD_PRIORITY 21
#define FINSH_USING_HISTORY
#define FINSH_HISTORY_LINES 5
#define FINSH_USING_MSH
#define FINSH_USING_MSH_ONLY
#define FINSH_USING_SYMTAB
#define FINSH_USING_DESCRIPTION

#define RT_USING_FAL
#define FAL_DEBUG 0
#define FAL_PART_HAS_TABLE_CFG

//#define RT_USING_DFS
//#define DFS_USING_POSIX
//#define DFS_USING_WORKDIR
//#define DFS_FILESYSTEMS_MAX 2
//#define DFS_FILESYSTEM_TYPES_MAX 2
//#define DFS_FD_MAX 16

/* elm-chan's FatFs, Generic FAT Filesystem Module */
//#define RT_DFS_ELM_CODE_PAGE 437
//#define RT_DFS_ELM_WORD_ACCESS
//#define RT_DFS_ELM_USE_LFN 3
//#define RT_DFS_ELM_LFN_UNICODE 0
//#define RT_DFS_ELM_MAX_LFN 255
//#define RT_DFS_ELM_DRIVES 1
//#define RT_DFS_ELM_MAX_SECTOR_SIZE 1024
//#define RT_DFS_ELM_REENTRANT
//#define RT_DFS_ELM_MUTEX_TIMEOUT 3000
//#define RT_USING_DFS_DEVFS

/* Device Drivers */
#define RT_USING_SERIAL
#define RT_USING_QSPI

#define BSP_SCB_ENABLE_I_CACHE
#define BSP_SCB_ENABLE_D_CACHE
#define BSP_USING_ON_CHIP_FLASH
#define BSP_USING_QSPI_FLASH


#endif

