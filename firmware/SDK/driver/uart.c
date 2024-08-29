#define  _UART_C

#include "uart.h"
#include "pxi_par2860_ble_lib.h"

static  UART_CTRL_TYPE* _uart_0_ctrl = ((UART_CTRL_TYPE*) UART_0_CTRL_BASE);
static  UART_CTRL_TYPE* _uart_1_ctrl = ((UART_CTRL_TYPE*) UART_1_CTRL_BASE);
static uart_irq_cb_type uart_cb[2];
void uart_0_init(uint8_t flowctrl, uint8_t baud)
{
    _uart_0_ctrl->CLK_SEL = 0;       /* 1=32M, 0=16M */

    _uart_0_ctrl->BAUDSEL = baud;
    _uart_0_ctrl->FLOW_EN = UART_RTS_CTS_DISABLE;
    _uart_0_ctrl->RX_INT_MASK = 1;   /* 1=MASK */
    _uart_0_ctrl->TX_INT_MASK = 1;   /* 1=MASK */

    _uart_0_ctrl->PAR_FAIL_INT_MASK = 1; /* 1=MASK */
    _uart_0_ctrl->par_rx_en = 0;     /* Rx Parity check enable */
    _uart_0_ctrl->par_rx_even = 1;   /* 1=Even, 0=Odd */

    _uart_0_ctrl->par_tx_en = 0;     /* Tx Parity check enable */
    _uart_0_ctrl->par_tx_even = 1;   /* 1=Even, 0=Odd */

    _uart_0_ctrl->RI = 0;
    _uart_0_ctrl->TI = 0;
    _uart_0_ctrl->PAR_FAIL = 1;

    _uart_0_ctrl->RX_FLUSH = 1;      /* clr rx fifo and set RX_FIFO_EMPTY */

    _uart_0_ctrl->UART_EN = 1;

    NVIC_DisableIRQ(UART0_IRQn);
}

void uart_0_write(uint8_t* buf, uint8_t sz)
{
    uint8_t idx = 0;

    CS_IN();

    for (idx = 0; idx < sz ; idx++)
    {
        _uart_0_ctrl->TX_DATA = buf[idx];

        while (_uart_0_ctrl->TI == 0);

        _uart_0_ctrl->TI = 0;
    }

    CS_OUT();
}

uint8_t uart_0_read(uint8_t* buf)
{
    if (!_uart_0_ctrl->RX_FIFO_EMPTY)
    {
        *buf = _uart_0_ctrl->RX_DATA;

        return true;
    }
    else
        return false;
}

void uart_0_rx_flush()
{
    _uart_0_ctrl->RX_FLUSH = 1;
}

void uart_1_init(uint8_t flowctrl, uint8_t baud)
{

    _uart_1_ctrl->CLK_SEL = 0;       /* 1=32M, 0=16M */

    _uart_1_ctrl->BAUDSEL = baud;
    _uart_1_ctrl->FLOW_EN = UART_RTS_CTS_DISABLE;
    _uart_1_ctrl->RX_INT_MASK = 1;   /* 1=MASK */
    _uart_1_ctrl->TX_INT_MASK = 1;   /* 1=MASK */

    _uart_1_ctrl->PAR_FAIL_INT_MASK = 1; /* 1=MASK */
    _uart_1_ctrl->par_rx_en = 0;     /* Rx Parity check enable */
    _uart_1_ctrl->par_rx_even = 1;   /* 1=Even, 0=Odd */

    _uart_1_ctrl->par_tx_en = 0;     /* Tx Parity check enable */
    _uart_1_ctrl->par_tx_even = 1;   /* 1=Even, 0=Odd */

    _uart_1_ctrl->RI = 0;
    _uart_1_ctrl->TI = 0;
    _uart_1_ctrl->PAR_FAIL = 1;

    _uart_1_ctrl->RX_FLUSH = 1;      /* clr rx fifo and set RX_FIFO_EMPTY */

    _uart_1_ctrl->UART_EN = 1;

    NVIC_DisableIRQ(UART1_IRQn);
}

void uart_1_write(uint8_t* buf, uint8_t sz)
{
    uint8_t idx = 0;

    CS_IN();
    for (idx = 0; idx < sz ; idx++)
    {
        _uart_1_ctrl->TX_DATA = buf[idx];

        //*((uint8_t *)(0x50001014)) |= (0x04); //gpio 2 output 1

        while (_uart_1_ctrl->TI == 0);

        _uart_1_ctrl->TI = 0;

        //*((uint8_t *)(0x50001014)) &= ~(0x04);    //gpio 2 output 0
    }

    CS_OUT();
}

uint8_t uart_1_read(uint8_t* buf)
{
    if (!_uart_1_ctrl->RX_FIFO_EMPTY)
    {
        *buf = _uart_1_ctrl->RX_DATA;

        return true;
    }
    else
        return false;
}

void uart_1_rx_flush(void)
{
    _uart_1_ctrl->RX_FLUSH = 1;
}
void uart_read(uint8_t id, uint8_t* buf)
{

    if (id == 0)
        uart_0_read(buf);

    if (id == 1)
        uart_1_read(buf);

}

void uart_0_en_int(uart_int_type int_type, uart_irq_cb_type callback)
{
    uart_cb[0] = callback;
    if(int_type == UART_RX_INT)
    {
        _uart_0_ctrl->RX_INT_MASK = 0;   /* 1=MASK */
        _uart_0_ctrl->TX_INT_MASK = 1;   /* 1=MASK */
    }

    if(int_type == UART_TX_INT)
    {
        _uart_0_ctrl->RX_INT_MASK = 1;   /* 1=MASK */
        _uart_0_ctrl->TX_INT_MASK = 0;   /* 1=MASK */
    }

    if(int_type == UART_ALL_INT)
    {
        _uart_0_ctrl->RX_INT_MASK = 0;   /* 1=MASK */
        _uart_0_ctrl->TX_INT_MASK = 0;   /* 1=MASK */
    }

    NVIC_EnableIRQ(UART0_IRQn);
}

void UART0_IRQHandler(void)
{
    uint8_t data;
    uint8_t int_st = _uart_0_ctrl->INT_STATUS;
    uint8_t clear_int = (int_st ^ UART_RX_INT) | UART_TX_INT;

    if ((int_st & UART_RX_INT) == UART_RX_INT)
    {
        while(!_uart_0_ctrl->RX_FIFO_EMPTY)
        {
            data = _uart_0_ctrl->RX_DATA;
            if(uart_cb[0] != 0)
                uart_cb[0](UART_RX_INT, data);
        }
        _uart_0_ctrl->INT_STATUS = clear_int;
    }

    if ((int_st & UART_TX_INT) == UART_TX_INT)
    {

        data = _uart_0_ctrl->TX_DATA;

        if(uart_cb[0] != 0)
            uart_cb[0](UART_TX_INT, data);

        _uart_0_ctrl->INT_STATUS = clear_int;
    }


    if ((_uart_0_ctrl->PAR_FAIL) == 1)
    {
        _uart_0_ctrl->PAR_FAIL = 1;
    }
}

void uart_1_en_int(uart_int_type int_type, uart_irq_cb_type callback)
{
    uart_cb[1] = callback;
    if(int_type == UART_RX_INT)
    {
        _uart_1_ctrl->RX_INT_MASK = 0;   /* 1=MASK */
        _uart_1_ctrl->TX_INT_MASK = 1;   /* 1=MASK */
    }

    if(int_type == UART_TX_INT)
    {
        _uart_1_ctrl->RX_INT_MASK = 1;   /* 1=MASK */
        _uart_1_ctrl->TX_INT_MASK = 0;   /* 1=MASK */
    }

    if(int_type == UART_ALL_INT)
    {
        _uart_1_ctrl->RX_INT_MASK = 0;   /* 1=MASK */
        _uart_1_ctrl->TX_INT_MASK = 0;   /* 1=MASK */
    }

    NVIC_EnableIRQ(UART1_IRQn);
}

void UART1_IRQHandler(void)
{
    uint8_t data;
    uint8_t int_st = _uart_1_ctrl->INT_STATUS;
    uint8_t clear_int = (int_st ^ UART_RX_INT) | UART_TX_INT;

    if ((int_st & UART_RX_INT) == UART_RX_INT)
    {
        while(!_uart_1_ctrl->RX_FIFO_EMPTY)
        {
            data = _uart_1_ctrl->RX_DATA;
            if(uart_cb[1] != 0)
                uart_cb[1](UART_RX_INT, data);
        }
        _uart_1_ctrl->INT_STATUS = clear_int;
    }

    if ((int_st & UART_TX_INT) == UART_TX_INT)
    {

        data = _uart_1_ctrl->TX_DATA;

        if(uart_cb[1] != 0)
            uart_cb[1](UART_TX_INT, data);

        _uart_1_ctrl->INT_STATUS = clear_int;
    }


    if ((_uart_1_ctrl->PAR_FAIL) == 1)
    {
        _uart_1_ctrl->PAR_FAIL = 1;
    }
}
