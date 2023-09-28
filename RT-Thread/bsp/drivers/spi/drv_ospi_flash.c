//
// Created by Xenon on 2023/4/4.
//

#include "drv_ospi_flash.h"

int8_t OSPI_W25Qxx_Init(void)
{
    uint32_t	Device_ID;
    Device_ID = OSPI_W25Qxx_ReadID();
    if( Device_ID == W25Qxx_FLASH_ID )
    {
        return OSPI_W25Qxx_OK;
    }
    else
    {
        return W25Qxx_ERROR_INIT;
    }
}

int8_t OSPI_W25Qxx_AutoPollingMemReady(void)
{
    OSPI_RegularCmdTypeDef  sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.Address            = 0x0;
    sCommand.AddressMode        = HAL_OSPI_ADDRESS_NONE;
    sCommand.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode           = HAL_OSPI_DATA_1_LINE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.NbData             = 1;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

    sCommand.Instruction        = W25Qxx_CMD_ReadStatus_REG1;

    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }

    sConfig.Match         = 0;
    sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval      = 0x10;
    sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
    sConfig.Mask          = W25Qxx_Status_REG1_BUSY;


    if (HAL_OSPI_AutoPolling(&hospi1, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return OSPI_W25Qxx_OK;
}

uint32_t OSPI_W25Qxx_ReadID(void)
{
    OSPI_RegularCmdTypeDef  sCommand;

    uint8_t	OSPI_ReceiveBuff[3];
    uint32_t	W25Qxx_ID;

    sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressMode        = HAL_OSPI_ADDRESS_NONE;
    sCommand.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode           = HAL_OSPI_DATA_1_LINE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.NbData             = 3;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

    sCommand.Instruction        = W25Qxx_CMD_JedecID;


    HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);

    HAL_OSPI_Receive (&hospi1, OSPI_ReceiveBuff, HAL_OSPI_TIMEOUT_DEFAULT_VALUE);

    W25Qxx_ID = (OSPI_ReceiveBuff[0] << 16) | (OSPI_ReceiveBuff[1] << 8 ) | OSPI_ReceiveBuff[2];

    return W25Qxx_ID;
}

int8_t OSPI_W25Qxx_MemoryMappedMode(void)
{
    OSPI_RegularCmdTypeDef     sCommand;
    OSPI_MemoryMappedTypeDef   sMemMappedCfg;

    sCommand.OperationType           = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId                 = HAL_OSPI_FLASH_ID_1;

    sCommand.Instruction             = W25Qxx_CMD_FastReadQuad_IO;
    sCommand.InstructionMode         = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize         = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode      = HAL_OSPI_INSTRUCTION_DTR_DISABLE;

    sCommand.AddressMode             = HAL_OSPI_ADDRESS_4_LINES;
    sCommand.AddressSize             = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.AddressDtrMode          = HAL_OSPI_ADDRESS_DTR_DISABLE;

    sCommand.AlternateBytesMode      = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.AlternateBytesDtrMode   = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;

    sCommand.DataMode                = HAL_OSPI_DATA_4_LINES;
    sCommand.DataDtrMode             = HAL_OSPI_DATA_DTR_DISABLE;

    sCommand.DummyCycles             = 6;
    sCommand.DQSMode                 = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode                = HAL_OSPI_SIOO_INST_EVERY_CMD;


    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;
    }

    sMemMappedCfg.TimeOutActivation  = HAL_OSPI_TIMEOUT_COUNTER_DISABLE;
    sMemMappedCfg.TimeOutPeriod      = 0;

    if (HAL_OSPI_MemoryMapped(&hospi1,  &sMemMappedCfg) != HAL_OK)
    {
        return W25Qxx_ERROR_MemoryMapped;
    }
    return OSPI_W25Qxx_OK;
}

int8_t OSPI_W25Qxx_WriteEnable(void)
{
    OSPI_RegularCmdTypeDef  sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    sCommand.OperationType           = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId                 = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode         = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize         = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode      = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.Address                 = 0;
    sCommand.AddressMode             = HAL_OSPI_ADDRESS_NONE;
    sCommand.AddressSize             = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.AddressDtrMode          = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.AlternateBytesDtrMode   = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;
    sCommand.AlternateBytesMode      = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode                = HAL_OSPI_DATA_NONE;
    sCommand.DataDtrMode             = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DummyCycles             = 0;
    sCommand.DQSMode                 = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode                = HAL_OSPI_SIOO_INST_EVERY_CMD;

    sCommand.Instruction             = W25Qxx_CMD_WriteEnable;


    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }

    sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressMode        = HAL_OSPI_ADDRESS_NONE;
    sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;
    sCommand.DataMode           = HAL_OSPI_DATA_1_LINE;
    sCommand.NbData             = 1;

    sCommand.Instruction        = W25Qxx_CMD_ReadStatus_REG1;

    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }

    sConfig.Match         = 0x02;
    sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval      = 0x10;
    sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
    sConfig.Mask          = W25Qxx_Status_REG1_WEL;

    if (HAL_OSPI_AutoPolling(&hospi1, &sConfig,HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return OSPI_W25Qxx_OK;
}

int8_t OSPI_W25Qxx_SectorErase(uint32_t SectorAddress)
{
    OSPI_RegularCmdTypeDef  sCommand;

    sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.Address            = SectorAddress;
    sCommand.AddressMode        = HAL_OSPI_ADDRESS_1_LINE;
    sCommand.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode           = HAL_OSPI_DATA_NONE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

    sCommand.Instruction        = W25Qxx_CMD_SectorErase;

    if (OSPI_W25Qxx_WriteEnable() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }

    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }

    if (OSPI_W25Qxx_AutoPollingMemReady() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return OSPI_W25Qxx_OK;
}

int8_t OSPI_W25Qxx_BlockErase_32K (uint32_t SectorAddress)
{
    OSPI_RegularCmdTypeDef  sCommand;

    sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.Address            = SectorAddress;
    sCommand.AddressMode        = HAL_OSPI_ADDRESS_1_LINE;
    sCommand.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode           = HAL_OSPI_DATA_NONE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

    sCommand.Instruction        = W25Qxx_CMD_BlockErase_32K;

    if (OSPI_W25Qxx_WriteEnable() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }

    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }

    if (OSPI_W25Qxx_AutoPollingMemReady() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return OSPI_W25Qxx_OK;
}

int8_t OSPI_W25Qxx_BlockErase_64K (uint32_t SectorAddress)
{
    OSPI_RegularCmdTypeDef  sCommand;

    sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.Address            = SectorAddress;
    sCommand.AddressMode        = HAL_OSPI_ADDRESS_1_LINE;
    sCommand.AddressSize        = HAL_OSPI_ADDRESS_24_BITS;
    sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode           = HAL_OSPI_DATA_NONE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

    sCommand.Instruction        = W25Qxx_CMD_BlockErase_64K;

    if (OSPI_W25Qxx_WriteEnable() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }

    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }

    if (OSPI_W25Qxx_AutoPollingMemReady() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return OSPI_W25Qxx_OK;
}

int8_t OSPI_W25Qxx_ChipErase (void)
{
    OSPI_RegularCmdTypeDef  sCommand;
    OSPI_AutoPollingTypeDef sConfig;

    sCommand.OperationType      = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId            = HAL_OSPI_FLASH_ID_1;
    sCommand.InstructionMode    = HAL_OSPI_INSTRUCTION_1_LINE;
    sCommand.InstructionSize    = HAL_OSPI_INSTRUCTION_8_BITS;
    sCommand.InstructionDtrMode = HAL_OSPI_INSTRUCTION_DTR_DISABLE;
    sCommand.AddressMode        = HAL_OSPI_ADDRESS_NONE;
    sCommand.AddressDtrMode     = HAL_OSPI_ADDRESS_DTR_DISABLE;
    sCommand.AlternateBytesMode = HAL_OSPI_ALTERNATE_BYTES_NONE;
    sCommand.DataMode           = HAL_OSPI_DATA_NONE;
    sCommand.DataDtrMode        = HAL_OSPI_DATA_DTR_DISABLE;
    sCommand.DummyCycles        = 0;
    sCommand.DQSMode            = HAL_OSPI_DQS_DISABLE;
    sCommand.SIOOMode           = HAL_OSPI_SIOO_INST_EVERY_CMD;

    sCommand.Instruction        = W25Qxx_CMD_ChipErase;

    if (OSPI_W25Qxx_WriteEnable() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;
    }

    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }

    sCommand.DataMode       = HAL_OSPI_DATA_1_LINE;
    sCommand.NbData         = 1;
    sCommand.Instruction    = W25Qxx_CMD_ReadStatus_REG1;

    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }

    sConfig.Match         = 0;
    sConfig.MatchMode     = HAL_OSPI_MATCH_MODE_AND;
    sConfig.Interval      = 0x10;
    sConfig.AutomaticStop = HAL_OSPI_AUTOMATIC_STOP_ENABLE;
    sConfig.Mask          = W25Qxx_Status_REG1_BUSY;

    if (HAL_OSPI_AutoPolling(&hospi1, &sConfig,W25Qxx_ChipErase_TIMEOUT_MAX) != HAL_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;
    }
    return OSPI_W25Qxx_OK;
}

int8_t OSPI_W25Qxx_WritePage(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    OSPI_RegularCmdTypeDef  sCommand;

    sCommand.OperationType           = HAL_OSPI_OPTYPE_COMMON_CFG;
    sCommand.FlashId                 = HAL_OSPI_FLASH_ID_1;

    sCommand.Instruction             = W25Qxx_CMD_QuadInputPageProgram;        // 1-1-4Ä£Ê½ÏÂ(1ÏßÖ¸Áî1ÏßµØÖ·4ÏßÊý¾Ý)£¬Ò³±à³ÌÖ¸Áî
    sCommand.InstructionMode         = HAL_OSPI_INSTRUCTION_1_LINE;            // 1ÏßÖ¸ÁîÄ£Ê½
    sCommand.InstructionSize         = HAL_OSPI_INSTRUCTION_8_BITS;            // Ö¸Áî³¤¶È8Î»
    sCommand.InstructionDtrMode      = HAL_OSPI_INSTRUCTION_DTR_DISABLE;       // ½ûÖ¹Ö¸ÁîDTRÄ£Ê½

    sCommand.Address                 = WriteAddr;                              // µØÖ·
    sCommand.AddressMode             = HAL_OSPI_ADDRESS_1_LINE;                // 1ÏßµØÖ·Ä£Ê½
    sCommand.AddressSize             = HAL_OSPI_ADDRESS_24_BITS;               // µØÖ·³¤¶È24Î»
    sCommand.AddressDtrMode          = HAL_OSPI_ADDRESS_DTR_DISABLE;           // ½ûÖ¹µØÖ·DTRÄ£Ê½

    sCommand.AlternateBytesMode      = HAL_OSPI_ALTERNATE_BYTES_NONE;          // ÎÞ½»Ìæ×Ö½Ú
    sCommand.AlternateBytesDtrMode   = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;   // ½ûÖ¹Ìæ×Ö½ÚDTRÄ£Ê½

    sCommand.DataMode                = HAL_OSPI_DATA_4_LINES;                  // 4ÏßÊý¾ÝÄ£Ê½
    sCommand.DataDtrMode             = HAL_OSPI_DATA_DTR_DISABLE;              // ½ûÖ¹Êý¾ÝDTRÄ£Ê½
    sCommand.NbData                  = NumByteToWrite;                         // Êý¾Ý³¤¶È

    sCommand.DummyCycles             = 0;                                      // ¿ÕÖÜÆÚ¸öÊý
    sCommand.DQSMode                 = HAL_OSPI_DQS_DISABLE;                   // ²»Ê¹ÓÃDQS
    sCommand.SIOOMode                = HAL_OSPI_SIOO_INST_EVERY_CMD;           // Ã¿´Î´«ÊäÊý¾Ý¶¼·¢ËÍÖ¸Áî

    // Ð´Ê¹ÄÜ
    if (OSPI_W25Qxx_WriteEnable() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_WriteEnable;	// Ð´Ê¹ÄÜÊ§°Ü
    }
    // Ð´ÃüÁî
    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;		// ´«ÊäÊý¾Ý´íÎó
    }
    // ¿ªÊ¼´«ÊäÊý¾Ý
    if (HAL_OSPI_Transmit(&hospi1, pBuffer, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;		// ´«ÊäÊý¾Ý´íÎó
    }
    // Ê¹ÓÃ×Ô¶¯ÂÖÑ¯±êÖ¾Î»£¬µÈ´ýÐ´ÈëµÄ½áÊø
    if (OSPI_W25Qxx_AutoPollingMemReady() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;		// ÂÖÑ¯µÈ´ýÎÞÏìÓ¦
    }
    return OSPI_W25Qxx_OK; // Ð´Êý¾Ý³É¹¦
}

int8_t OSPI_W25Qxx_WriteBuffer(uint8_t* pBuffer, uint32_t WriteAddr, uint32_t Size)
{
    uint32_t end_addr, current_size, current_addr;
    uint8_t *write_data;  // ÒªÐ´ÈëµÄÊý¾Ý

    current_size = W25Qxx_PageSize - (WriteAddr % W25Qxx_PageSize); // ¼ÆËãµ±Ç°Ò³»¹Ê£ÓàµÄ¿Õ¼ä

    if (current_size > Size)	// ÅÐ¶Ïµ±Ç°Ò³Ê£ÓàµÄ¿Õ¼äÊÇ·ñ×ã¹»Ð´ÈëËùÓÐÊý¾Ý
    {
        current_size = Size;		// Èç¹û×ã¹»£¬ÔòÖ±½Ó»ñÈ¡µ±Ç°³¤¶È
    }

    current_addr = WriteAddr;		// »ñÈ¡ÒªÐ´ÈëµÄµØÖ·
    end_addr = WriteAddr + Size;	// ¼ÆËã½áÊøµØÖ·
    write_data = pBuffer;			// »ñÈ¡ÒªÐ´ÈëµÄÊý¾Ý

    do
    {
        // °´Ò³Ð´ÈëÊý¾Ý
        if(OSPI_W25Qxx_WritePage(write_data, current_addr, current_size) != OSPI_W25Qxx_OK)
        {
            return W25Qxx_ERROR_TRANSMIT;
        }

        else // °´Ò³Ð´ÈëÊý¾Ý³É¹¦£¬½øÐÐÏÂÒ»´ÎÐ´Êý¾ÝµÄ×¼±¸¹¤×÷
        {
            current_addr += current_size;	// ¼ÆËãÏÂÒ»´ÎÒªÐ´ÈëµÄµØÖ·
            write_data += current_size;	// »ñÈ¡ÏÂÒ»´ÎÒªÐ´ÈëµÄÊý¾Ý´æ´¢ÇøµØÖ·
            // ¼ÆËãÏÂÒ»´ÎÐ´Êý¾ÝµÄ³¤¶È
            current_size = ((current_addr + W25Qxx_PageSize) > end_addr) ? (end_addr - current_addr) : W25Qxx_PageSize;
        }
    }
    while (current_addr < end_addr) ; // ÅÐ¶ÏÊý¾ÝÊÇ·ñÈ«²¿Ð´ÈëÍê±Ï

    return OSPI_W25Qxx_OK;	// Ð´ÈëÊý¾Ý³É¹¦
}

int8_t OSPI_W25Qxx_ReadBuffer(uint8_t* pBuffer, uint32_t ReadAddr, uint32_t NumByteToRead)
{
    OSPI_RegularCmdTypeDef  sCommand;// OSPI´«ÊäÅäÖÃ

    sCommand.OperationType           = HAL_OSPI_OPTYPE_COMMON_CFG;             // Í¨ÓÃÅäÖÃ
    sCommand.FlashId                 = HAL_OSPI_FLASH_ID_1;                    // flash ID

    sCommand.Instruction             = W25Qxx_CMD_FastReadQuad_IO;             // 1-4-4Ä£Ê½ÏÂ(1ÏßÖ¸Áî4ÏßµØÖ·4ÏßÊý¾Ý)£¬¿ìËÙ¶ÁÈ¡Ö¸Áî
    sCommand.InstructionMode         = HAL_OSPI_INSTRUCTION_1_LINE;            // 1ÏßÖ¸ÁîÄ£Ê½
    sCommand.InstructionSize         = HAL_OSPI_INSTRUCTION_8_BITS;            // Ö¸Áî³¤¶È8Î»
    sCommand.InstructionDtrMode      = HAL_OSPI_INSTRUCTION_DTR_DISABLE;       // ½ûÖ¹Ö¸ÁîDTRÄ£Ê½

    sCommand.Address                 = ReadAddr;                               // µØÖ·
    sCommand.AddressMode             = HAL_OSPI_ADDRESS_4_LINES;               // 4ÏßµØÖ·Ä£Ê½
    sCommand.AddressSize             = HAL_OSPI_ADDRESS_24_BITS;               // µØÖ·³¤¶È24Î»
    sCommand.AddressDtrMode          = HAL_OSPI_ADDRESS_DTR_DISABLE;           // ½ûÖ¹µØÖ·DTRÄ£Ê½

    sCommand.AlternateBytesMode      = HAL_OSPI_ALTERNATE_BYTES_NONE;          // ÎÞ½»Ìæ×Ö½Ú
    sCommand.AlternateBytesDtrMode   = HAL_OSPI_ALTERNATE_BYTES_DTR_DISABLE;   // ½ûÖ¹Ìæ×Ö½ÚDTRÄ£Ê½

    sCommand.DataMode                = HAL_OSPI_DATA_4_LINES;                  // 4ÏßÊý¾ÝÄ£Ê½
    sCommand.DataDtrMode             = HAL_OSPI_DATA_DTR_DISABLE;              // ½ûÖ¹Êý¾ÝDTRÄ£Ê½
    sCommand.NbData                  = NumByteToRead;                          // Êý¾Ý³¤¶È

    sCommand.DummyCycles             = 6;                                      // ¿ÕÖÜÆÚ¸öÊý
    sCommand.DQSMode                 = HAL_OSPI_DQS_DISABLE;                   // ²»Ê¹ÓÃDQS
    sCommand.SIOOMode                = HAL_OSPI_SIOO_INST_EVERY_CMD;           // Ã¿´Î´«ÊäÊý¾Ý¶¼·¢ËÍÖ¸Áî

    // Ð´ÃüÁî
    if (HAL_OSPI_Command(&hospi1, &sCommand, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;		// ´«ÊäÊý¾Ý´íÎó
    }
    //	½ÓÊÕÊý¾Ý
    if (HAL_OSPI_Receive(&hospi1, pBuffer, HAL_OSPI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
    {
        return W25Qxx_ERROR_TRANSMIT;		// ´«ÊäÊý¾Ý´íÎó
    }
    // Ê¹ÓÃ×Ô¶¯ÂÖÑ¯±êÖ¾Î»£¬µÈ´ý½ÓÊÕµÄ½áÊø
    if (OSPI_W25Qxx_AutoPollingMemReady() != OSPI_W25Qxx_OK)
    {
        return W25Qxx_ERROR_AUTOPOLLING;		// ÂÖÑ¯µÈ´ýÎÞÏìÓ¦
    }
    return OSPI_W25Qxx_OK;	// ¶ÁÈ¡Êý¾Ý³É¹¦
}
