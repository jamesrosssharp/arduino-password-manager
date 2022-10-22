
#pragma once

#include <stdio.h>

void	uart_initialize(void);
int	uart_send_byte(char byte, FILE *stream); 
