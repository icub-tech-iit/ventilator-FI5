/* mock */

int last_delay = 0;

void mdelay(int ms)
{
	last_delay = +ms;
}

void delay_zero(void)
{
	last_delay = 0;
}

int delay_get_last(void)
{
	return last_delay;
}
