/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "LCD.h"
#include "GPIO.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define LCD_TOTAL_PINS_NUM __NUM_OF_DATA_LINES + 3
#define LCD_USER_STATE_BUSY 1
#define LCD_USER_STATE_READY 0
#define LCD_REQ_TYPE_WRITE 0
#define LCD_REQ_TYPE_CLEAR 1
#define LCD_REQ_TYPE_SET_P 2
#define LCD_MAX_BUFFER_SIZE 5
#define STATIC_STATE_BUSY 0
#define STATIC_STATE_READY 1
#define COMMAND_NIBBLE_HIGH 1
#define COMMAND_NIBBLE_LOW 0
#define WRITE_COMMAND_STATE 0
#define WRITE_DATA_STATE 1

#define LCD_8_PIN_COMMAND_FUNC_SET 0x30
#define LCD_8_PIN_COMMAND_ONOFF 0x08
#define LCD_8_PIN_COMMAND_CLEAR 0x01
#define LCD_8_PIN_COMMAND_ENTRY 0x04
#define LCD_4_PIN_COMMAND_1_ST_FUNC_SET 0x02
#define LCD_4_PIN_COMMAND_2_ND_FUNC_SET 0x20
#define LCD_4_PIN_COMMAND_ONOFF 0x00
#define LCD_4_PIN_COMMAND_CLEAR 0x01
#define LCD_4_PIN_COMMAND_ENTRY 0x00
#define LCD_COMMAND_GOTOXY 128
#define LCD_SECOND_LINE_OFFSET 0x40

#define LCD_1_BIT_OFFSET 1
#define LCD_2_BIT_OFFSET 2
#define LCD_3_BIT_OFFSET 3
#define LCD_4_BIT_OFFSET 4

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
typedef struct
{
    const uint8_t *U_string;
    uint8_t U_length;
    uint8_t U_State;
    uint8_t U_Type;
} User_Req_t;

typedef struct
{
    uint8_t Curr_Pos;
    CallBack_t CallBack;
} Write_Req_t;

typedef struct
{
    CallBack_t CallBack;
} Clear_Req_t;

typedef struct
{
    uint8_t X_Position;
    uint8_t Y_Position;
    CallBack_t CallBack;
} SetCursor_Req_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
extern LCD_cfg_t LCDs_PinCfg;
static uint8_t LCD_State = LCD_STATE_OFF;
static uint8_t LCD_DigitCounter = 0;
static User_Req_t User_Resquest[LCD_MAX_BUFFER_SIZE];
static uint8_t User_CurrentRequest = 0;
static Write_Req_t Write_Request;
static Clear_Req_t Clear_Request;
static SetCursor_Req_t SetP_Request;
/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void LCD_WriteToPins(uint8_t Info, uint8_t State);
static void Init_State_Func();
static void OperationState_WriteFunc();
static void OperationState_ClearFunc();
// static void OperationState_SetPOSFunc();

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

HALStatus_t LCD_Init()
{
    HALStatus_t LOC_Status = HAL_NOK;
    MCALStatus_t LOC_MStatus = MCAL_ERROR;

    GPIO_StrCfg_t gpio_LcdPins[LCD_TOTAL_PINS_NUM];
    uint8_t Index;

    for (Index = 0; Index < __NUM_OF_DATA_LINES; Index++)
    {
        gpio_LcdPins[Index].pin = LCDs_PinCfg.LCD_data_pins[Index].pin;
        gpio_LcdPins[Index].port = LCDs_PinCfg.LCD_data_pins[Index].port;
        gpio_LcdPins[Index].mode = GPIO_MODE_Output;
        gpio_LcdPins[Index].out_type = GPIO_OUTPUT_PushPull;
        gpio_LcdPins[Index].speed = GPIO_SPEED_Medium;
    }
    gpio_LcdPins[__NUM_OF_DATA_LINES].pin = LCDs_PinCfg.E_pin.pin;
    gpio_LcdPins[__NUM_OF_DATA_LINES].port = LCDs_PinCfg.E_pin.port;
    gpio_LcdPins[__NUM_OF_DATA_LINES].mode = GPIO_MODE_Output;
    gpio_LcdPins[__NUM_OF_DATA_LINES].out_type = GPIO_OUTPUT_PushPull;
    gpio_LcdPins[__NUM_OF_DATA_LINES].speed = GPIO_SPEED_Medium;

    gpio_LcdPins[__NUM_OF_DATA_LINES + 1].pin = LCDs_PinCfg.R_S_pin.pin;
    gpio_LcdPins[__NUM_OF_DATA_LINES + 1].port = LCDs_PinCfg.R_S_pin.port;
    gpio_LcdPins[__NUM_OF_DATA_LINES + 1].mode = GPIO_MODE_Output;
    gpio_LcdPins[__NUM_OF_DATA_LINES + 1].out_type = GPIO_OUTPUT_PushPull;
    gpio_LcdPins[__NUM_OF_DATA_LINES + 1].speed = GPIO_SPEED_Medium;

    gpio_LcdPins[__NUM_OF_DATA_LINES + 2].pin = LCDs_PinCfg.R_W_pin.pin;
    gpio_LcdPins[__NUM_OF_DATA_LINES + 2].port = LCDs_PinCfg.R_W_pin.port;
    gpio_LcdPins[__NUM_OF_DATA_LINES + 2].mode = GPIO_MODE_Output;
    gpio_LcdPins[__NUM_OF_DATA_LINES + 2].out_type = GPIO_OUTPUT_PushPull;
    gpio_LcdPins[__NUM_OF_DATA_LINES + 2].speed = GPIO_SPEED_Medium;

    LOC_MStatus = GPIO_Init(gpio_LcdPins, (__NUM_OF_DATA_LINES + 3));

    LCD_State = LCD_STATE_INIT;

    return (LOC_Status | LOC_MStatus);
}

HALStatus_t LCD_WriteStringAsync(uint8_t *string, uint8_t length, CallBack_t CB)
{
    uint8_t Index;
    HALStatus_t LOC_Status = HAL_NOK;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            User_Resquest[Index].U_string = string;
            User_Resquest[Index].U_length = length;
            Write_Request.CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_WRITE;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;

            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = HAL_LCD_FULL_BUFFER;
    }
    else
    {
        LOC_Status = HAL_OK;
    }

    return LOC_Status;
}

HALStatus_t LCD_SetCursorAsync(uint8_t X_pos, uint8_t Y_pos, CallBack_t CB)
{
    uint8_t Index;
    HALStatus_t LOC_Status = HAL_NOK;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            SetP_Request.X_Position = X_pos;
            SetP_Request.Y_Position = Y_pos;
            SetP_Request.CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_SET_P;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;
            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = HAL_LCD_FULL_BUFFER;
    }
    else
    {

        LOC_Status = HAL_OK;
    }

    return LOC_Status;
}

HALStatus_t LCD_ClearScreenAsync(CallBack_t CB)
{
    HALStatus_t LOC_Status = HAL_NOK;
    uint8_t Index;

    for (Index = 0; Index < LCD_MAX_BUFFER_SIZE; Index++)
    {
        if (User_Resquest[Index].U_State == LCD_USER_STATE_READY)
        {
            Clear_Request.CallBack = CB;
            User_Resquest[Index].U_Type = LCD_REQ_TYPE_CLEAR;
            User_Resquest[Index].U_State = LCD_USER_STATE_BUSY;
            break;
        }
    }
    if (Index == LCD_MAX_BUFFER_SIZE)
    {
        LOC_Status = HAL_LCD_FULL_BUFFER;
    }
    else
    {
        LOC_Status = HAL_OK;
    }

    return LOC_Status;
}

#if __NUM_OF_DATA_LINES == DATA_8_PINS
static void Init_State_Func()
{
    static uint8_t Init_Call_Count = 0;

    uint8_t LOC_FunctionSetCommand = LCD_8_PIN_COMMAND_FUNC_SET;
    uint8_t LOC_DisplayOnOffCommand = LCD_8_PIN_COMMAND_ONOFF;
    uint8_t LOC_DisplayClearCommand = LCD_8_PIN_COMMAND_CLEAR;
    uint8_t LOC_EntryModeCommand = LCD_8_PIN_COMMAND_ENTRY;

    /*set the font and number of lines configuration to the command*/
    LOC_FunctionSetCommand |= (FONT_SIZE << LCD_2_BIT_OFFSET);
    LOC_FunctionSetCommand |= (NUMBER_OF_LINES << LCD_3_BIT_OFFSET);

    /*set the blink, cursor and display state configuration to the command*/
    LOC_DisplayOnOffCommand |= (BLINK_STATE);
    LOC_DisplayOnOffCommand |= (CUSOR_STATE << LCD_1_BIT_OFFSET);
    LOC_DisplayOnOffCommand |= (DISPLAY_STATE << LCD_2_BIT_OFFSET);

    /*set the font and number of lines configuration to the command*/
    LOC_EntryModeCommand |= (SHIFT_STATE);
    LOC_EntryModeCommand |= (INCREMENT_STATE << LCD_1_BIT_OFFSET);

    if (Init_Call_Count < 2)
    {
        /*call the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else if (Init_Call_Count < 4)
    {
        /*call the display on/off command*/
        LCD_WriteToPins(LOC_DisplayOnOffCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else if (Init_Call_Count < 6)
    {
        /*call the display clear command*/
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else if (Init_Call_Count < 8)
    {
        /*call the entry mode set command*/
        LCD_WriteToPins(LOC_EntryModeCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else
    {
        LCD_State = LCD_STATE_OPER;
        Init_Call_Count = 0;
    }
}
#endif

#if __NUM_OF_DATA_LINES == DATA_4_PINS
static void Init_State_Func()
{
    static uint8_t Init_Call_Count = 0;

    uint8_t LOC_FunctionSetCommand  = LCD_4_PIN_COMMAND_1_ST_FUNC_SET;
    uint8_t LOC_DisplayOnOffCommand = LCD_4_PIN_COMMAND_ONOFF;
    uint8_t LOC_DisplayClearCommand = LCD_4_PIN_COMMAND_CLEAR;
    uint8_t LOC_EntryModeCommand    = LCD_4_PIN_COMMAND_ENTRY;

    //  if (Init_Call_Count < 4)
    //  {
    //      /*first call of the function set command*/
    //      LCD_WriteToPins(0x03, WRITE_COMMAND_STATE);
    //      Init_Call_Count++;
    //  }
    //
    //  else if (Init_Call_Count < 6)
    //  {
    //      /*first call of the function set command*/
    //      Init_Call_Count++;
    //  }
    //
    //  else if (Init_Call_Count < 10)
    //  {
    //      /*first call of the function set command*/
    //      LCD_WriteToPins(0x33, WRITE_COMMAND_STATE);
    //      Init_Call_Count++;
    //  }
    //
    /********************************** call the function set command **********************************/
    if (Init_Call_Count < 4)
    {
        /*first call of the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }

    else if (Init_Call_Count < 8)
    {
        /*set the font and number of lines configuration to the command*/
        LOC_FunctionSetCommand = LCD_4_PIN_COMMAND_2_ND_FUNC_SET;
        LOC_FunctionSetCommand |= (__FONT_SIZE << LCD_2_BIT_OFFSET);
        LOC_FunctionSetCommand |= (__NUM_OF_LINES << LCD_3_BIT_OFFSET);
        /*second call of the function set command*/
        LCD_WriteToPins(LOC_FunctionSetCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }
    /********************************** call the display on/off command **********************************/
    else if (Init_Call_Count < 12)
    {
        /*set the blink, cursor and display state configuration to the command*/
        LOC_DisplayOnOffCommand |= (BLINK_STATE);
        LOC_DisplayOnOffCommand |= (CURSOR_STATE << LCD_1_BIT_OFFSET);
        LOC_DisplayOnOffCommand |= (DISPLAY_STATE << LCD_2_BIT_OFFSET);

        /*setting the DB7 bit to high*/
        LOC_DisplayOnOffCommand |= (1 << LCD_3_BIT_OFFSET);

        /*call of the display on/off command*/
        LCD_WriteToPins(LOC_DisplayOnOffCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }
    /********************************** call the display clear command **********************************/
    else if (Init_Call_Count < 16)
    {
        /*call of the display clear command*/
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }
    /********************************** call the entry mode set command **********************************/
    else if (Init_Call_Count < 20)
    {
        /*set the font and number of lines configuration to the command*/
        LOC_EntryModeCommand |= (SHIFT_STATE);
        LOC_EntryModeCommand |= (INCREMENT_STATE << LCD_1_BIT_OFFSET);

        /*call of the entry mode set command*/
        LCD_WriteToPins(LOC_EntryModeCommand, WRITE_COMMAND_STATE);
        Init_Call_Count++;
    }
    else
    {
        LCD_State = LCD_STATE_OPER;
        Init_Call_Count = 0;
    }
}
#endif

#if __NUM_OF_DATA_LINES == DATA_8_PINS
static void LCD_WriteToPins(uint8_t Info, uint8_t State)
{
    static uint8_t Command_State = STATIC_STATE_READY;
    uint8_t Index;

    switch (Command_State)
    {
    case STATIC_STATE_READY:
        GPIO_SetPinState(LCDs_PinCfg.R_W_pin.port, LCDs_PinCfg.R_W_pin.pin, GPIO_STATE_RESET);
        GPIO_SetPinState(LCDs_PinCfg.R_S_pin.port, LCDs_PinCfg.R_S_pin.pin, State);
        for (Index = 0; Index < 8; Index++)
        {
            GPIO_SetPinState(LCDs_PinCfg.LCD_data_pins[Index].port,
                             LCDs_PinCfg.LCD_data_pins[Index].pin,
                             ((Info >> Index) & 1));
        }
        GPIO_SetPinState(LCDs_PinCfg.E_pin.port, LCDs_PinCfg.E_pin.pin, GPIO_STATE_SET);
        Command_State = STATIC_STATE_BUSY;
        break;

    case STATIC_STATE_BUSY:
        GPIO_SetPinState(LCDs_PinCfg.E_pin.port, LCDs_PinCfg.E_pin.pin, GPIO_STATE_RESET);
        Command_State = STATIC_STATE_READY;

        break;

    default:
        break;
    }
}
#endif

#if __NUM_OF_DATA_LINES == DATA_4_PINS
static void LCD_WriteToPins(uint8_t Info, uint8_t State)
{
    static uint8_t Command_State = STATIC_STATE_READY;
    static uint8_t Command_Nibble = COMMAND_NIBBLE_HIGH;
    uint8_t Index;

    switch (Command_Nibble)
    {
    case COMMAND_NIBBLE_HIGH:
        switch (Command_State)
        {
        case STATIC_STATE_READY:
            GPIO_SetPinState(LCDs_PinCfg.R_W_pin.port, LCDs_PinCfg.R_W_pin.pin, GPIO_PINSTATE_LOW);
            GPIO_SetPinState(LCDs_PinCfg.R_S_pin.port, LCDs_PinCfg.R_S_pin.pin, State);
            for (Index = 0; Index < 4; Index++)
            {
                GPIO_SetPinState(LCDs_PinCfg.LCD_data_pins[Index].port,
                                 LCDs_PinCfg.LCD_data_pins[Index].pin,
                                 ((Info >> (Index + 4)) & 1));
            }
            GPIO_SetPinState(LCDs_PinCfg.E_pin.port, LCDs_PinCfg.E_pin.pin, GPIO_PINSTATE_HIGH);
            Command_State = STATIC_STATE_BUSY;
            break;

        case STATIC_STATE_BUSY:
            GPIO_SetPinState(LCDs_PinCfg.E_pin.port, LCDs_PinCfg.E_pin.pin, GPIO_PINSTATE_LOW);
            Command_State = STATIC_STATE_READY;
            Command_Nibble = COMMAND_NIBBLE_LOW;
            break;

        default:
            break;
        }
        break;

    case COMMAND_NIBBLE_LOW:
        switch (Command_State)
        {
        case STATIC_STATE_READY:
            GPIO_SetPinState(LCDs_PinCfg.R_W_pin.port, LCDs_PinCfg.R_W_pin.pin, GPIO_PINSTATE_LOW);
            GPIO_SetPinState(LCDs_PinCfg.R_S_pin.port, LCDs_PinCfg.R_S_pin.pin, State);
            for (Index = 0; Index < 4; Index++)
            {
                GPIO_SetPinState(LCDs_PinCfg.LCD_data_pins[Index].port,
                                 LCDs_PinCfg.LCD_data_pins[Index].pin,
                                 ((Info >> Index) & 1));
            }
            GPIO_SetPinState(LCDs_PinCfg.E_pin.port, LCDs_PinCfg.E_pin.pin, GPIO_PINSTATE_HIGH);
            Command_State = STATIC_STATE_BUSY;
            break;

        case STATIC_STATE_BUSY:
            GPIO_SetPinState(LCDs_PinCfg.E_pin.port, LCDs_PinCfg.E_pin.pin, GPIO_PINSTATE_LOW);
            Command_State = STATIC_STATE_READY;
            Command_Nibble = COMMAND_NIBBLE_HIGH;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }
}
#endif

static void OperationState_WriteFunc()
{

    static uint8_t write_state = STATIC_STATE_READY;
#if __NUM_OF_DATA_LINES == DATA_4_PINS
    static uint8_t CommandState = 0;
#endif
#if __NUM_OF_DATA_LINES == DATA_8_PINS

    if (Write_Request.Curr_Pos < User_Resquest[User_CurrentRequest].U_length)
    {
        switch (write_state)
        {
        case STATIC_STATE_READY:
            LCD_WriteToPins(*(User_Resquest[User_CurrentRequest].U_string + Write_Request.Curr_Pos), WRITE_DATA_STATE);
            write_state = STATIC_STATE_BUSY;
            break;
        case STATIC_STATE_BUSY:
            LCD_WriteToPins(*(User_Resquest[User_CurrentRequest].U_string + Write_Request.Curr_Pos), WRITE_DATA_STATE);
            write_state = STATIC_STATE_READY;
            Write_Request.Curr_Pos++;
            break;

        default:
            break;
        }
    }
#endif
#if __NUM_OF_DATA_LINES == DATA_4_PINS
    if (Write_Request.Curr_Pos < User_Resquest[User_CurrentRequest].U_length)
    {
        switch (write_state)
        {
        case STATIC_STATE_READY:
            LCD_WriteToPins(*(User_Resquest[User_CurrentRequest].U_string + Write_Request.Curr_Pos), WRITE_DATA_STATE);
            if (CommandState)
            {
                write_state = STATIC_STATE_BUSY;
                CommandState = 0;
            }
            else
            {
                CommandState++;
            }
            break;
        case STATIC_STATE_BUSY:
            LCD_WriteToPins(*(User_Resquest[User_CurrentRequest].U_string + Write_Request.Curr_Pos), WRITE_DATA_STATE);
            if (CommandState)
            {
                write_state = STATIC_STATE_READY;
                Write_Request.Curr_Pos++;
                CommandState = 0;
            }
            else
            {
                CommandState++;
            }
            break;

        default:
            break;
        }
    }
#endif
    else
    {
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;
        Write_Request.Curr_Pos = 0;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        LCD_DigitCounter++;

        /*check if the current position is the end of the first line*/
        if (LCD_DigitCounter == __NUM_OF_DIGITS)
        {
            /*go to the beginning of the second line*/
            LCD_SetCursorAsync(1, 0, NULL);
        }
        /*check if the current position is the end of the second line*/
        else if (LCD_DigitCounter == __NUM_OF_DIGITS * 2)
        {
            /*go to the beginning of the first line*/
            LCD_SetCursorAsync(0, 0, NULL);
        }
        else
        {
            /*do nothing*/
        }

        if (Write_Request.CallBack)
        {
            Write_Request.CallBack();
        }
    }
}

static void OperationState_ClearFunc()
{
    static uint8_t Clear_state = STATIC_STATE_READY;
    uint8_t LOC_DisplayClearCommand = LCD_8_PIN_COMMAND_CLEAR;
#if __NUM_OF_DATA_LINES == DATA_4_PINS
    static uint8_t CommandState = 0;
#endif
#if __NUM_OF_DATA_LINES == DATA_8_PINS

    switch (Clear_state)
    {
    case STATIC_STATE_READY:
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Clear_state = STATIC_STATE_BUSY;
        break;
    case STATIC_STATE_BUSY:
        LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
        Clear_state = STATIC_STATE_READY;
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        LCD_SetCursorAsync(0, 0, NULL);

        if (Clear_Request.CallBack)
        {
            Clear_Request.CallBack();
        }
        break;

    default:
        break;
    }
#endif
#if __NUM_OF_DATA_LINES == DATA_4_PINS

    switch (Clear_state)
    {
    case STATIC_STATE_READY:
        if (CommandState)
        {
            LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
            Clear_state = STATIC_STATE_BUSY;
            CommandState = 0;
        }
        else
        {
            CommandState++;
        }
        break;
    case STATIC_STATE_BUSY:
        if (CommandState)
        {
            LCD_WriteToPins(LOC_DisplayClearCommand, WRITE_COMMAND_STATE);
            Clear_state = STATIC_STATE_READY;
            User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

            User_CurrentRequest++;
            if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
            {
                User_CurrentRequest = 0;
            }
            CommandState = 0;

            LCD_SetCursorAsync(0, 0, NULL);

            if (Clear_Request.CallBack)
            {
                Clear_Request.CallBack();
            }
        }
        else
        {
            CommandState++;
        }
        break;

    default:
        break;
    }
#endif
}

static void OperationState_SetPFunc()
{
    static uint8_t SetPState = STATIC_STATE_READY;
    uint8_t LOC_Location = LCD_COMMAND_GOTOXY;
#if __NUM_OF_DATA_LINES == DATA_4_PINS
    static uint8_t CommandState = 0;
#endif
#if __NUM_OF_DATA_LINES == DATA_8_PINS

    /*check if the location is in the first line*/
    if (SetP_Request.X_Position == 0)
    {
        LOC_Location += SetP_Request.Y_Position;
    }
    else
    {
        LOC_Location += (SetP_Request.Y_Position + LCD_SECOND_LINE_OFFSET);
    }

    switch (SetPState)
    {
    case STATIC_STATE_READY:
        LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
        SetPState = STATIC_STATE_BUSY;
        break;
    case STATIC_STATE_BUSY:
        LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
        SetP_Request.X_Position = 0;
        SetP_Request.Y_Position = 0;
        SetPState = STATIC_STATE_READY;
        User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

        User_CurrentRequest++;
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
        {
            User_CurrentRequest = 0;
        }

        if (SetP_Request.CallBack)
        {
            SetP_Request.CallBack();
        }
        break;

    default:
        break;
    }
#endif

#if __NUM_OF_DATA_LINES == DATA_4_PINS

    /*check if the location is in the first line*/
    if (SetP_Request.X_Position == 0)
    {
        LOC_Location += SetP_Request.Y_Position;
    }
    else
    {
        LOC_Location += (SetP_Request.Y_Position + LCD_SECOND_LINE_OFFSET);
    }

    switch (SetPState)
    {
    case STATIC_STATE_READY:
        if (CommandState)
        {
            LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
            SetPState = STATIC_STATE_BUSY;
            CommandState = 0;
        }
        else
        {
            CommandState++;
        }
        break;
    case STATIC_STATE_BUSY:
        if (CommandState)
        {
            LCD_WriteToPins(LOC_Location, WRITE_COMMAND_STATE);
            SetP_Request.X_Position = 0;
            SetP_Request.Y_Position = 0;
            SetPState = STATIC_STATE_READY;
            User_Resquest[User_CurrentRequest].U_State = LCD_USER_STATE_READY;

            User_CurrentRequest++;
            if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_READY || User_CurrentRequest == LCD_MAX_BUFFER_SIZE)
            {
                User_CurrentRequest = 0;
            }
            CommandState = 0;

            if (SetP_Request.CallBack)
            {
                SetP_Request.CallBack();
            }
        }
        else
        {
            CommandState++;
        }
        break;

    default:
        break;
    }

#endif
    LCD_DigitCounter = (SetP_Request.X_Position * 16) + SetP_Request.Y_Position;
}

HALStatus_t LCD_GetStatus(uint8_t *Status)
{
    HALStatus_t LOC_Status = HAL_NOK;

    if (Status == NULL)
    {
        LOC_Status = HAL_NULL_PTR;
    }
    else
    {
        *Status = LCD_State;
    }
    return LOC_Status;
}

void LCD_Runnable(void)
{
    switch (LCD_State)
    {
    case LCD_STATE_INIT:
        Init_State_Func();
        break;
    case LCD_STATE_OPER:
        if (User_Resquest[User_CurrentRequest].U_State == LCD_USER_STATE_BUSY)
        {
            switch (User_Resquest[User_CurrentRequest].U_Type)
            {
            case LCD_REQ_TYPE_WRITE:
                OperationState_WriteFunc();
                break;
            case LCD_REQ_TYPE_CLEAR:
                OperationState_ClearFunc();
                break;
            case LCD_REQ_TYPE_SET_P:
                OperationState_SetPFunc();
                break;

            default:
                break;
            }
        }
        break;
    default:
        break;
    }
}