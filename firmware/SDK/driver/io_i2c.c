#include "gpio.h"
#include "io_i2c.h"
#include "debug.h"
#include <string.h>
#include "pxi_par2860_ble_lib.h"

static uint32_t i2c_sclk = 0;
static uint32_t i2c_sda = 0;

static void check_i2c_clk()
{
    uint8_t i=0;
    gpi_config(i2c_sclk, GPI_NO_PULL, GPI_NOT_INVERTED);

    while(1)
    {
        if(gpi_get_val(i2c_sclk))
            i++;
        else
            i=0;
        
        if(i==2)
            break;
    }
}

void io_i2c_config(uint32_t _i2c_sclk, uint32_t _i2c_sda)
{
	i2c_sclk = _i2c_sclk;
    i2c_sda = _i2c_sda;

	pad_mux_write(i2c_sclk, PIN_SEL_GPIO);
	pad_mux_write(i2c_sda, PIN_SEL_GPIO);
	
	gpi_config(i2c_sclk, GPI_NO_PULL, GPI_NOT_INVERTED);
	gpo_config(i2c_sda, GPO_HIGH);   
}

static void io_i2c_start()
{
	gpo_config(i2c_sda, GPO_HIGH);
	check_i2c_clk();
	gpo_config(i2c_sda, GPO_LOW);
	gpo_config(i2c_sclk, GPO_LOW);
}

static void io_i2c_stop()
{
	gpo_config(i2c_sda, GPO_LOW);
	check_i2c_clk();
	gpo_config(i2c_sda, GPO_HIGH);
}

static uint8_t io_i2c_get_sack()		// get the ack from the slave	
{
	uint8_t ack;
	gpo_config(i2c_sclk, GPO_LOW);   
    gpi_config(i2c_sda, GPI_PULL_UP, GPI_NOT_INVERTED);
    check_i2c_clk();
	ack = gpi_get_val(i2c_sda);
	gpo_config(i2c_sclk, GPO_LOW);
	
	return ack^0x01;		// return : 1 for Ack, 0 for Nack
}

static uint8_t iic_send_id(uint8_t id, i2c_direction_t dir)
{
	uint8_t j;
	id <<= 1;
	for(j = 0; j < 7; j++)
	{
        gpo_config(i2c_sclk, GPO_LOW);
		if((id & 0x80) == 0x80)
		{		
            gpo_config(i2c_sda, GPO_HIGH);
		}
		else
		{
            gpo_config(i2c_sda, GPO_LOW);
		}
		id = id << 1;
        check_i2c_clk();
	}
	gpo_config(i2c_sclk, GPO_LOW); 
    
	if(dir == I2C_WRITE)  
        gpo_config(i2c_sda, GPO_LOW);
	else 
        gpo_config(i2c_sda, GPO_HIGH);
	
    check_i2c_clk();

	return io_i2c_get_sack();
}

static uint8_t iic_send_data(uint8_t da)
{
	uint8_t m;
	uint8_t j;
	for(j = 0; j < 8; j++)
	{
		m = da;
		gpo_config(i2c_sclk, GPO_LOW); 
		m = m & 0x80;
		if(m == 0x80)
		{		
			gpo_config(i2c_sda, GPO_HIGH);
		}
		else
		{
			gpo_config(i2c_sda, GPO_LOW);
		}
		da = da << 1;
        check_i2c_clk();
	}

	return io_i2c_get_sack();   
}

static uint8_t iic_receive_data(uint8_t mack)
{
	uint8_t j,da;
    gpi_config(i2c_sda, GPI_PULL_UP, GPI_NOT_INVERTED);
    
	for(j = 0; j < 8; j++)
	{
        gpo_config(i2c_sclk, GPO_LOW); 
		check_i2c_clk();
		if(gpi_get_val(i2c_sda)) 
            da = (da << 1) + 1;
		else 
            da = da << 1;
	}

    gpo_config(i2c_sclk, GPO_LOW);  
    gpo_config(i2c_sda, GPO_HIGH);  
    
	if(mack)
        gpo_config(i2c_sda, GPO_LOW);
	else 
        gpo_config(i2c_sda, GPO_HIGH); 
     
    check_i2c_clk();
    gpo_config(i2c_sclk, GPO_LOW);
    
	return da;
}

uint8_t io_i2c_reg_write(uint8_t id, io_i2c_byte_address_t addr_len, uint16_t addr, uint8_t * buf, uint16_t sz)
{
	uint8_t i = 0;
	uint8_t ack = 0;
	
	CS_IN();
	io_i2c_start();
	
	ack = iic_send_id(id, I2C_WRITE);
	if(ack)
	{	
		if(addr_len == IO_I2C_2_BYTE_ADDRESS)
			ack = iic_send_data(addr >> 8);
		
		if(ack)
		{
            if(addr_len <= IO_I2C_1_BYTE_ADDRESS)
                ack = iic_send_data(addr);
            
			if(ack)
			{
				for(i = 0; i < sz; i++)
				{
					ack = iic_send_data(*buf);
					if(ack)
						buf++;
					else 
						break;
				}
			}
		}
	}
	
	io_i2c_stop();
	CS_OUT();
	return ack; 
}

uint8_t io_i2c_reg_read(uint8_t id, io_i2c_byte_address_t addr_len, uint16_t addr, uint8_t * buf, uint16_t sz, uint8_t stop_en)
{
    uint8_t i = 0;
	uint8_t ack = 0;
	
	CS_IN();
	io_i2c_start();
	
	ack = iic_send_id(id, I2C_WRITE);
	if(ack)
	{
		if(addr_len == IO_I2C_2_BYTE_ADDRESS)
			ack = iic_send_data(addr >> 8);
		
		if(ack)
		{
            if(addr_len <= IO_I2C_1_BYTE_ADDRESS)
                ack = iic_send_data(addr);
            
			if(ack)
			{
				io_i2c_start(); 
				
				ack = iic_send_id(id, I2C_READ);
				if(ack)
				{
					for(i = 0; i < sz; i++)
					{
						if((sz > 1) && (i<(sz - 1))) 
							*buf = iic_receive_data(1);
						else 
							*buf = iic_receive_data(0);
						buf++;
					}
				}
			}
		}
    }
	
	if(stop_en)
		io_i2c_stop();
	CS_OUT();
	return ack;
}

