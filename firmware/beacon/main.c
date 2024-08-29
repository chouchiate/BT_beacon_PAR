
#include "ARMCM0.h"

#include "debug.h"
#include "ble.h"
#include "gpio.h"

#include "timer.h"
#include "pxi_par2860_ble_lib.h"
#include "Beacon_profile.h"
#include "beacon.h"
#include "config.h"


#include <string.h>

#define UART0_RX_GPIO   15 //QFN 32 use 12 ,QFN 48 use 15
#define UART0_TX_GPIO 	16 //QFN 32 use 14 ,QFN 48 use 16

#define UART1_RX_GPIO   15
#define UART1_TX_GPIO 16
#define BTN_GPIO 9
#define PAYLOAD_SIZE 20

static uint32_t _wake_up_io_from_pd = 0;
static uint32_t _wake_up_io_from_sleep = 0;
static uint32_t _ble_evt = 0;
static uint32_t _sys_evt = 0;
static struct gap_wakeup_config _pw_cfg;
static uint32_t _ble_uart_send_cnt = 0;

#define REPORT_TIMER_ID 0
#define REPORT_TIMER_INTERVAL_MS 1000
#define REPORT_TIMER_INTERVAL_US (REPORT_TIMER_INTERVAL_MS*1000)
#define REPORT_TIMER (REPORT_TIMER_INTERVAL_US/31)

#define BLE_UART_SEND_TH_MS 5000
#define BLE_UART_SEND_TH    (BLE_UART_SEND_TH_MS/REPORT_TIMER_INTERVAL_MS)

static void _set_evt(uint8_t st, uint32_t evt)
{
    if (st == BLE_ST)
        _ble_evt |= evt;
    else if (st == SYS_ST)
        _sys_evt |= evt;
}

static uint8_t _check_evt(uint8_t st, uint32_t evt)
{
    if (st == BLE_ST)
    {
        if ((_ble_evt & evt) == evt)
            return 1;
        else
            return 0;
    }
    else
    {
        if ((_sys_evt & evt) == evt)
            return 1;
        else
            return 0;
    }
}

static void _clr_evt(uint8_t st, uint32_t evt)
{
    if (st == BLE_ST)
        _ble_evt &= (~evt);
    else if (st == SYS_ST)
        _sys_evt &= (~evt);
}
static void _gpio_callback()
{
    _set_evt(SYS_ST, SYS_GPIO_CALLBACK);
}
static void _gpio_task()
{
    DBGPRINTF(("_gpio_task\r\n"));

    if (gpi_get_int_pol_type(GPIO_9) == POL_FALLING_LOW)
    {
        DBGPRINTF(("POL_FALLING_LOW\r\n"));
        gpi_enable_int(GPIO_9, EDGE_TRIGGER, POL_RISING_HIGH, _gpio_callback);
        ble_disconnect();
    }
    else
    {
        DBGPRINTF(("POL_RISING_HIGH\r\n"));
        gpi_enable_int(GPIO_9, EDGE_TRIGGER, POL_FALLING_LOW, _gpio_callback);
    }
}

static void _timer_callback()
{
    _set_evt(SYS_ST, SYS_TIMER);
}

#ifndef CONFIG_32K_XTAL
static void _check_lpo_cali()
{
    CS_IN();
    pxi_sys_32k_lpo_calibration();
    DBGPRINTF(("lpo cal\r\n"));
    CS_OUT();
}
#endif

static void _timer_task()
{
    #ifndef CONFIG_32K_XTAL
        _check_lpo_cali();
    #endif
    
    DBGPRINTF(("_timer_task\r\n"));
}

static void _system_power_down()
{
    __disable_irq();

    DBGPRINTF((">>>_system_power_down\r\n"));

    _pw_cfg.wakeup_type = SLEEP_WAKEUP;
    _pw_cfg.timer_wakeup_en = 0;
    _pw_cfg.gpi_wakeup_en = 1;
    _pw_cfg.gpi_wakeup_cfg = _wake_up_io_from_sleep; // motion;
    pxi_pmu_wakeup_config(&_pw_cfg);

    pxi_system_deep_sleep();

    DBGPRINTF(("<<<_system_power_down\r\n"));

    __enable_irq();
}

static void _sys_init()
{
#ifdef CONFIG_32K_XTAL
    #ifdef XTAL_32K_MODULE_0
        DBG("XTAL_32K_MODULE_0\r\n");
        pxi_sys_32k_xosc_init(SYSTEM_32K_XOSC_MODULE_0);
        pxi_sys_32k_clock_set(SYSTEM_32K_CLOCK_XOSC);
    #endif
    
    #ifdef XTAL_32K_MODULE_1
        DBG("XTAL_32K_MODULE_1\r\n");
        pxi_sys_32k_xosc_init(SYSTEM_32K_XOSC_MODULE_1);
        pxi_sys_32k_clock_set(SYSTEM_32K_CLOCK_XOSC);
    #endif
#else
    #ifdef CONFIG_32MXO_DIV
        pxi_sys_32k_clock_set(SYSTEM_32K_CLOCK_32MXO_DIV);    
    #endif
    #ifdef CONFIG_32K_LPO
        pxi_sys_32k_clock_set(SYSTEM_32K_CLOCK_RCOSC);
        pxi_sys_32k_lpo_calibration();
    #endif
#endif    
}

static void _mcu_init()
{
    pxi_sys_mcu_rc_calibration();
    pxi_sys_mcu_clock_set(SYSTEM_CLOCK_64M_RCOSC);
}

static void _gpio_init(void)
{
    uint8_t i = 0;
    // QFN 48
    for (i = 0; i < 40; i++)
    {
        switch (i)
        { 
            case 11:
            case 12:
            case 13:
            case 14:
            case 15:
            case 16:  
                    pad_input_configure(i, 1);
                    pad_pull_config(i, GPI_PULL_UP);
                break;   
            default:  
                break;
        }
    }

    for (i = 0; i <= 31; i++)
    {
        switch (i)
        {
            #ifdef CONFIG_32K_XTAL
                #ifdef XTAL_32K_MODULE_0
                    case 0:
                    case 1:
                    break;
                #endif
                #ifdef XTAL_32K_MODULE_1
                    case 5:
                    case 6:
                    break;
                #endif
            #endif
            default:
                    pad_mux_write(i, PIN_SEL_GPIO);
                    gpi_config(i, GPI_PULL_UP, GPI_INVERTED);
                    pad_input_configure(i, 0);
                break;
        }
    }
    
    //_wake_up_io_from_pd |= U32BIT(BTN_GPIO);

    //_wake_up_io_from_sleep |= U32BIT(BTN_GPIO);
}

static void _gpio_pd_mode(void)
{
    gpi_disable_int(BTN_GPIO);
    gpi_config(BTN_GPIO, GPI_PULL_UP, GPI_INVERTED);

    gpi_config(BTN_GPIO, GPI_PULL_UP, GPI_INVERTED);
    gpi_config(BTN_GPIO, GPI_PULL_UP, GPI_INVERTED);
}

static void _ble_evt_callback(uint8_t evt)
{
    _set_evt(BLE_ST, evt);
}

static void _ble_evt_task(void)
{
    if (_check_evt(BLE_ST, BLE_ADV_END))
    {
        _clr_evt(BLE_ST, BLE_ADV_END);

        DBGPRINTF(("HID - BLE_ADV_END\r\n"));

        ble_adv_start(0);

        //_set_evt(SYS_ST, SYS_POWER_DOWN);
    }

    if (_check_evt(BLE_ST, BLE_CONNECTED))
    {
        _clr_evt(BLE_ST, BLE_CONNECTED);

        DBGPRINTF(("HID - BLE_CONNECTED\r\n"));
    }

    if (_check_evt(BLE_ST, BLE_DISCONNECTED))
    {
        _clr_evt(BLE_ST, BLE_DISCONNECTED);

        DBGPRINTF(("HID - BLE_DISCONNECTED\r\n"));

        if (_check_evt(SYS_ST, SYS_PAIRING))
        {
            DBGPRINTF(("HID - SYS_PAIRING\r\n"));
            _clr_evt(SYS_ST, SYS_PAIRING);

            ble_adv_start(1);
        }
        else
            _set_evt(SYS_ST, SYS_POWER_DOWN);
    }

    if (_check_evt(BLE_ST, BLE_NOTIFY_ENABLE))
    {
        _clr_evt(BLE_ST, BLE_NOTIFY_ENABLE);

        DBGPRINTF(("HID - BLE_NOTIFY_ENABLE\r\n"));
    }

    if (_check_evt(BLE_ST, BLE_LATENCY_DISABLE))
        _clr_evt(BLE_ST, BLE_LATENCY_DISABLE);

    if (_check_evt(BLE_ST, BLE_START_ENC))
    {
        DBG("BLE_START_ENC\r\n");
        _clr_evt(BLE_ST, BLE_START_ENC);
    }
}

static void _sys_evt_task(void)
{
    if (_check_evt(SYS_ST, SYS_POWER_DOWN))
    {
        struct gap_bond_dev bonds_device;

        memset((uint8_t*)&bonds_device, 0, sizeof(bonds_device));

        _clr_evt(SYS_ST, SYS_POWER_DOWN);

        _gpio_pd_mode();

        timer_disable(REPORT_TIMER_ID);

        //pxi_32m_clock_enable(0);

        _system_power_down();

        ble_init(&_ble_evt_callback);

        _gpio_init();

        _mcu_init();

        _sys_init();

        _pw_cfg.wakeup_type = SLEEP_WAKEUP;
        _pw_cfg.timer_wakeup_en = 0;
        _pw_cfg.gpi_wakeup_en = 1;
        _pw_cfg.gpi_wakeup_cfg = _wake_up_io_from_pd; // motion;
        pxi_pmu_wakeup_config(&_pw_cfg);

        ble_adv_start(0);
    }

    if (_check_evt(SYS_ST, SYS_GPIO_CALLBACK))
    {
        _clr_evt(SYS_ST, SYS_GPIO_CALLBACK);
        _gpio_task();
    }
    if (_check_evt(SYS_ST, SYS_TIMER))
    {
        _clr_evt(SYS_ST, SYS_TIMER);
        _timer_task();
    }
}

void test_cb(void)
{
    DBG("test_cb\r\n");
}

uint8_t test_p_cmd[20];
struct hci_cmd* pcmd = (struct hci_cmd*)test_p_cmd;

int main(void)
{
    __disable_irq();

    _gpio_init();
    
#ifdef CONFIG_DEBUG_MSG
    pxi_32m_clock_enable(1);

    if(pxi_get_qfn_type() == QFN_48)
    {
        pxi_32m_clock_enable(true);
        pad_mux_write(15,PIN_SEL_UART_RXD0);
        pad_mux_write(16,PIN_SEL_UART_TXD0);
        dbg_init();
    }
    else
    {
        pxi_32m_clock_enable(true);
        pad_mux_write(12,PIN_SEL_UART_RXD0);
        pad_mux_write(14,PIN_SEL_UART_TXD0);
        dbg_init();
    }

#endif
    
    ble_init(&_ble_evt_callback);

    _mcu_init();

    _sys_init();

#ifdef CONFIG_32K_LPO
    timer_enable(REPORT_TIMER_ID, _timer_callback, 0x5A0000,1); // 3 minutes
#endif

    ble_adv_start(0);

    DBGPRINTF(("par 2860 Beacon Example C\r\n"));
    DBGPRINTF(("%s\r\n", pxi_get_lib_version()));

    __enable_irq();

    _pw_cfg.wakeup_type = SLEEP_WAKEUP;
    _pw_cfg.timer_wakeup_en = 0;
    _pw_cfg.gpi_wakeup_en = 1;
    _pw_cfg.gpi_wakeup_cfg = _wake_up_io_from_sleep; // motion;
    pxi_pmu_wakeup_config(&_pw_cfg);

    while (1)
    {
        _sys_evt_task();

        _ble_evt_task();

        if ((_sys_evt == 0) && (_ble_evt == 0))
        {
            pxi_ble_sched_execute();

            pxi_system_sleep();
        }
    }
}

void HardFault_Handler(void)
{
    DBG("HardFault_Handler\r\n");
    while(1);
}
